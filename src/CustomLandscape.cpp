//CustomLandscape.cpp


#include "CustomLandscape.h"
#include "CustomFresco.h"
#include "PreCompiled.h"
#include "Landscape.h"
#include "Interface.h"
#include "Fresco.h"
#include "NoVeg.h"
#include "Stat.h"
#include "Except.h"
#include "Fire.h"
#include "RasterIO.h"
#include "Poco/Path.h"
#include "Poco/Stopwatch.h"


CustomLandscape::		    CustomLandscape(const int rows, const int cols) : Landscape (rows, cols)
{
	_pVegSpatialInput				= 0;
	_pSiteSpatialInput				= 0;
	_pTreeDensitySpatialInput		= 0;
	_pAgeSpatialInput				= 0;
	_pTopoSpatialInput				= 0;
	_pIgnitionFactorSpatialInput	= 0;
	_pSensitivitySpatialInput	    = 0;
	_pBurnSeveritySpatialInput		= 0;
	_pHistoricalFireSpatialInput	= 0;
    _pSuppressions	                = 0;
}

CustomLandscape::		    ~CustomLandscape()
{
    clearLayers();
}

void CustomLandscape::		clear() 
//Called by base class when clearing.  Clear existing run if any and return to before a run is specified.
{
	//Init
	ShowOutput(MODERATE, "Landscape Clear \n");
	//Reset landscape settings.
	_siteInputFile			= "";
	_treeDensityInputFile	= "";
	_ageInputFile			= "";
	_topoInputFile			= "";
	_vegInputFile			= "";
	_isForcedVegTransitions	= false;
	_vegTransitionFile		= "";
    _mapStats.clear();
    //Suppression settings
    _isFireSuppressionOn	= false;
    _suppressionFilename	= "";
    _suppressionTransitions.clear();
	for (int i=0;i<6;i++)	_pSuppressionClasses[i] = 1; // reset to no suppression
	_thresholdFireSize		= 50;
	_thresholdIgnitions		= 5;

    clearLayers();

    //Clear the static members of species types.
    Tundra::clear();
    ShrubTundra::clear();
    GraminoidTundra::clear();
    WetlandTundra::clear();
	Decid::clear();
	BSpruce::clear();
	WSpruce::clear();
	
    //Clear stats.
    _burnPartitionBySuppClassStats.clear();
    _burnPartitionBySuppClassStatFlags = 0;

	//Clear base class.
    Landscape::clear();
}

void CustomLandscape::    	clearLayers()
{
	if (_pVegSpatialInput)				{for (int r=0;r<gYSize;r++) delete[] _pVegSpatialInput[r];				delete[] _pVegSpatialInput;				_pVegSpatialInput				= 0; }
	if (_pSiteSpatialInput)			    {for (int r=0;r<gYSize;r++) delete[] _pSiteSpatialInput[r];				delete[] _pSiteSpatialInput;			_pSiteSpatialInput				= 0; }
	if (_pTreeDensitySpatialInput)		{for (int r=0;r<gYSize;r++) delete[] _pTreeDensitySpatialInput[r];		delete[] _pTreeDensitySpatialInput;		_pTreeDensitySpatialInput		= 0; }
	if (_pAgeSpatialInput)				{for (int r=0;r<gYSize;r++) delete[] _pAgeSpatialInput[r];				delete[] _pAgeSpatialInput;				_pAgeSpatialInput				= 0; }
	if (_pTopoSpatialInput)				{for (int r=0;r<gYSize;r++) delete[] _pTopoSpatialInput[r];				delete[] _pTopoSpatialInput;			_pTopoSpatialInput				= 0; }
	if (_pIgnitionFactorSpatialInput)	{for (int r=0;r<gYSize;r++) delete[] _pIgnitionFactorSpatialInput[r];	    delete[] _pIgnitionFactorSpatialInput;	_pIgnitionFactorSpatialInput	= 0; }
	if (_pSensitivitySpatialInput)      {for (int r=0;r<gYSize;r++) delete[] _pSensitivitySpatialInput[r];	    delete[] _pSensitivitySpatialInput;	    _pSensitivitySpatialInput	    = 0; }
	if (_pBurnSeveritySpatialInput)     {for (int r=0;r<gYSize;r++) delete[] _pBurnSeveritySpatialInput[r];	    delete[] _pBurnSeveritySpatialInput;    _pBurnSeveritySpatialInput	    = 0; }
	if (_pSuppressions)		            {for (int r=0;r<gYSize;r++) delete[] _pSuppressions[r];	                delete[] _pSuppressions;		        _pSuppressions                  = 0; }
    if (_pHistoricalFireSpatialInput)	{for (int r=0;r<gYSize;r++) delete[] _pHistoricalFireSpatialInput[r];		delete[] _pHistoricalFireSpatialInput; _pHistoricalFireSpatialInput		= 0; }
}

void CustomLandscape::		setup() 
//This function is called once per run.
//Initializes items that will stay the same through all reps and years.
{
	int         r,c,y		= 0;
	size_t		t			= 0;
	std::string	baseName;
	std::string	fileName;
	int			tranStart	= 0;
	std::ostringstream	stream;
	Fire::SFireTransition transition;
    std::vector<Fire::SFireTransition>::iterator iter;
	
    setupSuppressionStats();
    setupHabitatStats();
    setupMapStats();
    
    _isUsingUniqueVegAndAgePerRep   = FRESCO->fif().bGet("UseUniqueVegAndAgePerRep");
    _isUsingUniqueBurnSeverityPerRep= FRESCO->fif().bGet("UseUniqueBurnSeverityPerRep");
    _yearOfUniqueInputPerRep        = FRESCO->fif().nGet("YearOfUniqueInputPerRep");
    _vegInputFile			        = FormatDirectory(FRESCO->fif().sGet("VegInputFile"));
    _vegTransitionFile		        = FormatDirectory(FRESCO->fif().sGet("VegTransitionFile"));
    _isForcedVegTransitions	        = FRESCO->fif().bGet("IsForcedVegTransitions");
    _ageInputFile			        = FormatDirectory(FRESCO->fif().sGet("AgeInputFile"));
	_treeDensityInputFile	        = FormatDirectory(FRESCO->fif().sGet("TreeDensityInputFile"));
    _siteInputFile			        = FormatDirectory(FRESCO->fif().sGet("SiteInputFile"));
	_topoInputFile					= FormatDirectory(FRESCO->fif().sGet("TopoInputFile"));
	_burnSeverityInputFile = "";
	if (FRESCO->fif().CheckKey("BurnSeverityInputFile"))  
		_burnSeverityInputFile = FormatDirectory(FRESCO->fif().sGet("BurnSeverityInputFile"));

    Landscape::setup();


	//Make space for landscape data.
	_pVegSpatialInput				= new byte*[gYSize];
	_pSiteSpatialInput				= new float*[gYSize];
	_pTreeDensitySpatialInput		= new int*[gYSize];
	_pAgeSpatialInput				= new int*[gYSize];
	_pTopoSpatialInput				= new byte*[gYSize];
	_pIgnitionFactorSpatialInput	= new float*[gYSize];
	_pSensitivitySpatialInput	    = new float*[gYSize];
	_pBurnSeveritySpatialInput		= new byte*[gYSize];
	_pSuppressions                  = new byte*[gYSize];
	_pHistoricalFireSpatialInput	= new byte*[gYSize];
	for (r=0;r<gYSize;r++) {
		_pVegSpatialInput[r]		    = new byte[gXSize];
		_pSiteSpatialInput[r]			= new float[gXSize];
		_pTreeDensitySpatialInput[r]	= new int[gXSize];
		_pAgeSpatialInput[r]			= new int[gXSize];
		_pTopoSpatialInput[r]			= new byte[gXSize];
		_pIgnitionFactorSpatialInput[r]	= new float[gXSize];
		_pSensitivitySpatialInput[r]	= new float[gXSize];
		_pBurnSeveritySpatialInput[r]	= new byte[gXSize];
		_pSuppressions[r]				= new byte[gXSize];
		_pHistoricalFireSpatialInput[r] = new byte[gXSize];
        for (c=0;c<gXSize;c++) {
			_pVegSpatialInput[r][c]		        = 0;
			_pSiteSpatialInput[r][c]		    = 0;
			_pTreeDensitySpatialInput[r][c]	    = 0;
			_pAgeSpatialInput[r][c]			    = 0;
			_pTopoSpatialInput[r][c]		    = 0;
			_pIgnitionFactorSpatialInput[r][c]	= 0;
			_pSensitivitySpatialInput[r][c]     = 0;
			_pBurnSeveritySpatialInput[r][c]    = 0;
			_pSuppressions[r][c]	            = 0;
			_pHistoricalFireSpatialInput[r][c]	= 0;
		}
	}

	//Load fire transition data...
	if (0==Fire::fireTransitions.size())			throw Exception(Exception::INITFAULT,"Fire transitions missing.  There must be at least one fire transition.\n");
	if (gDetailLevel>=MAXIMUM) {
		ShowOutput("\tFire Transitions:\n");
		ShowOutput("\t\tYear Type\n");
	}
	//...check for transition at FirstYear
	std::string firstYearMsg("A fire trasition is required at the first year of simulation ("+ToS(gFirstYear)+".\n");
	if (Fire::fireTransitions.empty()) 
		throw Exception(Exception::INITFAULT,firstYearMsg,"");
	iter=Fire::fireTransitions.begin();
	if (iter->Year != gFirstYear) 
		throw Exception(Exception::INITFAULT,firstYearMsg,"");
	
	for (iter=Fire::fireTransitions.begin(); iter!=Fire::fireTransitions.end(); iter++)	{
		transition = Fire::fireTransitions[t];
        stream	<< "\t\t" << setiosflags( std::ios::left ) << setiosflags( std::ios::fixed ) << std::setprecision(3)	<< std::setw(4) << iter->Year;
		
		//...validate start year
		tranStart = iter->Year;
		if (tranStart < gFirstYear  || gLastYear < tranStart)
		{
			throw Exception(Exception::INITFAULT, "fire transition start year "
							"("+ToS(tranStart)+") must be within the range of "
							"this simulation's first and last years "
							"("+ToS(gFirstYear)+", "+ToS(gLastYear)+").\n", "");
		}
		
		//...set up fire type
		if (iter->Type==Fire::FIXED){
			stream	<< " FIXED";
			if (iter->Ignition==FRS_NULL)				throw Exception(Exception::INITFAULT,"Fire transition missing ignition.\n");
			if (iter->Sensitivity==FRS_NULL)			throw Exception(Exception::INITFAULT,"Fire transition missing sensitivity.\n");
		}
		else if (iter->Type==Fire::SPATIAL) {
			stream	<< " SPATIAL";
			if (iter->SpatialIgnitionFile=="\"")		throw Exception(Exception::INITFAULT,"Fire transition missing spatial ignition file.\n");
			if (iter->SpatialSensitivityFile=="\"")		throw Exception(Exception::INITFAULT,"Fire transition missing spatial sensitivity file.\n");
		}
		else if (iter->Type==Fire::HISTORICAL) {
			stream	<< " HISTORICAL";
			if (iter->HistoricalFile=="\"")	            throw Exception(Exception::INITFAULT,"Fire transition missing historical file name.\n");
		}
		stream	<< std::endl;
		if (gDetailLevel>=MAXIMUM) ShowOutput(stream);
	}

    //Load suppression transition data.
    if (_isFireSuppressionOn = FRESCO->fif().bGet("Fire.Suppression.On")) {
        _suppressionFilename = FormatDirectory(FRESCO->fif().sGet("Fire.Suppression.Basename"));
        setupSuppressionTransitions();
    }

	//Read in layers.
	ShowOutput(MODERATE, "\tReading topography layer.\n");
	gIO->readRasterFile(GetFullPath(gInputBasePath, _topoInputFile), _pTopoSpatialInput, false);
	ShowOutput(MODERATE, "\tReading site layer.\n");
	gIO->readRasterFile(GetFullPath(gInputBasePath, _siteInputFile), _pSiteSpatialInput, false);
	ShowOutput(MODERATE, "\tReading tree density layer.\n");
	gIO->readRasterFile(GetFullPath(gInputBasePath, _treeDensityInputFile), _pTreeDensitySpatialInput, false);
}

void CustomLandscape::		repStart() 
//Processing at the start of a replicate.
{
	ShowOutput(MODERATE, "\tLandscape Rep setup\n");
	ShowOutput(MODERATE, "\t\tCreating landscape of " + ToS(gYSize) + " rows by " + ToS(gXSize) + " cols.\n");
	ShowOutput(MODERATE, "\t\tReading from input dataset begins "+ToS(gYOffset)+" pixels from the top and "+ToS(gXOffset)+" pixels from the left.\n");


    //Read in layers that might be unique per rep.
	if (_isUsingUniqueVegAndAgePerRep) {
		//Veg
		std::string inputFileWithRepYear = AppendRepYear(_vegInputFile, gRep,_yearOfUniqueInputPerRep);
		ShowOutput(MODERATE, "\t\tReading veg layer from "+ GetFullPath(gInputBasePath, inputFileWithRepYear) +"\n");
		gIO->readRasterFile(GetFullPath(gInputBasePath, inputFileWithRepYear), _pVegSpatialInput, false);		
		//Age
		inputFileWithRepYear = AppendRepYear(_ageInputFile, gRep, _yearOfUniqueInputPerRep);
		ShowOutput(MODERATE, "\t\tReading age layer from "+ GetFullPath(gInputBasePath, inputFileWithRepYear)+"\n");
		gIO->readRasterFile(GetFullPath(gInputBasePath, inputFileWithRepYear), _pAgeSpatialInput, false);		
	}
	else if (FRESCO->isRunningFirstRep()) {  //Use one input map for all reps.
		//Veg
		ShowOutput(MODERATE, "\t\tReading veg layer from "+GetFullPath(gInputBasePath, _vegInputFile)+"\n");
		gIO->readRasterFile(GetFullPath(gInputBasePath, _vegInputFile), _pVegSpatialInput, false);		
		//Age
		ShowOutput(MODERATE, "\t\tReading age layer from "+GetFullPath(gInputBasePath, _ageInputFile)+"\n");
		gIO->readRasterFile(GetFullPath(gInputBasePath, _ageInputFile), _pAgeSpatialInput, false);		
	}


	//Burn Severity	
	if (_isUsingUniqueBurnSeverityPerRep && _burnSeverityInputFile != "") {
		//Use unique map per rep.
		std::string inputFileWithRepYear = AppendRepYear(_burnSeverityInputFile, gRep,_yearOfUniqueInputPerRep);
		ShowOutput(MODERATE, "\t\tReading burn severity layer from "+GetFullPath(gInputBasePath, inputFileWithRepYear)+"\n");
		gIO->readRasterFile(GetFullPath(gInputBasePath, inputFileWithRepYear), _pBurnSeveritySpatialInput, false);		
	}
	else if (FRESCO->isRunningFirstRep()) {  
		//Use one input map for all reps.
		std::string message = _burnSeverityInputFile==""? 
			"\t\tSetting burn severity to default, HighLSS (level 3), for all cells.\n" :
			"\t\tReading burn severity layer from "+_burnSeverityInputFile+"\n";
		ShowOutput(MODERATE, message);
		gIO->readRasterFile(GetFullPath(gInputBasePath, _burnSeverityInputFile), _pBurnSeveritySpatialInput, false);		
	}	
 
	//Create landscape cell-by-cell, assigning values from all the input 
	Frame* pFrame = 0;
	for (int r=0; r<gYSize; r++) {
		for (int c=0; c<gXSize; c++) {
			pFrame = _pFrames[r][c];
			if (pFrame) delete pFrame;
			byte frameTypeID = _pVegSpatialInput[r][c];
			if (frameTypeID==gBSpruceID)	{ _pFrames[r][c] = new BSpruce(gFirstYear - _pAgeSpatialInput[r][c], _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (frameTypeID==gWSpruceID)	{ _pFrames[r][c] = new WSpruce(gFirstYear - _pAgeSpatialInput[r][c], _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (frameTypeID==gGrasslandID)	{ _pFrames[r][c] = new Grassland(gFirstYear - _pAgeSpatialInput[r][c], _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (frameTypeID==gDecidID)	    { _pFrames[r][c] = new Decid(gFirstYear - _pAgeSpatialInput[r][c],   _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (frameTypeID==gTundraID)	{ _pFrames[r][c] = new Tundra(gFirstYear - _pAgeSpatialInput[r][c],  _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID, _pTreeDensitySpatialInput[r][c]);}
			else if (frameTypeID==gShrubTundraID)	{ _pFrames[r][c] = new ShrubTundra(gFirstYear - _pAgeSpatialInput[r][c],  _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID, _pTreeDensitySpatialInput[r][c]);}
			else if (frameTypeID==gGraminoidTundraID)	{ _pFrames[r][c] = new GraminoidTundra(gFirstYear - _pAgeSpatialInput[r][c],  _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID, _pTreeDensitySpatialInput[r][c]);}
			else if (frameTypeID==gWetlandTundraID)	{ _pFrames[r][c] = new WetlandTundra(gFirstYear - _pAgeSpatialInput[r][c],  _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID, _pTreeDensitySpatialInput[r][c]);}
			else if (frameTypeID==gNoVegID)	    { _pFrames[r][c] = new NoVeg(gFirstYear - _pAgeSpatialInput[r][c],   _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (IsNodata(frameTypeID))    { _pFrames[r][c] = new NoVeg(gFirstYear - _pAgeSpatialInput[r][c],   _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else								{ throw Exception(Exception::INITFAULT, "Unknown vegetation type ID at cell [" + ToS(r) + "][" + ToS(c) + "]: " + ToS((int)frameTypeID)); }
		}
	}

	Landscape::repStart();
}

void CustomLandscape::		yearStart() 
{
	//Process base class year start.
	Landscape::yearStart();
	//Process transitions.
	doFireTransitions();
    doFireSuppressionTransitions();
}

void CustomLandscape::		yearEnd()
{
	//Force veg transitions and write maps before Landscape yearEnd().
    doVegetationTransitions();
    writeMaps();
    collectStats();
    Landscape::yearEnd();
}

void CustomLandscape::		doFireTransitions() 
{
	Fire::EType	oldFireType;
	Fire*			pFire = 0;
    static std::vector<Fire::SFireTransition>::iterator transition;

	//Initialize transition iterator at start of run.
	if (gYear==gFirstYear) transition = Fire::fireTransitions.begin();
	//Skip if no transitions scheduled or we are at the end of transitions.
	if (transition==Fire::fireTransitions.end() || Fire::fireTransitions.empty()) return;
	oldFireType = Fire::fireType;
	if (transition->Year==gYear) {
		//Set new fire settings.
		Fire::fireType = transition->Type;
		switch (transition->Type) {
		case Fire::FIXED :
			//Set ignition and sensitivity for all cells to fixed values.
			ShowOutput(MODERATE, "\t\tFire Transition changed to FIXED.\n");
			if (gDetailLevel>=MAXIMUM) {
				ShowOutput("\t\t\tIgnition = " + ToS(transition->Ignition) + "\n");
				ShowOutput("\t\t\tSensitivity = " + ToS(transition->Sensitivity) + "\n");
			}
			for (int r=0; r<gYSize; r++) {
				for (int c=0; c<gXSize; c++) {
					pFire = (Fire*)_pFrames[r][c];	
					pFire->fireIgnitionFactor = transition->Ignition;
					pFire->fireSensitivity = transition->Sensitivity;
				}
			}
			break;
		case Fire::SPATIAL :
			//Set ignition and sensitivity to spatially explicit values.
			ShowOutput(MODERATE, "\t\tFire Transition changed to SPATIAL.\n");
			if (gDetailLevel>=MAXIMUM) {	
				ShowOutput("\t\t\tIgnition file = " + GetFullPath(gInputBasePath, transition->SpatialIgnitionFile) + "\n");
				ShowOutput("\t\t\tSensitivity file = " + GetFullPath(gInputBasePath, transition->SpatialSensitivityFile) + "\n");
			}
			//Read spatial data files.
			gIO->readRasterFile(GetFullPath(gInputBasePath, transition->SpatialIgnitionFile), _pIgnitionFactorSpatialInput, false);		
			gIO->readRasterFile(GetFullPath(gInputBasePath, transition->SpatialSensitivityFile), _pSensitivitySpatialInput, false);		
			for (int r=0; r<gYSize; r++) {
				for (int c=0; c<gXSize; c++) {
					pFire = (Fire*)_pFrames[r][c];
					pFire->fireIgnitionFactor = _pIgnitionFactorSpatialInput[r][c];
					pFire->fireSensitivity = _pSensitivitySpatialInput[r][c];
				}
			}
			break;
		case Fire::HISTORICAL :
			ShowOutput(MODERATE, "\t\tFire Transition changed to HISTORICAL.\n");
			ShowOutput(MAXIMUM, "\t\t\thistorical file = " + GetFullPath(gInputBasePath, transition->HistoricalFile) + "\n");
            pFire->historicalFiresFileName = transition->HistoricalFile;
			break;
		}
		//Advance to the next transition.
		transition++;
	}
}

void CustomLandscape::		doIgnitions() 
//This doIgnitions overloads the default ignitions by providing the option to read in prescribed fire maps.
{
	ShowOutput(MODERATE, "\t\tIgnitions\n");
	if (Fire::fireType==Fire::HISTORICAL) {
		ShowOutput(MAXIMUM, "\t\t\tFire set to HISTORICAL.\n");
		//
		// Read in historical fire map...
		//
		std::string	filename = AppendYear(Fire::historicalFiresFileName, gYear);
		ShowOutput(MAXIMUM, "\t\t\tReading in historical fire map: " + GetFullPath(gInputBasePath, filename) + "\n");
		gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pHistoricalFireSpatialInput, false);		
		//
		// Force cells to burn as specified in the file...
		//
		Frame*	pFrame		= 0;
		int		fireSize	= 0;
		int		numLow		= 0;
		int		numHiLSS	= 0;
		for (int r=0; r<gYSize; r++) {
			for (int c=0; c<gXSize; c++) {
				if (_pHistoricalFireSpatialInput[r][c]>0) {
					if (_pFrames[r][c]->type() != gNoVegID) {
						pFrame = _pFrames[_row=r][_col=c]; // <--be careful, spaghetti code: _row and _col are set here so Landscape::logFireStats() can access the species of this frame.  Should be cleaned.
						pFrame->historicalFire(this);
						fireSize++;
						//Set burn severity...
						if (pFrame->type() == gTundraID) { pFrame->burnSeverity=Fire::LOW; numLow++; }
						else if (pFrame->type() == gShrubTundraID) { pFrame->burnSeverity=Fire::LOW; numLow++; }
						else if (pFrame->type() == gGraminoidTundraID) { pFrame->burnSeverity=Fire::LOW; numLow++; }
						else if (pFrame->type() == gWetlandTundraID) { pFrame->burnSeverity=Fire::LOW; numLow++; }
						else { pFrame->burnSeverity=Fire::HIGH_LSS; numHiLSS++; }
					}
				}
			}
		}
		//Record fire size and fire num.
		if (fireSize>0) {
			_fireSizeStat.Add(gYear, gRep, fireSize, 0, numLow, 0, numHiLSS, 0);   //0=Natural ignition cause.
			_fireNumStat.Add(gYear, gRep, 1);			//0=Natural ignition cause.
		}
	}
	else 
	{
		ShowOutput(MAXIMUM, "\t\t\tFire set to " + std::string(Fire::fireType==Fire::SPATIAL ? "SPATIAL" : "FIXED") + ".\n");
		Landscape::doIgnitions();
	}
	////Exit
	//if (gDetailLevel>=MODERATE) {
	//	ShowOutput("\t\t\tNumber = " + ToS(_fireNumStat.SumOfYearRep(gYear,gRep)) + "\n");
	//	ShowOutput("\t\t\tSize   = " + ToS(_fireSizeStat.SumOfYearRep(gYear,gRep)) + "\n");
	//}
}

const float CustomLandscape::getCustomFireSpreadMultiplier(const unsigned int row, const unsigned int col, const unsigned int fireSizeTotal, const unsigned int fireNum)
{
    return getCellFireSuppression(row,col,fireSizeTotal,fireNum);
}

const float CustomLandscape::getCellFireSuppression(const unsigned int row, const unsigned int col, const unsigned int fireSizeTotal, const unsigned int fireNum)
//Worker function for doIgnitions.  Returns the suppression value that will be be multiplied against fire spread probability. 
//A return value less than 1 decreases spread probability, greater than 1 increases spread probability.  Returns 1 by default, 
//(spread probability is not affected by suppression of 1).
// Parameters:
//   fireSizeTotal - number of cells burned in the current year so far.
//   fireNum - number of ignitions in current year so far.
{
	if (_isFireSuppressionOn) {
		//Apply suppression factor if fire is below suppression threshold.
		if (fireNum <= _thresholdIgnitions && fireSizeTotal <= _thresholdFireSize) {
			byte s = _pSuppressions[_row][_col];
			if (s > 0  &&  s < 6)
				return _pSuppressionClasses[s];
			else
				throw Poco::Exception("invalid suppression class ID ("+ToS(s)+"). Valid suppression class IDs are 1 through 5.");
        }
	}
	return 1;
}

//Suppression Transitions
void CustomLandscape::      setupSuppressionTransitions()
{
    std::vector<SSuppressionTransition>::iterator it;
    //Get arrays of values and make sure all arrays have the same count.
    int           count = 0;
    const int     *pYears, *pThresholdFireSizes, *pThresholdIgnitions; 
    const bool    *pHasNewMapFlags;
    const double  *pClass1,*pClass2, *pClass3, *pClass4, *pClass5;
    std::string   errBase("Unexpected array size returned for Key: ");
    std::string   keyBase("Fire.Suppression.Tran.");
    std::string   yearsKey(keyBase + "Years");
    std::string   hasNewMapKey(keyBase + "HasNewMap");
    std::string   class1Key(keyBase + "Class1");
    std::string   class2Key(keyBase + "Class2");
    std::string   class3Key(keyBase + "Class3");
    std::string   class4Key(keyBase + "Class4");
    std::string   class5Key(keyBase + "Class5");
    std::string   fireSizeKey(keyBase + "Threshold.FireSize");
    std::string   ignitionKey(keyBase + "Threshold.Ignitions");
    count = FRESCO->fif().pnGet(yearsKey.c_str(), pYears);
    if (FRESCO->fif().pnGet(fireSizeKey.c_str(), pThresholdFireSizes) != count)   throw Exception(Exception::BADARRAYSIZE, errBase + fireSizeKey);
    if (FRESCO->fif().pnGet(ignitionKey.c_str(), pThresholdIgnitions) != count)   throw Exception(Exception::BADARRAYSIZE, errBase + ignitionKey);
    if (FRESCO->fif().pbGet(hasNewMapKey.c_str(), pHasNewMapFlags) != count)      throw Exception(Exception::BADARRAYSIZE, errBase + hasNewMapKey);
    if (FRESCO->fif().pdGet(class1Key.c_str(), pClass1) != count)                 throw Exception(Exception::BADARRAYSIZE, errBase + class1Key);
    if (FRESCO->fif().pdGet(class2Key.c_str(), pClass2) != count)                 throw Exception(Exception::BADARRAYSIZE, errBase + class2Key);
    if (FRESCO->fif().pdGet(class3Key.c_str(), pClass3) != count)                 throw Exception(Exception::BADARRAYSIZE, errBase + class3Key);
    if (FRESCO->fif().pdGet(class4Key.c_str(), pClass4) != count)                 throw Exception(Exception::BADARRAYSIZE, errBase + class4Key);
    if (FRESCO->fif().pdGet(class5Key.c_str(), pClass5) != count)                 throw Exception(Exception::BADARRAYSIZE, errBase + class5Key);

    //Load array values into transition list.
    std::string temp = "";
    for (int i=0; i<count; i++) {
	    //Prepare the transition.
	    SSuppressionTransition t;
	    t.Year					= pYears[i];
        t.ThresholdFireSize     = pThresholdFireSizes[i];
        t.ThresholdIgnitions    = pThresholdIgnitions[i];
        t.HasNewMap             = pHasNewMapFlags[i];
        t.Classes[1]            = pClass1[i];
        t.Classes[2]            = pClass2[i];
        t.Classes[3]            = pClass3[i];
        t.Classes[4]            = pClass4[i];
        t.Classes[5]            = pClass5[i];
	    //Insert transition into transition list in order.
        for (it=_suppressionTransitions.begin(); it!=_suppressionTransitions.end(); it++)
		    if (t.Year<it->Year) break;
	    _suppressionTransitions.insert(it,t);
        //Verify map availability.
        if (t.HasNewMap || t.Year==0) {
            if (_suppressionFilename.empty()) throw Poco::Exception("Developer note: Suppression filename expected during suppression transition setup.");
            std::string filepath(AppendYear(_suppressionFilename, t.Year));
            if (!InputFileExists(filepath)) 
                throw Exception(Exception::INITFAULT, "Expected suppression map for year " + ToS(t.Year) + " at " + GetFullPath(gInputBasePath, filepath));
        }
    }
    //Show a summary of scheduled suppression transitions.
    if (gDetailLevel>=MAXIMUM) {
		ShowOutput("\tFire Suppression Transitions:\n");
		ShowOutput("\t\tYear Map? Class1 Class2 Class3 Class4 Class5 SizeThreshold IgnitThreshold \n");
//                        1000 yes  0.23   0.1234 1      1      1      500000        500000
    }
    if (_suppressionTransitions.empty()) throw Exception(Exception::INITFAULT,"Suppression transitions missing.  There must be at least one fire suppression transition.\n");
    std::ostringstream	stream;
    for (it=_suppressionTransitions.begin(); it!=_suppressionTransitions.end(); it++)
    {
        stream  << "\t\t" << setiosflags( std::ios::left ) << setiosflags( std::ios::fixed ) << std::setprecision(3);
        stream 	<< std::setw(5) << it->Year;
        stream 	<< std::setw(5) << (it->HasNewMap ? "yes" : "no");
        stream 	<< std::setw(7) << it->Classes[1];
        stream 	<< std::setw(7) << it->Classes[2];
        stream 	<< std::setw(7) << it->Classes[3];
        stream 	<< std::setw(7) << it->Classes[4];
        stream 	<< std::setw(7) << it->Classes[5];
        stream 	<< std::setw(14)<< it->ThresholdFireSize;
        stream 	<< std::setw(14)<< it->ThresholdIgnitions;
		stream  << std::endl;
        if (gDetailLevel>=MAXIMUM) ShowOutput(stream);
    }
    it = _suppressionTransitions.begin();
    if (it->Year != gFirstYear) throw Exception(Exception::INITFAULT,"A suppression trasition is required at the first simulation year ("+ToS(gFirstYear)+").\n");
}


void CustomLandscape::      setCurrentSuppressionTransition(std::vector<SSuppressionTransition>::iterator transition)
{
    _pSuppressionClasses[0]     = 1; //No suppression.
    _pSuppressionClasses[1]     = transition->Classes[1];
    _pSuppressionClasses[2]     = transition->Classes[2];
    _pSuppressionClasses[3]     = transition->Classes[3];
    _pSuppressionClasses[4]     = transition->Classes[4];
    _pSuppressionClasses[5]     = transition->Classes[5];
    _thresholdFireSize          = transition->ThresholdFireSize;
    _thresholdIgnitions         = transition->ThresholdIgnitions;
    //Show summary
    std::stringstream m;
    m << "\t\tSuppression Transition:\n";
    for (int i=1; i<6; i++) m << "\t\t\tClass " << i << " = " << transition->Classes[i] << "\n";
    m << "\t\t\tIgnitions Threshold = " << _thresholdFireSize << "\n";
    m << "\t\t\tFire Size Threshold = " << _thresholdIgnitions << "\n";
    ShowOutput(MODERATE, m.str());
    //Process map
    if (transition->HasNewMap || transition->Year==gFirstYear) { 
        std::string filename(AppendYear(_suppressionFilename));
	    if (InputFileExists(filename)) {
		    ShowOutput(MAXIMUM, "\t\t\tProcessing suppression file: " + GetFullPath(gInputBasePath, filename) + ".\n");
            //if (_pSuppressions != 0)  { for (int r=0;r<gYSize;r++) delete[] _pSuppressions[r];  delete[] _pSuppressions;  _pSuppressions = 0; }
		    gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSuppressions, false);		
        } 
        else throw Poco::Exception("Expected suppression map for year " + ToS(gYear) + " at " + GetFullPath(gInputBasePath, filename));
    }
    else ShowOutput(MAXIMUM, "\t\t\tNo new map.\n");
}


void CustomLandscape::      doFireSuppressionTransitions() 
{
    if (_isFireSuppressionOn)
    {
	    std::vector<SSuppressionTransition>::iterator transition;
	    for (transition=CustomLandscape::_suppressionTransitions.begin(); transition!=CustomLandscape::_suppressionTransitions.end(); transition++) 
        {
		    if (transition->Year == gYear) {
			    setCurrentSuppressionTransition(transition);
			    break;
		    }
	    }
    }
}



void CustomLandscape::		doVegetationTransitions() 
{
	int		r,c			= 0;
	Frame*	pCurFrame	= 0;
	byte	curType		= 0;
	Frame*	pNewFrame	= 0;
	byte		newType		= 0;
	std::string	filename;

	if (_isForcedVegTransitions) {
		//Not running large memory model: read in historical fire now.
		filename = _vegTransitionFile;
		filename = AppendYear(filename, gYear);
		//Assume file does not exist on exception.
		try {gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pVegSpatialInput, false);}		
		catch (...) {return;}
		ShowOutput(MODERATE, "\t\tProcessing Vegetation Transitions\n");
		ShowOutput(MAXIMUM, "\t\t\tReading in veg transition file: " + GetFullPath(gInputBasePath, filename) + "\n");
		//Force cells to succeed to new types.
		for (r=0; r<gYSize; r++) {
			for (c=0; c<gXSize; c++) {
				pCurFrame	= _pFrames[r][c];
				curType		= pCurFrame->type();
				newType		= _pVegSpatialInput[r][c];
				if (newType!=curType) {
					//Get the new frame type.
					if (newType==gBSpruceID)		pNewFrame = new BSpruce(*pCurFrame);
					else if (newType==gWSpruceID)	pNewFrame = new WSpruce(*pCurFrame);
					else if (newType==gGrasslandID)	pNewFrame = new Grassland(*pCurFrame);
					else if (newType==gDecidID)		pNewFrame = new Decid(*pCurFrame);
					else if (newType==gTundraID)	pNewFrame = new Tundra(*pCurFrame);
					else if (newType==gShrubTundraID)	pNewFrame = new ShrubTundra(*pCurFrame);
					else if (newType==gGraminoidTundraID)	pNewFrame = new GraminoidTundra(*pCurFrame);
					else if (newType==gWetlandTundraID)	pNewFrame = new WetlandTundra(*pCurFrame);
					else if (newType==gNoVegID)		pNewFrame = new NoVeg(*pCurFrame);
					else if (IsNodata(newType))	pNewFrame = new NoVeg(*pCurFrame);
					else							throw Exception(Exception::UNKNOWN,"Failed vegetation transition.  Cell (" + ToS(r) + "," + ToS(c) + ") has an unknown type (" + ToS((int)newType) + ".\n");
					//Decrement cell count for old species and increment cell count for new species.
					_vegDistributionStat[curType]--;
					_vegDistributionStat[newType]++;
					//Update veg residence times.
					_vegResidenceStat[curType].Add(gYear, gRep, abs(pCurFrame->frameAge()));
					//Delete old frame and assign the new.
					delete pCurFrame;
					_pFrames[r][c] = pNewFrame;
				}
			}
		}
	}
}


void CustomLandscape::      setupSuppressionStats()
{
    _burnPartitionBySuppClassStatFlags = FRESCO->fif().nGet("Stat.BurnsBySupp.Flags");
    _burnPartitionBySuppClassStats.resize(NUM_SUPPRESSION_CLASSES+1);
	for (int s=1; s<=NUM_SUPPRESSION_CLASSES; s++) {
		_burnPartitionBySuppClassStats[s].setup("BurnsBySupp["+ToS(s)+"]", _burnPartitionBySuppClassStatFlags, false);

		int numYears = FRESCO->fif().nGet("LastYear") - FRESCO->fif().nGet("FirstYear") + 1;  // Used for number of rows in StatArray
		int numReps = FRESCO->fif().nGet("MaxReps");  // Used for number of columns in StatArray
		stringstream ss;
		ss << "BurnsBySupp[" << ToS(s) << "]";
		#ifdef WITHMPI
		MyStats->addStatFile(ss.str(), numYears, numReps, MATRIX);
		#endif
    }
}


void CustomLandscape::      setupHabitatStats()
{
    //Get values for each habitat type.
    long flags = FRESCO->fif().nGet("Stat.Habitat.Flags");
    if (FRESCO->fif().CheckKey("Stat.Habitat.Types")) {
        char *const* pTypes;
        int count = FRESCO->fif().psGet("Stat.Habitat.Types", pTypes);
        for (int i=0; i<count; i++) {
            //Get values from FIF for each habitat type.
            const int* pVegTypes;  int vegCount;  std::string keyVegTypes("Stat.Habitat." + ToS(pTypes[i]) + ".VegTypes");
            const int* pAgeRange;  int ageCount;  std::string keyAgeRange("Stat.Habitat." + ToS(pTypes[i]) + ".AgeRange"); 
            vegCount = FRESCO->fif().pnGet(keyVegTypes.c_str(), pVegTypes);
            ageCount = FRESCO->fif().pnGet(keyAgeRange.c_str(), pAgeRange);
            if (ageCount!=2) throw Poco::Exception("Expected array size of 2 for key, " + keyAgeRange + ".  Received " + ToS(ageCount) + ".");
            //Organize values into SHabitatStat collection.
            SHabitatStat habitat;
            habitat.Stat.setup("Habitat"+ToS(pTypes[i]), flags, false);

	    int numYears = FRESCO->fif().nGet("LastYear") - FRESCO->fif().nGet("FirstYear") + 1;  // Used for number of rows in StatArray
	    int numReps = FRESCO->fif().nGet("MaxReps");  // Used for number of columns in StatArray
	    #ifdef WITHMPI
	    MyStats->addStatFile("Habitat"+ToS(pTypes[i]), numYears, numReps, MATRIX);
	    #endif
            habitat.MinAge = pAgeRange[0];
            habitat.MaxAge = pAgeRange[1];
			if (!(habitat.MinAge < habitat.MaxAge)) 
				throw Poco::Exception("Invalid habitat age range " + ToS(habitat.MinAge) + " to " + ToS(habitat.MaxAge) + " in key " + keyAgeRange + ".  Must be in order.");
            for (int v=0; v<vegCount; v++) {
				if (!CustomFresco::isValidVegType(pVegTypes[v])) 
					throw Poco::Exception("Invalid habitat veg type " + ToS(pVegTypes[v]) + " in key " + keyVegTypes + ".");
                habitat.VegTypes.push_back(pVegTypes[v]);
            }
            _habitatStats.push_back(habitat);
        }
    }
}

void CustomLandscape::		setupMapStats()
{
    int mapCount = 0;
    char *const* pMapFiles;
    char *const* pMapCodes;
    const int* pMapFlags;
    const int* pMapRepStart;
    const int* pMapRepFreq;
    const int* pMapYearStart;
    const int* pMapYearFreq;

    if (FRESCO->fif().CheckKey("MapFiles")
        && FRESCO->fif().CheckKey("MapCodes")
        && FRESCO->fif().CheckKey("MapFlags")
        && FRESCO->fif().CheckKey("MapRepStart")
        && FRESCO->fif().CheckKey("MapRepFreq")
        && FRESCO->fif().CheckKey("MapYearStart")
        && FRESCO->fif().CheckKey("MapYearFreq")) 
    {
        mapCount = FRESCO->fif().psGet("MapFiles", pMapFiles);
        if (FRESCO->fif().psGet("MapCodes", pMapCodes) != mapCount)         throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: MapCodes");
        if (FRESCO->fif().pnGet("MapFlags", pMapFlags) != mapCount)         throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: MapFlags");
        if (FRESCO->fif().pnGet("MapRepStart", pMapRepStart) != mapCount)   throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: MapRepStart");
        if (FRESCO->fif().pnGet("MapRepFreq", pMapRepFreq) != mapCount)     throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: MapRepFreq");
        if (FRESCO->fif().pnGet("MapYearStart", pMapYearStart) != mapCount) throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: MapYearStart");
        if (FRESCO->fif().pnGet("MapYearFreq", pMapYearFreq) != mapCount)   throw Exception(Exception::BADARRAYSIZE,"Unexpected array size returned for Key: MapYearFreq");
    }
    else mapCount = 0;

    for (int i=0; i<mapCount; i++) {
	    SMapStat MapStat;
        //Create the map stat.
	    MapStat.File		= FormatDirectory(pMapFiles[i]);
	    MapStat.Code		= pMapCodes[i];
	    MapStat.Flags		= pMapFlags[i];
	    MapStat.repStart	= pMapRepStart[i];
	    MapStat.RepFreq		= pMapRepFreq[i];
	    MapStat.yearStart	= pMapYearStart[i];
	    MapStat.YearFreq	= pMapYearFreq[i];
        //Add to vector of stats.
	    _mapStats.push_back(MapStat);
    }
}


void CustomLandscape::    	writeMaps()
{
    bool isShown = false;
    std::string	filename;

	for (std::vector<SMapStat>::iterator iter=_mapStats.begin(); iter<_mapStats.end(); iter++) {
		if (iter->Flags & outData) {
			if (    (iter->RepFreq>0  &&  gRep>=iter->repStart    && !((gRep-iter->repStart)%iter->RepFreq)) 
                &&  (iter->YearFreq>0 &&  gYear>=iter->yearStart	&& !((gYear-iter->yearStart)%iter->YearFreq) ) ) {
				if (!isShown)  ShowOutput(MODERATE, "\t\tSave Maps: ");
				ShowOutput(MODERATE, iter->Code + ", ");
				isShown = true;
				filename = GetFullPath(gOutputDirectory, iter->File);
				filename = AppendRepYear(filename);
				gIO->writeRasterFile(filename, _pFrames, gIO->getMapType(iter->Flags), gYear, gRep);
			}
		}
	}
	if (isShown)  ShowOutput(MODERATE, "\n");
}

void CustomLandscape::      collectStats()
{
	ShowOutput(MODERATE, "\t\tCollecting stats.\n");
    //Poco::Stopwatch sw;
    //sw.start();

    //Habitat items.
    int veg;
    int age;
    std::vector<SHabitatStat>::iterator habitat;
    std::vector<int>::iterator vegTypes;

    Frame* pCurFrame = 0;
    const int curYear = gYear;
    for (int r=0; r<gYSize; r++) {
        for (int c=0; c<gXSize; c++) {
            pCurFrame = _pFrames[r][c];
            //Do evaluations for this cell.

            //---------------------------------------------------------
            //Burn partitioning by suppression zone
            if (pCurFrame->yearOfLastBurn == curYear) {
                switch(_pSuppressions[r][c]) {
                    case 1: _burnPartitionBySuppClassStats[1]++; break;
                    case 2: _burnPartitionBySuppClassStats[2]++; break;
                    case 3: _burnPartitionBySuppClassStats[3]++; break;
                    case 4: _burnPartitionBySuppClassStats[4]++; break;
                    case 5: _burnPartitionBySuppClassStats[5]++; break;
                }
            }

            //----------------------------------------------------------
            //Habitat - Add to habitat stat if vegType and age meet criteria.
            veg = pCurFrame->type();
            age = pCurFrame->age();
            for (habitat = _habitatStats.begin(); habitat<_habitatStats.end(); habitat++) {
                if (age > habitat->MinAge && age < habitat->MaxAge)
                {
                    for (vegTypes = habitat->VegTypes.begin(); vegTypes<habitat->VegTypes.end(); vegTypes++) {
                        if (veg == *vegTypes) {
                            habitat->Stat++;
                            break;
                        }
                    }
                }
            }
            //Done with evaluations for this cell.
        }
    }
    //Summarize stats.
    for (int s=1; s<=NUM_SUPPRESSION_CLASSES; s++) {
        _burnPartitionBySuppClassStats[s].Add(gYear,gRep);
	stringstream ss;
	ss << "BurnsBySupp[" << s << "]";
	#ifdef WITHMPI
	MyStats->addStat(ss.str(), gYear, gRep, _burnPartitionBySuppClassStats[s].m_lTally);
	#endif
        _burnPartitionBySuppClassStats[s].m_lTally = 0;
    }
    for (habitat = _habitatStats.begin(); habitat<_habitatStats.end(); habitat++) {
	#ifdef WITHMPI
	MyStats->addStat(habitat->Stat.m_sTitle, gYear, gRep, habitat->Stat.m_lTally);
	#endif
        habitat->Stat.Add(gYear,gRep);
        habitat->Stat.m_lTally = 0;
    }

    //Timing.
    //sw.stop();
    //std::cout << "Time to collectStats():" << sw.elapsed() << std::endl;
}
