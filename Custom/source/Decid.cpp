//Decid.cpp


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Custom/Decid.h"
#include "Fresco/Custom/BSpruce.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Custom/CustomLandscape.h"
#include "Fresco/Custom/WSpruce.h"


//Declare static private members
bool			Decid::_isStaticSetupAlready    = false;
float			Decid::_decidFireProb;
int				Decid::_decidHistory;
double			Decid::_tundraSpruceBasalArea;
const double*	Decid::_pDecidTundraParams;
const double*	Decid::_pDecidBSpruceParams;
const double*	Decid::_pDecidWSpruceParams;
EStartAgeType	Decid::_bspruceStartAgeType;
EStartAgeType	Decid::_wspruceStartAgeType;
double*			Decid::_pBSpruceWeibullIntegral;
double*			Decid::_pWSpruceWeibullIntegral;
const double*	Decid::_pBSpruceStartAge;
const double*	Decid::_pWSpruceStartAge;



Decid::Decid (
        const int& rAge, 
        const double& rSite, 
        const int& rLastBurn, 
        const double& rFireIgnitionFactor, 
        const double& rFire_Sensitivity, 
        const Species& rSpecSubCanopy)
    : Frame (rAge, rSite, rLastBurn, rFireIgnitionFactor, rFire_Sensitivity, rSpecSubCanopy) 
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
        _decidFireProb          = FRESCO->fif().dGet("Decid.FireProb");
        _humanIgnitionsProb	    = FRESCO->fif().dGet("Decid.HumanFireProb");
        _decidHistory           = FRESCO->fif().nGet("Decid.History");
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("Tundra->Spruce.BasalArea");
        _pBSpruceStartAge       = FRESCO->getStartAgeParms("Decid.StartAge.BSpruce", &_bspruceStartAgeType);
        _pWSpruceStartAge       = FRESCO->getStartAgeParms("Decid.StartAge.WSpruce", &_wspruceStartAgeType);
        if (2 != FRESCO->fif().pdGet("Decid->Tundra.Parms", _pDecidTundraParams))  {
            throw Exception(Exception::BADARRAYSIZE, "Unexpected array size returned for Key: Decid->Tundra.Parms");
        }
        if (2 != FRESCO->fif().pdGet("Decid->BSpruce.Parms", _pDecidBSpruceParams)) {
            throw Exception(Exception::BADARRAYSIZE, "Unexpected array size returned for Key: Decid->BSpruce.Parms");
        }
        if (2 != FRESCO->fif().pdGet("Decid->WSpruce.Parms", _pDecidWSpruceParams)) {
            throw Exception(Exception::BADARRAYSIZE, "Unexpected array size returned for Key: Decid->WSpruce.Parms");
        }

		//If using weibull, calculate start age parameters for use in startAge().  Otherwise, constant will be used.
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
	_decidHistory				    = 0;
	_tundraSpruceBasalArea		    = 0;
	_bspruceStartAgeType			= CONSTANT;
	_wspruceStartAgeType			= CONSTANT;
	_pBSpruceWeibullIntegral		= 0;
	_pWSpruceWeibullIntegral		= 0;

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
// Process succession: change to another frame type.
// Return NULL if no successional change.
// The general approach is:
// 1 Keep a running sum of "degree years": the sum of temperature during the years since last burn up to a historical limit.
// 2 Transition if degree years exceeds a limit.
// 3 Otherwise test for a random transition using a linear function of years since last burn.
{
	//Check immediately after burn
	const int yearsSinceLastBurn = gYear-yearOfLastBurn;
	if (yearsSinceLastBurn == gTimeStep) {
    	//This frame burned last year, so reset degree years to start tracking again.
		_yearEstablished = gYear;														
		_speciesSubCanopy = gDecidID;
		_degrees = -1;
	}

    //Check time dependant transitions
	//if (yearsSinceLastBurn < _decidHistory) {
	//	double	inc;    //What does "inc" mean?
	//	if (-1 == _degrees) {
	//		_degrees = 0.;
	//		int history = yearsSinceLastBurn;
	//		if (yearsSinceLastBurn > gYear)     history = gYear;
	//		if (history >= _decidHistory)       history = _decidHistory-gTimeStep;            //Don't let history exceed _decidHistory.
	//		for (int i=0; i<=history/gTimeStep; i++) {
	//			inc = 0;//_pDecidTundraParams[0] - Parent->cellClimate(i).Temp;
	//			_degrees += (inc>0) ? inc : 0.;
	//		}
	//	} 
	//	else {
	//		//Update degree years with current year.
	//		inc = _pDecidTundraParams[0] - Parent->cellClimate(0).Temp;
	//		_degrees += (inc>0) ? inc : 0.;
	//	}

	//	//Succeed to tundra if degree years exceeds decid to tundra trigger.
	//	if (_degrees>_pDecidTundraParams[1] * (_decidHistory/gTimeStep))
	//		//Return a tundra frame.  Current sub canopy is passed so that tundra can succeed back.
	//		return new Tundra(gYear, _site, yearOfLastBurn, _speciesSubCanopy);
	//}

	//If no degree year succession occured, test for random succession.
	//Probability is a linear function of years since last burn.
	if (_speciesTrajectory == gBSpruceID) {
		//If succession is to be to BSpruce, use BSpruce parameters.
		const double prob = _pDecidBSpruceParams[0] * (gYear-_yearEstablished) + _pDecidBSpruceParams[1];
		if (prob>GetNextRandom())
			return new BSpruce(*this, _yearEstablished);
	} 
	else {
		//All other succession trajectories will lead to WSpruce.
		const double prob = _pDecidWSpruceParams[0] * (gYear-_yearEstablished) + _pDecidWSpruceParams[1];
		if (prob>GetNextRandom())
			return new WSpruce(*this, _yearEstablished);
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
