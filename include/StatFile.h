#ifndef Fresco_StatFile_h
#define Fresco_StatFile_h
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

enum StatType { MATRIX, LIST, FIRESIZE };

class StatFile {
	public:
		StatFile(string t, int sx, int sy, StatType st);
		void write(int y);
		string getTitle();
		void addStat(int nYear, int nRep, int nVal);
		void addStat(int fYear, int nYear, int nRep, double dData, int nCause, int low, int mod, int highLSS, int highHSS);
		int getStat(int sx, int sy);
		int sendFile();
		int receiveFile();
		int getTally();
		void incTally();
		StatType statType;
		vector<vector<int> > statVector;
	private:
		string title;
		int **stats;
		int rows;
		int columns;
		int tally;
};
#endif
