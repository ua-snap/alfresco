#include "FIF.h"

FIF::FIF(){

}
void FIF::Initialize(std::string path, std::string fifName){
	/* Initialize the object, set input file name, parse JSON file */
	std::ifstream fif(fifName.c_str(), std::ifstream::binary);
	bool parsed = reader.parse(fif, root, false);
	
	if(!parsed) {
		std::cout << "Error parsing configuration file: " << fifName << "\n" << reader.getFormatedErrorMessages() << "\n";
	}
}
std::string FIF::fileName(){
	/* Return the file name */
	return _fileName;
}
bool FIF::CheckKey(Json::Value& n){
	/* Check if configuration key exists */	
	return !n.empty();

}
int FIF::pnGet(Json::Value& n, int*& ptr){
	/* Reference the array for specified 'integer' configuration item to ptr, return length of array */
	ptr = new int[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asInt();
	}
	return n.size();
}
int FIF::psGet(Json::Value& n, std::string *&ptr){
	/* Reference the array for specified 'string' configuration item to ptr, return length of array */
	ptr = new std::string[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asString();
	}
	return n.size();
}
int FIF::pdGet(Json::Value& n, double*& ptr){
	/* Reference the array for specified 'double' configuration item to ptr, return length of array */
	ptr = new double[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asDouble();
	}
	return n.size();
}
int FIF::pbGet(Json::Value& n, bool*& ptr){
	/* Reference the array for specified 'boolean' configuration item to ptr, return length of array */
	ptr = new bool[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asBool();
	}
	return n.size();
}
