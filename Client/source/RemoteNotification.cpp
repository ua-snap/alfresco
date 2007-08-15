

#include "Fresco/Client/RemoteNotification.h"
#include "Poco/AutoPtr.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/Exception.h"
#include "Poco/SAX/InputSource.h"
#include <sstream>
#include <vector>


//Declare static members.
const std::string RemoteNotification::clientIDField = "clientID";
const std::string RemoteNotification::commandField = "cmd";
const std::string RemoteNotification::parameterField = "parm";


RemoteNotification::                        RemoteNotification(const int clientID, const IncomingCommand command, const std::vector<std::string> parameters) :
    _clientID(clientID),
    _command(toString(command)),
    _parameters(std::vector<std::string>(parameters))
{
}

RemoteNotification::                        RemoteNotification(const int clientID, const OutgoingCommand command, const std::vector<std::string> parameters) :
    _clientID(clientID),
    _command(toString(command)),
    _parameters(std::vector<std::string>(parameters))
{
}

RemoteNotification::                        ~RemoteNotification()
{
    _parameters.clear();
}

const std::string RemoteNotification::      getXml()
{
    std::stringstream ss(std::ios_base::out);
    std::vector<std::string>::iterator parameter(_parameters.begin());

    //Construct XML formatted RPC message.
    ss << "<rpc>";
    ss << "<clientID>" << _clientID << "</clientID>";
    ss << "<cmd>" << _command << "</cmd>";
    while (parameter != _parameters.end())
        ss << "<parm>" << *parameter++ << "</parm>";
    ss << "</rpc>";
    return ss.str();
}

RemoteNotification* RemoteNotification::    parse(const std::string* rawData)
// Parses a RPC message from XML into a RemoteProcedureCall object.
// Expected format of rawData (white space doesn't matter): 
//   <rpc>
//       <clientID>1</clientID>
//       <cmd>TestCommand</cmd>
//       <parm>testing1</parm>
//       <parm>testing2</parm>
//       <parm>testingN</parm>
//   </rpc>
{
    using namespace Poco::XML;

    XMLString currentField;
    DOMParser parser;

    Poco::AutoPtr<Document> xml( parser.parseString(rawData->c_str()) );
    try
    {             
        //Get client ID.
        currentField = clientIDField;
        Poco::AutoPtr<NodeList> clientIDNodes(xml->getElementsByTagName(clientIDField));
        if (1 != clientIDNodes->length()) {
            throw Poco::Exception("RemoteProcedureCall.ParseMessage() Failed: One clientID field is allowed.  No more, no less.");
        }
        int clientID = atoi(clientIDNodes->item(0)->innerText().c_str()); 
        //Get command.
        currentField = commandField;
        Poco::AutoPtr<NodeList> commandNodes(xml->getElementsByTagName(commandField));
        if (1 != commandNodes->length()) {
            throw Poco::Exception("RemoteProcedureCall.ParseMessage() Failed: One cmd field is allowed.  No more, no less.");
        }
        std::string command(commandNodes->item(0)->innerText());
        //Get parameters.
        currentField = parameterField;
        std::vector<std::string> parameters;
        Poco::AutoPtr<NodeList> parametersNodes(xml->getElementsByTagName(parameterField));
        for (unsigned int i=0; i<parametersNodes->length(); i++) {
            parameters.push_back(parametersNodes->item(i)->innerText());
        }

        return new RemoteNotification(clientID, toIncomingCommand(command), parameters);
    }
    catch (Poco::Exception& e)
    {
        throw Poco::Exception("RemoteProcedureCall.ParseMessage() Failed: exception occured while retreiving a " + currentField + " field.", e);
    }
}

const RemoteNotification::IncomingCommand   RemoteNotification::toIncomingCommand(std::string command)
{
    IncomingCommand c;
    if ("VERSION_ASSERTION" == command)
        c = VERSION_ASSERTION;
    else if ("SET_CLIENT_ID" == command)
        c = SET_CLIENT_ID;
    else if ("REP_ASSIGNMENT" == command)
        c = REP_ASSIGNMENT;
    else if ("CLIENT_STATE_REQUEST" == command)
        c = CLIENT_STATE_REQUEST;
    else if ("CLIENT_NAME_REQUEST" == command)
        c = CLIENT_NAME_REQUEST;
    else if ("SERVER_STATE_REPORT" == command)
        c = SERVER_STATE_REPORT;
    else if ("SERVER_DISCONNECTING_REPORT" == command)
        c = SERVER_DISCONNECTING_REPORT;
    else if ("FIF_PATH_REPORT" == command)
        c = FIF_PATH_REPORT;
    else
        throw Poco::Exception("Invalid incoming command.");
    return c;
}

const RemoteNotification::OutgoingCommand   RemoteNotification::toOutgoingCommand(std::string command)
{
    OutgoingCommand c;
    if ("ClientStateReport" == command)
        c = CLIENT_STATE_REPORT;
    else if ("ComputerNameReport" == command)
        c = COMPUTER_NAME_REPORT;
    else if ("RepReport" == command)
        c = REP_REPORT;
    else if ("YearReport" == command)
        c = YEAR_REPORT;
    else if ("RepCompleted" == command)
        c = REP_COMPLETED;
    else if ("YearCompleted" == command)
        c = YEAR_COMPLETED;
    else if ("DisconnectReport" == command)
        c = DISCONNECT_REPORT;
    else if ("StatSetup" == command)
        c = STAT_SETUP;
    else if ("StatAdd" == command)
        c = STAT_ADD;
    else
        throw Poco::Exception("Invalid outgoing command.");
    return c;
}

const std::string RemoteNotification::      toString(const IncomingCommand command)
{
    switch(command)
    {
    case VERSION_ASSERTION:
        return "VERSION_ASSERTION";
    case SET_CLIENT_ID:
        return "SET_CLIENT_ID";
    case REP_ASSIGNMENT:
        return "REP_ASSIGNMENT";
    case CLIENT_STATE_REQUEST:
        return "CLIENT_STATE_REQUEST";
    case CLIENT_NAME_REQUEST:
        return "CLIENT_NAME_REQUEST";
    case SERVER_STATE_REPORT:
        return "SERVER_STATE_REPORT";
    case SERVER_DISCONNECTING_REPORT:
        return "SERVER_DISCONNECTING_REPORT";
    case FIF_PATH_REPORT:
        return "FIF_PATH_REPORT";
    default:
        throw Poco::Exception("Error converting an IncomingCommand to a string.");
    }
}

const std::string RemoteNotification::      toString(const OutgoingCommand command)
{
    switch(command)
    {
    case CLIENT_STATE_REPORT:
        return "CLIENT_STATE_REPORT";
    case COMPUTER_NAME_REPORT:
        return "COMPUTER_NAME_REPORT";
    case REP_REPORT:
        return "REP_REPORT";
    case YEAR_REPORT:
        return "YEAR_REPORT";
    case REP_COMPLETED:
        return "REP_COMPLETED";
    case YEAR_COMPLETED:
        return "YEAR_COMPLETED";
    case CLIENT_LOG_ENTRY:
        return "CLIENT_LOG_ENTRY";
    case DISCONNECT_REPORT:
        return "DISCONNECT_REPORT";
    case STAT_SETUP:
        return "STAT_SETUP";
    case STAT_ADD:
        return "STAT_ADD";
    default:
        throw Poco::Exception("Error converting an OutgoingCommand to a string.");
    }
}
