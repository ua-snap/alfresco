#ifndef Fresco_FrescoEngine_h
#define Fresco_FrescoEngine_h


#include "Global.h"
#include "Poco/Thread.h"
#include "Server.h"
#include "Fresco/Foundation/Stat.h"


class Client;
class CustomFresco;

class FrescoEngine : public Poco::Runnable
{
//Data
public:
    const int           rep() const { return _rep; }
    const int           year() const { return _year; }
    void                repStart(const int repStart) { _repStart = repStart; }
    void                repStop(const int repStop)   { _repStop = repStop; }
private:
    Client*             _ownerClient;
    Server::ServerState _serverState;
    CustomFresco*       _simulation;
    int                 _rep;
    int                 _year;
    int                 _repStart;
    int                 _repStop;
    static Poco::Thread* _thread;


//Functions
public:
                        FrescoEngine(Client* owner);
    virtual             ~FrescoEngine();
    void                clear();
    void                setup(const std::string basePath, const std::string fifName, const std::string outputDirectory, const long randomSeed);
    void                start();
    void                beginStop();
    void                end();
    void                onServerStateUpdated(const Server::ServerState newState);
private:
    virtual void        run();
    void                _runReps();
    void                _runReps(const int startRep, int startYear);
    void                _finishStop(const int rep);
    void                _finishStop(const int rep, const int year);
    void                _resume();
    void                _onClientLogEntry(const void* sender, const std::string& newName);
    void                _onClientErrorLogEntry(const void* sender, const std::string& errorMessage);
    void                _onBeforeRepStart(const void* sender, const int& rep);
    void                _onBeforeYearStart(const void* sender, const int& year);
    void                _onAfterYearEnd(const void* sender, const int& year);
    void                _onBeforeRepEnd(const void* sender, const int& rep);
    void                _onAfterRepEnd(const void* sender, const int& rep);
    void                _onBeforeRunEnd(const void* sender, const std::string& dummy);
    void                _onAfterRunEnd(const void* sender, const std::string& dummy);
    void                _onStatSetup(const void* sender, const SStatSetupEventArgs& stat);
    void                _onStatAdd(const void* sender, const SStatAddEventArgs& stat);
	void				_onFireSizeStatAdd(const void* sender, const SFireSizeStatAddEventArgs& addArgs);
};


#endif
