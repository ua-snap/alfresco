#ifndef Fresco_Client_h
#define Fresco_Client_h


#include "Poco/Net/SocketAddress.h"
#include "Poco/Path.h"
#include "RemoteNotification.h"
#include "Fresco/Client/Messenger.h"
#include "Fresco/Client/FrescoEngine.h"
#include "Fresco/Foundation/Stat.h"
#include <string>


class Client
{
//Data
public:
    enum ClientState
    {
        UNKNOWN_CLIENT_STATE,       //Not yet known.
        DISCONNECTED,               //Socket connection is disconnected.
        BUSY,                       //Busy running a different server's run.
        ERROR_OCCURRED,             //Client experienced an error and cannot recover.
        AVAILABLE,                  //Ready to process a run.
        CLEARING,                   //Clearing a run.
        SETTING_UP,                 //Getting init data.
        SETUP,                      //Done setting up.
        //SIMULATING and below all imply a simulation has been started.
        SIMULATING,				    //Simulating a run.
        STOPPING,				    //Stopping a simulation.
        STOPPED,				    //Simulation stopped.
        COMPLETED                   //Simulation completed.
    };
    const int                   clientID() const { return _clientID; }
    void                        clientID(const int value) { _clientID = value; }
    const std::string           fifName() const { return _fifName; }
    const std::string           baseDirectory() const { return _baseDirectory; }
    const std::string           outputDirectory() const { return _outputDirecotry; }
    const long                  randomSeed() const { return _randomSeed; }
    static const ClientState    state() { return _state; }
    bool                        isConnected; //TODO accessor / mutator;
    void                        setState(const ClientState newState)
    {
        _state = newState;
        sendStateToServer();
    }
private:
    FrescoEngine	            _engine;
    Messenger                   _messenger;
    int                         _clientID;
    static ClientState          _state;
    std::string                 _fifName;
    std::string                 _baseDirectory;
    std::string                 _outputDirecotry;
    long                        _randomSeed;


//Methods
public:
		                        Client(const std::string& serverIP, const std::string& serverPort);
		                        ~Client();
    void	                    selectProcedure(RemoteNotification* incomingNotification);
    void                        connectToServer();
    void	                    disconnectFromServers();
    void                        clearRandomSeed() { _randomSeed = -1;} 
    const std::string           getStateString();
    void                        sendRepToServer();
    void                        sendYearToServer();
    void                        sendRepCompletedToServer();
    void                        sendYearCompletedToServer();
    void	                    sendStateToServer();
    void                        sendComputerNameToServer();
    void                        sendClientLogEntry(const std::string& logEntry, bool isError);
    void                        sendStatSetup(const SStatSetupEventArgs& setupArgs);
    void                        sendStatAdd(const SStatAddEventArgs& addArgs);
};


#endif
