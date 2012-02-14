#include <iostream>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#ifdef WITHMPI
#include "mpi.h"
#endif

using namespace std;


class Args{
    string fifFile;
    string fifPath;
    string inPath;
    string outPath;
    bool debug;
    bool help;
    bool version;
public:
	Args();
	void parse(int argc, char** argv);
	void showHelp();
	string getFifName();
	string getFifPath();
	string getInPath();
	string getOutPath();

	inline const bool getDebug(){ return debug; }
	inline const bool getHelp() const { return help; }
	inline const bool getVersion() const { return version; }
};
