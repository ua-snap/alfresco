/**
 * @file
 *
 */


#include "PreCompiled.h"
#include "Fresco.h"
#include "Except.h"
#include "Interface.h"
#include "Landscape.h"
#include "Fire.h"
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
int  FrescoFoundation_API           gYOffset			= 0;					//Number of rows from top of input files to start reading in landscape.
int  FrescoFoundation_API           gXOffset			= 0;					//Number of cols from left of input files to start reading in landscape.
int  FrescoFoundation_API           gYSize			= 0;					//Number of rows in landscape.
int  FrescoFoundation_API           gXSize				= 0;					//Number of cols in landscape.
int  FrescoFoundation_API           gMaxRep				= 0;					//Number of replications to run.
int  FrescoFoundation_API           gFirstYear			= 0;					//First year to simulate.
int  FrescoFoundation_API           gLastYear			= 0;					//Last year to simulate.
int  FrescoFoundation_API           gYear				= 0;					//Current year being simulated
int  FrescoFoundation_API           gRep				= 0;					//Current replication being simulated.
float  FrescoFoundation_API         gCellSize			= 0;					//Cell size
long  FrescoFoundation_API          gNumSpecies			= 0;
byte  FrescoFoundation_API          gNoVegID			= 255;
byte  FrescoFoundation_API          gTundraID			= 255;
byte  FrescoFoundation_API          gBarrenLichenMossID			= 255;
byte  FrescoFoundation_API          gTemperateRainforestID			= 255;
byte  FrescoFoundation_API          gShrubTundraID			= 255;
byte  FrescoFoundation_API          gGraminoidTundraID			= 255;
byte  FrescoFoundation_API          gWetlandTundraID			= 255;
byte  FrescoFoundation_API          gDecidID			= 255;
byte  FrescoFoundation_API          gWSpruceID			= 255;
byte  FrescoFoundation_API          gBSpruceID			= 255;
byte  FrescoFoundation_API          gGrasslandID		= 255;
EDetailLevel    FrescoFoundation_API gDetailLevel		= MINIMAL;
Fresco  FrescoFoundation_API        *FRESCO				= 0;
RasterIO FrescoFoundation_API       *gIO				= 0;
Climate FrescoFoundation_API        *gClimate			= 0;
long FrescoFoundation_API        gTallyOfRandCalls		= 0;


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
/** Clear existing run if any and return to before a run is specified. */
{
	setState(CLEARING);
	
    _landscape->clear();  //Clear landscape first (it depends on the global settings below).
    //Clear FRESCO settings.	
    _isRunningFirstRep  = true;
    _isFireEnabled		= true;
    _randomSeed			= 0;
    //Clear global settings.
	gYOffset			= 0;
	gXOffset			= 0;
	gYSize				= 0;
    gXSize				= 0;
    gCellSize			= 0;
    gMaxRep				= 0;
	gFirstYear			= 0;
	gLastYear			= 0;
    gYear				= 0;
    gRep				= 0;
    gNumSpecies			= 0;
    gNoVegID			= 255;
    gBarrenLichenMossID		= 255;
    gTemperateRainforestID	= 255;
    gTundraID			= 255;
    gShrubTundraID		= 255;
    gGraminoidTundraID		= 255;
    gWetlandTundraID		= 255;
    gDecidID			= 255;
    gWSpruceID			= 255;
    gBSpruceID			= 255;
    gGrasslandID		= 255;
    gWorkingDirectory	= "";
    gInputBasePath		= "";
    gOutputBasePath		= "";
    gOutputDirectory	= "";
    gDetailLevel		= MINIMAL;  	//Reset detail level last so that current detail level causes output during clearing.
	gTallyOfRandCalls   = 0;

	setState(CLEARED);
}


void Fresco::		setup(std::string basePath, std::string fifName, std::string outputTimestamp, long randSeed)
{
	setState(SETTING_UP);
	
    output("Reading FIF File.\n");
    //_fif.Initialize(basePath, fifName);
    _fif.Initialize(basePath, fifName);

    output("Loading General settings.\n");
    std::string temp = "";
    //Get class variables with parsed FIF values.
    gInputBasePath      = FormatDirectory(_fif.root["PathSetup"]["ClientInputBasePath"].asString());
    gOutputBasePath     = FormatDirectory(_fif.root["PathSetup"]["ClientOutputBasePath"].asString());
    gOutputDirectory    = GetFullPath(gOutputBasePath, outputTimestamp);
    EnsureDirectoryExists(gOutputDirectory, false);
    gMaxRep		= fif().root["Simulation"]["MaxReps"].asInt();
    gFirstYear		= fif().root["Simulation"]["FirstYear"].asInt();
    gLastYear		= fif().root["Simulation"]["LastYear"].asInt();
	gDetailLevel        = (temp=fif().root["Simulation"]["Output.DetailLevel"].asString())=="MINIMAL" ? MINIMAL : (temp=="MODERATE" ? MODERATE : (temp=="MAXIMUM" ? MAXIMUM : throw Poco::Exception("Invalid input for Output.DetailLevel: "+temp)));
	try { //Initialize the random number generator.
        _randomSeed = SeedRandom(randSeed);
        ShowOutput(MODERATE, "\tRandom Seed " + ToS(_randomSeed));
    } catch (SimpleException& e) {throw SimpleException(SimpleException::INITFAULT,"Initializing random number generator failed.\n", e.message);}
    gNoVegID            = fif().root["Vegetation"]["NoVeg"]["id"].asInt(); 
        
    //Fire
    output("Loading Fire settings.\n");
    _isFireEnabled      = fif().root["Fire"]["Enabled"].asBool();
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
	
    if (yearResume == gFirstYear) {
        //Begin new rep.
        gRep = rep;
        raiseBeforeRepStart.notify(this, gRep);
        _landscape->repStart();
    }
    //else, pick up in middle of rep after last completed year.

    for (gYear=yearResume; gYear<=gLastYear; gYear++) {
        raiseBeforeYearStart.notify(this, gYear);
        _landscape->yearStart();
        raiseAfterYearStart.notify(this, gYear);
        
        if (gYear > gFirstYear) _landscape->succession();
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
		output("1. There were "+ToS(gTallyOfRandCalls)+" calls to GetNextRand().\n");
        raiseAfterRepEnd.notify(this, gRep);  //Possible Stop point.
		output("2. There were "+ToS(gTallyOfRandCalls)+" calls to GetNextRand().\n");
    }
}

void Fresco::runOneYear(const int rep, const int yearResume){
	setState(SIMULATING);
    	if (yearResume == gFirstYear) {
		//Begin new rep.
		gRep = rep;
		raiseBeforeRepStart.notify(this, gRep);
		_landscape->repStart();
	}
	gYear = yearResume;
	raiseBeforeYearStart.notify(this, gYear);
	_landscape->yearStart();
	raiseAfterYearStart.notify(this, gYear);

	if (gYear > gFirstYear) _landscape->succession();
	if (_isFireEnabled) _landscape->doIgnitions();

	raiseBeforeYearEnd.notify(this, gYear);
	_landscape->yearEnd();
	raiseAfterYearEnd.notify(this, gYear);

    	if (gYear == gLastYear) {
		//Finish rep.
		raiseBeforeRepEnd.notify(this, gRep);
		_landscape->repEnd();
		_isRunningFirstRep = false;
			output("1. There were "+ToS(gTallyOfRandCalls)+" calls to GetNextRand().\n");
		raiseAfterRepEnd.notify(this, gRep);  //Possible Stop point.
			output("2. There were "+ToS(gTallyOfRandCalls)+" calls to GetNextRand().\n");
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
/** Pass output to any subscribers of this event. */
{
   // raiseOutput.notify(this, Output);
   if (isDebugOn()){
		/* 
  		*	Produce a log file for each rep being run.
  		*/
		std::ofstream logfile;
                std::stringstream logfile_title;
                logfile_title << "./logs/LogFile_" << gRep << ".txt";
		logfile.open((logfile_title.str().c_str()), fstream::app);
		logfile << Output << std::endl;
		logfile.close();
   }
}


void Fresco::		outputError(const std::string message)
/** Pass output to cout and any subscribers of this event. */
{
    std::cout << std::endl << message << std::endl;
    raiseErrorOutput.notify(this, message);
}


double* Fresco::	getSpruceFireParms(Json::Value key)
{
    double* pTemp;
    if (_fif.pdGet(key, pTemp) != 3) 
        throw SimpleException(SimpleException::BADARRAYSIZE, (std::string("Unexpected array size returned for Key: ") + key.asCString()).c_str());
    
    //Transfer "const double*" to "double*"
    double* pReturn;
    pReturn = new double[3];
    pReturn[0] = pTemp[0] - _fif.root["Vegetation"]["Decid"]["FireProb"].asDouble();
    pReturn[1] = pTemp[1];
    pReturn[2] = pTemp[2];
    
    //delete[] pTemp; Don't delete, this points to the value stored in the CAssoc of _fif.
    return pReturn;
}


double* Fresco::   getStartAgeParms(Json::Value key, EStartAgeType* type)
{
    double* params;
    int count=_fif.pdGet(key,params);
    
    //Set start age type.
    if (1==count)      
        *type=CONSTANT;
    else if (2==count) 
        *type=WEIBULL;
    else
        throw SimpleException(SimpleException::BADARRAYSIZE,(std::string("Unexpected array size returned for Key: ") + key.asCString()).c_str());
    
    return params;
}


