#include <iostream>
#include <string>

using namespace std;

class Args{
    string ip;
    string port;
    string fifFile;
    bool daemon;
    bool debug;
    bool help;
    bool version;
public:
    Args();
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
