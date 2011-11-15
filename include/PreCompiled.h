#ifndef Fresco_PreCompiled_h
#define Fresco_PreCompiled_h

#pragma warning(disable: 4305 4244)			//Disable the warnings concerning conversion of doubles to floats

#define Fresco_Is_Client

#ifndef FRESCO_FOUNDATION_INCLUDED
    #define FRESCO_FOUNDATION_INCLUDED
    #include "Poco/Poco.h" // get system specific stuff
    #if defined(_WIN32) && defined(POCO_DLL)
	    #if defined(FRESCO_FOUNDATION_EXPORTS)
	    	#define FrescoFoundation_API __declspec(dllexport)
	    #else
	    	#define FrescoFoundation_API __declspec(dllimport)
	    #endif
    #endif
    #if !defined(FrescoFoundation_API)
    	#define FrescoFoundation_API
    #endif
#endif  //FRESCO_FOUNDATION_INCLUDED


#include "RasterIO.h"

//Standard Template Libraries
#include <iostream>
#include <sstream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
//VC libraries
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#ifdef WIN32
  #include <direct.h>
  #include <crtdbg.h>
#endif

//Define constants needed by the random number generator
#define		IA 16807					//Multiplier
#define		IM 2147483647				//Modulus
#define		AM (1.0/IM)					//Inverse of modulus
#define		IQ 127773					//Integer quotient of IM/IA
#define		IR 2836						//Remainder of IM/IA
#define		NTAB 32						//Number of bins to use in shuffling algorithm
#define		NDIV (1+(IM-1)/NTAB)		//Divisor producing a number on range [0,NTAB-1]
#define		EPS 1.2e-7					//A small(ish) number
#define		RNMX (1.0-EPS)				//A big(ish) number less than 1
#define		PI 3.1415926535				//Define PI
#define		BUFSIZE 512					//Define the default buffer size
const int	FRS_NULL					= -1;


typedef unsigned char byte;


//Define the output codes.  The resulting flag will be a sum of the relevent flags. Note that these are different
//than the codes specified in stat.h.  Those are the basal flags for each statistical variable specified at program
//initiation, these are the specific output flags used in the main program to specify particular output.
enum outCode  //Landscape
{
	//General codes - if not specified, nothing will get printed  
	outData				= 0x1,							//Output the data
	//outYearStats		= 0x2,							//Output the statistical data by year
	//outRepStats		= 0x4,							//Output the statistical data by rep
	//Format codes - specify the format of the output data and headers
	outFormat			= 0x10,							//Output the data so it is conducive to read it into a stats package for analysis
	outHeader			= 0x20,							//Tack a header onto the output with the number of years/reps for reference purposes
	outARC				= 0x40,							//Attach an header to the output file to facilitate import into ARCgis
	//Frame codes - used with outFrame
	out1				= 0x100,						//The exact data output is determined by the implementation of the frame class
	out2				= 0x200,
	out3				= 0x400,
	out4				= 0x800,
	//Data codes - used with outData
	outVeg				= 0x1000,						//The veg type
	outAge				= 0x2000,						//The stand age
	outSite				= 0x4000,						//The site variables
	outSub				= 0x8000,						//The subcanopy
	outFireAge			= 0x10000,						//The time since the cell last burned
	outTemp				= 0x20000,						//The temp offset
	outPrecip			= 0x40000,						//The precip offset
	outFireScar			= 0x80000,						//The ID of each fire scar [If ignition cell, '-'][LastBurnYear].[FireID]
	outfireSeverity		= 0x100000,						//The burn severity in a given year
	outfireSeverityHist	= 0x200000						//The last burn severity.
};
enum EStartAgeType		{CONSTANT, WEIBULL};    //Frame or Species
enum EDetailLevel		{MINIMAL, MODERATE, MAXIMUM};


//Global FRESCO object.
class Fresco;
extern Fresco       FrescoFoundation_API    *FRESCO;
class Climate;
extern Climate FrescoFoundation_API    *gClimate;

//Global simulation settings.
extern std::string  FrescoFoundation_API    gWorkingDirectory;
extern std::string  FrescoFoundation_API    gInputBasePath;
extern std::string  FrescoFoundation_API    gOutputBasePath;
extern std::string  FrescoFoundation_API    gOutputDirectory;
extern int          FrescoFoundation_API    gMaxRep;
extern int          FrescoFoundation_API    gRep;
extern int          FrescoFoundation_API    gFirstYear;
extern int          FrescoFoundation_API    gLastYear;
extern int          FrescoFoundation_API    gYear;
extern float        FrescoFoundation_API    gCellSize;
extern int          FrescoFoundation_API    gYOffset;
extern int          FrescoFoundation_API    gXOffset;
extern int          FrescoFoundation_API    gYSize;
extern int          FrescoFoundation_API    gXSize;
extern long         FrescoFoundation_API    gNumSpecies;
extern EDetailLevel FrescoFoundation_API    gDetailLevel;
extern byte          FrescoFoundation_API    gNoVegID;
extern byte          FrescoFoundation_API    gTundraID;
extern byte          FrescoFoundation_API    gDecidID;
extern byte          FrescoFoundation_API    gBSpruceID;
extern byte          FrescoFoundation_API    gWSpruceID;
extern byte          FrescoFoundation_API    gGrasslandID;
extern long          FrescoFoundation_API    gTallyOfRandCalls;



void	    FrescoFoundation_API            ShowOutput(const EDetailLevel detailLevel, const std::string output);
void	    FrescoFoundation_API            DoNothing(const bool doBreak);
void	    FrescoFoundation_API            ShowOutput(const std::string output);
void	    FrescoFoundation_API            ShowOutput(std::ostringstream& rOutput);
void	    FrescoFoundation_API            WriteDebug(const std::string message);
std::string FrescoFoundation_API            AppendRepYear(const std::string file, const int rep=gRep, const int year=gYear);
std::string	FrescoFoundation_API			AppendYearMonth(const std::string file, const int year, const int month);
std::string FrescoFoundation_API            AppendYear(const std::string file, const int year=gYear);
bool	    FrescoFoundation_API            InputFileExists(std::string path);
std::string FrescoFoundation_API            FormatDirectory(std::string path);
void	    FrescoFoundation_API            EnsureDirectoryExists(std::string path, bool includesFilename);
std::string FrescoFoundation_API            GetFullPath(const std::string base, const std::string path);
template <class nonString> 
std::string FrescoFoundation_API            ToS(nonString NonString);
std::string FrescoFoundation_API            ToS(bool nonString);
void	    FrescoFoundation_API            FileCheck(std::fstream& rStream, std::string fileName, int& rFileRows, int& rFileCols);
//template<class T> void FrescoFoundation_API ReadGISFile(T** Array, const int rows, const int cols, const std::string filename, const int flags, const T Default);

//Global math functions.
double	FrescoFoundation_API                ConstDist(const double* const params);
double	FrescoFoundation_API                NormDist(const double* const params);
double	FrescoFoundation_API                WeibullDist(const double* const params);
double	FrescoFoundation_API                WeibullReliability(const double* const params);
double	FrescoFoundation_API                WeibullHazard(const double* const params);
double	FrescoFoundation_API                FatTail(const double* const params);
double	FrescoFoundation_API                Site(const double site, const double exp);
double	FrescoFoundation_API                Logistic(const double age, const double* const params);
double	FrescoFoundation_API                LinInterp(double x, double y, const double* const m, const int dimX, const int dimY, const bool trunc);
void    FrescoFoundation_API                Integrate(double (*Func)(const double* const), const double* params, double* const result, const double start, const double end, const double Step);
long	FrescoFoundation_API                SeedRandom(long seed);
const float FrescoFoundation_API            GetNextRandom();
const float	FrescoFoundation_API            GetNextRandomNorm(const double mean, const double stdDev);
const bool	FrescoFoundation_API            DoublesEqual(const double left, const double right, const double epsilon);
const bool  FrescoFoundation_API            FloatsEqual(const float left, const float right, const float epsilon);

//Global IO functions.
class RasterIO;
extern RasterIO	    FrescoFoundation_API    *gIO;
//void	    FrescoFoundation_API            GetNoData(byte &returnVal);
//void	    FrescoFoundation_API            GetNoData(int &returnVal);
//void	    FrescoFoundation_API            GetNoData(float &returnVal);
//const byte  FrescoFoundation_API            ByteNodata();
//const int   FrescoFoundation_API            IntNodata();
//const float FrescoFoundation_API            FloatNodata();
const bool	FrescoFoundation_API            IsNodata(const byte val);
const bool	FrescoFoundation_API            IsNodata(const int val);
const bool	FrescoFoundation_API            IsNodata(const float val);

inline const bool DoublesEqual(const double left, const double right, const double epsilon)
{
	return (std::abs(left - right) < epsilon);
}
inline const bool FloatsEqual(const float left, const float right, const float epsilon)
{
	return (std::abs(left - right) < epsilon);
}

inline const bool IsNodata(const byte val)
{
	return (val == RasterIO::NODATA_BYTE);
}
inline const bool IsNodata(const int val)
{
	return (val == RasterIO::NODATA_INT);
}
inline const bool IsNodata(const float val)
{
	return FloatsEqual(val, RasterIO::NODATA_FLOAT, 0.00001);
}

//inline const byte ByteNodata()
//{
//	return RasterIO::NODATA_BYTE;
//}
//inline const int IntNodata()
//{
//	return RasterIO::NODATA_INT;
//}
//inline const float FloatNodata()
//{
//	return RasterIO::NODATA_FLOAT;
//}

//inline void					GetNoData(byte &returnVal)
//{
//	returnVal = RasterIO::NODATA_BYTE;
//}
//inline void					GetNoData(int &returnVal)
//{
//	returnVal = RasterIO::NODATA_INT;
//}
//inline void					GetNoData(float &returnVal)
//{
//	returnVal = RasterIO::NODATA_FLOAT;
//}


inline std::string		ToS(bool value)
{
    return (value ? "true" : "false");
}
template <class nonString>
inline std::string      ToS(nonString value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();   
}

#endif
