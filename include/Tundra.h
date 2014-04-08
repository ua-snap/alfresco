#ifndef Fresco_Tundra_h
#define Fresco_Tundra_h


#include "Custom.h"
#include "Frame.h"
#include "NoVeg.h"
#include "WSpruce.h"


class FrescoCustom_API Tundra : public Frame 
//The Tundra class is the definition of the tundra frame.  It is a child of the general frame type and inherits most
//of its functionality from there.  It provides instances of the pure virtual functions getFireProb, and success.  In 
//addition to the base class functionality, it maintains the tree density in stand.  This density is effected by climate
//and can grow, or it can be decreased by insect infestation (a worker function in this class).
{
//Data
private:
	double					_basalArea;             //Can increase or decrease over time leading to succession to forest
	int						_yearOfEstablishment;	//Date of tree establishment
	float					_degrees;				//Number of degree days - used to kill young seedlings
	static bool				_isStaticSetupAlready;
	static bool				_isFireProbAgeDependent;
	static double*	_pAgeDependentFireParams;		
	static float			_fireProb;
	static float			_ignitionDepressor;		//Dampens the ignition factor for any cells of type Tundra.
	static double			_seedRange;
	static double*	_pSeedSource;
	static double			_seedBasalArea;
	static double			_seedlingBasalArea;
	static int				_history;
	static double*	_pSeedEstParams;
	static double			_meanGrowth;
	static double*	_pClimateGrowth;
	static double*	_pCalibrationFactor;
	static double			_seedling;
	static double			_ratioAK;
	static double			_tundraSpruceBasalArea;
	static double*	_pStartAgeParms;
	static EStartAgeType	_startAgeType;
	static double*			_pIntegral;

//Functions
public:
							Tundra(
                                const int& rAge=0, 
								const bool& rIsTopoComplex=false, 
                                const float& rSite=0., 
                                const int& rYearOfLastBurn=-1, 
								const int& rLastBurnSeverity=0,
                                const double& rFireIgnitionFactor=0, 
                                const double& rFireSensitivity=0,  
                                const Species& rSpecSubCanopy=0, 
                                const int treeDensity=-1);
							Tundra(const Frame& rFrame, const int treeDensity=-1);
	virtual 				~Tundra();
    static void             setStaticData();
	static void				clear();
	Frame*					success(Landscape* pParent);
	const unsigned char		type() const;
	const float				getHumanIgnitionProb() const;
	const float				getIgnitionDepressor();
	//template<class T> T		get(RasterIO::ALFMapType mapType);
	virtual float			getAsFloat(RasterIO::ALFMapType mapType);
private:
	void					_Tundra(const int treeDensity);
	float			        getFireProb(const Landscape* pLandscape);
	double					queryReply(Landscape* pParent, const double weight, const double* pParams=NULL);
	double					getInitialBasalArea();
};


inline float                Tundra::getFireProb(const Landscape* pLandscape) 
// Overrides Fire::getFireProb() for the deciduous frame.  Returns this frame's fire probability calculated 
// by multiplying this frame's climate and species fire probabilities.
{
	if (_isFireProbAgeDependent)
		_fireProb = Logistic(age(), _pAgeDependentFireParams);
	return getClimateFireProb(pLandscape) * ((WSpruce::getFireParam(0)-_fireProb) * _basalArea/_tundraSpruceBasalArea + _fireProb);
}

inline const float			Tundra::getIgnitionDepressor()
{
	return _ignitionDepressor;
}

inline const float			Tundra::getHumanIgnitionProb() const 
{
    return _humanIgnitionsProb; 
}

inline const unsigned char  Tundra::type () const 
{ 
    return gTundraID; 
}

inline double				Tundra::queryReply (Landscape* pParent, const double weight, const double* pParams) 
//Return the weighted seed source from this cell
{ 
	return _basalArea*weight; 
}		


#endif
