//CustomFresco.cpp


#include "Fresco/Custom/CustomFresco.h"
#include "Fresco/Custom/CustomLandscape.h"
#include "Fresco/Custom/BSpruce.h"
#include "Fresco/Custom/Decid.h"
#include "Fresco/Custom/Tundra.h"
#include "Fresco/Custom/WSpruce.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/Landscape.h"
#include <set>


std::set<int>	CustomFresco::validVegTypes;

CustomFresco::		CustomFresco(bool isDebugOn) 
	: Fresco(new CustomLandscape(), isDebugOn)
{
}

CustomFresco::		~CustomFresco()
{
}

void CustomFresco::	customSetup()
{
    output("Loading Species settings.\n");
    gBSpruceID = fif().nGet("BSpruce");
    gWSpruceID = fif().nGet("WSpruce");
    gDecidID   = fif().nGet("Decid"); 
    gTundraID  = fif().nGet("Tundra");

	validVegTypes.insert(gBSpruceID);
	validVegTypes.insert(gWSpruceID);
	validVegTypes.insert(gDecidID);
	validVegTypes.insert(gTundraID);
	validVegTypes.insert(gNoVegID);

	BSpruce::setStaticData();
    WSpruce::setStaticData();
    Decid::setStaticData();
    Tundra::setStaticData();
}
