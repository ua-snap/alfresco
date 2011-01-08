#ifndef ALF_ReadRasterException_H
#define ALF_ReadRasterException_H

#include "Poco/Exception.h"

POCO_DECLARE_EXCEPTION(FrescoFoundation_API, ReadRasterException, Poco::Exception)

POCO_IMPLEMENT_EXCEPTION(ReadRasterException, Poco::Exception, "Error reading raster file")


//
//
//#include "PreCompiled.h"
//#include "Poco/Exception.h" 
//
//
//class FrescoFoundation_API ReadRasterException: public Poco::Exception
//{																				
//public:																			
//	ReadRasterException(int code = 0);															
//	ReadRasterException(const std::string& msg, int code = 0);									
//	ReadRasterException(const std::string& msg, const std::string& arg, int code = 0);			
//	ReadRasterException(const std::string& msg, const Poco::Exception& exc, int code = 0);		
//	ReadRasterException(const ReadRasterException& exc);														
//	~ReadRasterException() throw();																
//	ReadRasterException& operator = (const ReadRasterException& exc);											
//	const char* name() const throw();											
//	const char* className() const throw();										
//	Poco::Exception* clone() const;												
//	void rethrow() const;														
//};

#endif
