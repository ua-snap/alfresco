//CStat.cpp
//This module contains the code pertaining to the statistical gathering class.  This module has two class
//which are described in CStat.h.  This file contains constructors and destructors for these both classes,
//as well as some of the more detailed functions including output operator overloading.
#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Stat.h"
#include "Fresco/Foundation/Except.h"
#include "Fresco/Foundation/Fresco.h"

//Declare static variables.
Poco::BasicEvent<const SStatSetupEventArgs>   CStat::StatSetupEvent;
Poco::BasicEvent<const SStatAddEventArgs>     CStat::StatAddEvent;

#ifndef Fresco_Is_Client //BasicStat not needed in client.
CBasicStat::			CBasicStat() 
/**********************************************************************************
**********************************************************************************/
{
	// Constructor initializes everything to zero
	m_lNum		= 0;
	m_dSum		= 0;
	m_dSumSqr	= 0;
	m_dMin		= 0;
	m_dMax		= 0;
}


CBasicStat::			~CBasicStat() 
/**********************************************************************************
**********************************************************************************/
{
}


void CBasicStat::		clear()
//Clear existing run if any and return to before a run is specified.
{
	m_lNum		= 0;
	m_dSum		= 0;
	m_dSumSqr	= 0;
	m_dMin		= 0;
	m_dMax		= 0;
}
long CBasicStat::		Add(const double dValue) 
/**********************************************************************************
Add a statistic to the data storage.  Returns the number of entries.
**********************************************************************************/
{
	// Add a statistic to the data storage.  Returns the number of entries.
	m_dSum += dValue;
	m_dSumSqr += (dValue * dValue);
	if (m_lNum != 0) {
		m_dMin = (dValue < m_dMin) ? dValue : m_dMin;
		m_dMax = (dValue > m_dMax) ? dValue : m_dMax;
	} else {
		m_dMin = dValue;
		m_dMax = dValue;
	}
	return ++m_lNum;
}


double CBasicStat::		Mean()
/**********************************************************************************
**********************************************************************************/
{
	double Result = 0;
	if (m_lNum==0)	Result = 0;
	else			Result = m_dSum / (double)m_lNum;
	return Result;
}


double CBasicStat::		StdDev()
/**********************************************************************************
Return the standard deviation
**********************************************************************************/
{
	double Result = 0;
	if (m_lNum-1<=0)	Result = 0;
	else				Result = sqrt((m_dSumSqr-(m_dSum*m_dSum/(double)m_lNum)) / (double)(m_lNum-1)); 
	return Result;
}
#endif

CStat::					CStat () 
{
	//setup the default values
	m_sTitle		= "";
	m_lTally		= 0;
	m_nOutFlags		= 0;
    #ifndef Fresco_Is_Client
	m_nMaxYears		= 0;
	m_nMaxReps		= 0;
	m_nTimeStep		= 0;
	m_vEvents.resize(1);
	m_bSaveEventCause = false;
    #endif
}

CStat::					~CStat () 
//Destructor needs to destroy the two manually allocated arrays and the histogram array if it exists.
{
    #ifndef Fresco_Is_Client
	m_vBasicStatYear.clear();
	m_vBasicStatRep.clear();
	for (int y=0; y<(int)m_vData.size(); y++) 	
		m_vData[y].clear();
	m_vData.clear();
	ClearEvents();
    #endif
}


void CStat::			clear()
{
	m_lTally	= 0;
    #ifndef Fresco_Is_Client
	ClearEvents(); 
	m_vEvents.resize(1);
    #endif
}



void CStat::			setup(std::string sTitle, const int nYears, const int nReps, const int nTimeStep, const int nFlags, const bool bSaveEventCause) 
//setup run.
{
#ifdef Fresco_Is_Client
	m_sTitle		= sTitle;
	m_lTally		= 0;
	m_nOutFlags		= nFlags;
    SStatSetupEventArgs stat(sTitle,nYears,nReps,nTimeStep,nFlags,bSaveEventCause);
    StatSetupEvent.notify(this,  stat);
#else
	//setup the default values.
	m_sTitle		= sTitle;
	m_nMaxYears		= nYears;
	m_nMaxReps		= nReps;
	m_nTimeStep		= nTimeStep;
	m_lTally		= 0;
	m_bSaveEventCause = bSaveEventCause;
	//Allocate memory.
	m_vBasicStatYear.resize(m_nMaxYears/m_nTimeStep+1);					//Include the last year in the sequence
	m_vBasicStatRep.resize(m_nMaxReps);
	m_vData.resize(m_nMaxYears/m_nTimeStep+1);
	for (int y=0; y<=m_nMaxYears/m_nTimeStep; y++) {
		m_vData[y].resize(m_nMaxReps);
	}
#endif
}


void CStat::			Add(int nYear, int nRep) 
//This version of Add incorporates the m_nTally variable into the current dataset
//by calling the expanded version of Add.
{
	Add (nYear, nRep, m_lTally);
}

void CStat::			Add(int nYear, int nRep, double dData) 
//This version of Add incorporates a new piece of data (dData) into the current dataset
//as specified by nYear and nRep.  If an invalid Year/Rep is specified, an error is thrown.
{
	Add(nYear, nRep, dData, 0);
}

void CStat::			Add(int nYear, int nRep, double dData, int nCause) 
//This version of Add incorporates a the cause of an event into the current dataset
//as specified by nYear and nRep.  If an invalid Year/Rep is specified, an error is thrown.
{
#ifdef Fresco_Is_Client
	if (m_nOutFlags > 0)
	    StatAddEvent.notify(this, SStatAddEventArgs(m_sTitle,nYear,nRep,dData,nCause) );
#else
	int nPos = 0;
	//Error if year or rep is out of bounds.
	if (nYear>m_nMaxYears || nYear<0)	throw Exception(Exception::BADYEAR,"Invalid year specified to CStat::Add.\n");
	if (nRep>=m_nMaxReps || nRep<0)		throw Exception(Exception::BADREP,"Invalid replicate specified to CStat::Add.\n");
	//Add data to the by year statistic.
	m_vBasicStatYear[nYear/m_nTimeStep].Add(dData);
	//Add data to the by rep statistic.
	m_vBasicStatRep[nRep].Add(dData);
	//Add data to the summation for year and rep.
	m_vData[nYear/m_nTimeStep][nRep] += dData;
	//Add data to the event list. 
	if (m_nOutFlags & outEvents)
		AddEvent(nYear, nRep, dData, nCause);
#endif
}

#ifndef Fresco_Is_Client
void CStat::			AddEvent(int nYear, int nRep, double dData, int nCause)
{
	if (m_vEvents.empty())	throw Exception(Exception::UNKNOWN, "Events vector is empty.");
	//Adjust data structure's size if needed.
	if (m_vEvents.back().size() >= (int)m_vEvents.back().capacity()) {
		try {
			//Allocate space in blocks of 1000 for efficiency.
			m_vEvents.back().reserve(m_vEvents.back().size()+1000);
		} 
		catch(std::bad_alloc const&) {
			//Current events collection is full, create new one and start using it.
			m_vEvents.resize(m_vEvents.size()+1);
			m_vEvents.back().reserve(1000);
		}
	}
	//Add the event.
	SEvent e;
	e.Year = nYear;
	e.Rep = nRep;
	e.Value = dData;
	e.Cause = nCause;
	m_vEvents.back().push_back(e);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// THIS CODE IS FOR TESTING VECTOR BOUNDS
	//static bool flag = false;
	//if (m_vEvents.size() != 1)
	//	flag = true;
	//if (flag)
	//	return;
	//ShowOutput(MINIMAL,"\n\nStat Title=\t"+m_sTitle+"\n\n Allocating 10,850,000 SEvents...");
	//m_vEvents.back().resize(10855000);
	//ShowOutput(MINIMAL," Done.");
	//
	//m_vEvents.back().reserve(m_vEvents.back().size()+1000);
	//int size = (int)m_vEvents.back().size();
	//int incr = 1;
	//while(true) {
	//	size = (int)m_vEvents.back().size();
	//	if (size >= (int)m_vEvents.back().capacity()) {
	//		try {
	//			//Allocate space in blocks of 1000 for efficiency.
	//			FRESCO->output("\n siz=\t"+ToS(size));
	//			m_vEvents.back().reserve(size+1000); 
	//		} 
	//		catch(std::bad_alloc const&) {
	//			//Current events collection is full, create new one and start using it.
	//			FRESCO->output("\n####### New Bucket #######\n bad_alloc at size "+ToS((int)m_vEvents.back().size()));
	//			m_vEvents.resize(m_vEvents.size()+1);	//One new bucket. 	
	//			m_vEvents.back().reserve(1000);			//Allocate bucket space in blocks of 1000 for efficiency.
	//			//Add 500 events.
	//			for (int i=1; i<=500; i++) {
	//				SEvent e;
	//				e.Year = i;
	//				e.Rep = i;
	//				e.Value = i;
	//				e.Cause = i;
	//				m_vEvents.back().push_back(e);
	//			}
	//			return;
	//		}
	//	}
	//	//Add 1000 events.
	//	for (int i=0; i<1000; i++) {
	//		SEvent e;
	//		e.Year = incr;
	//		e.Rep = incr;
	//		e.Value = incr;
	//		e.Cause = incr++;
	//		m_vEvents.back().push_back(e);
	//	}
	//}
	//
	/////////////////////////////////////////////////////////////////////////////////////////////
}

void CStat::			ClearEvents()
{
	std::vector< std::vector<SEvent> >::iterator i;
	for (i=m_vEvents.begin(); i!=m_vEvents.end(); i++)
		i->clear();
	m_vEvents.clear();
}

double CStat::			SumOfYearRep(int nYear, int nRep)
{
	return m_vData[nYear/m_nTimeStep][nRep];
}
double CStat::			SumAcrossYears()
{
	double Return = 0;
	for (unsigned int i=0;i<m_vBasicStatYear.size();i++)
		Return += m_vBasicStatYear[i].Sum();
	return Return;
}
double CStat::			SumAcrossReps()
{
	double Return = 0;
	for (unsigned int i=0;i<m_vBasicStatRep.size();i++)
		Return += m_vBasicStatRep[i].Sum();
	return Return;
}
void CStat::			Save(std::ostream &sStream)
{
	//Output title as header.
	sStream << m_sTitle.c_str() << std::endl;
	//Output within year data
	int bFormat = m_nOutFlags & CStat::outFormat;
	if (m_nOutFlags & outYear) {
		if (bFormat) {
			sStream << "Year:";
			for (int Year = 0; Year <= m_nMaxYears/m_nTimeStep; Year++)
				sStream << "\t" << Year * m_nTimeStep;
			sStream << std::endl;
		}
		if (m_nOutFlags & outNum) {
			if (bFormat) sStream << "Num:";
			for (int Year = 0; Year <= m_nMaxYears/m_nTimeStep; Year++)
				sStream << "\t" << m_vBasicStatYear[Year].Num();
			sStream << std::endl;
		}
		if (m_nOutFlags & outMean) {
			if (bFormat) sStream << "Mean:";
			for (int Year = 0; Year <= m_nMaxYears/m_nTimeStep; Year++)
				sStream << "\t" << m_vBasicStatYear[Year].Mean();
			sStream << std::endl;
		}
		if (m_nOutFlags & outStdDev) {
			if (bFormat) sStream << "Std:";
			for (int Year = 0; Year <= m_nMaxYears/m_nTimeStep; Year++)
				sStream << "\t" << m_vBasicStatYear[Year].StdDev();
			sStream << std::endl;
		}
		if (m_nOutFlags & outMin) {
			if (bFormat) sStream << "Min:";
			for (int Year = 0; Year <= m_nMaxYears/m_nTimeStep; Year++)
				sStream << "\t" << m_vBasicStatYear[Year].Min();
			sStream << std::endl;
		}
		if (m_nOutFlags & outMax) {
			if (bFormat) sStream << "Max:";
			for (int Year = 0; Year <= m_nMaxYears/m_nTimeStep; Year++)
				sStream << "\t" << m_vBasicStatYear[Year].Max();
			sStream << std::endl;
		}
	}

	//Output across year data
	if (m_nOutFlags & outRep) {
		if (!bFormat) {
			sStream << "Rep:";
			for (int Rep = 0; Rep < m_nMaxReps; Rep++)
				sStream << "\t" << Rep;
			sStream << std::endl;
		}
		if (m_nOutFlags & outNum) {
			if (bFormat) sStream << "Num:";
			for (int Rep = 0; Rep < m_nMaxReps; Rep++)
				sStream << "\t" << m_vBasicStatRep[Rep].Num();
			sStream << std::endl;
		}
		if (m_nOutFlags & outMean) {
			if (bFormat) sStream << "Mean:";
			for (int Rep = 0; Rep < m_nMaxReps; Rep++)
				sStream << "\t" << m_vBasicStatRep[Rep].Mean();
			sStream << std::endl;
		}
		if (m_nOutFlags & outStdDev) {
			if (bFormat) sStream << "Std:";
			for (int Rep = 0; Rep < m_nMaxReps; Rep++)
				sStream << "\t" << m_vBasicStatRep[Rep].StdDev();
			sStream << std::endl;
		}
		if (m_nOutFlags & outMin) {
			if (bFormat) sStream << "Min:";
			for (int Rep = 0; Rep < m_nMaxReps; Rep++)
				sStream << "\t" << m_vBasicStatRep[Rep].Min();
			sStream << std::endl;
		}
		if (m_nOutFlags & outMax) {
			if (bFormat) sStream << "Max:";
			for (int Rep = 0; Rep < m_nMaxReps; Rep++)
				sStream << "\t" << m_vBasicStatRep[Rep].Max();
			sStream << std::endl;
		}
	}

	//Output data.
	if (m_nOutFlags & outData) {
		std::fstream fpData((gOutputDirectory + Poco::Path::separator() + m_sTitle + ".txt").c_str(), std::ios::out);
		fpData << "Year";
		for (int r=0; r<m_nMaxReps; r++) fpData << "\t" << "Rep " << r;
		fpData << std::endl;
		for (int y=0; y<=m_nMaxYears/m_nTimeStep; y++) {
			fpData << y << "\t";
			for (int r=0; r<m_nMaxReps; r++) {
				fpData << m_vData[y][r] << "\t";
			}
			fpData << std::endl;
		}
		fpData << std::endl;
	}

	//Output events.
	if (m_nOutFlags & outEvents) {
		std::fstream fpEvents((gOutputDirectory + Poco::Path::separator() + m_sTitle + "Events.txt").c_str(), std::ios::out);
		fpEvents << "Year" << "\t" << "Rep" << "\t" << "Value" << (m_bSaveEventCause?"\tCause":"") << std::endl;

		std::vector< std::vector<SEvent> >::iterator bucket;
		std::vector<SEvent>::iterator evnt;

		for (bucket=m_vEvents.begin(); bucket!=m_vEvents.end(); bucket++) { //Loop over buckets of events.
			for (evnt=bucket->begin(); evnt!=bucket->end(); evnt++) { //Loop over events in bucket.
				fpEvents << evnt->Year	<< "\t";
				fpEvents << evnt->Rep		<< "\t";
				fpEvents << evnt->Value	<< "\t";
				if (m_bSaveEventCause) fpEvents		<< evnt->Cause	<< "\t";
				fpEvents << std::endl;
			}
		}
	}
}

#endif
