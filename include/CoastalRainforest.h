#ifndef Fresco_CoastalRainforest_h
#define Fresco_CoastalRainforest_h


#include "Frame.h"


class FrescoFoundation_API CoastalRainforest : public Frame
/*
 * Intended primarily as a placeholder currently.  Coastal Rainforest has an 
 * extremely low likelihood of burn, so at this time, the class is intended
 * as a way to track this veg type, and little else.
 */
{
public:
    CoastalRainforest(const int& rAge=0,
          const bool& rIsTopoComplex=false,
          const float& rSite=0.,
          const int& rYearOfLastBurn=-1,
          const int& rLastBurnSeverity=0,
          const float& rFireIgnitionFactor=0.,
          const float& rFireSensitivity=0.,
          const Species& rSpeciesSubCanopy=gCoastalRainforestID);
    CoastalRainforest(const Frame& rFrame);
    ~CoastalRainforest() {}
    static void				clear();
    Frame*					success (Landscape* pParent) {
        return NULL;
    }
    const unsigned char		type () const {
        return gCoastalRainforestID;
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
