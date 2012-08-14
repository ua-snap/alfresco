#ifndef Fresco_Fresco_h
#define Fresco_Fresco_h


#include "PreCompiled.h"
#include "Interface.h"
#include "Poco/BasicEvent.h"
#include "Poco/File.h"
#include "Stat.h"
#include <queue>
#include <string>
#include <vector>


using Poco::BasicEvent;


class Landscape;


class FrescoFoundation_API Fresco 
{
//Data
public:
    enum FrescoState
    {	//In order of operation.
        ERROR_OCCURRED,             //Client experienced an error and cannot recover.
        CLEARED,                  	//Ready to process a run.
        CLEARING,                   //Clearing a run.
        SETTING_UP,                 //Getting init data.
        SETUP,                      //Done setting up.
        //SIMULATING and below all imply a simulation has been started.
        SIMULATING,				    //Simulating a run.
        STOPPING,				    //Stopping a simulation.
        STOPPED,				    //Simulation stopped.
        COMPLETED                   //Simulation completed.
    };	
    CInterface&             fif();
    const bool              getIsStopped() const;
    void                    setIsStopped(const bool newVal);
    const int               getLastYear() const;
    const FrescoState		getState() const { return _state; }
    void					setState(const FrescoState newVal) { _state = newVal; }
    const bool              isDebugOn() const;
    const bool              isRunningFirstRep() const;    
	const int               numGisHeaderRows() const;
    //Events: for subscribers to receive event notifications.  Very much like C# events.
    BasicEvent<const std::string>   raiseOutput;
	BasicEvent<const std::string>   raiseErrorOutput;
	BasicEvent<const std::string>   raiseAfterStopped;
	BasicEvent<const std::string>   raiseBeforeSetup;
	BasicEvent<const std::string>   raiseAfterSetup;
	BasicEvent<const std::string>   raiseBeforeRunStart;
	BasicEvent<const std::string>   raiseAfterRunStart;
	BasicEvent<const std::string>   raiseBeforeRunEnd;
	BasicEvent<const std::string>   raiseAfterRunEnd;
	BasicEvent<const int>           raiseBeforeRepStart;
	BasicEvent<const int>           raiseAfterRepStart;
	BasicEvent<const int>           raiseBeforeRepEnd;
	BasicEvent<const int>           raiseAfterRepEnd;
	BasicEvent<const int>           raiseBeforeYearStart;
	BasicEvent<const int>           raiseAfterYearStart;
	BasicEvent<const int>           raiseBeforeYearEnd;
	BasicEvent<const int>           raiseAfterYearEnd;
private:
    CInterface              _fif;
    Landscape*				_landscape;
    FrescoState				_state;
    bool                    _isDebugOn;
    long					_randomSeed;
    bool                    _isRunningFirstRep;
    bool                    _stopped;
    bool					_isFireEnabled;

//Functions
public:
							Fresco(Landscape* pLandscape, bool isDebugOn);
	virtual 				~Fresco();
	
	static const std::string version() { return "1.0.11"; };

	//Simulation
	virtual void			clear();
    void					setup(std::string basePath, std::string fifName, std::string outputDirectory, long randomSeed);
    virtual void			customSetup() = 0;
    virtual void            runRep(const int newRep);
    virtual void            runRep(const int rep, const int yearResume);
    virtual void            runOneYear(const int rep, const int yearResume);
    virtual void            runEnd();
	//Output
    virtual void			output(const std::string output);
	virtual void			outputError(std::string message);
    //Setup: Move these two to Fire?
    double*                 getSpruceFireParms(const std::string key);              
    const double*           getStartAgeParms(const std::string key, EStartAgeType* type);
};


inline CInterface&          Fresco::fif()
{
    return _fif;
}

inline const bool           Fresco::getIsStopped() const
{
    return _stopped;
}
inline void                 Fresco::setIsStopped(const bool newVal)
{
    _stopped = newVal;
}

inline const int            Fresco::getLastYear() const
{
    return gLastYear;
}

inline const bool           Fresco::isRunningFirstRep() const 
{
    return _isRunningFirstRep;
}

inline const bool           Fresco::isDebugOn() const
{
    return _isDebugOn;
}

#endif
