#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/ReadRasterException.h"
#include "Poco/Exception.h" 
#include <typeinfo>

ReadRasterException::ReadRasterException(int code): Poco::Exception(code)																	
{																								
}																								
ReadRasterException::ReadRasterException(const std::string& msg, int code): Poco::Exception(msg, code)										
{																								
}																								
ReadRasterException::ReadRasterException(const std::string& msg, const std::string& arg, int code): Poco::Exception(msg, arg, code)		
{																								
}																								
ReadRasterException::ReadRasterException(const std::string& msg, const Poco::Exception& exc, int code): Poco::Exception(msg, exc, code)	
{																								
}																								
ReadRasterException::ReadRasterException(const ReadRasterException& exc): Poco::Exception(exc)																
{																								
}																								
ReadRasterException::~ReadRasterException() throw()																				
{																								
}																								
ReadRasterException& ReadRasterException::operator = (const ReadRasterException& exc)															
{																								
	Poco::Exception::operator = (exc);																		
	return *this;																				
}																								
const char* ReadRasterException::name() const throw()															
{																								
	return "Error reading raster file";																				
}																								
const char* ReadRasterException::className() const throw()														
{																								
	return typeid(*this).name();																
}																								
Poco::Exception* ReadRasterException::clone() const																
{																								
	return new ReadRasterException(*this);																		
}																								
void ReadRasterException::rethrow() const																		
{																								
	throw *this;																				
}
