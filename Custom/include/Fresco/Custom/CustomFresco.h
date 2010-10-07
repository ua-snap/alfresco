#ifndef Fresco_CustomFresco_h
#define Fresco_CustomFresco_h


#include "Fresco/Custom/Custom.h"
#include "Fresco/Custom/CustomLandscape.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/Landscape.h"
#include <set>

class FrescoCustom_API CustomFresco : public Fresco
{
//Data
public:
	static std::set<unsigned char>	validVegTypes;

//Functionality
public:
						CustomFresco(bool isDebugOn);
						~CustomFresco();
    virtual void		customSetup();
	static const bool	isValidVegType(const int vegType);
};

inline const bool		CustomFresco::isValidVegType(const int vegType)
{
	return CustomFresco::validVegTypes.find(vegType) != CustomFresco::validVegTypes.end();
}

#endif
