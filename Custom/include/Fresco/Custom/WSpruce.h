#ifndef Fresco_WSpruce_h
#define Fresco_WSpruce_h


#include "Fresco/Custom/Custom.h"
#include "Fresco/Foundation/Frame.h"
#include "Fresco/Foundation/NoVeg.h"


class FrescoCustom_API WSpruce : public Frame 
//The WSpruce class is the definition of the white spruce frame.  It is a child of the general frame type and inherits
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
	static double			_tundraSpruceBasalArea;
	static const double*	_pStartAgeParms;
	static EStartAgeType	_startAgeType;
	static double*			_pWSpruceFireParms;

//Functions
public:
							WSpruce(
                                const int& rAge=0, 
                                const float& rSite=0.f, 
                                const int& rLastBurn=-1, 
                                const double& rFireIgnitionFactor=0, 
                                const double& rFireSensitivity=0, 
                                const Species& rSpeciesSubCanopy=gNoVegID);
							WSpruce(const Frame& rFrame, int& rAge=gYear);
							~WSpruce();
    static void             setStaticData();
	static void				clear();
	Frame*					success(Landscape* pParent);
	const int				type() const;
	const float				getHumanIgnitionProb() const {return _humanIgnitionsProb; }
    static double           getFireParam(const int index);
private:
	void					_WSpruce();
	float			        getFireProb(const Landscape* pLandscape);
	double					queryReply(Landscape* pParent, const double weight, const double* pParams = NULL);
};


inline float                WSpruce::getFireProb(const Landscape* pLandscape) 
//This function creates an instance of the pure virtual function getFireProb for the white spruce frame.  It simply
//returns the fire probability for this frame based on the basal fire probability computed by the worker function
//ClimateFireProb concatenated with the species factor computed from the specific information in this frame.
{
	return getClimateFireProb(pLandscape) * (_pWSpruceFireParms[0] + _decidFireProb);
}


inline double               WSpruce::getFireParam(const int index)
{
    return _pWSpruceFireParms[index];
}

inline const int            WSpruce::type() const 
{ 
    return gWSpruceID; 
}

inline double				WSpruce::queryReply(Landscape* pParent, const double weight, const double* pParams) 
//Return the weighted seed source from this cell
{ 
    return 2.0*_tundraSpruceBasalArea*weight;
}


#endif
