//CustomFresco.cpp


#include "CustomFresco.h"
#include "CustomLandscape.h"
#include "BarrenLichenMoss.h"
#include "TemperateRainforest.h"
#include "BSpruce.h"
#include "Decid.h"
#include "Tundra.h"
#include "ShrubTundra.h"
#include "GraminoidTundra.h"
#include "WetlandTundra.h"
#include "WSpruce.h"
#include "Grassland.h"
#include "Fresco.h"
#include "Landscape.h"
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
	gBSpruceID = (byte)fif().root["Vegetation"]["BSpruce"]["id"].asInt();
	gWSpruceID = (byte)fif().root["Vegetation"]["WSpruce"]["id"].asInt();
	gDecidID   = (byte)fif().root["Vegetation"]["Decid"]["id"].asInt(); 
	gShrubTundraID  = (byte)fif().root["Vegetation"]["ShrubTundra"]["id"].asInt();
	gGraminoidTundraID  = (byte)fif().root["Vegetation"]["GraminoidTundra"]["id"].asInt();
	gWetlandTundraID  = (byte)fif().root["Vegetation"]["WetlandTundra"]["id"].asInt();


	validVegTypes.insert(gBSpruceID);
	validVegTypes.insert(gWSpruceID);
	validVegTypes.insert(gDecidID);
	validVegTypes.insert(gShrubTundraID);
	validVegTypes.insert(gGraminoidTundraID);
	validVegTypes.insert(gWetlandTundraID);
	validVegTypes.insert(gNoVegID);

	// Handle Grassland here for backwards compatibility.
	if (fif().CheckKey(fif().root["Vegetation"]["Grassland"]["id"]))
	{
		gGrasslandID = (byte)fif().root["Vegetation"]["Grassland"]["id"].asInt();
		validVegTypes.insert(gGrasslandID);
		Grassland::setStaticData();
	}
	if (fif().CheckKey(fif().root["Vegetation"]["Tundra"]["id"])) 
	{
		gTundraID = (byte)fif().root["Vegetation"]["Tundra"]["id"].asInt();
		validVegTypes.insert(gTundraID);
		Tundra::setStaticData();
	}
	if (fif().CheckKey(fif().root["Vegetation"]["BarrenLichenMoss"]["id"]))
	{
		gBarrenLichenMossID = (byte)fif().root["Vegetation"]["BarrenLichenMoss"]["id"].asInt();
		validVegTypes.insert(gBarrenLichenMossID);
	//	BarrenLichenMoss::setStaticData();
	}
	if (fif().CheckKey(fif().root["Vegetation"]["TemperateRainforest"]["id"]))
	{
		gTemperateRainforestID = (byte)fif().root["Vegetation"]["TemperateRainforest"]["id"].asInt();
		validVegTypes.insert(gTemperateRainforestID);
	//	TemperateRainforest::setStaticData();
	}

	BSpruce::setStaticData();
	WSpruce::setStaticData();
	Decid::setStaticData();
	ShrubTundra::setStaticData();
	GraminoidTundra::setStaticData();
	WetlandTundra::setStaticData();
	gNumSpecies = (int)validVegTypes.size();
}
