#ifndef Fresco_TemperateRainforest_h
#define Fresco_TemperateRainforest_h

#include "Frame.h"
#include "PreCompiled.h"

class FrescoFoundation_API TemperateRainforest : public Frame
/*
 * Intended primarily as a placeholder currently.  Temperate Rainforest has an 
 * extremely low likelihood of burn, so at this time, the class is intended
 * as a way to track this veg type, and little else.
 */
{
public:
    TemperateRainforest(const int& rAge=0,
          const bool& rIsTopoComplex=false,
          const float& rSite=0.,
          const int& rYearOfLastBurn=-1,
          const int& rLastBurnSeverity=0,
          const float& rFireIgnitionFactor=0.,
          const float& rFireSensitivity=0.,
          const Species& rSpeciesSubCanopy=gTemperateRainforestID);
    TemperateRainforest(const Frame& rFrame);
    ~TemperateRainforest() {}
    static void				clear();
    Frame*					success (Landscape* pParent) {
        return NULL;
    }
    const unsigned char		type () const {
        return gTemperateRainforestID;
    }
    const float				getHumanIgnitionProb() const {
        return _humanIgnitionsProb;
    }
private:
    float					getFireProb (const Landscape* pLandscape) {
        return 0.;
    }
    void					historicalFire (Landscape* pParent) {
        return;
    }
};


#endif
