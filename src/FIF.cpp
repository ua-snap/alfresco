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
bool FIF::CheckKey(Json::Value& n){
	return !n.isNull();
}
int FIF::pnGet(Json::Value& n, int*& ptr){
	ptr = new int[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asInt();
	}
	return n.size();
}
int FIF::psGet(Json::Value& n, std::string *&ptr){
	ptr = new std::string[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asString();
	}
	return n.size();
}
int FIF::pdGet(Json::Value& n, double*& ptr){
	ptr = new double[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asDouble();
	}
	return n.size();
}
int FIF::pbGet(Json::Value& n, bool*& ptr){
	ptr = new bool[n.size()];
	for (int i = 0; i < n.size(); i++){
		ptr[i] = n[i].asBool();
	}
	return n.size();
}
