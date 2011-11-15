//Grassland.cpp
//This module contains the code relevant to the grassland frame, constructor(s), destructor(s), the fire
//probability function and the success function which are pure virtual functions and therefore require
//definition, and any other worker functions which may be required for the function of the module.


#include "PreCompiled.h"
#include "Fresco.h"
#include "Grassland.h"
#include "CustomLandscape.h"


//Declare static private members
bool			Grassland::_isStaticSetupAlready = false;
double*			Grassland::_pGrasslandFireParms = 0;
float			Grassland::_decidFireProb;
float			Grassland::_ignitionDepressor;
double			Grassland::_tundraSpruceBasalArea;
const double*	Grassland::_pStartAgeParms;
double*			Grassland::_pIntegral;
EStartAgeType	Grassland::_startAgeType;


Grassland::Grassland (
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
    _Grassland(); 
}


Grassland::Grassland(const Frame& rFrame, int& rAge) 
    : Frame(rFrame)
{	
	_yearEstablished		= rAge;
	_yearFrameEstablished	= rAge;
	_Grassland(); 
}


void Grassland::_Grassland() 
//Local constructor - Initialize the local member variables and give the frame an initial age.
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
}


Grassland::~Grassland()
{
}


void Grassland::setStaticData()
{
	if (!_isStaticSetupAlready) 
    {
        _humanIgnitionsProb	    = FRESCO->fif().dGet("Grassland.HumanFireProb");
        _decidFireProb          = FRESCO->fif().dGet("Decid.FireProb");
		if (FRESCO->fif().CheckKey("Grassland.IgnitionDepressor"))
			_ignitionDepressor = FRESCO->fif().dGet("Grassland.IgnitionDepressor");
		else
			_ignitionDepressor = 1;
		_tundraSpruceBasalArea  = FRESCO->fif().dGet("Tundra->Spruce.BasalArea");
        _pGrasslandFireParms      = FRESCO->getSpruceFireParms("Grassland.FireParms");
        _pStartAgeParms         = FRESCO->getStartAgeParms("Grassland.StartAge", &_startAgeType);

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


void Grassland::clear()
//Clear existing run if any and return to before a run is specified.
{
	_isStaticSetupAlready   = false;
	_startAgeType			= CONSTANT;
	_decidFireProb			= 0.f;
	_ignitionDepressor		= 1;
	_tundraSpruceBasalArea	= 0.;
	if (_pGrasslandFireParms) delete[] _pGrasslandFireParms; 	_pGrasslandFireParms = 0;
	delete[] _pIntegral; _pIntegral = 0;
}


Frame* Grassland::success(Landscape *Parent) 
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
