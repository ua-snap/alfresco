//ShrubTundra.cpp


#include "PreCompiled.h"
#include "ShrubTundra.h"
#include "BSpruce.h"
#include "Fresco.h"
#include "RasterIO.h"
#include "CustomLandscape.h"
#include "WSpruce.h"

//Declare static private members
bool			ShrubTundra::_isStaticSetupAlready		= false;
bool			ShrubTundra::_isFireProbAgeDependent;
const double*	ShrubTundra::_pAgeDependentFireParams;		
float			ShrubTundra::_fireProb;
float			ShrubTundra::_ignitionDepressor;
double			ShrubTundra::_seedRange;
const double*	ShrubTundra::_pSeedSource;
double			ShrubTundra::_seedBasalArea;
double			ShrubTundra::_seedlingBasalArea;
int				ShrubTundra::_history;
const double*	ShrubTundra::_pSeedEstParams;
double			ShrubTundra::_meanGrowth;
const double*	ShrubTundra::_pClimateGrowth;
const double*	ShrubTundra::_pCalibrationFactor;
double			ShrubTundra::_seedling;
double			ShrubTundra::_ratioAK = 0.;
double			ShrubTundra::_tundraSpruceBasalArea;
const double*	ShrubTundra::_pStartAgeParms;
double*			ShrubTundra::_pIntegral;
EStartAgeType	ShrubTundra::_startAgeType;


ShrubTundra::ShrubTundra(
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
    _ShrubTundra(treeDensity); 
}


ShrubTundra::				ShrubTundra(const Frame& rFrame, const int treeDensity) 
    : Frame(rFrame)
{
	_yearEstablished		= gYear;
	_yearFrameEstablished	= gYear;
	_ShrubTundra(treeDensity);
}


void ShrubTundra::			_ShrubTundra(const int treeDensity) 
//Local constructor - Initialize the local member variables and give the frame an initial age.  
//Default values are specified for all the arguments.  However, it is desired to pull the
//default tree density from the initialization file so the user can specify this on the fly.  As a result,
//a default argument for TreeDensity is provided, but it is overwritten if possible.
{
    if (!_isStaticSetupAlready)
        throw Exception(Exception::UNKNOWN, "Static data members must be set before initializing species.");

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


ShrubTundra::				~ShrubTundra() 
{
}


void ShrubTundra::           setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
        _humanIgnitionsProb	    = FRESCO->fif().dGet("ShrubTundra.HumanFireProb");
		_isFireProbAgeDependent = FRESCO->fif().bGet("ShrubTundra.FireProb.IsAgeDependent");
		if (_isFireProbAgeDependent) {
			if (3 != FRESCO->fif().pdGet("ShrubTundra.FireProb", _pAgeDependentFireParams))
				throw Exception(Exception::BADARRAYSIZE, "Expected array size of 3 for key: ShrubTundra.FireProb (because Tundra.FireProb.IsAgeDependent is set to TRUE)");
		}
		else
	        _fireProb = FRESCO->fif().dGet("ShrubTundra.FireProb");
		if (FRESCO->fif().CheckKey("ShrubTundra.IgnitionDepressor"))
			_ignitionDepressor = FRESCO->fif().dGet("ShrubTundra.IgnitionDepressor");
		else
			_ignitionDepressor = 1;
        _history			    = FRESCO->fif().nGet("ShrubTundra.History");
        _seedRange		        = FRESCO->fif().dGet("ShrubTundra.SeedRange");
        _seedBasalArea	        = FRESCO->fif().dGet("ShrubTundra.Seed.BasalArea");
        _seedling			    = FRESCO->fif().dGet("ShrubTundra.Seedling");
        _seedlingBasalArea      = FRESCO->fif().dGet("ShrubTundra.SeedlingBA");
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("ShrubTundra->Spruce.BasalArea");
        _pStartAgeParms         = FRESCO->getStartAgeParms("ShrubTundra.StartAge", &_startAgeType);
        _meanGrowth             = FRESCO->fif().dGet("ShrubTundra.MeanGrowth");
        if (2 != FRESCO->fif().pdGet("ShrubTundra.SeedEstParms", _pSeedEstParams)) {
            throw Exception(Exception::BADARRAYSIZE, "Expected array size of 2 for key: ShrubTundra.SeedEstParms");
        }
        if (3 != FRESCO->fif().pdGet("ShrubTundra.ClimGrowth", _pClimateGrowth)) {
            throw Exception(Exception::BADARRAYSIZE, "Expected array size of 3 for key: ShrubTundra.ClimGrowth");
        }
        if (2 != FRESCO->fif().pdGet("ShrubTundra.CalFactor", _pCalibrationFactor)) {
            throw Exception(Exception::BADARRAYSIZE, "Expected array size of 2 for key: ShrubTundra.CalFactor");
        }
        if (2 != FRESCO->fif().pdGet("ShrubTundra.SeedSource", _pSeedSource)) {
            throw Exception(Exception::BADARRAYSIZE, "Expected array size of 2 for key: ShrubTundra.SeedSource");
        }

		//Calculate _ratioAK for use in getInitialBasalAreaI()
        double spruceEstBasalAarea	= FRESCO->fif().dGet("ShrubTundra.Spruce.EstBA");
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


void ShrubTundra::			clear()
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



float			ShrubTundra::getAsFloat(RasterIO::ALFMapType mapType)
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



Frame *ShrubTundra::		    success(Landscape* pParent) 
//This function provides the successional information for the tundra frame.  It is expected to return NULL
//if the frame type does not change, and a pointer to a new frame if a transition occurs.  The general
//model used is to check immediate post burn stuff first, then time dependant state changes, and then 
//general (long term) state changes. Specific algorithms are documented in the code
{
	//Check immediately after burn
	const int yearsSinceLastBurn = gYear - yearOfLastBurn;
	if (yearsSinceLastBurn == 1) {
        //This frame burned last year, so reset degree years to start tracking again.
        _yearEstablished	= gYear;
		_speciesSubCanopy	= gShrubTundraID;
		_basalArea	        = 0.;
		_yearOfEstablishment= -_history;
		_degrees		    = -1.;
	}

	///////////////////////////////////////////
	//> temporarily commenting out these lines with '>'.   Turning off time dependant transitions.
	//Check time dependant transitions
	//int yearsSinceEstablishment = gYear - _yearOfEstablishment;
	//if (yearsSinceEstablishment<=_history && yearsSinceEstablishment>=0 ) {              //If it has been cold and the trees are young, then kill the trees
	//    double	inc;
	//	if (-1 == _degrees) {											                //This is the first go so compute all preceeding years - I think this is always just two, but there is no extra overhead to have it general
	//		_degrees = 0.;
	//		//for (int i=0; i<=yearsSinceEstablishment; i++) {				//Integrate degree-timesteps
	//		//	inc = _pSeedEstParams[0] - pParent->cellClimate(i).Temp;
	//		//	_degrees += (inc>0) ? inc : 0.;
	//		//}
	//	} 
	//	else {															                //This is a standard run so just add the current year
	//		//inc = _pSeedEstParams[0] - pParent->cellClimate(0).Temp;
	//		//_degrees += (inc > 0) ? inc : 0.;
	//	}

	//	if (_degrees > _pSeedEstParams[1] * _history) {                   //Check to see if we kill the trees
	//		_yearOfEstablishment = -_history;
	//		_basalArea = 0.;
	//		_degrees = -1.;
	//	}
	//}

	//Compute seed source
	double params[3] = {0., _pSeedSource[0], _pSeedSource[1]};		                    //The first location will get set to the actual distance
	double seeds = pParent->neighborsSuccess(&Frame::queryReply, &FatTail, _seedRange, params);	//Find the neighborhood seed source - returns the weighted basal area
	params[0] = 0.;
 	seeds -= queryReply(pParent, FatTail (params));						                //Subtract off the contribution from this cell to give the sum of weighted BA/Ha of adjacent cells
	seeds *= _seedBasalArea;									                        //Compute the seed source from by multiplying by seeds/(BA/Ha)
	seeds /= _seedling;										                            //Factor in the seed to seedling and viability ratios

	///////////////////////////////////////////
	//> temporarily commenting out these lines with '>'.   Setting growth factor to zero.
	//Compute growth factor from Climate
	//>  SClimate climate = pParent->cellClimate(0);								            //Get the climate
	//>  double growthFactor = _pClimateGrowth[0] + _pClimateGrowth[1]*climate.Temp + _pClimateGrowth[2]*climate.Precip;	//and plug in into the regression to compute the increase in BA/HA from growth
	//>  growthFactor = growthFactor/1000. * _meanGrowth;			                        //Convert growth factor to fraction and multiply by mean annual growth
	double growthFactor = 0;

	//Find the new basal area for the plot
	if (_basalArea == 0 && seeds > 0) _yearOfEstablishment = gYear;		            //If we are just getting establishment, record the year
	//>  _basalArea += _basalArea * growthFactor * _pCalibrationFactor[0];		            //Portion of new basal area due to growth.  This is fitted in the Excel model to a cohort type model.
	_basalArea += seeds * _seedlingBasalArea * _pCalibrationFactor[1];		            //Portion of new basal area due to seedlings.  This is also fitted in the Excel model.

	//Transition if necessary
	if (_basalArea>=_tundraSpruceBasalArea) {
		if (_speciesSubCanopy==gWSpruceID) return new WSpruce(*this);
		else if (_speciesSubCanopy==gBSpruceID) return new BSpruce(*this);
		else {																			
			//Evaluate site as per rest of model (decid)
			const double probability = Site(_site,0.5);
			if (probability > GetNextRandom())
				return new BSpruce(*this);
			else
				return new WSpruce(*this);
		}
	}
	return NULL;
}


double ShrubTundra::		getInitialBasalArea() 
//Returns an initial basal area for the cell based on the distribution derived in ShrubTundra.doc 
//and BasalArea.xls!PDFTest.  Basically selects a basal area from a distribution which matches the
//expected growth curve generated by the Excel model.  Hence the inital basal area distribution 
//matches what one would expect at any given time.  This reduces initial condition purturbations.
{
	if (_ratioAK)
		return _ratioAK * ( pow(_tundraSpruceBasalArea/_ratioAK + 1.,(double) GetNextRandom()) - 1 );
	else
		return 0.;
}