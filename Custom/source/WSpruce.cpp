//WSpruce.cpp
//This module contains the code relevant to the white spruce frame, constructor(s), destructor(s), the fire
//probability function and the success function which are pure virtual functions and therefore require
//definition, and any other worker functions which may be required for the function of the module.  


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Custom/WSpruce.h"
#include "Fresco/Custom/CustomLandscape.h"


//Declare static private members
bool			WSpruce::_isStaticSetupAlready = false;
double*			WSpruce::_pWSpruceFireParms = 0;
float			WSpruce::_decidFireProb;
double			WSpruce::_tundraSpruceBasalArea;
const double*	WSpruce::_pStartAgeParms;
double*			WSpruce::_pIntegral;
EStartAgeType	WSpruce::_startAgeType;


WSpruce::WSpruce (
        const int& rAge, 
		const bool& rIsTopoComplex, 
        const float& rSite, 
        const int& rYearOfLastBurn, 
		const int& rLastBurnSeverity,
        const double& rFireIgnitionFactor, 
        const double& rFireSensitivity, 
        const Species& rSpecSubCanopy)
    : Frame (rAge, rIsTopoComplex, rSite, rYearOfLastBurn, rLastBurnSeverity, rFireIgnitionFactor, rFireSensitivity, rSpecSubCanopy)
{ 
    _WSpruce(); 
}


WSpruce::WSpruce(const Frame& rFrame, int& rAge) 
    : Frame(rFrame)
{	
	_yearEstablished		= rAge;
	_yearFrameEstablished	= rAge;
	_WSpruce(); 
}


void WSpruce::_WSpruce() 
//Local constructor - Initialize the local member variables and give the frame an initial age.
{
    if (!_isStaticSetupAlready)
        throw Exception(Exception::UNKNOWN, "Static data members must be set before initializing species.");

	//Calc a starting age if not yet assigned.
	if (-1==_yearEstablished)	
    {
		if (_startAgeType==CONSTANT) {
			_yearEstablished = _yearFrameEstablished = (int)(-GetNextRandom() * _pStartAgeParms[0]);
		}
		else {
			const double random = GetNextRandom();
			int age = 0;
			while (random > _pIntegral[age++]);
			_yearEstablished = _yearFrameEstablished = 1 - age;
		}
	}
}


WSpruce::~WSpruce()
{
}


void WSpruce::setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
        _humanIgnitionsProb	    = FRESCO->fif().dGet("WSpruce.HumanFireProb");
        _decidFireProb          = FRESCO->fif().dGet("Decid.FireProb");
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("Tundra->Spruce.BasalArea");
        _pWSpruceFireParms      = FRESCO->getSpruceFireParms("WSpruce.FireParms");
        _pStartAgeParms         = FRESCO->getStartAgeParms("WSpruce.StartAge", &_startAgeType);

		if (WEIBULL==_startAgeType) {
			const int length = (int)(5*_pStartAgeParms[0]);
			_pIntegral = new double[length];
			Integrate (WeibullReliability, _pStartAgeParms, _pIntegral, 0, length-1, 1.);
			//Normalize so the integral is 1.
			for (int i=0; i<length; i++) _pIntegral[i] /= _pIntegral[length-1];
		}

        _isStaticSetupAlready = true;
	}
}


void WSpruce::clear()
//Clear existing run if any and return to before a run is specified.
{
	_isStaticSetupAlready   = false;
	_startAgeType			= CONSTANT;
	_decidFireProb			= 0.f;
	_tundraSpruceBasalArea	= 0.;
	if (_pWSpruceFireParms) delete[] _pWSpruceFireParms; 	_pWSpruceFireParms = 0;
	delete[] _pIntegral; _pIntegral = 0;
}


Frame* WSpruce::success(Landscape *Parent) 
//This function provides the successional information.  It is expected to return NULL if the frame type
//does not change, and a pointer to a new frame if a transition occurs.  The general model used is to 
//check immediate post burn stuff first, then time dependant state changes, and then general (long term)
//state changes.  In the case of spruce, it will always transition to deciduous immediately after fire,
//and that is the only transition that occurs.
{
	//Check immediately after burn
	if (gYear-yearOfLastBurn == gTimeStep)
		return new Decid(*this);
	return NULL;
}
