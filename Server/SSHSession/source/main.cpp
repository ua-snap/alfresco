#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Path.h"
#include "Poco/Thread.h"
#include <iostream>
#include <fstream>
#include <string>

void applyConsoleInput()
{
	std::string input = "";
	bool running = true;
    while (running) {
		if (std::cin.good()) {
            std::cin >> input;
            if (input == "exit" || input == "quit") {
                running = false;
            }
            input = "";
        } else { Poco::Thread::sleep(5000); }
        Poco::Thread::sleep(100);
	}
}

int main(int argc, char** argv) {
	std::cout << "SSH Test App " << std::endl;
	bool hadError = false;
    std::string errorMessage = "";
	try {
		applyConsoleInput();
    } catch (Poco::Exception& e) {  hadError=true;  errorMessage = "Error: " + e.displayText();
    } catch(...) {                  hadError=true;  errorMessage = "Error: Could not complete remote command: ."; }
    if (hadError) {
        std::cout << std::endl << errorMessage << std::endl;
    }
}