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
int			ShrubTundra::_spruceTransitionYear;
int			ShrubTundra::_tundraTransitionYear;
std::vector<double>	ShrubTundra::_rollingTempMean;
std::vector<double>	ShrubTundra::_rollingSWIMean;
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
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: ShrubTundra.FireProb (because Tundra.FireProb.IsAgeDependent is set to TRUE)");
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
	if (FRESCO->fif().CheckKey("ShrubTundra.SpruceTransitionYear")){
        	_spruceTransitionYear          = FRESCO->fif().nGet("ShrubTundra.SpruceTransitionYear");
	} else {
		_spruceTransitionYear		 = 0;
	}
	if (FRESCO->fif().CheckKey("ShrubTundra.TundraTransitionYear")){
        	_tundraTransitionYear          = FRESCO->fif().nGet("ShrubTundra.TundraTransitionYear");
	} else {
		_tundraTransitionYear		 = 0;
	}
        if (2 != FRESCO->fif().pdGet("ShrubTundra.SeedEstParms", _pSeedEstParams)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: ShrubTundra.SeedEstParms");
        }
        if (3 != FRESCO->fif().pdGet("ShrubTundra.ClimGrowth", _pClimateGrowth)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: ShrubTundra.ClimGrowth");
        }
        if (2 != FRESCO->fif().pdGet("ShrubTundra.CalFactor", _pCalibrationFactor)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: ShrubTundra.CalFactor");
        }
        if (2 != FRESCO->fif().pdGet("ShrubTundra.SeedSource", _pSeedSource)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: ShrubTundra.SeedSource");
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
		if (burnSeverity == MODERATE || burnSeverity == HIGH_LSS){
			//Reduce basal area by 50%
			_basalArea 		*= 0.5;
		} else if (burnSeverity == HIGH_HSS){
			//Reduce basal area to 0
			_basalArea	         = 0.;
			if (gYear >= _tundraTransitionYear){
				return new GraminoidTundra(*this);
			}
		} else if (burnSeverity == LOW){
			//Unchanged
			_basalArea               = _basalArea;
		}
		//if (burnSeverity == HIGH_LSS || burnSeverity == HIGH_HSS ){
	}
	float movingTempAverage = 0;
	float movingSWIAverage = 0;
	//Check to see if _rollingTempMean has been setup, or if this is the first pass
	if (_rollingTempMean.size() < 10){
		_rollingTempMean.push_back(pParent->cellTempByMonth(7));
	} else {
		_rollingTempMean.erase (_rollingTempMean.begin());	
		_rollingTempMean.push_back(pParent->cellTempByMonth(7));
	}
	for (int i = 0; i < _rollingTempMean.size(); i++){
		movingTempAverage += _rollingTempMean[i];
	}
	movingTempAverage /= 10.0;
	float summerWarmthIndex = 0;
	if (pParent->cellTempByMonth(3) > 0){ summerWarmthIndex += pParent->cellTempByMonth(3); }
	if (pParent->cellTempByMonth(4) > 0){ summerWarmthIndex += pParent->cellTempByMonth(4); }
	if (pParent->cellTempByMonth(5) > 0){ summerWarmthIndex += pParent->cellTempByMonth(5); }
	if (pParent->cellTempByMonth(6) > 0){ summerWarmthIndex += pParent->cellTempByMonth(6); }
	if (pParent->cellTempByMonth(7) > 0){ summerWarmthIndex += pParent->cellTempByMonth(7); }
	if (_rollingSWIMean.size() < 10){
                _rollingSWIMean.push_back(summerWarmthIndex);
        } else {
                _rollingSWIMean.erase (_rollingSWIMean.begin());
                _rollingSWIMean.push_back(summerWarmthIndex);
        }
	for (int i = 0; i < _rollingSWIMean.size(); i++){
                movingSWIAverage += _rollingSWIMean[i];
        }
	movingSWIAverage /= 10.0;
	if (gYear >= _spruceTransitionYear){
		if (movingTempAverage >= 10.0 && movingTempAverage <= 18.0){
			double params[3] = {0., _pSeedSource[0], _pSeedSource[1]};		                    //The first location will get set to the actual distance
			double seeds = pParent->neighborsSuccess(&Frame::queryReply, &FatTail, _seedRange, params);	//Find the neighborhood seed source - returns the weighted basal area
			params[0] = 0;
			seeds -= queryReply(pParent, FatTail (params));
			seeds *= _seedBasalArea;
			double modSeedling = 1;  // Modified seedling ratio based on Burn Severity
			if (yearsSinceLastBurn <= 5){
				if (burnSeverity == MODERATE || burnSeverity == HIGH_HSS){
					modSeedling = 0.5;
				} else if (burnSeverity == HIGH_LSS){
					modSeedling = 0.1;
				}
			}
			seeds /= (_seedling * modSeedling);
			if (_basalArea == 0 && seeds > 0) {
				_yearOfEstablishment = gYear; 
			}
			double gparams[3] = {movingTempAverage, 15., 2.};		                    //The first location will get set to the actual distance
			double modGrowth = NormDist(gparams);
			modGrowth *= 5;
			double baFromGrowth = 0;
			if (_basalArea > 0){
				baFromGrowth = -(_basalArea *_basalArea) * (0.00025) + (modGrowth * 0.2);
			}
			double baFromSeed = 0;
			if (seeds > 0.00001){
				baFromSeed = seeds * _seedlingBasalArea;
			}
			_basalArea += baFromGrowth + baFromSeed;
		} else {
			_basalArea = 0.0;
		}

		//Transition if necessary
		if (_basalArea >= FRESCO->fif().dGet("ShrubTundra.Spruce.EstBA")) {
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
