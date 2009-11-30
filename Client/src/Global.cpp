

#include "Fresco/Client/Global.h"
#include "Poco/File.h"
#include "Poco/Mutex.h"
#include <fstream>
#include <iostream>
#include <sstream>


//Declare static private members
Poco::Mutex     Global::_singletonMutex;
Poco::Mutex     Global::_localLogMutex;
Global*         Global::_instance = 0;


Global::            Global(void) : isDebugOn(false), version("1.0.7")
//Global is a singleton.  Keep this a private contructor and use getInstance().
{ 
}

Global::            ~Global(void)
{
}

Global* Global::    getInstance(void)
//Ensure only one instance of the Global class is made.
{
    Poco::ScopedLock<Poco::Mutex> lock(_singletonMutex);
	if (_instance == 0) _instance = new Global();
    return _instance;
}

void Global::       writeDebug(const std::string messege)
{
   if (Global::getInstance()->isDebugOn)
        std::cout << "DEBUG: " << messege << std::endl;
}

void Global::       writeToLocalLog(const std::string message)
{
    if (Global::getInstance()->isDebugOn)
    {
        Poco::ScopedLock<Poco::Mutex> lock(_localLogMutex);    
        static bool isFirstCall = true;
        std::ios_base::openmode mode = std::ios_base::out | std::ios_base::app;
    
        //Ensure log file exists.
        Poco::File f("./frescocLog-" + Global::getInstance()->localHostName + ".txt");
        if (isFirstCall) {
            //Ensure log is emptied.
            if (f.exists()) {
                //Clear the file.
                f.remove(false);
                f.createFile();
            }
            else {
                f.createFile();
            }
            mode = std::ios_base::out;
            isFirstCall = false; //future calls will append.
        }
        else {
            if (!f.exists() | !f.canWrite()){
                return;
            }
            //File already created, we enable append mode.
            mode = std::ios_base::out | std::ios_base::app;
        }
        
        //Open log and write message.
        std::ofstream out(f.path().c_str(), mode);
        if (out.good()) {
            out.write(message.c_str(), message.length());
        }
        out.close();
    }
}

std::string Global::toString(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
