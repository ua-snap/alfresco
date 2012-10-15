// CoastalRainforest.cpp


#include "PreCompiled.h"
#include "CoastalRainforest.h"


CoastalRainforest::CoastalRainforest (const Frame& rFrame)
    : Frame(rFrame)
{
    _yearEstablished         = gYear;
    _yearFrameEstablished	= gYear;
}


CoastalRainforest::CoastalRainforest (const int& rAge,
              const bool& rIsTopoComplex,
              const float& rSite,
              const int& rYearOfLastBurn,
              const int& rLastBurnSeverity,
              const float& rFireIgnitionFactor,
              const float& rFireSensitivity,
              const Species& rSpeciesSubCanopy)
    : Frame (rAge, rIsTopoComplex, rSite, rYearOfLastBurn, rLastBurnSeverity, rFireIgnitionFactor, rFireSensitivity, rSpeciesSubCanopy)
{
}


void CoastalRainforest::clear()
//Clear existing run if any and return to before a run is specified.
{
}
