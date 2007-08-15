#ifndef Fresco_Landscape_h
#define Fresco_Landscape_h


#include "PreCompiled.h"
#include "Frame.h"
#include "Stat.h"


class Climate;


class FrescoFoundation_API Landscape 
//This provides the basic functionality for the Landscape class. This class was deemed a minimum of interesting stuff
//relevent to the CustomLandscape class.  All the high level functionality pertaining to the class will be provided by 
//a child of this class which is the true Landscape class.  This abstraction was implemented to ease the maintenance
//of multiple versions increasing the amount of code held in common.  The change not only allowed a good portion of 
//the Landscape class to be abstracted to libraries, but also (due to include linkages) Frame, Fire and Climate
//could also be abstracted, yielding an entire second level library for the model.
{
//Data
protected:
	int					_row;                                                   //This class remembers where it is in the array
    int                 _col;												    //This class remembers where it is in the array
	Frame***			_pFrames;												//The landscape array which points to each frame
	Climate*			_pClimate;												//A landscape has a climate.  This also stores the climate history.
	//Stats
	CStat				_fireNumStat;											//Number of ignitions.
	CStat				_fireSizeStat;											//Fire sizes stratified by fire number
	std::vector<CStat>	_fireSpeciesStat;										//Fire sizes stratified by species (will become an array of Species)
	std::vector<CStat>	_fireIntervalStat;										//Fire interval by species (will become an array of Species)
	std::vector<CStat>	_vegDistributionStat;									//Vegetation distribution by species (will become an array of Species)
	std::vector<CStat>	_vegResidenceStat;										//Vegetation residence times by species
private:
    static std::string	_humanIgnitionsFilename;
	int**				_pHumanIgnitions;
	static bool			_cropNeighbors;											//Should we crop the neighbours algorithm to only look circular distance?
	float				_maxFireSizeEventWeight;								//Fire spread probability weight applied when an individual fire grows beyond a specified size.
	static double		_xllCorner;
	static double		_yllCorner;
	double*				_pfireSpreadParams;
	//Stats
	long				_fireNumStatFlags;
	long				_fireSizeStatFlags;
	long				_fireSpeciesStatFlags;
	long				_fireIntervalStatFlags;
	long				_vegDistributionStatFlags;
	long				_vegResidenceStatFlags;


//Functions
public:
						Landscape(const int rows=-1, const int cols=-1);
	virtual				~Landscape();
	virtual void		clear();												//Clear a run.
	virtual void		setup();												//setup a run.
	virtual void		runEnd();												//Post process the run.
	virtual void		repStart();												//Initialize a rep
	virtual void		repEnd();												//Post process the rep.
	virtual void		yearStart();											//Initialize a year.
	virtual void		yearEnd();												//Post process the year.
	virtual void		succession();											//Calls Frame::succession for each frame
	virtual void		doIgnitions();											//Ignite the landscape.
	double				neighborsSuccess(                                       //Same as neighbors, but accepts a numerical array to pass as a parameter to the query function.  Arguments are: the function to probe for data in question (seed or burn), a weighting function, a maximum distance within which to probe (or a minimum probability depending on the Neighbor implementation, optional pointer to parameters to pass to the weighting function which should know what they are.
                            double (Frame::*QueryFunction)(Landscape*, const double, const double*), 
                            double (*WeightFunc)(const double* const), 
                            const double maxDist, 
                            double* pParms=NULL, 
                            const double* FuncParm=NULL);
	const SClimate		cellClimate(const int yearsBeforePresent=0) const;	        //Get climate data for a give year before present
	const float			cellPrecipByMonth(const int month, const int yearsBeforePresent=0) const;
	const float			cellTempByMonth(const int month, const int yearsBeforePresent=0) const;

	virtual void		logFireStats(int interval, bool ignoreFirstInterval);	//Update the fire stats - this is just the by-species stats which need to be updated as that cell burns and is called from Fire
	void				saveMaps(const std::string filename, const int mapFlags);
protected:
	//Ignition worker functions.
	bool				testNaturalIgnition(Frame* pFrame);
	bool				testHumanIgnition(Frame* pFrame);
	bool				testFireSpread(Frame* pFrame, int rowOfNeighbor, int colOfNeighbor, float fireSuppressionFactor);
    virtual const float getCustomFireSpreadMultiplier(const unsigned int row, const unsigned int col, const unsigned int fireSizeTotal, const unsigned int fireNum) { return 1.0; }
	float				getCellDistanceToNeighbor(int rowOfNeighbor, int colOfNeighbor);
};


inline const SClimate			Landscape::cellClimate(const int yearsBeforePresent) const
{ 
    return _pClimate->getClimate(_row, _col, yearsBeforePresent); 
}	

inline const float			Landscape::cellTempByMonth(const int month, const int yearsBeforePresent) const
{ 
    return _pClimate->getTemp(_row, _col, month, yearsBeforePresent); 
}	

inline const float			Landscape::cellPrecipByMonth(const int month, const int yearsBeforePresent) const
{ 
    return _pClimate->getPrecip(_row, _col, month, yearsBeforePresent); 
}	


#endif
