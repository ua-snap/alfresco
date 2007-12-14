#ifndef Fresco_Frame_h
#define Fresco_Frame_h


#include "Climate.h"
#include "Fire.h"
#include "Species.h"


class Species;


class FrescoFoundation_API Frame : public Fire 
//The Frame class is really a intermediary between the parent classes specifying the functionality of
//the frame class and the child classes, which define the specific functionality of each fram type. It
//is necessary so that it is possible to have a common pointer which points to any child type, while
//still maintaining the same functionality for each frame type.  As a result, it has very little inherent
//functionality other than constructors and destructors.
{
//Data
public:
    static void             setOutFlagsForAllFrames(const int newOutFlags) { _outFlags = newOutFlags; }
	virtual const int		type() const = 0;										        //Force the class to declare what type it is when asked
	const int				age() const { return gYear - _yearEstablished; }				//Report the age - used for statistical purposes
	const int				frameAge() const { return gYear - _yearFrameEstablished; }	    //Report the frame age or residence time
    Species                 speciesSubCanopy() const { return _speciesSubCanopy; }
    const float             site() const { return _site; }
	const bool				hasComplexTopo() const { return _isTopoComplex; }
protected:
	static int				_outFlags;												        //A flag indicating which data to output when the object is written
	int						_yearEstablished;												//The year this stand was established
	int						_yearFrameEstablished;											//The year this frame type was established - i.e. residence times
	float					_site;													        //A surrogate variable to describe the site characteristics (currently just wet/dry)
	bool					_isTopoComplex;													//Topography is categorized as either flat or complex (hilly/mountainous)
	Species				    _speciesSubCanopy;											    //Store the type of the subcanopy if there is one. Should this be a linked list so we could potentially store more than one subcanopy.  Maybe with a percent cover type thing.  Maybe for a later implementation!?

//Functions
public:
							Frame(const int& rAge=0, 
								const bool& rIsTopoComplex=false,
								const float& rSite=0., 
                                const int& rYearOfLastBurn=-1, 
								const int& rLastBurnSeverity=0,
                                const float& rFireIgnitionFactor=0., 
                                const float& rFireSensitivity=0., 
                                const Species& rSpeciesSubCanopy=0);
							Frame(const Frame& rFrame);
	virtual					~Frame() {};
    static void				clear();
	virtual Frame*			success (Landscape* pParent) = 0;								    //Each child class needs to have it's own successional routine
	virtual double			queryReply(Landscape *Parent, const double Weight, const double *Parms = NULL) { return 0.; }	//Supply a general query function for intercell communitcation, parameter can pass data and/or a flag to indicate what type of query is being performed
	virtual void			writeData (std::ostream &s) const;							    //Children can output their data when requested if they implement it
	friend					std::ostream& operator<< (std::ostream& s, const Frame& rFram);	//Output the class to a stream - uses the Landscape flags
};


#endif
