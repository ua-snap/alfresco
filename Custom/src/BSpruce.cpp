//BSpruce.cpp
//This module contains the code relevant to the black spruce frame, constructor(s), destructor(s), the fire
//probability function and the success function which are pure virtual functions and therefore require
//definition, and any other worker functions which may be required for the function of the module.  


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Custom/BSpruce.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Custom/CustomLandscape.h"


//Declare static members
bool			BSpruce::_isStaticSetupAlready = false;
double*			BSpruce::_pBSpruceFireParms = 0;
float			BSpruce::_decidFireProb;
float			BSpruce::_ignitionDepressor;
double			BSpruce::_tundraSpruceBasalArea;
const double*	BSpruce::_pStartAgeParms;
double*			BSpruce::_pIntegral;
EStartAgeType	BSpruce::_startAgeType;


BSpruce::BSpruce (
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
    _BSpruce(); 
}


BSpruce::BSpruce(const Frame& rFrame, int& rAge) 
    : Frame(rFrame) 
{	
	_yearEstablished		= rAge;
	_yearFrameEstablished	= rAge;
	_BSpruce(); 
}


void BSpruce::_BSpruce() 
//Local constructor - Initialize the static variables and local member variables and give the frame an
//initial age.
{
    if (!_isStaticSetupAlready)
        throw Exception(Exception::UNKNOWN, "Static data members must be set before initializing species.");

	//Calc a starting age if not yet assigned.
	if (gFirstYear-1 ==_yearEstablished) 
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
}


BSpruce::~BSpruce() 
{
}


void BSpruce::setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
        _humanIgnitionsProb     = FRESCO->fif().dGet("BSpruce.HumanFireProb");
        _decidFireProb		    = FRESCO->fif().dGet("Decid.FireProb");
		if (FRESCO->fif().CheckKey("BSpruce.IgnitionDepressor"))
			_ignitionDepressor = FRESCO->fif().dGet("BSpruce.IgnitionDepressor");
		else
			_ignitionDepressor = 1;
        _tundraSpruceBasalArea  = FRESCO->fif().dGet("Tundra->Spruce.BasalArea");
        _pBSpruceFireParms      = FRESCO->getSpruceFireParms("BSpruce.FireParms");
        _pStartAgeParms         = FRESCO->getStartAgeParms("BSpruce.StartAge", &_startAgeType);

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


void BSpruce::clear()
//Clear existing run if any and return to before a run is specified.
{
	_isStaticSetupAlready	= false;
	_startAgeType			= CONSTANT;
	_decidFireProb			= 0.;
	_ignitionDepressor		= 1;
	_tundraSpruceBasalArea	= 0;
	if (_pBSpruceFireParms) delete[] _pBSpruceFireParms; 	_pBSpruceFireParms = 0;
	delete[] _pIntegral; _pIntegral = 0;
}


Frame* BSpruce::success(Landscape *Parent) 
//This function provides the successional information.  It is expected to return NULL if the frame type
//does not change, and a pointer to a new frame if a transition occurs.  The general model used is to 
//check immediate post burn stuff first, then time dependant state changes, and then general (long term)
//state changes.  In the case of spruce, it will always transition to deciduous immediately after fire,
//and that is the only transition that occurs.
{
	//Check immediately after burn
	if (gYear-yearOfLastBurn == 1)
		return new Decid(*this);
	return NULL;
}
