//Fire.cpp
//This module contains all the functions pertaining to the Fire class, constructor(s), destructor(s), a function
//to determing the climate portion of the fire probability and a function which, in conjunction with Landscape::Neighbors
//burns fires recursively.  This algorithm is a bit involved and is described in more detail in the Burn function.


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Fire.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/Interface.h"
#include "Fresco/Foundation/Landscape.h"	


//Declare static private members for optimization
std::vector<Fire::SFireTransition> Fire::fireTransitions;
Fire::EType	    Fire::fireType						    = Fire::FIXED;
std::string     Fire::historicalFiresFileName           = "";
float		    Fire::_fireSpreadRadius			        = 0;
const double*	Fire::_pFireSpreadParms			        = new double[2];
bool			Fire::_ignoringFirstInterval	        = false;
bool			Fire::_isMonthly						= false;
int				Fire::_maxEmpiricalFireSizeEvent        = -1;
float			Fire::_maxEmpiricalFireSizeEventWeight  = 1;
const double*	Fire::_pFireClimate				        = new double[3];
float			Fire::_climateFireProb			        = -1;
SClimate		Fire::_previousClimate;
int				Fire::_yearsOfHistory			        = -9999999;
float			Fire::_humanIgnitionsProb		        = 0;
Fire::SBurnSeveritySettings Fire::burnSeveritySettings;

Fire::Fire(const int& rYearOfLastBurn, const int& rLastBurnSeverity, const float& rFireIgnitionFactor, const float& rFireSensitivity) 
//Constructor for the fire class.  Initializes the global scope optimization variables and the fire age member
//variables.
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
//Clear existing run if any and return to before a run is specified.
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
	if (FRESCO->fif().pdGet("Fire.SpreadParms", _pFireSpreadParms) != 2)    throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.SpreadParms");

	_isMonthly = false;
	if (FRESCO->fif().CheckKey("Climate.IsMonthly"))
		_isMonthly = FRESCO->fif().bGet("Climate.IsMonthly");
	int numParams = _isMonthly ? 8 : 3;
	if (FRESCO->fif().pdGet("Fire.Climate", _pFireClimate) != numParams)            throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Climate");

	_fireSpreadRadius				    = FRESCO->fif().dGet("Fire.SpreadRadius");
	_ignoringFirstInterval              = FRESCO->fif().bGet("Fire.IgnoreFirstInterval");
    _maxEmpiricalFireSizeEvent          = FRESCO->fif().nGet("Fire.MaxEmpiricalFireSizeEvent");
    _maxEmpiricalFireSizeEventWeight    = FRESCO->fif().dGet("Fire.MaxEmpiricalFireSizeEventWeight");
    _yearsOfHistory                     = FRESCO->fif().nGet("Climate.NumHistory");


	numParams = 2;
	const double* params = new double[2];
	if (FRESCO->fif().pdGet("BurnSeverity.FxnOfFireSize", params) != numParams)		throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Climate");
	burnSeveritySettings.FxnIntercept = params[0];
	burnSeveritySettings.FxnSlope = params[1];
	burnSeveritySettings.LssVsHssWeight			= FRESCO->fif().dGet("BurnSeverity.LSS-vs-HSS.wt");
	burnSeveritySettings.LowVsModerateWeight	= FRESCO->fif().dGet("BurnSeverity.Low-vs-Moderate.wt");
	burnSeveritySettings.FlatTopoWeight			= FRESCO->fif().dGet("BurnSeverity.FlatTopo.wt");
	burnSeveritySettings.ComplexTopoWeight		= FRESCO->fif().dGet("BurnSeverity.ComplexTopo.wt");


}


const float Fire::getClimateFireProb (const Landscape* l) 
//This function is a simple worker function which is called by the burn function to provide 
//a base fire probability based on the climate data. The non monthly values were calibrated up using
//a regression developed from mean climate and fire data using the ecoregions to subdivide 
//the landscape into similar climatic regions.  This is considered a good relative indication
//of fire probability.  The model was then calibrated to produce observed mean # fires and 
//area burned to get the overall rate constant.
{
	if (_isMonthly)  //use monthly equation
	{
		_climateFireProb =    _pFireClimate[0]	//intercept
							//Temps: mar apr may jun
							+ _pFireClimate[1]*l->cellTempByMonth(3) 
							+ _pFireClimate[2]*l->cellTempByMonth(4) 
							+ _pFireClimate[3]*l->cellTempByMonth(5) 
							+ _pFireClimate[4]*l->cellTempByMonth(6)
							//Precips: jun jul
							+ _pFireClimate[5]*l->cellPrecipByMonth(6) 
							+ _pFireClimate[6]*l->cellPrecipByMonth(7)
							//aprT*julP
							+ _pFireClimate[7]*l->cellTempByMonth(4)*l->cellPrecipByMonth(7);
	}
	else //use growing season equation
	{
		const SClimate climate = l->cellClimate();
		_climateFireProb = _pFireClimate[0] + _pFireClimate[1]*climate.Temp + _pFireClimate[2]*climate.Precip;
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
//Fill vector of SFireTransition structs from FIF.
{
    int           count = 0;
    const int*    pYears;
    char *const*  pTypes;
    const double* pIgnitionFactors;
    const double* pSensitivities;
    char *const*  pSpatialIgnitionFactorFiles;
    char *const*  pSpatialSensitivityFiles;
    char *const*  pHistoryFireFiles;
    
    //Get arrays of values and make sure all arrays have the same count.
    count = FRESCO->fif().pnGet("Fire.TypeTransitionYears", pYears);
    if (FRESCO->fif().psGet("Fire.Types", pTypes) != count)                                          throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Types");
    if (FRESCO->fif().pdGet("Fire.IgnitionFactor", pIgnitionFactors) != count)                       throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.IgnitionFactor");
    if (FRESCO->fif().pdGet("Fire.Sensitivity", pSensitivities) != count)                            throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Sensitivity");
    if (FRESCO->fif().psGet("Fire.Spatial.IgnitionFactor", pSpatialIgnitionFactorFiles) != count)    throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Spatial.IgnitionFactor");
    if (FRESCO->fif().psGet("Fire.Spatial.Sensitivity", pSpatialSensitivityFiles) != count)          throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Spatial.Sensitivity");
    if (FRESCO->fif().psGet("Fire.Historical", pHistoryFireFiles) != count)                          throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: Fire.Historical");

    for (int i=0; i<count; i++) {
        SFireTransition FireTransition;
	    std::vector<SFireTransition>::iterator iter;
	    //Create the fire transition.
	    FireTransition.Year						= pYears[i];
        std::string type(pTypes[i]);
        if (type=="FIXED")                      FireTransition.Type = FIXED;
        else if (type=="SPATIAL")               FireTransition.Type = SPATIAL;
        else if (type=="HISTORICAL")            FireTransition.Type = HISTORICAL;
        else throw Exception(Exception::BADVARTYPE, "Unexpected value for Fire.Types", pTypes[i]);
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
