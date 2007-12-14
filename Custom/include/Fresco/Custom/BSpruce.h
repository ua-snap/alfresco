#ifndef Fresco_BSpruce_h
#define Fresco_BSpruce_h


#include "Fresco/Custom/Custom.h"
#include "Fresco/Foundation/Frame.h"
#include "Fresco/Foundation/NoVeg.h"


class FrescoCustom_API BSpruce : public Frame 
//The BSpruce class is the definition of the black spruce frame.  It is a child of the general frame type and inherits
//most of its functionality from there.  It provides instances of the pure virtual functions getFireProb, and success.  In 
//addition to the base class functionality, it records the date that the stand switched to spruce.  This is not provided
//for in the base class, because in spruce, this transition might not be caused by fire.  It also overloads the queryReply
//function. This is used when a frame prompts its neightbours for weather they can be used as a spruce seed source.
{
//Data
private:
	static bool				_isStaticSetupAlready;
	static double*			_pIntegral;
    static float			_decidFireProb;
	static double*	        _pBSpruceFireParms;
	static double			_tundraSpruceBasalArea;
	static const double*	_pStartAgeParms;
	static EStartAgeType	_startAgeType;

//Functions
public:
							BSpruce(
                                const int& rAge=0, 
								const bool& rIsTopoComplex=false, 
                                const float& rSite=0.0f, 
                                const int& rYearOfLastBurn=-1, 
								const int& rLastBurnSeverity=0,
                                const double& rFireIgnitionFactor=0.0, 
                                const double& rFire_Sensitivity=0.0, 
                                const Species& rSpeciesSubCanopy=gNoVegID);
							BSpruce(const Frame& rFrame, int& rAge=gYear);
							~BSpruce();
	void					_BSpruce();
    static void             setStaticData();
	static void				clear();
	Frame*					success(Landscape *Parent);
	const int				type() const;
	const float				getHumanIgnitionProb() const;
private:
	float			        getFireProb(const Landscape* pLandscape);
	double					queryReply(Landscape* pParent, const double weight, const double* pParams=NULL);
};


//This function creates an instance of the pure virtual function getFireProb for the black spruce frame.  It simply
//returns the fire probability for this frame based on the basal fire probability computed by the worker function
//ClimateFireProb concatenated with the species factor computed from the specific information in this frame.
inline float BSpruce::getFireProb(const Landscape* pLandscape) 
{
//OLD TODO	return getClimateFireProb(Clim) * (Logistic (gYear - _yearEstablished, _pBSpruceFireParms) + _decidFireProb);
	return getClimateFireProb(pLandscape) * (_pBSpruceFireParms[0] + _decidFireProb);
}

inline const int            BSpruce::type() const
{
    return gBSpruceID; 
}

inline const float			BSpruce::getHumanIgnitionProb() const 
{ 
    return _humanIgnitionsProb; 
}

inline double				BSpruce::queryReply(Landscape* pParent, const double weight, const double* pParams) 
//Return the weighted seed source from this cell
{ 
    return 2.0*_tundraSpruceBasalArea*weight; 
}	


#endif
