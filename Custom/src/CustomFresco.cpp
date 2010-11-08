//CustomFresco.cpp


#include "Fresco/Custom/CustomFresco.h"
#include "Fresco/Custom/CustomLandscape.h"
#include "Fresco/Custom/BSpruce.h"
#include "Fresco/Custom/Decid.h"
#include "Fresco/Custom/Tundra.h"
#include "Fresco/Custom/WSpruce.h"
#include "Fresco/Custom/Grassland.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/Landscape.h"
#include <set>


std::set<unsigned char>	CustomFresco::validVegTypes;

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
    gBSpruceID = (byte)fif().nGet("BSpruce");
    gWSpruceID = (byte)fif().nGet("WSpruce");
    gDecidID   = (byte)fif().nGet("Decid"); 
    gTundraID  = (byte)fif().nGet("Tundra");

	validVegTypes.insert(gBSpruceID);
	validVegTypes.insert(gWSpruceID);
	validVegTypes.insert(gDecidID);
	validVegTypes.insert(gTundraID);
	validVegTypes.insert(gNoVegID);

	// Handle Grassland here for backwards compatibility.
	if (fif().CheckKey("Grassland"))
	{
		gGrasslandID = (byte)fif().nGet("Grassland");
		validVegTypes.insert(gGrasslandID);
		Grassland::setStaticData();
	}

	BSpruce::setStaticData();
    WSpruce::setStaticData();
    Decid::setStaticData();
    Tundra::setStaticData();

	gNumSpecies = (int)validVegTypes.size();
}
