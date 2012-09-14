//
// RasterIO.cpp
//

#include "PreCompiled.h"
#include "RasterIO.h"
#include "ReadRasterException.h"
#include "WriteRasterException.h"
#include "Frame.h"
#include "Poco/Format.h"
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <ogr_spatialref.h> // for OGRSpatialReference
#include <iostream>
#include <string>
#include <queue>
#include <cstdlib>
#include <math.h>
#include <limits>
#include <sstream>
#include <cctype>

using std::cout;
using std::endl;
using std::string;
using std::queue;


const byte  RasterIO::NODATA_BYTE      = 255;
const int   RasterIO::NODATA_INT     = -2147483647;
const float RasterIO::NODATA_FLOAT   = -3.4e38f;
const float RasterIO::NODATA_FLOAT_ALTERNATE = -3.40282e+38f;


RasterIO::RasterIO(double xOrigin, double yOrigin, int xOffset, int yOffset, int xSize, int ySize, 
				double xPixelSize, double yPixelSize, double xRotation, double yRotation, 
				const std::string &softwareDescription,
				bool requireAaeacForInput, bool applyAaeacToOutput)
	: _xOffset(xOffset),
	_yOffset(yOffset),
	_xSize(xSize),
	_ySize(ySize),
	_xInputOrigin(xOrigin),
	_yInputOrigin(yOrigin),
	_xPixelSize(xPixelSize),
	_yPixelSize(yPixelSize),
	_xRotation(xRotation),
	_yRotation(yRotation),
	_softwareDescription(softwareDescription),
	_requireAaeacForInput(requireAaeacForInput), 
	_applyAaeacToOutput(applyAaeacToOutput),
	_mapDescriptions(10, "")
{
	//GDALAllRegister();
	GDALRegister_GTiff();
	//GDALRegister_AAIGrid();

	//
	// Determine origin for output rasters.
	//
	_xOutputOrigin = _xInputOrigin + (_xOffset * gCellSize);
	_yOutputOrigin = _yInputOrigin - (_yOffset * gCellSize);

	//
	// Create metadata items used for output.
	//
	_geoTransform[0] = _xOutputOrigin;
	_geoTransform[3] = _yOutputOrigin;
	_geoTransform[1] = _xPixelSize;
	_geoTransform[5] = _yPixelSize;
	_geoTransform[2] = _xRotation;
	_geoTransform[4] = _yRotation;

	//
	// Descriptions for output metadata.
	//
	_mapDescriptions[AGE] = "Stand Age for year %d of rep %d.";
	_mapDescriptions[SITE_VARIABLE] = "Site Variable for year %d of rep %d.";
	_mapDescriptions[FIRE_AGE] = "Fire Age for year %d of rep %d.";
	_mapDescriptions[FIRE_SCAR] = "Fire Scar for year %d of rep %d.  Value Key: [if ignition cell use -, otherwise +][YearOfLastBurn].[FireID]";
	_mapDescriptions[TUNDRA_BASAL_AREA] = "Tundra Basal Area for year %d of rep %d.";
	_mapDescriptions[VEGEGATION] = "Vegetation Type for year %d of rep %d.  Value Index: "+ToS((int)gNoVegID)+"=NoVeg, "+ToS((int)gTundraID)+"=Tundra, "+ToS((int)gBSpruceID)+"=Black Spruce, "+ToS((int)gWSpruceID)+"=White Spruce, "+ToS((int)gDecidID)+"=Deciduous"+ (gGrasslandID == NODATA_BYTE ? "" : ", "+ToS((int)gGrasslandID)+"=Grassland (grassland is in alpha)");
	_mapDescriptions[SUBCANOPY] = "Subcanopy Type for year %d of rep %d.  Value Index: "+ToS((int)NODATA_BYTE)+"=NoData, "+ToS((int)gTundraID)+"=Tundra, "+ToS((int)gBSpruceID)+"=Black Spruce, "+ToS((int)gWSpruceID)+"=White Spruce, "+ToS((int)gDecidID)+"=Deciduous"+ (gGrasslandID == NODATA_BYTE ? "" : ", "+ToS((int)gGrasslandID)+"=Grassland (grassland is in alpha)");
	_mapDescriptions[BURN_SEVERITY] = "Burn Severity for year %d of rep %d.  Value Index: "+ToS((int)NODATA_BYTE)+"=NoData (no burn for the given year), 0=No Burn, 1=Low, 2=Moderate, 3=High w/ Low Surface Severity, 4=High w/ High Surface Severity";
	_mapDescriptions[BURN_SEVERITY_HISTORY] = "Burn Severity History for year %d of rep %d.  Value Index: 0=No Burn, 1=Low, 2=Moderate, 3=High w/ Low Surface Severity, 4=High w/ High Surface Severity";
	_mapDescriptions[DECID_SPECIES_TRAJECTORY] = "Deciduous Species Trajectory for year %d of rep %d.  Value Index: "+ToS((int)NODATA_BYTE)+"=NoData, "+ToS((int)gBSpruceID)+"=Black Spruce, "+ToS((int)gWSpruceID)+"=White Spruce";


	//
	// Create default Alaska Equal Area Conic projection.
	//
	_pSpatialReference = NULL;
	if (_applyAaeacToOutput)
	{
		OGRSpatialReference sr;
		std::string msg = "unable to set Alaska Albers Equal Area Conic projection at ";

		if (OGRERR_NONE != sr.SetProjCS("Alaska_Albers_Equal_Area_Conic"))
			throw WriteRasterException(msg+"SetProjCS()");

		if (OGRERR_NONE != sr.SetGeogCS("GCS_North_American_1983", "NAD83", // "D_North_American_1983" doesn't register for some software (e.g. Quantum GIS)
			"GRS_1980", 6378137, 298.257222101, "Greenwich", 0.0, "Degree", 0.0174532925199432955))
			throw WriteRasterException(msg+"SetGeogCS()");

		if (OGRERR_NONE != sr.SetProjection("Albers_Conic_Equal_Area"))
			throw WriteRasterException(msg+"SetProjection()");

		if (OGRERR_NONE != sr.SetACEA(55,65, 50,-154, 0,0))
			throw WriteRasterException(msg+"SetACEA()");

		if (OGRERR_NONE != sr.SetLinearUnits("Meter", 1))
			throw WriteRasterException(msg+"SetLinearUnits()");

		if (OGRERR_NONE != sr.exportToWkt( &_pSpatialReference ))
			throw WriteRasterException(msg+"exportToWkt()");
	}

	//
	// Create color table for any veg type output map.
	//
	//  error = purple
	//  nodata = black
	//
	GDALColorEntry c;
	_pVegColorTable = new GDALColorTable(GPI_RGB);
	// first, fill all with error color -- purple
	c.c1=148; c.c2=0; c.c3=211; c.c4=255;
	for(int i=0; i<=255; i++)
		_pVegColorTable->SetColorEntry(i, &c);
	// nodata -- black
	c.c1=0; c.c2=0; c.c3=0; c.c4=255;
	_pVegColorTable->SetColorEntry(NODATA_BYTE, &c);
	// noveg -- gray
	c.c1=104; c.c2=104; c.c3=104; c.c4=255;
	_pVegColorTable->SetColorEntry(gNoVegID, &c);
	// tundra -- yellow
	c.c1=255; c.c2=190; c.c3=41; c.c4=255; 
	_pVegColorTable->SetColorEntry(gTundraID, &c);
	// shrub tundra -- brown
	c.c1=129; c.c2=105; c.c3=73; c.c4=255;
	_pVegColorTable->SetColorEntry(gShrubTundraID, &c);
	// graminoid tundra -- tan
	c.c1=217; c.c2=190; c.c3=150; c.c4=255;
	_pVegColorTable->SetColorEntry(gGraminoidTundraID, &c);
	// wetland tundra -- blue
	c.c1=2; c.c2=56; c.c3=188; c.c4=255;
	_pVegColorTable->SetColorEntry(gWetlandTundraID, &c);
	// decid -- light yellow
	c.c1=247; c.c2=252; c.c3=185; c.c4=255;
	_pVegColorTable->SetColorEntry(gDecidID, &c);
	// white spruce -- faded green
	c.c1=65; c.c2=175; c.c3=93; c.c4=255;
	_pVegColorTable->SetColorEntry(gWSpruceID, &c);
	// black spruce -- dark green
	c.c1=0; c.c2=69; c.c3=41; c.c4=255;
	_pVegColorTable->SetColorEntry(gBSpruceID, &c);
	// grassland spruce -- muddy yellow
	if (gGrasslandID != RasterIO::NODATA_BYTE)
	{
		c.c1=255; c.c2=211; c.c3=127; c.c4=255;
		_pVegColorTable->SetColorEntry(gGrasslandID, &c);
	}
	//
	// Create color table for BurnSeverity and BurnSeverityHistory.
	//
	//  error = purple
	//  nodata = black
	//	0 = white
	//	1 = yellow
	//	2 = orange
	//	3 = light red
	//	4 = dark red
	//
	_pBurnSevColorTable = new GDALColorTable(GPI_RGB);
	// first, fill all with error color -- purple
	c.c1=148; c.c2=0; c.c3=211; c.c4=255;
	for(int i=0; i<=255; i++)
		_pBurnSevColorTable->SetColorEntry(i, &c);
	// nodata -- black
	c.c1=0; c.c2=0; c.c3=0; c.c4=255;
	_pBurnSevColorTable->SetColorEntry(NODATA_BYTE, &c);
	// no burn -- white
	c.c1=255; c.c2=255; c.c3=255; c.c4=255;
	_pBurnSevColorTable->SetColorEntry(0, &c);
	// low -- yellow
	c.c1=252; c.c2=252; c.c3=101; c.c4=255;
	_pBurnSevColorTable->SetColorEntry(1, &c);
	// moderate -- orange
	c.c1=255; c.c2=165; c.c3=0; c.c4=255;
	_pBurnSevColorTable->SetColorEntry(2, &c);
	// high w/ low surface severity -- light red
	c.c1=255; c.c2=66; c.c3=11; c.c4=255;
	_pBurnSevColorTable->SetColorEntry(3, &c);
	// high w/ high surface severity -- dark red
	c.c1=210; c.c2=0; c.c3=0; c.c4=255;
	_pBurnSevColorTable->SetColorEntry(4, &c);
}


RasterIO::~RasterIO()
{
	if (_pSpatialReference != NULL)
		CPLFree(_pSpatialReference);
	if (_pVegColorTable != NULL)
		delete _pVegColorTable;
	if (_pBurnSevColorTable != NULL)
		delete _pBurnSevColorTable;
}


void RasterIO::getNodata(byte &result)
{
	result = RasterIO::NODATA_BYTE;
}
void RasterIO::getNodata(int &result)
{
	result = RasterIO::NODATA_INT;
}
void RasterIO::getNodata(float &result)
{
	result = RasterIO::NODATA_FLOAT;
}


void RasterIO::getAlternateNodata(byte &result)
{
	result = 0; // zero used to say n/a
}
void RasterIO::getAlternateNodata(int &result)
{
	result = 0; // zero used to say n/a
}
void RasterIO::getAlternateNodata(float &result)
{
	result = RasterIO::NODATA_FLOAT_ALTERNATE;
}


RasterIO::ALFMapType RasterIO::getMapType(int f)
{
	if (f & outVeg) return VEGEGATION;
	else if (f & outAge) return AGE;
	else if (f & outSite) return SITE_VARIABLE;
	else if (f & outSub) return SUBCANOPY;
	else if (f & outFireAge) return FIRE_AGE;
	else if (f & outFireScar) return FIRE_SCAR;
	else if (f & outfireSeverity) return BURN_SEVERITY;
	else if (f & outfireSeverityHist) return BURN_SEVERITY_HISTORY;
	else if (f & outfireSeverityHist) return BURN_SEVERITY_HISTORY;
	else if (f & out1) return DECID_SPECIES_TRAJECTORY;
	else if (f & out2) return TUNDRA_BASAL_AREA;

	std::ostringstream s;
	s << std::hex << "invalid map output flags -- " << f << " -- no data type was specified";
	throw WriteRasterException(s.str());
}


const std::string RasterIO::getMapTypeAsString(ALFMapType type)
{
	switch(type)
	{
	case VEGEGATION: return "VEGETATION"; break;
	case AGE: return "AGE"; break;
	case SUBCANOPY: return "SUBCANOPY"; break;
	case SITE_VARIABLE: return "SITE_VARIABLE"; break;
	case FIRE_AGE: return "FIRE_AGE"; break;
	case FIRE_SCAR: return "FIRE_SCAR"; break;
	case BURN_SEVERITY: return "BURN_SEVERITY"; break;
	case BURN_SEVERITY_HISTORY: return "BURN_SEVERITY_HISTORY"; break;
	case DECID_SPECIES_TRAJECTORY: return "DECID_SPECIES_TRAJECTORY"; break;
	case TUNDRA_BASAL_AREA: return "TUNDRA_BASAL_AREA"; break;
	default:
		throw WriteRasterException("undefined map type");
	}
	//return "";
}



// NOTE: Could turn these zap functions into a template,
// but I think there is a bit of a performance hit taken
// in templates so avoid it when not necessary.
void RasterIO::zapRasterMemory(byte** &pMatrix)
{
	if (pMatrix != NULL) 
	{
		for (int r=0; r<_ySize; r++) 
			free(pMatrix[r]);
		free(pMatrix);
		pMatrix = NULL;
	}
}
void RasterIO::zapRasterMemory(int** &pMatrix)
{
	if (pMatrix != NULL) 
	{
		for (int r=0; r<_ySize; r++) 
			free(pMatrix[r]);
		free(pMatrix);
		pMatrix = NULL;
	}
}
void RasterIO::zapRasterMemory(float** &pMatrix)
{
	if (pMatrix != NULL) 
	{
		for (int r=0; r<_ySize; r++) 
			free(pMatrix[r]);
		free(pMatrix);
		pMatrix = NULL;
	}
}


void RasterIO::readRasterFile(const string filepath, byte**  &pMatrix, const bool isMalloc) { _readRasterFile<byte>( filepath, pMatrix, isMalloc, GDT_Byte); }
void RasterIO::readRasterFile(const string filepath, int**   &pMatrix, const bool isMalloc) { _readRasterFile<int>(  filepath, pMatrix, isMalloc, GDT_Int32); }
void RasterIO::readRasterFile(const string filepath, float** &pMatrix, const bool isMalloc) { _readRasterFile<float>(filepath, pMatrix, isMalloc, GDT_Float32); }
void RasterIO::writeRasterFile(const string filepath, Frame*** pFrames, ALFMapType mapType, const int year, const int rep) 
{ 
	switch(mapType)
	{
	case VEGEGATION:
		_writeRasterFile<byte>(filepath, pFrames, mapType, GDT_Byte, 
			Poco::format(_mapDescriptions[VEGEGATION], year, rep), _pVegColorTable);
		break;
	case AGE:
		_writeRasterFile<int>(filepath, pFrames, mapType, GDT_Int32, 
			Poco::format(_mapDescriptions[AGE], year, rep));
		break;
	case SUBCANOPY:
		_writeRasterFile<byte>(filepath, pFrames, mapType, GDT_Byte, 
			Poco::format(_mapDescriptions[SUBCANOPY], year, rep), _pVegColorTable);
		break;
	case SITE_VARIABLE:
		_writeRasterFile<float>(filepath, pFrames, mapType, GDT_Float32, 
			Poco::format(_mapDescriptions[SITE_VARIABLE], year, rep));
		break;
	case FIRE_AGE:
		_writeRasterFile<int>(filepath, pFrames, mapType, GDT_Int32, 
			Poco::format(_mapDescriptions[FIRE_AGE], year, rep));
		break;
	case FIRE_SCAR:
		_writeRasterFile<float>(filepath, pFrames, mapType, GDT_Float32, 
			Poco::format(_mapDescriptions[FIRE_SCAR], year, rep));
		break;
	case BURN_SEVERITY:
		_writeRasterFile<byte>(filepath, pFrames, mapType, GDT_Byte, 
			Poco::format(_mapDescriptions[BURN_SEVERITY], year, rep),_pBurnSevColorTable);
		break;
	case BURN_SEVERITY_HISTORY:
		_writeRasterFile<byte>(filepath, pFrames, mapType, GDT_Byte, 
			Poco::format(_mapDescriptions[BURN_SEVERITY_HISTORY], year, rep),_pBurnSevColorTable);
		break;
	case DECID_SPECIES_TRAJECTORY:
		_writeRasterFile<byte>(filepath, pFrames, mapType, GDT_Byte, 
			Poco::format(_mapDescriptions[DECID_SPECIES_TRAJECTORY], year, rep), _pVegColorTable);
		break;
	case TUNDRA_BASAL_AREA:
		_writeRasterFile<float>(filepath, pFrames, mapType, GDT_Float32, 
			Poco::format(_mapDescriptions[TUNDRA_BASAL_AREA], year, rep));
		break;
	default:
		throw WriteRasterException("undefined map type");
	}
}



//
//
//	Template for _readRasterFile<type>
//
//
template void RasterIO::_readRasterFile<byte>   (const string filepath, byte**  &pMatrix, const bool isMalloc, const GDALDataType expectedType);
template void RasterIO::_readRasterFile<int>    (const string filepath, int**   &pMatrix, const bool isMalloc, const GDALDataType expectedType);
template void RasterIO::_readRasterFile<float>  (const string filepath, float** &pMatrix, const bool isMalloc, const GDALDataType expectedType);
template<class T> void RasterIO::_readRasterFile(const string filepath, T**     &pMatrix, const bool isMalloc, const GDALDataType expectedType)
{
	T d, a; 
	getNodata(d);
	getAlternateNodata(a);
	const T defaultNodata = d;
	const T alternateNodata = a;
	
	GDALDataset* pDataset = NULL;
	try
	{
		// Get dataset	
		pDataset = (GDALDataset*) GDALOpen(filepath.c_str(), GA_ReadOnly);
		if(pDataset == NULL) 
			throw ReadRasterException("unable to open raster file at " + filepath);
		
		// Get raster band (only 1 should exist)
		int rasterCount = pDataset->GetRasterCount();
		if (rasterCount < 1 || rasterCount > 1)
			throw ReadRasterException("a total of one raster band should be available, but " + ToS(rasterCount) + " were found at " + filepath);
		GDALRasterBand* pBand = pDataset->GetRasterBand(1);
		
		// Validate
		_validateMetadata(pDataset, pBand, filepath, expectedType);
		
		int blockXSize, blockYSize;
		pBand->GetBlockSize(&blockXSize, &blockYSize);
	
		int isNoDataValid;
		const T nodata = (T)pBand->GetNoDataValue(&isNoDataValid);
		if (defaultNodata == RasterIO::NODATA_BYTE)
		{
			//cast to int, otherwise value will show as char
			if (isNoDataValid)
				ShowOutput(MAXIMUM, "\t\t\tFound NoData metadata tag: " + ToS((int)nodata) + "\n");
			else
				ShowOutput(MAXIMUM, "\t\t\tWarning -- unable to find NoData metadata tag. Defaulting to " + ToS((int)defaultNodata) + "\n");
		}
		else
		{
			if (isNoDataValid)
				ShowOutput(MAXIMUM, "\t\t\tFound NoData metadata tag: " + ToS(nodata) + "\n");
			else
				ShowOutput(MAXIMUM, "\t\t\tWarning -- unable to find NoData metadata tag. Defaulting to " + ToS(defaultNodata) + "\n");
		}
	
		// check pointer
		if (isMalloc && pMatrix != NULL)
			throw ReadRasterException("logic bug -- matrix must be null before allocating memory to it. File", filepath);
		if (!isMalloc && pMatrix == NULL)
			throw ReadRasterException("logic bug -- matrix must not be null when not allocating new memory to it. File", filepath);
		
		// allocate row references
		if (isMalloc) 
			pMatrix = (T**) malloc(sizeof(T*) * _ySize);
		if (pMatrix == NULL) 
			throw ReadRasterException("unable to allocate memory needed to read raster file at " + filepath);

		CPLErr eErr = CE_None;
		for (int r=0; r < _ySize && eErr == CE_None; r++)
		{
			// allocate a row
			if (isMalloc) 
				pMatrix[r] = (T*) malloc(sizeof(T) * _xSize);	
			if (pMatrix[r] == NULL) 
				throw ReadRasterException("Unable to allocate memory needed to read raster file at " + filepath);
			
			// prepopulate with nodata
			for (int c=0; c < _xSize; c++)
				pMatrix[r][c] =  defaultNodata;
	
			// read in a row of data
			eErr = pBand->RasterIO(GF_Read, _xOffset, r + _yOffset, _xSize, 1, pMatrix[r], _xSize, 1, expectedType, 0, 0);

			// replace any of the file's nodata values with the default, or if metadata 
			// doesn't specify a nodata value, then replace some of the typical values used 
			if (isNoDataValid)
			{
				if (nodata != defaultNodata)
				{
					for (int c=0; c < _xSize; c++)
						if (isNodata(pMatrix[r][c]))
							pMatrix[r][c] = defaultNodata;
				}
			}
			else if (alternateNodata) // should only be true for float
			{
				for (int c=0; c < _xSize; c++)
					if (alternateNodata == pMatrix[r][c])
						pMatrix[r][c] = defaultNodata;
			}
		}
		
		// Clean up
		if(pDataset != NULL) 
			GDALClose((GDALDatasetH)pDataset);
		switch(eErr)
		{
			case CPLE_None:
				break;
			case CPLE_OutOfMemory:
				throw ReadRasterException("out of memory! when reading raster file at " + filepath);
				break;
			default:
				throw ReadRasterException("failed to read raster file at " + filepath);
				break; 
		}
	}
	catch(ReadRasterException& e)
	{
		if(pDataset != NULL) 
			GDALClose((GDALDatasetH)pDataset);
		zapRasterMemory(pMatrix);
		e.rethrow();
	}
	catch(...)
	{
		if (pDataset != NULL)
			GDALClose((GDALDatasetH)pDataset);
		throw ReadRasterException("unkown error while reading file at " + filepath);			
	}
}




//
//
//	Template for _writeRasterFile<type>
//
//
template void RasterIO::_writeRasterFile<byte>   (const string filepath, Frame*** pFrames, ALFMapType mapType, GDALDataType dataType, const std::string &description, GDALColorTable* pColorTable);
template void RasterIO::_writeRasterFile<int>    (const string filepath, Frame*** pFrames, ALFMapType mapType, GDALDataType dataType, const std::string &description, GDALColorTable* pColorTable);
template void RasterIO::_writeRasterFile<float>  (const string filepath, Frame*** pFrames, ALFMapType mapType, GDALDataType dataType, const std::string &description, GDALColorTable* pColorTable);
template<class T> void RasterIO::_writeRasterFile(const string filepath, Frame*** pFrames, ALFMapType mapType, GDALDataType dataType, const std::string &description, GDALColorTable* pColorTable)
{
	//TODO: validate driver in class constructor
	const char *pszFormat = "GTiff";
	GDALDriver *pDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	if(pDriver == NULL)
		throw WriteRasterException("unable to find the GeoTIFF driver");
	
	char **pMetadata = pDriver->GetMetadata();
	if( !CSLFetchBoolean( pMetadata, GDAL_DCAP_CREATE, FALSE ) )
		throw WriteRasterException("the GeoTIFF driver that was found does not support creating new files");
	//TODO: free pMetadata? or is it owned by the driver?

	char** pWriteOptions = NULL;
	pWriteOptions = CSLSetNameValue(pWriteOptions, "COMPRESS", "LZW");
	if (pColorTable != NULL)
		pWriteOptions = CSLSetNameValue(pWriteOptions, "PHOTOMETRIC", "PALETTE");

	EnsureDirectoryExists(filepath, true);
	GDALDataset *pDataset = pDriver->Create(filepath.c_str(), this->_xSize, this->_ySize, 1, dataType, pWriteOptions);
	if(pDataset == NULL)
		throw WriteRasterException("unable to create output file at " + filepath);



	T* buf;
	try
	{
		pDataset->SetGeoTransform(this->_geoTransform);
		if (_applyAaeacToOutput)
			pDataset->SetProjection(this->_pSpatialReference);
	
		GDALRasterBand *pBand = pDataset->GetRasterBand(1);
	
		T nodata;
		this->getNodata(nodata);
		if (CE_None != pBand->SetNoDataValue(nodata))
			throw WriteRasterException("unable to set the metadata tag value for NODATA");

		pDataset->SetMetadataItem("TIFFTAG_SOFTWARE", _softwareDescription.c_str());
		pDataset->SetMetadataItem("TIFFTAG_IMAGEDESCRIPTION", description.c_str());

		if (pColorTable != NULL)
		{
			//if (CE_None != pBand->SetColorInterpretation(GCI_PaletteIndex))
			//	throw WriteRasterException("unable to set the color interpratation");
			if (CE_None != pBand->SetColorTable(pColorTable))
				throw WriteRasterException("unable to set the color table");
		}

		buf = (T*) malloc(this->_xSize * sizeof(T));
		if (buf == NULL) throw WriteRasterException("out of memory!");

		// Set the write buffer values per pixel. Evaluate whether the nodata 
		// value is appropriate.
		CPLErr err = CE_None;
		float fscar = 0.0;
		float scale = 0.0;
		for (int r=0;  r < this->_ySize  &&  err == CE_None;  r++)
		{
			for (int c=0; c<this->_xSize; c++) 
			{
				switch(mapType)
				{
				case VEGEGATION:
					buf[c] = pFrames[r][c]->type();
					break;
				case AGE:
					if (pFrames[r][c]->type() == gNoVegID)
						buf[c] = nodata;
					else
						buf[c] = pFrames[r][c]->age();
					break;
				case SUBCANOPY:
					if (pFrames[r][c]->type() == gNoVegID 
						|| ((byte)pFrames[r][c]->speciesSubCanopy() == gNoVegID)
						|| ((byte)pFrames[r][c]->speciesSubCanopy() == nodata))
						buf[c] = nodata;
					else
						buf[c] = (byte)pFrames[r][c]->speciesSubCanopy();
					break;
				case SITE_VARIABLE:
					if (pFrames[r][c]->type() == gNoVegID)
						buf[c] = nodata;
					else
						buf[c] = pFrames[r][c]->site();
					break;
				case FIRE_AGE:
					if (pFrames[r][c]->type() == gNoVegID || pFrames[r][c]->yearOfLastBurn < 0)
						buf[c] = nodata;
					else
						buf[c] = pFrames[r][c]->yearOfLastBurn;
					break;
				case FIRE_SCAR:
					if (pFrames[r][c]->type() == gNoVegID || pFrames[r][c]->yearOfLastBurn < 0)
					{
						buf[c] = nodata;
					}
					else
					{
						// format: [if ignition cell use -, otherwise +][LastBurnYear].[FireID]
						if      (pFrames[r][c]->fireScarID < 10)       scale = 0.1;
						else if (pFrames[r][c]->fireScarID < 100)      scale = 0.01;
						else if (pFrames[r][c]->fireScarID < 1000)     scale = 0.001;
						else if (pFrames[r][c]->fireScarID < 10000)    scale = 0.0001;
						else if (pFrames[r][c]->fireScarID < 100000)   scale = 0.00001;
						else if (pFrames[r][c]->fireScarID < 1000000)  scale = 0.000001;
						else if (pFrames[r][c]->fireScarID < 10000000) scale = 0.0000001;
						fscar = pFrames[r][c]->yearOfLastBurn + (pFrames[r][c]->fireScarID * scale);
						if (pFrames[r][c]->lastBurnWasOrigin) fscar *= -1;
						buf[c] = fscar;
					}
					break;
				case BURN_SEVERITY:
					if (pFrames[r][c]->type() == gNoVegID || pFrames[r][c]->yearOfLastBurn != gYear)
						buf[c] = nodata;
					else
						buf[c] = (int)pFrames[r][c]->burnSeverity;
					break;
				case BURN_SEVERITY_HISTORY:
					if (pFrames[r][c]->type() == gNoVegID)
						buf[c] = nodata;
					else
						buf[c] = (int)pFrames[r][c]->burnSeverity;
					break;
				case DECID_SPECIES_TRAJECTORY:
					if (pFrames[r][c]->type() == gDecidID)
						buf[c] = pFrames[r][c]->getAsByte(DECID_SPECIES_TRAJECTORY);
					else
						buf[c] = nodata;
					break;
				case TUNDRA_BASAL_AREA:
					if (pFrames[r][c]->type() == gTundraID)
						buf[c] = pFrames[r][c]->getAsFloat(TUNDRA_BASAL_AREA);
					else
						buf[c] = nodata;
					break;
				default:
					throw WriteRasterException("undefined map type");
				}
			}
			// Write one row
			err = pBand->RasterIO(GF_Write, 0, r, this->_xSize, 1, 
									buf, this->_xSize, 1, 
									dataType, 0, 0);
		}

		if (buf != NULL) free(buf);
		if (pDataset != NULL)
			GDALClose( (GDALDatasetH) pDataset );
		CSLDestroy(pWriteOptions);
		if (err != CE_None)
			throw WriteRasterException("failed to write to file at " + filepath);
	}
	catch(WriteRasterException& e)
	{
		if (buf != NULL) free(buf);
		if (pDataset != NULL)
			GDALClose( (GDALDatasetH) pDataset );
		CSLDestroy(pWriteOptions);
		e.rethrow();
	}
	catch(...)
	{
		if (buf != NULL) free(buf);
		if (pDataset != NULL)
			GDALClose( (GDALDatasetH) pDataset );
		CSLDestroy(pWriteOptions);
		throw WriteRasterException("unkown error while writing to file at " + filepath);			
	}
}



void RasterIO::_validateProjectionMetadata(GDALDataset* pDataset, std::queue<string> &errors)
{
	using std::string;
	std::ostringstream s; 
	s << std::fixed;

	if (!_requireAaeacForInput)
		return;

	OGRSpatialReference sr(OGRSpatialReference(pDataset->GetProjectionRef()));
	if (sr.GetRoot() == NULL) {
		s << "the projection metadata is empty. Expected the Alaska Albers Equal Area Conic projection."; 
		errors.push(s.str()); s.str("");
		return;
	}
	if (!sr.IsProjected()) {
		string rootVal(sr.GetRoot()->GetValue());
		s << "expected a PROJCS node at the root of the projection metadata but found " << (rootVal.empty() ? "[empty]" : rootVal);
		errors.push(s.str()); s.str("");
		return;
	}

	// Check GEOCS and it's children nodes...
	//const char *val = sr.GetAttrValue("GEOCS");
	//std::string sval(stolower(val));
	//if (val==NULL || sval!="nad83" || !npos==sval.find("north_american") || !npos==sval.find("1983")) {
	//	s << "expected the projection's GEOCS name to be \"GCS_North_American_1983\" but found " 
	//		<< (val == NULL ? "[empty]" : val);
	//	errors.push(s.str()); s.str(""); if (val) delete[] val;
	//}
	// ...check prime meridian
	const char* val1 = sr.GetAttrValue("DATUM");
	std::string sval(stolower((char*)val1));
	if (val1==NULL || (string::npos==sval.find("north_american") && string::npos==sval.find("1983") && sval!="nad83")) {
		s << "expected the projection's DATUM to be \"NAD83\" but found " 
			<< (val1 == NULL ? "[empty]" : val1);
		errors.push(s.str()); s.str("");
	}
	// ...check spheriod's semi-major axis
	const char* val2 = sr.GetAttrValue("SPHEROID", 1);
	if (val2!=NULL)
	{
		if (!doubleEquals(6378137.0, CPLAtof(val2), 0.0001)) {
			s << "expected the projection's Semi-major Axis for SPHEROID to be 6378137.0 but found " << val2;
			errors.push(s.str()); s.str("");
		}
	}
	// ...check spheriod's inverse flattening
	const char* val3 = sr.GetAttrValue("SPHEROID", 2);
	if (val3!=NULL)
	{
		if (!doubleEquals(298.257222101, CPLAtof(val3), 0.000000001)) {
			s << "expected the projection's Inverse Flattening for SPHEROID to be 298.257222101 but found " << val3;
			errors.push(s.str()); s.str("");
		}
	}
	// ...check prime meridiam
	const char* val4 = sr.GetAttrValue("PRIMEM", 1);
	if (val4 != NULL) 
	{
		if (!doubleEquals(CPLAtof(val4), 0.0, 0.0001)) {
			s << "expected the projection's prime meridian to be \"Greenwich\" with a value of 0.0 but found " << val4;
			errors.push(s.str()); s.str("");
		}
	}
	// ...do the angular units match?
	const char* val5 = sr.GetAttrValue("GEOGCS|UNIT", 1);
	if (val5!=NULL)
	{
		if (!doubleEquals(CPLAtof(val5), 0.0174532925199433, 0.0000000000001) ) {
			s << "expected the projection GEOGCS UNITS to be 0.0174532925199433 (PI/180) but found " << val5;
			errors.push(s.str()); s.str("");
		}
	}

	// Check PROJECTION...
	const char* val6 = sr.GetAttrValue( "PROJECTION" );
	sval = stolower((char*)val6);
	if (val6 == NULL || string::npos == sval.find("albers")) {
		s << "expected projection parameter PROJECTION to be \"Albers_Conic_Equal_Area\" but found " 
		  << (val6 == NULL ? "[empty]" : val6);
		errors.push(s.str()); s.str("");
	}
	// ...check projection parameters
	double v = sr.GetProjParm("False_Easting");
	if (!doubleEquals(v, 0.0, 0.0001)) {
		s << "expected the value for projection parameter \"False_Easting\" to be 0 but found " << v;
		errors.push(s.str()); s.str("");
	}
	v = sr.GetProjParm("False_Northing");
	if (!doubleEquals(v, 0.0, 0.0001)) {
		s << "expected the value for projection parameter \"False_Northing\" to be 0 but found " << v;
		errors.push(s.str()); s.str("");
	}
	v = sr.GetProjParm("Central_Meridian");
	if (!doubleEquals(v, -154.0, 0.0001)) {
		s << "expected the value for projection parameter \"Central_Meridian\" to be -154 but found " << v;
		errors.push(s.str()); s.str("");
	}
	v = sr.GetProjParm("Standard_Parallel_1");
	if (!doubleEquals(v, 55.0, 0.0001)) {
		s << "expected the value for projection parameter \"Standard_Parallel_1\" to be 55 but found " << v;
		errors.push(s.str()); s.str("");
	}
	v = sr.GetProjParm("Standard_Parallel_2");
	if (!doubleEquals(v, 65.0, 0.0001)) {
		s << "expected the value for projection parameter \"Standard_Parallel_2\" to be 65 but found " << v;
		errors.push(s.str()); s.str("");
	}
	v = sr.GetProjParm("Latitude_Of_Origin");
	if (!doubleEquals(v, 50.0, 0.0001)) {
		s << "expected the value for projection parameter \"Latitude_Of_Origin\" to be 50 but found " << v;
		errors.push(s.str()); s.str("");
	}
	// ...check linear unit conversion
	v = sr.GetLinearUnits();
	if (!doubleEquals(v, 1.0, 0.0001)) {
		s << "expected the multiplier used to convert the projection's linear unit into meters to be 1 but found " << v;
		errors.push(s.str()); s.str("");
	}
}


void RasterIO::_validateMetadata(GDALDataset* pDataset, GDALRasterBand* pBand, const std::string filepath, const GDALDataType expectedType)
{
	// Get metadata	
	double geoTransform[6];
	if (pDataset->GetGeoTransform(geoTransform) != CE_None)
		throw ReadRasterException("unable to retrieve geo transform metadata for file at " + filepath);
	double xOrigin    = geoTransform[0];
	double yOrigin    = geoTransform[3];
	double xPixelSize = geoTransform[1];
	double yPixelSize = geoTransform[5];
	double xRotation  = geoTransform[2];
	double yRotation  = geoTransform[4];
	int xRasterSize   = pDataset->GetRasterXSize();
	int yRasterSize   = pDataset->GetRasterYSize();
	int xBandSize     = pBand->GetXSize();
	int yBandSize     = pBand->GetYSize();
	GDALDataType datatype   = pBand->GetRasterDataType();	


	queue<string> errors;
	std::ostringstream s; 
	s << std::fixed;

	if (datatype != expectedType) {
		s << "expected the datatype to be " << GDALGetDataTypeName(expectedType) 
			<< " but found " << GDALGetDataTypeName(datatype); 
		errors.push(s.str()); s.str("");
	}

	if (!doubleEquals(xOrigin, _xInputOrigin, 0.00001)  || !doubleEquals(yOrigin, _yInputOrigin, 0.00001)) {
		s << "expected an origin of (" << _xInputOrigin << ", " <<  _yInputOrigin << ")"
			<< " but found (" << xOrigin << ", " <<  yOrigin << ")";
		errors.push(s.str()); s.str("");
	}

	if (!doubleEquals(xPixelSize, _xPixelSize, 0.00001)  || !doubleEquals(yPixelSize, _yPixelSize, 0.00001)) {
		s << "expected a raster pixel size of " << _xPixelSize << " x " <<  _yPixelSize
			<< " but found " << xPixelSize << " x " <<  yPixelSize;
		errors.push(s.str()); s.str("");
	}

	if (!doubleEquals(xRotation, _xRotation, 0.00001)  || !doubleEquals(yRotation, _yRotation, 0.00001)) {
		s << "expected a rotation of (" << _xRotation << ", " <<  _yRotation << ")"
			<< " but found (" << xRotation << ", " <<  yRotation << ")";
		errors.push(s.str()); s.str("");
	}

	if (xRasterSize < _xSize + _xOffset) {
		s << "expected raster size to be at least " << _xSize + _xOffset << " columns wide"
			<< " but found only " << xRasterSize << " columns";
		errors.push(s.str()); s.str("");
	}
	if (yRasterSize < _ySize + _yOffset) {
		s << "expected raster size to be at least " <<  _ySize + _yOffset << " rows tall"
			<< " but found only " <<  yRasterSize << " rows";
		errors.push(s.str()); s.str("");
	}


	if (xBandSize < _xSize + _xOffset) {
		s << "expected raster band size to be at least " << _xSize + _xOffset << " columns wide"
			<< " but found only " << xBandSize << " columns";
		errors.push(s.str()); s.str("");
	}
	if (yBandSize < _ySize + _yOffset) {
		s << "expected raster band size to be at least " <<  _ySize + _yOffset << " rows tall"
			<< " but found only " <<  yBandSize << " rows";
		errors.push(s.str()); s.str("");
	}


	_validateProjectionMetadata(pDataset, errors);

	if (!errors.empty()) {
		int count = 1;
		while(!errors.empty()) {
			s << " (" << count << ") " << errors.front() << "; " << std::endl;
			errors.pop();
			count++;
		}
		throw ReadRasterException("invalid metadata in file at " + filepath + ". The following issues were found", s.str());
	}

	//std::ios_base::fmtflags originalFormat = cout.flags();
	//cout << "METADATA" << endl;
	//cout << std::fixed << "\txOrigin\t\t" << xOrigin << endl;
	//cout << "\tyOrigin\t\t" << yOrigin << endl;    
	//cout << "\txPixelSize\t" << xPixelSize << endl; 
	//cout << "\tyPixelSize\t" << yPixelSize << endl; 
	//cout << "\txRotation\t" << xRotation << endl;  
	//cout << "\tyRotation\t" << yRotation << endl;  
	//cout << "\txRasterSize\t" << xRasterSize << endl;   
	//cout << "\tyRasterSize\t" << yRasterSize << endl;   
	//cout << "\txBandSize\t" << xBandSize << endl;     
	//cout << "\tyBandSize\t" << yBandSize << endl;
	//cout << "\tdatatype\t" << GDALGetDataTypeName(datatype) << endl;
	//cout.flags(originalFormat); // reset flags
}


//void RasterIO::showLimits()
//{
//	using namespace std;
//
//	cout << endl << "NUMERIC LIMITS" << endl << endl;
//	cout << "    \t|\tbyte\t\tshort\t\tint\t\tfloat\t\tdouble" << endl;
//	cout << "------------------------------------------------------------------------" << endl;
//
//	cout << "min \t|\t" 
//				<< (int)numeric_limits<unsigned char>::min() << "\t\t"
//				<< numeric_limits<short>::min()  << "\t"
//				<< numeric_limits<int>::min()  << "\t"
//				<< -numeric_limits<float>::max()  << "\t"
//				<< -numeric_limits<double>::max()  << "\t"
//				<< endl;
//
//	cout << "max \t|\t" 
//				<< (int)numeric_limits<unsigned char>::max()  << "\t\t"
//				<< numeric_limits<short>::max()  << "\t"
//				<< numeric_limits<int>::max()  << "\t"
//				<< numeric_limits<float>::max()  << "\t"
//				<< numeric_limits<double>::max()  << "\t"
//				<< endl;
//
//	cout << "snan\t|\t" 
//				<< (int)numeric_limits<unsigned char>::signaling_NaN()  << "\t\t"
//				<< numeric_limits<short>::signaling_NaN()  << "\t\t"
//				<< numeric_limits<int>::signaling_NaN()  << "\t\t"
//				<< numeric_limits<float>::signaling_NaN()  << "\t\t"
//				<< numeric_limits<double>::signaling_NaN()  << "\t\t"
//				<< endl;
//
//	cout << "qnan\t|\t" 
//				<< (int)numeric_limits<unsigned char>::quiet_NaN()  << "\t\t"
//				<< numeric_limits<short>::quiet_NaN()  << "\t\t"
//				<< numeric_limits<int>::quiet_NaN()  << "\t\t"
//				<< numeric_limits<float>::quiet_NaN()  << "\t\t"
//				<< numeric_limits<double>::quiet_NaN()  << "\t\t"
//				<< endl;
//				
//	cout << endl;
//}














