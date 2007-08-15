#ifndef Fresco_Global_h
#define Fresco_Global_h


#include "Poco/Net/IPAddress.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Mutex.h"
#include <string>


class Global
{
//Data
public:
	bool isDebugOn;
	std::string version;
    std::string localHostName;
    Poco::Net::IPAddress localIp;
    Poco::Net::SocketAddress serverAddress;
private:
    //Static for cross thread access.
    static Poco::Mutex _singletonMutex;
    static Poco::Mutex _localLogMutex;
	static Global* _instance;


//Methods
public:
	static Global* getInstance(void);
    static void writeDebug(const std::string messege);
    static void writeToLocalLog(const std::string message);
    static std::string toString(int value);
private:
	Global(void);
	~Global(void);
};


#endif
