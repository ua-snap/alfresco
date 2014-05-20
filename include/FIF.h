#ifndef FIF_H
#define FIF_H
/**
 * This file is used to parse the input configuration file, and create a commonly
 * accessible config object for use throughout the simulation. File input is 
 * expected to be in the JSON format. FIF is a holdover from the old format
 * refrenced as Fire Input File.
 * */

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
