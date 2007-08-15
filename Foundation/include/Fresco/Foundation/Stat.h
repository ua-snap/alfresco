#ifndef Fresco_Stat_h
#define Fresco_Stat_h
#include "PreCompiled.h"
#include "Poco/BasicEvent.h"
#include <vector>
#include <string>


#ifndef Fresco_Is_Client //Not needed for client.
struct SMean 
{
	float Mean;
	float StandardDeviation;
	SMean() : Mean(0), StandardDeviation(0)	{};
};
struct SEvent   //Stat
{
	int		Year;
	int		Rep;
	double	Value;
	int		Cause;
	SEvent() : Year(0), Rep(0), Value(0), Cause(0) {};
};

class CBasicStat 
//The statistical gathering functions are housed in two classes.  The first (CBasicStat) 
//is the basic statistical routines which store enough data to provide basic stats on
//whatever data is stored in the object.  The second (CStat) provides a framework by which
//it is possible to automatically store data across replicates and time steps automatically.
//This is done by maintaining arrays of objects of CBasicStat and updating the relevent
//objects as necessary.
{
public:
							CBasicStat();
							~CBasicStat();
	void				    clear();
	long					Add(const double dValue);								//Add a value to the stats, returns the number of entries.
	long					Num()		{return m_lNum;}							//Return the number of entries
	double					Mean();			                                        //Return the mean
	double					StdDev();		                                        //Return the standard deviation
	double					Min()		{return m_dMin;}							//Return the minimum
	double					Max()		{return m_dMax;}							//Return the maximum
	double					Sum()		{return m_dSum;}							//Return the sum.
private:
	long					m_lNum;													//Number of entries so far
	double					m_dSum;													//Sum for computing the mean
	double					m_dSumSqr;												//Sum of squares for computing standard deviation
	double					m_dMin, m_dMax;											//Current min and max value
};		

typedef std::vector<double> VECTORVALUE;
typedef std::vector<VECTORVALUE> VECTORDATA;
#endif


struct SStatSetupEventArgs
{
    const std::string   Title;
    const int           MaxYears;
    const int           MaxReps;
    const int           TimeStep;
    const int           Flags;
    const bool          SavingEventCause;
    SStatSetupEventArgs(const std::string title, const int maxYear, const int maxRep, const int timeStep, const int flags, const bool savingEventCause)
        : Title(title), MaxYears(maxYear), MaxReps(maxRep), TimeStep(timeStep), Flags(flags), SavingEventCause(savingEventCause) {}
};

struct SStatAddEventArgs
{
    const std::string   Title;
    const int           Year;
    const int           Rep;
    const double        Data;
    const int           Cause;
    SStatAddEventArgs(const std::string title, const int year, const int rep, const double data, const int cause)
        : Title(title), Year(year), Rep(rep), Data(data), Cause(cause) {}
};

class FrescoFoundation_API CStat 
{
//Data
public:
    static Poco::BasicEvent<const SStatAddEventArgs>     StatAddEvent;              //Event raised when calling CStat::Add()
    static Poco::BasicEvent<const SStatSetupEventArgs>   StatSetupEvent;            //Event raised when calling CStat::setup()
	long					    m_lTally;											//Variable for totaling incremental types
	std::string			        m_sTitle;											//Title of the stat contents.
private:
	int						    m_nOutFlags;										//A flag indicating which data to output then the object is written
#ifndef Fresco_Is_Client
	int						    m_nMaxYears;										//Maintain a local copy of the number of replicates and time steps and stepsize
	int						    m_nMaxReps;											//Maintain a local copy of the number of replicates and time steps and stepsize
	int						    m_nTimeStep;										//Maintain a local copy of the number of replicates and time steps and stepsize
	bool					    m_bSaveEventCause;									//Determines whether the SEvent.Cause will be saved for this Stat. 
	VECTORDATA				    m_vData; 
	std::vector<CBasicStat>		m_vBasicStatYear;									//Maintain the statistics across replicates
	std::vector<CBasicStat>		m_vBasicStatRep;									//Maintain the statistics within a replicate
    std::vector< std::vector<SEvent> > m_vEvents;									//A vector of SEvent vectors is needed to prevent loss of stats in case of reaching the SEvent vector's max_size.
#endif

//Functions
public:
							CStat();
							~CStat();
	void					setup(std::string sTitle, const int nYears, const int nReps, const int nTimeStep, const int nFlags, const bool bSaveEventCause);	//Initializes everything to zero by default
	void					clear();												//Resets the stat to values at creation.
	void					Add(const int nYear, const int nRep);					//Add the Tally to the year and replicate
	void					Add(const int nYear, const int nRep, const double dData);	//Adds a given statistic to the year and replicate
	void					Add(const int nYear, const int nRep, const double dData, const int nCause);	//Adds a given statistic to the year and replicate
	long					operator++ (int)	{return ++m_lTally;}				//Increment the Tally by one and return Tally
	long					operator-- (int)	{return --m_lTally;}				//Decrement the Tally by one and return Tally
	enum 
	{
		outYear		= 0x1,															//Output the data averaged by year
		outRep		= 0x2,															//Output the data averaged by replicate
		outFormat	= 0x4,															//Format in a way to facilitate reading into a stats package for analysis
		outNum		= 0x10,															//Output the number of samples
		outMean		= 0x20,															//Output the mean of the data
		outStdDev	= 0x40,															//Output the standard deviation of the samples
		outMin		= 0x80,															//Output the minimum of the samples
		outMax		= 0x100,														//Output the maximum of the samples
		outHist		= 0x200,														//Output the histogram data if there is any
		outData		= 0x400,														//Output the file of data.
		outEvents	= 0x800															//Output the file of events.
	};
#ifndef Fresco_Is_Client
    double					SumOfYearRep(int nYear, int nRep);
	double					SumAcrossYears();
	double					SumAcrossReps();
    void					Save(std::ostream &sStream);
private:
	void					ClearEvents();											
	void					AddEvent(int nYear, int nRep, double dData, int nCause);
#endif 

};
#endif //Fresco_Stat_h end.

