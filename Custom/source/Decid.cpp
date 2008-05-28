//Decid.cpp


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Custom/Decid.h"
#include "Fresco/Custom/BSpruce.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Custom/CustomLandscape.h"
#include "Fresco/Custom/WSpruce.h"


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
}


void Decid::_Decid() 
//Local constructor - Initialize the local member variables and give the frame an initial age.
{
    if (!_isStaticSetupAlready) {
        throw Exception(Exception::UNKNOWN, "Static data members must be set before initializing species.");
    }

	_degrees = -1;

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
    if (-1==_yearEstablished) {
		_yearEstablished = _yearFrameEstablished = startAge(_speciesTrajectory);
    }
}


void Decid::setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
		_isFireProbAgeDependent = FRESCO->fif().bGet("Decid.FireProb.IsAgeDependent");
		if (_isFireProbAgeDependent) {
			if (3 != FRESCO->fif().pdGet("Decid.FireProb", _pAgeDependentFireParams))
				throw Exception(Exception::BADARRAYSIZE, "Expected array size of 3 for key: Decid.FireProb (because Decid.FireProb.IsAgeDependent is set to TRUE)");
		}
		else
	        _decidFireProb = FRESCO->fif().dGet("Tundra.FireProb");
		if (FRESCO->fif().CheckKey("Decid.IgnitionDepressor"))
			_ignitionDepressor = FRESCO->fif().dGet("Decid.IgnitionDepressor");
		else
			_ignitionDepressor = 1;
        _humanIgnitionsProb	    = FRESCO->fif().dGet("Decid.HumanFireProb");
        _decidHistory           = FRESCO->fif().nGet("Decid.History");
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("Tundra->Spruce.BasalArea");
        _pBSpruceStartAge       = FRESCO->getStartAgeParms("Decid.StartAge.BSpruce", &_bspruceStartAgeType);
        _pWSpruceStartAge       = FRESCO->getStartAgeParms("Decid.StartAge.WSpruce", &_wspruceStartAgeType);
        if (2 != FRESCO->fif().pdGet("Decid->Tundra.Parms", _pDecidTundraParams))  {
            throw Exception(Exception::BADARRAYSIZE, "Expected array size of 2 for key: Decid->Tundra.Parms");
        }

		//
		//Setup decid to bspruce and wspruce parameters per burn severity.
		//
		if (_pDecidToBSpruceParams == 0) { 
			_pDecidToBSpruceParams = new double*[5]; //for each burn severity.
			for (int i=0; i<5; i++)
				_pDecidToBSpruceParams[i]=new double[2];
		}
		for (int i=1; i<5; i++){
			const double* parms;
			std::string key("Decid->BSpruce.BurnSeverity["+ToS(i)+"]");
			if (2 != FRESCO->fif().pdGet(key.c_str(), parms)) {
				throw Exception(Exception::BADARRAYSIZE, "Expected array size of 2 for key: " + key);
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
			std::string key("Decid->WSpruce.BurnSeverity["+ToS(i)+"]");
			if (2 != FRESCO->fif().pdGet(key.c_str(), parms)) {
				throw Exception(Exception::BADARRAYSIZE, "Unexpected array size returned for Key: " + key);
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
	//if (_pDecidToBSpruceParams!=0) {
	//	for (int i=0; i<5; i++) delete[] _pDecidToBSpruceParams[i];
	//	delete[] _pDecidToBSpruceParams;
	//	_pDecidToBSpruceParams = 0;
	//}
}
void Decid::repStart()
//Set values to beginning of a rep.
{
}


void Decid::writeData (std::ostream &s, const int outFlags, const int doFormatting) const 
//Output the specific frame data.  Variable 1 reflects canopy and variable 2 reflects species trajectory.
{
	if (outFlags & out1) {
		if (!doFormatting)  s << "Traj=" << _speciesTrajectory << "\t";
        else                s << _speciesTrajectory << " ";
	}
    else {
    	bool showNoData = (outFlags & out1) || (outFlags & out2) || (outFlags & out3) || (outFlags & out4);
	    if (showNoData) {   //There was no data to output so display that info
            if (!doFormatting)  s << "NoData=" << 0. << "\t";
            else                s << 0. << " ";
        }
    }
}


Frame* Decid::success(Landscape* Parent) 
// Process succession: test for change to another frame type with a linear function 
// of years since last burn.  Parameters vary by speciesTrajectory and burn severity.
// Return NULL if no successional change.
{
	const int yearsSinceLastBurn = gYear-yearOfLastBurn;
	if (yearsSinceLastBurn == gTimeStep) {
		_yearEstablished = gYear;
		_speciesSubCanopy = gDecidID;
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
		return (int) (-GetNextRandom() * pParams[0]);
	} 
	else {
		const double random = GetNextRandom();
		int age = 0;
		while (random > pIntegral[age++]);
		return 1-age;   //Same as -(nAge - 1)
	}
}
