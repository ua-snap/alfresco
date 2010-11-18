//Fresco.cpp


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/Except.h"
#include "Fresco/Foundation/Interface.h"
#include "Fresco/Foundation/Landscape.h"
#include "Fresco/Foundation/Fire.h"
#include "Poco/BasicEvent.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"
#include "Poco/File.h"
#include <sys/timeb.h>


//Global variables
std::string  FrescoFoundation_API   gWorkingDirectory;
std::string  FrescoFoundation_API   gInputBasePath;
std::string  FrescoFoundation_API   gOutputBasePath;
std::string  FrescoFoundation_API   gOutputDirectory;
int  FrescoFoundation_API           gNumRows			= 0;					//Number of rows in landscape.
int  FrescoFoundation_API           gNumCol				= 0;					//Number of cols in landscape.
int  FrescoFoundation_API           gMaxRep				= 0;					//Number of replications to run.
int  FrescoFoundation_API           gMaxYear			= 0;					//Number of years to simulate.
int  FrescoFoundation_API           gYear				= 0;					//Current year being simulated
int  FrescoFoundation_API           gRep				= 0;					//Current replication being simulated.
int  FrescoFoundation_API           gTimeStep			= 0;					//Time step
float  FrescoFoundation_API         gCellSize			= 0;					//Cell size
long  FrescoFoundation_API          gNumSpecies			= 0;
byte  FrescoFoundation_API          gNoVegID			= 255;
byte  FrescoFoundation_API          gTundraID			= 255;
byte  FrescoFoundation_API          gDecidID			= 255;
byte  FrescoFoundation_API          gWSpruceID			= 255;
byte  FrescoFoundation_API          gBSpruceID			= 255;
byte  FrescoFoundation_API          gGrasslandID		= 255;
EDetailLevel    FrescoFoundation_API gDetailLevel		= MINIMAL;
Fresco  FrescoFoundation_API        *FRESCO				= 0;
RasterIO FrescoFoundation_API       *gIO				= 0;
Climate FrescoFoundation_API        *gClimate			= 0;


Fresco::			Fresco(Landscape* pLandscape, bool isDebugOn)
{
    _isDebugOn  = isDebugOn;
    FRESCO		= this;
    _landscape	= pLandscape;
    clear();    //Set FRESCO to pre run state.
}


Fresco::			~Fresco()
{
    if(_landscape!=0) 
    {
        delete _landscape;
        WriteDebug("Landscape...\t[DELETE]\n");
    }
}
//
//static const std::string Fresco::fifName() 
//{ 
//	return Fresco::_fifName; 
//}
//


void Fresco::		clear()
//Clear existing run if any and return to before a run is specified.
{
	setState(CLEARING);
	
    _landscape->clear();  //Clear landscape first (it depends on the global settings below).
    //Clear FRESCO settings.	
    _isRunningFirstRep  = true;
    _isFireEnabled		= true;
    _randomSeed			= 0;
    _numGisHeaderRows	= 6;
    //Clear global settings.
    gNumRows			= 0;
    gNumCol				= 0;
    gCellSize			= 0;
    gMaxRep				= 0;
    gMaxYear			= 0;
    gTimeStep			= 0;
    gYear				= 0;
    gRep				= 0;
    gNumSpecies			= 0;
    gNoVegID			= 255;
    gTundraID			= 255;
    gDecidID			= 255;
    gWSpruceID			= 255;
    gBSpruceID			= 255;
    gGrasslandID		= 255;
    gWorkingDirectory	= "";
    gInputBasePath		= "";
    gOutputBasePath		= "";
    gOutputDirectory	= "";
    gDetailLevel		= MINIMAL;  	//Reset detail level last so that current detail level causes output during clearing.
	
	setState(CLEARED);
}


void Fresco::		setup(std::string basePath, std::string fifName, std::string outputTimestamp, long randSeed)
{
	setState(SETTING_UP);
	
    output("Reading FIF File.\n");
    _fif.Initialize(basePath, fifName);

    output("Loading General settings.\n");
    std::string temp = "";
    //Get class variables with parsed FIF values.
    gInputBasePath      = FormatDirectory(_fif.sGet("ClientInputBasePath"));
    gOutputBasePath     = FormatDirectory(_fif.sGet("ClientOutputBasePath"));
    gOutputDirectory    = GetFullPath(gOutputBasePath, outputTimestamp);
    EnsureDirectoryExists(gOutputDirectory, false);
    gMaxRep			    = _fif.nGet("MaxReps");
    gMaxYear            = _fif.nGet("MaxYears");
    gTimeStep			= _fif.nGet("TimeStep");
	_numGisHeaderRows	= _fif.nGet("NumHeader");
	gDetailLevel		= (temp=_fif.sGet("Output.DetailLevel"))=="MINIMAL" ? MINIMAL : (temp=="MODERATE" ? MODERATE : (temp=="MAXIMUM" ? MAXIMUM : throw Poco::Exception("Invalid input for Output.DetailLevel: "+temp)));
	try { //Initialize the random number generator.
        _randomSeed = SeedRandom(randSeed);
        ShowOutput(MODERATE, "\tRandom Seed " + ToS(_randomSeed));
    } catch (Exception& e) {throw Exception(Exception::INITFAULT,"Initializing random number generator failed.\n", e.message);}
    gNoVegID            = _fif.nGet("NoVeg"); 
        
    //Fire
    output("Loading Fire settings.\n");
    _isFireEnabled      = _fif.bGet("Fire.Enabled");
    Fire::setup();
	
    customSetup();

    output("Loading Landscape settings.\n");
    _landscape->setup();

    
	setState(SETUP);
}


void Fresco::       runRep(const int newRep)
{
    runRep(newRep, 0); 
}


void Fresco::       runRep(const int rep, const int yearResume)
{
	setState(SIMULATING);
	
    if (0 == yearResume) {
        //Begin new rep.
        gRep = rep;
        raiseBeforeRepStart.notify(this, gRep);
        _landscape->repStart();
    }
    //else, pick up in middle of rep after last completed year.

    for (gYear=yearResume; gYear<=gMaxYear; gYear += gTimeStep) {
        raiseBeforeYearStart.notify(this, gYear);
        _landscape->yearStart();
        raiseAfterYearStart.notify(this, gYear);
        
        if (gYear>0) _landscape->succession();
        if (_isFireEnabled) _landscape->doIgnitions();
        
        raiseBeforeYearEnd.notify(this, gYear);
        _landscape->yearEnd();
        raiseAfterYearEnd.notify(this, gYear);
        
        if (_stopped) {
        	setState(STOPPED);
        	break; //Possible Stop point.
        }
    }

    if (!_stopped) {
        //Finish rep.
        raiseBeforeRepEnd.notify(this, gRep);
        _landscape->repEnd();
        _isRunningFirstRep = false;
        raiseAfterRepEnd.notify(this, gRep);  //Possible Stop point.
    }
}


void Fresco::       runEnd()
{
    raiseBeforeRunEnd.notify(this, ""); 
    _landscape->runEnd();
    setState(COMPLETED);
    raiseAfterRunEnd.notify(this, "");
}


void Fresco::		output(const std::string Output)
//Pass output to any subscribers of this event.
{
    raiseOutput.notify(this, Output);
}


void Fresco::		outputError(const std::string message)
//Pass output to cout and any subscribers of this event.
{
    std::cout << std::endl << message << std::endl;
    raiseErrorOutput.notify(this, message);
}


double* Fresco::	getSpruceFireParms(const std::string key)
{
    const double* pTemp;
    if (_fif.pdGet(key.c_str(), pTemp) != 3) 
        throw Exception(Exception::BADARRAYSIZE, (std::string("Unexpected array size returned for Key: ") + key).c_str());
    
    //Transfer "const double*" to "double*"
    double* pReturn;
    pReturn = new double[3];
    pReturn[0] = pTemp[0] - _fif.dGet("Decid.FireProb");
    pReturn[1] = pTemp[1];
    pReturn[2] = pTemp[2];
    
    //delete[] pTemp; Don't delete, this points to the value stored in the CAssoc of _fif.
    return pReturn;
}


const double* Fresco::   getStartAgeParms(const std::string key, EStartAgeType* type)
{
    const double* params;
    int count=_fif.pdGet(key.c_str(),params);
    
    //Set start age type.
    if (1==count)      
        *type=CONSTANT;
    else if (2==count) 
        *type=WEIBULL;
    else
        throw Exception(Exception::BADARRAYSIZE,(std::string("Unexpected array size returned for Key: ") + key).c_str());
    
    return params;
}


