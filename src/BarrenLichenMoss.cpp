// BarrenLichenMoss.cpp

#include "BarrenLichenMoss.h"

BarrenLichenMoss::BarrenLichenMoss (const Frame& rFrame)
    : Frame(rFrame)
{
    _yearEstablished         = gYear;
    _yearFrameEstablished	= gYear;
}


BarrenLichenMoss::BarrenLichenMoss (const int& rAge,
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


void BarrenLichenMoss::clear()
//Clear existing run if any and return to before a run is specified.
{
}
