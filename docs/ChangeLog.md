# ALFRESCO Change Log

## 2.2.1
- Fixed value index of vegetation GeoTIFF description tag.

## 2.2
- Converted main input configuration to JSON from custom .FIF format
- Added directory structures to outputs
- Added experimental fire seasons extending to August, September
- Added support for Ignition/Sensitivity map transitions at select time steps
- Added mychorrhizal dynamics to tundra cells.

## 2.1
- Added Tundra types (GraminoidTundra, ShrubTundra, WetlandTundra replace Tundra).
- Seed dispersal / establishment routines added.
- Allow colonization of Tundra

## 2.0
- Major changes to ALFRESCO.
- Windows client deprecated.
- Command line Linux client initiated.
- MPI Support added for parallel execution.
- Rework of how statistics files are gathered.
- New data sets supported, with larger extent covering much of Canada

## 1.0.11
Released: 1/31/2011

This release focuses on removing the need to clip a set of simulation landscape files from a larger master dataset, allowing a single input set to be used for many subset landscapes.

- New: read your simulation landscape in as a sub-domain from a larger set of raster files. To do so, add the new XOffset and YOffset FIF fields and set XSize and YSize (formerly MaxRow and MaxCol) to the size of your sub-domain simulation landscape.

  New FIF Fields:

  ```
  XOffset = [integer]  ; (optional)number of pixels from the left to start reading (first read pixel is at XOffset + 1) 
  YOffset = [integer]  ; (optional) number of pixels from the top to start reading (first read pixel is at YOffset + 1)
  XSize = [integer]    ; (required) width of simulated landscape in pixels.
  YSize = [integer]    ; (required) height of simulated landscape in pixels
  ```

- New: you can now use real years rather than zero based years (eg 1900 through 1999 rather than 0 through 99). Before, you would have to create a new input dataset or rename all the files if you wanted to change the year on which to start the simulation.

  New FIF Fields:

  ```
  FirstYear = [integer]    ; (required) first year of simulation
  LastYear  = [integer]    ; (required) last year of simulation
  ```

- Depricated FIF Fields (they can now be removed from your FIF files for use with 1.0.11 and beyond)
  - TimeStep (due to having become obsolete)
  - MaxYears (replaced by FirstYear and LastYear)
  - MaxCol (replaced by XSize)
  - MaxRow (replaced by YSize)
  - NumHeader (was a remnent from old versions that used ASCII rasters rather than GeoTIFF rasters)

## 1.0.10
Released: 11/18/2010

- Bug Fixed: Forced vegetation transition input rasters were still required to use the .txt file extention. Now .tif can be used.
- Major Change: Added a frame type, Grassland. If you do not want to use the Grassland frame, you do not need to add any new FIF fields. See below for an example of the new FIF fields. Values specific to Grassland are being determined, so the provided values are for example only.

  FIF Fields for the Grassland frame:

  ```
  ; THE GRASSLAND FRAME
  ; The Grassland frame is just like the spruce frames except for how Decid succeeds to Grassland. It 
  ; uses many of the same FIF fields as the spruces and it transitions to Decid immediately after a burn. 
  Grassland = 5; Integer. A unique veg ID between 0 and 254. If this field is not commented out, all other grassland fields are required.
  Grassland.StartAge = {11.3, 2.36}; A constant distribution is used if only one parameter is specified. Otherwise two parameters should be specified: the lifetime and the shape parameters for a Weibull distribution.
  Grassland.FireParms = {0.22623, 8., 0.08}; Parameters for the fire age function : k   Max Fire Factor  a   Age Coefficient (Age   a/b)  b   Slope Coefficient
  Grassland.HumanFireProb = 0.002184466; Probability of human caused burn for this frame type.

  ; Succession from Decid to Grassland:
  ; For a specified number of years (Decid->Grassland.History) after a Decid frame burns, a running sum 
  ; of results from a climate regression equation (using parameters from Decid->Grassland.ClimateWeight) 
  ; is compared to a climate threshold value (from Decid->Grassland.ClimateThreshholds). If the sum is 
  ; greater than the threshold, Decid succeeds to Grassland. The threshold used depends on the burn 
  ; severity of the fire on that cell.
  Decid->Grassland.History = 3; Integer. Number of years to collect results from the regression equation and compare to the threshold.
  Decid->Grassland.TempMonths = {3,4,5,6}; Array of integers. Months, in numeric order, to read temperature raster files for use in the regression equation.
  Decid->Grassland.PrecipMonths = {6,7}; Array of integers. Months, in numeric form and in order, to read precipitation raster files for use in the regression equation.
  Decid->Grassland.ClimateWeight = {.1, 1.0,  .03,.04,.05,.06,  .006,0.007}; A variable length array of regression function parameters as follows: {Intercept, IfFlat_InterceptAdjustment, [a temp multiplier per month in Grassland.TempMonths], [a precip multiplier per month in Grassland.PrecipMonths]}
  Decid->Grassland.ClimateThreshholds = {2, 2.5, 1, 1, 3, 3.2}; Array of 6 thresholds, one per burn severity level: {Low, Moderate, High_LSS, High_HSS, Low_WasGrassland, Moderate_WasGrassland}
  ```

## 1.0.9
Released: 10/25/2010

- Bug Fixed: it was possible for a temperature value to contain a nodata value but still be summed with a temperature offset value, resulting in an innappropriate temperature value for any cells that had set temperature to nodata. 
- Major Change: Spatial input files must now be in the GeoTIFF format and all spatial output is written in the GeoTIFF format. GeoTIFF is a binary file format that allows data to be stored more efficiently than the text based ASCII Grid format that has been used in ALFRESCO until now. There are many tools available to convert from ASCII Grid files to GeoTIFF files including the free GDAL tools. If the NODATA value metadata tag is not specified, the default NODATA value for the given datatype is assumed (see Supported Datatypes below for defaults). Best performance is seen when input files use the default NODATA value for the given datatype and the NODATA metadata tag is set with the same value.

  Supported Raster Datatypes:

  ```
     TYPE   |    MIN VAL    |   MAX VAL    | DEFAULT NODATA
  ----------------------------------------------------------
    Byte    |            0  |         255  |          255
    INT32   |  -2147483648  |  2147483648  |  -2147483648
    FLOAT32 |     -3.4e+38  |     3.4e+38  |     -3.4e+38
  ```

  ### Input Raster Files

  #### Byte (value range of 0 through 255, default nodata = 255)

  - Vegetation Type (any valid veg ID)
  - Vegetation Type Transition (any valid veg ID)
  - Topography (0 and 1)
  - Burn Severity (0=No Burn, 1=Low, 2=Moderate, 3=High w/ Low Surface Severity, 4=High w/ High Surface Severity)
  - Fire History (0 and 1)
  - Human Ignitions (0 and 1)
  - Fire Suppression Levels (0, 1, 2, 3, 4 and 5)
	
  #### Int (value range of -2147483648 through 2147483648, default nodata = -2147483648)

  - Stand Age
  - Tree Density

  #### Float (value range of -3.4e+38 through 3.4e+38, default nodata = -3.4e+38)

  - Site
  - Temperature
  - Precipitation
  - External Flammability
  - Fire Ignition Factor
  - Fire Spread Sensitivity

  ### Output Raster Files

  #### Byte (value range of 0 through 255, nodata = 255)

  - Vegetation Type  (any valid veg ID)
  - Subcanopy  (any valid veg ID)
  - Burn Severity (0=No Burn, 1=Low, 2=Moderate, 3=High w/ Low Surface Severity, 4=High w/ High Surface Severity)
  - Burn Severity History (0=No Burn, 1=Low, 2=Moderate, 3=High w/ Low Surface Severity, 4=High w/ High Surface Severity)
  - Decid: Species Trajectory  (Black Spruce and White Spruce IDs)

  #### Int (value range of -2147483648 through 2147483648, nodata = -2147483648)

  - Stand Age
  - Fire Age

  #### Float (value range of -3.4e+38 through 3.4e+38, nodata = -3.4e+38)

  - Site
  - Fire Scars
  - Tundra: Basal Area

  New FIF Fields

  ```
  RequireAAEACProjectedInputRasters = TRUE     ; (optional -- defaults to TRUE) if TRUE, each input raster file will be checked for the Alaska Albers projection.
  ApplyAAEACProjectionToOutputRasters = TRUE   ; (optional -- defaults to TRUE) if TRUE, each output raster file will include metadata for the Alaska Albers projection.
  ```

  Modified FIF Fields

  ```
  MapFlags -- the field hasn't changed, but the species specific output type flags for Decid and Tundra have been placed onto two separate flag values rather than sharing the 0x000100 flag. The FIF comments to describe the species specific options used to be this:
  ; 0x000100   Decid: species trajectory; Tundra: basal area
  Now it should read like this: 
  ; 0x000100   Decid: species trajectory
  ; 0x000200   Tundra: basal area
  ```

  Depricated FIF Fields (they can now be removed from your FIF files for use with 1.0.9 and beyond)

  - NoData (you can now specify nodata values per raster input file, those values will then be mapped to the default nodata values listed above for internal use and for output)

## 1.0.8
Released: 6/24/2010

- Bug Fixed: in some circumstances an empty directory tree was created -- typically at the root of the C drive.
- FRESCO Server can now be configured to accept client connections over ports other than the default 9051. In the top menu, select Settings | Edit Settings.
- Input and output can now live in very separate locations (eg: different storage volumes) and any FIF field that specifies a path can be either an absolute path (eg: C:\absolute\path\to\something\) or a relative path (relative\path\to\something\).  Relative paths are appended to the input base path if it is an input field or to the output base path if it is an output field.  Some FIF fields were changed to allow this.  These changes break backward compatibility with old FIFs.  

New Fields:

- ClientFifPath
- ClientInputBasePath
- ClientOutputBasePath
- ServerOutputBasePath

Depricated Fields (they can now be removed from your FIF files for use with 1.0.8 and beyond):

- ServerBaseDirectory
- ClusterBaseDirectory
- Output.Directory
- Output.Type  -- now defaults to using the timestamp output directory.

## 1.0.7
Released: 11/24/2009
- Note: this version is backwards compatible with FIF files as old as version 1.0.5.
- Added an optional FIF field to enable externally calculated climate-flammability values to be loaded per year rather than being calculated in ALFRESCO. The field is Climate.Flammability.File and it accepts a single string specifying the file path template for the series of yearly climate-flammability files.

## 1.0.6
Released: 9/24/2009
- Note: FIF files from version 1.0.5 still work with this version.
- Bug Fixed: Climate.Values.RandExplicit.MinYear and Climate.Values.RandExplicit.MaxYear FIF parameters can now have values outside the range of the min and max year for the simulation, but still must be greater or equal to 0.
- Spatial vegetation input files that contain the value given for the NoData FIF field are now used to create NoVeg grid cells.  This pertains to both landscape initialization and vegetation transitions.  Note that the NoVeg FIF parameter can also be changed to the same value used for NoData for a similar effect.
- Depricated the NumSpecies FIF field since it can be easily calculated in within the code.
- Added a Grassland vegetation type.  It is in a very preliminary stage for internal testing.

## 1.0.5   
Released: 1/6/2009
- Bug Fixed: unexpected behavior was possible when calculating the spread of burn severity.
- Bug Fixed: the FIF value, BurnSeverity.LSS-vs-HSS.wt, was evaluating the reverse of what is documented.  Now a low value (closer to 0.0) truely indicates that low surface severity (LSS) is more likely, and a high value (closer to 1.0) truely indicates a higher chance of HSS.

## 1.0.4   
Released: 6/2/2008
- Bug Fixed: Decid.FireProb was being set to the value of Tundra.FireProb when Decid.FireProb.IsAgeDependent was FALSE.

## 1.0.3   
Released: 5/28/2008
- New FIF Field: Tundra.FireProb.IsAgeDependent = <TRUE or FALSE>.  If TRUE: Tundra.FirePorb requires an array of three parameters {k,a,b} in that order for a logistic function k/(1+exp(a-b*x)) used to translate age to fire factor. If FALSE: Tundra.FireProb requires a single constant value (no array).
- New FIF Field: Decid.FireProb.IsAgeDependent = <TRUE or FALSE>.  If TRUE: Decid.FirePorb requires an array of three parameters {k,a,b} in that order for a logistic function k/(1+exp(a-b*x)) used to translate age to fire factor. If FALSE: Decid.FireProb requires a single constant value (no array).
- Bug Fixed: the NumHeader FIF field was being ignored.

## 1.0.2   
Released: 2/25/2008
- Added an optional FIF field, IgnitionDepressor, for each species (e.g., Tundra.IgnitionDepressor).
	
## 1.0.1
Released: 12/21/2007

Summary: More burn severity features and bug fixes.

- Added FIF field, UseUniqueBurnSeverityPerRep:
UseUniqueBurnSeverityPerRep = TRUE 
; Use unique burn severity input file per rep.  This can prevent a bias due to all reps starting from the same input data.
- Changed FIF field, YearOfVegAndAgeFiles, to YearOfUniqueInputPerRep.  This field now supplies the year to be used for Veg, Age and/or Burn Severity input maps that are unique per rep.
- When the BurnSeverityInputFile FIF field is NOT provided, all cells default to HighLSS (level 3) at year 0 for all reps (regardless of how UseUniqueBurnSeverityPerRep is set).
- Fixed a linux client crash.
- Fixed a few memory leaks in client (introduced in 1.0.0).
- Fixed bug when client connects to server while simulation already in progress.  In such cases, the server would not report that the simulation was complete upon all participating clients completing.
- Fixed bug when a client disconnects and remaining clients start a new simulation.  Reps were being distributed inappropriately in such a case.
- Fixed a menu button on the server.
- Fixed the Progress Viewer clear button icon and the Client Manager icon.  They were displaying defaults.
- Fixed NoData_value field in GIS headings.  Was showing -1 instead of value provided in FIF.

## 1.0.0	
Released: 12/13/2007

Summary: Added burn severity features to the model.

- Added FIF field, BurnSeverityInputFile, to initialize the burn severity levels of the landscape before the simulation starts.  The burn severity levels are:	
	- 0=no burn
	- 1=Low severity
	- 2=Moderate severity
	- 3=High crown severity with low surface severity
	- 4=High crown severity with high surface severity

- Burn severity levels are also generated and assigned to cells during historical fire:
		- Tundra, always Low (level 1)
    - Decid, BSpruce and WSpruce, always HighLSS (level 3)
- Burn severity levels are also generated ‘real-time’ during non-historical fire:
    - Tundra, always Low (level 1)
    - Decid, always adopts severity from the cell that spread the fire
    - BSpruce and WSpruce, take on any burn severity based on topography and size of the growing fire spread.

- Here are the new FIF fields used to generate burn severity levels for BSpruce and WSpruce:

  ```
  TopoInputFile = "Input\Initialization\Topography.txt"
  ; Spatial topogrphy input file.  0=flat, 1=complex
  BurnSeverity.FxnOfFireSize = {0.05, 0.001}
  ; Intercept and slope for logistic regression fxn returning probability of High Severity (level 3 or 4)
  BurnSeverity.LSS-vs-HSS.wt = .5
  ; Range: (0,1).  The probability that a burn that is assigned High Severity has HSS.  Low value is more likely to be LSS (level 3), high value is more likely to be HSS (level 4).
  BurnSeverity.Low-vs-Moderate.wt = .5
  ; Range: (0,1);  The probability that a burn not assigned High Severity has Moderate Severity. Low value is more likely to give Low Severity (level 1), high value is more likely give Moderate (level 2).
  BurnSeverity.FlatTopo.wt = .6
  ; Range: (0,1); Prob that Topo spatial correlation overrides initial Severity assignment; eg takes on Severity of ‘Spreader cell’
  BurnSeverity.ComplexTopo.wt = .2
  ; Range: (0,1); Prob that Topo spatial correlation overrides initial Severity assignment
  ```

- Decid cells transition over different time frames depending on species trajectory (as before) and the severity of their last burn.  New FIF fields were added in replacement of Decid->BSpruce.Parms and Decid->WSpruce.Parms.  These new fields provide different succession parameters per burn severity level (A*age+B = probability of transition):

  ```
  Decid->BSpruce.BurnSeverity[1] = {A, B}
  Decid->BSpruce.BurnSeverity[2] = {A, B}
  Decid->BSpruce.BurnSeverity[3] = {A, B}
  Decid->BSpruce.BurnSeverity[4] = {A, B}

  Decid->WSpruce.BurnSeverity[1] = {A, B}
  Decid->WSpruce.BurnSeverity[2] = {A, B}
  Decid->WSpruce.BurnSeverity[3] = {A, B}
  Decid->WSpruce.BurnSeverity[4] = {A, B}
  ```

- Added two spatial output types.  1) burn severity of fire activity within a given year.  2) the burn severity of the last burn for each cell.
- Added four new columns – one for each of the burn severities –  to the Fire Size Events stat file: Low, Mod, HighLSS, HighHSS.  Added up, they equal the Value column.
- Removed three fields from FIF: IsLargeMemoryModel, Output.DotDisplay and Output.ShowSeed (no harm done if fields still exist in your files, but the fields will not be used).

## Beta 0.2.3	
Released: 8/2/2007

Summary: New features.

- Added monthly climate input capabilities.  Only compatible with explicit climate input types.  Can still use growing season input.
- Added new fire equation used when monthly climate input is provided.  Growing season equation still used with growing season climate input.
- Turned off growth factor in the Tundra succession.  Due to the dependency on growing season climate and because the feature has been turned off via FIF settings for a long time, the growth factor is being set to zero.  The growth factor is used to calculate the portion of new basal area of Black Spruce or White Spruce due to growth.
- Turned off climate history dependant transitions in Tundra and Decid successions.  Again, this is being turned off due to the dependency on growing season climate and because the feature has been turned off via FIF settings for a long time.
- Added batch run ability via the new file list.  Just add more files to the list and run.

## Beta 0.2.2	
Released: 6/29/2007

Summary: Bug fixes

- Habitat Stats Were Not Using Age Criteria Correctly.  Fixed

## Beta 0.2.1	
Released: 6/28/2007

Summary: Bug fixes.

- Increased Stat Event Capacity
- Excluded NoVeg Cells From Historical Fires
- Set /LARGEADDRESSAWARE On Server To Use 3GB of Memory Instead of 2GB

## Beta 0.2.0
Released: 6/21/2007

Summary: Bug fixes and new features for existing tools.

- Added Suppression Transitions Functionality
  - FIF fields changed.  See new demo FIF.
- Suppression Stats
  - When suppression is on, stat files are produced showing the area burned within by suppression class.  Each class has its own stat output.  New FIF field: Stat.BurnsBySupp.Flags.
- Habitat Stats
  - New FIF fields describe the habitat criteria so that stat files can be produced showing the count of frames that match the criteria.  Right now the only criteria are valid veg types and the age range of vegetation.  New habitats can be tracked by adding new entires into the FIF.
- Client Running on Linux and Windows
- Client Logs on Server Filtered by Selected Client
  - Double click or press enter on a client in the Client Manager and the client’s log is shown.
- Client Can Run as Daemon
  - A daemon is a process that runs continuously even when the user that started it is no longer logged on.  The –daemon command line argument starts the client as a daemon.
- Simple Client Install
  - Installation of client on Linux no longer requires changing the LD_LIBRARY_PATH environment variable.  Follow ReadMe instructions included in Linux install.
  - ServerAddress.ini files are no longer used to specify the server’s address.  Instead, the ‘startclient’ script contains the server’s IP address within command line argument, --ip.
- Time Estimation
  - Time to completion estimates are given in the Server’s status bar.  The progress viewer provides start and stop timestamps.  The Client Manager shows percent completed for each client.  
- Version Assertion
  - Makes sure all clients are running the same version.
- Separate Base Directories
  - At the moment, the server is not able to access the cluster’s storage space.  As a temporary work around, the server uses a different base directory than the clients and therefore uses a separate copy of the FIF with the same filename.
  - This should only be a temporary work around until we are either able to provide the server with access to the same storage as the clients or until the server app takes care of all file transfers.
- ‘Unknown’ Connections and Delayed Connections Fixed
  - Clients were connecting, but sometimes with an unknown state.  As a result they would not participate in simulations.  Fixed.
  - Clients were also experiencing a delayed connection by more than 15 seconds.  Now clients should all connect within 2-5 seconds.

