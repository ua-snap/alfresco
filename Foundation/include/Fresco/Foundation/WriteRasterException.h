#ifndef ALF_WriteRasterException_H
#define ALF_WriteRasterException_H


#include "PreCompiled.h"
#include "Poco/Exception.h" 


class FrescoFoundation_API WriteRasterException: public Poco::Exception
{																				
public:
	WriteRasterException(int code = 0);
	WriteRasterException(const std::string& msg, int code = 0);
	WriteRasterException(const std::string& msg, const std::string& arg, int code = 0);
	WriteRasterException(const std::string& msg, const Poco::Exception& exc, int code = 0);
	WriteRasterException(const WriteRasterException& exc);
	~WriteRasterException() throw();
	WriteRasterException& operator = (const WriteRasterException& exc);
	const char* name() const throw();
	const char* className() const throw();
	Poco::Exception* clone() const;
	void rethrow() const;
};

#endif
