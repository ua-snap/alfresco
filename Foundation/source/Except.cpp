//Except.cpp


#include "Fresco/Foundation/Except.h"
#include "Fresco/Foundation/Fresco.h"


void Exception::print()
{
	std::string output;
	output = "Error: ";
	if (!message.empty())	output += message;
	if (!specifier.empty())	output += " - \"" + specifier + "\"";
	output += "\n";
	FRESCO->outputError(output);
}
