/** 
 * @file
 *
 * This module contains all the functions pertaining to the Fire class, constructor(s), destructor(s), a function
 * to determing the climate portion of the fire probability and a function which, in conjunction with Landscape::Neighbors
 * burns fires recursively.  This algorithm is a bit involved and is described in more detail in the Burn function.
 */


#include "Fire.h"
#include "Landscape.h"	


//Declare static private members for optimization
std::vector<Fire::SFireTransition> Fire::fireTransitions;
Fire::EType	    Fire::fireType						    = Fire::FIXED;
std::string     Fire::historicalFiresFileName           = "";
float		    Fire::_fireSpreadRadius			        = 0;
double*	Fire::_pFireSpreadParms;
bool			Fire::_ignoringFirstInterval	        = false;
bool			Fire::_isMonthly						= false;
bool			Fire::_isExperimental						= false;
int				Fire::_maxEmpiricalFireSizeEvent        = -1;
float			Fire::_maxEmpiricalFireSizeEventWeight  = 1;
double*	Fire::_pFireClimate;
float			Fire::_climateFireProb			        = -1;
SClimate		Fire::_previousClimate;
int				Fire::_yearsOfHistory			        = -9999999;
float			Fire::_humanIgnitionsProb		        = 0;
Fire::SBurnSeveritySettings Fire::burnSeveritySettings;

Fire::Fire(const int& rYearOfLastBurn, const int& rLastBurnSeverity, const float& rFireIgnitionFactor, const float& rFireSensitivity) 
/**
 * Constructor for the fire class.  Initializes the global scope optimization variables and the fire age member variables.
 */
{
	fireScarID			= 0;												            //Initial value for all cells when landscape is created.  Changed on new fires.  Successions pass on previous ID.
	yearOfLastBurn		= (rYearOfLastBurn==-1) ? -1 - _yearsOfHistory : rYearOfLastBurn;	    //Set the default value to beyond what we could want to check
	lastBurnWasOrigin	= false;
	lastBurnCause		= NATURAL;
	fireIgnitionFactor	= rFireIgnitionFactor;
	fireSensitivity		= rFireSensitivity;
	burnSeverity		= (Fire::EBurnSeverity)rLastBurnSeverity;
}


Fire::Fire(const Fire& rFire)
{ 
	fireScarID			= rFire.fireScarID;	
	yearOfLastBurn		= rFire.yearOfLastBurn;
	lastBurnWasOrigin	= rFire.lastBurnWasOrigin;
	lastBurnCause		= rFire.lastBurnCause; 
	fireIgnitionFactor	= rFire.fireIgnitionFactor;
	fireSensitivity		= rFire.fireSensitivity;
	burnSeverity		= rFire.burnSeverity;
}


Fire::~Fire()
{
}


void Fire::repStart()
{
}


void Fire::clear()
/**
 * Clear existing run if any and return to before a run is specified.
 */
{
	fireType							= Fire::FIXED;
	_humanIgnitionsProb			        = 0;
	_fireSpreadRadius			        = 0;
	_ignoringFirstInterval		        = false;
	_climateFireProb					= -1;
	_previousClimate.Temp				= 0;
	_previousClimate.Precip				= 0;
	_yearsOfHistory						= -9999999;
	fireTransitions.clear();
}


void Fire::setup()
{
    setupFireTransitions();
	if (FRESCO->fif().pdGet(FRESCO->fif().root["Fire"]["SpreadParms"], _pFireSpreadParms) != 2)    throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.SpreadParms");

	_isMonthly = false;
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Climate"]["IsMonthly"]))
		_isMonthly = FRESCO->fif().root["Climate"]["IsMonthly"].asBool();
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Climate"]["IsExperimental"]))
		_isExperimental = FRESCO->fif().root["Climate"]["IsExperimental"].asBool();
	int numParams;
	if (_isExperimental){
		numParams = 13;
	} else if (_isMonthly){
		numParams = 8;
	} else {
		numParams = 3;
	}
	if (FRESCO->fif().pdGet(FRESCO->fif().root["Fire"]["Climate"], _pFireClimate) != numParams){
		throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Climate");
	}

	_fireSpreadRadius				    = FRESCO->fif().root["Fire"]["SpreadRadius"].asDouble();
	_ignoringFirstInterval              = FRESCO->fif().root["Fire"]["IgnoreFirstInterval"].asBool();
    _maxEmpiricalFireSizeEvent          = FRESCO->fif().root["Fire"]["MaxEmpiricalFireSizeEvent"].asInt();
    _maxEmpiricalFireSizeEventWeight    = FRESCO->fif().root["Fire"]["MaxEmpiricalFireSizeEventWeight"].asDouble();
    _yearsOfHistory                     = FRESCO->fif().root["Climate"]["NumHistory"].asInt();

	double* params;
	//std::cout << FRESCO->fif().pdGet(FRESCO->fif().root["BurnSeverity"]["FxnOfFireSize"], params) << ":" << 2 << std::endl;
	if (FRESCO->fif().pdGet(FRESCO->fif().root["Fire"]["BurnSeverity"]["FxnOfFireSize"], params) != 2)		throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: BurnSeverity.FxnOfFireSize");
	burnSeveritySettings.FxnIntercept = params[0];
	burnSeveritySettings.FxnSlope = params[1];
	burnSeveritySettings.LssVsHssWeight			= FRESCO->fif().root["Fire"]["BurnSeverity"]["LSS-vs-HSS.wt"].asDouble();
	burnSeveritySettings.LowVsModerateWeight	= FRESCO->fif().root["Fire"]["BurnSeverity"]["Low-vs-Moderate.wt"].asDouble();
	burnSeveritySettings.FlatTopoWeight			= FRESCO->fif().root["Fire"]["BurnSeverity"]["FlatTopo.wt"].asDouble();
	burnSeveritySettings.ComplexTopoWeight		= FRESCO->fif().root["Fire"]["BurnSeverity"]["ComplexTopo.wt"].asDouble();
}


const float Fire::getClimateFireProb (const Landscape* l) 
/**
 * This function is a simple worker function which is called by the burn function to provide 
 * a base fire probability based on the climate data. The non monthly values were calibrated up using 
 * a regression developed from mean climate and fire data using the ecoregions to subdivide 
 * the landscape into similar climatic regions.  This is considered a good relative indication 
 * of fire probability.  The model was then calibrated to produce observed mean # fires and 
 * area burned to get the overall rate constant.
 */
{
	if (l->usingExternalClimateFlammabilityFile())
	{
		const float p = l->cellClimateFlammability();
		if (IsNodata(p)) {
			return 0;
		}
		else {
			_climateFireProb = p;
		}
	}
	else if (_isExperimental)  //use monthly equation
	{
		const float t3 = l->cellTempByMonth(3);
		const float t4 = l->cellTempByMonth(4);
		const float t5 = l->cellTempByMonth(5);
		const float t6 = l->cellTempByMonth(6);
		const float t7 = l->cellTempByMonth(7);
		const float t8 = l->cellTempByMonth(8);
		const float t9 = l->cellTempByMonth(9);
		const float p6 = l->cellPrecipByMonth(6);
		const float p7 = l->cellPrecipByMonth(7);
		const float p8 = l->cellPrecipByMonth(8);
		const float p9 = l->cellPrecipByMonth(9);

		if (IsNodata(t3) || IsNodata(t4) || IsNodata(t5) || IsNodata(t6)
			|| IsNodata(t7) || IsNodata(t8) || IsNodata(t9)
			|| IsNodata(p6) || IsNodata(p7) || IsNodata(p8) || IsNodata(p9))
			return 0;

		_climateFireProb =    _pFireClimate[0]	//intercept
							//Temps: mar apr may jun jul aug sep
							+ _pFireClimate[1] * t3 
							+ _pFireClimate[2] * t4 
							+ _pFireClimate[3] * t5 
							+ _pFireClimate[4] * t6
							+ _pFireClimate[5] * t7
							+ _pFireClimate[6] * t8
							+ _pFireClimate[7] * t9
							//Precips: jun jul
							+ _pFireClimate[8] * p6 
							+ _pFireClimate[9] * p7
							+ _pFireClimate[10] * p8
							+ _pFireClimate[11] * p9;
	}
	else if (_isMonthly)  //use monthly equation
	{
		const float t3 = l->cellTempByMonth(3);
		const float t4 = l->cellTempByMonth(4);
		const float t5 = l->cellTempByMonth(5);
		const float t6 = l->cellTempByMonth(6);
		const float p6 = l->cellPrecipByMonth(6);
		const float p7 = l->cellPrecipByMonth(7);

		if (IsNodata(t3) || IsNodata(t4) || IsNodata(t5) || IsNodata(t6)
			|| IsNodata(p6) || IsNodata(p7))
			return 0;

		_climateFireProb =    _pFireClimate[0]	//intercept
							//Temps: mar apr may jun
							+ _pFireClimate[1] * t3 
							+ _pFireClimate[2] * t4 
							+ _pFireClimate[3] * t5 
							+ _pFireClimate[4] * t6
							//Precips: jun jul
							+ _pFireClimate[5] * p6 
							+ _pFireClimate[6] * p7
							//aprT*julP
							+ _pFireClimate[7] * t4 * p7;
	}
	else //use growing season equation
	{
		const SClimate climate = l->cellClimate();
		if (IsNodata(climate.Temp) || IsNodata(climate.Precip)) {
			return 0;
		}
		else {
			_climateFireProb = _pFireClimate[0] + _pFireClimate[1]*climate.Temp + _pFireClimate[2]*climate.Precip;
		}
	}
	return _climateFireProb;
}

void Fire::historicalFire (Landscape* pParent) 
{
	//This call needs to preceed the resetting of the age of this cell
	pParent->logFireStats((yearOfLastBurn<0) ? -gYear : gYear - yearOfLastBurn, _ignoringFirstInterval);	
	//Update the date of last burn
	yearOfLastBurn = gYear;								
}


void Fire::setupFireTransitions() 
/**
 * Fill vector of SFireTransition structs from FIF.
 */
{
    int           count = 0;
    int*    pYears;
    //char *const*  pTypes;
    std::string*   pTypes;
    double* pIgnitionFactors;
    double* pSensitivities;
    std::string*  pSpatialIgnitionFactorFiles;
    std::string*  pSpatialSensitivityFiles;
    std::string*  pHistoryFireFiles;
    
    //Get arrays of values and make sure all arrays have the same count.
    count = FRESCO->fif().pnGet(FRESCO->fif().root["Fire"]["TypeTransitionYears"], pYears);
    if (FRESCO->fif().psGet(FRESCO->fif().root["Fire"]["Types"], pTypes) != count)                                          throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Types");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Fire"]["IgnitionFactor"], pIgnitionFactors) != count)                       throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.IgnitionFactor");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Fire"]["Sensitivity"], pSensitivities) != count)                            throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Sensitivity");
    if (FRESCO->fif().psGet(FRESCO->fif().root["Fire"]["Spatial.IgnitionFactor"], pSpatialIgnitionFactorFiles) != count)    throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Spatial.IgnitionFactor");
    if (FRESCO->fif().psGet(FRESCO->fif().root["Fire"]["Spatial.Sensitivity"], pSpatialSensitivityFiles) != count)          throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Spatial.Sensitivity");
    if (FRESCO->fif().psGet(FRESCO->fif().root["Fire"]["Historical"], pHistoryFireFiles) != count)                          throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Historical");

    for (int i=0; i<count; i++) {
        SFireTransition FireTransition;
	    std::vector<SFireTransition>::iterator iter;
	    //Create the fire transition.
	    FireTransition.Year						= pYears[i];
        std::string type(pTypes[i]);
        if (type=="FIXED")                      FireTransition.Type = FIXED;
        else if (type=="SPATIAL")               FireTransition.Type = SPATIAL;
        else if (type=="HISTORICAL")            FireTransition.Type = HISTORICAL;
        else throw SimpleException(SimpleException::BADVARTYPE, "Unexpected value for Fire.Types", pTypes[i]);
	    FireTransition.Ignition					= pIgnitionFactors[i];
	    FireTransition.Sensitivity				= pSensitivities[i];
        FireTransition.SpatialIgnitionFile		= FormatDirectory(std::string(pSpatialIgnitionFactorFiles[i]));
	    FireTransition.SpatialSensitivityFile	= FormatDirectory(std::string(pSpatialSensitivityFiles[i]));
        FireTransition.HistoricalFile           = FormatDirectory(std::string(pHistoryFireFiles[i]));
        //Insert it before the first later year.
	    for (iter=Fire::fireTransitions.begin(); iter!=Fire::fireTransitions.end(); iter++)
		    if (FireTransition.Year<iter->Year) break;
	    Fire::fireTransitions.insert(iter,FireTransition);
    }
}
