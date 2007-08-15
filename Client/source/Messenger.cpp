

#include "Fresco/Client/Messenger.h"
#include "Fresco/Client/Client.h"
#include "Fresco/Client/Global.h"
#include "Fresco/Client/RemoteNotification.h"
#include "Poco/ActiveMethod.h"
#include "Poco/Mutex.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Thread.h"
#include "Poco/Timer.h"
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>


using namespace Poco::Net;


//Declare static members.
std::queue<RemoteNotification*>     Messenger::_notifications;
Poco::Mutex                         Messenger::_sendingMutex;


Messenger::         Messenger(Client* owner, const std::string& serverIP, const std::string& serverPort) : 
    _ownerClient(owner), 
    _connectTimer(0), 
    _outTimer(0), 
    _inTimer(0),
    _serverAddress(serverIP,serverPort),
    _connection(0)
{
    _out = 0;
    _in = 0;
}

Messenger::         ~Messenger(void)
{
    _stopConnectingToServer();
    disconnect(false);
    _stopReceivingNotificationsFromServer();
    _stopActingOnNotifications();
    delete _connectTimer; _connectTimer = 0;
    delete _inTimer; _inTimer = 0;
    delete _outTimer; _outTimer = 0;
    Poco::Timer t;
}

// Connecting
void Messenger::    startConnectingToServer()
{
    if (_connectTimer != 0) 
    {
        _connectTimer->stop();
        delete _connectTimer; _connectTimer = 0;
    }
    Poco::TimerCallback<Messenger> callback(*this, &Messenger::_connectToServer);
    _connectTimer = new Poco::Timer(0,100);
    std::cout << "Attempting connection with server at " << _serverAddress.toString() << std::endl;
    _connectTimer->start(callback);
}

void Messenger::    _stopConnectingToServer()
{
    if (_connectTimer != 0)
        _connectTimer->restart(0);
}

void Messenger::    _connectToServer(Poco::Timer& timer)
{
    Poco::Timespan connectTimeout(2,0); //2 seconds

    try 
    {
    	//Connect.
        if (_connection != 0) throw Poco::Net::NetException("A previous connection was not properly disposed.");
        _connection = new StreamSocket();
        _connection->connect(_serverAddress, connectTimeout);
        _stopConnectingToServer();
        //Further setup of connection.
        _connection->setReusePort(false);
        _out = new SocketOutputStream(*_connection);
        _in = new SocketInputStream(*_connection);
        _ownerClient->isConnected = true;
        std::cout << "Connected to server at " << _connection->peerAddress().toString() << " from local " << _connection->address().toString() << std::endl;
        //Start using connection.
        _startReceivingNotificationsFromServer();
        _startActingOnNotifications();
    }
    catch (Poco::Exception& e)
    {
        Global::writeDebug("Failed connection to server: " + e.displayText());
        if (_connection != 0) delete _connection; _connection = 0;
        if (_out != 0) delete _out; _out = 0;
        if (_in != 0) delete _in; _in = 0;
    }
}

void Messenger::    disconnect(bool restartConnection)
{
    try
    {
        if (_connection != 0)
        {
            //Tell server this client is disconnecting.
            std::vector<std::string> emptyParms;
            RemoteNotification* rn = new RemoteNotification(_ownerClient->clientID(), RemoteNotification::DISCONNECT_REPORT, emptyParms);
            try
            {
                _connection->shutdownReceive();
                sendDataToServer(rn);
            }
            catch(Poco::Exception& e)
            {
                Global::writeDebug("Server unable to receive DisconnectReport - \""+e.displayText()+"\"");
            }
            delete rn;

            //Disconnect.
            _connection->shutdown();
            _connection->close();
            delete _connection;  _connection = 0;
            //delete out;         out = 0;
            //delete in;          in = 0;
            _ownerClient->isConnected = false;
            std::cout << "Disconnected from server." << std::endl;

            if (restartConnection) {
                Poco::Thread::sleep(10000);
                startConnectingToServer();
            }
        }
    } catch(Poco::Exception& e)
    {
        std::cout << "Error disconnecting from server: " << e.displayText() << std::endl;
    }
}

// Sending
void Messenger::    sendDataToServer(RemoteNotification* rn)
{
    if (_ownerClient->isConnected) {
        Poco::ScopedLock<Poco::Mutex> lock(_sendingMutex); //Lock is auto released on exiting function.
        *_out << rn->getXml() << std::flush;
    }
}

// Receiving
void Messenger::    _startReceivingNotificationsFromServer()
//Start calling getNotificationFromServer every 0.1 seconds.
{
    if (_inTimer != 0) 
    {
        _inTimer->restart();
    }
    else 
    {
        Poco::TimerCallback<Messenger> callback(*this, &Messenger::_getNotificationFromServer);
        _inTimer = new Poco::Timer(200,100);
        _inTimer->start(callback);
        Global::writeToLocalLog("Starting to receive notifications from server.\n");
    }
}

void Messenger::    _stopReceivingNotificationsFromServer()
//Stop calling getNotificationFromServer.
{
    if (_inTimer != 0)
        _inTimer->restart(0);
}

void Messenger::    _getNotificationFromServer(Poco::Timer& timer)
//Called by a timer.  Take one complete notification from the 
//socket and take action on it.
{
    static const int bufferLength = 2048;
    static char buffer[bufferLength]; 
    static int byteCount = 0;
    int ch = -1;
    if (_in != 0  &&  _in->good()  &&  _ownerClient->isConnected) 
    {
        try 
        {
        	ch = _in->get();
            while (ch != '&' && ch != -1 && byteCount < bufferLength) {
                buffer[byteCount++] = static_cast<char>(ch);
                ch = _in->get();
            }
        } 
        catch(Poco::Net::NetException& e) {
            Global::writeDebug("Error getting data from socket: " + e.displayText());
            Global::writeToLocalLog("Error getting data from socket: " + e.displayText() + ".\n");
        }
        
        if(byteCount > 0 && ch == '&') 
        {
        	std::string data(buffer, byteCount);
            RemoteNotification* notification = RemoteNotification::parse(&data);
            Global::writeDebug("Received " + RemoteNotification::toString(notification->inCommand()));
            _notifications.push(notification);
            //Reset for next round.
            byteCount = 0; 
        }
    }
}

void Messenger::    _startActingOnNotifications()
//Start calling actOnNextNotification every 0.1 seconds.
{
    if (_outTimer != 0) 
    {
        _outTimer->restart();
    }
    else
    {
        Poco::TimerCallback<Messenger> callback(*this, &Messenger::_actOnNextNotification);
        _outTimer = new Poco::Timer(0,100);
        _outTimer->start(callback);
    }
}

void Messenger::    _stopActingOnNotifications()
//Stop calling actOnNextNotification.
{
    if (_outTimer != 0)
        _outTimer->restart(0);
}

void Messenger::    _actOnNextNotification(Poco::Timer& timer)
//Called by a timer.
{
    if (!_notifications.empty())
    {
        RemoteNotification* notification = _notifications.front();
        _ownerClient->selectProcedure(notification);
        _notifications.pop();
        delete notification;
    }
}
