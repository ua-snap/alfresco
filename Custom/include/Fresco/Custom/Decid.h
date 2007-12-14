#ifndef Fresco_Decid_h
#define Fresco_Decid_h


#include "Fresco/Custom/Custom.h"
#include "Fresco/Foundation/Frame.h"
#include "Fresco/Foundation/NoVeg.h"


class FrescoCustom_API Decid : public Frame 
// The Decid class is the definition of the deciduous frame.  It is a child of the general frame type and inherits most
// of its functionality from there.  It provides instances of the pure virtual functions getFireProb, and success.  In 
// addition to the base class functionality, it maintains the variable representing the stuntedness of the trees.  When
// this variable reaches a user defined level, there is a chance it will transition to spruce.  It also overloads the 
// virtual function queryReply because deciduous frames can contribute spruce seeds to neighbouring cells.
{
//Data
private:
	float					_degrees;						//Running sum of degree years used to determine succession.
	Species				    _speciesTrajectory;				//The long term trajectory of the current cell (black or white spruce)
	static bool				_isStaticSetupAlready;
	static float			_decidFireProb;
	static int				_decidHistory;
	static double			_tundraSpruceBasalArea;
	static const double*	_pDecidTundraParams;
	static double**			_pDecidToBSpruceParams;			//Seperate parameters per burn severity.
	static double**			_pDecidToWSpruceParams;
	static EStartAgeType	_bspruceStartAgeType;
	static EStartAgeType	_wspruceStartAgeType;
	static double*			_pBSpruceWeibullIntegral;
	static double*			_pWSpruceWeibullIntegral;
	static const double*	_pBSpruceStartAge;
	static const double*	_pWSpruceStartAge;

//Functions
public:
							Decid(
                                const int& rAge=0, 
								const bool& rIsTopoComplex=false, 
                                const double& rSite=0., 
                                const int& rYearOfLastBurn=-1, 
								const int& rLastBurnSeverity=0,
								const double& rFireIgnitionFactor=0, 
                                const double& rFireSensitivity=0, 
                                const Species& rSpecSubCanopy=gNoVegID);
							Decid(const Frame& Frame);
	virtual 				~Decid() {};
    static void             setStaticData();
	static void				clear();
	virtual void			repStart();
	Frame*					success(Landscape *Parent);
	const int				type() const;
	const float				getHumanIgnitionProb() const;
	void					writeData(std::ostream& s, const int outFlags, const int formatting=false) const;
private:
	void					_Decid();
	float			        getFireProb(const Landscape* pLandscape);
	double					queryReply(Landscape* parent, const double weight, const double* params=NULL);
	int						startAge(Species speciesTrajectory);			//Select a starting age given a species trajectory.
};


inline float                Decid::getFireProb(const Landscape* pLandscape) 
// Overrides Fire::getFireProb() for the deciduous frame.  Returns this frame's fire probability calculated 
// by multiplying this frame's climate and species fire probabilities.
{
	return getClimateFireProb(pLandscape) * _decidFireProb;
}

inline const float			Decid::getHumanIgnitionProb() const
{
    return _humanIgnitionsProb; 
}

inline const int            Decid::type () const
{ 
    return gDecidID; 
}

inline double				Decid::queryReply (Landscape* parent, const double weight, const double* params)
// Return the weighted seed source from this cell
{
    if (_speciesSubCanopy==gDecidID) 
        return 0.; 
    else 
        return _tundraSpruceBasalArea*weight; 
}


#endif
