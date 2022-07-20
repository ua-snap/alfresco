#include "Args.h"

Args::Args(){
	fifFile = "default.fif";
	boost::filesystem::path p = boost::filesystem::initial_path();
	fifPath = p.string();
	p /= "";
	outPath = p.string();
	inPath = "";
	debug = false;
	nostats = false;
	help = false;
	showversion = false;
	#ifdef ALF_VERSION
		version = ALF_VERSION;
	#else
		version = "0.Dev.0";
	#endif
	startRep = 0;

}
void Args::parse(int argc, char** argv){

	desc.add_options()
		("help,h", "produces helps message")
		("version,v", "show the version information")
		("debug,d", "enable debug mode")
		("nostats,n", "disable summary stats gathering")
		("startrep,s", boost::program_options::value<int>(), "set the starting rep number")
		("fif,f", boost::program_options::value<string>(), "set the fif file")
		("fif-path", boost::program_options::value<string>(), "set the fif path")
		("input-path", boost::program_options::value<string>(), "set the input path")
		("output-path", boost::program_options::value<string>(), "set the output path")
	;
	
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), varmap);
	boost::program_options::notify(varmap);

	if (varmap.count("help")){
		help = true;
	}
	if (varmap.count("version")){
		showversion = true;
	}
	if (varmap.count("debug")){
		debug = true;
	}
	if (varmap.count("nostats")){
		nostats = true;
	}
	if (varmap.count("startrep")){
        	startRep = varmap["startrep"].as<int>();
	}
	if (varmap.count("fif")){
        	fifFile = varmap["fif"].as<string>();
	}
	if (varmap.count("fif-path")){
        	fifPath = varmap["fif-path"].as<string>();
	}
	if (varmap.count("input-path")){
        	inPath = varmap["input-path"].as<string>();
	}
	if (varmap.count("output-path")){
        	outPath = varmap["output-path"].as<string>();
	}
}
int Args::getStartRep(){
	return startRep;
}
string Args::getFifName(){
	return fifFile;
}
string Args::getFifPath(){
	return fifPath;
}
string Args::getOutPath(){
	return outPath;
}
void Args::showHelp(){
	/* Print out command help */
	std::cout << desc << std::endl;
}
void Args::showVersion(){
	std::cout << "alfresco version " << version << std::endl;
}

