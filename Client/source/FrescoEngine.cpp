

#include "Fresco/Client/FrescoEngine.h"
#include "Fresco/Client/Client.h"
#include "Fresco/Custom/CustomFresco.h"
#include "Fresco/Client/Global.h"
#include "Fresco/Client/Server.h"
#include "Fresco/Foundation/Fresco.h"
//#include "Fresco/Foundation/Interface.h"
#include "Fresco/Foundation/Stat.h"
#include "Poco/Delegate.h"
#include <iostream>


//Declare static members.
Poco::Thread* FrescoEngine::_thread = 0;


FrescoEngine::FrescoEngine(Client* owner) :
    _ownerClient(owner),
    _serverState(Server::S_UNKNOWN),
    _simulation(0),
    _rep(-1),
    _year(-1),
    _repStart(0),
    _repStop(0)
{
    CStat::StatSetupEvent += Poco::Delegate<FrescoEngine, const SStatSetupEventArgs>  (this, &FrescoEngine::_onStatSetup);
    CStat::StatAddEvent += Poco::Delegate<FrescoEngine, const SStatAddEventArgs>  (this, &FrescoEngine::_onStatAdd);
}

FrescoEngine::~FrescoEngine()
{
}

void FrescoEngine::clear()
{
    bool hadError = false;
    std::string errorMessage = "";

    try {
        _ownerClient->setState(Client::CLEARING);
        //Do clear work...
        _ownerClient->clearRandomSeed();
        if (0 != _simulation) 
        {
            _simulation->clear();
            delete _simulation; _simulation = 0;
        }
        _rep = -1;
        _year = -1;
        //...clear work done.
        _ownerClient->sendRepToServer();
        _ownerClient->sendYearToServer();
        _ownerClient->setState(Client::AVAILABLE);
    } catch (Poco::Exception& e) {  hadError=true;  errorMessage = "Clearing Error: " + e.displayText();
    } catch (Exception& e) {        hadError=true;  errorMessage = "Clearing Error: " + e.message;
    } catch(...) {                  hadError=true;  errorMessage = "Clearing Error."; }
    
    if (hadError) {
        std::cout << std::endl << errorMessage << std::endl;
        _onClientErrorLogEntry(this,errorMessage);
        _ownerClient->setState(Client::ERROR_OCCURRED);
    }
}

void FrescoEngine::setup(const std::string basePath, const std::string fifFile, const std::string outputDirectory, const long randomSeed)
{
    bool hadError = false;
    std::string errorMessage = "";
        
    try {    
        if (0 == _simulation) {
            _ownerClient->setState(Client::SETTING_UP);
            //Do setup work...
            _simulation = new CustomFresco(Global::getInstance()->isDebugOn);
            //Subscribe to events.
            _simulation->raiseOutput            += Poco::Delegate<FrescoEngine, const std::string>  (this, &FrescoEngine::_onClientLogEntry);
            _simulation->raiseErrorOutput       += Poco::Delegate<FrescoEngine, const std::string>  (this, &FrescoEngine::_onClientErrorLogEntry);
            _simulation->raiseBeforeRepStart    += Poco::Delegate<FrescoEngine, const int>          (this, &FrescoEngine::_onBeforeRepStart);
            _simulation->raiseBeforeYearStart   += Poco::Delegate<FrescoEngine, const int>          (this, &FrescoEngine::_onBeforeYearStart);
            _simulation->raiseAfterYearEnd      += Poco::Delegate<FrescoEngine, const int>          (this, &FrescoEngine::_onAfterYearEnd);
            _simulation->raiseBeforeRepEnd      += Poco::Delegate<FrescoEngine, const int>          (this, &FrescoEngine::_onBeforeRepEnd);
            _simulation->raiseAfterRepEnd       += Poco::Delegate<FrescoEngine, const int>          (this, &FrescoEngine::_onAfterRepEnd);
            _simulation->raiseBeforeRunEnd      += Poco::Delegate<FrescoEngine, const std::string>  (this, &FrescoEngine::_onBeforeRunEnd);
            _simulation->raiseAfterRunEnd       += Poco::Delegate<FrescoEngine, const std::string>  (this, &FrescoEngine::_onAfterRunEnd);
            
            _simulation->setup(basePath, fifFile, outputDirectory, randomSeed);
            _rep = -1;
            _year = -1;
            //...setup work done.
            _ownerClient->setState(Client::SETUP);
        }
        else
            throw Exception(Exception::UNKNOWN,"System did not properly dispose of the previous simulation.");
    } catch (Poco::Exception& e) {  hadError=true;  errorMessage = "Initialization Error: " + e.displayText();
    } catch (Exception& e) {        hadError=true;  errorMessage = "Initialization Error: " + e.message;
    } catch(...) {                  hadError=true;  errorMessage = "Initialization Error."; }
    
    if (hadError) {
        std::cout << std::endl << errorMessage << std::endl;
        _onClientErrorLogEntry(this,errorMessage);
        _ownerClient->setState(Client::ERROR_OCCURRED);
    }
}

void FrescoEngine::start()
{
    if (0 != _thread) {
        //Dispose of existing simulation thread.
        _thread->join();
        delete _thread; _thread = 0;
    }
    _rep = 0;
    _year = 0;
    if (0 == _thread) {
        //Start simulation thread.
        _thread = new Poco::Thread("SimulationThread");
        _thread->start(*this);  //Calls this->run() in new thread.
    }
}

void FrescoEngine::run()
{
    bool hadError = false;
    std::string errorMessage = "";
    
    try {
        _ownerClient->setState(Client::SIMULATING);
        _runReps();
    } catch (Poco::Exception& e) {  hadError=true;  errorMessage = "Simulation Error during rep " + ToS(_rep) + ": " + e.displayText();
    } catch (Exception& e) {        hadError=true;  errorMessage = "Simulation Error during rep " + ToS(_rep) + ": " + e.message;
    } catch(...) {                  hadError=true;  errorMessage = "Simulation Error during rep " + ToS(_rep) + "."; }
    
    if (hadError) {
        std::cout << std::endl << errorMessage << std::endl;
        _onClientErrorLogEntry(this,errorMessage);
        _ownerClient->setState(Client::ERROR_OCCURRED);
    }
}

void FrescoEngine::_runReps()
{
    _runReps(_rep,_year);
    //At this point: Either completed, stopped, or error.
    //If stopped, set this simulation thread to sleep until the client state changes.
    while (Client::STOPPED == _ownerClient->state()) {
        Poco::Thread::sleep(500); // 0.5sec
    }
    //No longer stopped, check if the simulation should resume.
    if (Client::SIMULATING == _ownerClient->state()) {
        _runReps(); //Resume by re-entering this function.
    }
}
void FrescoEngine::_runReps(const int startRep, int startYear)
{
    if (_rep == 0) _rep = _repStart;
    //else, resuming where stopped.
    for (; _rep<_repStop; _rep++)
    {
        _simulation->runRep(_rep, startYear);
        startYear=0;
        if (Client::STOPPED == _ownerClient->state()) {
            break;
        }
    }
    if (Client::STOPPED != _ownerClient->state()) {
        end();
    }
}

void FrescoEngine::_resume()
{
    if (_thread->isRunning()) 
    {
        if (_rep<_repStop-1 && _year<=_simulation->getMaxYear()) {
            //Was stopped in middle of rep, continue on next year.
            _year += _simulation->getTimeStep(); 
        }
        else if (_rep<_repStop-1) {
            //Was stopped after completing a rep. Start next rep.
            _rep++; 
            _year=0; 
        }
        else Exception(Exception::UNKNOWN, "Error while resuming simulation. Unexpected rep and/or year values.");

        //Wake up the SimulationThread in _runReps() by changing client state.  
        //Note: can't just call _runReps() here.  Must allow the SimulationThread to pickup where it left off.
        _simulation->setIsStopped(false);
        _ownerClient->setState(Client::SIMULATING);
    }
    else throw Exception(Exception::UNKNOWN, "Simulation Thread is missing.  Cannot continue.  Restart client.");
}

void FrescoEngine::end()
{
    _simulation->runEnd();
    _ownerClient->setState(Client::COMPLETED);
}

void FrescoEngine::onServerStateUpdated(const Server::ServerState newState)
{
    _serverState = newState;
    switch (newState) {
    case Server::S_SIMULATING:
        if (Client::AVAILABLE == _ownerClient->state()) {
            setup(_ownerClient->baseDirectory(), _ownerClient->fifName(), _ownerClient->outputDirectory(), _ownerClient->randomSeed());
            if (Client::SETUP == _ownerClient->state())
                start();
        }
        else if (Client::STOPPED == _ownerClient->state()) {
            _resume();
        }
        break;
    case Server::S_STOPPING:
        if (Client::SIMULATING == _ownerClient->state()) {
            if (! (_rep == _repStop-1 &&_year==_simulation->getMaxYear())) 
                beginStop();
        }
        break;
    case Server::S_CLEARING:
        clear();
        break;
    case Server::S_COMPLETED: 	break;
    case Server::S_LOADED: 		break;
    case Server::S_NOT_LOADED:	break;
    case Server::S_SAVING:		break;
    case Server::S_STOPPED:		break;
    case Server::S_UNKNOWN:		break;
    }
}

void FrescoEngine::beginStop()
{
    _ownerClient->setState(Client::STOPPING);
    //After year or rep events finish the stop.
}

void FrescoEngine::_finishStop(const int rep)
{
    _ownerClient->setState(Client::STOPPED);
    _simulation->setIsStopped(true);
    if (0 > rep)
        _onClientLogEntry(this, "Stopped before starting any reps.");
    else
        _onClientLogEntry(this, "Stopped after completing rep " + ToS(rep) + ".");
}

void FrescoEngine::_finishStop(const int rep, const int year)
{
    _ownerClient->setState(Client::STOPPED);
    _simulation->setIsStopped(true);
    _onClientLogEntry(this, "Stopped after completing rep "+ToS(rep)+", year "+ToS(year)+".");
}

void FrescoEngine::_onClientLogEntry(const void* sender, const std::string& logEntry)
{
    _ownerClient->sendClientLogEntry(logEntry, false);
}

void FrescoEngine::_onClientErrorLogEntry(const void* sender, const std::string& errorMessage)
{
    _ownerClient->sendClientLogEntry(errorMessage, true);
}

void FrescoEngine::_onBeforeRepStart(const void* sender, const int& rep)
{
    _rep = rep;
    _year = -1;
    _onClientLogEntry(this, "Rep " + ToS(rep) + "\n");
    _ownerClient->setState(Client::SIMULATING);
    _ownerClient->sendRepToServer();
}

void FrescoEngine::_onBeforeYearStart(const void* sender, const int& year)
{
    _year = year;
    ((Fresco*)sender)->setIsStopped(false);
    _ownerClient->setState(Client::SIMULATING);
    //Remote output.
    _onClientLogEntry(this, "\tYear " + ToS(year) + "\n");
    _ownerClient->sendYearToServer();
}

void FrescoEngine::_onAfterYearEnd(const void* sender, const int& year)
{
    _year = year;
    if (Client::STOPPING == _ownerClient->state()) {
        _finishStop(_rep, _year);
    }
    _ownerClient->sendYearToServer();
    _ownerClient->sendYearCompletedToServer();
}

void FrescoEngine::_onBeforeRepEnd(const void* sender, const int& rep)
{
    _year = -1; //Just to clear the year field...
    _ownerClient->sendYearToServer();
}

void FrescoEngine::_onAfterRepEnd(const void* sender, const int& rep)
{
    if (Client::STOPPING == _ownerClient->state()) {
        _finishStop(rep);
    }
    _ownerClient->sendRepToServer();
    _ownerClient->sendRepCompletedToServer();
}

void FrescoEngine::_onBeforeRunEnd(const void* sender, const std::string& dummy)
{
}

void FrescoEngine::_onAfterRunEnd(const void* sender, const std::string& dummy)
{
    _onClientLogEntry(this, "Simulation Complete.\n");
}

void FrescoEngine::_onStatSetup(const void* sender, const SStatSetupEventArgs& setupArgs)
{
    _ownerClient->sendStatSetup(setupArgs);
}

void FrescoEngine::_onStatAdd(const void* sender, const SStatAddEventArgs& addArgs)
{
    _ownerClient->sendStatAdd(addArgs);
}
