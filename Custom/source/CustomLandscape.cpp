//CustomLandscape.cpp


#include "Fresco/Custom/CustomLandscape.h"
#include "Fresco/Custom/CustomFresco.h"
#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Landscape.h"
#include "Fresco/Foundation/Interface.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/NoVeg.h"
#include "Fresco/Foundation/Stat.h"
#include "Fresco/Foundation/Except.h"
#include "Fresco/Foundation/Fire.h"
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
	if (_pVegSpatialInput)				{for (int r=0;r<gNumRows;r++) delete[] _pVegSpatialInput[r];				delete[] _pVegSpatialInput;				_pVegSpatialInput				= 0; }
	if (_pSiteSpatialInput)			    {for (int r=0;r<gNumRows;r++) delete[] _pSiteSpatialInput[r];				delete[] _pSiteSpatialInput;			_pSiteSpatialInput				= 0; }
	if (_pTreeDensitySpatialInput)		{for (int r=0;r<gNumRows;r++) delete[] _pTreeDensitySpatialInput[r];		delete[] _pTreeDensitySpatialInput;		_pTreeDensitySpatialInput		= 0; }
	if (_pAgeSpatialInput)				{for (int r=0;r<gNumRows;r++) delete[] _pAgeSpatialInput[r];				delete[] _pAgeSpatialInput;				_pAgeSpatialInput				= 0; }
	if (_pTopoSpatialInput)				{for (int r=0;r<gNumRows;r++) delete[] _pTopoSpatialInput[r];				delete[] _pTopoSpatialInput;			_pTopoSpatialInput				= 0; }
	if (_pIgnitionFactorSpatialInput)	{for (int r=0;r<gNumRows;r++) delete[] _pIgnitionFactorSpatialInput[r];	    delete[] _pIgnitionFactorSpatialInput;	_pIgnitionFactorSpatialInput	= 0; }
	if (_pSensitivitySpatialInput)      {for (int r=0;r<gNumRows;r++) delete[] _pSensitivitySpatialInput[r];	    delete[] _pSensitivitySpatialInput;	    _pSensitivitySpatialInput	    = 0; }
	if (_pBurnSeveritySpatialInput)     {for (int r=0;r<gNumRows;r++) delete[] _pBurnSeveritySpatialInput[r];	    delete[] _pBurnSeveritySpatialInput;    _pBurnSeveritySpatialInput	    = 0; }
	if (_pSuppressions)		            {for (int r=0;r<gNumRows;r++) delete[] _pSuppressions[r];	                delete[] _pSuppressions;		        _pSuppressions                  = 0; }
    if (_pHistoricalFireSpatialInput)	{for (int r=0;r<gNumRows;r++) delete[] _pHistoricalFireSpatialInput[r];		delete[] _pHistoricalFireSpatialInput; _pHistoricalFireSpatialInput		= 0; }
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
	int			tranStop	= 0;
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
	_pVegSpatialInput				= new int*[gNumRows];
	_pSiteSpatialInput				= new float*[gNumRows];
	_pTreeDensitySpatialInput		= new int*[gNumRows];
	_pAgeSpatialInput				= new int*[gNumRows];
	_pTopoSpatialInput				= new int*[gNumRows];
	_pIgnitionFactorSpatialInput	= new float*[gNumRows];
	_pSensitivitySpatialInput	    = new float*[gNumRows];
	_pBurnSeveritySpatialInput		= new int*[gNumRows];
	_pSuppressions                  = new int*[gNumRows];
	_pHistoricalFireSpatialInput	= new int*[gNumRows];
	for (r=0;r<gNumRows;r++) {
		_pVegSpatialInput[r]		    = new int[gNumCol];
		_pSiteSpatialInput[r]			= new float[gNumCol];
		_pTreeDensitySpatialInput[r]	= new int[gNumCol];
		_pAgeSpatialInput[r]			= new int[gNumCol];
		_pTopoSpatialInput[r]			= new int[gNumCol];
		_pIgnitionFactorSpatialInput[r]	= new float[gNumCol];
		_pSensitivitySpatialInput[r]	= new float[gNumCol];
		_pBurnSeveritySpatialInput[r]	= new int[gNumCol];
		_pSuppressions[r]				= new int[gNumCol];
		_pHistoricalFireSpatialInput[r] = new int[gNumCol];
        for (c=0;c<gNumCol;c++) {
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

	//Load fire transition data.
	if (0==Fire::fireTransitions.size())			throw Exception(Exception::INITFAULT,"Fire transitions missing.  There must be at least one fire transition.\n");
	if (gDetailLevel>=MAXIMUM) {
		ShowOutput("\tFire Transitions:\n");
		ShowOutput("\t\tYear Type\n");
	}
	if (Fire::fireTransitions.empty()) throw Exception(Exception::INITFAULT,"A fire trasition is required at year zero.\n","");
	iter=Fire::fireTransitions.begin();
	if (iter->Year != 0) throw Exception(Exception::INITFAULT,"A fire trasition is required at year zero.\n","");
	for (iter=Fire::fireTransitions.begin(); iter!=Fire::fireTransitions.end(); iter++)	{
		transition = Fire::fireTransitions[t];
        stream	<< "\t\t" << setiosflags( std::ios::left ) << setiosflags( std::ios::fixed ) << std::setprecision(3)	<< std::setw(4) << iter->Year;
		//Get beginning and end of this fire type.
		tranStart = iter->Year;
		if ((iter+1)!=Fire::fireTransitions.end()) tranStop = (iter+1)->Year-1;
		if (tranStop<=0 || gMaxYear<tranStop) tranStop = gMaxYear; 
		//Set up each fire type.
		if (iter->Type==Fire::FIXED){
			stream	<< " FIXED";
			if (iter->Ignition==FRS_NULL)				throw Exception(Exception::INITFAULT,"Fire transition missing ignition.\n");
			if (iter->Sensitivity==FRS_NULL)			throw Exception(Exception::INITFAULT,"Fire transition missing sensitivity.\n");
		}
		else if (iter->Type==Fire::SPATIAL) {
			stream	<< " SPATIAL";
			if (iter->SpatialIgnitionFile=="\"")			throw Exception(Exception::INITFAULT,"Fire transition missing spatial ignition file.\n");
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
	ReadGISFile<int>(_pTopoSpatialInput, gNumRows, gNumCol, _topoInputFile,std::ios::in, 0);
	ShowOutput(MODERATE, "\tReading site layer.\n");
	ReadGISFile<float>(_pSiteSpatialInput, gNumRows, gNumCol, _siteInputFile,std::ios::in, 0.);
	ShowOutput(MODERATE, "\tReading tree density layer.\n");
	ReadGISFile<int>(_pTreeDensitySpatialInput, gNumRows, gNumCol, _treeDensityInputFile,std::ios::in, 0);
}

void CustomLandscape::		repStart() 
//Processing at the start of a replicate.
{
	ShowOutput(MODERATE, "\tLandscape Rep setup\n");
	ShowOutput(MODERATE, "\t\tCreating landscape of " + ToS(gNumRows) + " rows by " + ToS(gNumCol) + " cols.\n");


    //Read in layers that might be unique per rep.
	if (_isUsingUniqueVegAndAgePerRep) {
		//Veg
		std::string inputFileWithRepYear = AppendRepYear(_vegInputFile, gRep,_yearOfUniqueInputPerRep);
		ShowOutput(MODERATE, "\t\tReading veg layer from "+inputFileWithRepYear+"\n");
		ReadGISFile<int>(_pVegSpatialInput, gNumRows, gNumCol, inputFileWithRepYear, std::ios::in,gNoVegID);
		//Age
		inputFileWithRepYear = AppendRepYear(_ageInputFile, gRep, _yearOfUniqueInputPerRep);
		ShowOutput(MODERATE, "\t\tReading age layer from "+inputFileWithRepYear+"\n");
		ReadGISFile<int>(_pAgeSpatialInput, gNumRows, gNumCol, inputFileWithRepYear,std::ios::in, 1);
	}
	else if (FRESCO->isRunningFirstRep()) {  //Use one input map for all reps.
		//Veg
		ShowOutput(MODERATE, "\t\tReading veg layer from "+_vegInputFile+"\n");
		ReadGISFile<int>(_pVegSpatialInput, gNumRows, gNumCol, _vegInputFile, std::ios::in, gNoVegID);
		//Age
		ShowOutput(MODERATE, "\t\tReading age layer from "+_ageInputFile+"\n");
		ReadGISFile<int>(_pAgeSpatialInput, gNumRows, gNumCol, _ageInputFile, std::ios::in, 1);
	}


	//Burn Severity	
	if (_isUsingUniqueBurnSeverityPerRep && _burnSeverityInputFile != "") {
		//Use unique map per rep.
		std::string inputFileWithRepYear = AppendRepYear(_burnSeverityInputFile, gRep,_yearOfUniqueInputPerRep);
		ShowOutput(MODERATE, "\t\tReading burn severity layer from "+inputFileWithRepYear+"\n");
		ReadGISFile<int>(_pBurnSeveritySpatialInput, gNumRows, gNumCol, inputFileWithRepYear, std::ios::in, 3);
	}
	else if (FRESCO->isRunningFirstRep()) {  
		//Use one input map for all reps.
		std::string message = _burnSeverityInputFile==""? 
			"\t\tSetting burn severity to default, HighLSS (level 3), for all cells.\n" :
			"\t\tReading burn severity layer from "+_burnSeverityInputFile+"\n";
		ShowOutput(MODERATE, message);
		ReadGISFile<int>(_pBurnSeveritySpatialInput, gNumRows, gNumCol, _burnSeverityInputFile, std::ios::in, 3);
	}	
 
	//Create landscape cell-by-cell, assigning values from all the input 
	Frame* pFrame = 0;
	for (int r=0; r<gNumRows; r++) {
		for (int c=0; c<gNumCol; c++) {
			pFrame = _pFrames[r][c];
			if (pFrame) delete pFrame;			
				 if (_pVegSpatialInput[r][c]==gBSpruceID)	{ _pFrames[r][c] = new BSpruce(-_pAgeSpatialInput[r][c], _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (_pVegSpatialInput[r][c]==gWSpruceID)	{ _pFrames[r][c] = new WSpruce(-_pAgeSpatialInput[r][c], _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (_pVegSpatialInput[r][c]==gDecidID)	    { _pFrames[r][c] = new Decid(-_pAgeSpatialInput[r][c],   _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
			else if (_pVegSpatialInput[r][c]==gTundraID)	{ _pFrames[r][c] = new Tundra(-_pAgeSpatialInput[r][c],  _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID, _pTreeDensitySpatialInput[r][c]);}
			else if (_pVegSpatialInput[r][c]==gNoVegID)	    { _pFrames[r][c] = new NoVeg(-_pAgeSpatialInput[r][c],   _pTopoSpatialInput[r][c]>0, _pSiteSpatialInput[r][c], -1, _pBurnSeveritySpatialInput[r][c], _pIgnitionFactorSpatialInput[r][c], _pSensitivitySpatialInput[r][c], gNoVegID);}
            else								            { throw Exception(Exception::INITFAULT, "Unknown vegetation type at cell [" + ToS(r) + "][" + ToS(c) + "]"); }
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
	if (gYear==0) transition = Fire::fireTransitions.begin();
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
			for (int r=0; r<gNumRows; r++) {
				for (int c=0; c<gNumCol; c++) {
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
				ShowOutput("\t\t\tIgnition file = " + transition->SpatialIgnitionFile + "\n");
				ShowOutput("\t\t\tSensitivity file = " + transition->SpatialSensitivityFile + "\n");
			}
			//Read spatial data files.
			ReadGISFile<float>	(_pIgnitionFactorSpatialInput, gNumRows, gNumCol, transition->SpatialIgnitionFile, std::ios::in, transition->Ignition);
			ReadGISFile<float>	(_pSensitivitySpatialInput, gNumRows, gNumCol, transition->SpatialSensitivityFile, std::ios::in, transition->Sensitivity);
			for (int r=0; r<gNumRows; r++) {
				for (int c=0; c<gNumCol; c++) {
					pFire = (Fire*)_pFrames[r][c];
					pFire->fireIgnitionFactor = _pIgnitionFactorSpatialInput[r][c];
					pFire->fireSensitivity = _pSensitivitySpatialInput[r][c];
				}
			}
			break;
		case Fire::HISTORICAL :
			ShowOutput(MODERATE, "\t\tFire Transition changed to HISTORICAL.\n");
			ShowOutput(MAXIMUM, "\t\t\thistorical file = " + transition->HistoricalFile + "\n");
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
		ShowOutput(MAXIMUM, "\t\t\tReading in historical fire map: " + filename + "\n");
		ReadGISFile<int>(_pHistoricalFireSpatialInput, gNumRows, gNumCol, filename.c_str(), std::ios::in, 0);
		//
		// Force cells to burn as specified in the file...
		//
		Frame*	pFrame		= 0;
		int		fireSize	= 0;
		int		numLow		= 0;
		int		numHiLSS	= 0;
		for (int r=0; r<gNumRows; r++) {
			for (int c=0; c<gNumCol; c++) {
				if (_pHistoricalFireSpatialInput[r][c]>0) {
					if (_pFrames[r][c]->type() != gNoVegID) {
						pFrame = _pFrames[_row=r][_col=c]; // <--be careful, spaghetti code: _row and _col are set here so Landscape::logFireStats() can access the species of this frame.  Should be cleaned.
						pFrame->historicalFire(this);
						fireSize++;
						//Set burn severity...
						if (pFrame->type() == gTundraID) { pFrame->burnSeverity=Fire::LOW; numLow++; }
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
			return _pSuppressionClasses[_pSuppressions[_row][_col]];
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
            if (!FileExistsInBaseDirectory(filepath)) 
                throw Exception(Exception::INITFAULT, "Expected suppression map for year " + ToS(t.Year) + " at " + filepath);
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
    if (it->Year != 0) throw Exception(Exception::INITFAULT,"A suppression trasition is required at year zero.\n");
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
    if (transition->HasNewMap || transition->Year==0) { 
        std::string filename(AppendYear(_suppressionFilename));
	    if (FileExistsInBaseDirectory(filename)) {
		    ShowOutput(MAXIMUM, "\t\t\tProcessing suppression file: " + filename + ".\n");
            //if (_pSuppressions != 0)  { for (int r=0;r<gNumRows;r++) delete[] _pSuppressions[r];  delete[] _pSuppressions;  _pSuppressions = 0; }
		    ReadGISFile<int>(_pSuppressions, gNumRows, gNumCol, filename.c_str(), std::ios::in, 0);
        } 
        else throw Poco::Exception("Expected suppression map for year " + ToS(gYear) + " at " + filename);
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
	int		curType		= 0;
	Frame*	pNewFrame	= 0;
	int		newType		= 0;
	std::string	filename;

	if (_isForcedVegTransitions) {
		//Not running large memory model: read in historical fire now.
		filename = _vegTransitionFile;
		filename = filename.substr(0,filename.size()-4) + "_" + ToS(gYear) + ".txt";
		//Assume file does not exist on exception.
		try {ReadGISFile<int>(_pVegSpatialInput, gNumRows, gNumCol, filename.c_str(), std::ios::in, -21);}
		catch (...) {return;}
		ShowOutput(MODERATE, "\t\tProcessing Vegetation Transitions\n");
		ShowOutput(MAXIMUM, "\t\t\tReading in veg transition file: " + filename + "\n");
		//Force cells to succeed to new types.
		for (r=0; r<gNumRows; r++) {
			for (c=0; c<gNumCol; c++) {
				if (_pVegSpatialInput[r][c]!=-21) {
					pCurFrame	= _pFrames[r][c];
					curType		= pCurFrame->type();
					newType		= _pVegSpatialInput[r][c];
					if (newType!=curType) {
						//Get the new frame type.
						if (newType==gBSpruceID)		pNewFrame = new BSpruce(*pCurFrame);
						else if (newType==gWSpruceID)	pNewFrame = new WSpruce(*pCurFrame);
						else if (newType==gDecidID)		pNewFrame = new Decid(*pCurFrame);
						else if (newType==gTundraID)	pNewFrame = new Tundra(*pCurFrame);
						else if (newType==gNoVegID)		pNewFrame = new NoVeg(*pCurFrame);
						else							throw Exception(Exception::UNKNOWN,"Failed vegetation transition.  Cell (" + ToS(r) + "," + ToS(c) + ") has unknown type (" + ToS(newType) + ".\n");
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
}


void CustomLandscape::		fillArray(LayerType layerType, std::vector< std::vector<double> >& rVector)
{
	Frame* pFrame = 0;
    if (FRESCO->getState()<Fresco::SETUP) return;
	else if (FRESCO->getState()==Fresco::SETUP)
	{
		if (layerType==VEGETATION)
			for (int r=0;r<gNumRows;r++)
				for (int c=0;c<gNumCol;c++) rVector[r][c]=_pVegSpatialInput[r][c];
	}
	else if (FRESCO->getState()>Fresco::SETUP)
	{
		switch(layerType) 
		{
		case FIRE:
			//0=NoFire  1=NaturalFireOrigin  2=NaturalFire 64=HumanFireOrigin  65=HumanFire 
			for (int r=0;r<gNumRows;r++) 
			{
				for (int c=0;c<gNumCol;c++) 
				{ 
					pFrame = _pFrames[r][c];
					//If burn occured...
					if (gYear==pFrame->yearOfLastBurn) 
					{
						//If natural cause...
						if (pFrame->lastBurnCause==Fire::NATURAL)	rVector[r][c] = (pFrame->lastBurnWasOrigin?1:2); 
						//If human cause...
						else if (pFrame->lastBurnCause==Fire::HUMAN) rVector[r][c] = (pFrame->lastBurnWasOrigin?64:65); 
					}
					else 
					{
						//No burn occured.
						rVector[r][c] = 0;
					}
				}
			}
			break;
		case FIRESUPPRESSION:
			//Show fire layer for any burned cells, otherwise show suppression classes.
			//0=NoFire  1=NaturalFireOrigin  2=NaturalFire 64=HumanFireOrigin  65=HumanFire 
			for (int r=0;r<gNumRows;r++) 
			{
				for (int c=0;c<gNumCol;c++) 
				{ 
					pFrame = _pFrames[r][c];
					//If burn occured...
					if (gYear==pFrame->yearOfLastBurn) 
					{
						//If natural cause...
						if (pFrame->lastBurnCause==Fire::NATURAL)	rVector[r][c] = (pFrame->lastBurnWasOrigin?1:2); 
						//If human cause...
						else if (pFrame->lastBurnCause==Fire::HUMAN) rVector[r][c] = (pFrame->lastBurnWasOrigin?64:65); 
					}
					else 
					{
						//No burn occured.  Show Suppression level.
						if (_isFireSuppressionOn)
						{
							switch(_pSuppressions[r][c])	//switch on suppression classes.
							{
							case 0:	//Class o is no suppression so show color 0: white.
								rVector[r][c] = 0;  
								break;
							case 1:
								rVector[r][c] = 251;  
								break;
							case 2:
								rVector[r][c] = 252;  
								break;
							case 3:
								rVector[r][c] = 253;  
								break;
							case 4:
								rVector[r][c] = 254;  
								break;
							case 5:
								rVector[r][c] = 255;  
								break;							
							}
						}
					}
				}
			}
			break;
		case FIREAGE:
			//0=NoFire  1=NaturalFireOrigin  2-63=NaturalFire 64=HumanFireOrigin  65-126=HumanFire (62 shade values for each fire type plus the origin value)
			int burnAge;
			for (int r=0;r<gNumRows;r++) 
			{
				for (int c=0;c<gNumCol;c++) 
				{
					pFrame = _pFrames[r][c];
					burnAge = gYear-pFrame->yearOfLastBurn;
					//If no burn history on record...
					if (pFrame->yearOfLastBurn<0) rVector[r][c] = 0;
					//If natural burn on record...
					else if (pFrame->lastBurnCause==Fire::NATURAL) 
						rVector[r][c] = pFrame->lastBurnWasOrigin? (burnAge<256?1:0)  : (burnAge<256?burnAge*0.239f+2:0);
					//If human burn on record...
					else if (pFrame->lastBurnCause==Fire::HUMAN) 
						rVector[r][c] = pFrame->lastBurnWasOrigin? (burnAge<256?64:0) : (burnAge<256?burnAge*0.239f+65:0);
				}
			}
			break;
		case AGE:
			for (int r=0;r<gNumRows;r++)
				for (int c=0;c<gNumCol;c++) rVector[r][c]=_pFrames[r][c]->age();
			break;
		case VEGETATION:
			for (int r=0;r<gNumRows;r++)
				for (int c=0;c<gNumCol;c++) rVector[r][c]=_pFrames[r][c]->type();
			break;
		case SUBCANOPY:
			for (int r=0;r<gNumRows;r++)
				for (int c=0;c<gNumCol;c++) rVector[r][c]=_pFrames[r][c]->speciesSubCanopy();
			break;
		case SITE:
			for (int r=0;r<gNumRows;r++)
				for (int c=0;c<gNumCol;c++) rVector[r][c]=_pFrames[r][c]->site();
			break;
		default:
			for (int r=0;r<gNumRows;r++)
				for (int c=0;c<gNumCol;c++) rVector[r][c]=0;
			break;
		}
	}
}


void CustomLandscape::      setupSuppressionStats()
{
    _burnPartitionBySuppClassStatFlags = FRESCO->fif().nGet("Stat.BurnsBySupp.Flags");
    _burnPartitionBySuppClassStats.resize(NUM_SUPPRESSION_CLASSES+1);
	for (int s=1; s<=NUM_SUPPRESSION_CLASSES; s++) {
		_burnPartitionBySuppClassStats[s].setup("BurnsBySupp["+ToS(s)+"]",	gMaxYear,gMaxRep,gTimeStep, _burnPartitionBySuppClassStatFlags, false);
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
            habitat.Stat.setup("Habitat"+ToS(pTypes[i]), gMaxYear, gMaxRep, gTimeStep, flags, false);
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
				filename = std::string(iter->File);
				filename = gOutputDirectory + Poco::Path::separator() + filename.substr(0,filename.size()-4) + "_" + ToS(gRep) + "_" + ToS(gYear) + ".txt";
				saveMaps(filename,iter->Flags);	//Output the data.
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
    for (int r=0; r<gNumRows; r++) {
        for (int c=0; c<gNumCol; c++) {
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
        _burnPartitionBySuppClassStats[s].m_lTally = 0;
    }
    for (habitat = _habitatStats.begin(); habitat<_habitatStats.end(); habitat++) {
        habitat->Stat.Add(gYear,gRep);
        habitat->Stat.m_lTally = 0;
    }

    //Timing.
    //sw.stop();
    //std::cout << "Time to collectStats():" << sw.elapsed() << std::endl;
}
