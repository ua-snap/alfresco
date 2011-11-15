#include "Client.h"
#include "demon_me.h"
#include "Global.h"
#include "Except.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Foundation.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Path.h"
#include <iostream>
#include <fstream>
#include <string>


//
// Args Class: Utility for managing options passed in.
//
using std::string;
const string DEFAULT_PORT = "9051";
class Args{
    string ip;
    string port;
	bool daemon;
	bool debug;
    bool help;
	bool version;
public:
    inline Args() : ip("localhost"), port(DEFAULT_PORT), daemon(false), debug(false), help(false), version(false){}
    void parse(int argc, char** argv);
    void showUsage();
    void showSummary();

    inline const string& getIP() const { return ip;}
    inline const string& getPort() const { return port; }
    inline const bool getDaemon(){ return daemon; }
    inline const bool getDebug(){ return debug; }
    inline const bool getHelp() const { return help; }
    inline const bool getVersion() const { return version; }
};
void Args::parse(int argc, char** argv){
    for(int i=1; i<argc; i++){
        string name(argv[i]);
        if("--help" == name){
            help = true;
            break;
		}else if("--version" == name){
			version = true;
		}else if("--ip" == name){
            ip = argv[++i];
        }else if("--port" == name){
            port = argv[++i];
        }else if("--daemon" == name){
            daemon = true;
        }else if("--debug" == name){
            debug = true;
        }else{
            std::cout << std::endl << "Warning: unrecognised option " << name << std::endl << std::endl;
            help = true;
        }
    }
}
void Args::showUsage(){
    std::cout << "Options:" << std::endl;
    std::cout << "   --help" << std::endl;
    std::cout << "          Prints this usage message and exits" << std::endl;
	std::cout << "   --version" << std::endl;
    std::cout << "          Prints version number and exits" << std::endl;
    std::cout << "   --ip <host>" << std::endl;
    std::cout << "          Specifies host to connect to (default is localhost)" << std::endl;
    std::cout << "   --port <port>" << std::endl;
    std::cout << "          Specifies port to connect to (default is " << DEFAULT_PORT << ")" << std::endl;
    std::cout << "   --daemon" << std::endl;
    std::cout << "          Forces the client to run as a daemon" << std::endl;
    std::cout << "   --debug" << std::endl;
    std::cout << "          Provides debug output to standard out (if not in daemon mode)" << std::endl;
    std::cout << std::endl;
    std::cout << "Interactive Commands: (if not using --daemon option)" << std::endl;
    std::cout << "    exit         Terminate program" << std::endl;
    std::cout << "    quit         Terminate program" << std::endl;
    std::cout << "    state        View current state" << std::endl;
    std::cout << "    disconnect   Manually disconnect from server" << std::endl;
}
void Args::showSummary() {
    std::cout << "Settings:" << std::endl;
	std::cout << "    IP     " << getIP() << std::endl;
	std::cout << "    Port   " << getPort() << std::endl;
	std::cout << "    Daemon " << (getDaemon() ? "Yes" : "No") << std::endl;
	std::cout << "    Debug  " << (getDebug() ? "Yes" : "No") << std::endl;
}
void applyConsoleInput()
{
	std::string input = "";
	bool running = true;
    while (running) {
		if (std::cin.good()) {
            std::cin >> input;
            if (input == "exit" || input == "quit") {
                running = false;
            }
            input = "";
        } else { Poco::Thread::sleep(5000); }
        Poco::Thread::sleep(100);
	}
}


//
// main(): Start fresco client.
//
int main(int argc, char** argv) {
	std::cout << "Fresco Client " << Global::getInstance()->version << std::endl;
    Args args;  args.parse(argc, argv);
    if (args.getHelp())    { args.showUsage();   return 0; } 
	if (args.getVersion()) { return 0; }
	args.showSummary();
    
	bool hadError = false;
    std::string errorMessage = "";
	try {
    	Global::getInstance()->isDebugOn = args.getDebug();
    	if (args.getDaemon()) daemonize();
		Client client(args.getIP(), args.getPort());
		client.connectToServer();
		applyConsoleInput();
    } catch (Poco::Exception& e) {  hadError=true;  errorMessage = "Error: " + e.displayText();
    } catch (Exception& e) {        hadError=true;  errorMessage = "Error: " + e.message;
    } catch(...) {                  hadError=true;  errorMessage = "Error: Could not complete remote command: ."; }
    if (hadError) {
        std::cout << std::endl << errorMessage << std::endl;
    }
}

