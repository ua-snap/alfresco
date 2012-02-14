#include "Args.h"
#include <boost/program_options.hpp>

Args::Args(){
	fifFile = "default.fif";
	boost::filesystem::path p = boost::filesystem::initial_path();
	fifPath = p.string();
	p /= "Output";
	outPath = p.string();
	inPath = "";
	std::cout << inPath << std::endl;
	debug = false;
	help = false;
	version = false;
	boost::program_options::options_description desc;
	boost::program_options::variables_map varmap;
	desc.add_options()
		("help", "produces helps message")
		("fif", "set the fif file")
	;
}
void Args::parse(int argc, char** argv){
    for(int i = 1; i < argc; i++){
        string name(argv[i]);
        if("--help" == name){
            help = true;
            break;
        }else if("--version" == name){
            version = true;
	}else if("--fif" == name){
            fifFile = argv[++i];
	}else if("--fif-path" == name){
            fifPath = argv[++i];
	}else if("--in-path" == name){
            inPath = argv[++i];
	}else if("--out-path" == name){
            outPath = argv[++i];
        }else if("--debug" == name){
            debug = true;
        }else{
            std::cout << std::endl << "Warning: unrecognised option " << name << std::endl << std::endl;
            help = true;
        }
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
	std::cout << "Options:" << std::endl;
	std::cout << "   --help" << std::endl;
	std::cout << "          Prints this usage message and exits" << std::endl;
	std::cout << "   --version" << std::endl;
	std::cout << "          Prints version number and exits" << std::endl;
	std::cout << "   --fif [FileName]" << std::endl;
	std::cout << "          Uses [FileName] as the fif file" << std::endl;
	std::cout << "   --debug" << std::endl;
	std::cout << "          Provides debug output to standard out (if not in daemon mode)" << std::endl;
	std::cout << std::endl;
	std::cout << "Interactive Commands: (if not using --daemon option)" << std::endl;
	std::cout << "    exit         Terminate program" << std::endl;
	std::cout << "    quit         Terminate program" << std::endl;
	std::cout << "    state        View current state" << std::endl;
	std::cout << "    disconnect   Manually disconnect from server" << std::endl;
}

