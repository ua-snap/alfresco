#ifndef Fresco_Stat_h
#define Fresco_Stat_h
#include "PreCompiled.h"
#include "Poco/BasicEvent.h"
#include <vector>
#include <string>


struct SStatSetupEventArgs
{
    const std::string   Title;
    const int           Flags;
    const bool          SavingEventCause;
    SStatSetupEventArgs(const std::string title, const int flags, const bool savingEventCause)
        : Title(title), Flags(flags), SavingEventCause(savingEventCause) {}
};

struct SStatAddEventArgs
{
    const std::string   Title;
    const int           Year;
    const int           Rep;
    const double        Data;
    SStatAddEventArgs(const std::string title, const int year, const int rep, const double data)
        : Title(title), Year(year), Rep(rep), Data(data) {}
};

struct SFireSizeStatAddEventArgs
{
    const std::string   Title;
    const int           Year;
    const int           Rep;
    const double        Data;
	const int			Cause;
	const int			LowCount;
	const int			ModerateCount;
	const int			HighLssCount;
	const int			HighHssCount;
    SFireSizeStatAddEventArgs(const std::string title, const int year, const int rep, const double data, const int cause, const int lowCount, const int moderateCount, const int highLssCount, const int highHssCount)
        : Title(title), Year(year), Rep(rep), Data(data), Cause(cause), LowCount(lowCount), ModerateCount(moderateCount), HighLssCount(highLssCount), HighHssCount(highHssCount) {}
};

class FrescoFoundation_API CStat 
{
//Data
public:
    static Poco::BasicEvent<const SStatAddEventArgs> StatAddEvent;					//Event raised when calling CStat::Add()
    static Poco::BasicEvent<const SFireSizeStatAddEventArgs> FireSizeStatAddEvent;  //Event raised when calling CStat::Add() with firesize specific params
    static Poco::BasicEvent<const SStatSetupEventArgs> StatSetupEvent;				//Event raised when calling CStat::setup()
	long					    m_lTally;											//Variable for totaling incremental types
	std::string			        m_sTitle;											//Title of the stat contents.
private:
	int						    m_nOutFlags;										//A flag indicating which data to output then the object is written

//Functions
public:
							CStat();
							~CStat();
	void					setup(std::string sTitle, const int nFlags, const bool bSaveEventCause);	//Initializes everything to zero by default
	void					clear();												//Resets the stat to values at creation.
	void					Add(const int nYear, const int nRep);					//Add the Tally to the year and replicate
	void					Add(const int nYear, const int nRep, const double dData);	//Adds a given statistic to the year and replicate
	void					Add(const int nYear, const int nRep, const double dData, const int nCause, const int low, const int mod, const int highLSS, const int highHSS);	//Adds a given statistic to the year and replicate
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

};
#endif //Fresco_Stat_h end.

