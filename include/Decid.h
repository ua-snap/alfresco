#ifndef Fresco_Decid_h
#define Fresco_Decid_h


#include "Custom.h"
#include "Frame.h"
#include "NoVeg.h"


//enum ALFMapType;


class FrescoCustom_API Decid : public Frame 
// The Decid class is the definition of the deciduous frame.  It is a child of the general frame type and inherits most
// of its functionality from there.  It provides instances of the pure virtual functions getFireProb, and success.  In 
// addition to the base class functionality, it maintains the variable representing the stuntedness of the trees.  When
// this variable reaches a user defined level, there is a chance it will transition to spruce.  It also overloads the 
// virtual function queryReply because deciduous frames can contribute spruce seeds to neighbouring cells.
{
//Data
private:
	enum EGrasslandThresholds {G_LOW, G_MODERATE, G_HIGH_LSS, G_HIGH_HSS, G_LOW_AND_WAS_GRASSLAND, G_MODERATE_AND_WAS_GRASSLAND};
	Species				    _speciesTrajectory;				//The long term trajectory of the current cell (black or white spruce)
	bool					_wasGrassland;
	float					_degreesForGrassland;			//Running sum of a climate based regression used to determine succession to Grassland.
	static int				_yearsOfGrasslandCheck;
	static double*	_pGrasslandThresholds;
	static double*	_pGrassClimateParams;
	static std::list<int>	_grassTempMonths;
	static std::list<int>	_grassPrecipMonths;
	static bool				_isStaticSetupAlready;
	static bool				_isFireProbAgeDependent;
	static double*	_pAgeDependentFireParams;		
	static float			_decidFireProb;
	static float			_ignitionDepressor;				//Dampens the ignition factor for any cells of type Decid.
	static int				_decidHistory;
	static double			_tundraSpruceBasalArea;
	static double*	_pDecidTundraParams;
	static double**			_pDecidToBSpruceParams;			//Seperate parameters per burn severity.
	static double**			_pDecidToWSpruceParams;
	static EStartAgeType	_bspruceStartAgeType;
	static EStartAgeType	_wspruceStartAgeType;
	static double*			_pBSpruceWeibullIntegral;
	static double*			_pWSpruceWeibullIntegral;
	static double*	_pBSpruceStartAge;
	static double*	_pWSpruceStartAge;

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
	const unsigned char		type() const;
	const float				getHumanIgnitionProb() const;
	const float				getIgnitionDepressor();
	//template<class T> T		get(RasterIO::ALFMapType mapType);
	virtual unsigned char	getAsByte(RasterIO::ALFMapType mapType);
	static const bool		usingGrassland();
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
	if (_isFireProbAgeDependent)
		_decidFireProb = Logistic(age(), _pAgeDependentFireParams);
	return getClimateFireProb(pLandscape) * _decidFireProb;
}

inline const float			Decid::getIgnitionDepressor()
{
	return _ignitionDepressor;
}

inline const float			Decid::getHumanIgnitionProb() const
{
    return _humanIgnitionsProb; 
}

inline const unsigned char            Decid::type () const
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

inline const bool			Decid::usingGrassland()
{
	return (gGrasslandID != 255);
}

#endif
