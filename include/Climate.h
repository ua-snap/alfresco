#ifndef Fresco_Climate_h
#define Fresco_Climate_h


#include "PreCompiled.h"
#include "Except.h"
#include "jsoncpp/json/json.h"
#include "Poco/Exception.h"
#include "RasterIO.h"
#include <list>


struct SClimate 
{
	float					Temp;
	float					Precip;
	SClimate() : Temp(0), Precip(0)	{};
};


class FrescoFoundation_API Climate 
{
//Data
private:
	enum EValuesType		{VTCONSTANT, VTSPATIAL, VTEXPLICIT, VTRANDEXPLICIT};
	enum EOffsetsType		{OTNONE, OTCONSTANT, OTFILE, OTRANDOM};										
	enum EClimateType		{TEMP, PRECIP};										
	enum EOffsetType		{STEP, RAMP};	
    struct SClimateTransition
    {
	    int						Year;
	    //Values
	    EValuesType	            ValuesType;
	    float					ConstantTemp;
	    float					ConstantPrecip;
	    std::string				SpatialTempFile;
	    std::string				SpatialPrecipFile;
	    int						RandExplicitMinYear;
	    int						RandExplicitMaxYear;
	    bool					IsRandExplicitReplicated;
	    //Offsets
	    EOffsetsType	        OffsetsType;
	    float					ConstantTempOffset;
	    float					ConstantPrecipOffset;
	    std::string				OffsetsFile;
	    float					RandomOffsetsTempMean;
	    float					RandomOffsetsTempStdDev;
	    float					RandomOffsetsPrecipMean;
	    float					RandomOffsetsPrecipStdDev;
	    bool					IsRandOffsetReplicated;
	    SClimateTransition() : Year(0), 
		    ValuesType(Climate::VTCONSTANT), ConstantTemp(0), ConstantPrecip(0), SpatialTempFile(""), SpatialPrecipFile(""), RandExplicitMinYear(0), RandExplicitMaxYear(0), IsRandExplicitReplicated(false),
		    OffsetsType(Climate::OTCONSTANT), 	ConstantTempOffset(0), ConstantPrecipOffset(0), OffsetsFile(""), RandomOffsetsTempMean(0), RandomOffsetsTempStdDev(0), RandomOffsetsPrecipMean(0), RandomOffsetsPrecipStdDev(0), IsRandOffsetReplicated(false) {};
    };
    struct SOffset		
    {
	    int		Year;
	    float	Amount;
	    SOffset() : Year(0), Amount(0) {};
    };

    
    static std::vector<SClimateTransition>  _transitions;
    SClimateTransition*		_pCurrentTransition;
    int						_yearsOfArchivedHistory;
	bool					_isMonthlyClimate;
	bool					_isExternFlam;
    float**					_pSpatialFlammability;				//The climate flammability values pre calculated outside of this model. If not used, values are calculated internally.
	std::string				_spatialFlamabilityFile;
    float****				_pSpatialTemp;						//The climate values per frame for the number of years we need to remember climate conditions.  The first index (year) is treated as a circular array to avoid allocating new blocks of memory.  Example: _pSpatialTemp[year][month][row][col];   
    float****				_pSpatialPrecip;					//"
	SClimate*				_pOffsets;							//The mean climate for the number of years we need to remember climate conditions.
    int*					_pRandomYears;
	bool					_stepsAndRampsEnabled;
	std::vector<SOffset>	_tempStep;
	std::vector<SOffset>	_precipStep;
	std::vector<SOffset>	_tempRamp;
	std::vector<SOffset>	_precipRamp;
public:
	std::list<int>			precipMonths;
	std::list<int>			tempMonths;
	const bool				isMonthly() const { return _isMonthlyClimate; }
	const bool				usingExternalFlammabiltiyFile() const { return _isExternFlam; }

//Functions
public:
							Climate();
							~Climate();

	void					clear();
	void					setup();
	void					runStart();
	void					runEnd();
	void					repStart();
	void					repEnd();
	void					yearStart();
	void					yearEnd();

	SClimate			    getClimate (const int row, const int col, const int yearBP=0) const;	//Overload the function operator to return the climate for a given row/col/year combination.
	const float				getTemp(const int row, const int col, const int month, const int yearBP=0) const; 
	const float				getPrecip(const int row, const int col, const int month, const int yearBP=0) const; 
	float					getClimateFlammability(int row, int col);
	void					assertTempMonth(const int month) const;
	void					assertPrecipMonth(const int month) const;
private:
    void                    deleteArrays();
	int						getRandExplicitYear();
	void					setOffsetsConstant(float temp, float precip, int firstYear, int lastYear);
	void					setOffsetsFromFile(std::string filePath, int firstYear, int lastYear);
	void					setOffsetsRandom(float tempMean, float tempStdDev, float precipMean, float precipStdDev, bool isReplicated, int firstYear, int lastYear);
	void					setStepsAndRamps();
	void					setStepsAndRampsInTimeRange(int firstYear, int lastYear);
	void					showOffsetSummary();
	std::string				climateValuesTypeToString(EValuesType valueType);
	std::string				climateOffsetsTypeToString(EOffsetsType offsetType);
	//Transitions
    void                    setupTransitions();
	void					clearTransition(SClimateTransition* pTransition);
	void					setCurrentTransition(std::vector<SClimateTransition>::iterator transition);
	void					applyTransitionIfExists(int year);
	//Steps/Ramps
    void                    setupStepsAndRamps();
    void                    setupStepOrRamp(const EClimateType climateType, const EOffsetType offsetType, Json::Value& yearsKey, Json::Value& offsetsKey);
	void					addOffset(EClimateType climateType, EOffsetType offsetType, int year, float amount);
};


inline SClimate Climate::getClimate(const int row, const int col, const int yearBP) const 
{
	//Calculate year to get climate.
	if (yearBP+0 > _yearsOfArchivedHistory)		throw SimpleException(SimpleException::UNKNOWN, "Cannot retrieve climate data older than "+ToS(_yearsOfArchivedHistory)+ " years before present.  The Climate.NumHistory FIF setting might need adjustment.");
	int year = gYear - gFirstYear - yearBP;  

	//Get climate from the spatial circular array and temporal array.
	SClimate climate;
	climate.Temp	= _pSpatialTemp[year % _yearsOfArchivedHistory][0][row][col];
	climate.Precip	= _pSpatialPrecip[year % _yearsOfArchivedHistory][0][row][col];

	if (IsNodata(climate.Temp) || IsNodata(climate.Precip))
	{
		climate.Temp = RasterIO::NODATA_FLOAT;
		climate.Precip = RasterIO::NODATA_FLOAT;
		return climate;
	}

	climate.Temp	+=_pOffsets[year].Temp;
	climate.Precip	+=_pOffsets[year].Precip;
	return climate;
}

inline const float Climate::getTemp(const int row, const int col, const int month, const int yearBP) const 
{
	//Calculate year to get climate.
	if (yearBP+0 > _yearsOfArchivedHistory)		throw SimpleException(SimpleException::UNKNOWN, "Cannot retrieve climate data older than "+ToS(_yearsOfArchivedHistory)+ " years before present.  The Climate.NumHistory FIF setting might need adjustment.");
	int year = gYear - gFirstYear - yearBP;

	float t = _pSpatialTemp[year % _yearsOfArchivedHistory][month][row][col];
	if (IsNodata(t))
		return RasterIO::NODATA_FLOAT;

    //Get climate from the spatial circular array and offset array.
    return t + _pOffsets[year].Temp;
}

inline const float Climate::getPrecip(const int row, const int col, const int month, const int yearBP) const 
{
	//Calculate year to get climate.
	if (yearBP+0 > _yearsOfArchivedHistory)		throw SimpleException(SimpleException::UNKNOWN, "Cannot retrieve climate data older than "+ToS(_yearsOfArchivedHistory)+ " years before present.  The Climate.NumHistory FIF setting might need adjustment.");
	int year = gYear - gFirstYear - yearBP;

	float p	 = _pSpatialPrecip[year % _yearsOfArchivedHistory][month][row][col];
	if (IsNodata(p))
		return RasterIO::NODATA_FLOAT;

	//Get climate from the spatial circular array and offset array.
	return p + _pOffsets[year].Precip;
}

inline void Climate::assertTempMonth(const int month) const
{
	bool found = false;
	std::list<int>::const_iterator m;
	for (m=tempMonths.begin();  m!=tempMonths.end(); m++)
	{
		if (*m == month)
		{
			found = true;
			break;
		}
	}
	if (!found)
		throw SimpleException(SimpleException::UNKNOWN, "Expected month "+ToS(month)+ " to be included in the FIF field TempMonths.");
}

inline void Climate::assertPrecipMonth(const int month) const
{
	bool found = false;
	std::list<int>::const_iterator m;
	for (m=precipMonths.begin();  m!=precipMonths.end(); m++)
	{
		if (*m == month)
		{
			found = true;
			break;
		}
	}
	if (!found)
		throw SimpleException(SimpleException::UNKNOWN, "Expected month "+ToS(month)+ " to be included in the FIF field PrecipMonths.");
}

#endif
