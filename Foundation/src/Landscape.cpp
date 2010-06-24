//Landscape.cpp
//This module contains the code relevent to the Landscape class.  This is a fairly widely varied range
//of functions, but includes the basic ones which would be expected to operate on any generic landscape
//class.  This includes constructor and destructor (of course), internal worker functions to aid in
//creating and writing stats variables, and public functions for probing neighbors, getting climate
//information, setting the output flags, and logging the fire statistics.  The general large scale
//functionality should be provided in the child class Landscape (including replicate initialization,
//succession, fire regime, and steps to iterate the internal state).


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Landscape.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/Stat.h"
#include "Fresco/Foundation/Fire.h"
#include "Fresco/Foundation/Frame.h"
#include "Fresco/Foundation/Interface.h"
#include "Fresco/Foundation/NoVeg.h"
#include "Fresco/Foundation/Climate.h"
#include "Fresco/Foundation/Except.h"
#include <stack>
#include <limits>

//Declare  static variables
int				Frame::_outFlags					= 0;				//A flag indicating which data to output when the object is written
bool			Landscape::_cropNeighbors			= false;
std::string		Landscape::_humanIgnitionsFilename	= "";
double			Landscape::_xllCorner                 =.0;
double			Landscape::_yllCorner                 =.0;


struct SNeighbor
{
    int Row,Col;		//Row and column position of this neighbor.
	int HQRow,HQCol;	//Row and column position of the center of the neighborhood that this neighbor was created in.   
	SNeighbor() : Row(0), Col(0), HQRow(0), HQCol(0) {};
};


Landscape::				Landscape (const int rows, const int cols) 
{
	_pClimate		    = new Climate();
	_pFrames			= 0;
	_pHumanIgnitions	= 0;
	_pfireSpreadParams  = 0;
}


Landscape::				~Landscape() 
{
	//Delete leftovers.
	delete _pClimate;
	if (_pHumanIgnitions)	{for (int r=0;r<gNumRows;r++) delete[] _pHumanIgnitions[r];	delete[] _pHumanIgnitions;	_pHumanIgnitions = 0; }
    //TODO: delete _pFrames[][]?
}


void Landscape::		clear()
//Clear existing run if any and return to before a run is specified.
{
	//Clear the static members of species types.
	Fire::clear();
	Frame::clear();
	NoVeg::clear();

	//Clear Landscape settings.
	_humanIgnitionsFilename	= "";
	_cropNeighbors			= false;
	_xllCorner				= 0;
	_yllCorner				= 0;
	_row					= 0;
	_col					= 0;
	//Clear cells.
 	if (_pFrames)					{
        for (int r=0;r<gNumRows;r++) {
            for (int c=0;c<gNumCol;c++) 
                delete _pFrames[r][c]; 
            delete[] _pFrames[r];
        }	
        delete[] _pFrames; _pFrames = 0;
    }
	//Clear layers.
	if (_pHumanIgnitions)	{ for (int r=0;r<gNumRows;r++) delete[] _pHumanIgnitions[r];	delete[] _pHumanIgnitions;	_pHumanIgnitions = 0; }
	if (_pfireSpreadParams) { delete[] _pfireSpreadParams; _pfireSpreadParams = 0; }
	//Clear stats.
	_vegDistributionStatFlags	= 0;
	_vegResidenceStatFlags		= 0;
	_fireSpeciesStatFlags		= 0;
	_fireIntervalStatFlags		= 0;
	_fireSizeStatFlags			= 0;
	_fireNumStatFlags			= 0;
	_vegDistributionStat.clear();
	_vegResidenceStat.clear();
	_fireSpeciesStat.clear();
	_fireIntervalStat.clear();
	_fireSizeStat.clear();
	_fireNumStat.clear();
    //Clear climate.
	_pClimate->clear();
}


void Landscape::		setup()
//setup a run.
{
    //Get variables from FIF.
    gNumRows				    = FRESCO->fif().nGet("MaxRow");
    gNumCol				        = FRESCO->fif().nGet("MaxCol");
    gCellSize				    = FRESCO->fif().dGet("CellSize");
    _cropNeighbors		        = FRESCO->fif().bGet("CropNeighbors");
    _xllCorner		            = FRESCO->fif().dGet("XLLCorner");
    _yllCorner			        = FRESCO->fif().dGet("YLLCorner");
    _vegDistributionStatFlags	= FRESCO->fif().nGet("Stat.VegDist.Flags");
    _vegResidenceStatFlags	    = FRESCO->fif().nGet("Stat.VegResidence.Flags");
    _fireSpeciesStatFlags	    = FRESCO->fif().nGet("Stat.FireSpecies.Flags");
    _fireIntervalStatFlags	    = FRESCO->fif().nGet("Stat.FireInterval.Flags");
    _fireSizeStatFlags		    = FRESCO->fif().nGet("Stat.FireSize.Flags");
    _fireNumStatFlags		    = FRESCO->fif().nGet("Stat.FireNum.Flags");
    
    // Should this be in Fire?
    _humanIgnitionsFilename     = FormatDirectory(FRESCO->fif().sGet("Fire.HumanIgnition.Basename"));
	_pfireSpreadParams = new double[3];
	_pfireSpreadParams[0] = 0.; //The first value is set to the cell's distance from a neighbor in each call to testFireSpread.
	_pfireSpreadParams[1] = Fire::fireSpreadMean();
	_pfireSpreadParams[2] = Fire::fireSpreadStdDev(); 
    
    
	//Allocate cells.
	_row = _col = 0;
	_pFrames = new Frame**[gNumRows];
	for (int r=0; r<gNumRows; r++) 
	{
		_pFrames[r] = new Frame*[gNumCol];
		for (int c=0; c<gNumCol; c++)
			_pFrames[r][c] = NULL;
	}
	//Allocate layers.
	_pHumanIgnitions = new int*[gNumRows];
	for(int r=0; r<gNumRows; r++) {
		_pHumanIgnitions[r] = new int[gNumCol];
		for(int c=0; c<gNumCol; c++) {
			_pHumanIgnitions[r][c]	= 0;
		}
	}
	//setup climate.
	_pClimate->setup();
	//Allocate stats.
	_vegDistributionStat.resize(gNumSpecies);
	_vegResidenceStat.resize(gNumSpecies);
	_fireSpeciesStat.resize(gNumSpecies);
	_fireIntervalStat.resize(gNumSpecies);
	for (int s=0; s<gNumSpecies; s++) 
	{
		_vegDistributionStat[s].setup("VegDist["+ToS(s)+"]",	gMaxYear,gMaxRep,gTimeStep, _vegDistributionStatFlags, false);
		_vegResidenceStat[s].setup("VegRes["+ToS(s)+"]",		gMaxYear,gMaxRep,gTimeStep, _vegResidenceStatFlags, false);
		_fireSpeciesStat[s].setup("FireSpecies["+ToS(s)+"]",	gMaxYear,gMaxRep,gTimeStep, _fireSpeciesStatFlags, false);
		_fireIntervalStat[s].setup("FireInterval["+ToS(s)+"]",	gMaxYear,gMaxRep,gTimeStep, _fireIntervalStatFlags, false);
	}
	_fireSizeStat.setup("FireSize", gMaxYear, gMaxRep, gTimeStep, _fireSizeStatFlags, true);
	_fireNumStat.setup("FireNum", gMaxYear, gMaxRep, gTimeStep, _fireNumStatFlags, false);
}


void Landscape::		runEnd()
//Finish up this run but do not clear.
{
	_pClimate->runEnd();
}


void Landscape::		repStart() 
//Processing at the start of a replicate.
{
	_pClimate->repStart();
	//Set veg distribution stats to zero.
	for (int s=0; s<gNumSpecies; s++) {
		_vegDistributionStat[s].m_lTally = 0;
	}
	//Set initial veg distribution (will change in each year's successions).
	for (int r=0; r<gNumRows; r++) {
		for (int c=0; c<gNumCol; c++) {
			_vegDistributionStat[_pFrames[r][c]->type()]++;
		}
	}
	//Do other odds and ends
	_row = _col = 0;
}


void Landscape::		repEnd() 
//Processing at the end of a replicate.
{
	_pClimate->repEnd();
}


void Landscape::		yearStart() 
//Start of a year processing.
{
	//Tell climate to do any start of year processing.
	_pClimate->yearStart();
	//Reset fire species stats.
	for (int s=0; s<gNumSpecies; s++) {
		_fireSpeciesStat[s].m_lTally = 0;
	}
}


void Landscape::		yearEnd() 
//Processing at the end of a year.
{
	for (int s=0;s<gNumSpecies;s++) {
		_vegDistributionStat[s].Add(gYear, gRep);	//Store the species distribtuion tally.
		_fireSpeciesStat[s].Add(gYear,gRep);	    //Store the species that burned tally.
	}
	_pClimate->yearEnd();
}


void Landscape::		succession() 
//This function controls succession at the landscape level.  Since succession is purely a cell
//level function, this algorithm steps through each cell on the landscape and calls 
//Frame::success to see if the cell goes through a successional transition.  It if does, the
//actual transition of pointers is a landscape level job and is executed.  Also, relevent
//statistics are updated in the event of a transfer.
{
	Frame* pSuccFrame;
	for (int r=0; r<gNumRows; r++) {
		for (int c=0; c<gNumCol; c++) {
			//Test for succession.
			if (NULL != (pSuccFrame=_pFrames[_row=r][_col=c]->success(this))) {
				//Decrement cell count for old species and increment cell count for new species.
				_vegDistributionStat[_pFrames[r][c]->type()]--;
				_vegDistributionStat[pSuccFrame->type()]++;
				//Update veg residence times.
				_vegResidenceStat[_pFrames[r][c]->type()].Add(gYear, gRep, abs(_pFrames[r][c]->frameAge()));
				//Process the succession.
				delete _pFrames[r][c];
				_pFrames[r][c] = pSuccFrame;
			}
		}
	}
}


void Landscape::		doIgnitions() 
{
	//Is there a human ignitions file for this year?
	std::string filename = AppendYear(_humanIgnitionsFilename);
	if (InputFileExists(filename)) {
      ShowOutput(MAXIMUM, "\t\t\tProcessing human ignitions file: " + GetFullPath(gInputBasePath, filename) + ".\n");
      ReadGISFile<int>(_pHumanIgnitions,gNumRows, gNumCol, filename.c_str(), std::ios::in, 0);
	}

	//Iterate over cells.
	//Note: r and c are not simply incremented each iteration.  They are manipulated to switch focus between 
	//evaluating burning neighborhoods and searching for new fires over the landscape.
	//When burnSpreadStack is empty, the loop is iterating over the landscape testing for a new ignition.
	//When burnSpreadStack is not empty, the loop is iterating over the "neighborhoods" testing for fire spread. 
	Frame*	            pFrame	            = 0;				// Pointer to the current cell's frame.
	int					rowStored			= 0;
	int					colStored			= 0;
	int					rowMin				= 0;				// Min and max position values to define neighborhood boundries.
	int					rowMax				= 0;				// "
	int					colMin				= 0;				// "
	int					colMax				= 0;				// "
	int					currRow			    = 0;				// When looping over neighborhoods, this is the position of the neighborhood's center cell.
	int					currCol			    = 0;				// "

	int					fireScarID			= 0;				// ID of a single fire ignition and its spread.
	int					fireSize			= 0;				// Size of each individual fire cluster.
	int*				severitySizes		= new int[5];		// Tallies of for each severity level.
	severitySizes[0]=0; severitySizes[1]=0; severitySizes[2]=0; severitySizes[3]=0; severitySizes[4]=0;
	int					fireSizeTotal		= 0;				// Sum of all fire sizes.
	int					fireNum			    = 0;				// Tally of individual fire clusters.

    bool				isTestingSpread		= false;			// True only when an initial cell has ignitied and we are spreading fire.
	bool				isNaturalIgnition;
	bool				isHumanIgnition;
	SNeighbor			neighbor;
	std::stack<SNeighbor> burnSpreadStack;						// Stack of cells to be tested for a fire spread
	Fire::ECause		currentBurnCause	= Fire::NATURAL;
	
    _maxFireSizeEventWeight = 1;								    //Cap individual fire sizes to with a max empirical fire size.
	int numCells = (int)ceil(Fire::fireSpreadRadius()/gCellSize);   // The width of the neighborhood's radius.

	_row = _col = 0;
	while (_row<gNumRows && _col<gNumCol) {
		pFrame = _pFrames[_row][_col];	
		if (pFrame->yearOfLastBurn!=gYear && pFrame->type()!=gNoVegID) {
			//Current cell is burnable and hasn't burned yet this year.

            //Test for fire on this frame (4 possibilities: noFire, new human caused, new natural caused, spread from existing fire).
			isHumanIgnition = testHumanIgnition(pFrame);
			if (!isHumanIgnition) {
				if (isTestingSpread)
					isNaturalIgnition = testFireSpread(pFrame, currRow, currCol, getCustomFireSpreadMultiplier(_row, _col, fireSizeTotal, fireNum));
                else
					isNaturalIgnition = testNaturalIgnition(pFrame);
			}
            
			if (isNaturalIgnition || isHumanIgnition) {
                //Prepare for new fire or existing fire to spread.
				if (!isTestingSpread) { //New fire.
					fireScarID++;
					currentBurnCause = (isHumanIgnition ? Fire::HUMAN : Fire::NATURAL);
					pFrame->burnSeverity = Fire::LOW;
					severitySizes[Fire::LOW]++;
					pFrame->lastBurnWasOrigin = true;
					rowStored = _row;   //Remember where to pick up when this fire is done spreading.
					colStored = _col;
				}
				else { //Existing (spreading) fire.
					pFrame->lastBurnWasOrigin = false;
					const Frame* pSpreaderFrame = _pFrames[currRow][currCol];
					pFrame->burnSeverity = selectSpreadBurnSeverity(pFrame, pSpreaderFrame, fireSize);
					severitySizes[pFrame->burnSeverity]++;
				}
                ////////////////////////////////////////////////
                //Burn this frame and spread to its neighbors.
				pFrame->fireScarID = fireScarID;
				logFireStats((pFrame->yearOfLastBurn<0 ? -gYear : gYear-pFrame->yearOfLastBurn), Fire::ignoringFirstFireInterval());    //Must preceed resetting of this cell's age.
				pFrame->yearOfLastBurn = gYear;																						
				pFrame->lastBurnCause = currentBurnCause;
				fireSize++;
				if (fireSize > Fire::maxEmpiricalFireSizeEvent())   //Cap individual fire sizes with a max empirical fire size.
					_maxFireSizeEventWeight = Fire::maxEmpiricalFireSizeEventWeight();
				//Find the row and column bounds to avoid range checking in the inner loop.
				rowMin = _row - numCells;		rowMin = (rowMin < 0) ? 0 : rowMin;
				rowMax = _row + numCells;		rowMax = (rowMax >= gNumRows) ? gNumRows-1 : rowMax;
				colMin = _col - numCells;		colMin = (colMin < 0) ? 0 : colMin;
				colMax = _col + numCells;		colMax = (colMax >= gNumCol) ? gNumCol-1 : colMax;
				//Add unburned neighbors to burn list.  Add to stack in reverse order so they are pulled off in correct order.
				for (int row=rowMax; row>=rowMin; row--) {
					for (int col=colMax; col>=colMin; col--) {
						if (_pFrames[row][col]->yearOfLastBurn!=gYear) {
							neighbor.Row	= row;
							neighbor.Col	= col;
							neighbor.HQRow	= _row;
							neighbor.HQCol	= _col;
							burnSpreadStack.push(neighbor);
						}
					}
				}
			}
		}
		
		if (burnSpreadStack.empty()) {  
            //No more neighbors to test in this fire.
			isTestingSpread = false;
			_maxFireSizeEventWeight = 1;    //Stop applying fire size cap wieght.
			if (fireSize>0) {  
                //This fire burned frames.  Record fire stats.
				
				_fireSizeStat.Add(gYear, gRep, fireSize, currentBurnCause==Fire::HUMAN?1:0, severitySizes[Fire::LOW], severitySizes[Fire::MODERATE], severitySizes[Fire::HIGH_LSS], severitySizes[Fire::HIGH_HSS]);
				
				fireSizeTotal += fireSize;
				fireNum++;
				fireSize = 0;
				severitySizes[0]=0; severitySizes[1]=0; severitySizes[2]=0; severitySizes[3]=0; severitySizes[4]=0;
			}	
			
            //Continue testing for new fires at next landscape cell.
			_col = currCol =  ++colStored;
			if (_col<gNumCol) {
				_row = currRow = rowStored;
			}
			else {
				_col = colStored = 0;
				_row = currRow = ++rowStored;
			}
		}
		else {
			isTestingSpread = true; //Testing for spread now, not ignitions.
			//Prepare to test fire spread on next frame on stack.
			_row	= burnSpreadStack.top().Row;
			_col	= burnSpreadStack.top().Col;
			currRow	= burnSpreadStack.top().HQRow;
			currCol	= burnSpreadStack.top().HQCol;
			burnSpreadStack.pop();
		}

	}
	_fireNumStat.Add(gYear, gRep, fireNum);
	delete[] severitySizes;
}


bool Landscape::		testNaturalIgnition(Frame* pFrame)
//Worker function for doIgnitions().  Returns true if the frame ignites.
{
	float test =  pFrame->fireIgnitionFactor * pFrame->getFireProb(this) * pFrame->getIgnitionDepressor();
	return (test > GetNextRandom());
}


bool Landscape::		testHumanIgnition(const Frame* pFrame)
//Worker function for doIgnitions().  Returns true if the frame ignites.
{
	//Was a human ignitions map provided for this year?
	if (_pHumanIgnitions) { 
		//0 represents no human ignition, anything else represents a human ignition.
		if (_pHumanIgnitions[_row][_col]!=0)
			return (pFrame->getHumanIgnitionProb() > GetNextRandom());
	}
	return false;
}


bool Landscape::        testFireSpread(Frame* pFrame, const int rowOfNeighbor, const int colOfNeighbor, const float fireSuppressionFactor)
//Worker function for doIgnitions().  Returns true if the frame burns due to spread.
{
	_pfireSpreadParams[0] = getCellDistanceToNeighbor(rowOfNeighbor, colOfNeighbor);
	bool isInSpreadRadius = !(_cropNeighbors && _pfireSpreadParams[0]>Fire::fireSpreadRadius());

	if (isInSpreadRadius) {
		//Spread fire from burning neighbor?
		double test	= pFrame->getFireProb(this) * pFrame->fireSensitivity * NormDist(_pfireSpreadParams) * fireSuppressionFactor  * _maxFireSizeEventWeight;
		return (test > GetNextRandom());
	}
	else
		return false;
}


Fire::EBurnSeverity  Landscape::selectSpreadBurnSeverity(const Frame* pFrame, const Frame* pSpreaderFrame, const int fireSize)
{
	if (pFrame->type() == gTundraID) return Fire::LOW;
	else if (pFrame->type() == gDecidID) return pSpreaderFrame->burnSeverity;
	// else BSpruce or WSpruce continue...

	// Test for spatial corellation (a.k.a: should severity be adopted from spreading cell?)
	const float test = GetNextRandom();
	if (!pFrame->hasComplexTopo() && test<Fire::burnSeveritySettings.FlatTopoWeight  
	  || pFrame->hasComplexTopo() && test<Fire::burnSeveritySettings.ComplexTopoWeight)
	{
		return pSpreaderFrame->burnSeverity;
	}
	
	// Didn't inherit from spreader frame, so select severity using the following logic...
	const double a = Fire::burnSeveritySettings.FxnIntercept;
	const double b = Fire::burnSeveritySettings.FxnSlope;
	const double ex = exp(a + b * fireSize); // possible overflow
	double highSevProb = ex / (1+ex); //no worries of divide-by-zero
	if (highSevProb != highSevProb) highSevProb = 1.0; // test for NaN due to overflow.
	if (highSevProb == std::numeric_limits<double>::infinity()) highSevProb = 1.0;
	if (GetNextRandom() < highSevProb)
	{   
		// ok, high crown severity, but what is the surface severity?  
		// Lower weight is more likely to give LSS
		if (GetNextRandom() < Fire::burnSeveritySettings.LssVsHssWeight) 
			return Fire::HIGH_HSS;
		else 
			return Fire::HIGH_LSS;    
	}
	else
	{
		// ok, not high crown severity, so either low or moderate...
		if (GetNextRandom() < Fire::burnSeveritySettings.LowVsModerateWeight)
			return Fire::MODERATE;
		else
			return Fire::LOW;
	}
}


float Landscape::       getCellDistanceToNeighbor(const int rowOfNeighbor, const int colOfNeighbor)
//Worker function for doIgnitions.  Returns the distance to the specified cell.
{
	int dY = rowOfNeighbor - _row;
	int dX = colOfNeighbor - _col;
	//Distance equation * cellsize.
	return sqrt((double)dY*dY + dX*dX) * gCellSize;
}


double Landscape::		neighborsSuccess (double (Frame::*QueryFunction)(Landscape *, const double, const double*), double (*WeightFunc)(const double* const), const double MaxDist, double *Parms, const double *FuncParms) 
//This function is the same as neighbors, except that it allows the user to pass an array of numbers
//to the query function - this is the second to last parameter.  All other parameters are the same.
{
	int		currRow	    = _row;														//These are necessary because they stay constant for a given frame neighborhood 
	int		currCol	    = _col;
	int		numCells	= (int) ceil(MaxDist/gCellSize);
	double	total		= 0;
	double	distance	= 0;															//If parms is NULL allocate some memory for it.  By doing it this way, garbage collection is automatic
	Frame*	pFrame		= 0;
	if (Parms == NULL)	
		Parms = &distance;
	//Find the row and column bounds to avoid range checking in the inner loop
	int rowMin = currRow - numCells;	rowMin = (rowMin < 0) ? 0 : rowMin;
	int rowMax = currRow + numCells;	rowMax = (rowMax >= gNumRows) ? gNumRows-1 : rowMax;
	int colMin = currCol - numCells;	colMin = (colMin < 0) ? 0 : colMin;
	int colMax = currCol + numCells;	colMax = (colMax >= gNumCol) ? gNumCol-1 : colMax;
	//Start the inner loop
	for (int row=rowMin; row<=rowMax; row++) {
		for (int col = colMin; col <= colMax; col++) {
			if ( (Parms[0] = sqrt((double)(currRow-row)*(currRow-row) + (currCol-col)*(currCol-col)) * gCellSize ) <= MaxDist || !_cropNeighbors)
				pFrame = _pFrames[_row=row][_col=col];
				if (pFrame->type()!=gNoVegID)
					total += (pFrame->*QueryFunction)(this, WeightFunc(Parms), FuncParms);
		}
	}
	_row = currRow;																//Need to reset these on exit for the next cell on the call stack
	_col = currCol;
	return total;
}


void Landscape::		logFireStats (int interval, bool ignoreFirstInterval) 
//This adds the frame type of the current cell to the fire by species statistical datastructure.  It
//is called when a cell burns and updates species specific statistical variables including the number
//of each species type that burned, and the fire interval for each species type.
{
	Species specSp(_pFrames[_row][_col]->type());
	_fireSpeciesStat[specSp]++;
	//Only update stats if it is the second time cell has burned to avoid startup bias
	if (interval>0 || !ignoreFirstInterval)
		_fireIntervalStat[specSp].Add(gYear, gRep, (interval > 0) ? interval : -interval);
}


void Landscape::		saveMaps(const std::string filePath, const int mapFlags)
//Save map files.
{
	if (mapFlags & outData) {
		//Open output stream.
        EnsureDirectoryExists(filePath, true);
		std::fstream sStream(filePath.c_str(), std::ios::out);
		if (!sStream.is_open()) throw Exception(Exception::FILEBAD,"Error opening output file:"+filePath+"\n");
		sStream.flags(std::ios::fixed);														//Ensure it doesn't bump up into scientific notation
		sStream.precision(1);
		//Output title with max years and max reps included.
		if (mapFlags & outHeader)
			sStream << "Model execution output (" << gMaxYear << "," << gMaxRep << ")" << std::endl;
		//Output ARCgis header to ensure formatting is correct for reading by ARC.
		if (mapFlags & outARC) {
			sStream << "ncols " << gNumCol << std::endl;
			sStream << "nrows " << gNumRows << std::endl;
			std::ios_base::fmtflags flags = sStream.flags();												//Store the current state of the output flags
			int nPrecision = sStream.precision();
			sStream.flags(std::ios::fixed);													//Set the state of the output flags
			sStream.precision(6);
			sStream << "xllcorner " << _xllCorner << std::endl;
			sStream << "yllcorner " << _yllCorner << std::endl;
			sStream << "cellsize " << gCellSize << std::endl;
			sStream.precision(0);
			sStream << "NODATA_value " << gNoDataID << std::endl;
			sStream.flags(flags);														//Return the flags to their previous state
			sStream.precision(nPrecision);
		}
		//Output data.
		if (mapFlags & outData) {
			int nMap = 0;																//Find out if more than one flag is set so we know if it can be a map file
			for (int nMask=0x100; nMask; nMask <<= 1)									//Ignore the first hex decimal which is the general flag set
				nMap += (nMask & mapFlags) ? 1 : 0;										//Basically just the number of true bits
			if (!(mapFlags & outFormat)) nMap = 2;										//If we aren't formatting the data, show it as rows - only useful in the case where there is only one bit of data because otherwise it will end up as rows anyway
            Frame::setOutFlagsForAllFrames(mapFlags);
			for (int r=0; r<gNumRows; r++) {
				for (int c=0; c<gNumCol; c++) {
					//Output cell.
					sStream << *_pFrames[r][c];
					//Output cell climate.
					int bFormat = mapFlags & outFormat;
					if (mapFlags & outTemp) {
						if (!bFormat) sStream << "Temp=";		
						sStream << _pClimate->getClimate(r,c).Temp;	
						if (!bFormat) sStream << "\t"; 
						else sStream << " ";	
					}
					if (mapFlags & outPrecip) {
						if (!bFormat) sStream << "Precip=";
						sStream << _pClimate->getClimate(r,c).Precip;
						if (!bFormat) sStream << "\t";
						else sStream << " ";
					}
					if (nMap > 1) sStream << std::endl;
				}
				sStream << std::endl;
			}
		}
		//Exit.
		sStream << std::endl;
		sStream.close();
	}
}

