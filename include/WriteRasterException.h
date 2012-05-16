#ifndef ALF_WriteRasterException_H
#define ALF_WriteRasterException_H


#include "Poco/Exception.h"
#include <typeinfo>

POCO_DECLARE_EXCEPTION(FrescoFoundation_API, WriteRasterException, Poco::Exception)

POCO_IMPLEMENT_EXCEPTION(WriteRasterException, Poco::Exception, "Error writing raster file")


#endif
