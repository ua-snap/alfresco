#ifndef Fresco_Messenger_h
#define Fresco_Messenger_h


#include "Poco/ActiveMethod.h"
#include "Poco/Mutex.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Timer.h"
#include "Poco/Void.h"
#include <string>
#include <queue>


class Client;
class RemoteNotification;


class Messenger
{
//Data
private:
    Client*                         _ownerClient;
    Poco::Timer*                    _connectTimer;
    Poco::Timer*                    _outTimer;
    Poco::Timer*                    _inTimer;
    Poco::Net::SocketAddress		_serverAddress;
    Poco::Net::StreamSocket*        _connection;
    Poco::Net::SocketOutputStream*  _out;
    Poco::Net::SocketInputStream*   _in;
    static Poco::Mutex              _sendingMutex;
    static std::queue<RemoteNotification*> _notifications;


//Methods
public:
                    Messenger(Client* owner, const std::string& serverIP, const std::string& serverPort);
                    ~Messenger(void);
    //Connecting
    void            startConnectingToServer();
    void            disconnect(bool restartConnection);
    //Sending
    void            sendDataToServer(RemoteNotification* rn);
private:
    //Connecting
    void            _stopConnectingToServer();
    void            _connectToServer(Poco::Timer& timer);
    //Receiving
    void            _startReceivingNotificationsFromServer();
    void            _stopReceivingNotificationsFromServer();
    void            _getNotificationFromServer(Poco::Timer& timer);
    void            _startActingOnNotifications();
    void            _stopActingOnNotifications();
    void            _actOnNextNotification(Poco::Timer& timer);
};


#endif
