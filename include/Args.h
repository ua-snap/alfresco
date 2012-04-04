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
