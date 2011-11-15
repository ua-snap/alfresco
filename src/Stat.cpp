//CStat.cpp
//This class provides the hooks to report stats to the server app.
#include "PreCompiled.h"
#include "Stat.h"
#include "Except.h"
#include "Fresco.h"


//Declare static variables.
Poco::BasicEvent<const SStatSetupEventArgs>			CStat::StatSetupEvent;
Poco::BasicEvent<const SStatAddEventArgs>			CStat::StatAddEvent;
Poco::BasicEvent<const SFireSizeStatAddEventArgs>   CStat::FireSizeStatAddEvent;


CStat::					CStat () 
{
	m_sTitle		= "";
	m_lTally		= 0;
	m_nOutFlags		= 0;
}

CStat::					~CStat () 
{
}

void CStat::			clear()
{
	m_lTally	= 0;
}

void CStat::			setup(std::string sTitle, const int nFlags, const bool bHasFireEventFields) 
{
	m_sTitle		= sTitle;
	m_lTally		= 0;
	m_nOutFlags		= nFlags;
    SStatSetupEventArgs stat(sTitle,nFlags,bHasFireEventFields);
    StatSetupEvent.notify(this, stat);
}

void CStat::			Add(const int nYear, const int nRep) 
//This version of Add incorporates the m_nTally variable into the current dataset
//by calling the expanded version of Add.
{
	Add (nYear, nRep, m_lTally);
}

void CStat::			Add(const int nYear, const int nRep, const double dData) 
//This version of Add incorporates a new piece of data (dData) into the current dataset
//as specified by nYear and nRep.
{
	if (m_nOutFlags > 0)
	    StatAddEvent.notify(this, SStatAddEventArgs(m_sTitle,nYear,nRep,dData) );
}

void CStat::			Add(const int nYear, const int nRep, const double dData, const int nCause, const int low, const int mod, const int highLSS, const int highHSS)
//This version of Add incorporates the cause of an event into the current dataset
//as specified by nYear and nRep.
{
	if (m_nOutFlags > 0)
	    FireSizeStatAddEvent.notify(this, SFireSizeStatAddEventArgs(m_sTitle,nYear,nRep,dData,nCause,low,mod,highLSS,highHSS) );
}
