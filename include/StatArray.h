#ifndef Fresco_StatArray_h
#define Fresco_StatArray_h
#include "StatFile.h"
#include <vector>

typedef std::vector<StatFile *> StatList;
class StatArray {
	public:
		StatArray();
		StatArray(const StatArray& s);
		void addStatFile(string t, int r, int c, StatType s);
		void addStat(string t, int r, int c, int y);
		void addStat(string t, int nYear, int nRep, double dData, int nCause, int low, int mod, int highLSS, int highHSS);
		void writeStats();
		void writeRepStats(int mr);
		void gatherStats();
		void setFirstYear(int n);
		StatList statArray;
		int firstYear;
};
#endif
