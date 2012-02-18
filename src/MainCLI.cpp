#include <iostream>
#include <fstream>
#include <string>
#include "Fresco.h"
#include "Interface.h"
#include "CustomFresco.h"
#include "StatArray.h"
#include "Args.h"
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
	Args *args = new Args();
	args->parse(argc, argv);

	#ifdef WITHMPI
	MPI::Init();
	id = MPI::COMM_WORLD.Get_rank();
	max = MPI::COMM_WORLD.Get_size();
	if (id == 0){
	#endif

	//args->parse(argc, argv);
	if (args->getHelp() == true){ args->showHelp(); }

	#ifdef WITHMPI
	}
	#endif

	if (args->getHelp() != true){
		MyStats = new StatArray();

		int rc = 0;

		//string runDirectory = "/home/apbennett/alfresco";
		//string outDirectory = runDirectory + "/Output";
		CustomFresco* _dummysim = new CustomFresco(false);
		_dummysim->setup(args->getFifPath(), args->getFifName(), args->getOutPath(), 1234763211);
		//int firstYear = _dummysim->fif().nGet("FirstYear");
		int maxReps = _dummysim->fif().nGet("MaxReps");
		MyStats->setFirstYear(_dummysim->fif().nGet("FirstYear"));
		#ifdef WITHMPI
		std::cout << "MPI Rank: " << id << " of: " << max << std::endl;
		#endif
		std::cout << "Fresco Client " << std::endl;
		int startRep = 0;
		_dummysim->clear();
		delete _dummysim;
		for (rc = startRep + id; rc < maxReps; rc+=max){
			CustomFresco* _simulation = new CustomFresco(false);
			_simulation->setIsStopped(false);
			_simulation->setup(args->getFifPath(), args->getFifName(), args->getOutPath(), 1234763211);
			_simulation->runRep(rc,_simulation->fif().nGet("FirstYear")); 
			_simulation->runEnd();
			_simulation->clear();
			delete _simulation; _simulation = 0;
		}


		#ifdef WITHMPI
		if (max > 1){
		MyStats->gatherStats();
		MPI::COMM_WORLD.Barrier();
		}
		#endif
		MyStats->writeStats();

	}
	#ifdef WITHMPI
	MPI::Finalize();
	#endif
	exit(0);
}

