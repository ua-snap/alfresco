#ifndef ALF_ReadRasterException_H
#define ALF_ReadRasterException_H


#include "Poco/Exception.h"
#include <typeinfo>

POCO_DECLARE_EXCEPTION(FrescoFoundation_API, ReadRasterException, Poco::Exception)

POCO_IMPLEMENT_EXCEPTION(ReadRasterException, Poco::Exception, "Error reading raster file")


#endif
