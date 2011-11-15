#include "Client.h"
#include "FrescoEngine.h"
#include "Global.h"
#include "Messenger.h"
#include "RemoteNotification.h"
#include "Server.h"
#include "Except.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/HostEntry.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Runnable.h"
#include <iostream>
#include <sstream>
#include <vector>


//Declare static members.
Client::ClientState         Client::_state = AVAILABLE;

Client::			        Client(const std::string& serverIP, const std::string& serverPort) :
	isConnected(false),
	_engine(this),
	_messenger(this,serverIP,serverPort),
	_clientID(-1)
{
    Global::getInstance()->localHostName = Poco::Net::DNS::thisHost().name();
    Global::getInstance()->localIp = Poco::Net::DNS::thisHost().addresses().front();
}

Client::			        ~Client()
{
}

const std::string Client::  getStateString()
{
    std::string s = "";
    switch(_state)
    {
    case UNKNOWN_CLIENT_STATE:
        s = "Unknown";
        break;
    case DISCONNECTED:
        s = "Disconnected";
        break;
    case BUSY:
        s = "Busy";
        break;
    case ERROR_OCCURRED:
    	s = "ErrorOccurred";
    	break;
    case AVAILABLE:
        s = "Available";
        break;
    case CLEARING:
        s = "Clearing";
        break;
    case SETTING_UP:
        s = "SettingUp";
        break;
    case SETUP:
        s = "Setup";
        break;
    case SIMULATING:
        s = "Simulating";
        break;
    case STOPPING:
        s = "Stopping";
        break;
    case STOPPED:
        s = "Stopped";
        break;
    case COMPLETED:
        s = "Completed";
    }
    return s;
}

void Client::               connectToServer()
{
	_messenger.startConnectingToServer();
}

void Client::		        disconnectFromServers()
{
    _messenger.disconnect(true);
}

void Client::		        selectProcedure(RemoteNotification* notification)
{
    bool hadError = false;
    std::string errorMessage = "";

    try 
    {
        //Using the RPC object passed in by the Messenger, choose which procedure to call.
        switch (notification->inCommand())
        {
        case RemoteNotification::VERSION_ASSERTION:
            if (Global::getInstance()->version != notification->parameters().at(0))
                throw Exception(Exception::INITFAULT, "Version conflict.  Server is version " + notification->parameters().at(0) + ", but client is version " + Global::getInstance()->version + ".");
            break;
        case RemoteNotification::SET_CLIENT_ID:
            _clientID = notification->clientID();
            break;
        case RemoteNotification::REP_ASSIGNMENT:
            _engine.repStart(atoi(notification->parameters().at(0).c_str()));
            _engine.repStop(atoi(notification->parameters().at(1).c_str()));
            break;
        case RemoteNotification::CLIENT_STATE_REQUEST:
            sendStateToServer();
            break;
        case RemoteNotification::CLIENT_NAME_REQUEST:
            sendComputerNameToServer();
            break;
        case RemoteNotification::SERVER_STATE_REPORT:
            {
                Server::ServerState serverState = (Server::ServerState)atoi(notification->parameters().at(0).c_str());
                _engine.onServerStateUpdated(serverState);
            }
            break;
        case RemoteNotification::SERVER_DISCONNECTING_REPORT:
            _messenger.disconnect(true);
            break;
        case RemoteNotification::FIF_PATH_REPORT:
            _fifName = notification->parameters().at(0);
            _baseDirectory = notification->parameters().at(1);
            _outputDirecotry = notification->parameters().at(2);
            _randomSeed = atoi(notification->parameters().at(3).c_str());
            break;
        default:
            throw Poco::Exception("SelectProcedure Failed: invalid command.");
        }
    } catch (Poco::Exception& e) {  hadError=true;  errorMessage = "Error: " + e.displayText();
    } catch (Exception& e) {        hadError=true;  errorMessage = "Error: " + e.message;
    } catch(...) {                  hadError=true;  errorMessage = "Error: Could not complete remote command: " + notification->commandString()+"."; }
    
    if (hadError) {
        std::cout << std::endl << errorMessage << std::endl;
        sendClientLogEntry(errorMessage, true);
        setState(ERROR_OCCURRED);
    }
}

void Client::               sendStateToServer()
//Called automatically when client's state changes but can be called anytime.
{
    std::vector<std::string> params;
    params.push_back(Global::toString((int)_state));
    RemoteNotification* rn = new RemoteNotification(_clientID,RemoteNotification::CLIENT_STATE_REPORT,params);
    _messenger.sendDataToServer(rn);
    delete rn;
    rn = 0;
}

void Client::               sendComputerNameToServer()
{
    std::vector<std::string> params;
    params.push_back(Global::getInstance()->localHostName);
    RemoteNotification* rn = new RemoteNotification(_clientID,RemoteNotification::COMPUTER_NAME_REPORT,params);
    _messenger.sendDataToServer(rn);
    delete rn;
}

void Client::               sendRepToServer()
{
    std::vector<std::string> params;
    params.push_back(Global::toString(_engine.rep()));
    RemoteNotification* rn = new RemoteNotification(_clientID,RemoteNotification::REP_REPORT,params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}

void Client::               sendYearToServer()
{
    std::vector<std::string> params;
    params.push_back(Global::toString(_engine.year()));
    RemoteNotification* rn = new RemoteNotification(_clientID,RemoteNotification::YEAR_REPORT,params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}

void Client::               sendYearCompletedToServer()
{
    std::vector<std::string> params;
    RemoteNotification* rn = new RemoteNotification(_clientID, RemoteNotification::YEAR_COMPLETED, params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}

void Client::               sendRepCompletedToServer()
{
    std::vector<std::string> params;
    RemoteNotification* rn = new RemoteNotification(_clientID, RemoteNotification::REP_COMPLETED, params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}

void Client::               sendClientLogEntry(const std::string& logEntry, const bool isError)
{
    std::vector<std::string> params;
    params.push_back(logEntry);
    params.push_back(ToS(isError));
    RemoteNotification* rn = new RemoteNotification(_clientID, RemoteNotification::CLIENT_LOG_ENTRY, params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}

void Client::               sendStatSetup(const SStatSetupEventArgs& setupArgs)
{
    std::vector<std::string> params;
    params.push_back(setupArgs.Title);
    params.push_back(ToS(setupArgs.Flags));
    params.push_back(ToS(setupArgs.SavingEventCause));
    RemoteNotification* rn = new RemoteNotification(_clientID, RemoteNotification::STAT_SETUP, params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}

void Client::               sendStatAdd(const SStatAddEventArgs& addArgs)
{
    std::vector<std::string> params;
    params.push_back(addArgs.Title);
    params.push_back(ToS(addArgs.Year));
    params.push_back(ToS(addArgs.Rep));
    params.push_back(ToS(addArgs.Data));
    RemoteNotification* rn = new RemoteNotification(_clientID, RemoteNotification::STAT_ADD, params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}

void Client::               sendFireSizeStatAdd(const SFireSizeStatAddEventArgs& addArgs)
{
    std::vector<std::string> params;
    params.push_back(addArgs.Title);
    params.push_back(ToS(addArgs.Year));
    params.push_back(ToS(addArgs.Rep));
    params.push_back(ToS(addArgs.Data));
    params.push_back(ToS(addArgs.Cause));
    params.push_back(ToS(addArgs.LowCount));
    params.push_back(ToS(addArgs.ModerateCount));
    params.push_back(ToS(addArgs.HighLssCount));
    params.push_back(ToS(addArgs.HighHssCount));
    RemoteNotification* rn = new RemoteNotification(_clientID, RemoteNotification::FIRESIZE_STAT_ADD, params);

    _messenger.sendDataToServer(rn); 
    
    delete rn;
}
