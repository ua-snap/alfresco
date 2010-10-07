#ifndef Fresco_NoVeg_h
#define Fresco_NoVeg_h


#include "Frame.h"


class FrescoFoundation_API NoVeg : public Frame 
//This frame is only a place holder at this time, but at some point in the future, could become a realistic
//frame.  It holds the place for a default frame when errors occur and could specify a true barrier to
//fire/seed dispersal at some point in the future.  As a result, it has no unique variables or functionality.
{
public:
							NoVeg(const int& rAge=0, 
						        const bool& rIsTopoComplex=false, 
                                const float& rSite=0., 
                                const int& rYearOfLastBurn=-1, 
								const int& rLastBurnSeverity=0,
                                const float& rFireIgnitionFactor=0., 
                                const float& rFireSensitivity=0.,  
                                const Species& rSpeciesSubCanopy=gNoVegID);
							NoVeg(const Frame& rFrame);
							~NoVeg() {}
	static void				clear();
	Frame*					success (Landscape* pParent) { return NULL; }
	const unsigned char		type () const { return gNoVegID; }
	const float				getHumanIgnitionProb() const { return _humanIgnitionsProb; }
private:
	float					getFireProb (const Landscape* pLandscape) { return 0.; }
	void					historicalFire (Landscape* pParent) { return; }
};


#endif
