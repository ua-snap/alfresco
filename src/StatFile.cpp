#include "StatFile.h"
#ifdef WITHMPI
#include "mpi.h"
#endif

StatFile::StatFile(string t, int sx, int sy){
/** 
 * Initialize StatFile, setting title, and the size of the stat array (sx, sy)
 */
	title = t;
	rows = sx;
	columns = sy;
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
void StatFile::write(){
/**
 * Write the stat file.  Only Task 0 does this
 */
	std::ofstream sfile;
	string sfile_ext = ".txt";
	sfile.open((title + sfile_ext).c_str());
	sfile << "Year";
	for (int i = 0; i < columns; i++){
		sfile << "\tRep" << i;
	}
	sfile << std::endl;
	for (int i = 0; i < rows; i++){
		sfile << 1860 + i << "\t";
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
void StatFile::addStat(int sx, int sy, int s){
	stats[sx][sy] = s;
}
int StatFile::getStat(int sx, int sy){
	return stats[sx][sy];
}
int StatFile::sendFile(){
	#ifdef WITHMPI
	if (MPI::COMM_WORLD.Get_rank() != 0){
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
	#endif
	return 0;
}

int StatFile::receiveFile(){
	#ifdef WITHMPI
	/*
	if (MPI::COMM_WORLD.Get_rank() == 0){
		MPI::Status status;
		int recvArray[3];
		int recvCount = MPI::COMM_WORLD.Get_size();
		do {
			MPI::COMM_WORLD.Recv(&recvArray, sizeof(recvArray), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, status);
			if (status.Get_tag() == 2){
				addStat(recvArray[0], recvArray[1], recvArray[2]);
			} else if (status.Get_tag() == 1){
				recvCount--;
			}
		} while (recvCount > 1);
	}
	*/
	#endif
	return 0;
}
