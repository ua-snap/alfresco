//Decid.cpp


#include "PreCompiled.h"
#include "Decid.h"
#include "BSpruce.h"
#include "Fresco.h"
#include "RasterIO.h"
#include "CustomLandscape.h"
#include "WSpruce.h"


//Declare static private members
bool			Decid::_isStaticSetupAlready    = false;
bool			Decid::_isFireProbAgeDependent;
const double*	Decid::_pAgeDependentFireParams;
float			Decid::_decidFireProb;
float			Decid::_ignitionDepressor;
int				Decid::_decidHistory;
double			Decid::_tundraSpruceBasalArea;
const double*	Decid::_pDecidTundraParams;
double**		Decid::_pDecidToBSpruceParams = 0;
double**		Decid::_pDecidToWSpruceParams = 0;
EStartAgeType	Decid::_bspruceStartAgeType;
EStartAgeType	Decid::_wspruceStartAgeType;
double*			Decid::_pBSpruceWeibullIntegral;
double*			Decid::_pWSpruceWeibullIntegral;
const double*	Decid::_pBSpruceStartAge;
const double*	Decid::_pWSpruceStartAge;
int				Decid::_yearsOfGrasslandCheck = 0;
const double*	Decid::_pGrasslandThresholds = 0;
const double*	Decid::_pGrassClimateParams = 0;
std::list<int>	Decid::_grassTempMonths;
std::list<int>	Decid::_grassPrecipMonths;


Decid::Decid (
        const int& rAge, 
        const bool& rIsTopoComplex, 
        const double& rSite, 
        const int& rYearOfLastBurn, 
		const int& rLastBurnSeverity,
        const double& rFireIgnitionFactor, 
        const double& rFire_Sensitivity, 
        const Species& rSpecSubCanopy)
    : Frame (rAge, rIsTopoComplex, rSite, rYearOfLastBurn, rLastBurnSeverity, rFireIgnitionFactor, rFire_Sensitivity, rSpecSubCanopy) 
{
    _Decid();
}


Decid::Decid(const Frame& rFrame) 
    : Frame(rFrame)
{	
	_yearEstablished		= gYear;
	_yearFrameEstablished	= gYear;
	_Decid(); 
	_wasGrassland = (rFrame.type() == gGrasslandID);
}


void Decid::_Decid() 
//Local constructor - Initialize the local member variables and give the frame an initial age.
{
    if (!_isStaticSetupAlready) {
        throw SimpleException(SimpleException::UNKNOWN, "Static data members must be set before initializing species.");
    }

	_degreesForGrassland = 0;
	_wasGrassland = false;

	//Pick a spruce trajectory at the frame initiation so we do not have a probability bias - use replacement stategy for now. OLD TODO add the aspect stuff back in
	if (_speciesSubCanopy==gBSpruceID) {
		_speciesTrajectory	= gBSpruceID;
	} 
	else if (_speciesSubCanopy==gWSpruceID) {
		_speciesTrajectory	= gWSpruceID;
	} 
	else {
		const double probability = Site(_site,0.5);
		if (probability > GetNextRandom())
			_speciesTrajectory = gBSpruceID;
		else
			_speciesTrajectory = gWSpruceID;
	}

	//If we don't have an age assigned yet, calculate it here.
    if (gFirstYear-1==_yearEstablished) {
		_yearEstablished = _yearFrameEstablished = startAge(_speciesTrajectory);
    }
}


void Decid::setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
		_isFireProbAgeDependent = FRESCO->fif().bGet("Decid.FireProb.IsAgeDependent");
		if (_isFireProbAgeDependent) {
			if (3 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["Decid"]["FireProb"], _pAgeDependentFireParams))
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 3 for key: Decid.FireProb (because Decid.FireProb.IsAgeDependent is set to TRUE)");
		}
		else
	        _decidFireProb = FRESCO->fif().root["Vegetation"]["Decid"]["FireProb"].asDouble();
		if (FRESCO->fif().CheckKey(FRESCO->fif().root["Vegetation"]["Decid"]["IgnitionDepressor"]))
			_ignitionDepressor = FRESCO->fif().root["Vegetation"]["Decid"]["IgnitionDepressor"].asDouble();
		else
			_ignitionDepressor = 1;
        _humanIgnitionsProb	    = FRESCO->fif().root["Vegetation"]["Decid"]["HumanFireProb"].asDouble();
        _decidHistory           = FRESCO->fif().root["Vegetation"]["Decid"]["History"].asInt();
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("Tundra->Spruce.BasalArea");
        _pBSpruceStartAge       = FRESCO->getStartAgeParms(FRESCO->fif().root["Vegetation"]["Decid"]["StartAge.BSpruce"], &_bspruceStartAgeType);
        _pWSpruceStartAge       = FRESCO->getStartAgeParms(FRESCO->fif().root["Vegetation"]["Decid"]["StartAge.WSpruce"], &_wspruceStartAgeType);
        if (2 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["Decid"]["Tundra.Parms"], _pDecidTundraParams))  {
            throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: Decid->Tundra.Parms");
        }
		
		//
		// Setup decid to grassland parameters
		//
		if (usingGrassland())
		{
			_yearsOfGrasslandCheck = FRESCO->fif().root["Vegetation"]["Decid"]["Grassland.History"].asInt();

			if (!IsNodata(gGrasslandID))
			{
				// Add months needed for Decid=>Grassland succession.
				_grassTempMonths.clear();
				_grassPrecipMonths.clear();
				const int* pPrecipMonths;
				const int* pTempMonths;

				int numMonths = FRESCO->fif().pnGet(FRESCO->fif().root["Vegetation"]["Decid"]["Grassland.TempMonths"], pTempMonths);
				if (numMonths > 12)
					throw SimpleException(SimpleException::BADARRAYSIZE, "Expected up to 12 values in the array for the key, Grassland.TempMonths. There are only 12 months in a year.");

				for (int i=0; i<numMonths; i++) {
					gClimate->tempMonths.push_back(pTempMonths[i]);
					_grassTempMonths.push_back(pTempMonths[i]);
				}

				numMonths = FRESCO->fif().pnGet(FRESCO->fif().root["Vegetation"]["Decid"]["Grassland.PrecipMonths"], pPrecipMonths);
				if (numMonths > 12)
					throw SimpleException(SimpleException::BADARRAYSIZE, "Expected up to 12 values in the array for the key, Grassland.PrecipMonths. There are only 12 months in a year.");

				for (int i=0; i<numMonths; i++) {
					gClimate->precipMonths.push_back(pPrecipMonths[i]);
					_grassPrecipMonths.push_back(pPrecipMonths[i]);
				}
			}
			size_t expectedSize = 2 + _grassTempMonths.size() + _grassPrecipMonths.size();
			if (expectedSize != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["Decid"]["Grassland.ClimateWeight"], _pGrassClimateParams))
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of "+ToS(expectedSize)+" for key: Decid->Grassland.ClimateWeight = {Intercept,IfFlatInterceptAdjusment, [a temp multiplier per month in Grassland.TempMonths], [a precip multiplier per month in Grassland.PrecipMonths]}");

			if (6 != FRESCO->fif().pdGet(FRESCO->fif().root["Vegetation"]["Decid"]["Grassland.ClimateThreshholds"], _pGrasslandThresholds))
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 6 for key: Decid->Grassland.ClimateThreshholds = {Low, Moderate, High_LSS, High_HSS, Low_And_WasGrassland, Moderate_And_WasGrassland}");
		}

		//
		// Setup decid to bspruce and wspruce parameters per burn severity.
		//
		if (_pDecidToBSpruceParams == 0) { 
			_pDecidToBSpruceParams = new double*[5]; //for each burn severity.
			for (int i=0; i<5; i++)
				_pDecidToBSpruceParams[i]=new double[2];
		}
		for (int i=1; i<5; i++){
			const double* parms;
			std::string key_burnsev("BSpruce.BurnSeverity["+ToS(i)+"]");
			Json::Value key(FRESCO->fif().root["Vegetation"]["Decid"][key_burnsev.c_str()]);
			if (2 != FRESCO->fif().pdGet(key, parms)) {
				throw SimpleException(SimpleException::BADARRAYSIZE, "Expected array size of 2 for key: " + key_burnsev);
			}
			_pDecidToBSpruceParams[i][0] = parms[0];
			_pDecidToBSpruceParams[i][1] = parms[1];
		}
		if (_pDecidToWSpruceParams == 0) { 
			_pDecidToWSpruceParams = new double*[5]; //for each burn severity.
			for (int i=0; i<5; i++)
				_pDecidToWSpruceParams[i]=new double[2];
		}
		for (int i=1; i<5; i++){
			const double* parms;
			std::string key_burnsev("WSpruce.BurnSeverity["+ToS(i)+"]");
			Json::Value key(FRESCO->fif().root["Vegetation"]["Decid"][key_burnsev.c_str()]);
			if (2 != FRESCO->fif().pdGet(key, parms)) {
				throw SimpleException(SimpleException::BADARRAYSIZE, "Unexpected array size returned for Key: " + key_burnsev);
			}
			_pDecidToWSpruceParams[i][0] = parms[0];
			_pDecidToWSpruceParams[i][1] = parms[1];
		}

		//
		// If using weibull, calculate start age parameters for use in startAge().  Otherwise, constant will be used.
		//
		if (_bspruceStartAgeType==WEIBULL) {						
			//Compute the Weibull pIntegral
			const int length = (int)(5*_pBSpruceStartAge[0]);
			_pBSpruceWeibullIntegral = new double[length];
			Integrate (WeibullReliability, _pBSpruceStartAge, _pBSpruceWeibullIntegral, 0, length-1, 1.);
			for (int i=0; i<length; i++) _pBSpruceWeibullIntegral[i] /= _pBSpruceWeibullIntegral[length-1];  			//Standardize so the pIntegral is 1
		}
		if (_wspruceStartAgeType==WEIBULL) {
			const int length = (int)(5*_pWSpruceStartAge[0]);
			_pWSpruceWeibullIntegral = new double[length];
			Integrate(WeibullReliability, _pWSpruceStartAge, _pWSpruceWeibullIntegral, 0, length-1, 1.);
			for (int i=0; i<length; i++) _pWSpruceWeibullIntegral[i] /= _pWSpruceWeibullIntegral[length-1];
		}

        _isStaticSetupAlready   = true;
	}
}


void Decid::clear()
//Clear existing run if any and return to before a run is specified.
{
	_isStaticSetupAlready			= false;
	_decidFireProb				    = 0.;
	_ignitionDepressor				= 1;
	_decidHistory				    = 0;
	_tundraSpruceBasalArea		    = 0;
	_bspruceStartAgeType			= CONSTANT;
	_wspruceStartAgeType			= CONSTANT;
	_pBSpruceWeibullIntegral		= 0;
	_pWSpruceWeibullIntegral		= 0;
	_yearsOfGrasslandCheck			= 0;
}
void Decid::repStart()
//Set values to beginning of a rep.
{
}



unsigned char	Decid::getAsByte(RasterIO::ALFMapType mapType)
{
	switch(mapType)
	{
	case RasterIO::DECID_SPECIES_TRAJECTORY:
		return _speciesTrajectory;
		break;
	default:
		throw Poco::Exception("This frame type ("+ToS((int)type())+") does not support the map type ("+ RasterIO::getMapTypeAsString(mapType) + ")");
	}
}


Frame* Decid::success(Landscape* l) 
// Process succession: test for change to another frame type with a linear function 
// of years since last burn.  Parameters vary by speciesTrajectory and burn severity.
// Return NULL if no successional change.
{
	const int yearsSinceLastBurn = gYear-yearOfLastBurn;
	if (yearsSinceLastBurn == 1) {
		// this is the first succession after a burn.
		_yearEstablished = gYear;
		_speciesSubCanopy = gDecidID;
		_degreesForGrassland = 0;
	}

	if (burnSeverity==0) return NULL;  // no transition values are given for no burn.  But with a correct BurnSeverityInputFile, no Decid should ever have burnSeverity==0;

	const float test = GetNextRandom();
	if (_speciesTrajectory == gWSpruceID)
	{
		const float a = _pDecidToWSpruceParams[burnSeverity][0];
		const float b = _pDecidToWSpruceParams[burnSeverity][1];
		const float prob = a * (gYear-_yearEstablished) + b;
		if (test < prob)
			return new WSpruce(*this, _yearEstablished);
	}
	else if (_speciesTrajectory == gBSpruceID)
	{
		if (usingGrassland() && age() <= _yearsOfGrasslandCheck)
		{
			// Build equation based on number of months for temp and precip.
			int i=0;
			bool foundNodata = false;
			float v=0; //climate value
			// ...y-intercept
			float r = _pGrassClimateParams[0]; // 'r' for regression result
			// ...if flat, apply y-intercept adjustment
			if (!hasComplexTopo()) 
				r += _pGrassClimateParams[1];
			// ...temperature months
			std::list<int>::iterator m;
			for (m=_grassTempMonths.begin(); m!=_grassTempMonths.end(); m++, i++) {
				v = l->cellTempByMonth(*m);
				if (!foundNodata) 
					foundNodata = IsNodata(v);
				r += v * _pGrassClimateParams[2 + i];				
			}
			// ...precipitation months
			for (m=_grassPrecipMonths.begin(); m!=_grassPrecipMonths.end(); m++, i++) {
				v = l->cellPrecipByMonth(*m);
				if (!foundNodata) 
					foundNodata = IsNodata(v);
				r += v * _pGrassClimateParams[2 + i]; // i continues incrementing from temp loop		
			}
			// ...if nodata wasn't found, apply the regression result
			if (!foundNodata)
				_degreesForGrassland += r;

			//_degreesForGrassland += l->cellTempByMonth(4)   * _pGrassClimateParams[T4];
			//_degreesForGrassland += l->cellTempByMonth(5)   * _pGrassClimateParams[T5];
			//_degreesForGrassland += l->cellTempByMonth(6)   * _pGrassClimateParams[T6];
			//_degreesForGrassland += l->cellTempByMonth(7)   * _pGrassClimateParams[T7];
			//_degreesForGrassland += l->cellPrecipByMonth(3) * _pGrassClimateParams[P3];
			//_degreesForGrassland += l->cellPrecipByMonth(4) * _pGrassClimateParams[P4];
			//_degreesForGrassland += l->cellPrecipByMonth(5) * _pGrassClimateParams[P5];
			//_degreesForGrassland += l->cellPrecipByMonth(6) * _pGrassClimateParams[P6];
			//_degreesForGrassland += l->cellPrecipByMonth(7) * _pGrassClimateParams[P7];

			float threshold = 0;
			if (_wasGrassland && burnSeverity == LOW)
				threshold = _pGrasslandThresholds[G_LOW_AND_WAS_GRASSLAND];
			else if (_wasGrassland && burnSeverity == MODERATE) 
				threshold = _pGrasslandThresholds[G_MODERATE_AND_WAS_GRASSLAND];
			else if (burnSeverity == LOW)
				threshold = _pGrasslandThresholds[G_LOW];
			else if (burnSeverity == MODERATE)
				threshold = _pGrasslandThresholds[G_MODERATE];
			else if (burnSeverity == HIGH_LSS)
				threshold = _pGrasslandThresholds[G_HIGH_LSS];
			else if (burnSeverity == HIGH_HSS)
				threshold = _pGrasslandThresholds[G_HIGH_HSS];

			if (_degreesForGrassland > threshold)
				return new Grassland(*this, _yearEstablished);
		}

		const float a = _pDecidToBSpruceParams[burnSeverity][0];
		const float b = _pDecidToBSpruceParams[burnSeverity][1];
		const float prob = a * (gYear-_yearEstablished) + b;
		if (test < prob)
			return new BSpruce(*this, _yearEstablished);
	}
	return NULL;
}



int Decid::startAge(Species speciesTrajectory) 
//This function computes a stochastically generated starting age for this species.  If only one parameter is
//specified in the FIF file, this will come from a constant distribution, otherwise (two parameters) it will
//be drawn from a Weibull distribution.
{
	const double* pParams;
    double* pIntegral = NULL;
	EStartAgeType startAgeType;

	//Check the trajectory and assign general pointers.  Load the data if it hasn't already been done and compute the Weibull pIntegral if necessary.
	if (speciesTrajectory==gBSpruceID) {
		//Assign a general pointer so the rest of the code can be common
		pParams = _pBSpruceStartAge;									
		startAgeType = _bspruceStartAgeType;
		pIntegral = _pBSpruceWeibullIntegral;
	} 
	else {
		pParams = _pWSpruceStartAge;
		startAgeType = _wspruceStartAgeType;
		pIntegral = _pWSpruceWeibullIntegral;
	}

	//Evaluate and return value
	if (startAgeType == CONSTANT) {
		return gFirstYear - (int)(GetNextRandom() * pParams[0]);
	} 
	else {
		const double random = GetNextRandom();
		int age = 0;
		while (random > pIntegral[age++]);
		return gFirstYear + 1-age;   //Same as -(nAge - 1)
	}
}
