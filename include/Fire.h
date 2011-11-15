#ifndef Fresco_Fire_h
#define Fresco_Fire_h


#include "PreCompiled.h"


class Landscape;
struct SClimate;
//struct SFireTransition;




class FrescoFoundation_API Fire 
//Fire provides functionality for the fire probability function.  
{
//Data
public:
	enum EType				{FIXED, SPATIAL, HISTORICAL};
	enum ECause             {NATURAL, HUMAN};		
	enum EBurnSeverity		{NO_BURN=0, LOW=1, MODERATE=2, HIGH_LSS=3, HIGH_HSS=4};
    struct SFireTransition
    {
	    int				Year;
	    Fire::EType		Type;
	    float			Ignition;
	    float			Sensitivity;
	    std::string		SpatialIgnitionFile;
	    std::string		SpatialSensitivityFile;
	    std::string		HistoricalFile;
	    SFireTransition() : Year(0), Type(Fire::FIXED), Ignition(0), Sensitivity(0), SpatialIgnitionFile(""), SpatialSensitivityFile(""), HistoricalFile("") {};
    };
	struct SBurnSeveritySettings
	{
		float FxnIntercept;
		float FxnSlope;
		float LssVsHssWeight;
		float LowVsModerateWeight;
		float FlatTopoWeight;
		float ComplexTopoWeight;
		SBurnSeveritySettings() : FxnIntercept(0), FxnSlope(0), LssVsHssWeight(0), LowVsModerateWeight(0), FlatTopoWeight(0), ComplexTopoWeight(0) 	{};
	};
	ECause					lastBurnCause;										//Cause of the most recent fire.
	EBurnSeverity			burnSeverity;										//Severity of the most recent fire.
	bool					lastBurnWasOrigin;	
	int						yearOfLastBurn;										//The last year this cell burned - if it is the current year then it has burned this year
	int						fireScarID;											//The ID of the last burn.
    float					fireSensitivity;
	float					fireIgnitionFactor;

	static std::vector<SFireTransition>	fireTransitions;
	static SBurnSeveritySettings burnSeveritySettings;
    static EType			fireType;
	static std::string		historicalFiresFileName;
    static const float&     fireSpreadRadius()                  { return _fireSpreadRadius; }
    static const double     fireSpreadMean()                    { return _pFireSpreadParms[0]; }
    static const double     fireSpreadStdDev()                  { return _pFireSpreadParms[1]; }
    static const bool       ignoringFirstFireInterval()         { return _ignoringFirstInterval; }
    //static const bool       isFireSuppressionOn()               { return _isFireSuppressionOn; }
    static const int        maxEmpiricalFireSizeEvent()         { return _maxEmpiricalFireSizeEvent; }
    static const float      maxEmpiricalFireSizeEventWeight()   { return _maxEmpiricalFireSizeEventWeight; }

protected:
    static float			_humanIgnitionsProb;
private:
	static float		    _fireSpreadRadius;
    static const double*	_pFireSpreadParms;
	static bool				_ignoringFirstInterval;
    static int				_maxEmpiricalFireSizeEvent;
	static float			_maxEmpiricalFireSizeEventWeight;
	static bool				_isMonthly;
	static const double*	_pFireClimate;
	static float			_climateFireProb;											//Stores the climate probability for a cell so it is only calculated once per cell per iteration
	static SClimate			_previousClimate;											//Indicates the last climate for which the probability was computed
	static int				_yearsOfHistory;

//Functions
public:
							Fire(const int& rYearOfLastBurn=-1, const int& rLastBurnSeverity=0, const float& rFireIgnitionFactor=0., const float& rFireSensitivity=0.);
							Fire(const Fire& rFire);
	virtual					~Fire();
    static void				clear();
    static void             setup();
	virtual void			repStart();
	virtual void			historicalFire(Landscape* pParent);
	virtual float			getFireProb(const Landscape* pLandscape) = 0;			    //The function will be redefined in each frame to be specific for the fire factor relating to that cell type.
	virtual const float		getIgnitionDepressor();
	virtual const float		getHumanIgnitionProb() const = 0;
	const float				getClimateFireProb(const Landscape* pLandscape);				//Returns the base fire prob based on the climate factors (temp/precip)
private:
    static void			    setupFireTransitions();
};

//Overridden in specific frames to give a depressed value. 
inline const float Fire::getIgnitionDepressor()
{
	return 1; //1 == no depression
}

#endif
