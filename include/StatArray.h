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
		void writeStats();
		void gatherStats();
	private:
		StatList statArray;
};
#endif
