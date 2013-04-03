#include <iostream>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

using namespace std;


class Args{
	boost::program_options::options_description desc;
	boost::program_options::variables_map varmap;
	string fifFile;
	string fifPath;
	string inPath;
	string outPath;
	string version;
	bool debug;
	bool help;
	bool showversion;
	int startRep;
public:
	Args();
	void parse(int argc, char** argv);
	void showHelp();
	void showVersion();
	string getFifName();
	string getFifPath();
	string getInPath();
	string getOutPath();
	
	int getStartRep();

	inline const bool getDebug(){ return debug; }
	inline const bool getHelp() const { return help; }
	inline const bool getVersion() const { return showversion; }
};
