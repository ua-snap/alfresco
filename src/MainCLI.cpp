#include <iostream>
#include <fstream>
#include <string>
#include "Fresco.h"
#include "Interface.h"
#include "CustomFresco.h"
#include "StatArray.h"
#ifdef WITHMPI
#include "mpi.h"
#endif
//
// Args Class: Utility for managing options passed in.
//
using std::string;
//
// main(): Start fresco client.
//
/** \brief Main function 
 *  * Contains the initial launching of the ALFRESCO model as well as the master/slave setup. 
 *  */

StatArray* MyStats;
int main(int argc, char** argv) {
	int id = 0;
	int max = 1;
	#ifdef WITHMPI
	MPI::Init();
	id = MPI::COMM_WORLD.Get_rank();
	max = MPI::COMM_WORLD.Get_size();
	if (id == 0){
		std::cout << "MPI Enabled" << std::endl;
	}
	#endif

	MyStats = new StatArray();
	int rc;

	CustomFresco* _dummysim = new CustomFresco(false);
	_dummysim->setup("/home/apbennett/fresco/", argv[1], "/home/apbennett/fresco/Output", 1234763211);
	//int firstYear = _dummysim->fif().nGet("FirstYear");
	int maxReps = _dummysim->fif().nGet("MaxReps");
	#ifdef WITHMPI
	std::cout << "MPI Rank: " << id << " of: " << max << std::endl;
	#endif
	std::cout << "Fresco Client " << std::endl;
	int startRep = 0;
		_dummysim->clear();
	for (rc = startRep + id; rc < maxReps; rc+=max){
		CustomFresco* _simulation = new CustomFresco(false);
		_simulation->setup("/home/apbennett/fresco/", argv[1], "/home/apbennett/fresco/Output", 1234763211);
		_simulation->runRep(rc,1860); 
		_simulation->runEnd();
		_simulation->clear();
        	delete _simulation; _simulation = 0;
	}


	#ifdef WITHMPI
	MyStats->transferStats();
	MPI::COMM_WORLD.Barrier();
	#endif
	MyStats->writeStats();
	#ifdef WITHMPI
	MPI::Finalize();
	#endif
	exit(0);
}

