#ifndef Fresco_StatFile_h
#define Fresco_StatFile_h
#include <cstdio>
#include <string>
#include <fstream>

using namespace std;

enum StatType { MATRIX, LIST, FIRESIZE };

class StatFile {
	public:
		StatFile(string t, int sx, int sy, StatType st);
		void write();
		string getTitle();
		void addStat(int sx, int sy, int s);
		int getStat(int sx, int sy);
		int sendFile();
		int receiveFile();
		StatType statType;
	private:
		string title;
		int **stats;
		int rows;
		int columns;
};
#endif
