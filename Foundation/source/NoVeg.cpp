// NoVeg.cpp


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/NoVeg.h"


NoVeg::NoVeg (const Frame& rFrame) 
    : Frame(rFrame)
{
	_yearEstablished         = gYear;
	_yearFrameEstablished	= gYear;
}


NoVeg::NoVeg (const int& rAge, 
              const float& rSite, 
              const int& rLastBurned, 
              const float& rFireIgnitionFactor, 
              const float& rFireSensitivity,  
              const Species& rSpeciesSubCanopy) 
        : Frame (rAge, rSite, rLastBurned, rFireIgnitionFactor, rFireSensitivity, rSpeciesSubCanopy) 
{
} 


void NoVeg::clear()
//Clear existing run if any and return to before a run is specified.
{
}
