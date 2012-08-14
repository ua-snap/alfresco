//WetlandTundra.cpp


#include "PreCompiled.h"
#include "WetlandTundra.h"
#include "BSpruce.h"
#include "Fresco.h"
#include "RasterIO.h"
#include "CustomLandscape.h"
#include "WSpruce.h"

//Declare static private members
bool			WetlandTundra::_isStaticSetupAlready		= false;
bool			WetlandTundra::_isFireProbAgeDependent;
const double*	WetlandTundra::_pAgeDependentFireParams;		
float			WetlandTundra::_fireProb;
float			WetlandTundra::_ignitionDepressor;
double			WetlandTundra::_seedRange;
const double*	WetlandTundra::_pSeedSource;
double			WetlandTundra::_seedBasalArea;
double			WetlandTundra::_seedlingBasalArea;
int				WetlandTundra::_history;
const double*	WetlandTundra::_pSeedEstParams;
double			WetlandTundra::_meanGrowth;
const double*	WetlandTundra::_pClimateGrowth;
const double*	WetlandTundra::_pCalibrationFactor;
double			WetlandTundra::_seedling;
double			WetlandTundra::_ratioAK = 0.;
double			WetlandTundra::_tundraSpruceBasalArea;
const double*	WetlandTundra::_pStartAgeParms;
double*			WetlandTundra::_pIntegral;
EStartAgeType	WetlandTundra::_startAgeType;


WetlandTundra::WetlandTundra(
        const int& rAge, 
        const bool& rIsTopoComplex, 
        const float& rSite, 
        const int& rYearOfLastBurn,
		const int& rLastBurnSeverity,
        const double& rFireIgnitionFactor, 
        const double& rFireSensitivity,  
        const Species& rSpecSubCanopy, 
        const int treeDensity)  
    : Frame (rAge, rIsTopoComplex, rSite, rYearOfLastBurn, rLastBurnSeverity, rFireIgnitionFactor, rFireSensitivity, rSpecSubCanopy)
{ 
    _WetlandTundra(treeDensity); 
}


WetlandTundra::				WetlandTundra(const Frame& rFrame, const int treeDensity) 
    : Frame(rFrame)
{
	_yearEstablished		= gYear;
	_yearFrameEstablished	= gYear;
	_WetlandTundra(treeDensity);
}


void WetlandTundra::			_WetlandTundra(const int treeDensity) 
//Local constructor - Initialize the local member variables and give the frame an initial age.  
//Default values are specified for all the arguments.  However, it is desired to pull the
//default tree density from the initialization file so the user can specify this on the fly.  As a result,
//a default argument for TreeDensity is provided, but it is overwritten if possible.
{
    if (!_isStaticSetupAlready)
        throw SimpleException(SimpleException::UNKNOWN, "Static data members must be set before initializing species.");

	//Calc a starting age if not yet assigned.
	if (gFirstYear-1==_yearEstablished)
    {
		if (_startAgeType==CONSTANT) {
			_yearEstablished = _yearFrameEstablished = gFirstYear - (int)(GetNextRandom() * _pStartAgeParms[0]);
		}
		else {
			const double random = GetNextRandom();
			int age = 0;
			while (random > _pIntegral[age++]);
			_yearEstablished = _yearFrameEstablished = gFirstYear + 1 - age;
		}
    }
	//Do other initializations
	if (treeDensity <= 0)
		_basalArea = 0.;
	else
		_basalArea = getInitialBasalArea();
	_yearOfEstablishment = 0;
//OLD TODO:	_yearOfEstablishment = -_history;
	_degrees = -1;
}


WetlandTundra::				~WetlandTundra() 
{
}


void WetlandTundra::           setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
        _humanIgnitionsProb	    = FRESCO->fif().dGet("WetlandTundra.HumanFireProb");
		_isFireProbAgeDependent = FRESCO->fif().bGet("WetlandTundra.FireProb.IsAgeDependent");
		if (_isFireProbAgeDependent) {
			if (3 != FRESCO->fif().pdGet("WetlandTundra.FireProb", _pAgeDependentFireParams))
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: WetlandTundra.FireProb (because Tundra.FireProb.IsAgeDependent is set to TRUE)");
		}
		else
	        _fireProb = FRESCO->fif().dGet("WetlandTundra.FireProb");
		if (FRESCO->fif().CheckKey("WetlandTundra.IgnitionDepressor"))
			_ignitionDepressor = FRESCO->fif().dGet("WetlandTundra.IgnitionDepressor");
		else
			_ignitionDepressor = 1;
        _history			    = FRESCO->fif().nGet("WetlandTundra.History");
        _seedRange		        = FRESCO->fif().dGet("WetlandTundra.SeedRange");
        _seedBasalArea	        = FRESCO->fif().dGet("WetlandTundra.Seed.BasalArea");
        _seedling			    = FRESCO->fif().dGet("WetlandTundra.Seedling");
        _seedlingBasalArea      = FRESCO->fif().dGet("WetlandTundra.SeedlingBA");
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("WetlandTundra->Spruce.BasalArea");
        _pStartAgeParms         = FRESCO->getStartAgeParms("WetlandTundra.StartAge", &_startAgeType);
        _meanGrowth             = FRESCO->fif().dGet("WetlandTundra.MeanGrowth");
        if (2 != FRESCO->fif().pdGet("WetlandTundra.SeedEstParms", _pSeedEstParams)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: WetlandTundra.SeedEstParms");
        }
        if (3 != FRESCO->fif().pdGet("WetlandTundra.ClimGrowth", _pClimateGrowth)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: WetlandTundra.ClimGrowth");
        }
        if (2 != FRESCO->fif().pdGet("WetlandTundra.CalFactor", _pCalibrationFactor)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: WetlandTundra.CalFactor");
        }
        if (2 != FRESCO->fif().pdGet("WetlandTundra.SeedSource", _pSeedSource)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: WetlandTundra.SeedSource");
        }

		//Calculate _ratioAK for use in getInitialBasalAreaI()
        double spruceEstBasalAarea	= FRESCO->fif().dGet("WetlandTundra.Spruce.EstBA");
		const double pSeedSourceFatTailParams[3] = {gCellSize, _pSeedSource[0], _pSeedSource[1]};  //Parameters for calling FatTail()
		const double alpha = _pCalibrationFactor[1] * spruceEstBasalAarea * _seedBasalArea * FatTail(pSeedSourceFatTailParams) / _seedling * _seedlingBasalArea;
		double k = _pCalibrationFactor[0] * _meanGrowth;
		_ratioAK = (k != 0) ? alpha/k : 0;

		if (_startAgeType==WEIBULL) {
			int length = (int)(5*_pStartAgeParms[0]);
			_pIntegral = new double[length];
			Integrate (WeibullReliability, _pStartAgeParms, _pIntegral, 0, length-1, 1.);
			//Normalize so the integral is 1.
			for (int i=0; i<length; i++) _pIntegral[i] /= _pIntegral[length-1];
		}
		
        _isStaticSetupAlready = true;
	}
}


void WetlandTundra::			clear()
//Clear existing run if any and return to before a run is specified.
{
	_isStaticSetupAlready	= false;
	_fireProb				= 0;
	_ignitionDepressor		= 1;
	_seedRange				= 0;
	_seedBasalArea		    = 0;
	_seedlingBasalArea		= 0;
	_history				= 0;
	_meanGrowth			    = 0;
	_seedling				= 0;
	_tundraSpruceBasalArea	= 0;
	delete[] _pIntegral; _pIntegral	= 0;
	_startAgeType			= CONSTANT;
}



float			WetlandTundra::getAsFloat(RasterIO::ALFMapType mapType)
{
	switch(mapType)
	{
	case RasterIO::TUNDRA_BASAL_AREA:
		return (float)_basalArea;
		break;
	default:
		throw Poco::Exception("This frame type ("+ToS((int)type())+") does not support the map type ("+ RasterIO::getMapTypeAsString(mapType) + ")");
	}
}



Frame *WetlandTundra::		    success(Landscape* pParent) 
//This function provides the successional information for the tundra frame.  It is expected to return NULL
//if the frame type does not change, and a pointer to a new frame if a transition occurs.  The general
//model used is to check immediate post burn stuff first, then time dependant state changes, and then 
//general (long term) state changes. Specific algorithms are documented in the code
{
	return NULL;
}


double WetlandTundra::		getInitialBasalArea() 
//Returns an initial basal area for the cell based on the distribution derived in WetlandTundra.doc 
//and BasalArea.xls!PDFTest.  Basically selects a basal area from a distribution which matches the
//expected growth curve generated by the Excel model.  Hence the inital basal area distribution 
//matches what one would expect at any given time.  This reduces initial condition purturbations.
{
	if (_ratioAK)
		return _ratioAK * ( pow(_tundraSpruceBasalArea/_ratioAK + 1.,(double) GetNextRandom()) - 1 );
	else
		return 0.;
}
