#include "StatFile.h"
#ifdef WITHMPI
#include "mpi.h"
#endif

StatFile::StatFile(string t, int sx, int sy, StatType st){
/** 
 * Initialize StatFile, setting title, and the size of the stat array (sx, sy)
 */
	title = t;
	rows = sx;
	columns = sy;
	statType = st;
	stats = new int*[rows];
	for (int i = 0; i < rows; i++){
		stats[i] = new int[columns];
		for (int j = 0; j < columns; j++){
			stats[i][j] = 0;
		}
	}
}
string StatFile::getTitle(){
/** 
 * Return the title of the StatFile
 */
	return title;
}
void StatFile::write(int y){
/**
 * Write the stat file.  
 * Only Task 0 does this
 */
	int firstYear = y;

	std::ofstream sfile;
	string sfile_ext = ".txt";
	if (statType == MATRIX){
		sfile.open((title + sfile_ext).c_str());
		sfile << "Year";
		for (int i = 0; i < columns; i++){
			sfile << "\tRep" << i;
		}
		sfile << std::endl;
		for (int i = 0; i < rows; i++){
			sfile << firstYear + i << "\t";
			for (int j = 0; j < columns; j++){
				if (j > 0){
					sfile << "\t";
				}
				sfile << stats[i][j];
			}
			sfile << std::endl;
		}
		sfile.close();
	}
	if (statType == LIST){
		sfile.open((title + sfile_ext).c_str());
		sfile << "Year" << "\tRep" << "\tVal" << std::endl;
		for (unsigned int i = 0; i < statVector.size(); i++){
			sfile << statVector[i][2] + firstYear << "\t" << statVector[i][1] << "\t" << statVector[i][0] << std::endl;
		}
		sfile.close();
	}
	if (statType == FIRESIZE){
		sfile.open(("FireSizeEvents" + sfile_ext).c_str());
		sfile << "Year" << "\tRep" << "\tVal" << "\tCause" << "\tLow" << "\tMod" << "\tHighLSS" << "\tHighHSS" << std::endl;
		for (unsigned int i = 0; i < statVector.size(); i++){
			for (unsigned int j = 0; j < statVector[i].size(); j++){
				if (j > 0){
					sfile << "\t";
				}
				sfile << statVector[i][j];
			}
			sfile << std::endl;
		}
		sfile.close();
/*
		sfile.open((title + sfile_ext).c_str());
		sfile << "Year";
		for (int i = 0; i < columns; i++){
			sfile << "\tRep" << i;
		}
		sfile << std::endl;
		for (int i = 0; i < rows; i++){
			sfile << firstYear + i << "\t";
			for (int j = 0; j < columns; j++){
				if (j > 0){
					sfile << "\t";
				}
				sfile << stats[i][j];
			}
			sfile << std::endl;
		}
		sfile.close();
*/
	}
}
void StatFile::addStat(int nYear, int nRep, int nVal){
	stats[nYear][nRep] += nVal;
	if (statType == LIST){
		vector<int> nextRow;
		nextRow.push_back(nVal);
		nextRow.push_back(nRep);
		nextRow.push_back(nYear);
		statVector.push_back(nextRow);	
	}
}
void StatFile::addStat(int fYear, int nYear, int nRep, double dData, int nCause, int low, int mod, int highLSS, int highHSS){
	vector<int> nextRow;
	nextRow.push_back(nYear);
	nextRow.push_back(nRep);
	nextRow.push_back(dData);
	nextRow.push_back(nCause);
	nextRow.push_back(low);
	nextRow.push_back(mod);
	nextRow.push_back(highLSS);
	nextRow.push_back(highHSS);
	statVector.push_back(nextRow);	
}
int StatFile::getStat(int sx, int sy){
	return stats[sx][sy];
}
int StatFile::sendFile(){
/**
 * Send the array for each file to Task 0
 * These files are collected in the StatArray::gatherStats() function
 */
	#ifdef WITHMPI
	if (MPI::COMM_WORLD.Get_rank() != 0){
		if (statType == MATRIX){
			int sendArray[3];
			for (int i = 0; i < rows; i++){
				sendArray[0] = i;
				for (int j = 0; j < columns; j++){
					sendArray[1] = j;
					sendArray[2] = stats[i][j];
					if (stats[i][j] != 0){
						MPI::COMM_WORLD.Send(&sendArray, sizeof(sendArray), MPI_INT, 0, 2); //Tag 2 Indicates stat value
					}
				}
			}
			MPI::COMM_WORLD.Send(&sendArray, sizeof(sendArray), MPI_INT, 0, 1);  //Tag 1 Indicates completion of stat file
		}
		if (statType == LIST){
			int sendArray[3];
			for (int i = statVector.size() - 1; i >= 0; i--){
				sendArray[0] = statVector[i][0];
				sendArray[1] = statVector[i][1];
				sendArray[2] = statVector[i][2];
				MPI::COMM_WORLD.Send(&sendArray, sizeof(sendArray), MPI_INT, 0, 2); //Tag 2 Indicates stat value
			}
			MPI::COMM_WORLD.Send(&sendArray, sizeof(sendArray), MPI_INT, 0, 1);  //Tag 1 Indicates completion of stat file
		}
	}
	#endif
	return 0;
}

int StatFile::receiveFile(){
/** 
 * Initially intended to counter sendFile.
 * Not needed since this is handled by StatArray
 */
	return 0;
}
int StatFile::getTally(){
	return tally;
}
void StatFile::incTally(){
	tally++;
}
