using System;
using System.Text;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;
//using FRESCOOL;

namespace FRESCO_Server
{
	public partial class EditorFIF : Editor
	{
        #region Map codes.
		//General codes - if not specified, nothing will get printed  
		private int outMapData		= 0x1;							//Output the data
		//Format codes - specify the format of the output data and headers
		private int outMapFormat	= 0x10;							//Output the data so it is conducive to read it in into a stats package for analysis
		private int outHeader		= 0x20;							//Tack a header onto the output with the number of years/reps for reference purposes
		private int outARC			= 0x40;							//Attach an header to the output file to facilitate import into ARCgis
		//Frame codes - used with outFrame
		private int out1			= 0x100;						//The exact data output is determined by the implementation of the frame class
		private int out2			= 0x200;
		private int out3			= 0x400;
		private int out4			= 0x800;
		//Data codes - used with outData
		private int outVeg			= 0x1000;						//The veg type
		private int outAge			= 0x2000;						//The stand age
		private int outSite			= 0x4000;						//The site variables
		private int outSub			= 0x8000;						//The subcanopy
		private int outFireAge		= 0x10000;						//The time since the cell last burned
		private int outTemp			= 0x20000;						//The temp offset
		private int outPrecip		= 0x40000;						//The precip offset
		private int outFireScar		= 0x80000;						//The ID of each fire scar [If ignition cell, '-'][LastBurnYear].[FireID]
		//Stat codes
		private int outYear		= 0x1;								//Output the data averaged by year
		private int outRep		= 0x2;								//Output the data averaged by replicate
		private int outFormat	= 0x4;								//Format in a way to facilitate reading into a stats package for analysis
		private int outNum		= 0x10;								//Output the number of samples
		private int outMean		= 0x20;								//Output the mean of the data
		private int outStdDev	= 0x40;								//Output the standard deviation of the samples
		private int outMin		= 0x80;								//Output the minimum of the samples
		private int outMax		= 0x100;							//Output the maximum of the samples
		private int outHist		= 0x200;							//Output the histogram data if there is any
		private int outData		= 0x400;							//Output the file of data.
		private int outEvents	= 0x800;							//Output the file of events.
        #endregion

        private int	storedMapIndex	= -1;
		private int FireTransitionToSave = -1;
        private int ClimateTransitionToSave = -1;
		
		public						EditorFIF()
		{
			InitializeComponent();
		}	
		public						EditorFIF(Global G) : base(G)
		{
			InitializeComponent();
            Global.ProgramStateEvent += new ProgramStateEventHandler(ProgramStateChanged);
		}
		public override void		Refresh()
		{
			FIF	FIF;
			FormState=EnumFormState.REFRESH;
			FIF = Global.FIF;
			fldFileName.Text						=	FIF.FileName;
			//General.
			txbGenBaseDirectory.Text				=	FIF.BaseDirectory;									ToolTips.SetToolTip(txbGenBaseDirectory, FIF.BaseDirectoryTip);
			txbGenMaxYears.Text						=	FIF.MaxYears.ToString();							ToolTips.SetToolTip(txbGenMaxYears,	FIF.MaxYearsTip);
			txbGenMaxReps.Text						=	FIF.MaxReps.ToString();								ToolTips.SetToolTip(txbGenMaxReps, FIF.MaxRepsTip);
			txbGenTimeStep.Text						=	FIF.TimeStep.ToString();							ToolTips.SetToolTip(txbGenTimeStep, FIF.TimeStepTip);
			cbbGenRandSeed.Text						=	FIF.RandSeed;										ToolTips.SetToolTip(cbbGenRandSeed, FIF.RandSeedTip);
			txbGenNumHeader.Text					=	FIF.NumHeader.ToString();							ToolTips.SetToolTip(txbGenNumHeader, FIF.NumHeaderTip);
			txbGenNoData.Text						=	FIF.NoData.ToString();								ToolTips.SetToolTip(txbGenNoData, FIF.NoDataTip);
			txbGenOutputDirectory.Text				=	FIF.OutputDirectory;								ToolTips.SetToolTip(txbGenOutputDirectory, FIF.OutputDirectoryTip);
			cbbGenOutputType.Text					=	(FIF.OutputType==EOutputType.DELETEOUTPUTDIRECTORY ? "DELETE" : (FIF.OutputType==EOutputType.OVERWRITE ? "OVERWRITE" : "APPEND"));							ToolTips.SetToolTip(cbbGenOutputDetailLevel, FIF.OutputDetailLevelTip);
			chkGenIsLargeMemoryModel.Checked		=	FIF.IsLargeMemoryModel;								ToolTips.SetToolTip(chkGenIsLargeMemoryModel, FIF.IsLargeMemoryModelTip);
			//txbGenOutputDotDisplay.Text				=	FIF.OutputDotDisplay.ToString();					ToolTips.SetToolTip(txbGenOutputDotDisplay, FIF.OutputDotDisplayTip);
			cbbGenOutputDetailLevel.Text			=	(FIF.OutputDetailLevel==EDetailLevel.MINIMAL ? "MINIMAL" : (FIF.OutputDetailLevel==EDetailLevel.MODERATE ? "MODERATE" : "MAXIMUM"));						ToolTips.SetToolTip(cbbGenOutputDetailLevel, FIF.OutputDetailLevelTip);
			//Landscape.
			txbLandMaxRow.Text						=	FIF.MaxRow.ToString();								ToolTips.SetToolTip(txbLandMaxRow, FIF.MaxRowTip);
			txbLandMaxCol.Text						=	FIF.MaxCol.ToString();								ToolTips.SetToolTip(txbLandMaxCol, FIF.MaxColTip);
			txbLandCellSize.Text					=	FIF.CellSize.ToString();							ToolTips.SetToolTip(txbLandCellSize, FIF.CellSizeTip);
			txbLandNumSpecies.Text					=	FIF.NumSpecies.ToString();							ToolTips.SetToolTip(txbLandNumSpecies, FIF.NumSpeciesTip);
			txbLandNoVeg.Text						=	FIF.NoVeg.ToString();								ToolTips.SetToolTip(txbLandNoVeg, FIF.NoVegTip);
			txbLandXllCorner.Text					=	FIF.XLLCorner.ToString();							ToolTips.SetToolTip(txbLandXllCorner, FIF.XLLCornerTip);
			txbLandYllCorner.Text					=	FIF.YLLCorner.ToString();							ToolTips.SetToolTip(txbLandYllCorner, FIF.YLLCornerTip);
            chkLandIsUsingUniqueVegAndAgePerRep.Checked = FIF.IsUsingUniqueVegAndAgePerRep;                 ToolTips.SetToolTip(chkLandIsUsingUniqueVegAndAgePerRep, FIF.IsUsingUniqueVegAndAgePerRepTip);
			txbLandYearOfVegAndAgeFiles.Text		=	FIF.YearOfVegAndAgeFiles.ToString();				ToolTips.SetToolTip(txbLandYearOfVegAndAgeFiles, FIF.YearOfVegAndAgeFilesTip);
            txbLandVegInputFile.Text				=	FIF.VegInputFile.ToString();						ToolTips.SetToolTip(txbLandVegInputFile, FIF.VegInputFileTip);
			txbLandVegTransitionFile.Text			=	FIF.VegTransitionFile.ToString();					ToolTips.SetToolTip(txbLandVegTransitionFile, FIF.VegTransitionFileTip);
			chkLandIsForcedVegTransitions.Checked	=	FIF.IsForcedVegTransitions;							ToolTips.SetToolTip(chkLandIsForcedVegTransitions, FIF.IsForcedVegTransitionsTip);
			txbLandAgeInputFile.Text				=	FIF.AgeInputFile.ToString();						ToolTips.SetToolTip(txbLandAgeInputFile, FIF.AgeInputFileTip);
			txbLandSiteInputFile.Text				=	FIF.SiteInputFile.ToString();						ToolTips.SetToolTip(txbLandSiteInputFile, FIF.SiteInputFileTip);
			txbLandTreeDensityInputFile.Text		=	FIF.TreeDensityInputFile.ToString();				ToolTips.SetToolTip(txbLandTreeDensityInputFile, FIF.TreeDensityInputFileTip);
			chkLandCropNeighbors.Checked			=	FIF.IsCropNeighbors;								ToolTips.SetToolTip(chkLandCropNeighbors, FIF.CropNeighborsTip);
			//Climate.
            lstClimTransitions.Items.Clear();
            ClimateTransitionsRefresh();
            txbClimTransitionYear.Text              = "";                                                   ToolTips.SetToolTip(txbClimTransitionYear, FIF.ClimateTransitionYearsTip);
            cbbClimValuesType.Text                  = "";                                                   ToolTips.SetToolTip(cbbClimValuesType, FIF.ClimValuesTypesTip);
			txbClimNumHistory.Text					=	FIF.ClimNumHistory.ToString();						ToolTips.SetToolTip(txbClimNumHistory, FIF.ClimNumHistoryTip);
			cbbClimValuesType.Text					= "";				                        			ToolTips.SetToolTip(cbbClimValuesType, FIF.ClimValuesTypesTip);
            txbClimValuesTemp.Text                  = "";                                                   ToolTips.SetToolTip(txbClimValuesTemp, FIF.ClimValuesTempTip);
			txbClimValuesPrecip.Text				= "";                                                   ToolTips.SetToolTip(txbClimValuesPrecip, FIF.ClimValuesPrecipTip);
            txbClimRandExplicitMinYear.Text         = "";                                                   ToolTips.SetToolTip(txbClimRandExplicitMinYear, FIF.ClimValuesRandExplicitMinYearsTip);
            txbClimRandExplicitMaxYear.Text         = "";                                                   ToolTips.SetToolTip(txbClimRandExplicitMaxYear, FIF.ClimValuesRandExplicitMaxYearsTip);
            chkClimValuesRandExplicitIsReplicated.Checked = false;                                          ToolTips.SetToolTip(chkClimValuesRandExplicitIsReplicated, FIF.ClimValuesRandExplicitIsReplicatedTip);
            cbbClimOffsetsType.Text                 = "";                                                   ToolTips.SetToolTip(cbbClimOffsetsType, FIF.ClimOffsetsTypesTip);
            chkClimOffsetsRandIsReplicated.Checked  = false;                                                ToolTips.SetToolTip(chkClimOffsetsRandIsReplicated, FIF.ClimOffsetsRandIsReplicatedTip);
			txbClimOffsetsFile.Text					= "";                           						ToolTips.SetToolTip(txbClimOffsetsFile, FIF.ClimOffsetsFilesTip);
			txbClimOffsetsTempConstant.Text			= "";                           						ToolTips.SetToolTip(txbClimOffsetsTempConstant, FIF.ClimOffsetsTempsTip);
			txbClimOffsetsPrecipConstant.Text		= "";                           						ToolTips.SetToolTip(txbClimOffsetsPrecipConstant, FIF.ClimOffsetsPrecipsTip);
			txbClimOffsetsTempMean.Text				= "";                           						ToolTips.SetToolTip(txbClimOffsetsTempMean, FIF.ClimOffsetsRandTempMeansTip);
			txbClimOffsetsTempStdDev.Text			= "";                           						ToolTips.SetToolTip(txbClimOffsetsTempStdDev, FIF.ClimOffsetsRandTempStdDevsTip);
			txbClimOffsetsPrecipMean.Text			= "";                           						ToolTips.SetToolTip(txbClimOffsetsPrecipMean, FIF.ClimOffsetsRandPrecipMeansTip);
			txbClimOffsetsPrecipStdDev.Text			= "";                           						ToolTips.SetToolTip(txbClimOffsetsPrecipStdDev, FIF.ClimOffsetsRandPrecipStdDevsTip);
			chkStepsAndRampsEnabled.Checked         =   FIF.ClimStepsAndRampsEnabled;                       ToolTips.SetToolTip(chkStepsAndRampsEnabled, FIF.ClimStepsAndRampsEnabledTip);
            //TODO: Error if for example: ___StepYear.count != ___Step.count
			//Clear lists before adding to them.
			lstClimTempSteps.Items.Clear();
			lstClimTempRamps.Items.Clear();
			lstClimPrecipSteps.Items.Clear();
			lstClimPrecipRamps.Items.Clear();
			for (int i=0; i<FIF.ClimOffsetsTempStepYear.Count; i++)
				lstClimTempSteps.Items.Add(FIF.ClimOffsetsTempStepYear[i] + ": " + FIF.ClimOffsetsTempStep[i]);
			for (int i=0; i<FIF.ClimOffsetsTempRampYear.Count; i++)
				lstClimTempRamps.Items.Add(FIF.ClimOffsetsTempRampYear[i] + ": " + FIF.ClimOffsetsTempRamp[i]);
			for (int i=0; i<FIF.ClimOffsetsPrecipStepYear.Count; i++)
				lstClimPrecipSteps.Items.Add(FIF.ClimOffsetsPrecipStepYear[i] + ": " + FIF.ClimOffsetsPrecipStep[i]);
			for (int i=0; i<FIF.ClimOffsetsPrecipRampYear.Count; i++)
				lstClimPrecipRamps.Items.Add(FIF.ClimOffsetsPrecipRampYear[i] + ": " + FIF.ClimOffsetsPrecipRamp[i]);
			//Fire.
			lstFireTypeTransitions.Items.Clear();
			FireTransitionsRefresh();
			txbFireTypeTransitionYear.Text			=	"";													ToolTips.SetToolTip(txbFireTypeTransitionYear, FIF.FireTypeTransitionYearTip);
			cbbFireType.Text						=	"";													ToolTips.SetToolTip(cbbFireType, FIF.FireTypeTransitionTip);
			chkFireEnabled.Checked					=	FIF.FireEnabled;									ToolTips.SetToolTip(chkFireEnabled, FIF.FireEnabledTip);
			txbFireSpreadRadius.Text 				= 	FIF.FireSpreadRadius.ToString();					ToolTips.SetToolTip(txbFireSpreadRadius, FIF.FireSpreadRadiusTip);
			txbFireSpreadMean.Text					= 	FIF.FireSpreadParms[0].ToString();					ToolTips.SetToolTip(txbFireSpreadMean, FIF.FireSpreadParmsTip); 
			txbFireSpreadStdDev.Text				= 	FIF.FireSpreadParms[1].ToString();					ToolTips.SetToolTip(txbFireSpreadStdDev, FIF.FireSpreadParmsTip); 
			txbFireClimateA.Text	 				= 	FIF.FireClimate[0].ToString();						ToolTips.SetToolTip(txbFireClimateA, FIF.FireClimateTip); 
			txbFireClimateB.Text	 				= 	FIF.FireClimate[1].ToString();						ToolTips.SetToolTip(txbFireClimateB, FIF.FireClimateTip); 
			txbFireClimateC.Text	 				= 	FIF.FireClimate[2].ToString();						ToolTips.SetToolTip(txbFireClimateC, FIF.FireClimateTip); 
			chkFireIgnoreFirstInterval.Checked		=  	FIF.FireIgnoreFirstInterval;						ToolTips.SetToolTip(chkFireIgnoreFirstInterval, FIF.FireIgnoreFirstIntervalTip); 
			txbFireIgnition.Text				    =  	"";													ToolTips.SetToolTip(txbFireIgnition, FIF.FireIgnitionTip); 
			txbFireSensitivity.Text					=  	"";													ToolTips.SetToolTip(txbFireSensitivity, FIF.FireSensitivityTip); 
			txbFireMaxEmpiricalFireSizeEvent.Text   =   FIF.FireMaxEmpiricalFireSizeEvent.ToString();                  ToolTips.SetToolTip(txbFireMaxEmpiricalFireSizeEvent,FIF.FireMaxEmpiricalFireSizeEventTip);
			txbFireMaxEmpiricalFireSizeEventWeight.Text  =   FIF.FireMaxEmpiricalFireSizeEventWeight.ToString();            ToolTips.SetToolTip(txbFireMaxEmpiricalFireSizeEventWeight,FIF.FireMaxEmpiricalFireSizeEventWeightTip);
            txbFireHistorical.Text					=  	FIF.FireHistorical.ToString();						ToolTips.SetToolTip(txbFireHistorical, FIF.FireHistoricalTip); 
			txbFireHumanIgnitionFile.Text			=  	FIF.FireHumanIgnitionFile.ToString();				ToolTips.SetToolTip(txbFireHumanIgnitionFile, FIF.FireHumanIgnitionFileTip); 
//			txbFireHumanIgnitionProb.Text			=  	FIF.FireHumanIgnitionProb.ToString();				ToolTips.SetToolTip(txbFireHumanIgnitionProb, FIF.FireHumanIgnitionProbTip); 
			chkFireSuppressionOn.Checked			=	FIF.FireSuppressionOn;								ToolTips.SetToolTip(chkFireSuppressionOn, FIF.FireSuppressionOnTip);
			txbFireSuppressionFile.Text				=	FIF.FireSuppressionFile.ToString();					ToolTips.SetToolTip(txbFireSuppressionFile, FIF.FireSuppressionFileTip);
			txbFireSuppressionClass1.Text			=	FIF.FireSuppressionClass1.ToString();				ToolTips.SetToolTip(txbFireSuppressionClass1, FIF.FireSuppressionClass1Tip);
			txbFireSuppressionClass2.Text			=	FIF.FireSuppressionClass2.ToString();				ToolTips.SetToolTip(txbFireSuppressionClass2, FIF.FireSuppressionClass2Tip);
			txbFireSuppressionClass3.Text			=	FIF.FireSuppressionClass3.ToString();				ToolTips.SetToolTip(txbFireSuppressionClass3, FIF.FireSuppressionClass3Tip);
			txbFireSuppressionClass4.Text			=	FIF.FireSuppressionClass4.ToString();				ToolTips.SetToolTip(txbFireSuppressionClass4, FIF.FireSuppressionClass4Tip);
			txbFireSuppressionClass5.Text			=	FIF.FireSuppressionClass5.ToString();				ToolTips.SetToolTip(txbFireSuppressionClass5, FIF.FireSuppressionClass5Tip);
			txbFireSuppressionThresholdFireSize.Text	=	FIF.FireSuppressionThresholdFireSize.ToString();ToolTips.SetToolTip(txbFireSuppressionThresholdFireSize, FIF.FireSuppressionThresholdFireSizeTip);
			txbFireSuppressionThresholdIgnitions.Text	=	FIF.FireSuppressionThresholdIgnitions.ToString();ToolTips.SetToolTip(txbFireSuppressionThresholdIgnitions, FIF.FireSuppressionThresholdIgnitionsTip);
			txbFireSpatialIgnition.Text	 	=  	"";													ToolTips.SetToolTip(txbFireSpatialIgnition, FIF.FireSpatialIgnitionTip); 
			txbFireSpatialSensitivity.Text	 		=  	"";													ToolTips.SetToolTip(txbFireSpatialSensitivity, FIF.FireSpatialSensitivityTip); 
//			txbFireSpatialIgnitionFactor.Text	 	=  	FIF.FireSpatialIgnitionFactor.ToString();			ToolTips.SetToolTip(txbFireSpatialIgnitionFactor, FIF.FireSpatialIgnitionFactorTip); 
//			txbFireSpatialSensitivity.Text	 		=  	FIF.FireSpatialSensitivity.ToString();				ToolTips.SetToolTip(txbFireSpatialSensitivity, FIF.FireSpatialSensitivityTip); 
			//Map Output.
			txbMapCodes.Text 						=	"";													ToolTips.SetToolTip(txbMapCodes, FIF.MapCodesTip); 
			lblMapFlags.Text						=	"";													ToolTips.SetToolTip(lblMapFlags, FIF.MapFlagsTip); 
			txbMapRepStart.Text						=	"";													ToolTips.SetToolTip(txbMapRepStart, FIF.MapRepStartTip);
			txbMapRepFreq.Text						=	""; 												ToolTips.SetToolTip(txbMapRepFreq, FIF.MapRepFreqTip);
			txbMapYearStart.Text					=	"";													ToolTips.SetToolTip(txbMapYearStart, FIF.MapYearStartTip);
			txbMapYearFreq.Text						=	"";													ToolTips.SetToolTip(txbMapYearFreq, FIF.MapYearFreqTip);
			//TODO: Give tooltips to flag checkboxes.
			txbMapCodes.Text = lblMapFlags.Text = txbMapRepStart.Text = txbMapRepFreq.Text = txbMapYearStart.Text = txbMapYearFreq.Text = "";
			cbbMapOutputSelect.Items.Clear();
			for (int i=0; i<FIF.MapFiles.Count; i++)
				cbbMapOutputSelect.Items.Add(FIF.MapFiles[i].ToString());
			RefreshCbbMapOutputSelect();
			cbbMapOutputSelect.Text = "";
			//Stat Output.
			txbStatSummaryFile.Text					=	FIF.StatSummaryFile.ToString();						ToolTips.SetToolTip(txbStatSummaryFile, FIF.StatSummaryFileTip);
			RefreshCbbStatSelect();		
			//Tundra.
			txbTundraStartAge.Text					=	FIF.TundraStartAge[0].ToString()	+ ", " + FIF.TundraStartAge[1].ToString();		ToolTips.SetToolTip(txbTundraStartAge, FIF.TundraStartAgeTip);
			txbTundraFireProb.Text					=	FIF.TundraFireProb.ToString();						ToolTips.SetToolTip(txbTundraFireProb, FIF.TundraFireProbTip);
			txbTundraHumanFireProb.Text				=	FIF.TundraHumanFireProb.ToString();					ToolTips.SetToolTip(txbTundraHumanFireProb,FIF.TundraHumanFireProbTip);
			txbTundraSpruceEstBA.Text				=	FIF.TundraSpruceEstBA.ToString();					ToolTips.SetToolTip(txbTundraSpruceEstBA, FIF.TundraSpruceEstBATip);
			txbTundraSeedRange.Text					=	FIF.TundraSeedRange.ToString();						ToolTips.SetToolTip(txbTundraSeedRange, FIF.TundraSeedRangeTip);
			txbTundraSeedSource.Text				=	FIF.TundraSeedSource[0].ToString()	+ ", " + FIF.TundraSeedSource[1].ToString();		ToolTips.SetToolTip(txbTundraSeedSource, FIF.TundraSeedSourceTip);
			txbTundraSeedBasalArea.Text				=	FIF.TundraSeedBasalArea.ToString();					ToolTips.SetToolTip(txbTundraSeedBasalArea, FIF.TundraSeedBasalAreaTip);
			txbTundraSeedling.Text					=	FIF.TundraSeedling.ToString();						ToolTips.SetToolTip(txbTundraSeedling, FIF.TundraSeedlingTip);
			txbTundraSeedlingBA.Text				=	FIF.TundraSeedlingBA.ToString();					ToolTips.SetToolTip(txbTundraSeedlingBA, FIF.TundraSeedlingBATip);
			txbTundraHistory.Text					=	FIF.TundraHistory.ToString();						ToolTips.SetToolTip(txbTundraHistory, FIF.TundraHistoryTip);
			txbTundraSeedEstParms.Text				=	FIF.TundraSeedEstParms[0].ToString()	+ ", " + FIF.TundraSeedEstParms[1].ToString();		ToolTips.SetToolTip(txbTundraSeedEstParms, FIF.TundraSeedEstParmsTip);
			txbTundraMeanGrowth.Text				=	FIF.TundraMeanGrowth.ToString();					ToolTips.SetToolTip(txbTundraMeanGrowth, FIF.TundraMeanGrowthTip);
			txbTundraClimGrowth.Text				=	FIF.TundraClimGrowth[0].ToString()	+ ", " + FIF.TundraClimGrowth[1].ToString() + ", " + FIF.TundraClimGrowth[2].ToString();		ToolTips.SetToolTip(txbTundraClimGrowth, FIF.TundraClimGrowthTip);
			txbTundraCalFactor.Text					=	FIF.TundraCalFactor[0].ToString()	+ ", " + FIF.TundraCalFactor[1].ToString();		ToolTips.SetToolTip(txbTundraCalFactor, FIF.TundraCalFactorTip);
			txbTundraSpruceBasalArea.Text			=	FIF.TundraSpruceBasalArea.ToString();				ToolTips.SetToolTip(txbTundraSpruceBasalArea, FIF.TundraSpruceBasalAreaTip);
			//Black Spruce.
			txbBSpruceStartAge.Text					=	FIF.BSpruceStartAge[0].ToString()	+ ", " + FIF.BSpruceStartAge[1].ToString();		ToolTips.SetToolTip(txbBSpruceStartAge, FIF.BSpruceStartAgeTip);
			txbBSpruceFireParms.Text				=	FIF.BSpruceFireParms[0].ToString()	+ ", " + FIF.BSpruceFireParms[1].ToString()	+ ", " + FIF.BSpruceFireParms[2].ToString();		ToolTips.SetToolTip(txbBSpruceFireParms, FIF.BSpruceFireParmsTip);
			txbBSpruceHumanFireProb.Text			=	FIF.BSpruceHumanFireProb.ToString();				ToolTips.SetToolTip(txbBSpruceHumanFireProb,FIF.BSpruceHumanFireProbTip);
			//White Spruce.
			txbWSpruceStartAge.Text					=	FIF.WSpruceStartAge[0].ToString()	+ ", " + FIF.WSpruceStartAge[1].ToString();		ToolTips.SetToolTip(txbWSpruceStartAge, FIF.WSpruceStartAgeTip);
			txbWSpruceFireParms.Text				=	FIF.WSpruceFireParms[0].ToString()	+ ", " + FIF.WSpruceFireParms[1].ToString()	+ ", " + FIF.WSpruceFireParms[2].ToString();		ToolTips.SetToolTip(txbWSpruceFireParms, FIF.WSpruceFireParmsTip);
			txbWSpruceHumanFireProb.Text			=	FIF.WSpruceHumanFireProb.ToString();				ToolTips.SetToolTip(txbWSpruceHumanFireProb,FIF.WSpruceHumanFireProbTip);
			//Deciduous.
			txbDecidStartAgeWSpruce.Text			=	FIF.DecidStartAgeWSpruce[0].ToString()	+ (FIF.DecidStartAgeWSpruce.Count < 2 ? "" : ", " + FIF.DecidStartAgeWSpruce[1].ToString());		ToolTips.SetToolTip(txbDecidStartAgeWSpruce, FIF.DecidStartAgeWSpruceTip);
			txbDecidStartAgeBSpruce.Text			=	FIF.DecidStartAgeBSpruce[0].ToString()	+ (FIF.DecidStartAgeBSpruce.Count < 2 ? "" : ", " + FIF.DecidStartAgeBSpruce[1].ToString());		ToolTips.SetToolTip(txbDecidStartAgeBSpruce, FIF.DecidStartAgeBSpruceTip);
			txbDecidFireProb.Text					=	FIF.DecidFireProb.ToString();						ToolTips.SetToolTip(txbDecidFireProb, FIF.DecidFireProbTip);
			txbDecidHumanFireProb.Text				=	FIF.DecidHumanFireProb.ToString();					ToolTips.SetToolTip(txbDecidHumanFireProb,FIF.DecidHumanFireProbTip);
			txbDecidHistory.Text					=	FIF.DecidHistory.ToString();						ToolTips.SetToolTip(txbDecidHistory, FIF.DecidHistoryTip);
			txbDecidTundraParms.Text				=	FIF.DecidTundraParms[0].ToString()	+ ", " + FIF.DecidTundraParms[1].ToString();		ToolTips.SetToolTip(txbDecidTundraParms, FIF.DecidTundraParmsTip);
			txbDecidBSpruceParms.Text				=	FIF.DecidBSpruceParms[0].ToString()	+ ", " + FIF.DecidBSpruceParms[1].ToString();	ToolTips.SetToolTip(txbDecidBSpruceParms, FIF.DecidBSpruceParmsTip);
			txbDecidWSpruceParms.Text				=	FIF.DecidWSpruceParms[0].ToString()	+ ", " + FIF.DecidWSpruceParms[1].ToString();	ToolTips.SetToolTip(txbDecidWSpruceParms, FIF.DecidWSpruceParmsTip);
		
			FormState=EnumFormState.DISPLAY;
		}
		public override void		Save()
		{
			FIF				FIF;
			List<int>		Years;
			List<double>	Values;
			List<int>		FireTypeTransitionYears = new List<int>();
			List<string>	FireTypeTransitions = new List<string>();
			List<double>	FireIgnitionFactors = new List<double>();
			List<double>	FireSensitivities = new List<double>();
			List<string>	FireSpatialIgnitionFactors = new List<string>();
			List<string>	FireSpatialSensitivities = new List<string>();
            List<int>       ClimateTransitionYears = new List<int>();
            List<string>    ClimateValuesTypes = new List<string>();
            List<float>     ClimateValuesConstantTemps = new List<float>();
            List<float>     ClimateValuesConstantPrecips = new List<float>();
            List<string>    ClimateValuesFileTemps = new List<string>();
            List<string>    ClimateValuesFilePrecips = new List<string>();
            List<int>       ClimateValuesRandExplicitMinYears = new List<int>();
            List<int>       ClimateValuesRandExplicitMaxYears = new List<int>();
            List<bool>      ClimateValuesRandExpliciteRepicatedFlags = new List<bool>();
            List<string>    ClimateOffsetsTypes = new List<string>();
            List<string>    ClimateOffsetsFiles = new List<string>();
            List<float>     ClimateOffsetsConstantTemps = new List<float>();
            List<float>     ClimateOffsetsConstantPrecips = new List<float>();
            List<float>     ClimateOffsetsRandomTempMeans = new List<float>();
            List<float>     ClimateOffsetsRandomTempStdDevs = new List<float>();
            List<float>     ClimateOffsetsRandomPrecipMeans = new List<float>();
            List<float>     ClimateOffsetsRandomPrecipStdDev = new List<float>();
            List<bool>      ClimateOffsetsRandomRepicatedFlags = new List<bool>();
            int            ParsedRandomSeed;

            StreamWriter fifFile;
			//Init
            try
            {
                if (ClimateTransitionToSave != -1)
                {
                    int savedIndex = ClimateTransitionToSave;
                    ClimateTransitionSave(ClimateTransitionToSave);
                    lstClimTransitions.SetSelected(savedIndex, true);
                }
                if (FireTransitionToSave != -1)
                {
                    int savedIndex = FireTransitionToSave;
                    FireTransitionSave(FireTransitionToSave);
                    lstFireTypeTransitions.SetSelected(savedIndex, true);
                }
                fifFile = new StreamWriter(Global.FIF.FileName, false, System.Text.Encoding.ASCII);
                Years = new List<int>();
                Values = new List<double>();
                FireTypeTransitionYears = new List<int>();
                FireTypeTransitions = new List<string>();
                FireIgnitionFactors = new List<double>();
                FireSensitivities = new List<double>();
                FireSpatialIgnitionFactors = new List<string>();
                FireSpatialSensitivities = new List<string>();
                FIF = Global.FIF;
                //General.
                fifFile.WriteLine(";FIF INPUT FILE FOR FRESCO");
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                fifFile.WriteLine("; PROGRAM PARAMETERS");
                fifFile.WriteLine("BaseDirectory                            = " + FIF.Format(txbGenBaseDirectory.Text) + FIF.BaseDirectoryTip);
                fifFile.WriteLine("MaxYears                                 = " + FIF.Format(Convert.ToInt32(txbGenMaxYears.Text)) + FIF.MaxYearsTip);
                fifFile.WriteLine("MaxReps                                  = " + FIF.Format(Convert.ToInt32(txbGenMaxReps.Text)) + FIF.MaxRepsTip);
                fifFile.WriteLine("TimeStep                                 = " + FIF.Format(Convert.ToInt32(txbGenTimeStep.Text)) + FIF.TimeStepTip);
                fifFile.WriteLine("RandSeed                                 = " + (int.TryParse(cbbGenRandSeed.Text, out ParsedRandomSeed) ? FIF.Format(ParsedRandomSeed) : FIF.Format(cbbGenRandSeed.Text)) + FIF.RandSeedTip);  //Could be "CURRENTTIME" or could be integer.
                fifFile.WriteLine("NumHeader                                = " + FIF.Format(Convert.ToInt32(txbGenNumHeader.Text)) + FIF.NumHeaderTip);
                fifFile.WriteLine("NoData                                   = " + FIF.Format(Convert.ToInt32(txbGenNoData.Text)) + FIF.NoDataTip);
                fifFile.WriteLine("Output.Directory                         = " + FIF.Format(txbGenOutputDirectory.Text) + FIF.OutputDirectoryTip);
                fifFile.WriteLine("Output.Type                              = " + FIF.Format(cbbGenOutputType.Text) + FIF.OutputTypeTip);
                fifFile.WriteLine("Output.DotDisplay                        = " + FIF.Format(0) + "No longer used.");
                fifFile.WriteLine("Output.ShowSeed                          = " + FIF.Format(true) + "Should the program display the random seed to the Progress Viewer?");
                fifFile.WriteLine("Output.DetailLevel                       = " + FIF.Format(cbbGenOutputDetailLevel.Text) + FIF.OutputDetailLevelTip);
                fifFile.WriteLine("IsLargeMemoryModel                       = " + FIF.Format(chkGenIsLargeMemoryModel.Checked) + FIF.IsLargeMemoryModelTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Landscape.
                fifFile.WriteLine("; LANDSCAPE PARAMETERS");
                fifFile.WriteLine("MaxRow                                   = " + FIF.Format(Convert.ToInt32(txbLandMaxRow.Text)) + FIF.MaxRowTip);
                fifFile.WriteLine("MaxCol                                   = " + FIF.Format(Convert.ToInt32(txbLandMaxCol.Text)) + FIF.MaxColTip);
                fifFile.WriteLine("CellSize                                 = " + FIF.Format(Convert.ToDouble(txbLandCellSize.Text)) + FIF.CellSizeTip);
                fifFile.WriteLine("CropNeighbors                            = " + FIF.Format(chkLandCropNeighbors.Checked) + FIF.CropNeighborsTip);
                fifFile.WriteLine("NumSpecies                               = " + FIF.Format(Convert.ToInt32(txbLandNumSpecies.Text)) + FIF.NumSpeciesTip);
                fifFile.WriteLine("NoVeg                                    = " + FIF.Format(Convert.ToInt32(txbLandNoVeg.Text)) + FIF.NoVegTip);
                fifFile.WriteLine("XLLCorner                                = " + FIF.Format(Convert.ToDouble(txbLandXllCorner.Text)) + FIF.XLLCornerTip);
                fifFile.WriteLine("YLLCorner                                = " + FIF.Format(Convert.ToDouble(txbLandYllCorner.Text)) + FIF.YLLCornerTip);
                fifFile.WriteLine("UseUniqueVegAndAgePerRep                 = " + FIF.Format(chkLandIsUsingUniqueVegAndAgePerRep.Checked) + FIF.IsUsingUniqueVegAndAgePerRepTip);
                fifFile.WriteLine("YearOfVegAndAgeFiles                     = " + FIF.Format(Convert.ToInt32(txbLandYearOfVegAndAgeFiles.Text)) + FIF.YearOfVegAndAgeFilesTip);
                fifFile.WriteLine("VegInputFile                             = " + FIF.Format(txbLandVegInputFile.Text) + FIF.VegInputFileTip);
                fifFile.WriteLine("VegTransitionFile                        = " + FIF.Format(txbLandVegTransitionFile.Text) + FIF.VegTransitionFileTip);
                fifFile.WriteLine("IsForcedVegTransitions                   = " + FIF.Format(chkLandIsForcedVegTransitions.Checked) + "TRUE: Succession trajectories are read from files as well as simulated.  FALSE: Succession is simulated only.");
                fifFile.WriteLine("AgeInputFile                             = " + FIF.Format(txbLandAgeInputFile.Text) + FIF.AgeInputFileTip);
                fifFile.WriteLine("SiteInputFile                            = " + FIF.Format(txbLandSiteInputFile.Text) + FIF.SiteInputFileTip);
                fifFile.WriteLine("TreeDensityInputFile                     = " + FIF.Format(txbLandTreeDensityInputFile.Text) + FIF.TreeDensityInputFileTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Map Output.
                fifFile.WriteLine("; MAP OUTPUT PARAMETERS");
                CommitCbbMapOutputSelect();
                fifFile.WriteLine("MapFiles                                 = " + FIF.Format(FIF.MapFiles) + FIF.MapFilesTip);
                fifFile.WriteLine("MapCodes                                 = " + FIF.Format(FIF.MapCodes) + FIF.MapCodesTip);
                fifFile.WriteLine("MapFlags                                 = " + FIF.FormatHex(FIF.MapFlags) + FIF.MapFlagsTip);
                fifFile.WriteLine("MapRepStart                              = " + FIF.Format(FIF.MapRepStart) + FIF.MapRepStartTip);
                fifFile.WriteLine("MapRepFreq                               = " + FIF.Format(FIF.MapRepFreq) + FIF.MapRepFreqTip);
                fifFile.WriteLine("MapYearStart                             = " + FIF.Format(FIF.MapYearStart) + FIF.MapYearStartTip);
                fifFile.WriteLine("MapYearFreq                              = " + FIF.Format(FIF.MapYearFreq) + FIF.MapYearFreqTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Stat Output.
                fifFile.WriteLine("; STAT OUTPUT FLAGS");
                fifFile.WriteLine(";0x000  Nothing is output.");
                fifFile.WriteLine(";0x001	Output the data averaged by year.");
                fifFile.WriteLine(";0x002	Output the data averaged by replicate.");
                fifFile.WriteLine(";0x004	Show row titles in output.  IE \"Year\", \"Mean\", \"Min\", \"Max\", etc.");
                fifFile.WriteLine(";0x010	Output the number of samples.");
                fifFile.WriteLine(";0x020	Output the mean of the data.");
                fifFile.WriteLine(";0x040	Output the standard deviation of the samples.");
                fifFile.WriteLine(";0x080	Output the minimum of the samples.");
                fifFile.WriteLine(";0x100	Output the maximum of the samples.");
                fifFile.WriteLine(";0x200	Output a histogram of samples.");
                fifFile.WriteLine(";0x400	Output the sample data for the statistic.  Note that sample data may already be an average or sum of event data.  For example, FireSize sample data is the sum of all fires in a given rep and year.");
                fifFile.WriteLine(";0x800	Output the event data for the statistic.  Event data is a record of individual events rather than an average or sum.");
                fifFile.WriteLine("; 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15");
                fifFile.WriteLine("; 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,  A,  B,  C,  D,  E,  F ");
                fifFile.WriteLine("; STAT OUTPUT PARAMETERS");
                fifFile.WriteLine("Stat.Summary.File                        = " + FIF.Format(txbStatSummaryFile.Text) + FIF.StatSummaryFileTip);
                CommitCbbStatSelect();
                fifFile.WriteLine("Stat.FireNum.Flags                       = " + FIF.FormatHex(FIF.StatFireNumFlags) + FIF.StatFireNumFlagsTip);
                fifFile.WriteLine("Stat.FireSize.Flags                      = " + FIF.FormatHex(FIF.StatFireSizeFlags) + FIF.StatFireSizeFlagsTip);
                fifFile.WriteLine("Stat.VegDist.Flags                       = " + FIF.FormatHex(FIF.StatVegDistFlags) + FIF.StatVegDistFlagsTip);
                fifFile.WriteLine("Stat.VegResidence.Flags                  = " + FIF.FormatHex(FIF.StatVegResidenceFlags) + FIF.StatVegResidenceFlagsTip);
                fifFile.WriteLine("Stat.FireSpecies.Flags                   = " + FIF.FormatHex(FIF.StatFireSpeciesFlags) + FIF.StatFireSpeciesFlagsTip);
                fifFile.WriteLine("Stat.FireInterval.Flags                  = " + FIF.FormatHex(FIF.StatFireIntervalFlags) + FIF.StatFireIntervalFlagsTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Climate.
                fifFile.WriteLine("; CLIMATE PARAMETERS");
                fifFile.WriteLine("Climate.NumHistory                       = " + FIF.Format(Convert.ToInt32(txbClimNumHistory.Text)) + FIF.ClimNumHistoryTip);
                if (FIF.ClimateTransitions.Count > 0)
                {
                    foreach (SClimateTransition ClimateTransition in FIF.ClimateTransitions.Values)
                    {
                        ClimateTransitionYears.Add(ClimateTransition.Year);
                        ClimateValuesTypes.Add(ClimateValuesTypeToString(ClimateTransition.ValuesType));
                        ClimateValuesConstantTemps.Add(ClimateTransition.ConstantTemp);
                        ClimateValuesConstantPrecips.Add(ClimateTransition.ConstantPrecip);
                        ClimateValuesFileTemps.Add(ClimateTransition.SpatialTempFile);
                        ClimateValuesFilePrecips.Add(ClimateTransition.SpatialPrecipFile);
                        ClimateValuesRandExplicitMinYears.Add(ClimateTransition.RandExplicitMinYear);
                        ClimateValuesRandExplicitMaxYears.Add(ClimateTransition.RandExplicitMaxYear);
                        ClimateValuesRandExpliciteRepicatedFlags.Add(ClimateTransition.IsRandExplicitReplicated);
                        ClimateOffsetsTypes.Add(ClimateOffsetsTypeToString(ClimateTransition.OffsetsType));
                        ClimateOffsetsFiles.Add(ClimateTransition.OffsetsFile);
                        ClimateOffsetsConstantTemps.Add(ClimateTransition.ConstantTempOffset);
                        ClimateOffsetsConstantPrecips.Add(ClimateTransition.ConstantPrecipOffset);
                        ClimateOffsetsRandomTempMeans.Add(ClimateTransition.RandomOffsetsTempMean);
                        ClimateOffsetsRandomTempStdDevs.Add(ClimateTransition.RandomOffsetsTempStdDev);
                        ClimateOffsetsRandomPrecipMeans.Add(ClimateTransition.RandomOffsetsPrecipMean);
                        ClimateOffsetsRandomPrecipStdDev.Add(ClimateTransition.RandomOffsetsPrecipStdDev);
                        ClimateOffsetsRandomRepicatedFlags.Add(ClimateTransition.IsRandOffsetReplicated);
                    }
                }
                fifFile.WriteLine("Climate.TransitionYears                  = " + FIF.Format(ClimateTransitionYears) + FIF.ClimateTransitionYearsTip);
                fifFile.WriteLine("Climate.Values.Type                      = " + FIF.Format(ClimateValuesTypes) + FIF.ClimValuesTypesTip);
                fifFile.WriteLine("Climate.Values.Temp.Constant             = " + FIF.Format(ClimateValuesConstantTemps) + FIF.ClimValuesConstantTempsTip);
                fifFile.WriteLine("Climate.Values.Precip.Constant           = " + FIF.Format(ClimateValuesConstantPrecips) + FIF.ClimValuesConstantPrecipsTip);
                fifFile.WriteLine("Climate.Values.Temp.File                 = " + FIF.Format(ClimateValuesFileTemps) + FIF.ClimValuesTempFilesTip);
                fifFile.WriteLine("Climate.Values.Precip.File               = " + FIF.Format(ClimateValuesFilePrecips) + FIF.ClimValuesPrecipFilesTip);
                fifFile.WriteLine("Climate.Values.RandExplicit.MinYear      = " + FIF.Format(ClimateValuesRandExplicitMinYears) + FIF.ClimValuesRandExplicitMinYearsTip);
                fifFile.WriteLine("Climate.Values.RandExplicit.MaxYear      = " + FIF.Format(ClimateValuesRandExplicitMaxYears) + FIF.ClimValuesRandExplicitMaxYearsTip);
                fifFile.WriteLine("Climate.Values.RandExplicit.Replicate    = " + FIF.Format(ClimateValuesRandExpliciteRepicatedFlags) + FIF.ClimValuesRandExplicitIsReplicatedTip);
                fifFile.WriteLine("Climate.Offsets.Type                     = " + FIF.Format(ClimateOffsetsTypes) + FIF.ClimOffsetsTypesTip);
                fifFile.WriteLine("Climate.Offsets.File                     = " + FIF.Format(ClimateOffsetsFiles) + FIF.ClimOffsetsFilesTip);
                fifFile.WriteLine("Climate.Offsets.Constant.Temp            = " + FIF.Format(ClimateOffsetsConstantTemps) + FIF.ClimOffsetsTempsTip);
                fifFile.WriteLine("Climate.Offsets.Constant.Precip          = " + FIF.Format(ClimateOffsetsConstantPrecips) + FIF.ClimOffsetsPrecipsTip);
                fifFile.WriteLine("Climate.Offsets.Random.Temp.Mean         = " + FIF.Format(ClimateOffsetsRandomTempMeans) + FIF.ClimOffsetsRandTempMeansTip);
                fifFile.WriteLine("Climate.Offsets.Random.Temp.StdDev       = " + FIF.Format(ClimateOffsetsRandomTempStdDevs) + FIF.ClimOffsetsRandTempStdDevsTip);
                fifFile.WriteLine("Climate.Offsets.Random.Precip.Mean       = " + FIF.Format(ClimateOffsetsRandomPrecipMeans) + FIF.ClimOffsetsRandPrecipMeansTip);
                fifFile.WriteLine("Climate.Offsets.Random.Precip.StdDev     = " + FIF.Format(ClimateOffsetsRandomPrecipStdDev) + FIF.ClimOffsetsRandPrecipStdDevsTip);
                fifFile.WriteLine("Climate.Offsets.Random.Replicate         = " + FIF.Format(ClimateOffsetsRandomRepicatedFlags) + FIF.ClimOffsetsRandIsReplicatedTip);
                fifFile.WriteLine("Climate.StepsAndRampsEnabled             = " + FIF.Format(chkStepsAndRampsEnabled.Checked) + FIF.ClimStepsAndRampsEnabledTip);
                ParseListBox(lstClimTempSteps, Years, Values);
                fifFile.WriteLine("Climate.Offsets.TempStepYear             = " + FIF.Format(Years) + FIF.ClimOffsetsTempStepYearTip);
                fifFile.WriteLine("Climate.Offsets.TempStep                 = " + FIF.Format(Values) + FIF.ClimOffsetsTempStepTip);
                ParseListBox(lstClimTempRamps, Years, Values);
                fifFile.WriteLine("Climate.Offsets.TempRampYear             = " + FIF.Format(Years) + FIF.ClimOffsetsTempRampYearTip);
                fifFile.WriteLine("Climate.Offsets.TempRamp                 = " + FIF.Format(Values) + FIF.ClimOffsetsTempRampTip);
                ParseListBox(lstClimPrecipSteps, Years, Values);
                fifFile.WriteLine("Climate.Offsets.PrecipStepYear           = " + FIF.Format(Years) + FIF.ClimOffsetsPrecipStepYearTip);
                fifFile.WriteLine("Climate.Offsets.PrecipStep               = " + FIF.Format(Values) + FIF.ClimOffsetsPrecipStepTip);
                ParseListBox(lstClimPrecipRamps, Years, Values);
                fifFile.WriteLine("Climate.Offsets.PrecipRampYear           = " + FIF.Format(Years) + FIF.ClimOffsetsPrecipRampYearTip);
                fifFile.WriteLine("Climate.Offsets.PrecipRamp               = " + FIF.Format(Values) + FIF.ClimOffsetsPrecipRampTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Fire.
                fifFile.WriteLine("; FIRE PARAMETERS");
                if (FIF.FireTransitions.Count > 0)
                {
                    foreach (SFireTransition FireTransition in FIF.FireTransitions.Values)
                    {
                        FireTypeTransitionYears.Add(FireTransition.Year);
                        FireTypeTransitions.Add(FireTransition.Type.ToString());
                        FireIgnitionFactors.Add(FireTransition.Ignition);
                        FireSensitivities.Add(FireTransition.Sensitivity);
                        FireSpatialIgnitionFactors.Add(FireTransition.SpatialIgnitionFile);
                        FireSpatialSensitivities.Add(FireTransition.SpatialSensitivityFile);
                    }
                }
                fifFile.WriteLine("Fire.Types                               = " + FIF.Format(FireTypeTransitions) + FIF.FireTypeTransitionTip);
                fifFile.WriteLine("Fire.TypeTransitionYears                 = " + FIF.Format(FireTypeTransitionYears) + FIF.FireTypeTransitionYearTip);
                fifFile.WriteLine("Fire.Enabled                             = " + FIF.Format(chkFireEnabled.Checked) + FIF.FireEnabledTip);
                fifFile.WriteLine("Fire.SpreadRadius                        = " + FIF.Format(Convert.ToDouble(txbFireSpreadRadius.Text)) + FIF.FireSpreadRadiusTip);
                Values.Clear(); Values.Add(Convert.ToDouble(txbFireSpreadMean.Text)); Values.Add(Convert.ToDouble(txbFireSpreadStdDev.Text));
                fifFile.WriteLine("Fire.SpreadParms                         = " + FIF.Format(Values) + FIF.FireSpreadParmsTip);
                Values.Clear(); Values.Add(Convert.ToDouble(txbFireClimateA.Text)); Values.Add(Convert.ToDouble(txbFireClimateB.Text)); Values.Add(Convert.ToDouble(txbFireClimateC.Text));
                fifFile.WriteLine("Fire.Climate                             = " + FIF.Format(Values) + FIF.FireClimateTip);
                fifFile.WriteLine("Fire.IgnoreFirstInterval                 = " + FIF.Format(chkFireIgnoreFirstInterval.Checked) + FIF.FireIgnoreFirstIntervalTip);
                fifFile.WriteLine("Fire.IgnitionFactor                      = " + FIF.Format(FireIgnitionFactors) + FIF.FireIgnitionTip);
                fifFile.WriteLine("Fire.Sensitivity                         = " + FIF.Format(FireSensitivities) + FIF.FireSensitivityTip);
                fifFile.WriteLine("Fire.MaxEmpiricalFireSizeEvent           = " + FIF.Format(Convert.ToInt32(txbFireMaxEmpiricalFireSizeEvent.Text)) + FIF.FireMaxEmpiricalFireSizeEventTip);
                fifFile.WriteLine("Fire.MaxEmpiricalFireSizeEventWeight     = " + FIF.Format(Convert.ToDouble(txbFireMaxEmpiricalFireSizeEventWeight.Text)) + FIF.FireMaxEmpiricalFireSizeEventWeightTip);
                fifFile.WriteLine("Fire.Historical                          = " + FIF.Format(txbFireHistorical.Text) + FIF.FireHistoricalTip);
                fifFile.WriteLine("Fire.HumanIgnition.Basename              = " + FIF.Format(txbFireHumanIgnitionFile.Text) + FIF.FireHumanIgnitionFileTip);
                //			fifFile.WriteLine("Fire.HumanIgnition.Prob		            = " + txbFireHumanIgnitionProb.Text + "\t\t; " + FIF.FireHumanIgnitionProbTip);
                fifFile.WriteLine("Fire.Suppression.On                      = " + FIF.Format(chkFireSuppressionOn.Checked) + FIF.FireSuppressionOnTip);
                fifFile.WriteLine("Fire.Suppression.Basename                = " + FIF.Format(txbFireSuppressionFile.Text) + FIF.FireSuppressionFileTip);
                fifFile.WriteLine("Fire.Suppression.Class1                  = " + FIF.Format(Convert.ToDouble(txbFireSuppressionClass1.Text)) + FIF.FireSuppressionClass1Tip);
                fifFile.WriteLine("Fire.Suppression.Class2                  = " + FIF.Format(Convert.ToDouble(txbFireSuppressionClass2.Text)) + FIF.FireSuppressionClass2Tip);
                fifFile.WriteLine("Fire.Suppression.Class3                  = " + FIF.Format(Convert.ToDouble(txbFireSuppressionClass3.Text)) + FIF.FireSuppressionClass3Tip);
                fifFile.WriteLine("Fire.Suppression.Class4                  = " + FIF.Format(Convert.ToDouble(txbFireSuppressionClass4.Text)) + FIF.FireSuppressionClass4Tip);
                fifFile.WriteLine("Fire.Suppression.Class5                  = " + FIF.Format(Convert.ToDouble(txbFireSuppressionClass5.Text)) + FIF.FireSuppressionClass5Tip);
                fifFile.WriteLine("Fire.Suppression.Threshold.FireSize      = " + FIF.Format(Convert.ToInt32(txbFireSuppressionThresholdFireSize.Text)) + FIF.FireSuppressionThresholdFireSizeTip);
                fifFile.WriteLine("Fire.Suppression.Threshold.Ignitions     = " + FIF.Format(Convert.ToInt32(txbFireSuppressionThresholdIgnitions.Text)) + FIF.FireSuppressionThresholdIgnitionsTip);
                fifFile.WriteLine("Fire.Spatial.IgnitionFactor              = " + FIF.Format(FireSpatialIgnitionFactors) + FIF.FireSpatialIgnitionTip);
                fifFile.WriteLine("Fire.Spatial.Sensitivity                 = " + FIF.Format(FireSpatialSensitivities) + FIF.FireSpatialSensitivityTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Tundra.
                fifFile.WriteLine("; PARAMETERS FOR THE TUNDRA FRAME");
                fifFile.WriteLine("Tundra                                   = " + FIF.Format(FIF.Tundra) + FIF.TundraTip);
                fifFile.WriteLine("Tundra.StartAge                          = " + FIF.Format(ParseList(txbTundraStartAge.Text)) + FIF.TundraStartAgeTip);
                fifFile.WriteLine("Tundra.FireProb                          = " + FIF.Format(Convert.ToDouble(txbTundraFireProb.Text)) + FIF.TundraFireProbTip);
                fifFile.WriteLine("Tundra.HumanFireProb                     = " + FIF.Format(Convert.ToDouble(txbTundraHumanFireProb.Text)) + FIF.TundraHumanFireProbTip);
                fifFile.WriteLine("Tundra.Spruce.EstBA                      = " + FIF.Format(Convert.ToDouble(txbTundraSpruceEstBA.Text)) + FIF.TundraSpruceEstBATip);
                fifFile.WriteLine("Tundra.SeedRange                         = " + FIF.Format(Convert.ToDouble(txbTundraSeedRange.Text)) + FIF.TundraSeedRangeTip);
                fifFile.WriteLine("Tundra.SeedSource                        = " + FIF.Format(ParseList(txbTundraSeedSource.Text)) + FIF.TundraSeedSourceTip);
                fifFile.WriteLine("Tundra.Seed.BasalArea                    = " + FIF.Format(Convert.ToDouble(txbTundraSeedBasalArea.Text)) + FIF.TundraSeedBasalAreaTip);
                fifFile.WriteLine("Tundra.Seedling                          = " + FIF.Format(Convert.ToDouble(txbTundraSeedling.Text)) + FIF.TundraSeedlingTip);
                fifFile.WriteLine("Tundra.SeedlingBA                        = " + FIF.Format(Convert.ToDouble(txbTundraSeedlingBA.Text)) + FIF.TundraSeedlingBATip);
                fifFile.WriteLine("Tundra.History                           = " + FIF.Format(Convert.ToInt32(txbTundraHistory.Text)) + FIF.TundraHistoryTip);
                fifFile.WriteLine("Tundra.SeedEstParms                      = " + FIF.Format(ParseList(txbTundraSeedEstParms.Text)) + FIF.TundraSeedEstParmsTip);
                fifFile.WriteLine("Tundra.MeanGrowth                        = " + FIF.Format(Convert.ToDouble(txbTundraMeanGrowth.Text)) + FIF.TundraMeanGrowthTip);
                fifFile.WriteLine("Tundra.ClimGrowth                        = " + FIF.Format(ParseList(txbTundraClimGrowth.Text)) + FIF.TundraClimGrowthTip);
                fifFile.WriteLine("Tundra.CalFactor                         = " + FIF.Format(ParseList(txbTundraCalFactor.Text)) + FIF.TundraCalFactorTip);
                fifFile.WriteLine("Tundra->Spruce.BasalArea                 = " + FIF.Format(Convert.ToDouble(txbTundraSpruceBasalArea.Text)) + FIF.TundraSpruceBasalAreaTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Black Spruce.
                fifFile.WriteLine("; PARAMETERS FOR THE BLACK SPRUCE FRAME");
                fifFile.WriteLine("BSpruce                                  = " + FIF.Format(FIF.BSpruce) + FIF.BSpruceTip);
                fifFile.WriteLine("BSpruce.StartAge                         = " + FIF.Format(ParseList(txbBSpruceStartAge.Text)) + FIF.BSpruceStartAgeTip);
                fifFile.WriteLine("BSpruce.FireParms                        = " + FIF.Format(ParseList(txbBSpruceFireParms.Text)) + FIF.BSpruceFireParmsTip);
                fifFile.WriteLine("BSpruce.HumanFireProb                    = " + FIF.Format(Convert.ToDouble(txbBSpruceHumanFireProb.Text)) + FIF.BSpruceHumanFireProbTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //White Spruce.
                fifFile.WriteLine("; PARAMETERS FOR THE WHITE SPRUCE FRAME");
                fifFile.WriteLine("WSpruce                                  = " + FIF.Format(FIF.WSpruce) + FIF.WSpruceTip);
                fifFile.WriteLine("WSpruce.StartAge                         = " + FIF.Format(ParseList(txbWSpruceStartAge.Text)) + FIF.WSpruceStartAgeTip);
                fifFile.WriteLine("WSpruce.FireParms                        = " + FIF.Format(ParseList(txbWSpruceFireParms.Text)) + FIF.WSpruceFireParmsTip);
                fifFile.WriteLine("WSpruce.HumanFireProb                    = " + FIF.Format(Convert.ToDouble(txbWSpruceHumanFireProb.Text)) + FIF.WSpruceHumanFireProbTip);
                fifFile.WriteLine("");
                fifFile.WriteLine("");
                //Deciduous.
                fifFile.WriteLine("; PARAMETERS FOR THE DECIDUOUS FRAME");
                fifFile.WriteLine("Decid                                    = " + FIF.Format(FIF.Decid) + FIF.DecidTip);
                fifFile.WriteLine("Decid.StartAge.WSpruce                   = " + FIF.Format(ParseList(txbDecidStartAgeWSpruce.Text)) + FIF.DecidStartAgeWSpruceTip);
                fifFile.WriteLine("Decid.StartAge.BSpruce                   = " + FIF.Format(ParseList(txbDecidStartAgeBSpruce.Text)) + FIF.DecidStartAgeBSpruceTip);
                fifFile.WriteLine("Decid.FireProb                           = " + FIF.Format(Convert.ToDouble(txbDecidFireProb.Text)) + FIF.DecidFireProbTip);
                fifFile.WriteLine("Decid.HumanFireProb                      = " + FIF.Format(Convert.ToDouble(txbDecidHumanFireProb.Text)) + FIF.DecidHumanFireProbTip);
                fifFile.WriteLine("Decid.History                            = " + FIF.Format(Convert.ToInt32(txbDecidHistory.Text)) + FIF.DecidHistoryTip);
                fifFile.WriteLine("Decid->Tundra.Parms                      = " + FIF.Format(ParseList(txbDecidTundraParms.Text)) + FIF.DecidTundraParmsTip);
                fifFile.WriteLine("Decid->BSpruce.Parms                     = " + FIF.Format(ParseList(txbDecidBSpruceParms.Text)) + FIF.DecidBSpruceParmsTip);
                fifFile.WriteLine("Decid->WSpruce.Parms                     = " + FIF.Format(ParseList(txbDecidWSpruceParms.Text)) + FIF.DecidWSpruceParmsTip);

                fifFile.Flush();
                fifFile.Close();
            }
            catch (Exception e) {
                Global.Main.ViewerProgress.AddError("Save FIF failed.  File: " + Global.FIF.FileName + ".\n"+e.Message+"\n");
            }
        }

		private void				RefreshCbbMapOutputSelect()
		{
			int i=-1;
			int flags=0;
			bool isValid=false;
			//Display current index from Global.FIF variables.
			i=cbbMapOutputSelect.SelectedIndex;
			storedMapIndex=i;
			if (i != -1) {
				isValid = true;
				txbMapCodes.Text=Global.FIF.MapCodes[i].ToString();
				txbMapRepStart.Text=Global.FIF.MapRepStart[i].ToString();
				txbMapRepFreq.Text=Global.FIF.MapRepFreq[i].ToString();
				txbMapYearStart.Text=Global.FIF.MapYearStart[i].ToString();
				txbMapYearFreq.Text=Global.FIF.MapYearFreq[i].ToString();		
				//Display map flags.
				flags = Convert.ToInt32(Global.FIF.MapFlags[i].ToString());
				chkMapIsOutput.Checked = ((flags & outMapData) == outMapData);			
//				chkMapYearStats.Checked = ((flags & outYearStats) == outYearStats);			
//				chkMapRepStats.Checked = ((flags & outRepStats) == outRepStats);			
				chkMapFormat.Checked = ((flags & outMapFormat) == outMapFormat);			
				chkMapHeader.Checked = ((flags & outHeader) == outHeader);			
				chkMapARC.Checked = ((flags & outARC) == outARC);			
				chkMapVeg.Checked = ((flags & outVeg) == outVeg);			
				chkMapAge.Checked = ((flags & outAge) == outAge);			
				chkMapSite.Checked = ((flags & outSite) == outSite);			
				chkMapSub.Checked = ((flags & outSub) == outSub);			
				chkMapFireAge.Checked = ((flags & outFireAge) == outFireAge);			
				chkMapTemp.Checked = ((flags & outTemp) == outTemp);			
				chkMapPrecip.Checked = ((flags & outPrecip) == outPrecip);
				chkMapFireScar.Checked = ((flags & outFireScar) == outFireScar);			
//				chkMapFireScarSum.Checked = ((flags & outFireScarSum) == outFireScarSum);			
				chkMapFrame1.Checked = ((flags & out1) == out1);			
				chkMapFrame2.Checked = ((flags & out2) == out2);			
				chkMapFrame3.Checked = ((flags & out3) == out3);			
				chkMapFrame4.Checked = ((flags & out4) == out4);
				lblMapFlags.Text = "FIF Flags: " + Global.FIF.MapFlags[i].ToString();			ToolTips.SetToolTip(lblMapFlags, Global.FIF.MapFlagsTip);
			}
			//Enable/Disable controls cbbMapOutputSelect is valid.
			txbMapCodes.Enabled = isValid;
			txbMapRepStart.Enabled = isValid;
			txbMapRepFreq.Enabled = isValid;
			txbMapYearStart.Enabled = isValid;
			txbMapYearFreq.Enabled = isValid;
			chkMapIsOutput.Enabled = isValid;
			chkMapFormat.Enabled = isValid;
			chkMapHeader.Enabled = isValid;
			chkMapARC.Enabled = isValid;
			chkMapVeg.Enabled = isValid;
			chkMapAge.Enabled = isValid;
			chkMapSite.Enabled = isValid;
			chkMapSub.Enabled = isValid;
			chkMapFireAge.Enabled = isValid;
			chkMapTemp.Enabled = isValid;
			chkMapPrecip.Enabled = isValid;
			chkMapFireScar.Enabled = isValid;
//			chkMapFireScarSum.Enabled = isValid;
			chkMapFrame1.Enabled = isValid;
			chkMapFrame2.Enabled = isValid;
			chkMapFrame3.Enabled = isValid;
			chkMapFrame4.Enabled = isValid;
			lblMapFlags.Enabled = isValid;
			if (!isValid) {
				//txbMapFiles.Text = "";
				txbMapCodes.Text = "";
				txbMapRepStart.Text = "";
				txbMapRepFreq.Text = "";
				txbMapYearStart.Text = "";
				txbMapYearFreq.Text = "";
				lblMapFlags.Text = "FIF Flags: ";
				chkMapIsOutput.Checked = false;
				chkMapFormat.Checked = false;
				chkMapHeader.Checked = false;
				chkMapARC.Checked = false;
				chkMapVeg.Checked = false;
				chkMapAge.Checked = false;
				chkMapSite.Checked = false;
				chkMapSub.Checked = false;
				chkMapFireAge.Checked = false;
				chkMapTemp.Checked = false;
				chkMapPrecip.Checked = false;
				chkMapFireScar.Checked = false;
//				chkMapFireScarSum.Checked = false;
				chkMapFrame1.Checked = false;
				chkMapFrame2.Checked = false;
				chkMapFrame3.Checked = false;
				chkMapFrame4.Checked = false;
			}
		}
		private void				CommitCbbMapOutputSelect()
		{
			int i=-1;
			int flags=0;
			//Commit map field edits to the Global.FIF variables.
			i=storedMapIndex;
			if (i>=0 && i<cbbMapOutputSelect.Items.Count) {
				//Sum all map flags.
				flags += chkMapIsOutput.Checked ? outMapData : 0;
//				flags += chkMapYearStats.Checked ? outYearStats : 0;
//				flags += chkMapRepStats.Checked ? outRepStats : 0;
				flags += chkMapFormat.Checked ? outMapFormat : 0;
				flags += chkMapHeader.Checked ? outHeader : 0;
				flags += chkMapARC.Checked ? outARC : 0;
				flags += chkMapVeg.Checked ? outVeg : 0;
				flags += chkMapAge.Checked ? outAge : 0;
				flags += chkMapSite.Checked ? outSite : 0;
				flags += chkMapSub.Checked ? outSub : 0;
				flags += chkMapFireAge.Checked ? outFireAge : 0;
				flags += chkMapTemp.Checked ? outTemp : 0;
				flags += chkMapPrecip.Checked ? outPrecip : 0;
				flags += chkMapFireScar.Checked ? outFireScar : 0;
//				flags += chkMapFireScarSum.Checked ? outFireScarSum : 0;
				flags += chkMapFrame1.Checked ? out1 : 0;
				flags += chkMapFrame2.Checked ? out2 : 0;
				flags += chkMapFrame3.Checked ? out3 : 0;
				flags += chkMapFrame4.Checked ? out4 : 0;
				//Commit edits to Global.FIF.
				Global.FIF.MapCodes[i]		= txbMapCodes.Text;
				Global.FIF.MapFlags[i]		= flags;
				Global.FIF.MapRepStart[i]	= Convert.ToInt32(txbMapRepStart.Text);
				Global.FIF.MapRepFreq[i]	= Convert.ToInt32(txbMapRepFreq.Text);
				Global.FIF.MapYearStart[i]	= Convert.ToInt32(txbMapYearStart.Text);
				Global.FIF.MapYearFreq[i]	= Convert.ToInt32(txbMapYearFreq.Text);	
				//Update cbbMapOutputSelect (Mapfiles has already been updated in cbbMapOutputSelect_TextChanged).
				if (cbbMapOutputSelect.Focused) {
					int temp = cbbMapOutputSelect.SelectedIndex;
					cbbMapOutputSelect.Items.RemoveAt(i);
					cbbMapOutputSelect.Items.Insert(i, Global.FIF.MapFiles[i]);
					cbbMapOutputSelect.SelectedIndex = temp;
				}
			}
		}
		private void				RefreshCbbStatSelect()
		{
			//Show stat flags for the new selection.
			string stat = cbbStatSelect.SelectedItem != null ? cbbStatSelect.SelectedItem.ToString() : cbbStatSelect.SelectedText;
			int flags = 0;
			bool isValid = false;
			switch(stat)
			{
				case "Fire Number" :		flags=Global.FIF.StatFireNumFlags;			isValid=true;	break;
				case "Fire Size" :			flags=Global.FIF.StatFireSizeFlags;			isValid=true;	break;
				case "Veg Distribution" :	flags=Global.FIF.StatVegDistFlags;			isValid=true;	break;
				case "Veg Residence Time" :	flags=Global.FIF.StatVegResidenceFlags;		isValid=true;	break;
				case "Fire by Species" :	flags=Global.FIF.StatFireSpeciesFlags;		isValid=true;	break;
				case "Fire Interval" :		flags=Global.FIF.StatFireIntervalFlags;		isValid=true;	break;
			}
			chkStatYear.Checked = ((flags & outYear) == outYear);			
			chkStatRep.Checked = ((flags & outRep) == outRep);
			chkStatFormat.Checked = ((flags & outFormat) == outFormat);
			chkStatNum.Checked = ((flags & outNum) == outNum);
			chkStatMean.Checked = ((flags & outMean) == outMean);
			chkStatStdDev.Checked = ((flags & outStdDev) == outStdDev);
			chkStatMin.Checked = ((flags & outMin) == outMin);
			chkStatMax.Checked = ((flags & outMax) == outMax);
			chkStatHist.Checked = ((flags & outHist) == outHist);
			chkStatData.Checked = ((flags & outData) == outData);
			chkStatEvents.Checked = ((flags & outEvents) == outEvents);	
			chkStatYear.Enabled = isValid;		
			chkStatRep.Enabled = isValid;		
			chkStatFormat.Enabled = isValid;		
			chkStatNum.Enabled = isValid;		
			chkStatMean.Enabled = isValid;		
			chkStatStdDev.Enabled = isValid;		
			chkStatMin.Enabled = isValid;		
			chkStatMax.Enabled = isValid;		
			chkStatHist.Enabled = isValid;		
			chkStatData.Enabled = isValid;		
			chkStatEvents.Enabled = isValid;	
		}
		private void				CommitCbbStatSelect()
		{
			//Commit any flag edits to the Global.FIF variable.
			string stat = cbbStatSelect.SelectedItem != null ? cbbStatSelect.SelectedItem.ToString() : cbbStatSelect.SelectedText;
			int flags = 0;
			flags += chkStatYear.Checked ? outYear : 0;
			flags += chkStatRep.Checked ? outRep : 0;
			flags += chkStatFormat.Checked ? outFormat : 0;
			flags += chkStatNum.Checked ? outNum : 0;
			flags += chkStatMean.Checked ? outMean : 0;
			flags += chkStatStdDev.Checked ? outStdDev : 0;
			flags += chkStatMin.Checked ? outMin : 0;
			flags += chkStatMax.Checked ? outMax : 0;
			flags += chkStatHist.Checked ? outHist : 0;
			flags += chkStatData.Checked ? outData : 0;
			flags += chkStatEvents.Checked ? outEvents : 0;
			switch(stat)
			{
				case "Fire Number" :		Global.FIF.StatFireNumFlags = flags;			break;
				case "Fire Size" :			Global.FIF.StatFireSizeFlags = flags;			break;
				case "Veg Distribution" :	Global.FIF.StatVegDistFlags = flags;			break;
				case "Veg Residence Time" :	Global.FIF.StatVegResidenceFlags = flags;		break;
				case "Fire by Species" :	Global.FIF.StatFireSpeciesFlags = flags;		break;
				case "Fire Interval" :		Global.FIF.StatFireIntervalFlags = flags;		break;
			}		
		}

		private void				ParseListBox(ListBox List, List<int> Return1, List<double> Return2)
		{
			string[] Temp;
			Return1.Clear();
			Return2.Clear();
			if (List.Items.Count>0) {
				for (int i=0; i<List.Items.Count; i++) {
					Temp	= List.Items[i].ToString().Split(new char[] {':'});
					Return1.Add(Convert.ToInt32(Temp[0]));
					Return2.Add(Convert.ToDouble(Temp[1]));
				}
			}
		}
		private List<double>		ParseList(String List)
		{
			List<double>	Values	= new List<double>();
			String[]		Temp	= List.Split(new char[] {',',' ',});
			Values.Add(Convert.ToDouble(Temp[0]));
			if (Temp.Length>2)	Values.Add(Convert.ToDouble(Temp[2]));
			if (Temp.Length>4)	Values.Add(Convert.ToDouble(Temp[4]));
			if (Temp.Length>6)	Values.Add(Convert.ToDouble(Temp[6]));
			if (Temp.Length>8)	Values.Add(Convert.ToDouble(Temp[8]));
			return Values;
		}

		#region Events
		public new void				ProgramStateChanged(object sender, ProgramStateEventArgs e)
		{
            bool enable = (e.ProgramState!=EnumProgramState.SIMULATING);
            SafeInvokeHelper.Invoke(this, "SetEnabled", enable);
		}
        private void SetEnabled(bool isEnabled)
        {
            Enabled = isEnabled;
        }
		//File Lookup Buttons.
		private void				btnBaseDirLookup_Click(object sender, System.EventArgs e)
		{
			FolderBrowserDialog FolderBrowserDialogBox = new FolderBrowserDialog();
			if(FolderBrowserDialogBox.ShowDialog() == DialogResult.OK)	
				txbGenBaseDirectory.Text = FolderBrowserDialogBox.SelectedPath;		
		}
		private void				btnOutputDirLookup_Click(object sender, EventArgs e)
		{
			FolderBrowserDialog FolderBrowserDialogBox = new FolderBrowserDialog();
			if(FolderBrowserDialogBox.ShowDialog() == DialogResult.OK)
                txbGenOutputDirectory.Text = FolderBrowserDialogBox.SelectedPath.Replace(txbGenBaseDirectory.Text, "");		
		}
        private void                btnLandVegInputFileLookup_Click(object sender, System.EventArgs e)
        {
            OpenFileDialog OpenFileDialogBox = new OpenFileDialog();
            OpenFileDialogBox.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
            OpenFileDialogBox.InitialDirectory = txbGenBaseDirectory.Text;
            if (OpenFileDialogBox.ShowDialog() == DialogResult.OK)
                txbLandVegInputFile.Text = OpenFileDialogBox.FileName.Replace(txbGenBaseDirectory.Text, "");
        }
        private void                btnLandVegTransitionInputFileLookup_Click(object sender, System.EventArgs e)
        {
            OpenFileDialog OpenFileDialogBox = new OpenFileDialog();
            OpenFileDialogBox.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
            OpenFileDialogBox.InitialDirectory = txbGenBaseDirectory.Text;
            if (OpenFileDialogBox.ShowDialog() == DialogResult.OK)
                this.txbLandVegTransitionFile.Text = OpenFileDialogBox.FileName.Replace(txbGenBaseDirectory.Text, "");
        }
        private void                btnLandAgeInputFileLookup_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog OpenFileDialogBox = new OpenFileDialog();
			OpenFileDialogBox.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*" ;
			OpenFileDialogBox.InitialDirectory = txbGenBaseDirectory.Text;
			if(OpenFileDialogBox.ShowDialog() == DialogResult.OK)
                txbLandAgeInputFile.Text = OpenFileDialogBox.FileName.Replace(txbGenBaseDirectory.Text, "");				
		}
		private void				btnLandSiteInputFileLookup_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog OpenFileDialogBox = new OpenFileDialog();
			OpenFileDialogBox.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*" ;
			OpenFileDialogBox.InitialDirectory = txbGenBaseDirectory.Text;
			if(OpenFileDialogBox.ShowDialog() == DialogResult.OK)
                txbLandSiteInputFile.Text = OpenFileDialogBox.FileName.Replace(txbGenBaseDirectory.Text, "");	
		}
		private void				btnLandTreeDensityInputFileLookup_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog OpenFileDialogBox = new OpenFileDialog();
			OpenFileDialogBox.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*" ;
			OpenFileDialogBox.InitialDirectory = txbGenBaseDirectory.Text;
            if (OpenFileDialogBox.ShowDialog() == DialogResult.OK)
                txbLandTreeDensityInputFile.Text = OpenFileDialogBox.FileName.Replace(txbGenBaseDirectory.Text, "");
		}

        //Landscape
        private void                chkLandIsUsingUniqueVegAndAgePerRep_CheckedChanged(object sender, EventArgs e)
        {
            LayoutLandscape();
        }

		//Climate
        private void                lstClimTransitions_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            if (lstClimTransitions.SelectedIndex != -1)
            {
                if (ClimateTransitionToSave != -1)
                    ClimateTransitionSave(ClimateTransitionToSave);
                else
                    ClimateTransitionRefresh(Global.FIF.ClimateTransitions.Values[lstClimTransitions.SelectedIndex]);
            }
            else
            {
                ClearClimateTransitionControls();
            }
        }
        private void                btnClimTransitionAdd_Click(object sender, System.EventArgs e)
        {
            SClimateTransition ClimTransition;
            ClimTransition = new SClimateTransition();
            ClimTransition.Year = (Global.FIF.ClimateTransitions.Count > 0 ? Global.FIF.ClimateTransitions.Values[Global.FIF.ClimateTransitions.Count - 1].Year + 1 : 0);
            ClimTransition.ValuesType = EClimateValuesType.VTCONSTANT;
            ClimTransition.OffsetsType = EClimateOffsetsType.OTNONE;
            Global.FIF.ClimateTransitions.Add(ClimTransition.Year, ClimTransition);
            ClimateTransitionRefresh(ClimTransition);
            ClimateTransitionsRefresh();
        }
        private void                btnClimTransitionRemove_Click(object sender, System.EventArgs e)
        {
            if (lstClimTransitions.SelectedIndex != -1) ClimateTransitionRemove(lstClimTransitions.SelectedIndex);
        }
        private void                ClearClimateTransitionControls()
        {
            txbClimTransitionYear.Text = "";
            cbbClimValuesType.Text = "";
            txbClimValuesTemp.Text = "";
            txbClimValuesPrecip.Text = "";
            txbClimRandExplicitMaxYear.Text = "";
            txbClimRandExplicitMinYear.Text = "";
            chkClimValuesRandExplicitIsReplicated.Checked = false;
            cbbClimOffsetsType.Text = "";
            txbClimOffsetsFile.Text = "";
            txbClimOffsetsTempConstant.Text = "";
            txbClimOffsetsPrecipConstant.Text = "";
            txbClimOffsetsTempMean.Text = "";
            txbClimOffsetsTempStdDev.Text = "";
            txbClimOffsetsPrecipMean.Text = "";
            txbClimOffsetsPrecipStdDev.Text = "";
            chkClimOffsetsRandIsReplicated.Checked = false;
            //Disable controls.
            txbClimTransitionYear.Enabled = false;
            cbbClimValuesType.Enabled = false;
            pnlClimValues.Enabled = false;
            pnlClimRand.Enabled = false;
            cbbClimOffsetsType.Enabled = false;
            pnlOffsetsFile.Enabled = false;
            pnlOffsetsConstant.Enabled = false;
            pnlOffsetsRandom.Enabled = false;
        }
        private void                ClimateTransitionRefresh(SClimateTransition ClimateTransition)
        {
            FormState = EnumFormState.REFRESH;

            ClearClimateTransitionControls();
            
            //Enable controls and set values to current transition.
            txbClimTransitionYear.Enabled = true;
            cbbClimValuesType.Enabled = true;
            txbClimTransitionYear.Text = ClimateTransition.Year.ToString();
            cbbClimValuesType.Text = ClimateValuesTypeToString(ClimateTransition.ValuesType);
            switch (ClimateTransition.ValuesType)
            {
                case EClimateValuesType.VTCONSTANT:
                    pnlClimValues.Enabled = true;
                    txbClimValuesTemp.Text = ClimateTransition.ConstantTemp.ToString();
                    txbClimValuesPrecip.Text = ClimateTransition.ConstantPrecip.ToString();
                    break;
                case EClimateValuesType.VTSPATIAL:
                    pnlClimValues.Enabled = true;
                    txbClimValuesTemp.Text = ClimateTransition.SpatialTempFile;
                    txbClimValuesPrecip.Text = ClimateTransition.SpatialPrecipFile;
                    break;
                case EClimateValuesType.VTEXPLICIT:
                    pnlClimValues.Enabled = true;
                    txbClimValuesTemp.Text = ClimateTransition.SpatialTempFile;
                    txbClimValuesPrecip.Text = ClimateTransition.SpatialPrecipFile;
                    break;
                case EClimateValuesType.VTRANDEXPLICIT:
                    pnlClimValues.Enabled = true;
                    pnlClimRand.Enabled = true;
                    txbClimValuesTemp.Text = ClimateTransition.SpatialTempFile;
                    txbClimValuesPrecip.Text = ClimateTransition.SpatialPrecipFile;
                    txbClimRandExplicitMinYear.Text = ClimateTransition.RandExplicitMinYear.ToString();
                    txbClimRandExplicitMaxYear.Text = ClimateTransition.RandExplicitMaxYear.ToString();
                    chkClimValuesRandExplicitIsReplicated.Checked = ClimateTransition.IsRandExplicitReplicated;
                    break;
            }
            cbbClimOffsetsType.Enabled = true;
            cbbClimOffsetsType.Text = ClimateOffsetsTypeToString(ClimateTransition.OffsetsType);
            switch (ClimateTransition.OffsetsType)
            {
                case EClimateOffsetsType.OTNONE:
                    break;
                case EClimateOffsetsType.OTCONSTANT:
                    pnlOffsetsConstant.Enabled = true;
                    txbClimOffsetsTempConstant.Text = ClimateTransition.ConstantTempOffset.ToString();
                    txbClimOffsetsPrecipConstant.Text = ClimateTransition.ConstantPrecipOffset.ToString();
                    break;
                case EClimateOffsetsType.OTFILE:
                    pnlOffsetsFile.Enabled = true;
                    txbClimOffsetsFile.Text = ClimateTransition.OffsetsFile;
                    break;
                case EClimateOffsetsType.OTRANDOM:
                    pnlOffsetsRandom.Enabled = true;
                    txbClimOffsetsTempMean.Text = ClimateTransition.RandomOffsetsTempMean.ToString();
                    txbClimOffsetsTempStdDev.Text = ClimateTransition.RandomOffsetsTempStdDev.ToString();
                    txbClimOffsetsPrecipMean.Text = ClimateTransition.RandomOffsetsPrecipMean.ToString();
                    txbClimOffsetsPrecipStdDev.Text = ClimateTransition.RandomOffsetsPrecipStdDev.ToString();
                    chkClimOffsetsRandIsReplicated.Checked = ClimateTransition.IsRandOffsetReplicated;
                    break;
            }
            FormState = EnumFormState.DISPLAY;
        }
        private void                ClimateTransitionSave(int Index)
        {
            float f = 0;
            //First, place new values into a ClimateTransition struct.
            SClimateTransition ClimateTransition = Global.FIF.ClimateTransitions.Values[Index];
            ClimateTransition.Year = Convert.ToInt32(txbClimTransitionYear.Text);
            ClimateTransition.ValuesType = (cbbClimValuesType.Text == "CONSTANT" ? EClimateValuesType.VTCONSTANT : (cbbClimValuesType.Text == "SPATIAL" ? EClimateValuesType.VTSPATIAL : (cbbClimValuesType.Text == "EXPLICIT" ? EClimateValuesType.VTEXPLICIT : EClimateValuesType.VTRANDEXPLICIT)));
            switch (ClimateTransition.ValuesType)
            {
                case EClimateValuesType.VTCONSTANT:
                    try {
                        ClimateTransition.ConstantTemp = Convert.ToSingle(txbClimValuesTemp.Text);
                        ClimateTransition.ConstantPrecip = Convert.ToSingle(txbClimValuesPrecip.Text);
                    } catch { throw new Exception("Climate transition error: \n\tNumber values expected for Temp and Precip Values when Climate Type is CONSTANT.\n"); }
                    break;
                case EClimateValuesType.VTSPATIAL:
                    if (float.TryParse(txbClimValuesTemp.Text, out f) || float.TryParse(txbClimValuesPrecip.Text, out f))
                        throw new Exception("Climate transition error: \n\tFile path expected for Temp and Precip Values when Climate Type is not CONSTANT.\n");
                    ClimateTransition.SpatialTempFile = txbClimValuesTemp.Text;
                    ClimateTransition.SpatialPrecipFile = txbClimValuesPrecip.Text;
                    break;
                case EClimateValuesType.VTEXPLICIT:
                    if (float.TryParse(txbClimValuesTemp.Text, out f) || float.TryParse(txbClimValuesPrecip.Text, out f))
                        throw new Exception("Climate transition error: \n\tFile path expected for Temp and Precip Values when Climate Type is not CONSTANT.\n");
                    ClimateTransition.SpatialTempFile = txbClimValuesTemp.Text;
                    ClimateTransition.SpatialPrecipFile = txbClimValuesPrecip.Text;
                    break;
                case EClimateValuesType.VTRANDEXPLICIT:
                    if (float.TryParse(txbClimValuesTemp.Text, out f) || float.TryParse(txbClimValuesPrecip.Text, out f)) 
                        throw new Exception("Climate transition error: \n\tFile path expected for Temp and Precip Values when Climate Type is not CONSTANT.\n");
                    ClimateTransition.SpatialTempFile = txbClimValuesTemp.Text;
                    ClimateTransition.SpatialPrecipFile = txbClimValuesPrecip.Text;
                    try {
                        ClimateTransition.RandExplicitMinYear = Convert.ToInt32(txbClimRandExplicitMinYear.Text);
                        ClimateTransition.RandExplicitMaxYear = Convert.ToInt32(txbClimRandExplicitMaxYear.Text);
                    } catch (System.FormatException) {
                        throw new Exception("Climate transition error: \n\tMin Rand Year and Max Rand Year are required when using the RANDEXPLICIT climate value type.\n");
                    }
                    ClimateTransition.IsRandExplicitReplicated = chkClimValuesRandExplicitIsReplicated.Checked;
                    break;
            }
            ClimateTransition.OffsetsType = (cbbClimOffsetsType.Text == "NONE" ? EClimateOffsetsType.OTNONE : (cbbClimOffsetsType.Text == "CONSTANT" ? EClimateOffsetsType.OTCONSTANT : (cbbClimOffsetsType.Text == "FILE" ? EClimateOffsetsType.OTFILE : EClimateOffsetsType.OTRANDOM)));
            switch (ClimateTransition.OffsetsType)
            {
                case EClimateOffsetsType.OTCONSTANT:
                    ClimateTransition.ConstantTempOffset = Convert.ToSingle(txbClimOffsetsTempConstant.Text);
                    ClimateTransition.ConstantPrecipOffset = Convert.ToSingle(txbClimOffsetsPrecipConstant.Text);
                    break;
                case EClimateOffsetsType.OTFILE:
                    ClimateTransition.OffsetsFile = txbClimOffsetsFile.Text;
                    break;
                case EClimateOffsetsType.OTRANDOM:
                    ClimateTransition.OffsetsFile = txbClimOffsetsFile.Text;
                    ClimateTransition.RandomOffsetsTempMean = Convert.ToSingle(txbClimOffsetsTempMean.Text);
                    ClimateTransition.RandomOffsetsTempStdDev = Convert.ToSingle(txbClimOffsetsTempStdDev.Text);
                    ClimateTransition.RandomOffsetsPrecipMean = Convert.ToSingle(txbClimOffsetsPrecipMean.Text);
                    ClimateTransition.RandomOffsetsPrecipStdDev = Convert.ToSingle(txbClimOffsetsPrecipStdDev.Text);
                    ClimateTransition.IsRandOffsetReplicated = chkClimOffsetsRandIsReplicated.Checked;
                    break;
            }
            //Second, remove old settings, add new settings for given index.
            Global.FIF.ClimateTransitions.RemoveAt(Index);
            Global.FIF.ClimateTransitions.Add(ClimateTransition.Year, ClimateTransition);
            ClimateTransitionToSave = -1;
            int newIndex = lstClimTransitions.SelectedIndex;
            ClimateTransitionsRefresh();
            lstClimTransitions.SetSelected(newIndex, true);
            btnClimTransitionAdd.Enabled = true;
        }
        private void                ClimateTransitionRemove(int Index)
        {
            Global.FIF.ClimateTransitions.RemoveAt(Index);
            lstClimTransitions.Items.RemoveAt(Index);
            btnClimTransitionAdd.Enabled = true;
        }
        private string              ClimateTransitionFormat(SClimateTransition ClimateTransition)
        {
            string Format;
            Format = ClimateTransition.Year.ToString(); Format = Format.PadRight(5, ' ');
            Format += ClimateValuesTypeToString(ClimateTransition.ValuesType); Format = Format.PadRight(23, ' ');
            Format += ClimateOffsetsTypeToString(ClimateTransition.OffsetsType); Format = Format.PadRight(25, ' ');
            return Format;
        }
        private void                ClimateTransitionsRefresh()
        {
            lstClimTransitions.Items.Clear();
            foreach (SClimateTransition ClimateTransition in Global.FIF.ClimateTransitions.Values) lstClimTransitions.Items.Add(ClimateTransitionFormat(ClimateTransition));
        }
        private void                OnClimateTransitionChanged(object sender, EventArgs e)
        {
            if (FormState == EnumFormState.DISPLAY) ClimateTransitionToSave = lstClimTransitions.SelectedIndex;
        }

        private void                cbbClimValuesType_SelectionChangeCommited(object sender, EventArgs e)
        {
            LayoutClimate();
            if (FormState == EnumFormState.DISPLAY) ClimateTransitionToSave = lstClimTransitions.SelectedIndex;
        }
        private void                cbbClimOffsetsType_SelectionChangeCommited(object sender, EventArgs e)
        {
            LayoutClimate();
            if (FormState == EnumFormState.DISPLAY) ClimateTransitionToSave = lstClimTransitions.SelectedIndex;
        }
        private void                chkStepsAndRampsEnabled_CheckedChanged(object sender, EventArgs e)
        {
            LayoutClimate();
        }
        private void                btnAddTempStep_Click(object sender, System.EventArgs e)
		{
			if (txbClimOffsetsTempNewStepYear.Text != ""  &&  txbClimOffsetsTempNewStepValue.Text != "") 
			{
				lstClimTempSteps.Items.Add(txbClimOffsetsTempNewStepYear.Text + ": " + txbClimOffsetsTempNewStepValue.Text);
				txbClimOffsetsTempNewStepYear.Clear();
				txbClimOffsetsTempNewStepValue.Clear();
			}
		}
		private void 				btnRemoveTempStep_Click(object sender, System.EventArgs e)
		{
			if (lstClimTempSteps.SelectedIndex >= 0) 
			{
				lstClimTempSteps.Items.Remove(lstClimTempSteps.SelectedItem);
			}
		}
		private void 				btnAddTempRamp_Click(object sender, System.EventArgs e)
		{
			if (txbClimOffsetsTempNewRampYear.Text != ""  &&  txbClimOffsetsTempNewRampValue.Text != "") 
			{
				lstClimTempRamps.Items.Add(txbClimOffsetsTempNewRampYear.Text + ": " + txbClimOffsetsTempNewRampValue.Text);
				txbClimOffsetsTempNewRampYear.Clear();
				txbClimOffsetsTempNewRampValue.Clear();
			}
		}
		private void 				btnRemoveTempRamp_Click(object sender, System.EventArgs e)
		{
			if (lstClimTempRamps.SelectedIndex >= 0) 
			{
				lstClimTempRamps.Items.Remove(lstClimTempRamps.SelectedItem);
			}
		}
		private void 				btnAddPrecipStep_Click(object sender, System.EventArgs e)
		{
			if (txbClimOffsetsPrecipNewStepYear.Text != ""  &&  txbClimOffsetsPrecipNewStepValue.Text != "") 
			{
				lstClimPrecipSteps.Items.Add(txbClimOffsetsPrecipNewStepYear.Text + ": " + txbClimOffsetsPrecipNewStepValue.Text);
				txbClimOffsetsPrecipNewStepYear.Clear();
				txbClimOffsetsPrecipNewStepValue.Clear();
			}
		}
		private void 				btnRemovePrecipStep_Click(object sender, System.EventArgs e)
		{
			if (lstClimPrecipSteps.SelectedIndex >= 0) 
			{
				lstClimPrecipSteps.Items.Remove(lstClimPrecipSteps.SelectedItem);
			}
		}
        private void                btnAddPrecipRamp_Click(object sender, EventArgs e)
        {
            if (txbClimOffsetsPrecipNewRampYear.Text != "" && txbClimOffsetsPrecipNewRampValue.Text != "")
            {
                lstClimPrecipRamps.Items.Add(txbClimOffsetsPrecipNewRampYear.Text + ": " + txbClimOffsetsPrecipNewRampValue.Text);
                txbClimOffsetsPrecipNewRampYear.Clear();
                txbClimOffsetsPrecipNewRampValue.Clear();
            }
        }
		private void 				btnRemovePrecipRamp_Click(object sender, System.EventArgs e)
		{
			if (lstClimPrecipRamps.SelectedIndex >= 0) 
			{
				lstClimPrecipRamps.Items.Remove(lstClimPrecipRamps.SelectedItem);
			}
		}
        

		//Fire
		private void				lstFireTransitions_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if (lstFireTypeTransitions.SelectedIndex!=-1) 
			{
				if (FireTransitionToSave!=-1) 
					FireTransitionSave(FireTransitionToSave);
				else
					FireTransitionRefresh(Global.FIF.FireTransitions.Values[lstFireTypeTransitions.SelectedIndex]);
			}
			else
				ClearFireTranstionControls();
		}
		private void				cbbFire_SelectionChangeCommitted(object sender, System.EventArgs e)
		{
			txbFireIgnition.Enabled				= false;
			txbFireSensitivity.Enabled			= false;		
			txbFireSpatialIgnition.Enabled		= false;
			txbFireSpatialSensitivity.Enabled	= false;		
			txbFireHistorical.Enabled			= false;
			txbFireHistorical.Enabled			= false;
			switch(cbbFireType.SelectedItem!=null ? cbbFireType.SelectedItem.ToString() : cbbFireType.SelectedText) 
			{
			case "FIXED" :
				txbFireIgnition.Enabled = true;
				txbFireSensitivity.Enabled = true;		
				break;
			case "SPATIAL" :
				txbFireSpatialIgnition.Enabled = true;
				txbFireSpatialSensitivity.Enabled = true;		
				break;
			case "HISTORICAL" :
				txbFireHistorical.Enabled = true;
				break;
			}					
			if (FormState==EnumFormState.DISPLAY) FireTransitionToSave = lstFireTypeTransitions.SelectedIndex;
		}
		private void				btnFireTransitionAdd_Click(object sender, System.EventArgs e)
		{
			SFireTransition FireTransition;
			FireTransition = new SFireTransition();
			FireTransition.Year = (Global.FIF.FireTransitions.Count>0 ? Global.FIF.FireTransitions.Values[Global.FIF.FireTransitions.Count-1].Year+1 : 0);
			FireTransition.Type = EFireType.FIXED;
			Global.FIF.FireTransitions.Add(FireTransition.Year,FireTransition);
			FireTransitionRefresh(FireTransition);
			FireTransitionsRefresh();
		}
		private void				btnFireTransitionRemove_Click(object sender, System.EventArgs e)
		{
			if (lstFireTypeTransitions.SelectedIndex!=-1) FireTransitionRemove(lstFireTypeTransitions.SelectedIndex);
		}
        private void                ClearFireTranstionControls()
        {
            txbFireTypeTransitionYear.Text = "";
            cbbFireType.Text = "";
            txbFireIgnition.Text = "";
            txbFireSensitivity.Text = "";
            txbFireSpatialIgnition.Text = "";
            txbFireSpatialSensitivity.Text = "";
            txbFireTypeTransitionYear.Enabled = false;
            cbbFireType.Enabled = false;
            txbFireIgnition.Enabled = false;
            txbFireSensitivity.Enabled = false;
            txbFireSpatialIgnition.Enabled = false;
            txbFireSpatialSensitivity.Enabled = false;
            txbFireHistorical.Enabled = false;
        }
		private void				FireTransitionRefresh(SFireTransition FireTransition)
		{
			FormState=EnumFormState.REFRESH;
            
            ClearFireTranstionControls();
            
            txbFireTypeTransitionYear.Text	= FireTransition.Year.ToString();
			cbbFireType.Text				= FireTransition.Type.ToString();
			txbFireIgnition.Text			= FireTransition.Ignition.ToString();
			txbFireSensitivity.Text			= FireTransition.Sensitivity.ToString();
			txbFireSpatialIgnition.Text		= FireTransition.SpatialIgnitionFile;
			txbFireSpatialSensitivity.Text	= FireTransition.SpatialSensitivityFile;
			txbFireHistorical.Text			= FireTransition.HistoricalFile;
			txbFireTypeTransitionYear.Enabled = true;
			cbbFireType.Enabled = true;
			switch(FireTransition.Type) 
			{
				case  EFireType.FIXED :
					txbFireIgnition.Enabled = true;
					txbFireSensitivity.Enabled = true;		
					break;
				case  EFireType.SPATIAL :
					txbFireSpatialIgnition.Enabled = true;
					txbFireSpatialSensitivity.Enabled = true;		
					break;
				case  EFireType.HISTORICAL :
					txbFireHistorical.Enabled = true;
					break;
			}

			FormState=EnumFormState.DISPLAY;
		}
		private void				FireTransitionSave(int Index)
		{
			SFireTransition FireTransition = Global.FIF.FireTransitions.Values[Index];
			Global.FIF.FireTransitions.RemoveAt(Index);
			FireTransition.Year							= Convert.ToInt32(txbFireTypeTransitionYear.Text);
			FireTransition.Type							= (cbbFireType.Text=="FIXED"?EFireType.FIXED : (cbbFireType.Text=="HISTORICAL"?EFireType.HISTORICAL : EFireType.SPATIAL));
			FireTransition.Ignition						= Convert.ToSingle(txbFireIgnition.Text);
			FireTransition.Sensitivity					= Convert.ToSingle(txbFireSensitivity.Text);
			FireTransition.SpatialIgnitionFile			= txbFireSpatialIgnition.Text;
			FireTransition.SpatialSensitivityFile		= txbFireSpatialSensitivity.Text;
			FireTransition.HistoricalFile				= txbFireHistorical.Text;
			Global.FIF.FireTransitions.Add(FireTransition.Year,FireTransition);
			FireTransitionToSave = -1;
            int newIndex = lstFireTypeTransitions.SelectedIndex;
            FireTransitionsRefresh();
            lstFireTypeTransitions.SetSelected(newIndex, true);
			btnFireTransitionAdd.Enabled = true;
		}
		private void				FireTransitionRemove(int Index)
		{
			Global.FIF.FireTransitions.RemoveAt(Index);
			lstFireTypeTransitions.Items.RemoveAt(Index);
			btnFireTransitionAdd.Enabled = true;
		}
		private string				FireTransitionFormat(SFireTransition FireTransition)
		{
			string Format;
			Format = FireTransition.Year.ToString();		Format = Format.PadRight(5,' ');
			Format += FireTransition.Type.ToString();		Format = Format.PadRight(16,' ');
			switch(FireTransition.Type) 
			{
				case EFireType.FIXED:
					Format += "IGN:"	+ FireTransition.Ignition.ToString();
					Format += ", SEN:"	+ FireTransition.Sensitivity.ToString();
					break;
				case EFireType.SPATIAL:
					Format += "IGN:"	+ FireTransition.SpatialIgnitionFile;
					Format += ", SEN:"	+ FireTransition.SpatialSensitivityFile;
					break;
				case EFireType.HISTORICAL:
					Format += "HIST:"	+ FireTransition.HistoricalFile;
					break;
			}
			return Format;
		}
		private void				FireTransitionsRefresh()
		{
			lstFireTypeTransitions.Items.Clear();
			foreach (SFireTransition FireTransition in Global.FIF.FireTransitions.Values) lstFireTypeTransitions.Items.Add(FireTransitionFormat(FireTransition));
		}
		private void				OnFireTransitionChanged(object sender, EventArgs e)
		{
			if (FormState==EnumFormState.DISPLAY) FireTransitionToSave = lstFireTypeTransitions.SelectedIndex;
		}

		//Map
		private void				FIFFlagsChanged(object sender, System.EventArgs e)
		{
			int flags=0;
			//Sum all map flags.
			flags += chkMapIsOutput.Checked ? outMapData : 0;
			flags += chkMapFormat.Checked ? outMapFormat : 0;
			flags += chkMapHeader.Checked ? outHeader : 0;
			flags += chkMapARC.Checked ? outARC : 0;
			flags += chkMapVeg.Checked ? outVeg : 0;
			flags += chkMapAge.Checked ? outAge : 0;
			flags += chkMapSite.Checked ? outSite : 0;
			flags += chkMapSub.Checked ? outSub : 0;
			flags += chkMapFireAge.Checked ? outFireAge : 0;
			flags += chkMapTemp.Checked ? outTemp : 0;
			flags += chkMapPrecip.Checked ? outPrecip : 0;
			flags += chkMapFireScar.Checked ? outFireScar : 0;
//			flags += chkMapFireScarSum.Checked ? outFireScarSum : 0;
			flags += chkMapFrame1.Checked ? out1 : 0;
			flags += chkMapFrame2.Checked ? out2 : 0;
			flags += chkMapFrame3.Checked ? out3 : 0;
			flags += chkMapFrame4.Checked ? out4 : 0;		
			//Update label.
			lblMapFlags.Text = "FIF Flags: 0x" + flags.ToString("x");
		}
		private void				cbbMapOutputSelect_SelectionChangeCommited(object sender, System.EventArgs e)
		{
			CommitCbbMapOutputSelect();
			RefreshCbbMapOutputSelect();		
		}
		private void				cbbMapOutputSelect_TextChanged(object sender, System.EventArgs e)
		{
			if (storedMapIndex>=0 && storedMapIndex<cbbMapOutputSelect.Items.Count) 
			{
				//Index is in range.
				if (cbbMapOutputSelect.Text != Global.FIF.MapFiles[storedMapIndex].ToString()) {
					//Text has been changed by user.
					Global.FIF.MapFiles[storedMapIndex] = cbbMapOutputSelect.Text;
				}
			}
		}
		private void				cbbMapOutputSelect_DropDown(object sender, System.EventArgs e)
		{
			//Update the list before displaying.
			if (storedMapIndex>=0 && storedMapIndex<cbbMapOutputSelect.Items.Count) 
			{
				cbbMapOutputSelect.Items.RemoveAt(storedMapIndex);
				cbbMapOutputSelect.Items.Insert(storedMapIndex, Global.FIF.MapFiles[storedMapIndex].ToString());
				cbbMapOutputSelect.Text = Global.FIF.MapFiles[storedMapIndex].ToString();
			}
		}

		private void				btnMapAdd_Click(object sender, System.EventArgs e)
		{
			string msg = "Enter new file Filename here";
			//Commit current edits.
			CommitCbbMapOutputSelect();
			//Add default values to Global.FIF.
			Global.FIF.MapFiles[0]= msg;
			Global.FIF.MapCodes.Add("New");
			Global.FIF.MapFlags.Add(0);
			Global.FIF.MapRepStart.Add(0);
			Global.FIF.MapRepFreq.Add(1);
			Global.FIF.MapYearStart.Add(0);
			Global.FIF.MapYearFreq.Add(1);
			//Display new values.
			cbbMapOutputSelect.Items.Add(msg);
			cbbMapOutputSelect.SelectedIndex = storedMapIndex = cbbMapOutputSelect.Items.IndexOf(msg);
			RefreshCbbMapOutputSelect();
			cbbMapOutputSelect.Focus();
		}
		private void				btnMapDelete_Click(object sender, System.EventArgs e)
		{
			//Delete current index from Global.Fif.
			int i = cbbMapOutputSelect.SelectedIndex;
			if (i>=0 && i<Global.FIF.MapFiles.Count) 
			{
				Global.FIF.MapFiles.RemoveAt(i);
				Global.FIF.MapCodes.RemoveAt(i);
				Global.FIF.MapFlags.RemoveAt(i);
				Global.FIF.MapRepStart.RemoveAt(i);
				Global.FIF.MapRepFreq.RemoveAt(i);
				Global.FIF.MapYearStart.RemoveAt(i);
				Global.FIF.MapYearFreq.RemoveAt(i);
				//Delete current index from combo box.
				cbbMapOutputSelect.Items.RemoveAt(i);
				RefreshCbbMapOutputSelect();
			}
		}

		//Stat Tab Events.
		private void				cbbStatSelect_DropDown(object sender, System.EventArgs e)
		{
			CommitCbbStatSelect();
		}
		private void				cbbStatSelect_SelectionChangeCommitted(object sender, System.EventArgs e)
		{
			RefreshCbbStatSelect();
		}

		//Species Tab Events.
		private void				cmbSpecies_SelectionChanged(object sender, System.EventArgs e)
		{
			pnlTundra.Visible	= false;
			pnlBSpruce.Visible	= false;
			pnlWSpruce.Visible	= false;
			pnlDecid.Visible	= false;

			pnlTundra.Enabled	= false;
			pnlBSpruce.Enabled	= false;
			pnlWSpruce.Enabled	= false;
			pnlDecid.Enabled	= false;

			switch(cmbSpecies.SelectedItem.ToString())
			{
				case "Tundra" :
					pnlTundra.BringToFront();
					pnlTundra.Enabled	= true;
					pnlTundra.Visible	= true;
					break;
				case "Black Spruce" :
					pnlBSpruce.BringToFront();
					pnlBSpruce.Enabled	= true;
					pnlBSpruce.Visible	= true;
					break;
				case "White Spruce" :
					pnlWSpruce.BringToFront();
					pnlWSpruce.Enabled	= true;
					pnlWSpruce.Visible	= true;
					break;
				case "Deciduous" :
					pnlDecid.BringToFront();
					pnlDecid.Enabled	= true;
					pnlDecid.Visible	= true;
					break;
			}
		}

		//Form Events.
		private void				FIFEditor_Load(object sender, System.EventArgs e)
		{
			pnlTundra.Visible	= false;
			pnlBSpruce.Visible	= false;
			pnlWSpruce.Visible	= false;
			pnlDecid.Visible	= false;

			pnlTundra.Enabled	= false;
			pnlBSpruce.Enabled	= false;
			pnlWSpruce.Enabled	= false;
			pnlDecid.Enabled	= false;

			chkStatYear.Enabled = true;		
			chkStatRep.Enabled = true;		
			chkStatFormat.Enabled = true;		
			chkStatNum.Enabled = true;		
			chkStatMean.Enabled = true;		
			chkStatStdDev.Enabled = true;		
			chkStatMin.Enabled = true;		
			chkStatMax.Enabled = true;		
			chkStatHist.Enabled = true;		
			chkStatData.Enabled = true;		
			chkStatEvents.Enabled = true;

			Refresh();
            LayoutClimate();
            LayoutLandscape();
        }
		#endregion //End Events.

        public override void		Edited(bool IsValid)
		{
        }
        private void                LayoutLandscape()
        {
            txbLandYearOfVegAndAgeFiles.Enabled = chkLandIsUsingUniqueVegAndAgePerRep.Checked;
        }
        private void                LayoutClimate()
        {
            //Values Layout.
            string valueType = cbbClimValuesType.SelectedItem != null ? cbbClimValuesType.SelectedItem.ToString() : cbbClimValuesType.SelectedText;
            switch (valueType)
            {
                case "CONSTANT":
                    pnlClimRand.Enabled = false;
                    lblTempValues.Text = "Constant Temp";
                    lblPrecipValues.Text = "Constant Precip";
                    break;
                case "SPATIAL":
                    pnlClimRand.Enabled = false;
                    lblTempValues.Text = "Temp File";
                    lblPrecipValues.Text = "Precip File";
                    break;
                case "EXPLICIT":
                    pnlClimRand.Enabled = false;
                    lblTempValues.Text = "Temp File";
                    lblPrecipValues.Text = "Precip File";
                    break;
                case "RANDEXPLICIT":
                    pnlClimRand.Enabled = true;
                    lblTempValues.Text = "Temp File";
                    lblPrecipValues.Text = "Precip File";
                    break;
            }

            //Offsets Layout.
            string offsetType = cbbClimOffsetsType.SelectedItem != null ? cbbClimOffsetsType.SelectedItem.ToString() : cbbClimOffsetsType.SelectedText;
            switch (offsetType)
            {
                case "NONE":
                    pnlOffsetsConstant.Enabled = false;
                    pnlOffsetsFile.Enabled = false;
                    pnlOffsetsRandom.Enabled = false;
                    break;
                case "CONSTANT":
                    pnlOffsetsConstant.Enabled = true;
                    pnlOffsetsFile.Enabled = false;
                    pnlOffsetsRandom.Enabled = false;
                    break;
                case "FILE":
                    pnlOffsetsConstant.Enabled = false;
                    pnlOffsetsFile.Enabled = true;
                    pnlOffsetsRandom.Enabled = false;
                    break;
                case "RANDOM":
                    pnlOffsetsConstant.Enabled = false;
                    pnlOffsetsFile.Enabled = false;
                    pnlOffsetsRandom.Enabled = true;
                    break;
            }

            //Ramping Layout.
            tabStepsAndRamps.Enabled = chkStepsAndRampsEnabled.Checked;
        }
        string			            ClimateValuesTypeToString(EClimateValuesType VT)
	    {
		    string result= "INVALID";
		    if		(VT == EClimateValuesType.VTCONSTANT)		result="CONSTANT";
		    else if (VT == EClimateValuesType.VTSPATIAL)		result="SPATIAL";
		    else if (VT == EClimateValuesType.VTEXPLICIT)		result="EXPLICIT";
		    else if (VT == EClimateValuesType.VTRANDEXPLICIT)	result="RANDEXPLICIT";
            return result;
	    }
        string                      ClimateOffsetsTypeToString(EClimateOffsetsType OT)
        {
		    string result= "INVALID";
            if      (OT == EClimateOffsetsType.OTNONE)         result = "NONE";
            else if (OT == EClimateOffsetsType.OTCONSTANT)     result = "CONSTANT";
            else if (OT == EClimateOffsetsType.OTFILE)         result = "FILE";
            else if (OT == EClimateOffsetsType.OTRANDOM)       result = "RANDOM";
            return result;
        }
    }
}
