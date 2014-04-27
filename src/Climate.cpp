/** 
 * @file
 * 
 * This module contains the code pertaining to the climate routines.  In particular it contains
 * the code to increment the climate to the next year as well as a number of worker routines
 * used for returning the current climate.
 */


#include "PreCompiled.h"
#include "Climate.h"
#include "Except.h"
#include "Fresco.h"
#include "RasterIO.h"
#include "Poco/Path.h"
#include <sstream>


//Declare static members.
std::vector<Climate::SClimateTransition> Climate::_transitions;


Climate::				Climate () 
{
	_pSpatialFlammability = 0;
	_pSpatialTemp		= 0;
	_pSpatialPrecip		= 0;
	_pOffsets	        = 0;
	_pRandomYears		= 0;
	_pCurrentTransition = new SClimateTransition();
	gClimate = this;
}


Climate::				~Climate () 
{
    deleteArrays();
    delete _pCurrentTransition;
}


void Climate::          deleteArrays()
{
    if (_pRandomYears)      { delete[] _pRandomYears; _pRandomYears = 0; }
	if (_pOffsets)			{ delete[] _pOffsets; _pOffsets = 0; }	
	std::list<int>::iterator m;
	if (_pSpatialTemp != 0) {
		for (int y=0;y<_yearsOfArchivedHistory;y++) { 
			for (m=tempMonths.begin(); m!=tempMonths.end(); m++) {
				if (&_pSpatialTemp[y][*m] != 0)
				{
					if (&_pSpatialTemp[y][*m][0] != 0)
						for (int r=0;r<gYSize;r++)
							delete[] _pSpatialTemp[y][*m][r];
					delete[] _pSpatialTemp[y][*m];
				}
			}
			delete[] _pSpatialTemp[y]; 
		} 
		delete[] _pSpatialTemp; _pSpatialTemp=0; 
	}	
	if (_pSpatialPrecip) { 
		for (int y=0;y<_yearsOfArchivedHistory;y++) { 
			for (m=precipMonths.begin(); m!=precipMonths.end(); m++) {
				if (&_pSpatialPrecip[y][*m] != 0)
				{
					if (&_pSpatialPrecip[y][*m][0] != 0)
						for (int r=0;r<gYSize;r++) 
							delete[] _pSpatialPrecip[y][*m][r];
					delete[] _pSpatialPrecip[y][*m];
				}
			}
			delete[] _pSpatialPrecip[y]; 
		} 
		delete[] _pSpatialPrecip; _pSpatialPrecip=0; 
	}
	if (_pSpatialFlammability) { 
		for (int r=0;r<gYSize;r++) 
			delete[] _pSpatialFlammability[r];
		delete[] _pSpatialFlammability;  _pSpatialFlammability=0;
	}
}


void Climate::			clear()
/** Clear existing run if any and return to before a run is specified. */
{
	ShowOutput(MODERATE, "Climate Clear \n");
    deleteArrays();
	clearTransition(_pCurrentTransition);
    _transitions.clear();
	_tempStep.clear();
	_precipStep.clear();
	_tempRamp.clear();
	_precipRamp.clear();
	precipMonths.clear();
	tempMonths.clear();
}


void Climate::			setup()
/** setup a run. */
{
    _yearsOfArchivedHistory  = 1; //FRESCO->fif().nGet("Climate.NumHistory");
	// Hard coded to 1 for now, because no where in code is using climate history, 
	// so there's no need in allowing users to unknowingly allocate unused memory. 
	
	if (_yearsOfArchivedHistory < 1) throw Poco::Exception("Climate.NumHistory must be 1 or greater.");
	_isExternFlam = FRESCO->fif().CheckKey(FRESCO->fif().root["Climate"]["Values"]["Flammability.File"]);
	setupTransitions();
    setupStepsAndRamps();

	if (_isExternFlam) {
		_pSpatialFlammability = new float*[gYSize];
		for (int r=0; r<gYSize; r++) {
			_pSpatialFlammability[r] = new float[gXSize];
			for (int c=0; c<gXSize; c++) {
				_pSpatialFlammability[r][c] = 0.;
			}
		}
		_spatialFlamabilityFile = FormatDirectory(FRESCO->fif().root["Climate"]["Values"]["Flammability.File"].asString());
		if (_spatialFlamabilityFile == "") 	throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing climate flammability file name.\n");
	}

	//Set monthly status.
	_isMonthlyClimate = false;
	if (FRESCO->fif().CheckKey(FRESCO->fif().root["Climate"]["IsMonthly"]))
		_isMonthlyClimate = FRESCO->fif().root["Climate"]["IsMonthly"].asBool();
	if (_isMonthlyClimate)
	{
		if (!_isExternFlam)
		{
			// Add months needed by internal climate fire prob equationA
			tempMonths.clear();
			precipMonths.clear();
			tempMonths.push_back(3);
			tempMonths.push_back(4);
			tempMonths.push_back(5);
			tempMonths.push_back(6);
			tempMonths.push_back(7);
			tempMonths.push_back(8);
			tempMonths.push_back(9);
			precipMonths.push_back(6);
			precipMonths.push_back(7);
			precipMonths.push_back(8);
			precipMonths.push_back(9);
		}
		//
		// Moved this chunk of code to Decid's setup due to the this 
		// function being called after the Decid setup... still sorting 
		// and removing duplicates here.
		//
		//if (!IsNodata(gGrasslandID))
		//{
		//	// Add months needed for Decid=>Grassland succession.
		//	const int* pPrecipMonths;
		//	const int* pTempMonths;
		//	int numMonths = FRESCO->fif().pnGet("Grassland.TempMonths", pTempMonths);
		//	for (int i=0; i<numMonths; i++)
		//		tempMonths.push_back(pTempMonths[i]);
		//	
		//	numMonths = FRESCO->fif().pnGet("Grassland.PrecipMonths", pPrecipMonths);
		//	for (int i=0; i<numMonths; i++)
		//		precipMonths.push_back(pPrecipMonths[i]);
		//}
		// Sort and remove duplicates.
		tempMonths.sort();
		tempMonths.unique();
		precipMonths.sort();
		precipMonths.unique();
	}
	else
	{	
		tempMonths.push_back(0);
		precipMonths.push_back(0);
	}
	//if (tempMonths.empty() && precipMonths.empty())
	//{
	//	// Either it's growing season climate (not monthly) or climate is set to monthly but no months were needed.
	//	tempMonths.push_back(0);
	//	precipMonths.push_back(0);
	//}

	_pOffsets = new SClimate[gLastYear - gFirstYear + 1];
	_pRandomYears = new int[gLastYear - gFirstYear + 1];  //Store for repStart();
	//Create spatial climate arrays.
	_pSpatialTemp = new float***[_yearsOfArchivedHistory];
	std::list<int>::iterator m;
	for (int y=0; y<_yearsOfArchivedHistory; y++) {
		_pSpatialTemp[y] = new float**[13];
		for (m=tempMonths.begin(); m!=tempMonths.end(); m++) {
			_pSpatialTemp[y][*m] = new float*[gYSize];
			for (int r=0; r<gYSize; r++) {
				_pSpatialTemp[y][*m][r] = new float[gXSize];
				for (int c=0; c<gXSize; c++)	{
					_pSpatialTemp[y][*m][r][c] = 0.;
				}
			}
		}
	}
	_pSpatialPrecip = new float***[_yearsOfArchivedHistory];
	for (int y=0; y<_yearsOfArchivedHistory; y++) {
		_pSpatialPrecip[y] = new float**[13];
		for (m=precipMonths.begin(); m!=precipMonths.end(); m++) {
			_pSpatialPrecip[y][*m] = new float*[gYSize];
			for (int r=0; r<gYSize; r++) {
				_pSpatialPrecip[y][*m][r] = new float[gXSize];
				for (int c=0; c<gXSize; c++)	{
					_pSpatialPrecip[y][*m][r][c] = 0.;
				}
			}
		}
	}

    //Show a summary of scheduled climate transitions.
    if (gDetailLevel>=MAXIMUM) {
		ShowOutput("\tClimate Transitions:\n");
		ShowOutput("\t\tYear ValuesType     OffsetsType\n");
	}
	std::string errMsg("A climate trasition is required at the first simulation year ("+ToS(gFirstYear)+").\n");
	std::vector<SClimateTransition>::iterator T;
	if (_transitions.empty())
		throw SimpleException(SimpleException::INITFAULT,errMsg,"");
    std::vector<SClimateTransition>::iterator iter = _transitions.begin();
	if (iter->Year != gFirstYear)
		throw SimpleException(SimpleException::INITFAULT,errMsg,"");
	
    std::ostringstream	Stream;
	for (iter=_transitions.begin(); iter!=_transitions.end(); iter++)	{
        //show year...
        Stream	<< "\t\t" << std::setiosflags( std::ios::left ) << std::setiosflags( std::ios::fixed ) << std::setprecision(3)	<< std::setw(4) << iter->Year << std::setw(15);
        //show ValueType...
		switch (iter->ValuesType) {
		case VTCONSTANT:
			Stream	<< " CONSTANT";
			if (_isMonthlyClimate)				throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tCONSTANT climate value type is not valid when using the monthly climate input option.\n");
			if (_isExternFlam)					throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tCONSTANT climate value type is not valid when using the optional Climate.Flammability.File field. You can comment it out with a semi-colon at the beginning of the line.\n");
			break;
		case VTSPATIAL:
			Stream	<< " SPATIAL";
			if (_isMonthlyClimate)				throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tSPATIAL climate value type is not valid when using the monthly climate input option.\n");
			if (iter->SpatialTempFile=="")		throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing spatial temperature file name.\n");
			if (iter->SpatialPrecipFile=="")	throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing spatial precipitation file name.\n");
			if (_isExternFlam)					throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tSPATIAL climate value type is not valid when using the optional Climate.Flammability.File field. You can comment it out with a semi-colon at the beginning of the line.\n");
			break;
		case VTEXPLICIT:
			Stream	<< " EXPLICIT";
			if (iter->SpatialTempFile=="")		throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing explicit temperature file name.\n");
			if (iter->SpatialPrecipFile=="")	throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing explicit precipitation file name.\n");
			break;
		case VTRANDEXPLICIT:
			Stream	<< " RANDEXPLICIT";
			int min = iter->RandExplicitMinYear;
			int max = iter->RandExplicitMaxYear;
			if (min<0)						    throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMin Rand Year must be greater than or equal to 0.\n");
			if (max<0)						    throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMax Rand Year must be greater than or equal to 0.\n");
			if (max-min < 1)					throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMax Rand Year must be greater than Min Rand Year.\n");
			if (iter->SpatialTempFile=="")		throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing explicit temperature file name.\n");
			if (iter->SpatialPrecipFile=="")	throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing explicit precipitation file name.\n");
			break;
		}
        //show OffsetsType...
		Stream << std::setw(15);
		switch (iter->OffsetsType) {
		case OTNONE:
			Stream	<< " NONE";
			break;
		case OTCONSTANT:
			Stream	<< " CONSTANT";
			break;
		case OTFILE:
			Stream	<< " FILE";
			if (iter->OffsetsFile=="")			throw SimpleException(SimpleException::INITFAULT,"\nInvalid Climate Transition Value: \n\tMissing offsets file name.\n");
			break;
		case OTRANDOM:
			Stream	<< " RANDOM";
			break;
		}
		Stream	<< std::endl;
		if (gDetailLevel>=MAXIMUM) ShowOutput(Stream);
	}
}


void Climate::			runStart()
{
	ShowOutput(MAXIMUM, "\t\tClimate Run Init\n");
}


void Climate::			runEnd()
//Finish up this run but do not clear.
{
}


void Climate::			repStart()
/** Calculate temporal offsets. */
{
	ShowOutput(MAXIMUM, "\t\tClimate Rep setup\n");
	//Evaluate each transition period for temporal offset settings.
	std::vector<SClimateTransition>::iterator T;
	int firstYear, lastYear;
	for (T=_transitions.begin(); T!=_transitions.end(); T++) {
		firstYear = T->Year;
		lastYear = (T+1)!=_transitions.end() ? (T+1)->Year-1 : gLastYear;
		if (!T->OffsetsType==OTNONE) {
            if (T->OffsetsType==OTCONSTANT && FRESCO->isRunningFirstRep())	
				setOffsetsConstant(T->ConstantTempOffset,T->ConstantPrecipOffset,firstYear,lastYear);
			else if (T->OffsetsType==OTFILE && FRESCO->isRunningFirstRep())	
				setOffsetsFromFile(GetFullPath(gInputBasePath, T->OffsetsFile),firstYear,lastYear);
			else if (T->OffsetsType==OTRANDOM && (FRESCO->isRunningFirstRep() || !T->IsRandOffsetReplicated))	{
				setOffsetsRandom(T->RandomOffsetsTempMean,T->RandomOffsetsTempStdDev,T->RandomOffsetsPrecipMean,T->RandomOffsetsPrecipStdDev,T->IsRandOffsetReplicated,firstYear,lastYear);
				if (!FRESCO->isRunningFirstRep())	setStepsAndRampsInTimeRange(firstYear, lastYear);
			}
		}
	}

	if (_stepsAndRampsEnabled && FRESCO->isRunningFirstRep())
		setStepsAndRamps();

	showOffsetSummary();
}


void Climate::			repEnd()
{
}


void Climate::			yearStart()
/** Start of year processing. */
{
	applyTransitionIfExists(gYear);
	ShowOutput(MAXIMUM, "\t\tClimate set to "+climateValuesTypeToString(_pCurrentTransition->ValuesType)
		+ " with offset type "+climateOffsetsTypeToString(_pCurrentTransition->OffsetsType) 
		+ (_pCurrentTransition->OffsetsType==OTRANDOM ? (_pCurrentTransition->IsRandOffsetReplicated ? " (replicated)" : " (not replicated)") : "")
		+ ".\n");

	std::string filename;
	int circularIndex = (gYear - gFirstYear) % _yearsOfArchivedHistory;
	int previousCirularIndex = (gYear - gFirstYear -1) % _yearsOfArchivedHistory;
	std::list<int>::iterator month;
	switch (_pCurrentTransition->ValuesType)
	{
	case VTCONSTANT :
		//Single temp and precip value used for every cell in all years and reps.
		for (int r=0; r<gYSize; r++) {
			for (int c=0; c<gXSize; c++)	{
				_pSpatialTemp[circularIndex][0][r][c] = _pCurrentTransition->ConstantTemp;
				_pSpatialPrecip[circularIndex][0][r][c] = _pCurrentTransition->ConstantPrecip;
			}
		}
		break;
		
	case VTSPATIAL :
		//Read in spatially unique temp and precip values from a file.  Reuse each year and rep.
		if (gYear == _pCurrentTransition->Year) {
			gIO->readRasterFile(GetFullPath(gInputBasePath, _pCurrentTransition->SpatialTempFile), _pSpatialTemp[circularIndex][0], false);		
			gIO->readRasterFile(GetFullPath(gInputBasePath, _pCurrentTransition->SpatialPrecipFile), _pSpatialPrecip[circularIndex][0], false);		
		}
		else if (circularIndex != previousCirularIndex)
		{
			size_t numBytesToCopy = gXSize*sizeof(float);
			for (int r=0; r<gYSize; r++)
			{
				memcpy(_pSpatialTemp[circularIndex][0][r],    _pSpatialTemp[previousCirularIndex][0][r],   numBytesToCopy);
				memcpy(_pSpatialPrecip[circularIndex][0][r],  _pSpatialPrecip[previousCirularIndex][0][r], numBytesToCopy);
			}
		}
		break;
	
	case VTEXPLICIT :
		//Read in spatial temp and precip files each year.
		if (_isExternFlam)
		{
			// Use external calculations for annual climate flammability.
			filename = AppendYear(_spatialFlamabilityFile, gYear);

			ShowOutput(MAXIMUM, "\t\t\tReading climate flammability file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
			gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialFlammability, false);
		}
		else if (!_isExternFlam && !_isMonthlyClimate)
		{
			// Use internal calculations for annual climate flammability.
			
			//Read temp file.
			filename = AppendYear(_pCurrentTransition->SpatialTempFile, gYear);
			ShowOutput(MAXIMUM, "\t\t\tReading temp file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");

			gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialTemp[circularIndex][0], false);
			//Read precip file.
			filename = AppendYear(_pCurrentTransition->SpatialPrecipFile, gYear);
			ShowOutput(MAXIMUM, "\t\t\tReading precip file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
			gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialPrecip[circularIndex][0], false);
		}

		if (_isMonthlyClimate)
		{
			// Some monthly climate files might be loaded even if using external flammability,
			// because Decid's succession to Grassland uses a variable set of monthly climate files.
			for (month=tempMonths.begin(); month!=tempMonths.end(); month++) {
				//Read temp file.
				//filename = AppendYearMonth(_pCurrentTransition->SpatialTempFile, gYear, *month);
				filename = AppendMonthYear(_pCurrentTransition->SpatialTempFile, *month, gYear);
				ShowOutput(MAXIMUM, "\t\t\tReading temp file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
				gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialTemp[circularIndex][*month], false);
			}
			for (month=precipMonths.begin(); month!=precipMonths.end(); month++) {
				//Read precip file.
				//filename = AppendYearMonth(_pCurrentTransition->SpatialPrecipFile, gYear, *month);
				filename = AppendMonthYear(_pCurrentTransition->SpatialPrecipFile, *month, gYear);
				ShowOutput(MAXIMUM, "\t\t\tReading precip file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
				gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialPrecip[circularIndex][*month], false);
			}
		}
		break;

	case VTRANDEXPLICIT :
		//Read in temp and precip values from randomly picked spatial file.
		int year = this->getRandExplicitYear();
		if (_isExternFlam)
		{
			// Use external calculations for annual climate flammability.
			filename = AppendYear(_spatialFlamabilityFile, year);
			ShowOutput(MAXIMUM, "\t\t\tReading climate flammability file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
			gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialFlammability, false);
		}
		else if (!_isExternFlam && !_isMonthlyClimate)
		{
			// Use internal calculations for annual climate flammability.

			//Read temp file.
			filename = AppendYear(_pCurrentTransition->SpatialTempFile, year);
			ShowOutput(MAXIMUM, "\t\t\tReading temp file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
			gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialTemp[circularIndex][0], false);
			//Read precip file.
			filename = AppendYear(_pCurrentTransition->SpatialPrecipFile, year);
			ShowOutput(MAXIMUM, "\t\t\tReading precip file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
			gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialPrecip[circularIndex][0], false);
		}

		if (_isMonthlyClimate)
		{
			// Some monthly climate files might be loaded even if using external flammability,
			// because Decid's succession to Grassland uses a variable set of monthly climate files.
			for (month=tempMonths.begin(); month!=tempMonths.end(); month++) {
				//Read temp file.
				//filename = AppendYearMonth(_pCurrentTransition->SpatialTempFile, year, *month);
				filename = AppendMonthYear(_pCurrentTransition->SpatialTempFile, *month, year);
				ShowOutput(MAXIMUM, "\t\t\tReading temp file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
				gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialTemp[circularIndex][*month], false);
			}
			for (month=precipMonths.begin(); month!=precipMonths.end(); month++) {
				//Read precip file.
				//filename = AppendYearMonth(_pCurrentTransition->SpatialPrecipFile, year, *month);
				filename = AppendMonthYear(_pCurrentTransition->SpatialPrecipFile, *month, year);
				ShowOutput(MAXIMUM, "\t\t\tReading precip file: " + GetFullPath(gInputBasePath, filename)); ShowOutput(MAXIMUM, " \n");
				gIO->readRasterFile(GetFullPath(gInputBasePath, filename), _pSpatialPrecip[circularIndex][*month], false);
			}
		}
		break;
	}
}


void Climate::			yearEnd()
{
}


//Transitions
void Climate::          setupTransitions()
{
    int           count = 0;
    int     *pnYears;
    std::string   *psValTypes;
    double  *pdValTempConstants, *pdValPrecipConstants;
    std::string   *psValTempFiles;
    std::string   *psValPrecipFiles;
    int     *pnValRandExplicitMinYears, *pnValRandExplicitMaxYears;
    bool    *pbValRandExplicitReplicate;
    
    std::string   *psOffTypes;
    std::string   *psOffFiles;
    double  *pdOffConstantTemps, *pdOffConstantPrecips;
    double  *pdOffRandomTempMeans, *pdOffRandomPrecipMeans;
    double  *pdOffRandomTempStdDevs, *pdOffRandomPrecipStdDevs;
    bool    *pbOffRandomReplicate;
    
    //Get arrays of values and make sure all arrays have the same count.
    count = FRESCO->fif().pnGet(FRESCO->fif().root["Climate"]["TransitionYears"], pnYears);
    if (FRESCO->fif().psGet(FRESCO->fif().root["Climate"]["Values"]["Type"], psValTypes) != count)                                      throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.Type");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Values"]["Temp.Constant"], pdValTempConstants) != count)                     throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.Temp.Constant");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Values"]["Precip.Constant"], pdValPrecipConstants) != count)                 throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.Precip.Constant");
    if (FRESCO->fif().psGet(FRESCO->fif().root["Climate"]["Values"]["Temp.File"], psValTempFiles) != count)                             throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.Temp.File");
    if (FRESCO->fif().psGet(FRESCO->fif().root["Climate"]["Values"]["Precip.File"], psValPrecipFiles) != count)                         throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.Precip.File");
    if (FRESCO->fif().pnGet(FRESCO->fif().root["Climate"]["Values"]["RandExplicit.MinYear"], pnValRandExplicitMinYears) != count)       throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.RandExplicit.MinYear");
    if (FRESCO->fif().pnGet(FRESCO->fif().root["Climate"]["Values"]["RandExplicit.MaxYear"], pnValRandExplicitMaxYears) != count)       throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.RandExplicit.MaxYear");
    if (FRESCO->fif().pbGet(FRESCO->fif().root["Climate"]["Values"]["RandExplicit.Replicate"], pbValRandExplicitReplicate) != count)    throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Values.RandExplicit.Replicate");

    if (FRESCO->fif().psGet(FRESCO->fif().root["Climate"]["Offsets"]["Type"], psOffTypes) != count)                                     throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Type");
    if (FRESCO->fif().psGet(FRESCO->fif().root["Climate"]["Offsets"]["File"], psOffFiles) != count)                                     throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.File");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Offsets"]["Constant.Temp"], pdOffConstantTemps) != count)                    throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Constant.Temp");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Offsets"]["Constant.Precip"], pdOffConstantPrecips) != count)                throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Constant.Precip");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Offsets"]["Random.Temp.Mean"], pdOffRandomTempMeans) != count)               throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Random.Temp.Mean");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Offsets"]["Random.Temp.StdDev"], pdOffRandomPrecipMeans) != count)           throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Random.Temp.StdDev");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Offsets"]["Random.Precip.Mean"], pdOffRandomTempStdDevs) != count)           throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Random.Precip.Mean");
    if (FRESCO->fif().pdGet(FRESCO->fif().root["Climate"]["Offsets"]["Random.Precip.StdDev"], pdOffRandomPrecipStdDevs) != count)       throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Random.Precip.StdDev");
    if (FRESCO->fif().pbGet(FRESCO->fif().root["Climate"]["Offsets"]["Random.Replicate"], pbOffRandomReplicate) != count)               throw SimpleException(SimpleException::BADARRAYSIZE,"Unexpected array size returned for Key: Climate.Offsets.Random.Replicate");

    std::string temp = "";
    for (int i=0; i<count; i++) {
	    SClimateTransition Transition;
	    std::vector<SClimateTransition>::iterator Iter;
	    //Create the climate transition.
	    Transition.Year						= pnYears[i];
        Transition.ValuesType				= ((temp=psValTypes[i]) == "CONSTANT" ? VTCONSTANT : (temp == "SPATIAL" ? VTSPATIAL : (temp == "EXPLICIT" ? VTEXPLICIT : VTRANDEXPLICIT)));
	    Transition.ConstantTemp				= pdValTempConstants[i];
	    Transition.ConstantPrecip			= pdValPrecipConstants[i];
        Transition.SpatialTempFile			= FormatDirectory(std::string(psValTempFiles[i]));
	    Transition.SpatialPrecipFile		= FormatDirectory(std::string(psValPrecipFiles[i]));
	    Transition.RandExplicitMinYear		= pnValRandExplicitMinYears[i];
	    Transition.RandExplicitMaxYear		= pnValRandExplicitMaxYears[i];
	    Transition.IsRandExplicitReplicated = pbValRandExplicitReplicate[i];
        Transition.OffsetsType				= ((temp=psOffTypes[i]) == "NONE" ? OTNONE : (temp == "CONSTANT" ? OTCONSTANT : (temp == "FILE" ? OTFILE : OTRANDOM)));
        Transition.OffsetsFile				= FormatDirectory(std::string(psOffFiles[i]));
	    Transition.ConstantTempOffset		= pdOffConstantTemps[i];
	    Transition.ConstantPrecipOffset		= pdOffConstantPrecips[i];
	    Transition.RandomOffsetsTempMean	= pdOffRandomTempMeans[i];
	    Transition.RandomOffsetsTempStdDev	= pdOffRandomTempStdDevs[i];
	    Transition.RandomOffsetsPrecipMean	= pdOffRandomPrecipMeans[i];
	    Transition.RandomOffsetsPrecipStdDev= pdOffRandomPrecipStdDevs[i];
	    Transition.IsRandOffsetReplicated	= pbOffRandomReplicate[i];
	    //Insert it before the first later year.
	    for (Iter=Climate::_transitions.begin(); Iter!=Climate::_transitions.end(); Iter++)
		    if (Transition.Year<Iter->Year) break;
	    Climate::_transitions.insert(Iter,Transition);
    }
}


void Climate::			clearTransition(SClimateTransition* pTransition)
{
	pTransition->Year = 0;
	pTransition->ValuesType = VTCONSTANT;
	pTransition->ConstantTemp = 0;
	pTransition->ConstantPrecip = 0;
	pTransition->SpatialTempFile = "";
	pTransition->SpatialPrecipFile = "";
	pTransition->RandExplicitMinYear	= 0;
	pTransition->RandExplicitMaxYear	= 0;
	pTransition->IsRandExplicitReplicated = false;
	
	pTransition->OffsetsType	= OTNONE;
	pTransition->ConstantTempOffset = 0;
	pTransition->ConstantPrecipOffset = 0;
	pTransition->OffsetsFile = "";
	pTransition->RandomOffsetsTempMean = 0;
	pTransition->RandomOffsetsTempStdDev	= 0;
	pTransition->RandomOffsetsPrecipMean	= 0;
	pTransition->RandomOffsetsPrecipStdDev = 0;
	pTransition->IsRandOffsetReplicated = false;
}


void Climate::			setCurrentTransition(std::vector<SClimateTransition>::iterator transition)
{
	_pCurrentTransition->Year = transition->Year;
	//Values
	_pCurrentTransition->ValuesType                 = transition->ValuesType;
	_pCurrentTransition->ConstantTemp               = transition->ConstantTemp;
	_pCurrentTransition->ConstantPrecip             = transition->ConstantPrecip;
	_pCurrentTransition->SpatialTempFile            = transition->SpatialTempFile;
	_pCurrentTransition->SpatialPrecipFile          = transition->SpatialPrecipFile;
	_pCurrentTransition->RandExplicitMinYear	    = transition->RandExplicitMinYear;
	_pCurrentTransition->RandExplicitMaxYear	    = transition->RandExplicitMaxYear;
	_pCurrentTransition->IsRandExplicitReplicated   = transition->IsRandExplicitReplicated;
	//Offsets
	_pCurrentTransition->OffsetsType	            = transition->OffsetsType;
	_pCurrentTransition->ConstantTempOffset         = transition->ConstantTempOffset;
	_pCurrentTransition->ConstantPrecipOffset       = transition->ConstantPrecipOffset;
	_pCurrentTransition->OffsetsFile                = transition->OffsetsFile;
	_pCurrentTransition->RandomOffsetsTempMean      = transition->RandomOffsetsTempMean;
	_pCurrentTransition->RandomOffsetsTempStdDev	= transition->RandomOffsetsTempStdDev;
	_pCurrentTransition->RandomOffsetsPrecipMean	= transition->RandomOffsetsPrecipMean;
	_pCurrentTransition->RandomOffsetsPrecipStdDev  = transition->RandomOffsetsPrecipStdDev;
	_pCurrentTransition->IsRandOffsetReplicated     = transition->IsRandOffsetReplicated;
	
	ShowOutput(MODERATE, "\t\tClimate transition changed to "+climateValuesTypeToString(_pCurrentTransition->ValuesType)
		+ " with offset type "+climateOffsetsTypeToString(_pCurrentTransition->OffsetsType) 
		+ (_pCurrentTransition->OffsetsType==OTRANDOM ? (_pCurrentTransition->IsRandOffsetReplicated ? " (replicated)" : " (not replicated)") : "")
		+ ".\n");
}


void Climate::			applyTransitionIfExists(int year)
{
	std::vector<SClimateTransition>::iterator transition;
	for (transition=Climate::_transitions.begin(); transition!=Climate::_transitions.end(); transition++) {
		if (transition->Year == year) {
			setCurrentTransition(transition);
			break;
		}
	}
}


//Values
int Climate::			getRandExplicitYear()
{
	//OLD TODO: Seperate filling the list, from selecting getting a specific year
	int randomYear;
	if (!FRESCO->isRunningFirstRep() && _pCurrentTransition->IsRandExplicitReplicated) {
		randomYear = this->_pRandomYears[gYear];
	}
	else {
		int min = _pCurrentTransition->RandExplicitMinYear; 
		int max = _pCurrentTransition->RandExplicitMaxYear;
		int canidateCount = max-min+1;
		if (canidateCount < 1)	
			throw SimpleException(SimpleException::UNKNOWN,"\tFailed loading climate: Min Year must be less than Max Year when using RANDEXPLICIT climate type.\n"); 

        randomYear = (int)floor(GetNextRandom()*canidateCount+min);

		//Store selected year for next Rep cycle if replicating.
		if (FRESCO->isRunningFirstRep() && _pCurrentTransition->IsRandExplicitReplicated) 
			this->_pRandomYears[gYear] = randomYear; 		
	}
	return randomYear;
}


std::string Climate::	climateValuesTypeToString(EValuesType type)
{
    std::string result= "INVALID";
	if		(type == VTCONSTANT)	    result="CONSTANT";
	else if (type == VTSPATIAL)		    result="SPATIAL";
    else if (type == VTEXPLICIT)		result="EXPLICIT";
    else if (type == VTRANDEXPLICIT)	result="RANDEXPLICIT";
    return result;
}


float Climate::			getClimateFlammability(int row, int col)
{
	if (_pSpatialFlammability)
	{
		//if (IsNodata(_pSpatialFlammability[row][col]))
		//	throw Poco::Exception("invalid use of nodata value ("+ToS(_pSpatialFlammability[row][col])+") in external climate flammability map. Nodata values should not exist except where a cell's vegetation type is NoVeg.");
		return _pSpatialFlammability[row][col];
	}
	else
		throw Poco::Exception("Climate Flammabiltiy file did not load correctly.");
}

//Offsets
void Climate::          setupStepsAndRamps() //(EClimateType ClimateType, EOffsetType OffsetType, int Year, float Amount)
{
    _stepsAndRampsEnabled = FRESCO->fif().root["Climate"]["StepsAndRampsEnabled"].asBool();
    if (_stepsAndRampsEnabled)
    {
        setupStepOrRamp(TEMP,     STEP,  FRESCO->fif().root["Climate"]["Offsets"]["TempStepYear"],     FRESCO->fif().root["Climate"]["Offsets"]["TempStep"]);
        setupStepOrRamp(TEMP,     RAMP,  FRESCO->fif().root["Climate"]["Offsets"]["TempRampYear"],     FRESCO->fif().root["Climate"]["Offsets"]["TempRamp"]);
        setupStepOrRamp(PRECIP,   STEP,  FRESCO->fif().root["Climate"]["Offsets"]["PrecipStepYear"],   FRESCO->fif().root["Climate"]["Offsets"]["PrecipStep"]);
        setupStepOrRamp(PRECIP,   RAMP,  FRESCO->fif().root["Climate"]["Offsets"]["PrecipRampYear"],   FRESCO->fif().root["Climate"]["Offsets"]["PrecipRamp"]);
    }
}

void Climate::          setupStepOrRamp(const EClimateType climateType, const EOffsetType offsetType, Json::Value& yearsKey, Json::Value& offsetsKey)
{        
    //Get values from FIF.
    int*      pYears;
    double*   pAmounts;
    int count = FRESCO->fif().pnGet(yearsKey, pYears);
    if (FRESCO->fif().pdGet(offsetsKey, pAmounts) != count)     
        throw SimpleException(SimpleException::BADARRAYSIZE, std::string("Unexpected array size returned for Key: ") + offsetsKey.asCString());
    //Place values into appropriate list for later use.
    for (int i=0; i<count; i++) {
        //Create the offset with values from FIF.
        SOffset offset;
        offset.Year		= pYears[i];
        offset.Amount	= pAmounts[i];
        //Get the appropriate list for the new offset.
        std::vector<SOffset>* offsets;
        if (offsetType==STEP)
            offsets = &(climateType==TEMP ? _tempStep : _precipStep);
        else
            offsets = &(climateType==TEMP ? _tempRamp : _precipRamp);
        //Insert the offset in order, before the first later year.
        std::vector<SOffset>::iterator iter;
        for (iter=offsets->begin(); iter<offsets->end(); iter++)
            if (pYears[i]<(*iter).Year) break;
        offsets->insert(iter,offset);
    }
}


void Climate::			setOffsetsConstant(float temp, float precip, int firstYear, int lastYear)
/** Sets temporal offsets to constant temp and precip for firstYear through lastYear. */
{
	for (int y=firstYear; y<=lastYear && y<=gLastYear; y++) {
        _pOffsets[y - gFirstYear].Temp	= temp;
		_pOffsets[y - gFirstYear].Precip	= precip;
	}
}


void Climate::			setOffsetsFromFile(std::string filePath, int firstYear, int lastYear)
/** Sets temporal offsets, for years from firstYear through lastYear, to values from a file. */
{
	std::fstream	fp;
	float temp, precip;

	fp.open(filePath.c_str(),std::ios::in);
	if (!fp.is_open()) throw SimpleException(SimpleException::FILEBAD,"Open file failed: " + filePath + "\n");
	for (int y=gFirstYear; y<=lastYear && y<=gLastYear; y++) {
		//Read a year in from Climate.File.
		fp >> temp >> precip;
		if (firstYear<=y && y<=lastYear) {
			_pOffsets[y - gFirstYear].Temp = temp; 
			_pOffsets[y - gFirstYear].Precip = precip;
		}
	}
}


void Climate::			setOffsetsRandom(float tempMean, float tempStdDev, float precipMean, float precipStdDev, bool isReplicated, int firstYear, int lastYear)
/**
 * Sets temporal offsets to random temp and precip for firstYear through lastYear.
 * If isReplicated, this function will only be called for the first rep.
 */
{
	for (int y=firstYear; y<=lastYear && y<=gLastYear; y++) {
		_pOffsets[y - gFirstYear].Temp	    = GetNextRandomNorm(tempMean, tempStdDev);
		_pOffsets[y - gFirstYear].Precip	= GetNextRandomNorm(precipMean, precipStdDev);
	}
}


void Climate::			showOffsetSummary() 
/** Output offsets to console. */
{
	if (gDetailLevel>=MAXIMUM) {
		ShowOutput("\t\t\tClimate Offsets (including steps and ramps):\n");
		ShowOutput("\t\t\tYear     Temp   Precip\n");
	    std::ostringstream	stream;
		for (int y=gFirstYear; y<=gLastYear; y++) {
			stream	<< "\t\t\t" << setiosflags( std::ios::left ) << std::setiosflags( std::ios::fixed ) << std::setprecision(3)
						<< std::setw(4) << y 
						<< std::resetiosflags( std::ios::left )  //Align right.
						<< std::setw(9) << _pOffsets[y - gFirstYear].Temp  
						<< std::setw(9) << _pOffsets[y - gFirstYear].Precip 
						<< std::endl;	
			ShowOutput(stream);
		}
	}
}


std::string Climate::	climateOffsetsTypeToString(EOffsetsType type)
{
    std::string result= "INVALID";
    if      (type == OTNONE)         result = "NONE";
    else if (type == OTCONSTANT)     result = "CONSTANT";
    else if (type == OTFILE)         result = "FILE";
    else if (type == OTRANDOM)       result = "RANDOM";
    return result;
}


//Steps and Ramps
void Climate::			setStepsAndRamps()
/** Calculate temporal steps and ramps for all years and apply them to temporal offsets. */
{
	setStepsAndRampsInTimeRange(gFirstYear, gLastYear);
}


void Climate::          setStepsAndRampsInTimeRange(int firstYear, int lastYear)
/** Calculate temporal steps and ramps for the given time range and apply them to temporal offsets. */
{
	SClimate		tempOffset;			//Climate offsets (temp and precip) due to ramping or stepwise changes.
	int				offsetYears;
	std::vector<SOffset>::iterator offset;

	ShowOutput(MAXIMUM, "\t\t\tUsing ramped offsets.\n");

	//Must calculate from gFirstYear even if firstYear is greater, because the 
	//steps and ramps need to be built up appropriately.
	for (int y=gFirstYear; y<=lastYear && y<=gLastYear; y++) {
		try {
			//Calculate temperature steps in year y.
			for (offset=_tempStep.begin(); offset<_tempStep.end(); offset++) {
				if (y==(*offset).Year) {tempOffset.Temp += (*offset).Amount; break;}
				if (y<(*offset).Year) break;	//Break if before date of offset.
			}
			//Calculate precip steps in year y.
			for (offset=_precipStep.begin(); offset<_precipStep.end(); offset++) {
				if (y==(*offset).Year) {tempOffset.Precip += (*offset).Amount; break;}
				if (y<(*offset).Year) break;	//Break if before date of offset.
			}
			//Calculate temperature ramps in year y (loop runs in reverse order).
			if (!_tempRamp.empty()) {
				for (offset=--_tempRamp.end(); offset>=_tempRamp.begin(); offset--) {
					offsetYears = y - offset->Year;	//Calc time since the last ramp.
					if (offsetYears>0) {								
						tempOffset.Temp += offset->Amount * offsetYears;						//Add the ramp offset to any step offset applied above.
						//If not the first ramp then apply the partial ramp.	
						if (offset>_tempRamp.begin())	
							tempOffset.Temp += (offset-1)->Amount * (1 - offsetYears);	
						break;
					}
					if (offset==_tempRamp.begin()) break;
				}
			}
			//Calculate precip ramps in year y  (loop runs in reverse order).
			if (!_precipRamp.empty()) {
				for (offset=--_precipRamp.end(); offset>=_precipRamp.begin(); offset--) {
					offsetYears = y - offset->Year;	//Calc time since the last ramp.
					if (offsetYears>0) {								
						tempOffset.Precip += offset->Amount * offsetYears;						//Add the ramp offset to any step offset applied above.
						//If not the first ramp then apply the partial ramp.
						if (offset>_precipRamp.begin())				
							tempOffset.Precip += (offset-1)->Amount * (1 - offsetYears);	
						break;
					}
					if (offset==_precipRamp.begin()) break;
				}
			}
		}
		catch (...) {						
            throw SimpleException(SimpleException::INITFAULT, "Failed to calculate climate offset ramps and steps.\n");
		}
		if (firstYear <= y && y <= lastYear) {
			//Apply steps and ramps to temporal climate array.
			_pOffsets[y - gFirstYear].Temp	+= tempOffset.Temp;
			_pOffsets[y - gFirstYear].Precip	+= tempOffset.Precip;
		}
	}
}
