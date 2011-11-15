#include <iostream>
#include <fstream>
#include <string>
#include "Fresco.h"
#include "Interface.h"
#include "CustomFresco.h"
#include "mpi.h"
//
// Args Class: Utility for managing options passed in.
//
using std::string;
//
// main(): Start fresco client.
//
/*! \brief Main function 
 *  * Contains the initial launching of the ALFRESCO model as well as the master/slave setup. 
 *  */
int main(int argc, char** argv) {
	MPI::Init();
	int id = MPI::COMM_WORLD.Get_rank();
	int max = MPI::COMM_WORLD.Get_size();
	std::cout << "MPI Rank: " << id << " of: " << max << std::endl;
	std::cout << "Fresco Client " << std::endl;
	int repNum = atoi(argv[2]);
	int startRep = atoi(argv[3]);
	if (repNum > 1000 || repNum < 1){
		std::cout << "Rep Count is outside parameters" << std::endl;
		MPI::Finalize();
		exit(0);
	}
	for (int rc = startRep + id; rc < repNum; rc+=max){
		CustomFresco* _simulation = new CustomFresco(true);
		_simulation->setup("/home/apbennett/fresco/", argv[1], "/home/apbennett/fresco/Output", -1234763511);
		_simulation->runRep(rc,1860); 
		_simulation->clear();
        	delete _simulation; _simulation = 0;
	}
	MPI::Finalize();
	exit(0);
}

