#ifndef Fresco_Grassland_h
#define Fresco_Grassland_h

#include "Custom.h"
#include "Frame.h"
#include "Fresco.h"
#include "PreCompiled.h"
#include "NoVeg.h"

class FrescoCustom_API Grassland : public Frame 
//The Grassland class is the definition of the white spruce frame.  It is a child of the general frame type and inherits
//most of its functionality from there.  It provides instances of the pure virtual functions getFireProb, and success.  In 
//addition to the base class functionality, it records the date that the stand switched to spruce.  This is not provided
//for in the base class, because in spruce, this transition might not be caused by fire.  It also overloads the queryReply
//function. This is used when a frame prompts its neighbors for whether they can be used as a spruce seed source.
{
//Data
private:
	static bool				_isStaticSetupAlready;
	static double*			_pIntegral;
	static float			_decidFireProb;
	static float			_ignitionDepressor;		//Dampens the ignition factor for any cells of type Grassland.
	static double			_tundraSpruceBasalArea;
	static const double*	_pStartAgeParms;
	static EStartAgeType	_startAgeType;
	static double*			_pGrasslandFireParms;

//Functions
public:
							Grassland(
                                const int& rAge=0, 
								const bool& rIsTopoComplex=false, 
                                const float& rSite=0.f, 
                                const int& rYearOfLastBurn=-1, 
								const int& rLastBurnSeverity=0, 
                                const double& rFireIgnitionFactor=0, 
                                const double& rFireSensitivity=0, 
                                const Species& rSpeciesSubCanopy=gNoVegID);
							Grassland(const Frame& rFrame, int& rAge=gYear);
							~Grassland();
    static void             setStaticData();
	static void				clear();
	Frame*					success(Landscape* pParent);
	const unsigned char		type() const;
	const float				getHumanIgnitionProb() const {return _humanIgnitionsProb; }
	const float				getIgnitionDepressor();
    static double           getFireParam(const int index);
private:
	void					_Grassland();
	float			        getFireProb(const Landscape* pLandscape);
	double					queryReply(Landscape* pParent, const double weight, const double* pParams = NULL);
};


inline float                Grassland::getFireProb(const Landscape* pLandscape) 
//This function creates an instance of the pure virtual function getFireProb for the white spruce frame.  It simply
//returns the fire probability for this frame based on the basal fire probability computed by the worker function
//ClimateFireProb concatenated with the species factor computed from the specific information in this frame.
{
	return getClimateFireProb(pLandscape) * (_pGrasslandFireParms[0] + _decidFireProb);
}

inline const float			Grassland::getIgnitionDepressor()
{
	return _ignitionDepressor;
}

inline double               Grassland::getFireParam(const int index)
{
    return _pGrasslandFireParms[index];
}

inline const unsigned char	Grassland::type() const 
{ 
    return gGrasslandID; 
}

inline double				Grassland::queryReply(Landscape* pParent, const double weight, const double* pParams) 
//Return the weighted seed source from this cell
{ 
    return 2.0*_tundraSpruceBasalArea*weight;
}


#endif
