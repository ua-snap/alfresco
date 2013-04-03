#include <iostream>
#include <fstream>
#include <string>
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

StatArray* RunStats;
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

	if (args->getHelp() == true){ args->showHelp(); }
	if (args->getVersion() == true){ args->showVersion(); }
	
	#ifdef WITHMPI
	}
	#endif


	if (args->getHelp() != true){
		RunStats = new StatArray();

		int rc = 0;
		int startRep = 0;
		if (args->getStartRep() > 0){
			startRep = args->getStartRep();
		}

		CustomFresco* _dummysim = new CustomFresco(args->getDebug());
		long randSeedVal = 0;
		_dummysim->setup(args->getFifPath(), args->getFifName(), args->getOutPath(), randSeedVal);
		if (FRESCO->fif().CheckKey("RandSeed")){
			randSeedVal = _dummysim->fif().nGet("RandSeed");
		}
		int maxReps = _dummysim->fif().nGet("MaxReps");
		RunStats->setFirstYear(_dummysim->fif().nGet("FirstYear"));
		#ifdef WITHMPI
			std::cout << "Fresco Client Rank: " << id << " of: " << max << std::endl;
		#else 
			std::cout << "Fresco Client " << std::endl;
		#endif

		_dummysim->clear();
		delete _dummysim;
		for (rc = startRep + id; rc < maxReps; rc+=max){
			CustomFresco* _simulation = new CustomFresco(args->getDebug());
			_simulation->setIsStopped(false);
			srand(randSeedVal + rc);
			long repRand = rand();
			_simulation->setup(args->getFifPath(), args->getFifName(), args->getOutPath(), repRand);
			_simulation->runRep(rc,_simulation->fif().nGet("FirstYear")); 
			_simulation->runEnd();
			_simulation->clear();
			delete _simulation; _simulation = 0;
			std::cout << "Rep " << rc + 1 << " of " << maxReps << " complete" << std::endl;
		}


		#ifdef WITHMPI
		if (max > 1){
		RunStats->gatherStats();
		MPI::COMM_WORLD.Barrier();
		}
		#endif
		RunStats->writeStats();
		RunStats->writeRepStats(maxReps);
	}
	#ifdef WITHMPI
	MPI::Finalize();
	#endif
	exit(0);
}

