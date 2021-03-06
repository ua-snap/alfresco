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
	for (unsigned int i = 0; i < statArray.size(); i++){
		if (statArray[i]->getTitle().compare(t) == 0){
			statExists = true;
		}
	}
	if (statExists == false){
		statArray.push_back(new StatFile(t, r, c, s));
	}
}
void StatArray::addStat(string t, int r, int c, int y){
	for (unsigned int i = 0; i < statArray.size(); i++){
		if (statArray[i]->getTitle().compare(t) == 0){
			statArray[i]->addStat(r - firstYear, c, y);
		}
	}
}
void StatArray::addStat(string t, int nYear, int nRep, double dData, int nCause, int low, int mod, int highLSS, int highHSS){
	for (unsigned int i = 0; i < statArray.size(); i++){
		if (statArray[i]->getTitle().compare(t) == 0){
			statArray[i]->addStat(firstYear, nYear, nRep, dData, nCause, low, mod, highLSS, highHSS);
		}
	}
}
void StatArray::writeStats(){
	for (unsigned int i = 0; i < statArray.size(); i++){
		#ifdef WITHMPI
		if (MPI::COMM_WORLD.Get_rank() == 0){
		#endif
		statArray[i]->write(firstYear);
		#ifdef WITHMPI
		}
		#endif
	}
}
void StatArray::writeRepStats(int mr){
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
	for (int i = 0; i < mr; i++){
		for (unsigned int j = 0; j < statArray.size(); j++){
			if (statArray[j]->statType == FIRESIZE){
				sfile.open("FireSizeEvents.txt",ios_base::app);
				for (unsigned int k = 0; k < statArray[j]->statVector.size(); k++){
					if (statArray[j]->statVector[k][1] == i){
						sfile << statArray[j]->statVector[k][0] << "\t";
						sfile << statArray[j]->statVector[k][1] << "\t";
						sfile << statArray[j]->statVector[k][2] << "\t";
						sfile << statArray[j]->statVector[k][3] << "\t";
						sfile << statArray[j]->statVector[k][4] << "\t";
						sfile << statArray[j]->statVector[k][5] << "\t";
						sfile << statArray[j]->statVector[k][6] << "\t";
						sfile << statArray[j]->statVector[k][7] << std::endl;
					}
				}
				sfile.close();
			}	
			if (statArray[j]->statType == LIST){
				sfile.open((statArray[j]->getTitle() + sfile_ext).c_str(), ios_base::app);
				for (unsigned int k = 0; k < statArray[j]->statVector.size(); k++){
					if (statArray[j]->statVector[k][1] == i){
						sfile << firstYear + statArray[j]->statVector[k][0] << "\t";
						sfile << statArray[j]->statVector[k][1] << "\t";
						sfile << statArray[j]->statVector[k][2] << std::endl;
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
void StatArray::gatherStats(){
/**
 * Gather stats sent by the StatFile::sendFile() function
 * Aggregates all files to Task 0 for writing
 */
	#ifdef WITHMPI
	if (MPI::COMM_WORLD.Get_rank() == 0){
		int callCount = 0;
		MPI::Status status;
		int recvCount = MPI::COMM_WORLD.Get_size();
		for (unsigned int i = 0; i < statArray.size(); i++){
			std::cout << "Gathering Stats for " << statArray[i]->getTitle() << "...";
			if (statArray[i]->statType == MATRIX){
				int recvArray[3];
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
			if (statArray[i]->statType == LIST){
				int recvArray[3];
				recvCount = MPI::COMM_WORLD.Get_size();
				do {
					MPI::COMM_WORLD.Recv(&recvArray, sizeof(recvArray), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, status);
					vector<int> tmpRow;
					tmpRow.push_back(recvArray[0]);
					tmpRow.push_back(recvArray[1]);
					tmpRow.push_back(recvArray[2]);
					if (status.Get_tag() == 2){
						if (statArray[i]->statVector.size() < 1){
							statArray[i]->statVector.push_back(tmpRow);
						} else {
							for (unsigned int j = 0; j < statArray[i]->statVector.size(); j++){
								if (recvArray[1] < statArray[i]->statVector[j][1]){
									if (j > 0){
										if (recvArray[1] > statArray[i]->statVector[j-1][1]){
											statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j, tmpRow);
											break;
										}
									} else {
										statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j, tmpRow);
										break;
									}
								} else if (recvArray[1] == statArray[i]->statVector[j][1]){
									statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j, tmpRow);
									break;
								} else if (recvArray[1] > statArray[i]->statVector[j][1]){
									if (j < statArray[i]->statVector.size() - 1){
										if (recvArray[1] < statArray[i]->statVector[j+1][1]){
											statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j + 1, tmpRow);
											break;
										}
									} else {
										statArray[i]->statVector.push_back(tmpRow);
										break;
									}
								} else {
									statArray[i]->statVector.push_back(tmpRow);
									break;
								}
							}
						}
						callCount++;
					} else if (status.Get_tag() == 1){
						recvCount--;
					} else {
						std::cout << "MPI Tag Mismatch Error" << std::endl;
					}
				} while (recvCount > 1);
				MPI::COMM_WORLD.Barrier();
			}
			if (statArray[i]->statType == FIRESIZE){
				int recvArray[9];
				recvCount = MPI::COMM_WORLD.Get_size();
				do {
					MPI::COMM_WORLD.Recv(&recvArray, sizeof(recvArray), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, status);
					vector<int> tmpRow;
					tmpRow.push_back(recvArray[0]);
					tmpRow.push_back(recvArray[1]);
					tmpRow.push_back(recvArray[2]);
					tmpRow.push_back(recvArray[3]);
					tmpRow.push_back(recvArray[4]);
					tmpRow.push_back(recvArray[5]);
					tmpRow.push_back(recvArray[6]);
					tmpRow.push_back(recvArray[7]);
					if (status.Get_tag() == 2){
						if (statArray[i]->statVector.size() < 1){
							statArray[i]->statVector.push_back(tmpRow);
						} else {
							for (unsigned int j = 0; j < statArray[i]->statVector.size(); j++){
								if (recvArray[1] < statArray[i]->statVector[j][1]){
									if (j > 0){
										if (recvArray[1] > statArray[i]->statVector[j-1][1]){
											statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j, tmpRow);
											break;
										}
									} else {
										statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j, tmpRow);
										break;
									}
								} else if (recvArray[1] == statArray[i]->statVector[j][1]){
									statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j, tmpRow);
									break;
								} else if (recvArray[1] > statArray[i]->statVector[j][1]){
									if (j < statArray[i]->statVector.size() - 1){
										if (recvArray[1] < statArray[i]->statVector[j+1][1]){
											statArray[i]->statVector.insert(statArray[i]->statVector.begin() + j + 1, tmpRow);
											break;
										}
									} else {
										statArray[i]->statVector.push_back(tmpRow);
										break;
									}
								} else {
									statArray[i]->statVector.push_back(tmpRow);
									break;
								}
							}
						}
					} else if (status.Get_tag() == 1){
						recvCount--;
					} else {
						std::cout << "MPI Tag Mismatch Error" << std::endl;
					}
				} while (recvCount > 1);
				MPI::COMM_WORLD.Barrier();
			}
			std::cout << " Complete\n";
		}
	} else {
		for (unsigned int i = 0; i < statArray.size(); i++){
			if (statArray[i]->statType == MATRIX || statArray[i]->statType == LIST || statArray[i]->statType == FIRESIZE){
				statArray[i]->sendFile();
				MPI::COMM_WORLD.Barrier();
			}
		}
	}
	MPI::COMM_WORLD.Barrier();
	#endif
}
