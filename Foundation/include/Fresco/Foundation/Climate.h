#ifndef Fresco_Climate_h
#define Fresco_Climate_h


#include "PreCompiled.h"
#include "Except.h"
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
	std::list<int>			_precipMonths;
	std::list<int>			_tempMonths;
    float**					_pSpatialFlammability;				//The climate flammability values pre calculated outside of this model. If not used, values are calculated internally.
	std::string				_SpatialFlamabilityFile;
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
    void                    setupStepOrRamp(const EClimateType climateType, const EOffsetType offsetType, const char* yearsKey, const char* offsetsKey);
	void					addOffset(EClimateType climateType, EOffsetType offsetType, int year, float amount);
};


inline SClimate Climate::getClimate(const int row, const int col, const int yearBP) const 
{
	//Calculate year to get climate.
	if (yearBP+0 > _yearsOfArchivedHistory)		throw Exception(Exception::UNKNOWN, "Cannot retrieve climate data older than "+ToS(_yearsOfArchivedHistory)+ " years before present.  The Climate.NumHistory FIF setting might need adjustment.");
	int year = gYear - yearBP;  
	
    //Get climate from the spatial circular array and temporal array.
	SClimate climate;
    climate.Temp	= _pSpatialTemp[year % _yearsOfArchivedHistory][0][row][col]      + _pOffsets[year].Temp;
	climate.Precip	= _pSpatialPrecip[year % _yearsOfArchivedHistory][0][row][col]    + _pOffsets[year].Precip;
	return climate;
}

inline const float Climate::getTemp(const int row, const int col, const int month, const int yearBP) const 
{
	//Calculate year to get climate.
	if (yearBP+0 > _yearsOfArchivedHistory)		throw Exception(Exception::UNKNOWN, "Cannot retrieve climate data older than "+ToS(_yearsOfArchivedHistory)+ " years before present.  The Climate.NumHistory FIF setting might need adjustment.");
	int year = gYear - yearBP;
	
    //Get climate from the spatial circular array and offset array.
    return _pSpatialTemp[year % _yearsOfArchivedHistory][month][row][col] + _pOffsets[year].Temp;
}

inline const float Climate::getPrecip(const int row, const int col, const int month, const int yearBP) const 
{
	//Calculate year to get climate.
	if (yearBP+0 > _yearsOfArchivedHistory)		throw Exception(Exception::UNKNOWN, "Cannot retrieve climate data older than "+ToS(_yearsOfArchivedHistory)+ " years before present.  The Climate.NumHistory FIF setting might need adjustment.");
	int year = gYear - yearBP;
	
    //Get climate from the spatial circular array and offset array.
    return _pSpatialPrecip[year % _yearsOfArchivedHistory][month][row][col] + _pOffsets[year].Precip;
}


#endif
