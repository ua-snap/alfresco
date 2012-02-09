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

