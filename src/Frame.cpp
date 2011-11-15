//Frame.cpp


#include "PreCompiled.h"
#include "Frame.h"
#include "RasterIO.h"
#include "Poco/Exception.h"

Frame::Frame(const int& rAge, 
			 const bool& rIsTopoComplex,
             const float& rSite, 
             const int& rYearOfLastBurn,
			 const int& rLastBurnSeverity,
             const float& rFireIgnitionFactor, 
             const float& rFireSensitivity, 
             const Species& rSpeciesSubCanopy)
    : Fire(rYearOfLastBurn, rLastBurnSeverity, rFireIgnitionFactor, rFireSensitivity), _yearEstablished(rAge), _yearFrameEstablished(rAge), _site(rSite), _isTopoComplex(rIsTopoComplex), _speciesSubCanopy(rSpeciesSubCanopy)
{
	if (type() != gNoVegID)
	{
		std::string tailMsg =  " Nodata values should only be used where vegetation is set to the NoVeg ID ("+ToS(gNoVegID)+").";
		if (IsNodata(rAge))
			throw Poco::Exception("invalid use of nodata value in age." + tailMsg);
		if (IsNodata(rSite))
			throw Poco::Exception("invalid use of nodata value in site." + tailMsg);
		if (!(rLastBurnSeverity > -1 && rLastBurnSeverity < 6))
			throw Poco::Exception("invalid burn severity class ID used ("+ToS(rLastBurnSeverity)+"). Valid burn severity class IDs are 0 through 5.");
		if (IsNodata(rFireIgnitionFactor))
			throw Poco::Exception("invalid use of nodata value in fire ignition factor." + tailMsg);
		if (IsNodata(rFireSensitivity))
			throw Poco::Exception("invalid use of nodata value in fire sensitivity." + tailMsg);
		//if (bnd == (int)rSpeciesSubCanopy)
		//	throw Poco::Exception("invalid use of nodata value in species sub canopy." + tailMsg);
	}
}


//Frame::Frame(const Frame &Frame, const int &nAge) : 
Frame::Frame(const Frame &Frame) 
    : Fire(Frame), _yearEstablished(Frame._yearEstablished), _yearFrameEstablished(Frame._yearFrameEstablished), _site(Frame._site), _isTopoComplex(Frame._isTopoComplex),  _speciesSubCanopy(Frame.type())
{ 
}


void Frame::clear()
{
}


unsigned char	Frame::getAsByte(RasterIO::ALFMapType mapType)
{
	throw Poco::Exception("This frame type ("+ToS(type())+") does not support the map type ("+ RasterIO::getMapTypeAsString(mapType) + ")");
}
float			Frame::getAsFloat(RasterIO::ALFMapType mapType)
{
	throw Poco::Exception("This frame type ("+ToS(type())+") does not support the map type ("+ RasterIO::getMapTypeAsString(mapType) + ")");
}
