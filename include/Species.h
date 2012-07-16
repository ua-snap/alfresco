#ifndef Fresco_Species_h
#define Fresco_Species_h

#include "PreCompiled.h"
#include <iostream>


typedef unsigned char byte;


class FrescoFoundation_API Species 
// This class is a simple base class that behaves like a basic type.  It allows the user to
// set/store species type as an enumeration.  It overloads the increment and decrement operators, 
// and provides a default value to facilitate loop control variables.
{
public :
							Species (byte spec = 0) { _species = spec; }							    //  Starting at zero means we are always starting at the beginning regardless of what species type it is
							~Species () {}
	Species& operator		= (const Species& spec) { _species = spec._species; return *this; }	    // Assignment operator
	Species& operator		= (const byte spec)	{ _species = spec; return *this; }				    // Assignment operator
	bool operator			==(const byte spec)	{ return _species == spec; } 						// Equality operator
	bool operator			!=(const byte spec)	{ return _species != spec; }						// Inequality operator
	bool operator			>(const byte spec)	{ return _species > spec; }						    // Greater than operator
	bool operator			>=(const byte spec)	{ return _species >= spec; }						// Greater than or equal operator
	bool operator			<(const byte spec)	{ return _species < spec; }						    // Less than operator
	bool operator			<=(const byte spec)	{ return _species <= spec; }						// Less than or equal operator
	Species& operator		++()				{ _species++; return *this; }						// Prefix increment
	Species operator		++(int)				{ byte specTmp = _species; _species++;	return Species(specTmp); }	// Postfix increment
	Species& operator		--()				{ _species--; return *this; }						// Prefix decrement
	Species operator		--(int)				{ byte specTmp = _species; _species--;	return Species(specTmp); }	// Postfix decrement
	operator				int()				{ return (int)_species; } 							// int typecast operator
	operator				byte()				{ return _species; }								// byte typecast operator
    friend std::ostream&	operator<<(std::ostream& s, const Species& rSpecies) { s << rSpecies._species << std::flush; return s; }	// Output operator
private :
	byte			_species;
};


#endif
