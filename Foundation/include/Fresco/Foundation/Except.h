#ifndef Fresco_Except_h
#define Fresco_Except_h


#include "PreCompiled.h"
#include <string>


class FrescoFoundation_API Exception 
//The Exception class handles [very] basic exception handling.  Since most of the errors encountered in the code can be handled
//in some default way, and are non-fatal, the only thing we really need to do here is print the error.  It is conceivable that
//as the error handling demand increases other functions could be added.  For example one to retreive the error code in cases
//where multiple errors could occur and would be handled differently.
{
public :
	// A semi complete list of errors that could be encountered
	enum ErrorCause 
	{
		UNKNOWN		= 0x00,				
		INITFAULT		= 0x01,
		BADYEAR			= 0x02,
		BADREP			= 0x03,
		BADINDEX		= 0x04,
		BADARRAYDIM		= 0x05,
		FILENULL		= 0x10,
		FILEBAD			= 0x11,
		FILEEOF			= 0x12,
		BADKEY			= 0x20,
		KEYDUP			= 0x21,
		BADARRAYSIZE	= 0x22,
		BADVARTYPE		= 0x23
	};

 	ErrorCause	    state;			//The error state as defined in enumState
    std::string		message;		//The message
	std::string		specifier;		//A specifier that might accompany the message, for example a filename.  Really just another pointer to string.

public :
    Exception (const ErrorCause S=UNKNOWN, const std::string M="", const std::string Spec="") : state(S), message(M), specifier(Spec) {}
	~Exception () {}
	void print ();
};


#endif
