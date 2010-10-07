#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/WriteRasterException.h"
#include "Poco/Exception.h" 
#include <typeinfo>

WriteRasterException::WriteRasterException(int code): Poco::Exception(code)																	
{																								
}																								
WriteRasterException::WriteRasterException(const std::string& msg, int code): Poco::Exception(msg, code)										
{																								
}																								
WriteRasterException::WriteRasterException(const std::string& msg, const std::string& arg, int code): Poco::Exception(msg, arg, code)		
{																								
}																								
WriteRasterException::WriteRasterException(const std::string& msg, const Poco::Exception& exc, int code): Poco::Exception(msg, exc, code)	
{																								
}																								
WriteRasterException::WriteRasterException(const WriteRasterException& exc): Poco::Exception(exc)																
{																								
}																								
WriteRasterException::~WriteRasterException() throw()																				
{																								
}																								
WriteRasterException& WriteRasterException::operator = (const WriteRasterException& exc)															
{																								
	Poco::Exception::operator = (exc);																		
	return *this;																				
}																								
const char* WriteRasterException::name() const throw()															
{																								
	return "Error writing raster file";
}																								
const char* WriteRasterException::className() const throw()														
{																								
	return typeid(*this).name();																
}																								
Poco::Exception* WriteRasterException::clone() const																
{																								
	return new WriteRasterException(*this);																		
}																								
void WriteRasterException::rethrow() const																		
{																								
	throw *this;																				
}
