# JSON Configuration

The JSON configuration file contains all parameterizations for a particular simulation scenario. An example JSON file is provided in `examples/alfresco.json`. See below for descriptions of each parameter, broken down by section.

## High level structure

```
[{
  "type": "AlfrescoConfig",
  "PathSetup": {},
  "Landscape": {},
  "RasterSettings": {},
  "Climate": {},
  "Fire": {},
  "MapOutput": {},
  "Stat": {},
  "Vegetation": {},
  "Simulation": {}
}]
```

## Simulation

```
Simulation {
  // This can be any 'long int'.  If the same number is used, it will produce
  // the same simulation if all other variables are fixed
  "RandSeed": 1234567890,

  // Level of detail to produce in Log Files
  "Output.DetailLevel": "MAXIMUM",

  // Simulation starting year.  Typically, no earlier than 1000.
  "FirstYear": 1000,

  // Simulation ending year.
  "LastYear": 2100,

  // Total number of replicate simulations to perform
  "MaxReps": 200,

  // Determines whether neighbor algorithm will crop calls to cells outside
  // the circular boundary defined by size or should it include all the cells
  // withing the square boundary defined by size.
  "CropNeighbors": true
}
```

## RasterSettings

```
"RasterSettings": {
  // X Upper Left coordinate of the origin point
  "XULCorner": -1725223.205807,

  // Y Upper Left coordinate of the origin point
  "YULCorner": 2544412.932644,

  // The number of pixels to offset from the X origin point
  "XOffset": 0,

  // The number of pixels to offset from the Y origin point
  "YOffset": 0,

  // Number of pixels to include, starting from the YULCorner+YOffset
  "XSize": 3650,

  // Number of pixels to include, starting from the XULCorner+XOffset
  "YSize": 2100,

  // The size of each cell (in meters)
  "CellSize": 1000,

  // If true, each input raster file will be checked for the Alaska Albers
  // projection. (Default: true)
  "RequireAAEACProjectedInputRasters": false,

  // If true, each output raster file will include metadata for the Alaska
  // Albers projection. (Default: true)
  "ApplyAAEACProjectionToOutputRasters": true
}
```

## Landscape

```
"Landscape": {
  // Aspect input file
  "SiteInputFile": "Elevation/ALF_AK_CAN_prism_aspect_1km.tif",

  // Canopy / density input file
  "TreeDensityInputFile": "Initialization_MarkO/POSTProc_CreateCanopy.tif",

  // Topographic / elevation file
  "TopoInputFile": "Initialization_MarkO/POSTProc_ALF_AK_CAN_prism_slopeComplexity_1km.tif",

  // Burn severity file
  "BurnSeverityInputFile": "Initialization_MarkO/POSTProc_CreateBSH.tif",

  // Starting age input file
  "AgeInputFile": "Initialization_MarkO/POSTProc_CreateAge.tif",

  // Starting vegetation input file
  "VegInputFile": "Landcover/LandCover_alf_2005.tif",

  // Base filename for forced vegetation transitions. Year is appended as _YR
  "VegTransitionFile": "Input/Initialization/VegTransitions.tif",

  // If true, succession trajectories are read from files as well as simulated. If false,
  // succession is simulated only.
  "IsForcedVegTransitions": false,

  // Whether to use unique veg/age files per rep, or start all via one input
  "UseUniqueVegAndAgePerRep": false,

  // Whether to use unique burn severity files per rep, or start all via one input
  "UseUniqueBurnSeverityPerRep": false,

  // Starting year for unique input file
  "YearOfUniqueInputPerRep": 1000
}
```

## Climate

```
"Climate": {
  // Determine whether climate is based on monthly inputs
  "IsMonthly": true,

  // Use the extended, "Experimental" climate algorithms
  "IsExperimental": true,

  // Number of years of climate history to maintain within the model
  "NumHistory": 1,

  // Years that climate transitions occur.  The number of transitions will
  // impact the number of variables in multiple other Climate properties.
  "TransitionYears": [1000, 1901, 2009],

  "Values": {
    // CONSTANT: Single temp and precip value used for every cell over all
    //   years and reps.
    // SPATIAL: Read in spatially unique temp and precip values from a file
    //   once and reusecit each year and rep.
    // EXPLICIT: Read in spatially unique temp and precip values from a
    //   different file each year. Year is appended as _YR.
    // RANDEXPLICIT: Read in spatially unique temp and precip values from
    //   files in random order. Year is appended as _YR.
    "Type": ["RANDEXPLICIT", "EXPLICIT", "EXPLICIT"],

    // Constant temperature value for entire simulation on all land cells
    "Temp.Constant": [0.0, 0.0, 0.0],

    // Constant precipitation value for entire simulation on all land cells
    "Precip.Constant": [0.0, 0.0, 0.0],

    // Base filename for temperature maps
    "Temp.File": [
      "Climate/cru_TS31/historical/tas/tas_mean_C_alf_cru_TS31.tif",
      "Climate/cru_TS31/historical/tas/tas_mean_C_alf_cru_TS31.tif",
      "CLIMATE_TAS"
    ],

    // Base filename for precipitation maps
    "Precip.File": [
      "Climate/cru_TS31/historical/pr/pr_total_mm_alf_cru_TS31.tif",
      "Climate/cru_TS31/historical/pr/pr_total_mm_alf_cru_TS31.tif",
      "CLIMATE_PR"
    ],

    // For use with RANDEXPLICIT climate type: Minimum simulation year for
    // the random selection of climate temperature and precipitation maps
    "RandExplicit.MinYear": [1901, 1901, 2009],

    // For use with RANDEXPLICIT climate type: Maximum simulation year for
    // the random selection of climate temperature and precipitation maps
    "RandExplicit.MaxYear": [1999, 1901, 2009],

    // If true the RANDOMEXPLICIT climate type will apply the same random
    // sequence of climate files every rep. Otherwise a different sequence
    // is genereated per rep
    "RandExplicit.Replicate": [false, false, false],

    // If true ramps and steps will be applied to the temporal offsets
    "StepsAndRampsEnabled": false
  },
  "Offsets": {
    // NONE: No offsets applied
    // CONSTANT: Single temp and precip offset used for all cells on all
    //   years and reps
    // FILE: Offsets are read in from a file
    // RANDOM: Offsets are generated stochastically
    "Type": ["NONE", "NONE", "NONE"],

    // The file to use as input when using the FILE offset type
    "File": ["na", "na", "na"],

    // The temperature offset used when for CONSTANT offset type scenarios
    "Constant.Temp": [0.0, 0.0, 0.0],

    // The precipitation offset used when for CONSTANT offset type
    // scenarios
    "Constant.Precip": [0.0, 0.0, 0.0],

    // The mean of the offset from climate temperatures. Mean is of course
    // expected to be 0 as the mean difference from the mean for that cell
    // should be 0.
    "Random.Temp.Mean": [0.0, 0.0, 0.0],

    // The standard deviation of the offset from mean climate temperatures
    "Random.Temp.StdDev": [0.0, 0.0, 0.0],

    // The mean of the offset from mean climate precipitation.
    "Random.Precip.Mean": [0.0, 0.0, 0.0],

    // The standard deviation of the offset from mean climate precipitation
    "Random.Precip.StdDev": [0.0, 0.0, 0.0],

    // If true, RANDOM offset type will apply the same stochastic offset
    // sequence every rep. Otherwise a different sequence is genereated per
    // rep.
    "Random.Replicate": [false, false, false],

    // The year that temperature steps occur. Must be a multiple of
    // TempStep to occur
    "TempStepYear": [],

    // The amount of the step that occurs in the specified year
    "TempStep": [],

    // The year that temperature ramps change i.e. the new ramp is applied
    // if the year is > the specified year
    "TempRampYear": [],

    // The ramp (amount per year) that starts in the specified year.
    "TempRamp": [],

    // The year that precipitation steps occur. Must be a multiple of
    // TempStep to occur
    "PrecipStepYear": [],

    // The amount of the step that occurs in the specified year
    "PrecipStep": [],

    // The year that precipitation ramps change i.e. the new ramp is
    // applied if the year is > the specified year.
    "PrecipRampYear": [],

    // The ramp (amount per year) that starts in the specified year
    "PrecipRamp": []
  }
}
```

## Fire

```
"Fire": {
  // FIXED: Single ignition and sensitivity factor used
  // SPATIAL: Read ignition and sensitivity factors from files
  // HISTORICAL: Burns are scheduled across the landscape
  "Types": ["SPATIAL", "SPATIAL", "SPATIAL"],

  // The years in which fire type transitions occur
  "TypeTransitionYears": [1000, 1950, 2009],

  // Enable the fire model. Usually set to true
  "Enabled": true,

  // The maximum distance fire can spread (meters). If CropNeighbours is set
  // to false and this is less than CellSize it will check the adjacent cells
  // only
  "SpreadRadius": 1000,

  // The mean and stddev of the fire spread weighting function. Mean should
  // always remain 0
  "SpreadParms": [0.0, 500.0],

  // Regression relationship between fire probability and climate
  // When Climate.IsMonthly is false, the coefficients = {
  //   intercept, temp, precip
  // }
  // When Climate.IsMonthly is true, the coefficients = {
  //   intercept, tMar, tApr, tMay, tJun, pJun, pJul, tAprXpJul
  // }
  "Climate": [1.0, 0.05, 0.1, 0.5, 0.85, 0.15, 0.50, 0.2, -0.075, -0.025, -0.085, -0.005, 0.0],

  // Ignore the first fire interval for a cell when calculating statistics.
  // Used to help eliminate startup bias in statistical calculations.
  "IgnoreFirstInterval": true,

  // Fire ignition multiplier. Used to tweak fire ignition rates.
  "IgnitionFactor": [2.95e-05, 2.95e-05, 2.95e-05],

  // General fire sensitivity parameter. Used to tweak over fire sizenumber
  // for a region.
  "Sensitivity": [475.0, 475.0, 475.0],

  // Maximum fire size event as observed in empirical data
  "MaxEmpiricalFireSizeEvent": 6900,

  // Weight to be applied to fire spread probability when fire size exceeds
  // the maximum
  "MaxEmpiricalFireSizeEventWeight": 0.80,

  // Base filename for historical fire maps
  "Historical": ["", "", ""],

  // Base filename for human ignition fire maps.
  // Map Legend:
  //   0: No human ignition
  //   1: Possible human ignition (dependent on Human Ignition Prob)
  "HumanIgnition.Basename": "Fire/HumanIgnitions/HIg_Default_0.txt",

  // Filenames for spatially explicit ignition maps
  "Spatial.IgnitionFactor": [
    "/path/to/Ignition1.tif",
    "/path/to/Ignition2.tif",
    "/path/to/Ignition3.tif"
  ],

  // Filenames for spatially explicit sensitivity maps
  "Spatial.Sensitivity": [
    "/path/to/Sensitivity1.tif",
    "/path/to/Sensitivity2.tif",
    "/path/to/Sensitivity3.tif"
  ],

  "Suppression": {
    // If true, apply suppression maps to fire spread calculations
    "On": false,

    // Base filename for suppression maps
    // Map Legend: 1-5 where 1 Most Suppression and 5 Least
    "Basename": "Fire/Suppression/old_mngt.txt",

    "Tran": {
      // Year of suppression transitions
      "Years": [0, 34, 56],
        
      // If false, do not input new map
      // If true, input new map (must be supplied w corresponding _tran.year.txt)
      "HasNewMap": [true, true, true],

      // Highest suppression level. Used as a multiplier in the fire spread function.
      "Class1": [1.0, 0.05, 0.05],

      // Used as a multiplier in the fire spread function.
      "Class2": [1.0, 0.15, 0.15],

      // Used as a multiplier in the fire spread function.
      "Class3": [1.0, 0.6, 0.6],

      // Used as a multiplier in the fire spread function.
      "Class4": [1.0, 1.0, 1.0],

      // Lowest suppression level. Usually 1 to give no suppression.
      // Used as a multiplier in the fire spread function.
      "Class5": [1.0, 1.0, 1.0],

      // The cutting point where suppressions stop being applied because the
      // sum of the year's fire sizes exceeds the capability of available
      // suppression resources
      "Threshold.FireSize": [28461, 28461, 28461],

      // The cutting point where suppressions stop being applied because
      // the amount of ignitions for the year exceeds the capability of
      // available suppression resources
      "Threshold.Ignitions": [500, 500, 500]
    }
  },
  "BurnSeverity": {
    // The intercept and slope for logistic regression fxn returning
    // probability of High Severity (level 3 or 4)
    "FxnOfFireSize": [-3.0, 0.00333],

    // Probability that a burn that is assigned High Severity has HSS.
    // Low value is more likely to be LSS (level 3), high value is more
    // likely to be HSS (level 4).
    "LSS-vs-HSS.wt": 0.5,

    // Probability that a burn not assigned High Severity has Moderate
    // Severity. Low value is more likely to give Low Severity (level 1),
    // high value is more likely give Moderate (level 2)
    "Low-vs-Moderate.wt": 0.5,

    // Probability that Topo spatial correlation overrides initial Severity
    // assignment;  e.g., takes on Severity of Spreader cell
    "FlatTopo.wt": 0.5,

    // Probability that Topo spatial correlation overrides initial Severity
    // assignment
    "ComplexTopo.wt": 0.2
  }
}
```

## Vegetation

```
"Vegetation": {
  // If true, inoculum values can be provided for Graminoid Tundra and Shrub Tundra
  "enableInoculum": false,

  "NoVeg": {},
  "Tundra": {),
  "BSpruce": {},
  "WSpruce": {},
  "Decid": {},
  "Graminoid Tundra": {},
  "Wetland Tundra": {},
  "Shrub Tundra": {},
  "BarrenLichenMoss": {},
  "TemperateRainforest": {}
}
```

### NoVeg

```
"NoVeg": {
  // The species number for the default frame type. Should be 0 if possible.
  "id": 0
}
```

### Tundra

```
"Tundra": {
  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge": [40.0, 1.0],

  // Is fire probability age dependent
  "FireProb.IsAgeDependent": true,

  // If true, Tundra.FireProb requires an array of three parameters {k,a,b}
  // in that order for a logistic function k/(1+exp(a-b*x)) used to translate
  // age to fire factor. If false, Tundra.FireProb requires a single constant
  // value (no array).
  "FireProb": [0.10, 6.0, 0.1],

  // Probability that human causes will cause this species type to burn
  "HumanFireProb": 2.432667E-05,

  // Estimate of the basal area of mature forest. Used to calculate seed
  // dispersal for the initial BA assignment routine.
  "Spruce.EstBA": 23.5,

  // Distance to the maximum seed source in meters
  "SeedRange": 1000,

  // The mean travel distance(meters) and fraction of thin tail (1-fraction
  // fat tail) in the distribution kernel.
  "SeedSource": [35.0, 0.95],

  // The number of seeds/Ha produced per unit basal area (m2/Ha)
  "Seed.BasalArea": 70000,

  // Seed to seedling ratio including viability factor
  "Seedling": 100,

  // Initial basal area of a seedling (m2), based on a 5mm diameter
  "SeedlingBA": 7.854E-05,

  // The number of years of climate that a deciduous frame uses to make
  // decisions. Includes the current year.
  "History": 20,

  // Seedling establishment parameters. Cuttoff temp avg degree days/year
  // cutoff.
  "SeedEstParms": [7.5, 0.085],

  // Mean tundra growth (m). This discounts the known trend in growth through
  // time.
  "MeanGrowth": 0.001,

  // Three coefficients (Int Temp Precip) for the relative growth factor.
  // From a regression against climate.
  "ClimGrowth": [0.0, 0.0, 0.0],

  // Calibration factors to move from cohort growth model to exponential
  // growth model - growth seed set
  "CalFactor": [100.0, 100.0],

  // Basal area at which tundra transitions to spruce (m2/Ha). A mature
  // spruce stand isassumed to be about double this.
  "Spruce.BasalArea": 10,

  // Decreases the likelihood of ignitions for any cell of this species.
  // Value should be between 0 and 1. (Optional)
  "IgnitionDepressor": 0.1,

  // Enable transitions
  "Transitions": false
}
```

### BSpruce

```
"BSpruce": {
  // The species number for the black spruce frame type.
  // Should be 1 if possible.
  "id": 1,

  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge": [3.59, 0.87],

  // Parameters for the fire age function:
  //   k Max Fire Factor
  //   a Age Coefficient (Age ab) b Slope Coefficient
  "FireParms": [0.95, 4.0, 0.08],

  // Probability that human causes will cause this species type to burn
  "HumanFireProb": 0.00119969
}
```

### WSpruce

```
"WSpruce": {
  // The species number for the white spruce frame type.
  // Should be 2 if possible.
  "id": 2,

  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge": [11.3, 2.36],

  // Parameters for the fire age function:
  //   k Max Fire Factor
  //   a Age Coefficient (Age ab) b Slope Coefficient
  "FireParms": [0.87, 8.0, 0.08],

  // Probability that human causes will cause this species type to burn
  "HumanFireProb": 0.002184466
}
```

### Decid

```
"Decid": {
  // The species number for the deciduous frame type.
  // Should be 3 if possible.
  "id": 3,

  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge.WSpruce": [9.0],

  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge.BSpruce": [4.0],

  // Is fire probability age dependent
  "FireProb.IsAgeDependent": false,

  // If true, Decid.FireProb requires an array of three parameters {k,a,b}
  // in that order for a logistic function k/(1+exp(a-b*x)) used to translate
  // age to fire factor. If false, Tundra.FireProb requires a single constant
  // value (no array).
  "FireProb": 0.020,

  // Probability that human causes will cause this species type to burn
  "HumanFireProb": 0.003060109,

  // The number of years of climate that a deciduous frame uses to make
  // decisions. Includes the current year.
  "History": 20,

  // Degree-years are integrated from base A (i.e. temp < A). A transition
  // occurs if the integrand exceeds B* (the maximum number of steps i.e.
  // HistoryTimeStep).
  "Tundra.Parms": [4.0, 19.32],

  "Grassland.History": 3,
  "Grassland.TempMonths": [3, 4, 5, 6],
  "Grassland.PrecipMonths": [6, 7],
  "Grassland.ClimateWeight": [0.1, 1.0, 0.03, 0.04, 0.05, 0.06, 0.006, 0.007],
  "Grassland.ClimateThreshholds": [2, 2.5, 1, 1, 3, 3.2],

  // The following 8 fields are sets of two coefficients, {A, B}, used
  // depending on the trajectory and last burn severity of the given decid
  // cell. The chosen values are used in a linear function relating age to
  // the probability of transitioning to white or black spruce: A*Age + B.
  "WSpruce.BurnSeverity[1]": [0.001000000, -0.07500000],
  "WSpruce.BurnSeverity[2]": [0.001000000, -0.07500000],
  "WSpruce.BurnSeverity[3]": [0.001000000, -0.09000000],
  "WSpruce.BurnSeverity[4]": [0.001000000, -0.09000000],
  "BSpruce.BurnSeverity[1]": [0.001428571, -0.07142857],
  "BSpruce.BurnSeverity[2]": [0.001428571, -0.07142857],
  "BSpruce.BurnSeverity[3]": [0.001000000, -0.07500000],
  "BSpruce.BurnSeverity[4]": [0.001000000, -0.07500000]
}
```

### ShrubTundra

```
"ShrubTundra": {
  // The species number for the shrub frame type.
  // Should be 4 if possible.
  "id": 4,

  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge": [40.0, 1.0],

  // Is fire probability age dependent
  "FireProb.IsAgeDependent": true,

  // If true, Tundra.FireProb requires an array of three parameters {k,a,b}
  // in that order for a logistic function k/(1+exp(a-b*x)) used to translate
  // age to fire factor. If false, Tundra.FireProb requires a single constant
  // value (no array).
  "FireProb": [0.17, 6.0, 0.15],

  // Probability of human causes that will cause this species type to burn
  "HumanFireProb": 2.432667E-05,

  // Estimate of the basal area of mature forest. Used to calculate seed
  // dispersal for the initial BA assignment routine.
  "Spruce.EstBA": 20,

  // Distance to the maximum seed source in meters
  "SeedRange": 1000,

  // The mean travel distance(meters) and fraction of thin tail (1-fraction
  // fat tail) in the distribution kernel
  "SeedSource": [35.0, 0.95],

  // The number of seeds/Ha produced per unit basal area (m2/Ha)
  "Seed.BasalArea": 70000,

  // Seed to seedling ratio including viability factor
  "Seedling": 100,

  // Initial basal area of a seedling (m2), based on a 5mm diameter
  "SeedlingBA": 7.854E-05,

  // The number of years of climate that a deciduous frame uses to make
  // decisions. Includes the current year.
  "History": 20,

  // Seedling establishment parameters. Cuttoff temp avg degree days/year
  // cutoff.
  "SeedEstParms": [7.5, 0.085],

  // Mean tundra growth (m). This discounts the known trend in growth through
  // time.
  "MeanGrowth": 0.00044,

  // Three coefficients (Int Temp Precip) for the relative growth factor.
  // From a regression against climate.
  "ClimGrowth": [0.0, 0.0, 0.0],

  // Calibration factors to move from cohort growth model to exponential
  // growth model - growth seed set
  "CalFactor": [0.0, 0.0],

  // Basal area at which tundra transitions to spruce (m2/Ha). A mature
  // spruce stand isassumed to be about double this.
  "Spruce.BasalArea": 10,

  // Decreases the likelihood of ignitions for any cell of this species.
  // Value should be between 0 and 1. (Optional)
  "IgnitionDepressor": 0.1,

  // Enable transitions
  "Transitions": true,

  // Year to begin dispersal of Spruce into Shrub Tundra
  "SpruceTransitionYear": 1860,

  // Year to begin succession transitions for Shrub Tundra
  "TundraTransitionYear": 1960
}
```

### GraminoidTundra

```
"GraminoidTundra": {
  // The species number for the shrub frame type.
  // Should be 5 if possible.
  "id": 5,

  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge": [40.0, 1.0],

  // Is fire probability age dependent
  "FireProb.IsAgeDependent": true,

  // If true, GraminoidTundra.FireProb requires an array of three parameters
  // {k,a,b} in that order for a logistic function k/(1+exp(a-b*x)) used to
  // translate age to fire factor. If false, Tundra.FireProb requires a
  // single constant value (no array).
  "FireProb": [0.17, 6.0, 0.15],

  // Probability that human causes will cause this species type to burn
  "HumanFireProb": 2.432667E-05,

  // Estimate of the basal area of mature forest. Used to calculate seed
  // dispersal for the initial BA assignment routine.
  "Spruce.EstBA": 20,

  // Distance to the maximum seed source in meters
  "SeedRange": 1000,

  // The mean travel distance(meters) and fraction of thin tail (1-fraction
  // fat tail) in the distribution kernel
  "SeedSource": [35.0, 0.95],

  // The number of seeds/Ha produced per unit basal area (m2/Ha)
  "Seed.BasalArea": 70000,

  // Seed to seedling ratio including viability factor
  "Seedling": 100,

  // Initial basal area of a seedling (m2), based on a 5mm diameter
  "SeedlingBA": 7.854E-05,

  // The number of years of climate that a deciduous frame uses to make
  // decisions. Includes the current year.
  "History": 20,

  // Seedling establishment parameters. Cuttoff temp avg degree days/year
  // cutoff.
  "SeedEstParms": [7.5, 0.085],

  // Mean tundra growth (m). This discounts the known trend in growth through
  // time.
  "MeanGrowth": 0.00044,

  // Three coefficients (Int Temp Precip) for the relative growth factor.
  // From a regression against climate.
  "ClimGrowth": [0.0, 0.0, 0.0],

  // Calibration factors to move from cohort growth model to exponential
  // growth model - growth seed set
  "CalFactor": [0.0, 0.0],

  // The number of seeds/Ha produced per unit basal area (m2/Ha)
  "Spruce.BasalArea": 10,

  // Decreases the likelihood of ignitions for any cell of this species.
  // Value should be between 0 and 1. (Optional)
  "IgnitionDepressor": 0.1,

  // Enable transitions
  "Transitions": true,

  // Year to begin dispersal of Spruce into Shrub Tundra
  "SpruceTransitionYear": 1860,

  // Year to begin succession transitions for Shrub Tundra
  "TundraTransitionYear": 1960,

  // Summer Warmth Index Value (Summer mean monthly degrees > 0) at which
  // transitions toward Shrub Tundra may begin
  "SummerWarmthIndex": 27
}
```

### WetlandTundra

```
"WetlandTundra": {
  // The species number for the shrub frame type.
  // Should be 6 if possible.
  "id": 6,

  // A constant distribution is used if only one parameter is specified.
  // Otherwise two parameters should be specified: the lifetime and the shape
  // parameters for a Weibull distribution.
  "StartAge": [40.0, 1.0],

  // Is fire probability age dependent
  "FireProb.IsAgeDependent": true,

  // If true, Tundra.FireProb requires an array of three parameters {k,a,b}
  // in that order for a logistic function k/(1+exp(a-b*x)) used to translate
  // age to fire factor. If false, Tundra.FireProb requires a single constant
  // value (no array).
  "FireProb": [0.0, 0.0, 0.0],

  // Probability that human causes will cause this species type to burn
  "HumanFireProb": 2.432667E-05,

  // Estimate of the basal area of mature forest. Used to calculate seed
  // dispersal for the initial BA assignment routine.
  "Spruce.EstBA": 23.5,

  // Distance to the maximum seed source in meters
  "SeedRange": 1000,

  // The mean travel distance(meters) and fraction of thin tail (1-fraction
  // fat tail) in the distribution kernel
  "SeedSource": [35.0, 0.95],

  // The number of seeds/Ha produced per unit basal area (m2/Ha)
  "Seed.BasalArea": 70000,

  // Seed to seedling ratio including viability factor
  "Seedling": 5000,

  // Initial basal area of a seedling (m2), based on a 5mm diameter
  "SeedlingBA": 7.854E-05,

  // The number of years of climate that a deciduous frame uses to make
  // decisions. Includes the current year.
  "History": 20,

  // Seedling establishment parameters. Cuttoff temp avg degree days/year
  // cutoff.
  "SeedEstParms": [7.5, 0.085],

  // Mean tundra growth (m). This discounts the known trend in growth through
  // time.
  "MeanGrowth": 0.00044,

  // Three coefficients (Int Temp Precip) for the relative growth factor.
  // From a regression against climate.
  "ClimGrowth": [0.0, 0.0, 0.0],

  // Calibration factors to move from cohort growth model to exponential
  // growth model - growth seed set
  "CalFactor": [0.0, 0.0],

  // The number of seeds/Ha produced per unit basal area (m2/Ha)
  "Spruce.BasalArea": 10,

  // Decreases the likelihood of ignitions for any cell of this species.
  // Value should be between 0 and 1. (Optional)
  "IgnitionDepressor": 0.1,

  // Enable transitions
  "Transitions": false
}
```

### BarrenLichenMoss

```
"BarrenLichenMoss": {
  // The species number for the shrub frame type.
  // Should be 7 if possible.
  "id": 7
}
```

### TemperateRainforest

```
"TemperateRainforest": {
  // The species number for the shrub frame type.
  // Should be 8 if possible.
  "id": 8
}
```