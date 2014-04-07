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
		bool CheckKey(std::string key);
		bool CheckKey(Json::Value& n);
		bool bGet(std::string key);
		double dGet(std::string key);
		int nGet(std::string key);
		std::string sGet(std::string key);
		//int pnGet(const char* key, const int*& ptr);
		int pnGet(Json::Value& n, const int*& ptr);
		int psGet(Json::Value& n, std::string *&ptr);

		int psGet(const char* key, char* const*& ptr);
		int pdGet(const char* key, const double*& ptr);
		int pdGet(Json::Value& n, const double*& ptr);
		int pbGet(const char* key, const bool*& ptr);
		int pbGet(Json::Value& key, const bool*& ptr);
};
#endif
