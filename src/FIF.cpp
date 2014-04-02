#include "FIF.h"


FIF::FIF(){

}
void FIF::Initialize(std::string path, std::string fifName){
	std::ifstream fif("Alfresco.JSON", std::ifstream::binary);
	bool parsed = reader.parse(fif, root, false);
	
	if(!parsed) {
		std::cout << "Error parsing JSON" << "\n" << reader.getFormatedErrorMessages() << "\n";
	}
//	std::cout << root.toStyledString() << "\n";
}
std::string FIF::fileName(){
	return _fileName;
}
bool FIF::CheckKey(std::string key){
	return root[key].isNull();
}
bool FIF::CheckKey(Json::Value& n){
	return !n.isNull();
}
bool FIF::bGet(std::string key){
	return root[key].asBool();
}
std::string FIF::sGet(std::string key){
	return root[key].asString();
}
int FIF::nGet(std::string key){
	return root[key].asInt();
}
double FIF::dGet(std::string key){
	return root[key].asDouble();
}
int FIF::pnGet(const char* key, const int*& ptr){
	return root[key].size();
}

int FIF::pnGet(Json::Value& n, const int*& ptr){
	int temp_array[n.size()];
	for (int i = 0; i < n.size(); i++){
		temp_array[i] = n[i].asInt();
	}
	ptr = temp_array;
	return n.size();
}
int FIF::psGet(const char* key, const char*& ptr){
	return root[key].size();
}
int FIF::psGet(Json::Value& n, const char*& ptr){
	std::cout << "Way1";
	//char* temp_array[n.size()];
	//for (int i = 0; i < n.size(); i++){
	//	temp_array[i] = n[i].asCString();
	//}
	//ptr = temp_array;
	return n.size();
}
int FIF::psGet(Json::Value& n, char* const*& ptr){
	std::cout << "Way2" << n.size() << std::endl;
	const char* temp_array[n.size()];
	for (int i = 0; i < n.size(); i++){
		temp_array[i] = n[i].asCString();
		std::cout << temp_array[i] << std::endl;
	}
	std::cout << &ptr << std::endl;
	//ptr = temp_array;
	return n.size();
}
int FIF::psGet(const char* key, char* const*& ptr){
	return root[key].size();
}
int FIF::pdGet(const char* key, const double*& ptr){
	return root[key].size();
}
int FIF::pdGet(Json::Value& n, const double*& ptr){
	double temp_array[n.size()];
	for (int i = 0; i < n.size(); i++){
		temp_array[i] = n[i].asDouble();
	}
	ptr = temp_array;
	return n.size();
}
int FIF::pbGet(const char* key, const bool*& ptr){
	return root[key].size();
}
int FIF::pbGet(Json::Value& n, const bool*& ptr){
	bool temp_array[n.size()];
	for (int i = 0; i < n.size(); i++){
		temp_array[i] = n[i].asBool();
	}
	ptr = temp_array;
	return n.size();
}
