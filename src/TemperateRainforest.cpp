// TemperateRainforest.cpp


#include "PreCompiled.h"
#include "TemperateRainforest.h"


TemperateRainforest::TemperateRainforest (const Frame& rFrame)
    : Frame(rFrame)
{
    _yearEstablished         = gYear;
    _yearFrameEstablished	= gYear;
}


TemperateRainforest::TemperateRainforest (const int& rAge,
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


void TemperateRainforest::clear()
//Clear existing run if any and return to before a run is specified.
{
}
