using System;
using System.IO;
using System.Text;
using System.Globalization;
using System.Collections;
using System.Collections.Generic;
//using FRESCOOL;

namespace FRESCO_Server
{
    public class FIF
	{

		#region FIF Variables
		//Output Directory
		public string		FileName				= "";
		public string		BaseDirectory			= "";
		public string		BaseDirectoryTip		= "";
        public string       ClusterBaseDirectory    = "";
        public string       ClusterBaseDirectoryTip = "";
		public string		OutputDirectory			= "";
		public string		OutputDirectoryTip		= "";
		public EOutputType	OutputType				= EOutputType.OVERWRITE;
		public string		OutputTypeTip			= "";
		public int			OutputInterval			= -1;
		public string		OutputIntervalTip		= "";
		public EDetailLevel	OutputDetailLevel		= EDetailLevel.MODERATE;
		public string		OutputDetailLevelTip	= "";
		//Program
		public string		RandSeed 				= "CURRENTTIME";
		public string		RandSeedTip				= "";
		public bool			IsLargeMemoryModel		= false;
		public string		IsLargeMemoryModelTip	= "";
		//Replication
		public int			MaxReps 				= -1;
		public string		MaxRepsTip				= "";
		//Year
		public int			MaxYears 				= -1;
		public string		MaxYearsTip				= "";
		public int			TimeStep 				= -1;
		public string		TimeStepTip				= "";
		//Landscape
		public int			NumHeader				= -1;
		public string		NumHeaderTip			= "";
		public int			MaxRow					= -1;														//; The total number of rows in the landscape.
		public string		MaxRowTip				= "";
		public int			MaxCol 					= -1;														//; The total number of columns in the landscape.
		public string		MaxColTip				= "";
		public float		CellSize 				= -1;														//; The size of each cell. The current size is in meters
		public string		CellSizeTip				= "";
		public bool			IsCropNeighbors 		= false;													//; Should the neighbours algorithm crop calls to cells outside the circular boundary defined by size or should it include all the cells withing the square boundary defined by size. 
		public string		CropNeighborsTip		= "";
		public int			NumSpecies 				= -1;														//; The number of species present in the model (including the NoVeg type)
		public string		NumSpeciesTip			= "";
		public int			NoVeg 					= -1;														//; The species number for the default frame type - should be 0 if possible
		public string		NoVegTip				= "";
		public double		XLLCorner 				= -1;														//; X Corner specified by ARCgis
		public string		XLLCornerTip			= "";
		public double		YLLCorner 				= -1;														//; Y Corner specified by ARCgis
		public string		YLLCornerTip			= "";
        
        public bool         IsUsingUniqueVegAndAgePerRep = false;
        public string       IsUsingUniqueVegAndAgePerRepTip = "";
        public int          YearOfVegAndAgeFiles    = -1;
        public string       YearOfVegAndAgeFilesTip = "";

        //Veg Layer
		public string		VegInputFile 			= "";														//; The vegetation input file
		public string		VegInputFileTip			= "";
		public string		VegTransitionFile 		= "";
		public string		VegTransitionFileTip	= "";
		public bool			IsForcedVegTransitions 		= false;
		public string		IsForcedVegTransitionsTip	= "";
		//Age Layer
		public string		AgeInputFile 			= "";														//; The initial age input file - years before present so positive values
		public string		AgeInputFileTip			= "";
		//Site Layer
		public string		SiteInputFile 			= "";														//; The site input file
		public string		SiteInputFileTip		= "";
		//Tree Density Layer
		public string		TreeDensityInputFile 	= "";														//; The initial tree density input file
		public string		TreeDensityInputFileTip	= "";
		//Climate
		public int				ClimNumHistory 				        = -1;											//; The number of years to remember climate history including the current year
		public string			ClimNumHistoryTip			        = "";
        public SortedList<int, SClimateTransition> ClimateTransitions = new SortedList<int, SClimateTransition>();
        public List<int>        ClimateTransitionYears;
        public string           ClimateTransitionYearsTip           = "";
        public List<string>		ClimValuesTypes;                                         						//; CONSTANT=Single temp and precip value used for every cell in all years and reps.  SPATIAL=Read in spatially unique temp and precip values from a file.  Reuse each year and rep.  EXPLICIT=Read in spatially unique temp and precip values from a different file each year.  Year is appended as "_YR".
		public string			ClimValuesTypesTip			        = "";
		public List<float>      ClimValuesConstantTemps; 
        public string           ClimValuesConstantTempsTip          = "";    
		public List<float>      ClimValuesConstantPrecips; 
        public string           ClimValuesConstantPrecipsTip          = "";    
        public string			ClimValuesTemp 				        = "";											//; Base filename for temperature maps used for fixed spatial climate
		public string			ClimValuesTempTip			        = "";
		public string			ClimValuesPrecip 			        = "";											//; Base filename for precipitation maps used for fixed spatial climate
		public string			ClimValuesPrecipTip			        = "";
        public List<string>		ClimValuesTempFiles;
		public string			ClimValuesTempFilesTip              = "";
        public List<string>		ClimValuesPrecipFiles;
		public string			ClimValuesPrecipFilesTip            = "";
        public List<int>        ClimValuesRandExplicitMinYears;
		public string			ClimValuesRandExplicitMinYearsTip   = "";
        public List<int>        ClimValuesRandExplicitMaxYears;
        public string           ClimValuesRandExplicitMaxYearsTip   = "";
        public List<bool>       ClimValuesRandExplicitIsReplicated;             								    //; If true, RANDOMEXPLICIT climate type will apply the same random sequence of climate files every rep.  Otherwise a different sequence is genereated per rep.  
        public string           ClimValuesRandExplicitIsReplicatedTip = "";
        public List<string>     ClimOffsetsTypes;									                                //; NONE=No offsets applied.  CONSTANT=Single temp and precip offset used for all cells, years and reps. FILE=Offsets are read in from a file.  RAMPED=Offsets are calculated using stepping and ramping.
		public string			ClimOffsetsTypesTip			        = "";
        public List<string>     ClimOffsetsFiles;											                        //; The file to use as input/output for fixed climate scenarios
		public string			ClimOffsetsFilesTip			        = "";
        public List<float>      ClimOffsetsTemps;											                        //; The temperature offset used when for constant offset type scenarios.
		public string			ClimOffsetsTempsTip			        = "";
        public List<float>      ClimOffsetsPrecips;											                        //; The precipitation offset used when for constant offset type scenarios.
		public string			ClimOffsetsPrecipsTip		        = "";
        public List<float>      ClimOffsetsRandTempMeans;
        public string           ClimOffsetsRandTempMeansTip         = "";
        public List<float>      ClimOffsetsRandTempStdDevs;
        public string           ClimOffsetsRandTempStdDevsTip       = "";
        public List<float>      ClimOffsetsRandPrecipMeans;
        public string           ClimOffsetsRandPrecipMeansTip       = "";
        public List<float>      ClimOffsetsRandPrecipStdDevs;
        public string           ClimOffsetsRandPrecipStdDevsTip     = "";
        public List<bool>       ClimOffsetsRandIsReplicated;                    								    //; If true, RANDOM offset type will apply the same stochastic offset sequence every rep.  Otherwise a different sequence is genereated per rep.  
        public string           ClimOffsetsRandIsReplicatedTip      = "";
        public bool             ClimStepsAndRampsEnabled            = false;
        public string           ClimStepsAndRampsEnabledTip         = "";
        public List<int>		ClimOffsetsTempStepYear;															//; The year that temperature steps occur.  Must be a multiple of TempStep to occur.
		public string			ClimOffsetsTempStepYearTip	        = "";
		public List<float>		ClimOffsetsTempStep;																//; The amount of the step that occurs in the specified year
		public string			ClimOffsetsTempStepTip		        = "";
		public List<int>		ClimOffsetsTempRampYear;															//; The year that temperature ramps change, i.e. the new ramp is applied if the year is >= the specified year.
		public string			ClimOffsetsTempRampYearTip	        = "";
		public List<float>		ClimOffsetsTempRamp;																//; The ramp (amount per year) that starts in the specified year
		public string			ClimOffsetsTempRampTip		        = "";	
		public List<int>		ClimOffsetsPrecipStepYear;															//; The year that precipitation steps occur.  Must be a multiple of TempStep to occur.
		public string			ClimOffsetsPrecipStepYearTip        = "";
		public List<float>		ClimOffsetsPrecipStep;																//; The amount of the step that occurs in the specified year
		public string			ClimOffsetsPrecipStepTip	        = "";
		public List<int>		ClimOffsetsPrecipRampYear;															//; The year that precipitation ramps change, i.e. the new ramp is applied if the year is >= the specified year.
		public string			ClimOffsetsPrecipRampYearTip        = "";
		public List<float>		ClimOffsetsPrecipRamp;																//; The ramp (amount per year) that starts in the specified year
		public string			ClimOffsetsPrecipRampTip	        = "";
		//Fire
		public bool				FireEnabled 				= false;												//; Enable the fire model - usually set to true
		public string			FireEnabledTip				= "";
		public float			FireSpreadRadius 			= -1;													//; The maximum distance fire can spread (meters) - if CropNeighbours is set to false and this is less than CellSize, it will check the adjacent cells only
		public string			FireSpreadRadiusTip			= "";
		public List<double>		FireSpreadParms;																	//; The mean and stddev of the fire spread weighting function.  Mean should always remain 0. 
		public string			FireSpreadParmsTip			= "";
		public List<double>		FireClimate;																		//; Relationship between fire probability and climate.  Results of a regression analysis.
		public string			FireClimateTip				= "";
		public bool				FireIgnoreFirstInterval 	= false;												//; Ignore the first fire interval for a cell when calculating statistics - used to help eliminate startup bias in statistical calculations.
		public string			FireIgnoreFirstIntervalTip	= "";
		public List<float>		FireIgnition;																		//; Fire ignition multiplier - used to tweak fire ignition rates
		public string			FireIgnitionTip		= "";
		public List<float>		FireSensitivity;																	//; General fire sensitivity parameter - used to tweak over fire size/number for a region
        public string           FireSensitivityTip = "";
        public int              FireMaxEmpiricalFireSizeEvent = -1;
        public string           FireMaxEmpiricalFireSizeEventTip = "";
        public float            FireMaxEmpiricalFireSizeEventWeight = -1;
        public string           FireMaxEmpiricalFireSizeEventWeightTip = "";
		public string			FireHistorical				= "";													//; Base filename for historical fire maps.
		public string			FireHistoricalTip			= "";
		public string			FireHumanIgnitionFile		= "";
		public string			FireHumanIgnitionFileTip	= "";
		public bool				FireSuppressionOn			= false;
		public string			FireSuppressionOnTip		= "";
		public string			FireSuppressionFile			= "";
		public string			FireSuppressionFileTip		= "";
		public double			FireSuppressionClass1		= -1;
		public string			FireSuppressionClass1Tip	= "";
		public double			FireSuppressionClass2		= -1;
		public string			FireSuppressionClass2Tip	= "";
		public double			FireSuppressionClass3		= -1;
		public string			FireSuppressionClass3Tip	= "";
		public double			FireSuppressionClass4		= -1;
		public string			FireSuppressionClass4Tip	= "";
		public double			FireSuppressionClass5		= -1;
		public string			FireSuppressionClass5Tip	= "";
		public int				FireSuppressionThresholdFireSize	= -1;
		public string			FireSuppressionThresholdFireSizeTip	= "";
		public int				FireSuppressionThresholdIgnitions	= -1;
		public string			FireSuppressionThresholdIgnitionsTip= "";
		public SortedList<int,SFireTransition> FireTransitions	= new SortedList<int,SFireTransition>();
		public string			FireTypeTransitionYearTip	= "";
		public string			FireTypeTransitionTip		= "";
		public string			FireSpatialIgnitionTip = "";
		public string			FireSpatialSensitivityTip	= "";
		List<string>			FireTransitionTypes;
		List<int>				FireTransitionYears;
		List<string>			FireSpatialIgnitionFile;
		List<string>			FireSpatialSensitivityFile;
		//Species
		public int				NoData	 					= -1;													//; No Data value specified by ARCgis
		public string			NoDataTip					= "";
		//Tundra
		public int				Tundra 						= -1;													//; The species number for this species
		public string			TundraTip					= "";
		public List<double>		TundraStartAge;																		//; Parameters for the starting age function.  If one parameter is specified assume a constant distribution, otherwise parameters are the lifetime and shape parameters for a Weibull distribution
		public string			TundraStartAgeTip			= "";
        public float            TundraFireProb = -1;													//; Probablity of this species type burning
		public string			TundraFireProbTip			= "";
        public float            TundraHumanFireProb = -1;
		public string			TundraHumanFireProbTip		= "";
		public double			TundraSpruceEstBA 			= -1;													//; Estimate of the BA of mature forest - used to calculate seed dispersal for initial BA assignment routine. 
		public string			TundraSpruceEstBATip		= "";
		public double			TundraSeedRange 			= -1;													//; Distance to the maximum seed source in meters
		public string			TundraSeedRangeTip			= "";
		public List<double>		TundraSeedSource;																	//; The mean travel distance(meters) and fraction of thin tail (1-fraction fat tail) in the distribution kernel
		public string			TundraSeedSourceTip			= "";
		public double			TundraSeedBasalArea 		= -1;													//; The number of seeds/Ha produced per unit basal area (m2/Ha)
		public string			TundraSeedBasalAreaTip		= "";
		public double			TundraSeedling 				= -1;													//; Seed to seedling ratio including viability factor
		public string			TundraSeedlingTip			= "";
		public double			TundraSeedlingBA 			= -1;													//; Initial basal area of a seedling (m2) - based on a 5mm diameter
		public string			TundraSeedlingBATip			= "";
		public int				TundraHistory 				= -1;													//; The number of years of climate this frame uses to make decisions including the current year
		public string			TundraHistoryTip			= "";
		public List<double>		TundraSeedEstParms;																	//; Seedling establishment parameters - {cuttoff temp, avg degree days/year cutoff}
		public string			TundraSeedEstParmsTip		= "";
		public double			TundraMeanGrowth 			= -1;													//; Mean spruce growth (m) - this discounts the known trend in growth through time
		public string			TundraMeanGrowthTip			= "";
		public List<double>		TundraClimGrowth;																	//; Coefficients to the relative growth factor - from a regression against climate (Int, Temp, Precip)
		public string			TundraClimGrowthTip			= "";
		public List<double>		TundraCalFactor;																	//; Calibration factors to move from cohort growth model to exponential growth model - {growth, seed set}
		public string			TundraCalFactorTip			= "";
		public double			TundraSpruceBasalArea 		= -1;													//; Basal area at which tundra transitions to spruce (m2/Ha).  A mature spruce stand is assumed to be about double this.
		public string			TundraSpruceBasalAreaTip	= "";
		//Black Spruce
		public int				BSpruce 					= -1;													//; The species number for this species
		public string			BSpruceTip					= "";
		public List<double>		BSpruceStartAge;																	//; Parameters for the starting age function.  If one parameter is specified assume a constant distribution, otherwise parameters are the lifetime and shape parameters for a Weibull distribution
		public string			BSpruceStartAgeTip			= "";
		public List<double>		BSpruceFireParms;																	//; Parameters for the fire age function : k = Max Fire Factor, a = Age Coefficient (Age = a/b), b = Slope Coefficient
		public string			BSpruceFireParmsTip			= "";
		public float			BSpruceHumanFireProb		= -1;
		public string			BSpruceHumanFireProbTip		= "";
		//White Spruce
		public int				WSpruce 					= -1;													//; The species number for this species
		public string			WSpruceTip					= "";
		public List<double>		WSpruceStartAge;																	//; Parameters for the starting age function.  If one parameter is specified assume a constant distribution, otherwise parameters are the lifetime and shape parameters for a Weibull distribution
		public string			WSpruceStartAgeTip			= "";
		public List<double>		WSpruceFireParms;																	//; Parameters for the fire age function : k = Max Fire Factor, a = Age Coefficient (Age = a/b), b = Slope Coefficient
		public string			WSpruceFireParmsTip			= "";
        public float            WSpruceHumanFireProb = -1;
		public string			WSpruceHumanFireProbTip		= "";
		//Deciduous
		public int				Decid 						= -1;													//; The species number for this species
		public string			DecidTip					= "";
		public List<double>		DecidStartAgeWSpruce;																//; TODOParameters for the starting age function.  If one parameter is specified assume a constant distribution, otherwise parameters are the lifetime and shape parameters for a Weibull distribution
		public string			DecidStartAgeWSpruceTip		= "";
		public List<double>		DecidStartAgeBSpruce;																//; TODOParameters for the starting age function.  If one parameter is specified assume a constant distribution, otherwise parameters are the lifetime and shape parameters for a Weibull distribution
		public string			DecidStartAgeBSpruceTip		= "";
        public float            DecidFireProb = -1;													//; Probablity of this species type burning
		public string			DecidFireProbTip			= "";
        public float            DecidHumanFireProb = -1;
		public string			DecidHumanFireProbTip		= "";
		public int				DecidHistory 				= -1;													//; The number of years of climate this frame uses to make decisions including the current year
		public string			DecidHistoryTip				= "";
		public List<double>		DecidTundraParms;																	//; Degree-years are integrated from base A (i.e. temp < A).  A transition occurs if the integrand exceeds B*(The maximum number of steps i.e. History/TimeStep).
		public string			DecidTundraParmsTip			= "";
		public List<double>		DecidBSpruceParms; 																	//; Cefficients to linear function relating age to probability of transitioning to BLACK spruce: A*Age + B
		public string			DecidBSpruceParmsTip		= "";		
		public List<double>		DecidWSpruceParms;							 										//; Cefficients to linear function relating age to probability of transitioning to WHITE spruce: A*Age + B
		public string			DecidWSpruceParmsTip		= "";
		//Maps
		public List<string>		MapFiles;																			//; The base names for the output maps
		public string			MapFilesTip					= "";
		public List<string>		MapCodes;																			//; The code to output so the user knows	 this map has been written
		public string			MapCodesTip					= "";
		public List<int>		MapFlags;																			//; The output codes for the map in question
		public string			MapFlagsTip					= "";
		public List<int>		MapRepStart;																		//; The first replication that outputs maps.
		public string			MapRepStartTip				= "";
		public List<int>		MapRepFreq;																			//; The replication frequency to output maps.
		public string			MapRepFreqTip				= "";
		public List<int>		MapYearStart;																		//; The first year that outputs maps.
		public string			MapYearStartTip				= "";
		public List<int>		MapYearFreq;																		//; The year frequency to output maps.
		public string			MapYearFreqTip				= "";
		//Statistics Output
		public string			StatSummaryFile 			= "";													//; The name of the statistical data output file
		public string			StatSummaryFileTip			= "";
		public int				StatFireNumFlags			= 0x0;													//; Output flags for the fire size stats
		public string			StatFireNumFlagsTip			= "";
		public int				StatFireSizeFlags 			= 0x0;													//; Output flags for the fire size stats
		public string			StatFireSizeFlagsTip		= "";
		public int				StatVegDistFlags 			= 0x0;													//; Output flags for the vegetation stats
		public string			StatVegDistFlagsTip			= "";
		public int				StatVegResidenceFlags 		= 0x0;													//; Output flags for the vegetation residence times
		public string			StatVegResidenceFlagsTip	= "";
		public int				StatFireSpeciesFlags 		= 0x0;													//; Output flags for the fire by species stats
		public string			StatFireSpeciesFlagsTip		= "";
		public int				StatFireIntervalFlags		= 0x0;													//; Output flags for the fire interval stats
		public string			StatFireIntervalFlagsTip	= "";
		#endregion

		public enum EnumFIFSettingType
		{
			BOOL,
			ENUM,
			INT,
			FLOAT,
			STRING
		};
		private string[]		words;
		private Global			Global;
		public					FIF(Global G)
		{
			Global = G;
		}

		public void				Load(string filename)
		{
			string				line			= "";
			string				Temp			= "";
            SFireTransition FireTransition;
            SClimateTransition ClimateTransition;

			FileName= filename;
			if (!File.Exists(FileName))	throw new Exception("File " + FileName + " does not exist.\n");
			//Parse file.
			try 
			{
				using (StreamReader sr = new StreamReader(FileName)) 
				{
					//Find General Settings.
					while ((line=sr.ReadLine()) != "; PROGRAM PARAMETERS")	{if (line==null) throw new Exception("'PROGRAM PARAMETERS' missing.\n");}
					//Parse General Settings.
					line = sr.ReadLine();
					BaseDirectory				= GetVal(line);	
					BaseDirectoryTip			= GetTip(line);								line = sr.ReadLine();
					//A \ is interpreted to mean the applications startup directory.
					if (BaseDirectory.StartsWith("\\"))	BaseDirectory = Global.WorkingDirectory + BaseDirectory;
					MaxYears					= Convert.ToInt32(GetVal(line));
					MaxYearsTip					= GetTip(line);								line = sr.ReadLine();
					MaxReps						= Convert.ToInt32(GetVal(line));
					MaxRepsTip					= GetTip(line);								line = sr.ReadLine();
					TimeStep 					= Convert.ToInt32(GetVal(line));
					TimeStepTip					= GetTip(line);								line = sr.ReadLine();
					RandSeed 					= GetVal(line);
					RandSeedTip					= GetTip(line);								line = sr.ReadLine();
					NumHeader					= Convert.ToInt32(GetVal(line));
					NumHeaderTip				= GetTip(line);								line = sr.ReadLine();
					NoData	 					= Convert.ToInt32(GetVal(line));
					NoDataTip					= GetTip(line);								line = sr.ReadLine();
					OutputDirectory				= GetVal(line);
					OutputDirectoryTip			= GetTip(line);								line = sr.ReadLine();
					OutputType					= ((Temp=GetVal(line))=="DELETE" ? EOutputType.DELETEOUTPUTDIRECTORY : (Temp=="OVERWRITE" ? EOutputType.OVERWRITE : EOutputType.APPENDDATETIME));
					OutputTypeTip				= GetTip(line);								line = sr.ReadLine();
					line = sr.ReadLine();		//Skip DotDisplay
					line = sr.ReadLine();		//Skip IsOutputSeed
					OutputDetailLevel			= ((Temp=GetVal(line))=="MINIMAL" ? EDetailLevel.MINIMAL : (Temp=="MODERATE" ? EDetailLevel.MODERATE : EDetailLevel.MAXIMUM));
					OutputDetailLevelTip		= GetTip(line);								line = sr.ReadLine();
					IsLargeMemoryModel			= GetVal(line).ToUpper() == "TRUE" ? true : false;
					IsLargeMemoryModelTip		= GetTip(line);
					//Find Landscape Settings.
					while ((line = sr.ReadLine()) != "; LANDSCAPE PARAMETERS")	{ if (line==null) throw new Exception("'LANDSCAPE PARAMETERS' missing.\n");}
					//Parse Landscape Settings.
					line = sr.ReadLine();
					MaxRow						    = Convert.ToInt32(GetVal(line));
					MaxRowTip					    = GetTip(line);								line = sr.ReadLine();
					MaxCol 						    = Convert.ToInt32(GetVal(line));
					MaxColTip					    = GetTip(line);								line = sr.ReadLine();
					CellSize 					    = Convert.ToSingle(GetVal(line));
					CellSizeTip					    = GetTip(line);								line = sr.ReadLine();
					IsCropNeighbors 			    = GetVal(line).ToUpper() == "TRUE" ? true : false;
					CropNeighborsTip			    = GetTip(line);								line = sr.ReadLine();
					NumSpecies 					    = Convert.ToInt32(GetVal(line));	
					NumSpeciesTip				    = GetTip(line);								line = sr.ReadLine();
					NoVeg 						    = Convert.ToInt32(GetVal(line));	
					NoVegTip					    = GetTip(line);								line = sr.ReadLine();
					XLLCorner 					    = Convert.ToDouble(GetVal(line));	
					XLLCornerTip				    = GetTip(line);								line = sr.ReadLine();
					YLLCorner 					    = Convert.ToDouble(GetVal(line));	
					YLLCornerTip				    = GetTip(line);								line = sr.ReadLine();
                    IsUsingUniqueVegAndAgePerRep    = GetVal(line).ToUpper() == "TRUE" ? true : false;
                    IsUsingUniqueVegAndAgePerRepTip = GetTip(line);                             line = sr.ReadLine();
                    YearOfVegAndAgeFiles            = Convert.ToInt32(GetVal(line));
                    YearOfVegAndAgeFilesTip         = GetTip(line); line = sr.ReadLine();

                    //Veg Layer
					VegInputFile 				= GetVal(line);	
					VegInputFileTip				= GetTip(line);								line = sr.ReadLine();
					VegTransitionFile 			= GetVal(line);	
					VegTransitionFileTip		= GetTip(line);								line = sr.ReadLine();
					IsForcedVegTransitions 		= GetVal(line).ToUpper() == "TRUE" ? true : false;
					IsForcedVegTransitionsTip	= GetTip(line);								line = sr.ReadLine();
					//Age Layer
					AgeInputFile				= GetVal(line);							
					AgeInputFileTip				= GetTip(line);								line = sr.ReadLine();
					//Site Layer
					SiteInputFile 				= GetVal(line);	
					SiteInputFileTip			= GetTip(line);								line = sr.ReadLine();
					//Tree Density Layer
					TreeDensityInputFile 		= GetVal(line);	
					TreeDensityInputFileTip		= GetTip(line);
					//Find Map Output Settings.
					while ((line = sr.ReadLine()) != "; MAP OUTPUT PARAMETERS")	{ if (line==null) throw new Exception("'MAP OUTPUT PARAMETERS' missing.\n"); }
					//Parse Map Output Settings.
					line = sr.ReadLine();
					MapFiles					= GetStringList(line);
					MapFilesTip					= GetTip(line);								line = sr.ReadLine();
					MapCodes 					= GetStringList(line);
					MapCodesTip					= GetTip(line);								line = sr.ReadLine();
					MapFlags 					= GetHexList(line);
					MapFlagsTip					= GetTip(line);								line = sr.ReadLine();
					MapRepStart 				= GetIntList(line);
					MapRepStartTip				= GetTip(line);								line = sr.ReadLine();
					MapRepFreq 					= GetIntList(line);
					MapRepFreqTip				= GetTip(line);								line = sr.ReadLine();
					MapYearStart 				= GetIntList(line);
					MapYearStartTip				= GetTip(line);								line = sr.ReadLine();
					MapYearFreq 				= GetIntList(line);
					MapYearFreqTip				= GetTip(line);
					//Find Stat Output Settings.
					while ((line = sr.ReadLine()) != "; STAT OUTPUT PARAMETERS")	{ if (line==null) throw new Exception("'STAT OUTPUT PARAMETERS' missing.\n"); }
					//Parse Stat Output Settings.
					line = sr.ReadLine();
					StatSummaryFile 			= GetVal(line);
					StatSummaryFileTip			= GetTip(line);								line = sr.ReadLine();
					StatFireNumFlags			= Convert.ToInt32(GetVal(line),16);
					StatFireNumFlagsTip			= GetTip(line);								line = sr.ReadLine();
					StatFireSizeFlags 			= Convert.ToInt32(GetVal(line),16);
					StatFireSizeFlagsTip		= GetTip(line);								line = sr.ReadLine();
					StatVegDistFlags 			= Convert.ToInt32(GetVal(line),16);
					StatVegDistFlagsTip			= GetTip(line);								line = sr.ReadLine();
					StatVegResidenceFlags 		= Convert.ToInt32(GetVal(line),16);
					StatVegResidenceFlagsTip	= GetTip(line);								line = sr.ReadLine();
					StatFireSpeciesFlags 		= Convert.ToInt32(GetVal(line),16);
					StatFireSpeciesFlagsTip		= GetTip(line);								line = sr.ReadLine();
					StatFireIntervalFlags		= Convert.ToInt32(GetVal(line),16);
					StatFireIntervalFlagsTip	= GetTip(line);
					//Find Climate Settings.
					while ((line = sr.ReadLine()) != "; CLIMATE PARAMETERS")	{ if (line==null) throw new Exception("'CLIMATE PARAMETERS' missing.\n");}
					//Parse Climate Settings.
					line = sr.ReadLine();
					ClimNumHistory 				            = Convert.ToInt32(GetVal(line));
					ClimNumHistoryTip			            = GetTip(line);					line = sr.ReadLine();
                    ClimateTransitionYears                  = GetIntList(line);
                    ClimateTransitionYearsTip               = GetTip(line);                 line = sr.ReadLine();
                    ClimValuesTypes                         = GetStringList(line);
					ClimValuesTypesTip			            = GetTip(line);					line = sr.ReadLine();
                    ClimValuesConstantTemps                 = GetFloatList(line);
                    ClimValuesConstantTempsTip              = GetTip(line);                 line = sr.ReadLine();
                    ClimValuesConstantPrecips               = GetFloatList(line);
                    ClimValuesConstantPrecipsTip            = GetTip(line);                 line = sr.ReadLine();
                    ClimValuesTempFiles                     = GetStringList(line);
                    ClimValuesTempFilesTip                  = GetTip(line);                 line = sr.ReadLine();
                    ClimValuesPrecipFiles                   = GetStringList(line);
                    ClimValuesPrecipFilesTip                = GetTip(line);                 line = sr.ReadLine();
                    ClimValuesRandExplicitMinYears          = GetIntList(line);
                    ClimValuesRandExplicitMinYearsTip       = GetTip(line);                 line = sr.ReadLine();
                    ClimValuesRandExplicitMaxYears          = GetIntList(line);
                    ClimValuesRandExplicitMaxYearsTip       = GetTip(line);                 line = sr.ReadLine();
                    ClimValuesRandExplicitIsReplicated      = GetBoolList(line);
                    ClimValuesRandExplicitIsReplicatedTip   = GetTip(line);                 line = sr.ReadLine();
                    ClimOffsetsTypes                        = GetStringList(line);
					ClimOffsetsTypesTip			            = GetTip(line);					line = sr.ReadLine();
					ClimOffsetsFiles 			            = GetStringList(line);
					ClimOffsetsFilesTip			            = GetTip(line);					line = sr.ReadLine();
					ClimOffsetsTemps				        = GetFloatList(line);
					ClimOffsetsTempsTip			            = GetTip(line);					line = sr.ReadLine();
					ClimOffsetsPrecips			            = GetFloatList(line);
					ClimOffsetsPrecipsTip		            = GetTip(line);					line = sr.ReadLine();
                    ClimOffsetsRandTempMeans                = GetFloatList(line);
                    ClimOffsetsRandTempMeansTip             = GetTip(line);                 line = sr.ReadLine();
                    ClimOffsetsRandTempStdDevs              = GetFloatList(line);
                    ClimOffsetsRandTempStdDevsTip           = GetTip(line);                 line = sr.ReadLine();
                    ClimOffsetsRandPrecipMeans              = GetFloatList(line);
                    ClimOffsetsRandPrecipMeansTip           = GetTip(line);                 line = sr.ReadLine();
                    ClimOffsetsRandPrecipStdDevs            = GetFloatList(line);
                    ClimOffsetsRandPrecipStdDevsTip         = GetTip(line);                 line = sr.ReadLine();
                    ClimOffsetsRandIsReplicated             = GetBoolList(line);
                    ClimOffsetsRandIsReplicatedTip          = GetTip(line);                 line = sr.ReadLine();
                    ClimateTransitions.Clear();
                    for (int i = 0; i < ClimateTransitionYears.Count; i++)
                    {
                        ClimateTransition = new SClimateTransition();
                        try
                        {
                            ClimateTransition.Year = ClimateTransitionYears[i];
                            //Values
                            Temp = ClimValuesTypes[i];
                            ClimateTransition.ValuesType                = (Temp == "CONSTANT" ? EClimateValuesType.VTCONSTANT : (Temp == "SPATIAL" ? EClimateValuesType.VTSPATIAL : (Temp == "EXPLICIT" ? EClimateValuesType.VTEXPLICIT : EClimateValuesType.VTRANDEXPLICIT)));
                            ClimateTransition.ConstantTemp              = ClimValuesConstantTemps[i];
                            ClimateTransition.ConstantPrecip            = ClimValuesConstantPrecips[i];
                            ClimateTransition.SpatialTempFile           = ClimValuesTempFiles[i];
                            ClimateTransition.SpatialPrecipFile         = ClimValuesPrecipFiles[i];
                            ClimateTransition.RandExplicitMinYear       = ClimValuesRandExplicitMinYears[i];
                            ClimateTransition.RandExplicitMaxYear       = ClimValuesRandExplicitMaxYears[i];
                            ClimateTransition.IsRandExplicitReplicated  = ClimValuesRandExplicitIsReplicated[i];
                            //Offsets
                            Temp = ClimOffsetsTypes[i];
                            ClimateTransition.OffsetsType               = (Temp == "NONE" ? EClimateOffsetsType.OTNONE : (Temp == "CONSTANT" ? EClimateOffsetsType.OTCONSTANT : (Temp == "FILE" ? EClimateOffsetsType.OTFILE : EClimateOffsetsType.OTRANDOM)));
                            ClimateTransition.ConstantTempOffset        = ClimOffsetsTemps[i];
                            ClimateTransition.ConstantPrecipOffset      = ClimOffsetsPrecips[i];
                            ClimateTransition.OffsetsFile               = ClimOffsetsFiles[i];
                            ClimateTransition.RandomOffsetsTempMean     = ClimOffsetsRandTempMeans[i];
                            ClimateTransition.RandomOffsetsTempStdDev   = ClimOffsetsRandTempStdDevs[i];
                            ClimateTransition.RandomOffsetsPrecipMean   = ClimOffsetsRandPrecipMeans[i];
                            ClimateTransition.RandomOffsetsPrecipStdDev = ClimOffsetsRandPrecipStdDevs[i];
                            ClimateTransition.IsRandOffsetReplicated    = ClimOffsetsRandIsReplicated[i];
                            ClimateTransitions.Add(ClimateTransition.Year, ClimateTransition);
                        }
                        catch
                        {
                            throw new Exception("A climate transition setting is missing.\n");
                        }
                    }
                    ClimStepsAndRampsEnabled    = GetVal(line).ToUpper() == "TRUE" ? true : false;
                    ClimStepsAndRampsEnabledTip = GetTip(line);                             line = sr.ReadLine();
                    ClimOffsetsTempStepYear 	= GetIntList(line);	
					ClimOffsetsTempStepYearTip	= GetTip(line);								line = sr.ReadLine();
					ClimOffsetsTempStep 		= GetFloatList(line);
					ClimOffsetsTempStepTip		= GetTip(line);								line = sr.ReadLine();
					ClimOffsetsTempRampYear		= GetIntList(line);	
					ClimOffsetsTempRampYearTip	= GetTip(line);								line = sr.ReadLine();
					ClimOffsetsTempRamp 		= GetFloatList(line);
					ClimOffsetsTempRampTip		= GetTip(line);								line = sr.ReadLine();
					ClimOffsetsPrecipStepYear 	= GetIntList(line);	
					ClimOffsetsPrecipStepYearTip = GetTip(line);							line = sr.ReadLine();
					ClimOffsetsPrecipStep 		= GetFloatList(line);
					ClimOffsetsPrecipStepTip	= GetTip(line);								line = sr.ReadLine();
					ClimOffsetsPrecipRampYear 	= GetIntList(line);	
					ClimOffsetsPrecipRampYearTip = GetTip(line);							line = sr.ReadLine();
					ClimOffsetsPrecipRamp 		= GetFloatList(line);
					ClimOffsetsPrecipRampTip	= GetTip(line);
					//Parse Fire Settings.
					while ((line=sr.ReadLine())!="; FIRE PARAMETERS")	{if (line==null) throw new Exception("'FIRE PARAMETERS' missing.\n");}
					line = sr.ReadLine();
					FireTransitionTypes						= GetStringList(line);						FireTypeTransitionTip					= GetTip(line);		line = sr.ReadLine();
					FireTransitionYears						= GetIntList(line);							FireTypeTransitionYearTip				= GetTip(line);		line = sr.ReadLine();
					FireEnabled 							= GetVal(line).ToUpper() == "TRUE" ? true : false;	FireEnabledTip							= GetTip(line);		line = sr.ReadLine();
					FireSpreadRadius 						= Convert.ToSingle(GetVal(line));			FireSpreadRadiusTip						= GetTip(line);		line = sr.ReadLine();
					FireSpreadParms 						= GetDoubleList(line);						FireSpreadParmsTip						= GetTip(line);		line = sr.ReadLine();
					FireClimate 							= GetDoubleList(line);						FireClimateTip							= GetTip(line);		line = sr.ReadLine();
					FireIgnoreFirstInterval 				= GetVal(line).ToUpper() == "TRUE" ? true : false;	FireIgnoreFirstIntervalTip				= GetTip(line);		line = sr.ReadLine();
					FireIgnition	 						= GetFloatList(line);						FireIgnitionTip							= GetTip(line);		line = sr.ReadLine();
					FireSensitivity 						= GetFloatList(line);						FireSensitivityTip						= GetTip(line);		line = sr.ReadLine();
                    FireMaxEmpiricalFireSizeEvent           = Convert.ToInt32(GetVal(line));            FireMaxEmpiricalFireSizeEventTip        = GetTip(line);     line = sr.ReadLine();
                    FireMaxEmpiricalFireSizeEventWeight     = Convert.ToSingle(GetVal(line));           FireMaxEmpiricalFireSizeEventWeightTip  = GetTip(line);     line = sr.ReadLine();
                    FireHistorical							= GetVal(line);								FireHistoricalTip						= GetTip(line);		line = sr.ReadLine();
					FireHumanIgnitionFile					= GetVal(line);								FireHumanIgnitionFileTip				= GetTip(line);		line = sr.ReadLine();
					FireSuppressionOn						= GetVal(line).ToUpper() == "TRUE" ? true : false;	FireSuppressionOnTip					= GetTip(line);		line = sr.ReadLine();
					FireSuppressionFile						= GetVal(line);								FireSuppressionFileTip					= GetTip(line);		line = sr.ReadLine();
					FireSuppressionClass1					= Convert.ToDouble(GetVal(line));			FireSuppressionClass1Tip				= GetTip(line);		line = sr.ReadLine();
					FireSuppressionClass2					= Convert.ToDouble(GetVal(line));			FireSuppressionClass2Tip				= GetTip(line);		line = sr.ReadLine();
					FireSuppressionClass3					= Convert.ToDouble(GetVal(line));			FireSuppressionClass3Tip				= GetTip(line);		line = sr.ReadLine();
					FireSuppressionClass4					= Convert.ToDouble(GetVal(line));			FireSuppressionClass4Tip				= GetTip(line);		line = sr.ReadLine();
					FireSuppressionClass5					= Convert.ToDouble(GetVal(line));			FireSuppressionClass5Tip				= GetTip(line);		line = sr.ReadLine();
					FireSuppressionThresholdFireSize		= Convert.ToInt32(GetVal(line));			FireSuppressionThresholdFireSizeTip		= GetTip(line);		line = sr.ReadLine();
					FireSuppressionThresholdIgnitions		= Convert.ToInt32(GetVal(line));			FireSuppressionThresholdIgnitionsTip	= GetTip(line);		line = sr.ReadLine();
					FireSpatialIgnitionFile 				= GetStringList(line);						FireSpatialIgnitionTip					= GetTip(line);		line = sr.ReadLine();
					FireSpatialSensitivityFile 				= GetStringList(line);						FireSpatialSensitivityTip				= GetTip(line);
					FireTransitions.Clear();
					for (int i=0; i<FireTransitionTypes.Count; i++)
					{
						FireTransition								= new SFireTransition();
						try {
							FireTransition.Type						= (FireTransitionTypes[i]=="FIXED" ? EFireType.FIXED : (FireTransitionTypes[i]=="SPATIAL" ? EFireType.SPATIAL : EFireType.HISTORICAL));
							FireTransition.Year						= FireTransitionYears[i];
							FireTransition.HistoricalFile			= FireHistorical;
							FireTransition.Ignition					= FireIgnition[i];
							FireTransition.Sensitivity				= FireSensitivity[i];
							FireTransition.SpatialIgnitionFile		= FireSpatialIgnitionFile[i];
							FireTransition.SpatialSensitivityFile	= FireSpatialSensitivityFile[i];
							FireTransitions.Add(FireTransition.Year,FireTransition);
						} catch {
							throw new Exception("A fire transition setting is missing.\n");
						}
					}

					//Parse Tundra Settings.
					while ((line = sr.ReadLine()) != "; PARAMETERS FOR THE TUNDRA FRAME")	{ if (line==null) throw new Exception("'PARAMETERS FOR THE TUNDRA FRAME' missing.\n");}
					line = sr.ReadLine();
					Tundra 						= Convert.ToInt32(GetVal(line));													//; The species number for this species
					TundraTip					= GetTip(line);								line = sr.ReadLine();
					TundraStartAge 				= GetDoubleList(line);																//; Parameters for the starting age function.  If one parameter is specified assume a constant distribution, otherwise parameters are the lifetime and shape parameters for a Weibull distribution
					TundraStartAgeTip			= GetTip(line);								line = sr.ReadLine();
					TundraFireProb 				= Convert.ToSingle(GetVal(line));													//; Probablity of this species type burning
					TundraFireProbTip			= GetTip(line);								line = sr.ReadLine();
					TundraHumanFireProb			= Convert.ToSingle(GetVal(line));
					TundraHumanFireProbTip		= GetTip(line);								line = sr.ReadLine();
					TundraSpruceEstBA 			= Convert.ToDouble(GetVal(line));													//; Estimate of the BA of mature forest - used to calculate seed dispersal for initial BA assignment routine. 
					TundraSpruceEstBATip		= GetTip(line);								line = sr.ReadLine();
					TundraSeedRange 			= Convert.ToDouble(GetVal(line));													//; Distance to the maximum seed source in meters
					TundraSeedRangeTip			= GetTip(line);								line = sr.ReadLine();
					TundraSeedSource 			= GetDoubleList(line);																//; The mean travel distance(meters) and fraction of thin tail (1-fraction fat tail) in the distribution kernel
					TundraSeedSourceTip			= GetTip(line);								line = sr.ReadLine();
					TundraSeedBasalArea 		= Convert.ToDouble(GetVal(line));													//; The number of seeds/Ha produced per unit basal area (m2/Ha)
					TundraSeedBasalAreaTip		= GetTip(line);								line = sr.ReadLine();
					TundraSeedling 				= Convert.ToDouble(GetVal(line));													//; Seed to seedling ratio including viability factor
					TundraSeedlingTip			= GetTip(line);								line = sr.ReadLine();
					TundraSeedlingBA 			= Convert.ToDouble(GetVal(line));													//; Initial basal area of a seedling (m2) - based on a 5mm diameter
					TundraSeedlingBATip			= GetTip(line);								line = sr.ReadLine();
					TundraHistory 				= Convert.ToInt32(GetVal(line));													//; The number of years of climate this frame uses to make decisions including the current year
					TundraHistoryTip			= GetTip(line);								line = sr.ReadLine();
					TundraSeedEstParms 			= GetDoubleList(line);													//; Seedling establishment parameters - {cuttoff temp, avg degree days/year cutoff}
					TundraSeedEstParmsTip		= GetTip(line);								line = sr.ReadLine();
					TundraMeanGrowth 			= Convert.ToDouble(GetVal(line));													//; Mean spruce growth (m) - this discounts the known trend in growth through time
					TundraMeanGrowthTip			= GetTip(line);								line = sr.ReadLine();
					TundraClimGrowth 			= GetDoubleList(line);													//; Coefficients to the relative growth factor - from a regression against climate (Int, Temp, Precip)
					TundraClimGrowthTip			= GetTip(line);								line = sr.ReadLine();
					TundraCalFactor 			= GetDoubleList(line);													//; Calibration factors to move from cohort growth model to exponential growth model - {growth, seed set}
					TundraCalFactorTip			= GetTip(line);								line = sr.ReadLine();
					TundraSpruceBasalArea 		= Convert.ToDouble(GetVal(line));													//; Basal area at which tundra transitions to spruce (m2/Ha).  A mature spruce stand is assumed to be about double this.
					TundraSpruceBasalAreaTip	= GetTip(line);
					//Find Black Spruce Settings.
					while ((line = sr.ReadLine()) != "; PARAMETERS FOR THE BLACK SPRUCE FRAME")	{ if (line==null) throw new Exception("'PARAMETERS FOR THE BLACK SPRUCE FRAME' missing.\n");}
					//Parse Black Spruce Settings.
					line = sr.ReadLine();
					BSpruce 					= Convert.ToInt32(GetVal(line));
					BSpruceTip					= GetTip(line);								line = sr.ReadLine();
					BSpruceStartAge 			= GetDoubleList(line);	
					BSpruceStartAgeTip			= GetTip(line);								line = sr.ReadLine();
					BSpruceFireParms 			= GetDoubleList(line);	
					BSpruceFireParmsTip			= GetTip(line);								line = sr.ReadLine();
					BSpruceHumanFireProb		= Convert.ToSingle(GetVal(line));
					BSpruceHumanFireProbTip		= GetTip(line);								line = sr.ReadLine();
					//Find White Spruce Settings.
					while ((line = sr.ReadLine()) != "; PARAMETERS FOR THE WHITE SPRUCE FRAME")	{ if (line==null) throw new Exception("'PARAMETERS FOR THE WHITE SPRUCE FRAME' missing.\n");}
					//Parse White Spruce Settings.
					line = sr.ReadLine();
					WSpruce 					= Convert.ToInt32(GetVal(line));
					WSpruceTip					= GetTip(line);								line = sr.ReadLine();
					WSpruceStartAge 			= GetDoubleList(line);
					WSpruceStartAgeTip			= GetTip(line);								line = sr.ReadLine();
					WSpruceFireParms 			= GetDoubleList(line);
					WSpruceFireParmsTip			= GetTip(line);								line = sr.ReadLine();
					WSpruceHumanFireProb		= Convert.ToSingle(GetVal(line));
					WSpruceHumanFireProbTip		= GetTip(line);								line = sr.ReadLine();
					//Find Deciduous Settings.
					while ((line = sr.ReadLine()) != "; PARAMETERS FOR THE DECIDUOUS FRAME")	{ if (line==null) throw new Exception("'PARAMETERS FOR THE DECIDUOUS FRAME' missing.\n");}
					//Parse Deciduous Settings.
					line = sr.ReadLine();
					Decid 						= Convert.ToInt32(GetVal(line));
					DecidTip					= GetTip(line);								line = sr.ReadLine();
					DecidStartAgeWSpruce 		= GetDoubleList(line);
					DecidStartAgeWSpruceTip		= GetTip(line);								line = sr.ReadLine();
					DecidStartAgeBSpruce 		= GetDoubleList(line);
					DecidStartAgeBSpruceTip		= GetTip(line);								line = sr.ReadLine();
					DecidFireProb 				= Convert.ToSingle(GetVal(line));
					DecidFireProbTip			= GetTip(line);								line = sr.ReadLine();
					DecidHumanFireProb			= Convert.ToSingle(GetVal(line));
					DecidHumanFireProbTip		= GetTip(line);								line = sr.ReadLine();
					DecidHistory 				= Convert.ToInt32(GetVal(line));
					DecidHistoryTip				= GetTip(line);								line = sr.ReadLine();
					DecidTundraParms 			= GetDoubleList(line);
					DecidTundraParmsTip			= GetTip(line);								line = sr.ReadLine();
					DecidBSpruceParms 			= GetDoubleList(line);
					DecidBSpruceParmsTip		= GetTip(line);								line = sr.ReadLine();
					DecidWSpruceParms 			= GetDoubleList(line);
					DecidWSpruceParmsTip		= GetTip(line);								line = sr.ReadLine();
				}
			}
			catch (Exception E) 
			{
                StringBuilder sb = new StringBuilder();
                string errorLine = line.Split(';')[0].Trim();

                sb.Append("Error occurred while evaluating the following line:\n");
                if (!string.IsNullOrEmpty(errorLine))
                    sb.Append("\t"+errorLine+"\n");
                else
                    sb.Append("\tEMPTY LINE\n");
                sb.Append("System message:\n");
                sb.Append("\t"+E.Message+"\n");

                throw new Exception(sb.ToString());
			}
		}

        public void             LoadLite(string filename)
        {	
			if (!File.Exists(filename))	
                throw new Exception("File " + filename + " does not exist.\n");
            this.FileName = filename;

            //Parse only values needed for server.
            string temp;
            MaxYears        = Convert.ToInt32(GetValLite("MaxYears"));
            MaxReps         = Convert.ToInt32(GetValLite("MaxReps"));
            //TimeStep        = Convert.ToInt32(GetValLite("TimeStep"));
            BaseDirectory   = GetValLite("ServerBaseDirectory");
            ClusterBaseDirectory = GetValLite("ClusterBaseDirectory");
            RandSeed        = GetValLite("RandSeed");
            OutputDirectory = GetValLite("Output.Directory"); //BaseDirectory + "\\" + GetValLite("Output.Directory");
            OutputType      = ((temp = GetValLite("Output.Type")) == "DELETE" ? EOutputType.DELETEOUTPUTDIRECTORY : (temp == "OVERWRITE" ? EOutputType.OVERWRITE : EOutputType.APPENDDATETIME));
        }

        private string GetValLite(string key)
        {
            string line = "";
            //1. Find line.
            using (StreamReader sr = new StreamReader(FileName))
            {
                bool found = false;
                while (!sr.EndOfStream) 
                {
                    line = sr.ReadLine().Trim();
                    if (line.Length >= key.Length) 
                    {
                        if (line.Substring(0, key.Length) == key) 
                        {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found)
                    throw new Exception("Field not found: " + key);
            }

            //2. Extract value(s) from line.
            if (!line.Contains(";")) throw new Exception("Invalid format.  Missing ';' in the following line: " + line);
            if (!line.Contains("=")) throw new Exception("Invalid format.  Missing '=' in the following line: " + line);
            int begin = line.IndexOf('=') + 1;
            int end = line.IndexOf(';');
            line = line.Substring(begin, end - begin).Trim();
            line = line.Replace("\"","");
            line = line.Replace("{", "");
            line = line.Replace("}", "");
            return line;

            //3. Convert string to generics type.
        }
		
        private string			GetVal(string Line) 
		{
			string	Setting		= "";
			string	Value		= "";
			int		Begin		= 0;
			int		End			= 0;
			Setting	= Line.Substring(0,Line.IndexOf(' ')).Trim();
			Begin	= Line.IndexOf('=')+1;
            End     = Line.IndexOfAny(new char[] {';','\t'});
			Value	= Line.Substring(Begin,End-Begin).Trim();
            if (Value.Length > 0)
			    if (Value[0]=='\"') Value = Value.Substring(1,Value.Length-2);
			return Value;
		}
		private List<string>	GetStringList(string line) 
		{
			char[]			delims	= {' ','=','{','}',',','\t'};
			List<string>	vals	= new List<string>();
			int				count	= 1;
			words = line.Split(delims);
			while(count<words.Length) {
				while (words.GetValue(count).ToString()=="") 
				{	
					count++;
					if (count>=words.Length) return vals;
				}
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(words.GetValue(count++).ToString().Replace("\"",""));
			}
			return vals;
		}
		private List<int>		GetIntList(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};
			List<int>	vals	= new List<int>();
			int			count	= 1;
			words = line.Split(delims);
			while(count<words.Length) 
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(Convert.ToInt32(words.GetValue(count++).ToString()));
			}
			return vals;
		}
		private List<int>		GetHexList(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};
			List<int>	vals	= new List<int>();
			int			count	= 1;
			words = line.Split(delims);
			while(count<words.Length) 
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(System.Int32.Parse(words[count].Substring(2),NumberStyles.HexNumber));
				count++;
				//vals.Add(Convert.ToInt32(words.GetValue(count++).ToString()));
			}
			return vals;
		}
		private List<float>		GetFloatList(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};
			List<float>	vals	= new List<float>();
			int			count	= 1;
			words = line.Split(delims);
			while(count<words.Length)
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(Convert.ToSingle(words.GetValue(count++).ToString()));
			}
			return vals;
		}
		private List<double>	GetDoubleList(string line) 
		{
			char[]			delims	= {' ','=','{','}',',','\"','\t'};
			List<double>	vals	= new List<double>();
			int				count	= 1;
			words = line.Split(delims);
			while(count<words.Length)
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(Convert.ToDouble(words.GetValue(count++).ToString()));
			}
			return vals;
		}
        private List<bool>      GetBoolList(string line)
		{
			char[]			delims	= {' ','=','{','}',',','\"','\t'};
			List<bool>	vals	= new List<bool>();
			int				count	= 1;
			words = line.Split(delims);
			while(count<words.Length)
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(words.GetValue(count++).ToString() == "TRUE" ? true : false);
			}
			return vals;
        }
		private string			GetTip(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};		//Semicolon removed from list.
			string		tip		= "";
			int			i		= 0;
			words = line.Split(delims);									
			//Find begining of the tooltip.
			while (words.GetValue(i).ToString() != ";")			i++;			
			//Concat remaining words into the tooltip.
			for (++i; i < words.Length; i++)					tip += words.GetValue(i).ToString() + " "; 
			return tip;
		}
		public string			Format(bool Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			Return.Append(Setting?"TRUE":"FALSE");
			Return.Append(' ' ,(Setting?46:45));
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(int Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			String Value = Setting.ToString("0");
			Return.Append(Value);
			Return.Append(' ' ,50-Value.Length);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(float Setting)
		{
			return Format((double)Setting);
		}
		public string			Format(double Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			String Value = Setting.ToString();
			Return.Append(Value);
			Return.Append(' ' ,50-Value.Length);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(string Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			int				PadLength	= 0;
			//Create pad string.
			PadLength = (Setting.Length<48 ? 48 : 78) - Setting.Length;
			//Create formatted setting.
			Return.Append("\"");
			Return.Append(Setting);
			Return.Append("\"");
			Return.Append(' ',PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
        public string           Format(List<bool> Setting)
        {
            StringBuilder Return = new StringBuilder("", 255);
            StringBuilder Value = new StringBuilder("", 255);
            int PadLength = 0;
            if (Setting.Count > 0)
            {
                for (int i = 0; i < Setting.Count; i++)
                {
                    Value.Append("");
                    Value.Append(Setting[i].ToString() == bool.TrueString ? "TRUE" : "FALSE");
                    Value.Append(", ");
                }
                Value.Remove(Value.Length - 2, 2);
            }
            //Create pad string.
            PadLength = (Value.Length < 48 ? 48 - Value.Length : (Value.Length < 78 ? 78 - Value.Length : 4));
            Return.Append("{");
            Return.Append(Value);
            Return.Append("}");
            Return.Append(' ', PadLength);
            Return.Append("; ");
            return Return.ToString();
        }
        public string			Format(List<int> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("");
					Value.Append(Setting[i].ToString("0"));
					Value.Append(", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
        public string           Format(List<float> Setting)
        {
            StringBuilder Return = new StringBuilder("", 255);
            StringBuilder Value = new StringBuilder("", 255);
            int PadLength = 0;
            if (Setting.Count > 0) {
                for (int i = 0; i < Setting.Count; i++) {
                    Value.Append("");
                    Value.Append(Setting[i].ToString());
                    Value.Append(", ");
                }
                Value.Remove(Value.Length - 2, 2);
            }
            //Create pad string.
            PadLength = (Value.Length < 48 ? 48 - Value.Length : (Value.Length < 78 ? 78 - Value.Length : 4));
            Return.Append("{");
            Return.Append(Value);
            Return.Append("}");
            Return.Append(' ', PadLength);
            Return.Append("; ");
            return Return.ToString();
        }
        public string           Format(List<double> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("");
					Value.Append(Setting[i].ToString());
					Value.Append(", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(List<string> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("\"");
					Value.Append(Setting[i]);
					Value.Append("\", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(string Setting,FIF.EnumFIFSettingType Type)
		{
			string	Return		= "";
			string	Spaces		= "";
			int		PadLength	= 0;
			//Create pad string.
			PadLength	= (Setting.Length<50 ? 50-Setting.Length : (Setting.Length<80 ? 80-Setting.Length : 4));
			PadLength  -= (Type==FIF.EnumFIFSettingType.STRING ? 2 : 0);
			for (int i=0;i<PadLength;i++) Spaces += " ";
			//Create formatted setting.
			switch (Type) {
			case FIF.EnumFIFSettingType.BOOL :
				goto case FIF.EnumFIFSettingType.FLOAT;
			case FIF.EnumFIFSettingType.ENUM :
				goto case FIF.EnumFIFSettingType.FLOAT;
			case FIF.EnumFIFSettingType.INT :
				goto case FIF.EnumFIFSettingType.FLOAT;
			case FIF.EnumFIFSettingType.FLOAT :
				Return = Setting + Spaces;
				break;
			case FIF.EnumFIFSettingType.STRING : 			
				Return = "\"" + Setting + "\"" + Spaces;
				break;
			}
			Return += "; ";
			return Return;
		}
		public string			FormatHex(int Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			String Value = Setting.ToString("x");
			Return.Append("0x");
			Return.Append(Value);
			Return.Append(' ' ,48-Value.Length);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			FormatHex(List<int> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("0x");
					Value.Append(Setting[i].ToString("x"));
					Value.Append(", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
	}
}
