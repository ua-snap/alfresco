

#ifndef Fresco_RemoteNotification_h
#define Fresco_RemoteNotification_h


#include "Poco/DOM/Document.h"
#include "Poco/DOM/DOMParser.h"
#include <sstream>
#include <string>
#include <vector>


class RemoteNotification
{
//Data
public:
    enum IncomingCommand
    {
        VERSION_ASSERTION,
        SET_CLIENT_ID,
        REP_ASSIGNMENT,
        CLIENT_STATE_REQUEST,
        CLIENT_NAME_REQUEST,
        SERVER_STATE_REPORT,
        SERVER_DISCONNECTING_REPORT,
        FIF_PATH_REPORT
    };
    enum OutgoingCommand
    {
        CLIENT_STATE_REPORT,
        COMPUTER_NAME_REPORT,
        REP_REPORT,
        YEAR_REPORT,
        REP_COMPLETED,
        YEAR_COMPLETED,
        CLIENT_LOG_ENTRY,
        DISCONNECT_REPORT,
        STAT_SETUP,
        STAT_ADD,
		FIRESIZE_STAT_ADD
    };
    const int                       clientID() const  { return _clientID; }
    const std::string               commandString() const { return _command; }
    const enum IncomingCommand      inCommand() { return toIncomingCommand(_command); }
    const enum OutgoingCommand      outCommand() { return toOutgoingCommand(_command); }
    const std::vector<std::string>  parameters() const  { return _parameters; }
private:
    int                             _clientID;
    std::string                     _command;
    std::vector<std::string>        _parameters;

    static const std::string        clientIDField;
    static const std::string        commandField;
    static const std::string        parameterField;

//Methods
public:
    RemoteNotification(const int clientID, const IncomingCommand command, const std::vector<std::string> para);
    RemoteNotification(const int clientID, const OutgoingCommand command, const std::vector<std::string> para);
    ~RemoteNotification();
    const std::string               getXml();

    static RemoteNotification*      parse(const std::string* rawData);
    static const IncomingCommand    toIncomingCommand(std::string command);
    static const OutgoingCommand    toOutgoingCommand(std::string command);
    static const std::string        toString(const IncomingCommand command);
    static const std::string        toString(const OutgoingCommand command);
};


#endif
