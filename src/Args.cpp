#include "Args.h"


Args::Args(){
	fifFile = "default.fif";
	boost::filesystem::path p = boost::filesystem::initial_path();
	fifPath = p.string();
	p /= "Output";
	outPath = p.string();
	inPath = "";
	debug = false;
	help = false;
	version = false;

}
void Args::parse(int argc, char** argv){

	desc.add_options()
		("help,h", "produces helps message")
		("version,v", "show the version information")
		("debug,d", "enable debug mode")
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
	if (varmap.count("debug")){
		debug = true;
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
	std::cout << desc << std::endl;
}

