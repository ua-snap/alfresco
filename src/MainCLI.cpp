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
		CustomFresco* _dummysim = new CustomFresco(args->getDebug());
		//std::cout << "XYZ" << std::endl;
		string x = "ABC";
		//long int xx = (long) _dummysim->fif().dGet("RandSeed");
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
		MyStats->gatherStats();
		MPI::COMM_WORLD.Barrier();
		}
		#endif
		MyStats->writeStats();

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
			for (int j = 0; j < MyStats->statArray.size(); j++){
				if (MyStats->statArray[j]->statType == FIRESIZE){
					sfile.open("FireSizeEvents.txt",ios_base::app);
					for (int k = 0; k < MyStats->statArray[j]->statVector.size(); k++){
						if (MyStats->statArray[j]->statVector[k][1] == i){
							sfile << MyStats->statArray[j]->statVector[k][0] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][1] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][2] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][3] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][4] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][5] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][6] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][7] << std::endl;
						}
					}
					sfile.close();
				}	
				if (MyStats->statArray[j]->statType == LIST){
					sfile.open((MyStats->statArray[j]->getTitle() + sfile_ext).c_str(), ios_base::app);
					for (int k = 0; k < MyStats->statArray[j]->statVector.size(); k++){
						if (MyStats->statArray[j]->statVector[k][1] == i){
							sfile << MyStats->firstYear + MyStats->statArray[j]->statVector[k][0] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][1] << "\t";
							sfile << MyStats->statArray[j]->statVector[k][2] << std::endl;
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

