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
		if (FRESCO->fif().CheckKey(FRESCO->fif().root["Simulation"]["RandSeed"])){
			randSeedVal = _dummysim->fif().root["Simulation"]["RandSeed"].asInt();
		}
		int maxReps = _dummysim->fif().root["Simulation"]["MaxReps"].asInt();
		RunStats->setFirstYear(_dummysim->fif().root["Simulation"]["FirstYear"].asInt());
		#ifdef WITHMPI
			std::cout << "Fresco Client Rank: " << id << " of: " << max << std::endl;
		#else 
			std::cout << "Fresco Client " << std::endl;
		#endif

		_dummysim->clear();
		delete _dummysim;
	
		boost::filesystem::path dir("logs");
		if (boost::filesystem::create_directory(dir))
		    std::cout << "Created Log Directory" << "\n";
	
		
		for (rc = startRep + id; rc < maxReps; rc+=max){
			std::ofstream logfile;
			std::stringstream logfile_title;
			logfile_title << "./logs/LogFile_" << rc << ".txt";
			logfile.open((logfile_title.str().c_str()));
			logfile << "Rep " << rc << " started on Task ID (" << id << ")\n";
			logfile << "======================================\n\n" << std::endl;
			logfile.close();
			CustomFresco* _simulation = new CustomFresco(args->getDebug());
			_simulation->setIsStopped(false);
			srand(randSeedVal + rc);
			long repRand = rand();
			_simulation->setup(args->getFifPath(), args->getFifName(), args->getOutPath(), repRand);
			_simulation->runRep(rc,_simulation->fif().root["Simulation"]["FirstYear"].asInt()); 
			_simulation->runEnd();
			_simulation->clear();
			delete _simulation; _simulation = 0;
			//std::cout << "Rep " << rc + 1 << " of " << maxReps << " complete" << std::endl;
			logfile.open((logfile_title.str().c_str()), fstream::app);
			logfile << "======================================" << std::endl;
			logfile << "Rep " << rc << " complete" << std::endl;
			logfile.close();
		}

		if (args->getNostats() != true){
			#ifdef WITHMPI
			if (max > 1){
			RunStats->gatherStats();
			MPI::COMM_WORLD.Barrier();
			}
			#endif
			RunStats->writeStats();
			RunStats->writeRepStats(maxReps);
		}
	}
	#ifdef WITHMPI
	MPI::Finalize();
	#endif
	exit(0);
}

