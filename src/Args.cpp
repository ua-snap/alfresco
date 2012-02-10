#include "Args.h"

Args::Args(){
	ip = "localhost";
	port = "9000";
	daemon = false;
	debug = false;
	help = false;
	version = false;
}
void Args::parse(int argc, char** argv){
    for(int i = 1; i < argc; i++){
        string name(argv[i]);
        if("--help" == name){
            help = true;
            break;
        }else if("--version" == name){
            version = true;
        }else if("--debug" == name){
            debug = true;
        }else{
            std::cout << std::endl << "Warning: unrecognised option " << name << std::endl << std::endl;
            help = true;
        }
    }

}
void Args::showHelp(){
	std::cout << "Options:" << std::endl;
	std::cout << "   --help" << std::endl;
	std::cout << "          Prints this usage message and exits" << std::endl;
	std::cout << "   --version" << std::endl;
	std::cout << "          Prints version number and exits" << std::endl;
	std::cout << "   --debug" << std::endl;
	std::cout << "          Provides debug output to standard out (if not in daemon mode)" << std::endl;
	std::cout << std::endl;
	std::cout << "Interactive Commands: (if not using --daemon option)" << std::endl;
	std::cout << "    exit         Terminate program" << std::endl;
	std::cout << "    quit         Terminate program" << std::endl;
	std::cout << "    state        View current state" << std::endl;
	std::cout << "    disconnect   Manually disconnect from server" << std::endl;
}

