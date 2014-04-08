#ifndef FIF_H
#define FIF_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "jsoncpp/json/json.h"



class FIF {
	public:
		std::string _fileName;
		Json::Value root;
		Json::Reader reader;
		FIF();
		std::string fileName();
		void Initialize(std::string path, std::string fifName);
		bool CheckKey(Json::Value& n);
		int pnGet(Json::Value& n, int*& ptr);
		int psGet(Json::Value& n, std::string *&ptr);
		int pdGet(Json::Value& n, double*& ptr);
		int pbGet(Json::Value& key, bool*& ptr);
};
#endif
