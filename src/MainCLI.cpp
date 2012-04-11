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

	//args->parse(argc, argv);
	if (args->getHelp() == true){ args->showHelp(); }

	#ifdef WITHMPI
	}
	#endif

	if (args->getHelp() != true){
		RunStats = new StatArray();

		int rc = 0;

		//string runDirectory = "/home/apbennett/alfresco";
		//string outDirectory = runDirectory + "/Output";
		CustomFresco* _dummysim = new CustomFresco(args->getDebug());
		_dummysim->setup(args->getFifPath(), args->getFifName(), args->getOutPath(), 1234763211);
		//int firstYear = _dummysim->fif().nGet("FirstYear");
		int maxReps = _dummysim->fif().nGet("MaxReps");
		RunStats->setFirstYear(_dummysim->fif().nGet("FirstYear"));
		#ifdef WITHMPI
		std::cout << "MPI Rank: " << id << " of: " << max << std::endl;
		#endif
		std::cout << "Fresco Client " << std::endl;
		int startRep = 0;
		_dummysim->clear();
		delete _dummysim;
		for (rc = startRep + id; rc < maxReps; rc+=max){
			CustomFresco* _simulation = new CustomFresco(args->getDebug());
			_simulation->setIsStopped(false);
			_simulation->setup(args->getFifPath(), args->getFifName(), args->getOutPath(), 1234763211);
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

		std::ofstream sfile;
		string sfile_ext = ".txt";
		#ifdef WITHMPI
		if (MPI::COMM_WORLD.Get_rank() == 0){
		#endif
			sfile.open("FireSizeEvents.txt");
			sfile << "Year" << "\tRep" << "\tVal" << "\tCause" << "\tLow" << "\tMod" << "\tHighLSS" << "\tHighHSS" << std::endl;
			sfile.close();
		#ifdef WITHMPI
		}
		#endif
		for (int i = 0; i < maxReps; i++){
			for (unsigned int j = 0; j < RunStats->statArray.size(); j++){
				if (RunStats->statArray[j]->statType == FIRESIZE){
					sfile.open("FireSizeEvents.txt",ios_base::app);
					for (unsigned int k = 0; k < RunStats->statArray[j]->statVector.size(); k++){
						if (RunStats->statArray[j]->statVector[k][1] == i){
							sfile << RunStats->statArray[j]->statVector[k][0] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][1] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][2] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][3] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][4] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][5] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][6] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][7] << std::endl;
						}
					}
					sfile.close();
				}	
				if (RunStats->statArray[j]->statType == LIST){
					sfile.open((RunStats->statArray[j]->getTitle() + sfile_ext).c_str(), ios_base::app);
					for (unsigned int k = 0; k < RunStats->statArray[j]->statVector.size(); k++){
						if (RunStats->statArray[j]->statVector[k][1] == i){
							sfile << RunStats->firstYear + RunStats->statArray[j]->statVector[k][0] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][1] << "\t";
							sfile << RunStats->statArray[j]->statVector[k][2] << std::endl;
						}
					}
					sfile.close();
				}
			}

			#ifdef WITHMPI
			MPI::COMM_WORLD.Barrier();
			#endif
		}
	}
	#ifdef WITHMPI
	MPI::Finalize();
	#endif
	exit(0);
}

