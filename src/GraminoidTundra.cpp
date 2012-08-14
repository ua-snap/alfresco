//GraminoidTundra.cpp


#include "PreCompiled.h"
#include "GraminoidTundra.h"
#include "BSpruce.h"
#include "Fresco.h"
#include "RasterIO.h"
#include "CustomLandscape.h"
#include "WSpruce.h"

//Declare static private members
bool			GraminoidTundra::_isStaticSetupAlready		= false;
bool			GraminoidTundra::_isFireProbAgeDependent;
const double*	GraminoidTundra::_pAgeDependentFireParams;		
float			GraminoidTundra::_fireProb;
float			GraminoidTundra::_ignitionDepressor;
double			GraminoidTundra::_seedRange;
const double*	GraminoidTundra::_pSeedSource;
double			GraminoidTundra::_seedBasalArea;
double			GraminoidTundra::_seedlingBasalArea;
int				GraminoidTundra::_history;
const double*	GraminoidTundra::_pSeedEstParams;
double			GraminoidTundra::_meanGrowth;
const double*	GraminoidTundra::_pClimateGrowth;
const double*	GraminoidTundra::_pCalibrationFactor;
double			GraminoidTundra::_seedling;
double			GraminoidTundra::_ratioAK = 0.;
double			GraminoidTundra::_tundraSpruceBasalArea;
const double*	GraminoidTundra::_pStartAgeParms;
double*			GraminoidTundra::_pIntegral;
EStartAgeType	GraminoidTundra::_startAgeType;


GraminoidTundra::GraminoidTundra(
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
    _GraminoidTundra(treeDensity); 
}


GraminoidTundra::				GraminoidTundra(const Frame& rFrame, const int treeDensity) 
    : Frame(rFrame)
{
	_yearEstablished		= gYear;
	_yearFrameEstablished	= gYear;
	_GraminoidTundra(treeDensity);
}


void GraminoidTundra::			_GraminoidTundra(const int treeDensity) 
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


GraminoidTundra::				~GraminoidTundra() 
{
}


void GraminoidTundra::           setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
        _humanIgnitionsProb	    = FRESCO->fif().dGet("GraminoidTundra.HumanFireProb");
		_isFireProbAgeDependent = FRESCO->fif().bGet("GraminoidTundra.FireProb.IsAgeDependent");
		if (_isFireProbAgeDependent) {
			if (3 != FRESCO->fif().pdGet("GraminoidTundra.FireProb", _pAgeDependentFireParams))
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: GraminoidTundra.FireProb (because Tundra.FireProb.IsAgeDependent is set to TRUE)");
		}
		else
	        _fireProb = FRESCO->fif().dGet("GraminoidTundra.FireProb");
		if (FRESCO->fif().CheckKey("GraminoidTundra.IgnitionDepressor"))
			_ignitionDepressor = FRESCO->fif().dGet("GraminoidTundra.IgnitionDepressor");
		else
			_ignitionDepressor = 1;
        _history			    = FRESCO->fif().nGet("GraminoidTundra.History");
        _seedRange		        = FRESCO->fif().dGet("GraminoidTundra.SeedRange");
        _seedBasalArea	        = FRESCO->fif().dGet("GraminoidTundra.Seed.BasalArea");
        _seedling			    = FRESCO->fif().dGet("GraminoidTundra.Seedling");
        _seedlingBasalArea      = FRESCO->fif().dGet("GraminoidTundra.SeedlingBA");
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("GraminoidTundra->Spruce.BasalArea");
        _pStartAgeParms         = FRESCO->getStartAgeParms("GraminoidTundra.StartAge", &_startAgeType);
        _meanGrowth             = FRESCO->fif().dGet("GraminoidTundra.MeanGrowth");
        if (2 != FRESCO->fif().pdGet("GraminoidTundra.SeedEstParms", _pSeedEstParams)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: GraminoidTundra.SeedEstParms");
        }
        if (3 != FRESCO->fif().pdGet("GraminoidTundra.ClimGrowth", _pClimateGrowth)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: GraminoidTundra.ClimGrowth");
        }
        if (2 != FRESCO->fif().pdGet("GraminoidTundra.CalFactor", _pCalibrationFactor)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: GraminoidTundra.CalFactor");
        }
        if (2 != FRESCO->fif().pdGet("GraminoidTundra.SeedSource", _pSeedSource)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: GraminoidTundra.SeedSource");
        }

		//Calculate _ratioAK for use in getInitialBasalAreaI()
        double spruceEstBasalAarea	= FRESCO->fif().dGet("GraminoidTundra.Spruce.EstBA");
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


void GraminoidTundra::			clear()
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



float			GraminoidTundra::getAsFloat(RasterIO::ALFMapType mapType)
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



Frame *GraminoidTundra::		    success(Landscape* pParent) 
//This function provides the successional information for the tundra frame.  It is expected to return NULL
//if the frame type does not change, and a pointer to a new frame if a transition occurs.  The general
//model used is to check immediate post burn stuff first, then time dependant state changes, and then 
//general (long term) state changes. Specific algorithms are documented in the code
{
	//Check immediately after burn
	const int yearsSinceLastBurn = gYear - yearOfLastBurn;
	//std::cout << gYear << std::endl;
	if (yearsSinceLastBurn == 1) {
        //This frame burned last year, so reset degree years to start tracking again.
        	_yearEstablished	= gYear;
		_speciesSubCanopy	= gGraminoidTundraID;
		_basalArea	        = 0.;
		_yearOfEstablishment= -_history;
		_degrees		    = -1.;
	//} else if (gYear > 20 && yearsSinceLastBurn >= 50) {
	//	return new ShrubTundra(*this);
	}
	double avgMonthlyTemp = (pParent->cellTempByMonth(3) + pParent->cellTempByMonth(4) + pParent->cellTempByMonth(5) + pParent->cellTempByMonth(6)) / 4;
	if (_basalArea < 5){
		double params[3] = {0., _pSeedSource[0], _pSeedSource[1]};		                    //The first location will get set to the actual distance
		double seeds = pParent->neighborsSuccess(&Frame::queryReply, &FatTail, _seedRange, params);	//Find the neighborhood seed source - returns the weighted basal area
		_basalArea += seeds * 10;
	} else {
		double basalIncrement = 0;
		if ((rand() % 10) < 6){
			_basalArea += 0.1;
		} else {
			_basalArea += 0.9;
		}
	}

	//Transition if necessary
	if (_basalArea >= _tundraSpruceBasalArea) {
		const double probability = Site(_site,0.5);
		if (probability > GetNextRandom())
			return new BSpruce(*this);
		else
			return new WSpruce(*this);
	}
	return NULL;
}


double GraminoidTundra::		getInitialBasalArea() 
//Returns an initial basal area for the cell based on the distribution derived in GraminoidTundra.doc 
//and BasalArea.xls!PDFTest.  Basically selects a basal area from a distribution which matches the
//expected growth curve generated by the Excel model.  Hence the inital basal area distribution 
//matches what one would expect at any given time.  This reduces initial condition purturbations.
{
	if (_ratioAK)
		return _ratioAK * ( pow(_tundraSpruceBasalArea/_ratioAK + 1.,(double) GetNextRandom()) - 1 );
	else
		return 0.;
}
