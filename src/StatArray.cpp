#include "StatArray.h"
#include "StatFile.h"
#include <iostream>
#ifdef WITHMPI
#include "mpi.h"
#endif

StatArray::StatArray(){

}
StatArray::StatArray(const StatArray& s){

}
void StatArray::setFirstYear(int n){
	if (n > 0){
		firstYear = n;
	}
}
void StatArray::addStatFile(string t, int r, int c, StatType s){
	bool statExists = false;
	for (int i = 0; i < statArray.size(); i++){
		if (statArray[i]->getTitle().compare(t) == 0){
			statExists = true;
		}
	}
	if (statExists == false){
		statArray.push_back(new StatFile(t, r, c, s));
	}
}
void StatArray::addStat(string t, int r, int c, int y){
	for (int i = 0; i < statArray.size(); i++){
		if (statArray[i]->getTitle().compare(t) == 0){
			statArray[i]->addStat(r - firstYear, c, y);
		}
	}
}
void StatArray::writeStats(){
	for (int i = 0; i < statArray.size(); i++){
		#ifdef WITHMPI
		if (MPI::COMM_WORLD.Get_rank() == 0){
		#endif
		statArray[i]->write(firstYear);
		#ifdef WITHMPI
		}
		#endif
	}
}
void StatArray::gatherStats(){
/**
 * Gather stats sent by the StatFile::sendFile() function
 * Aggregates all files to Task 0 for writing
 */
	#ifdef WITHMPI
	if (MPI::COMM_WORLD.Get_rank() == 0){
		int callCount = 0;
		MPI::Status status;
		int recvArray[3];
		int recvCount = MPI::COMM_WORLD.Get_size();
		for (int i = 0; i < statArray.size(); i++){
			recvCount = MPI::COMM_WORLD.Get_size();
			do {
				MPI::COMM_WORLD.Recv(&recvArray, sizeof(recvArray), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, status);
				if (status.Get_tag() == 2){
					statArray[i]->addStat(recvArray[0], recvArray[1], recvArray[2]);
					callCount++;
				} else if (status.Get_tag() == 1){
					recvCount--;
				} else {
					std::cout << "Error" << std::endl;
				}
				
			} while (recvCount > 1);
			MPI::COMM_WORLD.Barrier();
		}
	} else {
		for (int i = 0; i < statArray.size(); i++){
			statArray[i]->sendFile();
			MPI::COMM_WORLD.Barrier();
		}
	}
	MPI::COMM_WORLD.Barrier();
	#endif
}
