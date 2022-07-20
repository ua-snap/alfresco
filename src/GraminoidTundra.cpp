//GraminoidTundra.cpp

#include "GraminoidTundra.h"
#include "CustomLandscape.h"

//Declare static private members
bool			GraminoidTundra::_isStaticSetupAlready		= false;
bool			GraminoidTundra::_isFireProbAgeDependent;
bool			GraminoidTundra::_isInoculumEnabled = false;
double*	GraminoidTundra::_pAgeDependentFireParams;		
float			GraminoidTundra::_fireProb;
float			GraminoidTundra::_ignitionDepressor;
double			GraminoidTundra::_seedRange;
double*	GraminoidTundra::_pSeedSource;
double			GraminoidTundra::_seedBasalArea;
double			GraminoidTundra::_seedlingBasalArea;
int				GraminoidTundra::_history;
double*	GraminoidTundra::_pSeedEstParams;
double			GraminoidTundra::_meanGrowth;
double*	GraminoidTundra::_pClimateGrowth;
double*	GraminoidTundra::_pCalibrationFactor;
double			GraminoidTundra::_seedling;
double			GraminoidTundra::_ratioAK = 0.;
double			GraminoidTundra::_tundraSpruceBasalArea;
double*	GraminoidTundra::_pStartAgeParms;
double*			GraminoidTundra::_pIntegral;
int			GraminoidTundra::_spruceTransitionYear;
int			GraminoidTundra::_tundraTransitionYear;
std::vector<double>	GraminoidTundra::_rollingTempMean;
std::vector<double>	GraminoidTundra::_rollingSWIMean;
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
	_basalArea = rFrame.basalArea();
	_inoculumScore = rFrame.inoculumScore();
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
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["enableInoculum"])){
		_isInoculumEnabled = FRESCO->fif().root["Vegetation"]["enableInoculum"].asBool();
	}
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Inoculum"])){
		_inoculumMax = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Inoculum"].asDouble();
	} else {
		_inoculumMax = 1.0;
	}
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["InoculumReturn"])){
		_inoculumReturn= FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["InoculumReturn"].asDouble();
	} else {
		_inoculumReturn = 1.0;
	}
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["ShrubTundra"]["InoculumBA"])){
		_inoculumBA = FRESCO->fif().root["Vegetation"]["ShrubTundra"]["InoculumBA"].asDouble();
	} else {
		_inoculumBA = 1.0;
	}
	_inoculumScore = _inoculumMax;
}


GraminoidTundra::				~GraminoidTundra() 
{
}


void GraminoidTundra::           setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
        _humanIgnitionsProb	    = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["HumanFireProb"].asDouble();
		_isFireProbAgeDependent = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["FireProb.IsAgeDependent"].asBool();
		if (_isFireProbAgeDependent) {
			if (3 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["FireProb"], _pAgeDependentFireParams))
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: GraminoidTundra.FireProb (because Tundra.FireProb.IsAgeDependent is set to TRUE)");
		}
		else
	        _fireProb = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["FireProb"].asDouble();
		if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["IgnitionDepressor"]))
			_ignitionDepressor = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["IgnitionDepressor"].asDouble();
		else
			_ignitionDepressor = 1;
        _history			    = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["History"].asInt();
        _seedRange		        = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["SeedRange"].asDouble();
        _seedBasalArea	        = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Seed.BasalArea"].asDouble();
        _seedling			    = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Seedling"].asDouble();
        _seedlingBasalArea      = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["SeedlingBA"].asDouble();
        _tundraSpruceBasalArea  = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Spruce.BasalArea"].asDouble();
        _pStartAgeParms         = FRESCO->getStartAgeParms(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["StartAge"], &_startAgeType);
        _meanGrowth             = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["MeanGrowth"].asDouble();

	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["SpruceTransitionYear"])){
		_spruceTransitionYear 	= FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["SpruceTransitionYear"].asInt();
	} else {
		_spruceTransitionYear		= 0;
	}	
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["TundraTransitionYear"])){
		_tundraTransitionYear 	= FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["TundraTransitionYear"].asInt();
	} else {
		_tundraTransitionYear		= 0;
	}	
        if (2 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["SeedEstParms"], _pSeedEstParams)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: GraminoidTundra.SeedEstParms");
        }
        if (3 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["ClimGrowth"], _pClimateGrowth)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: GraminoidTundra.ClimGrowth");
        }
        if (2 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["CalFactor"], _pCalibrationFactor)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: GraminoidTundra.CalFactor");
        }
        if (2 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["SeedSource"], _pSeedSource)) {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: GraminoidTundra.SeedSource");
        }

		//Calculate _ratioAK for use in getInitialBasalAreaI()
        double spruceEstBasalAarea	= FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Spruce.EstBA"].asDouble();
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
	if (FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Transitions"].asBool()){
	//Check immediately after burn
	double swi = FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["SummerWarmthIndex"].asDouble();
	const int yearsSinceLastBurn = gYear - yearOfLastBurn;
	if (yearsSinceLastBurn == 1) {
        //This frame burned last year, so reset degree years to start tracking again.
        	_yearEstablished	= gYear;
		_speciesSubCanopy	= gGraminoidTundraID;
		if (burnSeverity == HIGH_HSS){
			//Reduce basal area to 0
			_basalArea	         = 0;
			//Reduce inoculum to 20%
			if (_isInoculumEnabled){
				_inoculumScore *= 0.2;
			}
		} else if (burnSeverity == HIGH_LSS){
			//Reduce basal area by 50%
			_basalArea 		*= 0.5;
			//Reduce inoculum to 50%
			if (_isInoculumEnabled){
				_inoculumScore *= 0.5;
			}
		} else if (burnSeverity == MODERATE){
			//Reduce basal area by 50%
			_basalArea 		*= 0.5;

		} else if (burnSeverity == LOW){
			//Unchanged
			_basalArea               = _basalArea;
		}
		_yearOfEstablishment= -_history;
		_degrees		    = -1.;
	}
	
	if (_isInoculumEnabled){
		if (_inoculumScore < _inoculumMax){
			_inoculumScore += _inoculumReturn;
			if (_inoculumScore > _inoculumMax){
				_inoculumScore = _inoculumMax;
			}
		}
	}
	double movingTempAverage = 0;
	double movingSWIAverage = 0;
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
	if (pParent->cellTempByMonth(8) > 0){ summerWarmthIndex += pParent->cellTempByMonth(8); }
	if (pParent->cellTempByMonth(9) > 0){ summerWarmthIndex += pParent->cellTempByMonth(9); }
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
	if (gYear >= _tundraTransitionYear && _tundraTransitionYear > 0){
		if (movingTempAverage >= 10.0 && _rollingTempMean.size() == 10){
			if (movingSWIAverage > swi){
				if (yearsSinceLastBurn > 32 && yearsSinceLastBurn <= 52 && yearOfLastBurn >= 1){
					if (GetNextRandom() < 0.05){
						return new ShrubTundra(*this);
					}
				} else {
					if (GetNextRandom() < 0.01){
						return new ShrubTundra(*this);
					}
				}
			}
		}
	}
	if (_rollingTempMean.size() > 10){
		std::cout <<"ERROR"<<std::endl;
	}
	if (gYear >= _spruceTransitionYear && _spruceTransitionYear > 0){
		if (movingTempAverage >= 10.0 && movingTempAverage <= 20.0){
			double params[3] = {0., _pSeedSource[0], _pSeedSource[1]};		                    //The first location will get set to the actual distance
			double seeds = 0;
			params[0] = 0;
			if (_isInoculumEnabled){
				if (GetNextRandom() <= _inoculumScore){
					seeds = pParent->neighborsSuccess(&Frame::queryReply, &FatTail, _seedRange, params);	//Find the neighborhood seed source - returns the weighted basal area
					seeds -= queryReply(pParent, FatTail (params));
					seeds *= _seedBasalArea;
				}

			} else {
					seeds = pParent->neighborsSuccess(&Frame::queryReply, &FatTail, _seedRange, params);	//Find the neighborhood seed source - returns the weighted basal area
					seeds -= queryReply(pParent, FatTail (params));
					seeds *= _seedBasalArea;
			}


			double modSeedling = 1;  // Modified seedling ratio based on Burn Severity
			if (yearsSinceLastBurn <= 5){  // Seedling establishment is modified by recent burnSeverity
				if (burnSeverity == MODERATE || burnSeverity == HIGH_LSS){ //High severity results in higher establishment rates
					modSeedling = 0.5;
				} else if (burnSeverity == HIGH_HSS){
					modSeedling = 0.1;
				}
			}
			seeds /= (_seedling * modSeedling);
			if (_basalArea == 0 && seeds > 0) {
				_yearOfEstablishment = gYear; 
			}
			double gparams[3] = {movingTempAverage, 15., 2.};
			double modGrowth = NormDist(gparams);
			modGrowth *= 5;
			if (_isInoculumEnabled){
				modGrowth *= _inoculumScore;
			}
			double baFromGrowth = 0;
			if (_basalArea > 0){
				baFromGrowth = -(_basalArea *_basalArea) * (0.00025) + (modGrowth * 0.2);
				if (_isInoculumEnabled && _basalArea < _inoculumBA){
					baFromGrowth *= _inoculumScore;
				}
			}
			double baFromSeed = 0;
			if (seeds > 0.00001){
				baFromSeed = seeds * _seedlingBasalArea;
			}
			_basalArea += baFromGrowth + baFromSeed;
		}
		//Transition if necessary
		if (_basalArea >= FRESCO->fif().root["Vegetation"]["GraminoidTundra"]["Spruce.EstBA"].asDouble()) {
			return new WSpruce(*this);
		}
	}
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
