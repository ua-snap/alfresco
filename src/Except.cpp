//Except.cpp


#include "Except.h"
#include "Fresco.h"


void SimpleException::print()
{
	std::string output;
	output = "Error: ";
	if (!message.empty())	output += message;
	if (!specifier.empty())	output += " - \"" + specifier + "\"";
	output += "\n";
	FRESCO->outputError(output);
}
