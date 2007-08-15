
#ifndef FRESCO_CUSTOM_INCLUDED
    #define FRESCO_CUSTOM_INCLUDED
    //
	// Set FrescoCustom_API.
	//
    // For Windows: set to import/export status for Windows dll. 
	// FRESCO_CUSTOM_EXPORTS must be defined in the library's project 
	// settings.  Any other project that makes use of this library must 
	// NOT define FRESCO_CUSTOM_EXPORTS.
	// 
	// For Unix platforms: no action required.  Just let code below
	// define FrescoCustom_API as blank.
    //
    #include "Poco/Poco.h" // get system specific stuff
    #if defined(_WIN32) && defined(POCO_DLL)
	    #if defined(FRESCO_CUSTOM_EXPORTS)
	    	#define FrescoCustom_API __declspec(dllexport)
	    #else
	    	#define FrescoCustom_API __declspec(dllimport)
	    #endif
    #endif
    #if !defined(FrescoCustom_API)
    	#define FrescoCustom_API
    #endif
#endif  //FRESCO_CUSTOM_INCLUDED
