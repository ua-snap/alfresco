#ifndef Fresco_StatFile_h
#define Fresco_StatFile_h
#include <cstdio>
#include <string>
#include <fstream>

using namespace std;
class StatFile {
	public:
		StatFile(string t, int sx, int sy);
		void write();
		string getTitle();
		void addStat(int sx, int sy, int s);
		int getStat(int sx, int sy);
		int sendFile();
		int receiveFile();
	private:
		string title;
		int **stats;
		int rows;
		int columns;
};
#endif
