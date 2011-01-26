#ifndef Fresco_Interface_h
#define Fresco_Interface_h
// Provide a comprehensive include list so anything that includes CInterface also includes the relevent library includes
// this means than any file that includes CInterface in it's include chain won't need to include any of these files.

#pragma warning( disable : 4305 4244 )						// Disable the warnings concerning conversion of doubles to floats

#include "PreCompiled.h"
#include "Except.h"										    // Include the rudimentary error handling capabilities

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define PI 3.1415926535										// Define PI
#define BUFSIZE 512											// Define the default buffer size

//using namespace std;

// Advanced declarations
class CInterface;

// Global variables and constants
const int MAXSTRINGLENGTH = 255;							// The maximum string length -- used for filenames at this time
//extern CInterface _fif;									// Global IO interface storage object defined in main program
extern int FrescoFoundation_API     gYear;											// Global variable storing the current year of execution
extern int FrescoFoundation_API     gRep;											// Global variable storing the current rep
extern float FrescoFoundation_API   gCellSize;									// Global variable storing the cellsize

// Define the output codes.  The resulting flag will be a sum of the relevent flags. Note that these are different
// than the codes specified in stat.h.  Those are the basal flags for each statistical variable specified at program
// initiation, these are the specific output flags used in the main program to specify particular output.
//enum outCode {
//// General codes - if not specified, nothing will get printed  
//	outData					= 0x1,		// Output the data
//    outYearStats			= 0x2,		// Output the statistical data by year
//	outRepStats				= 0x4,		// Output the statistical data by rep
//// Format codes - specify the format of the output data and headers
//	outFormat				= 0x10,		// Output the data so it is conducive to read it in into a stats package for analysis
//	outHeader				= 0x20,		// Tack a header onto the output with the number of years/reps for reference purposes
//	outARC					= 0x40,		// Attach an header to the output file to facilitate import into ARCgis
//// Data codes - used with outData
//	outVeg					= 0x100,	// The veg type
//	outAge					= 0x200,	// The stand age
//	outSite					= 0x400,	// The site variables
//	outSub					= 0x800,	// The subcanopy
//	outFireAge				= 0x1000,	// The time since the cell last burned
//	outTemp					= 0x2000,	// The temp offset
//	outPrecip				= 0x4000,	// The precip offset
//// Frame codes - used with outFrame
//	out1					= 0x10000,	// The exact data output is determined by the implementation of the frame class
//	out2					= 0x20000,
//	out3					= 0x40000,
//	out4					= 0x80000,
//};


// CVariable is a variable data type class - in other words, it accepts data of many differente types and will store
// that data as needed.  It has a number of constructors which are used to create an object of this class with a bit
// of data, It also allows the user to get data of some type back from the associative array.  It remembers the size
// of arrays, and the type of data so type conversions can be used where applicable.

// Declare the known types globally so they can be accessed by both CVariable and CInterface
enum Tag {UNKNOWN, nTag, pnTag, dTag, pdTag, bTag, pbTag, sTag, psTag};

class FrescoFoundation_API CVariable {											// A managed union for storing the data of variable type
private:
	const union {
		int n;												// Integers
		int *pn;											// Arrays of integers
		double d;											// Reals
		double *pd;											// Arrays of reals
		bool b;												// Boolean
		bool *pb;											// Array of booleans
		char *s;											// Null terminated string
		char *const*ps;										// Array of null terminated strings
	};

	int nSize;												// The size of an array.
	Tag tag;												// The type of data stored

public:
	CVariable() : n(0) { tag = nTag; nSize = 1; }
	CVariable(const int nVar) : n(nVar) { tag = nTag; nSize = 1; }
	CVariable(int *const pnVar, const int Size) : pn(pnVar) { tag = pnTag; nSize = Size; }
	CVariable(const double dVar) : d(dVar) { tag = dTag; nSize = 1; }
	CVariable(double *const pdVar, const int Size) : pd(pdVar) { tag = pdTag; nSize = Size; }
	CVariable(const bool bVar) : b(bVar) { tag = bTag; nSize = 1; }
	CVariable(bool *const pbVar, const int Size) : pb(pbVar) { tag = pbTag; nSize = Size; }
	CVariable(char *const sVar) : s(sVar) { tag = sTag; nSize = (int)strlen(sVar); }
	CVariable(char *const*psVar, const int Size) : ps(psVar) { tag = psTag; nSize = Size; }
	~CVariable ();

	const int nVal() const;									// Check tag and return some value for each of these i.e. use type casting if necessary
	const int pnVal(const int *&plArg) const;				// Return the size of the array
	const double dVal() const;
	const int pdVal(const double *&pdArg) const;			// Return the size of the array
	const bool bVal() const;
	const int pbVal(const bool *&pbArg) const;				// Return the size of the array
	const char *sVal() const;
	const int psVal(char *const*&psArg) const;				// Return the size of the array
	friend std::ostream& operator<< (std::ostream &s, const CVariable &varOut);	// Output the class to a stream
};

// CAssoc is an implementation of an associative array.  It takes data in the form of a CVariable and adds it to the 
// data structure and associates a string key with the data.  When requested, it searches through the data to find
// the string key and returns the data in CVariable.  At this time, the data structure is a simple linked list.  At
// some point, it would make sense to make this an AVL tree or some other such efficient dynamic data structure.  There
// is also provisions for printing out the whole data structure for debugging purposes.  Error handling is used in the 
// event the key isn't found by throwing an error of type Exception.

class FrescoFoundation_API CAssoc {												// An associative array class for managing string table. A linked list for first implementation, but should be made an AVL tree.
public:
// Types
	enum AVLRes { OK, BALANCE };							// Result of an AVL insertion
	enum AVLSkew { LEFT, RIGHT, NONE };						// Possible skew states

private:
// Types
	struct typeEntry {
		char *pszKey;										// Hold the key (name) of this entry
		const CVariable *varData;							// Holds the data for this entry
		AVLSkew nSkew;										// Relative heights of left and right children (positive indicates left > right)
		typeEntry *pLeft, *pRight;							// Pointers to the two children

		typeEntry (char * Key, const CVariable *const Data) : pszKey(Key), varData(Data) { nSkew = NONE; pLeft = pRight = NULL; }	// A default constructor for creating the constant objects
	};

// Variables
	typeEntry *Head;

// Functions
	void AVLRotLeft(typeEntry *&pn);						// Rotate the AVL tree to the left
	void AVLRotRight(typeEntry *&pn);						// Rotate the AVL tree to the right
	AVLRes AVLGrownLeft(typeEntry *&pn);					// Check that the tree is balanced as the left side has grown
	AVLRes AVLGrownRight(typeEntry *&pn);					// Check that the tree is balanced as the right side has grown
	AVLRes AVLPut (typeEntry *&pNode, typeEntry *const pInsert); // Add an element (recursively) to the AVL tree
	const CVariable *AVLGet (const typeEntry *const pNode, const char *Key) const;	// Retreive an element (recursively) from the AVL tree
	void AVLPrint (const typeEntry *const pNode, int depth, std::ostream &s) const;	// Print out the entire tree to the specified stream
	void AVLDestroy(const typeEntry *const &pNode);			// Delete the whole tree

public:
	CAssoc () { Head = NULL; }
    ~CAssoc () { AVLDestroy(Head); delete Head; Head = NULL; }	// Need to destroy the tree
    
	void Put (char *Key, const CVariable *Data) { AVLPut (Head, new typeEntry(Key, Data)); }	// Store a bit of data in the tree
	const CVariable *Get (const char *Key) { return AVLGet(Head, Key); }	// Retreive some data from the tree - returns NULL if it isn't found
	friend std::ostream& operator<< (std::ostream &s, const CAssoc &assocOut) { assocOut.AVLPrint (assocOut.Head, 0, s); return s; }	// Output the class to a stream
};

// CInterface is described in some detail in the introduction to CInterface.cpp.  The basic idea behind
// this class is to provide for all (most) of the I/O which needs to be done in the model.  The largest
// portion of this job is interfacing with the configuration file.  There is (will be) a parser included
// which reads in the data from the file and stores it in the CAssoc.  There are also functions provided
// for recovering data from the associative array of various types.

class FrescoFoundation_API CInterface {											// The general interface class for all IO esp. .fif files
private:
// Variables
	CAssoc IOAssoc;											// The data array...
	std::string _fifName;

// Functions
	void ParseFile (const char *pszFileName);				// Parse the entire FIF file and add the data to the associated array
	int ParseLine (char *pszLine, char Words[][BUFSIZE], int nLen);	// Parses the next line - returns the number of words in the line
	int GetWord (char pcDest[], const char *pcSource, const int nLen);	// Make a copy of the next word in the line - equivalent to strncpy, but allocates memory and appends a null character
	Tag CheckType (const char *pcWord);						// Attempts to identify the type of the word
	Tag CheckNumber (const char *pcWord);					// Checks to see if the word is a well formed number

public:
	CInterface () {};
	~CInterface () {};

// FIF information functions
	const std::string fileName() { return _fifName; };
	void Initialize (const std::string &basePath, const std::string &fileName);				// Load the data from the .fif file (parser)
	const bool CheckKey (const char *Key)				{ if(IOAssoc.Get(Key)) return true; else return false; }	// Check to see if an entry exists
	const int nGet (const char *Key)					
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        int val = 0;
        try { val = varTmp->nVal(); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return val; 
    }
	const int pnGet (const char *Key, const int *&pn)	
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        int count = 0;
        try { count = varTmp->pnVal(pn); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return count; 
    }
	const double dGet (const char *Key)					
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        double val = 0;
        try { val = varTmp->dVal(); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return val; 
    }
	const int pdGet (const char *Key, const double *&pd)
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        int count = 0;
        try { count = varTmp->pdVal(pd); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return count; 
    }
	const bool bGet (const char *Key)					
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        bool val = false;
        try { val = varTmp->bVal(); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return val; 
    }
	const int pbGet (const char *Key, const bool *&pb)	
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        int count;
        try { count = varTmp->pbVal(pb); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return count; 
    }
	const char *sGet (const char *Key)					
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        const char *val;
        try { val = varTmp->sVal(); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return val; 
    }
	const int psGet (const char *Key, char *const*&ps)	
    { 
        const CVariable *varTmp = IOAssoc.Get(Key); 
        if (!varTmp) throw Exception(Exception::BADKEY,"Key not found: " + std::string(Key)); 
        int count = 0;
        try { count = varTmp->psVal(ps); } catch(Exception& e) { throw Exception(Exception::BADVARTYPE, e.message + ": " + Key); }
        return count; 
    }
};

#endif






//#include <map>
//#include <string>
//#include "Poco/Any.h"
//
//class Value 
//{
//private:
//    Poco::Any val;
//    
//public:
//    Value(Poco::Any* value) { val = value; }
//};
//
//
//class Interface
//{
////Data
//private:
//    std::map<const std::string key, Any value*, std::less<std::string> > fields;
//    template <class T> T get(std::string key) { return AnyCast<T>(fields[key]); };
////Methods
//public:
//    Interface(void);
//    ~Interface(void);
//    operator [] // randomSeed = fif["RandomSeed"];
//};

//#endif



