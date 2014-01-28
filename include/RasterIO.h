#ifndef ALF_RasterIO_H
#define ALF_RasterIO_H
//
// RasterIO.cpp
//
// Reads and writes raster GIS maps using the GDAL API.
//
// Supported Input: 
//  - GeoTIFF
//
// Supported Output:
//  - GeoTIFF
//
// Support NODATA:
//  - Best performance is seen when using the default NODATA value for the given datatype 
//    AND set the NODATA metadata tag with the same value (see Supported Datatypes below 
//    for defaults).
//  - If the NODATA value metadata tag is not specified, the default NODATA value 
//    for the datatype is used.
//
// Supported Datatypes:
//            |   MIN         |     MAX      |    NODATA
//  ---------------------------------------------------------
//    Byte    |            0  |         255  |          255
//    INT32   |  -2147483648  |  2147483648  |  -2147483648
//    FLOAT32 |     -3.4e+38  |     3.4e+38  |     -3.4e+38
//  
//
//
// UnitTests:
//  - read GeoTIFF without LZW
//  - read GeoTIFF with LZW
//  - write GeoTIFF with LZW compression
//  - memory capacity tests to see how many rasters, each in contiguous chunks of 
//    memory can be used.
//
// Known Issues: 
//  - reading ASCII --- origin is off
//  - not able to write to ASCII using GDAL.
//
// Todo
//	- int32_t?
//	- transfer memory allocation and deallocation to RasterIO class (with the isMalloc flag)
//	  - use single call to malloc, assign row pointers, fill array, free
//  - prepopulation neccessary?
//	- namespace Alf
//  - time comparisons


#include "PreCompiled.h"
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>
#include <queue>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>


class Frame;

typedef unsigned char   byte;


class FrescoFoundation_API RasterIO
{
public:
	static const byte  NODATA_BYTE;
	static const int   NODATA_INT;
	static const float NODATA_FLOAT;
	static const float NODATA_FLOAT_ALTERNATE;

	enum ALFMapType 
	{
		VEGETATION,
		AGE,
		SUBCANOPY,
		SITE_VARIABLE,
		FIRE_AGE,                 // time since the cell last burned
		FIRE_SCAR,                // the ID of each fire scar [if ignition cell, '-'][LastBurnYear].[FireID]
		BURN_SEVERITY,            // for burns occuring in the given year
		BURN_SEVERITY_HISTORY,    // for the last known burn of each cell
		DECID_SPECIES_TRAJECTORY,
		TUNDRA_BASAL_AREA
	};

	RasterIO(double xOrigin, double yOrigin, int xOffset, int yOffset, int xSize, int ySize,  
				double xPixelSize, double yPixelSize, double xRotation, double yRotation, 
				const std::string &softwareDescription = "ALFRESCO from UAF",
				bool requireAaeacForInput = true, bool applyAaeacToOutput = true);
	~RasterIO();

	//void showLimits();

	void getNodata(byte &result);
	void getNodata(int &result);
	void getNodata(float &result);
	void getAlternateNodata(byte &result);
	void getAlternateNodata(int &result);
	void getAlternateNodata(float &result);
	void zapRasterMemory(byte** &matrix);
	void zapRasterMemory(int** &matrix);
	void zapRasterMemory(float** &matrix);
	void readRasterFile(const std::string filepath, byte**  &matrix, const bool isMalloc);
	void readRasterFile(const std::string filepath, int**   &matrix, const bool isMalloc);
	void readRasterFile(const std::string filepath, float** &matrix, const bool isMalloc);
	void writeRasterFile(const std::string filepath, Frame*** pFrames, ALFMapType mapType, const int year, const int rep); 

	ALFMapType getMapType(int legacyStyleMapFlags);
	static const std::string getMapTypeAsString(ALFMapType type);

	char* getProjection() 
	{
		return _pSpatialReference; 
	};

	bool doubleEquals(double left, double right, double epsilon) const
	{
		return (std::abs(left - right) < epsilon);
	};
	bool floatEquals(float left, float right, float epsilon) const
	{
		return (std::abs(left - right) < epsilon);
	};

	const bool isNodata(const byte val) const
	{
		return (val == RasterIO::NODATA_BYTE);
	};
	const bool isNodata(const int val) const
	{
		return (val == RasterIO::NODATA_INT);
	};
	const bool isNodata(const float val) const
	{
		return floatEquals(val, RasterIO::NODATA_FLOAT, 0.00001);
	};
	const bool isAltNodata(const byte val) const
	{
		return false;
	};
	const bool isAltNodata(const int val) const
	{
		return false;
	};
	const bool isAltNodata(const float val) const
	{
		return floatEquals(val, RasterIO::NODATA_FLOAT_ALTERNATE, 0.00001);
	};
  
protected:
  
private:
	int _xOffset;
	int _yOffset;
	int _xSize;
	int _ySize;
	double _xInputOrigin; 
	double _yInputOrigin;
	double _xOutputOrigin;
	double _yOutputOrigin;
	double _xPixelSize; 
	double _yPixelSize;
	double _xRotation;
	double _yRotation;
	double _geoTransform[6];
	// Geo Transform index reference:
	// 0 -- top left x
	// 3 -- top left y
	// 1 -- west to east pixel resolution
	// 5 -- north to south pixel resolution
	// 2 -- rotation, 0 if image is "north up"
	// 4 -- rotation, 0 if image is "north up"
	std::string _softwareDescription;
	std::vector<std::string> _mapDescriptions;


	bool _requireAaeacForInput;
	bool _applyAaeacToOutput;

	char* _pSpatialReference;
	GDALColorTable* _pVegColorTable;
	GDALColorTable* _pBurnSevColorTable;

	template<class T> void _readRasterFile (const std::string filepath, T** &pMatrix, const bool isMalloc, const GDALDataType expectedType);
	template<class T> void _writeRasterFile(const std::string filepath, Frame*** pFrames, ALFMapType mapType, GDALDataType dataType, const std::string &description = "", GDALColorTable* pColorTable = NULL);
	void _validateMetadata(GDALDataset* pDataset, GDALRasterBand* pB, const std::string filepath, const GDALDataType expectedType);
	void _validateProjectionMetadata(GDALDataset* pDataset, std::queue<std::string> &errors);
	std::string stolower(const std::string& s)
	{
		std::string result(s);
		std::transform(s.begin(), s.end(), result.begin(), std::ptr_fun <int, int> (::tolower));
		return result;
	}
};
#endif
