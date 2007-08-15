using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;

namespace FRESCO_Server
{
	partial class EditorFIF
	{
		#region Designer Code

		private System.Windows.Forms.ToolTip ToolTips;
		private System.Windows.Forms.CheckBox chkIsLargeMemoryModel;	
		private System.ComponentModel.IContainer components;
	
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditorFIF));
            this.chkIsLargeMemoryModel = new System.Windows.Forms.CheckBox();
            this.ToolTips = new System.Windows.Forms.ToolTip(this.components);
            this.txbGenBaseDirectory = new System.Windows.Forms.TextBox();
            this.fldFileName = new System.Windows.Forms.Label();
            this.lblFileName = new System.Windows.Forms.Label();
            this.tabFIF = new System.Windows.Forms.TabControl();
            this.tabGeneral = new System.Windows.Forms.TabPage();
            this.btnOutputDirLookup = new System.Windows.Forms.Button();
            this.cbbGenRandSeed = new System.Windows.Forms.ComboBox();
            this.txbGenMaxYears = new System.Windows.Forms.TextBox();
            this.btnBaseDirLookup = new System.Windows.Forms.Button();
            this.chkGenIsLargeMemoryModel = new System.Windows.Forms.CheckBox();
            this.label9 = new System.Windows.Forms.Label();
            this.cbbGenOutputDetailLevel = new System.Windows.Forms.ComboBox();
            this.cbbGenOutputType = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label__ = new System.Windows.Forms.Label();
            this.txbGenOutputDirectory = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txbGenTimeStep = new System.Windows.Forms.TextBox();
            this.txbGenNumHeader = new System.Windows.Forms.TextBox();
            this.txbGenNoData = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txbGenMaxReps = new System.Windows.Forms.TextBox();
            this.tabFire = new System.Windows.Forms.TabPage();
            this.txbFireMaxEmpiricalFireSizeEvent = new System.Windows.Forms.TextBox();
            this.txbFireMaxEmpiricalFireSizeEventWeight = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.txbFireSpreadStdDev = new System.Windows.Forms.TextBox();
            this.txbFireSpreadMean = new System.Windows.Forms.TextBox();
            this.txbFireSpreadRadius = new System.Windows.Forms.TextBox();
            this.label48 = new System.Windows.Forms.Label();
            this.chkFireIgnoreFirstInterval = new System.Windows.Forms.CheckBox();
            this.label49 = new System.Windows.Forms.Label();
            this.label50 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.txbFireSuppressionClass2 = new System.Windows.Forms.TextBox();
            this.chkFireSuppressionOn = new System.Windows.Forms.CheckBox();
            this.txbFireSuppressionClass1 = new System.Windows.Forms.TextBox();
            this.label106 = new System.Windows.Forms.Label();
            this.label105 = new System.Windows.Forms.Label();
            this.txbFireSuppressionFile = new System.Windows.Forms.TextBox();
            this.label104 = new System.Windows.Forms.Label();
            this.label103 = new System.Windows.Forms.Label();
            this.label97 = new System.Windows.Forms.Label();
            this.label102 = new System.Windows.Forms.Label();
            this.label94 = new System.Windows.Forms.Label();
            this.txbFireSuppressionClass5 = new System.Windows.Forms.TextBox();
            this.txbFireSuppressionClass4 = new System.Windows.Forms.TextBox();
            this.txbFireSuppressionClass3 = new System.Windows.Forms.TextBox();
            this.txbFireSuppressionThresholdIgnitions = new System.Windows.Forms.TextBox();
            this.label107 = new System.Windows.Forms.Label();
            this.txbFireSuppressionThresholdFireSize = new System.Windows.Forms.TextBox();
            this.label108 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label42 = new System.Windows.Forms.Label();
            this.btnFireTransitionRemove = new System.Windows.Forms.Button();
            this.txbFireTypeTransitionYear = new System.Windows.Forms.TextBox();
            this.label95 = new System.Windows.Forms.Label();
            this.btnFireTransitionAdd = new System.Windows.Forms.Button();
            this.lstFireTypeTransitions = new System.Windows.Forms.ListBox();
            this.label60 = new System.Windows.Forms.Label();
            this.label63 = new System.Windows.Forms.Label();
            this.label52 = new System.Windows.Forms.Label();
            this.txbFireIgnition = new System.Windows.Forms.TextBox();
            this.txbFireSpatialSensitivity = new System.Windows.Forms.TextBox();
            this.txbFireSensitivity = new System.Windows.Forms.TextBox();
            this.txbFireSpatialIgnition = new System.Windows.Forms.TextBox();
            this.label47 = new System.Windows.Forms.Label();
            this.label46 = new System.Windows.Forms.Label();
            this.label44 = new System.Windows.Forms.Label();
            this.label43 = new System.Windows.Forms.Label();
            this.cbbFireType = new System.Windows.Forms.ComboBox();
            this.label45 = new System.Windows.Forms.Label();
            this.txbFireHistorical = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.txbFireClimateC = new System.Windows.Forms.TextBox();
            this.txbFireClimateB = new System.Windows.Forms.TextBox();
            this.txbFireClimateA = new System.Windows.Forms.TextBox();
            this.label56 = new System.Windows.Forms.Label();
            this.label55 = new System.Windows.Forms.Label();
            this.label54 = new System.Windows.Forms.Label();
            this.label51 = new System.Windows.Forms.Label();
            this.chkFireEnabled = new System.Windows.Forms.CheckBox();
            this.label68 = new System.Windows.Forms.Label();
            this.txbFireHumanIgnitionFile = new System.Windows.Forms.TextBox();
            this.label27 = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.tabClimate = new System.Windows.Forms.TabPage();
            this.grpClimateTransitions = new System.Windows.Forms.GroupBox();
            this.txbClimTransitionYear = new System.Windows.Forms.TextBox();
            this.label125 = new System.Windows.Forms.Label();
            this.cbbClimValuesType = new System.Windows.Forms.ComboBox();
            this.lstClimTransitions = new System.Windows.Forms.ListBox();
            this.label128 = new System.Windows.Forms.Label();
            this.label39 = new System.Windows.Forms.Label();
            this.grpClimOffsets = new System.Windows.Forms.GroupBox();
            this.pnlOffsetsConstant = new System.Windows.Forms.Panel();
            this.label113 = new System.Windows.Forms.Label();
            this.label112 = new System.Windows.Forms.Label();
            this.txbClimOffsetsPrecipConstant = new System.Windows.Forms.TextBox();
            this.txbClimOffsetsTempConstant = new System.Windows.Forms.TextBox();
            this.pnlOffsetsRandom = new System.Windows.Forms.Panel();
            this.chkClimOffsetsRandIsReplicated = new System.Windows.Forms.CheckBox();
            this.label26 = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.txbClimOffsetsPrecipStdDev = new System.Windows.Forms.TextBox();
            this.txbClimOffsetsPrecipMean = new System.Windows.Forms.TextBox();
            this.label30 = new System.Windows.Forms.Label();
            this.txbClimOffsetsTempStdDev = new System.Windows.Forms.TextBox();
            this.label29 = new System.Windows.Forms.Label();
            this.txbClimOffsetsTempMean = new System.Windows.Forms.TextBox();
            this.cbbClimOffsetsType = new System.Windows.Forms.ComboBox();
            this.label35 = new System.Windows.Forms.Label();
            this.pnlOffsetsFile = new System.Windows.Forms.Panel();
            this.label111 = new System.Windows.Forms.Label();
            this.txbClimOffsetsFile = new System.Windows.Forms.TextBox();
            this.label36 = new System.Windows.Forms.Label();
            this.label40 = new System.Windows.Forms.Label();
            this.pnlClimRand = new System.Windows.Forms.Panel();
            this.label6 = new System.Windows.Forms.Label();
            this.chkClimValuesRandExplicitIsReplicated = new System.Windows.Forms.CheckBox();
            this.txbClimRandExplicitMinYear = new System.Windows.Forms.TextBox();
            this.txbClimRandExplicitMaxYear = new System.Windows.Forms.TextBox();
            this.label110 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.pnlClimValues = new System.Windows.Forms.Panel();
            this.txbClimValuesPrecip = new System.Windows.Forms.TextBox();
            this.lblTempValues = new System.Windows.Forms.Label();
            this.txbClimValuesTemp = new System.Windows.Forms.TextBox();
            this.lblPrecipValues = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label121 = new System.Windows.Forms.Label();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.textBox8 = new System.Windows.Forms.TextBox();
            this.textBox9 = new System.Windows.Forms.TextBox();
            this.label122 = new System.Windows.Forms.Label();
            this.btnClimTransitionRemove = new System.Windows.Forms.Button();
            this.panel5 = new System.Windows.Forms.Panel();
            this.textBox10 = new System.Windows.Forms.TextBox();
            this.label123 = new System.Windows.Forms.Label();
            this.textBox11 = new System.Windows.Forms.TextBox();
            this.label124 = new System.Windows.Forms.Label();
            this.label126 = new System.Windows.Forms.Label();
            this.btnClimTransitionAdd = new System.Windows.Forms.Button();
            this.label127 = new System.Windows.Forms.Label();
            this.tabStepsAndRamps = new System.Windows.Forms.TabControl();
            this.tabPageSteps = new System.Windows.Forms.TabPage();
            this.btnRemoveTempStep = new System.Windows.Forms.Button();
            this.txbClimOffsetsTempNewStepYear = new System.Windows.Forms.TextBox();
            this.label22 = new System.Windows.Forms.Label();
            this.txbClimOffsetsTempNewStepValue = new System.Windows.Forms.TextBox();
            this.lblTempStepYear = new System.Windows.Forms.Label();
            this.lstClimTempSteps = new System.Windows.Forms.ListBox();
            this.btnAddTempStep = new System.Windows.Forms.Button();
            this.txbClimOffsetsPrecipNewStepYear = new System.Windows.Forms.TextBox();
            this.btnRemovePrecipStep = new System.Windows.Forms.Button();
            this.lstClimPrecipSteps = new System.Windows.Forms.ListBox();
            this.btnAddPrecipStep = new System.Windows.Forms.Button();
            this.txbClimOffsetsPrecipNewStepValue = new System.Windows.Forms.TextBox();
            this.label34 = new System.Windows.Forms.Label();
            this.label33 = new System.Windows.Forms.Label();
            this.tabPageRamps = new System.Windows.Forms.TabPage();
            this.txbClimOffsetsPrecipNewRampYear = new System.Windows.Forms.TextBox();
            this.btnRemovePrecipRamp = new System.Windows.Forms.Button();
            this.lstClimPrecipRamps = new System.Windows.Forms.ListBox();
            this.btnAddPrecipRamp = new System.Windows.Forms.Button();
            this.label32 = new System.Windows.Forms.Label();
            this.label31 = new System.Windows.Forms.Label();
            this.txbClimOffsetsPrecipNewRampValue = new System.Windows.Forms.TextBox();
            this.txbClimOffsetsTempNewRampYear = new System.Windows.Forms.TextBox();
            this.btnAddTempRamp = new System.Windows.Forms.Button();
            this.txbClimOffsetsTempNewRampValue = new System.Windows.Forms.TextBox();
            this.btnRemoveTempRamp = new System.Windows.Forms.Button();
            this.lstClimTempRamps = new System.Windows.Forms.ListBox();
            this.label23 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.chkStepsAndRampsEnabled = new System.Windows.Forms.CheckBox();
            this.txbClimNumHistory = new System.Windows.Forms.TextBox();
            this.label38 = new System.Windows.Forms.Label();
            this.tabMapOutput = new System.Windows.Forms.TabPage();
            this.chkMapFireScar = new System.Windows.Forms.CheckBox();
            this.btnMapDelete = new System.Windows.Forms.Button();
            this.chkMapFrame4 = new System.Windows.Forms.CheckBox();
            this.chkMapFrame3 = new System.Windows.Forms.CheckBox();
            this.chkMapFrame2 = new System.Windows.Forms.CheckBox();
            this.chkMapFrame1 = new System.Windows.Forms.CheckBox();
            this.chkMapPrecip = new System.Windows.Forms.CheckBox();
            this.chkMapTemp = new System.Windows.Forms.CheckBox();
            this.chkMapFireAge = new System.Windows.Forms.CheckBox();
            this.chkMapSub = new System.Windows.Forms.CheckBox();
            this.chkMapSite = new System.Windows.Forms.CheckBox();
            this.chkMapAge = new System.Windows.Forms.CheckBox();
            this.chkMapVeg = new System.Windows.Forms.CheckBox();
            this.chkMapARC = new System.Windows.Forms.CheckBox();
            this.chkMapHeader = new System.Windows.Forms.CheckBox();
            this.chkMapFormat = new System.Windows.Forms.CheckBox();
            this.chkMapIsOutput = new System.Windows.Forms.CheckBox();
            this.cbbMapOutputSelect = new System.Windows.Forms.ComboBox();
            this.label67 = new System.Windows.Forms.Label();
            this.label66 = new System.Windows.Forms.Label();
            this.label65 = new System.Windows.Forms.Label();
            this.label64 = new System.Windows.Forms.Label();
            this.btnMapAdd = new System.Windows.Forms.Button();
            this.lblMapFlags = new System.Windows.Forms.Label();
            this.txbMapYearFreq = new System.Windows.Forms.TextBox();
            this.txbMapYearStart = new System.Windows.Forms.TextBox();
            this.txbMapRepFreq = new System.Windows.Forms.TextBox();
            this.txbMapRepStart = new System.Windows.Forms.TextBox();
            this.txbMapCodes = new System.Windows.Forms.TextBox();
            this.label61 = new System.Windows.Forms.Label();
            this.label59 = new System.Windows.Forms.Label();
            this.label58 = new System.Windows.Forms.Label();
            this.label57 = new System.Windows.Forms.Label();
            this.label53 = new System.Windows.Forms.Label();
            this.tabStatOutput = new System.Windows.Forms.TabPage();
            this.chkStatEvents = new System.Windows.Forms.CheckBox();
            this.chkStatData = new System.Windows.Forms.CheckBox();
            this.chkStatHist = new System.Windows.Forms.CheckBox();
            this.chkStatMax = new System.Windows.Forms.CheckBox();
            this.chkStatMin = new System.Windows.Forms.CheckBox();
            this.chkStatStdDev = new System.Windows.Forms.CheckBox();
            this.chkStatMean = new System.Windows.Forms.CheckBox();
            this.chkStatNum = new System.Windows.Forms.CheckBox();
            this.chkStatFormat = new System.Windows.Forms.CheckBox();
            this.chkStatRep = new System.Windows.Forms.CheckBox();
            this.chkStatYear = new System.Windows.Forms.CheckBox();
            this.cbbStatSelect = new System.Windows.Forms.ComboBox();
            this.txbStatSummaryFile = new System.Windows.Forms.TextBox();
            this.label62 = new System.Windows.Forms.Label();
            this.label96 = new System.Windows.Forms.Label();
            this.tabLandscape = new System.Windows.Forms.TabPage();
            this.txbLandYearOfVegAndAgeFiles = new System.Windows.Forms.TextBox();
            this.chkLandIsUsingUniqueVegAndAgePerRep = new System.Windows.Forms.CheckBox();
            this.chkLandIsForcedVegTransitions = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label109 = new System.Windows.Forms.Label();
            this.txbLandVegTransitionFile = new System.Windows.Forms.TextBox();
            this.btnLandTreeDensityInputFileLookup = new System.Windows.Forms.Button();
            this.btnLandSiteInputFileLookup = new System.Windows.Forms.Button();
            this.btnLandAgeInputFileLookup = new System.Windows.Forms.Button();
            this.btnLandVegInputFileLookup = new System.Windows.Forms.Button();
            this.label21 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.chkLandCropNeighbors = new System.Windows.Forms.CheckBox();
            this.txbLandMaxCol = new System.Windows.Forms.TextBox();
            this.txbLandCellSize = new System.Windows.Forms.TextBox();
            this.txbLandNumSpecies = new System.Windows.Forms.TextBox();
            this.txbLandNoVeg = new System.Windows.Forms.TextBox();
            this.txbLandXllCorner = new System.Windows.Forms.TextBox();
            this.txbLandYllCorner = new System.Windows.Forms.TextBox();
            this.txbLandSiteInputFile = new System.Windows.Forms.TextBox();
            this.txbLandAgeInputFile = new System.Windows.Forms.TextBox();
            this.txbLandTreeDensityInputFile = new System.Windows.Forms.TextBox();
            this.txbLandVegInputFile = new System.Windows.Forms.TextBox();
            this.txbLandMaxRow = new System.Windows.Forms.TextBox();
            this.label37 = new System.Windows.Forms.Label();
            this.pnlSpecies = new System.Windows.Forms.TabPage();
            this.cmbSpecies = new System.Windows.Forms.ComboBox();
            this.label69 = new System.Windows.Forms.Label();
            this.pnlDecid = new System.Windows.Forms.Panel();
            this.txbDecidHumanFireProb = new System.Windows.Forms.TextBox();
            this.label98 = new System.Windows.Forms.Label();
            this.txbDecidWSpruceParms = new System.Windows.Forms.TextBox();
            this.txbDecidBSpruceParms = new System.Windows.Forms.TextBox();
            this.txbDecidTundraParms = new System.Windows.Forms.TextBox();
            this.txbDecidHistory = new System.Windows.Forms.TextBox();
            this.txbDecidFireProb = new System.Windows.Forms.TextBox();
            this.txbDecidStartAgeBSpruce = new System.Windows.Forms.TextBox();
            this.txbDecidStartAgeWSpruce = new System.Windows.Forms.TextBox();
            this.label89 = new System.Windows.Forms.Label();
            this.label88 = new System.Windows.Forms.Label();
            this.label87 = new System.Windows.Forms.Label();
            this.label86 = new System.Windows.Forms.Label();
            this.label85 = new System.Windows.Forms.Label();
            this.label81 = new System.Windows.Forms.Label();
            this.label71 = new System.Windows.Forms.Label();
            this.pnlBSpruce = new System.Windows.Forms.Panel();
            this.txbBSpruceHumanFireProb = new System.Windows.Forms.TextBox();
            this.label101 = new System.Windows.Forms.Label();
            this.txbBSpruceFireParms = new System.Windows.Forms.TextBox();
            this.txbBSpruceStartAge = new System.Windows.Forms.TextBox();
            this.label92 = new System.Windows.Forms.Label();
            this.label93 = new System.Windows.Forms.Label();
            this.pnlTundra = new System.Windows.Forms.Panel();
            this.txbTundraHumanFireProb = new System.Windows.Forms.TextBox();
            this.label84 = new System.Windows.Forms.Label();
            this.label83 = new System.Windows.Forms.Label();
            this.label82 = new System.Windows.Forms.Label();
            this.label80 = new System.Windows.Forms.Label();
            this.label79 = new System.Windows.Forms.Label();
            this.label78 = new System.Windows.Forms.Label();
            this.label77 = new System.Windows.Forms.Label();
            this.label76 = new System.Windows.Forms.Label();
            this.label75 = new System.Windows.Forms.Label();
            this.label74 = new System.Windows.Forms.Label();
            this.label73 = new System.Windows.Forms.Label();
            this.label72 = new System.Windows.Forms.Label();
            this.label500 = new System.Windows.Forms.Label();
            this.txbTundraSpruceBasalArea = new System.Windows.Forms.TextBox();
            this.txbTundraCalFactor = new System.Windows.Forms.TextBox();
            this.txbTundraClimGrowth = new System.Windows.Forms.TextBox();
            this.txbTundraMeanGrowth = new System.Windows.Forms.TextBox();
            this.txbTundraSeedEstParms = new System.Windows.Forms.TextBox();
            this.txbTundraHistory = new System.Windows.Forms.TextBox();
            this.txbTundraSeedlingBA = new System.Windows.Forms.TextBox();
            this.txbTundraSeedling = new System.Windows.Forms.TextBox();
            this.txbTundraSeedBasalArea = new System.Windows.Forms.TextBox();
            this.txbTundraSeedSource = new System.Windows.Forms.TextBox();
            this.txbTundraSeedRange = new System.Windows.Forms.TextBox();
            this.txbTundraSpruceEstBA = new System.Windows.Forms.TextBox();
            this.txbTundraFireProb = new System.Windows.Forms.TextBox();
            this.txbTundraStartAge = new System.Windows.Forms.TextBox();
            this.label70 = new System.Windows.Forms.Label();
            this.label99 = new System.Windows.Forms.Label();
            this.pnlWSpruce = new System.Windows.Forms.Panel();
            this.txbWSpruceHumanFireProb = new System.Windows.Forms.TextBox();
            this.label100 = new System.Windows.Forms.Label();
            this.txbWSpruceFireParms = new System.Windows.Forms.TextBox();
            this.txbWSpruceStartAge = new System.Windows.Forms.TextBox();
            this.label91 = new System.Windows.Forms.Label();
            this.label90 = new System.Windows.Forms.Label();
            this.tabFIF.SuspendLayout();
            this.tabGeneral.SuspendLayout();
            this.tabFire.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabClimate.SuspendLayout();
            this.grpClimateTransitions.SuspendLayout();
            this.grpClimOffsets.SuspendLayout();
            this.pnlOffsetsConstant.SuspendLayout();
            this.pnlOffsetsRandom.SuspendLayout();
            this.pnlOffsetsFile.SuspendLayout();
            this.pnlClimRand.SuspendLayout();
            this.pnlClimValues.SuspendLayout();
            this.panel4.SuspendLayout();
            this.panel5.SuspendLayout();
            this.tabStepsAndRamps.SuspendLayout();
            this.tabPageSteps.SuspendLayout();
            this.tabPageRamps.SuspendLayout();
            this.tabMapOutput.SuspendLayout();
            this.tabStatOutput.SuspendLayout();
            this.tabLandscape.SuspendLayout();
            this.pnlSpecies.SuspendLayout();
            this.pnlDecid.SuspendLayout();
            this.pnlBSpruce.SuspendLayout();
            this.pnlTundra.SuspendLayout();
            this.pnlWSpruce.SuspendLayout();
            this.SuspendLayout();
            // 
            // chkIsLargeMemoryModel
            // 
            this.chkIsLargeMemoryModel.Location = new System.Drawing.Point(0, 0);
            this.chkIsLargeMemoryModel.Name = "chkIsLargeMemoryModel";
            this.chkIsLargeMemoryModel.Size = new System.Drawing.Size(104, 24);
            this.chkIsLargeMemoryModel.TabIndex = 0;
            this.chkIsLargeMemoryModel.Text = "Is Large Memory Model";
            // 
            // ToolTips
            // 
            this.ToolTips.AutoPopDelay = 20000;
            this.ToolTips.InitialDelay = 400;
            this.ToolTips.ReshowDelay = 70;
            // 
            // txbGenBaseDirectory
            // 
            this.txbGenBaseDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbGenBaseDirectory.Location = new System.Drawing.Point(124, 19);
            this.txbGenBaseDirectory.Name = "txbGenBaseDirectory";
            this.txbGenBaseDirectory.Size = new System.Drawing.Size(274, 20);
            this.txbGenBaseDirectory.TabIndex = 0;
            this.ToolTips.SetToolTip(this.txbGenBaseDirectory, "111");
            // 
            // fldFileName
            // 
            this.fldFileName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.fldFileName.Location = new System.Drawing.Point(40, 8);
            this.fldFileName.Name = "fldFileName";
            this.fldFileName.Size = new System.Drawing.Size(366, 16);
            this.fldFileName.TabIndex = 2;
            // 
            // lblFileName
            // 
            this.lblFileName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblFileName.Location = new System.Drawing.Point(8, 8);
            this.lblFileName.Name = "lblFileName";
            this.lblFileName.Size = new System.Drawing.Size(32, 16);
            this.lblFileName.TabIndex = 1;
            this.lblFileName.Text = "File: ";
            // 
            // tabFIF
            // 
            this.tabFIF.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabFIF.Controls.Add(this.tabGeneral);
            this.tabFIF.Controls.Add(this.tabFire);
            this.tabFIF.Controls.Add(this.tabClimate);
            this.tabFIF.Controls.Add(this.tabMapOutput);
            this.tabFIF.Controls.Add(this.tabStatOutput);
            this.tabFIF.Controls.Add(this.tabLandscape);
            this.tabFIF.Controls.Add(this.pnlSpecies);
            this.tabFIF.Location = new System.Drawing.Point(4, 24);
            this.tabFIF.Multiline = true;
            this.tabFIF.Name = "tabFIF";
            this.tabFIF.SelectedIndex = 0;
            this.tabFIF.Size = new System.Drawing.Size(415, 680);
            this.tabFIF.TabIndex = 0;
            // 
            // tabGeneral
            // 
            this.tabGeneral.Controls.Add(this.btnOutputDirLookup);
            this.tabGeneral.Controls.Add(this.cbbGenRandSeed);
            this.tabGeneral.Controls.Add(this.txbGenMaxYears);
            this.tabGeneral.Controls.Add(this.btnBaseDirLookup);
            this.tabGeneral.Controls.Add(this.chkGenIsLargeMemoryModel);
            this.tabGeneral.Controls.Add(this.label9);
            this.tabGeneral.Controls.Add(this.cbbGenOutputDetailLevel);
            this.tabGeneral.Controls.Add(this.cbbGenOutputType);
            this.tabGeneral.Controls.Add(this.label8);
            this.tabGeneral.Controls.Add(this.label__);
            this.tabGeneral.Controls.Add(this.txbGenOutputDirectory);
            this.tabGeneral.Controls.Add(this.label7);
            this.tabGeneral.Controls.Add(this.label20);
            this.tabGeneral.Controls.Add(this.label5);
            this.tabGeneral.Controls.Add(this.label4);
            this.tabGeneral.Controls.Add(this.txbGenTimeStep);
            this.tabGeneral.Controls.Add(this.txbGenNumHeader);
            this.tabGeneral.Controls.Add(this.txbGenNoData);
            this.tabGeneral.Controls.Add(this.label3);
            this.tabGeneral.Controls.Add(this.label2);
            this.tabGeneral.Controls.Add(this.label1);
            this.tabGeneral.Controls.Add(this.txbGenMaxReps);
            this.tabGeneral.Controls.Add(this.txbGenBaseDirectory);
            this.tabGeneral.Location = new System.Drawing.Point(4, 22);
            this.tabGeneral.Name = "tabGeneral";
            this.tabGeneral.Size = new System.Drawing.Size(407, 654);
            this.tabGeneral.TabIndex = 0;
            this.tabGeneral.Text = "General";
            // 
            // btnOutputDirLookup
            // 
            this.btnOutputDirLookup.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOutputDirLookup.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnOutputDirLookup.Location = new System.Drawing.Point(542, 182);
            this.btnOutputDirLookup.Name = "btnOutputDirLookup";
            this.btnOutputDirLookup.Size = new System.Drawing.Size(24, 16);
            this.btnOutputDirLookup.TabIndex = 24;
            this.btnOutputDirLookup.Text = "...";
            this.btnOutputDirLookup.Click += new System.EventHandler(this.btnOutputDirLookup_Click);
            // 
            // cbbGenRandSeed
            // 
            this.cbbGenRandSeed.FormattingEnabled = true;
            this.cbbGenRandSeed.Items.AddRange(new object[] {
            "CURRENTTIME"});
            this.cbbGenRandSeed.Location = new System.Drawing.Point(124, 114);
            this.cbbGenRandSeed.Name = "cbbGenRandSeed";
            this.cbbGenRandSeed.Size = new System.Drawing.Size(104, 21);
            this.cbbGenRandSeed.TabIndex = 4;
            // 
            // txbGenMaxYears
            // 
            this.txbGenMaxYears.Location = new System.Drawing.Point(124, 43);
            this.txbGenMaxYears.Name = "txbGenMaxYears";
            this.txbGenMaxYears.Size = new System.Drawing.Size(50, 20);
            this.txbGenMaxYears.TabIndex = 1;
            // 
            // btnBaseDirLookup
            // 
            this.btnBaseDirLookup.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBaseDirLookup.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnBaseDirLookup.Location = new System.Drawing.Point(542, 16);
            this.btnBaseDirLookup.Name = "btnBaseDirLookup";
            this.btnBaseDirLookup.Size = new System.Drawing.Size(24, 16);
            this.btnBaseDirLookup.TabIndex = 1;
            this.btnBaseDirLookup.Text = "...";
            this.btnBaseDirLookup.Click += new System.EventHandler(this.btnBaseDirLookup_Click);
            // 
            // chkGenIsLargeMemoryModel
            // 
            this.chkGenIsLargeMemoryModel.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.chkGenIsLargeMemoryModel.Location = new System.Drawing.Point(5, 243);
            this.chkGenIsLargeMemoryModel.Name = "chkGenIsLargeMemoryModel";
            this.chkGenIsLargeMemoryModel.Size = new System.Drawing.Size(133, 24);
            this.chkGenIsLargeMemoryModel.TabIndex = 9;
            this.chkGenIsLargeMemoryModel.Text = "Large Memory Model";
            // 
            // label9
            // 
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(4, 277);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(112, 16);
            this.label9.TabIndex = 21;
            this.label9.Text = "Console Detail Level";
            // 
            // cbbGenOutputDetailLevel
            // 
            this.cbbGenOutputDetailLevel.FormattingEnabled = true;
            this.cbbGenOutputDetailLevel.Items.AddRange(new object[] {
            "MINIMAL",
            "MODERATE",
            "MAXIMUM"});
            this.cbbGenOutputDetailLevel.Location = new System.Drawing.Point(124, 273);
            this.cbbGenOutputDetailLevel.Name = "cbbGenOutputDetailLevel";
            this.cbbGenOutputDetailLevel.Size = new System.Drawing.Size(104, 21);
            this.cbbGenOutputDetailLevel.TabIndex = 10;
            // 
            // cbbGenOutputType
            // 
            this.cbbGenOutputType.FormattingEnabled = true;
            this.cbbGenOutputType.Items.AddRange(new object[] {
            "APPEND",
            "DELETE",
            "OVERWRITE"});
            this.cbbGenOutputType.Location = new System.Drawing.Point(124, 211);
            this.cbbGenOutputType.Name = "cbbGenOutputType";
            this.cbbGenOutputType.Size = new System.Drawing.Size(104, 21);
            this.cbbGenOutputType.TabIndex = 8;
            // 
            // label8
            // 
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(4, 215);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(112, 16);
            this.label8.TabIndex = 18;
            this.label8.Text = "Output Type";
            // 
            // label__
            // 
            this.label__.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label__.Location = new System.Drawing.Point(4, 191);
            this.label__.Name = "label__";
            this.label__.Size = new System.Drawing.Size(100, 16);
            this.label__.TabIndex = 17;
            this.label__.Text = "Output Directory";
            // 
            // txbGenOutputDirectory
            // 
            this.txbGenOutputDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbGenOutputDirectory.Location = new System.Drawing.Point(124, 187);
            this.txbGenOutputDirectory.Name = "txbGenOutputDirectory";
            this.txbGenOutputDirectory.Size = new System.Drawing.Size(274, 20);
            this.txbGenOutputDirectory.TabIndex = 7;
            // 
            // label7
            // 
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(4, 95);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(88, 16);
            this.label7.TabIndex = 13;
            this.label7.Text = "Time Step";
            // 
            // label20
            // 
            this.label20.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label20.Location = new System.Drawing.Point(4, 119);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(88, 16);
            this.label20.TabIndex = 12;
            this.label20.Text = "Random Seed";
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(4, 143);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(88, 16);
            this.label5.TabIndex = 11;
            this.label5.Text = "Num Header";
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(4, 167);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(88, 16);
            this.label4.TabIndex = 10;
            this.label4.Text = "No Data ID";
            // 
            // txbGenTimeStep
            // 
            this.txbGenTimeStep.Location = new System.Drawing.Point(124, 91);
            this.txbGenTimeStep.Name = "txbGenTimeStep";
            this.txbGenTimeStep.Size = new System.Drawing.Size(50, 20);
            this.txbGenTimeStep.TabIndex = 3;
            // 
            // txbGenNumHeader
            // 
            this.txbGenNumHeader.Location = new System.Drawing.Point(124, 139);
            this.txbGenNumHeader.Name = "txbGenNumHeader";
            this.txbGenNumHeader.Size = new System.Drawing.Size(50, 20);
            this.txbGenNumHeader.TabIndex = 5;
            // 
            // txbGenNoData
            // 
            this.txbGenNoData.Location = new System.Drawing.Point(124, 163);
            this.txbGenNoData.Name = "txbGenNoData";
            this.txbGenNoData.Size = new System.Drawing.Size(50, 20);
            this.txbGenNoData.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(4, 71);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(88, 16);
            this.label3.TabIndex = 5;
            this.label3.Text = "Max Reps";
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(4, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(88, 16);
            this.label2.TabIndex = 4;
            this.label2.Text = "Max Years";
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(4, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 16);
            this.label1.TabIndex = 3;
            this.label1.Text = "Base Directory";
            // 
            // txbGenMaxReps
            // 
            this.txbGenMaxReps.Location = new System.Drawing.Point(124, 67);
            this.txbGenMaxReps.Name = "txbGenMaxReps";
            this.txbGenMaxReps.Size = new System.Drawing.Size(50, 20);
            this.txbGenMaxReps.TabIndex = 2;
            // 
            // tabFire
            // 
            this.tabFire.AutoScroll = true;
            this.tabFire.Controls.Add(this.txbFireMaxEmpiricalFireSizeEvent);
            this.tabFire.Controls.Add(this.txbFireMaxEmpiricalFireSizeEventWeight);
            this.tabFire.Controls.Add(this.groupBox3);
            this.tabFire.Controls.Add(this.groupBox4);
            this.tabFire.Controls.Add(this.groupBox2);
            this.tabFire.Controls.Add(this.groupBox1);
            this.tabFire.Controls.Add(this.chkFireEnabled);
            this.tabFire.Controls.Add(this.label68);
            this.tabFire.Controls.Add(this.txbFireHumanIgnitionFile);
            this.tabFire.Controls.Add(this.label27);
            this.tabFire.Controls.Add(this.label28);
            this.tabFire.Location = new System.Drawing.Point(4, 22);
            this.tabFire.Name = "tabFire";
            this.tabFire.Size = new System.Drawing.Size(407, 654);
            this.tabFire.TabIndex = 3;
            this.tabFire.Text = "Fire";
            // 
            // txbFireMaxEmpiricalFireSizeEvent
            // 
            this.txbFireMaxEmpiricalFireSizeEvent.Location = new System.Drawing.Point(152, 509);
            this.txbFireMaxEmpiricalFireSizeEvent.Name = "txbFireMaxEmpiricalFireSizeEvent";
            this.txbFireMaxEmpiricalFireSizeEvent.Size = new System.Drawing.Size(74, 20);
            this.txbFireMaxEmpiricalFireSizeEvent.TabIndex = 13;
            // 
            // txbFireMaxEmpiricalFireSizeEventWeight
            // 
            this.txbFireMaxEmpiricalFireSizeEventWeight.Location = new System.Drawing.Point(298, 509);
            this.txbFireMaxEmpiricalFireSizeEventWeight.Name = "txbFireMaxEmpiricalFireSizeEventWeight";
            this.txbFireMaxEmpiricalFireSizeEventWeight.Size = new System.Drawing.Size(37, 20);
            this.txbFireMaxEmpiricalFireSizeEventWeight.TabIndex = 12;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.txbFireSpreadStdDev);
            this.groupBox3.Controls.Add(this.txbFireSpreadMean);
            this.groupBox3.Controls.Add(this.txbFireSpreadRadius);
            this.groupBox3.Controls.Add(this.label48);
            this.groupBox3.Controls.Add(this.chkFireIgnoreFirstInterval);
            this.groupBox3.Controls.Add(this.label49);
            this.groupBox3.Controls.Add(this.label50);
            this.groupBox3.Location = new System.Drawing.Point(184, 532);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(216, 114);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Fire Spread";
            // 
            // txbFireSpreadStdDev
            // 
            this.txbFireSpreadStdDev.Location = new System.Drawing.Point(63, 85);
            this.txbFireSpreadStdDev.Name = "txbFireSpreadStdDev";
            this.txbFireSpreadStdDev.Size = new System.Drawing.Size(43, 20);
            this.txbFireSpreadStdDev.TabIndex = 27;
            // 
            // txbFireSpreadMean
            // 
            this.txbFireSpreadMean.Location = new System.Drawing.Point(63, 63);
            this.txbFireSpreadMean.Name = "txbFireSpreadMean";
            this.txbFireSpreadMean.Size = new System.Drawing.Size(43, 20);
            this.txbFireSpreadMean.TabIndex = 26;
            // 
            // txbFireSpreadRadius
            // 
            this.txbFireSpreadRadius.Location = new System.Drawing.Point(63, 39);
            this.txbFireSpreadRadius.Name = "txbFireSpreadRadius";
            this.txbFireSpreadRadius.Size = new System.Drawing.Size(43, 20);
            this.txbFireSpreadRadius.TabIndex = 25;
            // 
            // label48
            // 
            this.label48.Location = new System.Drawing.Point(7, 42);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(47, 16);
            this.label48.TabIndex = 17;
            this.label48.Text = "Radius";
            // 
            // chkFireIgnoreFirstInterval
            // 
            this.chkFireIgnoreFirstInterval.Location = new System.Drawing.Point(63, 14);
            this.chkFireIgnoreFirstInterval.Name = "chkFireIgnoreFirstInterval";
            this.chkFireIgnoreFirstInterval.Size = new System.Drawing.Size(128, 24);
            this.chkFireIgnoreFirstInterval.TabIndex = 24;
            this.chkFireIgnoreFirstInterval.Text = "Ignore First Interval";
            // 
            // label49
            // 
            this.label49.Location = new System.Drawing.Point(7, 91);
            this.label49.Name = "label49";
            this.label49.Size = new System.Drawing.Size(48, 16);
            this.label49.TabIndex = 18;
            this.label49.Text = "Std Dev";
            // 
            // label50
            // 
            this.label50.Location = new System.Drawing.Point(7, 66);
            this.label50.Name = "label50";
            this.label50.Size = new System.Drawing.Size(35, 16);
            this.label50.TabIndex = 19;
            this.label50.Text = "Mean";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.txbFireSuppressionClass2);
            this.groupBox4.Controls.Add(this.chkFireSuppressionOn);
            this.groupBox4.Controls.Add(this.txbFireSuppressionClass1);
            this.groupBox4.Controls.Add(this.label106);
            this.groupBox4.Controls.Add(this.label105);
            this.groupBox4.Controls.Add(this.txbFireSuppressionFile);
            this.groupBox4.Controls.Add(this.label104);
            this.groupBox4.Controls.Add(this.label103);
            this.groupBox4.Controls.Add(this.label97);
            this.groupBox4.Controls.Add(this.label102);
            this.groupBox4.Controls.Add(this.label94);
            this.groupBox4.Controls.Add(this.txbFireSuppressionClass5);
            this.groupBox4.Controls.Add(this.txbFireSuppressionClass4);
            this.groupBox4.Controls.Add(this.txbFireSuppressionClass3);
            this.groupBox4.Controls.Add(this.txbFireSuppressionThresholdIgnitions);
            this.groupBox4.Controls.Add(this.label107);
            this.groupBox4.Controls.Add(this.txbFireSuppressionThresholdFireSize);
            this.groupBox4.Controls.Add(this.label108);
            this.groupBox4.Location = new System.Drawing.Point(8, 368);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(392, 136);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Fire Suppression";
            // 
            // txbFireSuppressionClass2
            // 
            this.txbFireSuppressionClass2.Location = new System.Drawing.Point(185, 60);
            this.txbFireSuppressionClass2.Name = "txbFireSuppressionClass2";
            this.txbFireSuppressionClass2.Size = new System.Drawing.Size(39, 20);
            this.txbFireSuppressionClass2.TabIndex = 15;
            // 
            // chkFireSuppressionOn
            // 
            this.chkFireSuppressionOn.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkFireSuppressionOn.Location = new System.Drawing.Point(132, 16);
            this.chkFireSuppressionOn.Name = "chkFireSuppressionOn";
            this.chkFireSuppressionOn.Size = new System.Drawing.Size(164, 16);
            this.chkFireSuppressionOn.TabIndex = 12;
            this.chkFireSuppressionOn.Text = "Apply Suppression Maps";
            // 
            // txbFireSuppressionClass1
            // 
            this.txbFireSuppressionClass1.Location = new System.Drawing.Point(132, 60);
            this.txbFireSuppressionClass1.Name = "txbFireSuppressionClass1";
            this.txbFireSuppressionClass1.Size = new System.Drawing.Size(39, 20);
            this.txbFireSuppressionClass1.TabIndex = 14;
            // 
            // label106
            // 
            this.label106.Location = new System.Drawing.Point(333, 64);
            this.label106.Name = "label106";
            this.label106.Size = new System.Drawing.Size(12, 16);
            this.label106.TabIndex = 55;
            this.label106.Text = "5";
            // 
            // label105
            // 
            this.label105.Location = new System.Drawing.Point(280, 64);
            this.label105.Name = "label105";
            this.label105.Size = new System.Drawing.Size(12, 16);
            this.label105.TabIndex = 54;
            this.label105.Text = "4";
            // 
            // txbFireSuppressionFile
            // 
            this.txbFireSuppressionFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbFireSuppressionFile.Location = new System.Drawing.Point(132, 36);
            this.txbFireSuppressionFile.Name = "txbFireSuppressionFile";
            this.txbFireSuppressionFile.Size = new System.Drawing.Size(252, 20);
            this.txbFireSuppressionFile.TabIndex = 13;
            // 
            // label104
            // 
            this.label104.Location = new System.Drawing.Point(227, 64);
            this.label104.Name = "label104";
            this.label104.Size = new System.Drawing.Size(12, 16);
            this.label104.TabIndex = 53;
            this.label104.Text = "3";
            // 
            // label103
            // 
            this.label103.Location = new System.Drawing.Point(173, 64);
            this.label103.Name = "label103";
            this.label103.Size = new System.Drawing.Size(12, 16);
            this.label103.TabIndex = 52;
            this.label103.Text = "2";
            // 
            // label97
            // 
            this.label97.Location = new System.Drawing.Point(4, 40);
            this.label97.Name = "label97";
            this.label97.Size = new System.Drawing.Size(112, 16);
            this.label97.TabIndex = 43;
            this.label97.Text = "Fire Suppression File";
            // 
            // label102
            // 
            this.label102.Location = new System.Drawing.Point(121, 64);
            this.label102.Name = "label102";
            this.label102.Size = new System.Drawing.Size(12, 16);
            this.label102.TabIndex = 51;
            this.label102.Text = "1";
            // 
            // label94
            // 
            this.label94.Location = new System.Drawing.Point(4, 64);
            this.label94.Name = "label94";
            this.label94.Size = new System.Drawing.Size(116, 16);
            this.label94.TabIndex = 50;
            this.label94.Text = "Suppression Classes:";
            // 
            // txbFireSuppressionClass5
            // 
            this.txbFireSuppressionClass5.Location = new System.Drawing.Point(345, 60);
            this.txbFireSuppressionClass5.Name = "txbFireSuppressionClass5";
            this.txbFireSuppressionClass5.Size = new System.Drawing.Size(39, 20);
            this.txbFireSuppressionClass5.TabIndex = 18;
            // 
            // txbFireSuppressionClass4
            // 
            this.txbFireSuppressionClass4.Location = new System.Drawing.Point(292, 60);
            this.txbFireSuppressionClass4.Name = "txbFireSuppressionClass4";
            this.txbFireSuppressionClass4.Size = new System.Drawing.Size(39, 20);
            this.txbFireSuppressionClass4.TabIndex = 17;
            // 
            // txbFireSuppressionClass3
            // 
            this.txbFireSuppressionClass3.Location = new System.Drawing.Point(239, 60);
            this.txbFireSuppressionClass3.Name = "txbFireSuppressionClass3";
            this.txbFireSuppressionClass3.Size = new System.Drawing.Size(39, 20);
            this.txbFireSuppressionClass3.TabIndex = 16;
            // 
            // txbFireSuppressionThresholdIgnitions
            // 
            this.txbFireSuppressionThresholdIgnitions.Location = new System.Drawing.Point(132, 108);
            this.txbFireSuppressionThresholdIgnitions.Name = "txbFireSuppressionThresholdIgnitions";
            this.txbFireSuppressionThresholdIgnitions.Size = new System.Drawing.Size(39, 20);
            this.txbFireSuppressionThresholdIgnitions.TabIndex = 20;
            // 
            // label107
            // 
            this.label107.Location = new System.Drawing.Point(4, 88);
            this.label107.Name = "label107";
            this.label107.Size = new System.Drawing.Size(112, 16);
            this.label107.TabIndex = 57;
            this.label107.Text = "Fire Size Threshold";
            // 
            // txbFireSuppressionThresholdFireSize
            // 
            this.txbFireSuppressionThresholdFireSize.Location = new System.Drawing.Point(132, 84);
            this.txbFireSuppressionThresholdFireSize.Name = "txbFireSuppressionThresholdFireSize";
            this.txbFireSuppressionThresholdFireSize.Size = new System.Drawing.Size(39, 20);
            this.txbFireSuppressionThresholdFireSize.TabIndex = 119;
            // 
            // label108
            // 
            this.label108.Location = new System.Drawing.Point(4, 112);
            this.label108.Name = "label108";
            this.label108.Size = new System.Drawing.Size(136, 16);
            this.label108.TabIndex = 59;
            this.label108.Text = "Ignition Count Threshold";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label42);
            this.groupBox2.Controls.Add(this.btnFireTransitionRemove);
            this.groupBox2.Controls.Add(this.txbFireTypeTransitionYear);
            this.groupBox2.Controls.Add(this.label95);
            this.groupBox2.Controls.Add(this.btnFireTransitionAdd);
            this.groupBox2.Controls.Add(this.lstFireTypeTransitions);
            this.groupBox2.Controls.Add(this.label60);
            this.groupBox2.Controls.Add(this.label63);
            this.groupBox2.Controls.Add(this.label52);
            this.groupBox2.Controls.Add(this.txbFireIgnition);
            this.groupBox2.Controls.Add(this.txbFireSpatialSensitivity);
            this.groupBox2.Controls.Add(this.txbFireSensitivity);
            this.groupBox2.Controls.Add(this.txbFireSpatialIgnition);
            this.groupBox2.Controls.Add(this.label47);
            this.groupBox2.Controls.Add(this.label46);
            this.groupBox2.Controls.Add(this.label44);
            this.groupBox2.Controls.Add(this.label43);
            this.groupBox2.Controls.Add(this.cbbFireType);
            this.groupBox2.Controls.Add(this.label45);
            this.groupBox2.Controls.Add(this.txbFireHistorical);
            this.groupBox2.Location = new System.Drawing.Point(8, 34);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(392, 300);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Fire Transitions";
            // 
            // label42
            // 
            this.label42.Location = new System.Drawing.Point(4, 156);
            this.label42.Name = "label42";
            this.label42.Size = new System.Drawing.Size(72, 16);
            this.label42.TabIndex = 37;
            this.label42.Text = "Fire Type";
            // 
            // btnFireTransitionRemove
            // 
            this.btnFireTransitionRemove.Location = new System.Drawing.Point(67, 99);
            this.btnFireTransitionRemove.Name = "btnFireTransitionRemove";
            this.btnFireTransitionRemove.Size = new System.Drawing.Size(61, 20);
            this.btnFireTransitionRemove.TabIndex = 3;
            this.btnFireTransitionRemove.Text = "Remove";
            this.btnFireTransitionRemove.Click += new System.EventHandler(this.btnFireTransitionRemove_Click);
            // 
            // txbFireTypeTransitionYear
            // 
            this.txbFireTypeTransitionYear.Location = new System.Drawing.Point(128, 129);
            this.txbFireTypeTransitionYear.Name = "txbFireTypeTransitionYear";
            this.txbFireTypeTransitionYear.Size = new System.Drawing.Size(43, 20);
            this.txbFireTypeTransitionYear.TabIndex = 4;
            this.txbFireTypeTransitionYear.TextChanged += new System.EventHandler(this.OnFireTransitionChanged);
            // 
            // label95
            // 
            this.label95.Location = new System.Drawing.Point(4, 133);
            this.label95.Name = "label95";
            this.label95.Size = new System.Drawing.Size(84, 16);
            this.label95.TabIndex = 34;
            this.label95.Text = "Transition Year";
            // 
            // btnFireTransitionAdd
            // 
            this.btnFireTransitionAdd.Location = new System.Drawing.Point(8, 99);
            this.btnFireTransitionAdd.Name = "btnFireTransitionAdd";
            this.btnFireTransitionAdd.Size = new System.Drawing.Size(55, 20);
            this.btnFireTransitionAdd.TabIndex = 2;
            this.btnFireTransitionAdd.Text = "Add";
            this.btnFireTransitionAdd.Click += new System.EventHandler(this.btnFireTransitionAdd_Click);
            // 
            // lstFireTypeTransitions
            // 
            this.lstFireTypeTransitions.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lstFireTypeTransitions.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lstFireTypeTransitions.FormattingEnabled = true;
            this.lstFireTypeTransitions.HorizontalScrollbar = true;
            this.lstFireTypeTransitions.ItemHeight = 14;
            this.lstFireTypeTransitions.Location = new System.Drawing.Point(8, 35);
            this.lstFireTypeTransitions.Name = "lstFireTypeTransitions";
            this.lstFireTypeTransitions.Size = new System.Drawing.Size(376, 60);
            this.lstFireTypeTransitions.TabIndex = 1;
            this.lstFireTypeTransitions.SelectedIndexChanged += new System.EventHandler(this.lstFireTransitions_SelectedIndexChanged);
            // 
            // label60
            // 
            this.label60.Location = new System.Drawing.Point(44, 19);
            this.label60.Name = "label60";
            this.label60.Size = new System.Drawing.Size(52, 16);
            this.label60.TabIndex = 32;
            this.label60.Text = "Fire Type";
            // 
            // label63
            // 
            this.label63.Location = new System.Drawing.Point(124, 19);
            this.label63.Name = "label63";
            this.label63.Size = new System.Drawing.Size(164, 16);
            this.label63.TabIndex = 33;
            this.label63.Text = "Value Descriptions";
            // 
            // label52
            // 
            this.label52.Location = new System.Drawing.Point(8, 19);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(32, 16);
            this.label52.TabIndex = 31;
            this.label52.Text = "Year";
            // 
            // txbFireIgnition
            // 
            this.txbFireIgnition.AcceptsTab = true;
            this.txbFireIgnition.Location = new System.Drawing.Point(128, 176);
            this.txbFireIgnition.Name = "txbFireIgnition";
            this.txbFireIgnition.Size = new System.Drawing.Size(100, 20);
            this.txbFireIgnition.TabIndex = 6;
            this.txbFireIgnition.TextChanged += new System.EventHandler(this.OnFireTransitionChanged);
            // 
            // txbFireSpatialSensitivity
            // 
            this.txbFireSpatialSensitivity.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbFireSpatialSensitivity.Location = new System.Drawing.Point(128, 247);
            this.txbFireSpatialSensitivity.Name = "txbFireSpatialSensitivity";
            this.txbFireSpatialSensitivity.Size = new System.Drawing.Size(256, 20);
            this.txbFireSpatialSensitivity.TabIndex = 9;
            this.txbFireSpatialSensitivity.TextChanged += new System.EventHandler(this.OnFireTransitionChanged);
            // 
            // txbFireSensitivity
            // 
            this.txbFireSensitivity.Location = new System.Drawing.Point(128, 199);
            this.txbFireSensitivity.Name = "txbFireSensitivity";
            this.txbFireSensitivity.Size = new System.Drawing.Size(68, 20);
            this.txbFireSensitivity.TabIndex = 7;
            this.txbFireSensitivity.TextChanged += new System.EventHandler(this.OnFireTransitionChanged);
            // 
            // txbFireSpatialIgnition
            // 
            this.txbFireSpatialIgnition.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbFireSpatialIgnition.Location = new System.Drawing.Point(128, 223);
            this.txbFireSpatialIgnition.Name = "txbFireSpatialIgnition";
            this.txbFireSpatialIgnition.Size = new System.Drawing.Size(256, 20);
            this.txbFireSpatialIgnition.TabIndex = 8;
            this.txbFireSpatialIgnition.TextChanged += new System.EventHandler(this.OnFireTransitionChanged);
            // 
            // label47
            // 
            this.label47.Location = new System.Drawing.Point(4, 180);
            this.label47.Name = "label47";
            this.label47.Size = new System.Drawing.Size(100, 16);
            this.label47.TabIndex = 16;
            this.label47.Text = "Ignition Factor";
            // 
            // label46
            // 
            this.label46.Location = new System.Drawing.Point(4, 203);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(100, 16);
            this.label46.TabIndex = 15;
            this.label46.Text = "Sensitivity";
            // 
            // label44
            // 
            this.label44.Location = new System.Drawing.Point(4, 251);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(100, 16);
            this.label44.TabIndex = 13;
            this.label44.Text = "Spatial Sensitivity";
            // 
            // label43
            // 
            this.label43.Location = new System.Drawing.Point(4, 227);
            this.label43.Name = "label43";
            this.label43.Size = new System.Drawing.Size(116, 16);
            this.label43.TabIndex = 12;
            this.label43.Text = "Spatial Ignition Factor";
            // 
            // cbbFireType
            // 
            this.cbbFireType.FormattingEnabled = true;
            this.cbbFireType.Items.AddRange(new object[] {
            "",
            "FIXED",
            "SPATIAL",
            "HISTORICAL"});
            this.cbbFireType.Location = new System.Drawing.Point(128, 152);
            this.cbbFireType.Name = "cbbFireType";
            this.cbbFireType.Size = new System.Drawing.Size(104, 21);
            this.cbbFireType.TabIndex = 5;
            this.cbbFireType.SelectionChangeCommitted += new System.EventHandler(this.cbbFire_SelectionChangeCommitted);
            // 
            // label45
            // 
            this.label45.Location = new System.Drawing.Point(4, 275);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(100, 16);
            this.label45.TabIndex = 14;
            this.label45.Text = "Historical File";
            // 
            // txbFireHistorical
            // 
            this.txbFireHistorical.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbFireHistorical.Location = new System.Drawing.Point(128, 271);
            this.txbFireHistorical.Name = "txbFireHistorical";
            this.txbFireHistorical.Size = new System.Drawing.Size(256, 20);
            this.txbFireHistorical.TabIndex = 10;
            this.txbFireHistorical.TextChanged += new System.EventHandler(this.OnFireTransitionChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.txbFireClimateC);
            this.groupBox1.Controls.Add(this.txbFireClimateB);
            this.groupBox1.Controls.Add(this.txbFireClimateA);
            this.groupBox1.Controls.Add(this.label56);
            this.groupBox1.Controls.Add(this.label55);
            this.groupBox1.Controls.Add(this.label54);
            this.groupBox1.Controls.Add(this.label51);
            this.groupBox1.Location = new System.Drawing.Point(8, 532);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(171, 114);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Fire Climate Equation";
            // 
            // txbFireClimateC
            // 
            this.txbFireClimateC.Location = new System.Drawing.Point(44, 86);
            this.txbFireClimateC.Name = "txbFireClimateC";
            this.txbFireClimateC.Size = new System.Drawing.Size(60, 20);
            this.txbFireClimateC.TabIndex = 23;
            // 
            // txbFireClimateB
            // 
            this.txbFireClimateB.Location = new System.Drawing.Point(44, 63);
            this.txbFireClimateB.Name = "txbFireClimateB";
            this.txbFireClimateB.Size = new System.Drawing.Size(60, 20);
            this.txbFireClimateB.TabIndex = 22;
            // 
            // txbFireClimateA
            // 
            this.txbFireClimateA.Location = new System.Drawing.Point(44, 39);
            this.txbFireClimateA.Name = "txbFireClimateA";
            this.txbFireClimateA.Size = new System.Drawing.Size(60, 20);
            this.txbFireClimateA.TabIndex = 21;
            // 
            // label56
            // 
            this.label56.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label56.Location = new System.Drawing.Point(24, 67);
            this.label56.Name = "label56";
            this.label56.Size = new System.Drawing.Size(24, 16);
            this.label56.TabIndex = 5;
            this.label56.Text = "b = ";
            // 
            // label55
            // 
            this.label55.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label55.Location = new System.Drawing.Point(24, 92);
            this.label55.Name = "label55";
            this.label55.Size = new System.Drawing.Size(24, 16);
            this.label55.TabIndex = 4;
            this.label55.Text = "c = ";
            // 
            // label54
            // 
            this.label54.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label54.Location = new System.Drawing.Point(24, 43);
            this.label54.Name = "label54";
            this.label54.Size = new System.Drawing.Size(28, 16);
            this.label54.TabIndex = 3;
            this.label54.Text = "a = ";
            // 
            // label51
            // 
            this.label51.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label51.Location = new System.Drawing.Point(5, 20);
            this.label51.Name = "label51";
            this.label51.Size = new System.Drawing.Size(163, 16);
            this.label51.TabIndex = 0;
            this.label51.Text = "Prob = a + b(Temp) + c(Precip)";
            // 
            // chkFireEnabled
            // 
            this.chkFireEnabled.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkFireEnabled.Location = new System.Drawing.Point(19, 9);
            this.chkFireEnabled.Name = "chkFireEnabled";
            this.chkFireEnabled.Size = new System.Drawing.Size(104, 24);
            this.chkFireEnabled.TabIndex = 0;
            this.chkFireEnabled.Text = "Enable Fires";
            // 
            // label68
            // 
            this.label68.Location = new System.Drawing.Point(12, 348);
            this.label68.Name = "label68";
            this.label68.Size = new System.Drawing.Size(104, 16);
            this.label68.TabIndex = 2;
            this.label68.Text = "Human Ignition File";
            // 
            // txbFireHumanIgnitionFile
            // 
            this.txbFireHumanIgnitionFile.Location = new System.Drawing.Point(136, 344);
            this.txbFireHumanIgnitionFile.Name = "txbFireHumanIgnitionFile";
            this.txbFireHumanIgnitionFile.Size = new System.Drawing.Size(256, 20);
            this.txbFireHumanIgnitionFile.TabIndex = 11;
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(5, 512);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(146, 13);
            this.label27.TabIndex = 14;
            this.label27.Text = "Max Empirical Fire Size Event";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(255, 512);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(41, 13);
            this.label28.TabIndex = 15;
            this.label28.Text = "Weight";
            // 
            // tabClimate
            // 
            this.tabClimate.AutoScroll = true;
            this.tabClimate.Controls.Add(this.grpClimateTransitions);
            this.tabClimate.Controls.Add(this.tabStepsAndRamps);
            this.tabClimate.Controls.Add(this.chkStepsAndRampsEnabled);
            this.tabClimate.Controls.Add(this.txbClimNumHistory);
            this.tabClimate.Controls.Add(this.label38);
            this.tabClimate.Location = new System.Drawing.Point(4, 22);
            this.tabClimate.Name = "tabClimate";
            this.tabClimate.Size = new System.Drawing.Size(407, 654);
            this.tabClimate.TabIndex = 2;
            this.tabClimate.Text = "Climate";
            // 
            // grpClimateTransitions
            // 
            this.grpClimateTransitions.Controls.Add(this.txbClimTransitionYear);
            this.grpClimateTransitions.Controls.Add(this.label125);
            this.grpClimateTransitions.Controls.Add(this.cbbClimValuesType);
            this.grpClimateTransitions.Controls.Add(this.lstClimTransitions);
            this.grpClimateTransitions.Controls.Add(this.label128);
            this.grpClimateTransitions.Controls.Add(this.label39);
            this.grpClimateTransitions.Controls.Add(this.grpClimOffsets);
            this.grpClimateTransitions.Controls.Add(this.label36);
            this.grpClimateTransitions.Controls.Add(this.label40);
            this.grpClimateTransitions.Controls.Add(this.pnlClimRand);
            this.grpClimateTransitions.Controls.Add(this.button3);
            this.grpClimateTransitions.Controls.Add(this.pnlClimValues);
            this.grpClimateTransitions.Controls.Add(this.button2);
            this.grpClimateTransitions.Controls.Add(this.panel4);
            this.grpClimateTransitions.Controls.Add(this.btnClimTransitionRemove);
            this.grpClimateTransitions.Controls.Add(this.panel5);
            this.grpClimateTransitions.Controls.Add(this.label126);
            this.grpClimateTransitions.Controls.Add(this.btnClimTransitionAdd);
            this.grpClimateTransitions.Controls.Add(this.label127);
            this.grpClimateTransitions.Location = new System.Drawing.Point(6, 32);
            this.grpClimateTransitions.Name = "grpClimateTransitions";
            this.grpClimateTransitions.Size = new System.Drawing.Size(396, 440);
            this.grpClimateTransitions.TabIndex = 1;
            this.grpClimateTransitions.TabStop = false;
            this.grpClimateTransitions.Text = "Climate Transitions";
            // 
            // txbClimTransitionYear
            // 
            this.txbClimTransitionYear.Location = new System.Drawing.Point(95, 121);
            this.txbClimTransitionYear.Name = "txbClimTransitionYear";
            this.txbClimTransitionYear.Size = new System.Drawing.Size(47, 20);
            this.txbClimTransitionYear.TabIndex = 4;
            this.txbClimTransitionYear.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // label125
            // 
            this.label125.Location = new System.Drawing.Point(6, 17);
            this.label125.Name = "label125";
            this.label125.Size = new System.Drawing.Size(32, 14);
            this.label125.TabIndex = 52;
            this.label125.Text = "Year";
            // 
            // cbbClimValuesType
            // 
            this.cbbClimValuesType.FormattingEnabled = true;
            this.cbbClimValuesType.Items.AddRange(new object[] {
            "CONSTANT",
            "SPATIAL",
            "EXPLICIT",
            "RANDEXPLICIT"});
            this.cbbClimValuesType.Location = new System.Drawing.Point(95, 144);
            this.cbbClimValuesType.Name = "cbbClimValuesType";
            this.cbbClimValuesType.Size = new System.Drawing.Size(112, 21);
            this.cbbClimValuesType.TabIndex = 5;
            this.cbbClimValuesType.SelectionChangeCommitted += new System.EventHandler(this.cbbClimValuesType_SelectionChangeCommited);
            // 
            // lstClimTransitions
            // 
            this.lstClimTransitions.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lstClimTransitions.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lstClimTransitions.FormattingEnabled = true;
            this.lstClimTransitions.HorizontalScrollbar = true;
            this.lstClimTransitions.ItemHeight = 14;
            this.lstClimTransitions.Location = new System.Drawing.Point(5, 32);
            this.lstClimTransitions.Name = "lstClimTransitions";
            this.lstClimTransitions.Size = new System.Drawing.Size(385, 60);
            this.lstClimTransitions.TabIndex = 1;
            this.lstClimTransitions.SelectedIndexChanged += new System.EventHandler(this.lstClimTransitions_SelectedIndexChanged);
            // 
            // label128
            // 
            this.label128.Location = new System.Drawing.Point(12, 124);
            this.label128.Name = "label128";
            this.label128.Size = new System.Drawing.Size(82, 16);
            this.label128.TabIndex = 58;
            this.label128.Text = "Transition Year";
            // 
            // label39
            // 
            this.label39.Location = new System.Drawing.Point(11, 147);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(71, 16);
            this.label39.TabIndex = 31;
            this.label39.Text = "Climate Type";
            // 
            // grpClimOffsets
            // 
            this.grpClimOffsets.Controls.Add(this.pnlOffsetsConstant);
            this.grpClimOffsets.Controls.Add(this.pnlOffsetsRandom);
            this.grpClimOffsets.Controls.Add(this.cbbClimOffsetsType);
            this.grpClimOffsets.Controls.Add(this.label35);
            this.grpClimOffsets.Controls.Add(this.pnlOffsetsFile);
            this.grpClimOffsets.Location = new System.Drawing.Point(7, 262);
            this.grpClimOffsets.Name = "grpClimOffsets";
            this.grpClimOffsets.Size = new System.Drawing.Size(383, 173);
            this.grpClimOffsets.TabIndex = 13;
            this.grpClimOffsets.TabStop = false;
            this.grpClimOffsets.Text = "Offsets";
            // 
            // pnlOffsetsConstant
            // 
            this.pnlOffsetsConstant.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlOffsetsConstant.Controls.Add(this.label113);
            this.pnlOffsetsConstant.Controls.Add(this.label112);
            this.pnlOffsetsConstant.Controls.Add(this.txbClimOffsetsPrecipConstant);
            this.pnlOffsetsConstant.Controls.Add(this.txbClimOffsetsTempConstant);
            this.pnlOffsetsConstant.Location = new System.Drawing.Point(5, 61);
            this.pnlOffsetsConstant.Name = "pnlOffsetsConstant";
            this.pnlOffsetsConstant.Size = new System.Drawing.Size(375, 47);
            this.pnlOffsetsConstant.TabIndex = 17;
            // 
            // label113
            // 
            this.label113.AutoSize = true;
            this.label113.Location = new System.Drawing.Point(0, 26);
            this.label113.Name = "label113";
            this.label113.Size = new System.Drawing.Size(82, 13);
            this.label113.TabIndex = 31;
            this.label113.Text = "Constant Precip";
            // 
            // label112
            // 
            this.label112.AutoSize = true;
            this.label112.Location = new System.Drawing.Point(0, 5);
            this.label112.Name = "label112";
            this.label112.Size = new System.Drawing.Size(79, 13);
            this.label112.TabIndex = 30;
            this.label112.Text = "Constant Temp";
            // 
            // txbClimOffsetsPrecipConstant
            // 
            this.txbClimOffsetsPrecipConstant.Location = new System.Drawing.Point(83, 23);
            this.txbClimOffsetsPrecipConstant.Name = "txbClimOffsetsPrecipConstant";
            this.txbClimOffsetsPrecipConstant.Size = new System.Drawing.Size(47, 20);
            this.txbClimOffsetsPrecipConstant.TabIndex = 19;
            this.txbClimOffsetsPrecipConstant.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // txbClimOffsetsTempConstant
            // 
            this.txbClimOffsetsTempConstant.Location = new System.Drawing.Point(83, 2);
            this.txbClimOffsetsTempConstant.Name = "txbClimOffsetsTempConstant";
            this.txbClimOffsetsTempConstant.Size = new System.Drawing.Size(47, 20);
            this.txbClimOffsetsTempConstant.TabIndex = 18;
            this.txbClimOffsetsTempConstant.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // pnlOffsetsRandom
            // 
            this.pnlOffsetsRandom.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlOffsetsRandom.Controls.Add(this.chkClimOffsetsRandIsReplicated);
            this.pnlOffsetsRandom.Controls.Add(this.label26);
            this.pnlOffsetsRandom.Controls.Add(this.label25);
            this.pnlOffsetsRandom.Controls.Add(this.txbClimOffsetsPrecipStdDev);
            this.pnlOffsetsRandom.Controls.Add(this.txbClimOffsetsPrecipMean);
            this.pnlOffsetsRandom.Controls.Add(this.label30);
            this.pnlOffsetsRandom.Controls.Add(this.txbClimOffsetsTempStdDev);
            this.pnlOffsetsRandom.Controls.Add(this.label29);
            this.pnlOffsetsRandom.Controls.Add(this.txbClimOffsetsTempMean);
            this.pnlOffsetsRandom.Location = new System.Drawing.Point(5, 108);
            this.pnlOffsetsRandom.Name = "pnlOffsetsRandom";
            this.pnlOffsetsRandom.Size = new System.Drawing.Size(375, 59);
            this.pnlOffsetsRandom.TabIndex = 20;
            // 
            // chkClimOffsetsRandIsReplicated
            // 
            this.chkClimOffsetsRandIsReplicated.AutoSize = true;
            this.chkClimOffsetsRandIsReplicated.Location = new System.Drawing.Point(198, 29);
            this.chkClimOffsetsRandIsReplicated.Name = "chkClimOffsetsRandIsReplicated";
            this.chkClimOffsetsRandIsReplicated.Size = new System.Drawing.Size(71, 17);
            this.chkClimOffsetsRandIsReplicated.TabIndex = 25;
            this.chkClimOffsetsRandIsReplicated.Text = "Replicate";
            this.chkClimOffsetsRandIsReplicated.UseVisualStyleBackColor = true;
            this.chkClimOffsetsRandIsReplicated.CheckedChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // label26
            // 
            this.label26.Location = new System.Drawing.Point(0, 40);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(80, 16);
            this.label26.TabIndex = 40;
            this.label26.Text = "Random Precip";
            // 
            // label25
            // 
            this.label25.Location = new System.Drawing.Point(0, 19);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(80, 16);
            this.label25.TabIndex = 39;
            this.label25.Text = "Random Temp";
            // 
            // txbClimOffsetsPrecipStdDev
            // 
            this.txbClimOffsetsPrecipStdDev.Location = new System.Drawing.Point(136, 37);
            this.txbClimOffsetsPrecipStdDev.Name = "txbClimOffsetsPrecipStdDev";
            this.txbClimOffsetsPrecipStdDev.Size = new System.Drawing.Size(51, 20);
            this.txbClimOffsetsPrecipStdDev.TabIndex = 24;
            this.txbClimOffsetsPrecipStdDev.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // txbClimOffsetsPrecipMean
            // 
            this.txbClimOffsetsPrecipMean.Location = new System.Drawing.Point(83, 37);
            this.txbClimOffsetsPrecipMean.Name = "txbClimOffsetsPrecipMean";
            this.txbClimOffsetsPrecipMean.Size = new System.Drawing.Size(51, 20);
            this.txbClimOffsetsPrecipMean.TabIndex = 23;
            this.txbClimOffsetsPrecipMean.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // label30
            // 
            this.label30.Location = new System.Drawing.Point(138, 2);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(49, 12);
            this.label30.TabIndex = 14;
            this.label30.Text = "Std Dev";
            // 
            // txbClimOffsetsTempStdDev
            // 
            this.txbClimOffsetsTempStdDev.Location = new System.Drawing.Point(136, 15);
            this.txbClimOffsetsTempStdDev.Name = "txbClimOffsetsTempStdDev";
            this.txbClimOffsetsTempStdDev.Size = new System.Drawing.Size(51, 20);
            this.txbClimOffsetsTempStdDev.TabIndex = 22;
            this.txbClimOffsetsTempStdDev.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // label29
            // 
            this.label29.Location = new System.Drawing.Point(84, 2);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(50, 12);
            this.label29.TabIndex = 15;
            this.label29.Text = "Mean";
            // 
            // txbClimOffsetsTempMean
            // 
            this.txbClimOffsetsTempMean.Location = new System.Drawing.Point(83, 15);
            this.txbClimOffsetsTempMean.Name = "txbClimOffsetsTempMean";
            this.txbClimOffsetsTempMean.Size = new System.Drawing.Size(51, 20);
            this.txbClimOffsetsTempMean.TabIndex = 21;
            this.txbClimOffsetsTempMean.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // cbbClimOffsetsType
            // 
            this.cbbClimOffsetsType.FormattingEnabled = true;
            this.cbbClimOffsetsType.Items.AddRange(new object[] {
            "NONE",
            "CONSTANT",
            "FILE",
            "RANDOM"});
            this.cbbClimOffsetsType.Location = new System.Drawing.Point(88, 14);
            this.cbbClimOffsetsType.Name = "cbbClimOffsetsType";
            this.cbbClimOffsetsType.Size = new System.Drawing.Size(121, 21);
            this.cbbClimOffsetsType.TabIndex = 14;
            this.cbbClimOffsetsType.SelectionChangeCommitted += new System.EventHandler(this.cbbClimOffsetsType_SelectionChangeCommited);
            // 
            // label35
            // 
            this.label35.Location = new System.Drawing.Point(5, 17);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(79, 16);
            this.label35.TabIndex = 24;
            this.label35.Text = "Offset Type";
            // 
            // pnlOffsetsFile
            // 
            this.pnlOffsetsFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlOffsetsFile.Controls.Add(this.label111);
            this.pnlOffsetsFile.Controls.Add(this.txbClimOffsetsFile);
            this.pnlOffsetsFile.Location = new System.Drawing.Point(5, 36);
            this.pnlOffsetsFile.Name = "pnlOffsetsFile";
            this.pnlOffsetsFile.Size = new System.Drawing.Size(375, 25);
            this.pnlOffsetsFile.TabIndex = 15;
            // 
            // label111
            // 
            this.label111.Location = new System.Drawing.Point(0, 6);
            this.label111.Name = "label111";
            this.label111.Size = new System.Drawing.Size(61, 16);
            this.label111.TabIndex = 40;
            this.label111.Text = "Offsets File";
            // 
            // txbClimOffsetsFile
            // 
            this.txbClimOffsetsFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbClimOffsetsFile.Location = new System.Drawing.Point(83, 2);
            this.txbClimOffsetsFile.Name = "txbClimOffsetsFile";
            this.txbClimOffsetsFile.Size = new System.Drawing.Size(287, 20);
            this.txbClimOffsetsFile.TabIndex = 16;
            this.txbClimOffsetsFile.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // label36
            // 
            this.label36.Location = new System.Drawing.Point(11, 147);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(71, 16);
            this.label36.TabIndex = 31;
            this.label36.Text = "Climate Type";
            // 
            // label40
            // 
            this.label40.Location = new System.Drawing.Point(12, 125);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(84, 16);
            this.label40.TabIndex = 58;
            this.label40.Text = "Transition Year";
            // 
            // pnlClimRand
            // 
            this.pnlClimRand.Controls.Add(this.label6);
            this.pnlClimRand.Controls.Add(this.chkClimValuesRandExplicitIsReplicated);
            this.pnlClimRand.Controls.Add(this.txbClimRandExplicitMinYear);
            this.pnlClimRand.Controls.Add(this.txbClimRandExplicitMaxYear);
            this.pnlClimRand.Controls.Add(this.label110);
            this.pnlClimRand.Location = new System.Drawing.Point(7, 211);
            this.pnlClimRand.Name = "pnlClimRand";
            this.pnlClimRand.Size = new System.Drawing.Size(380, 48);
            this.pnlClimRand.TabIndex = 9;
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(4, 5);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(82, 16);
            this.label6.TabIndex = 36;
            this.label6.Text = "Min Rand Year";
            // 
            // chkClimValuesRandExplicitIsReplicated
            // 
            this.chkClimValuesRandExplicitIsReplicated.AutoSize = true;
            this.chkClimValuesRandExplicitIsReplicated.Location = new System.Drawing.Point(143, 15);
            this.chkClimValuesRandExplicitIsReplicated.Name = "chkClimValuesRandExplicitIsReplicated";
            this.chkClimValuesRandExplicitIsReplicated.Size = new System.Drawing.Size(71, 17);
            this.chkClimValuesRandExplicitIsReplicated.TabIndex = 12;
            this.chkClimValuesRandExplicitIsReplicated.Text = "Replicate";
            this.chkClimValuesRandExplicitIsReplicated.UseVisualStyleBackColor = true;
            this.chkClimValuesRandExplicitIsReplicated.CheckedChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // txbClimRandExplicitMinYear
            // 
            this.txbClimRandExplicitMinYear.Location = new System.Drawing.Point(88, 1);
            this.txbClimRandExplicitMinYear.Name = "txbClimRandExplicitMinYear";
            this.txbClimRandExplicitMinYear.Size = new System.Drawing.Size(47, 20);
            this.txbClimRandExplicitMinYear.TabIndex = 10;
            this.txbClimRandExplicitMinYear.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // txbClimRandExplicitMaxYear
            // 
            this.txbClimRandExplicitMaxYear.Location = new System.Drawing.Point(88, 22);
            this.txbClimRandExplicitMaxYear.Name = "txbClimRandExplicitMaxYear";
            this.txbClimRandExplicitMaxYear.Size = new System.Drawing.Size(47, 20);
            this.txbClimRandExplicitMaxYear.TabIndex = 11;
            this.txbClimRandExplicitMaxYear.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // label110
            // 
            this.label110.Location = new System.Drawing.Point(4, 26);
            this.label110.Name = "label110";
            this.label110.Size = new System.Drawing.Size(82, 16);
            this.label110.TabIndex = 37;
            this.label110.Text = "Max Rand Year";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(65, 94);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(61, 20);
            this.button3.TabIndex = 3;
            this.button3.Text = "Remove";
            this.button3.Click += new System.EventHandler(this.btnClimTransitionRemove_Click);
            // 
            // pnlClimValues
            // 
            this.pnlClimValues.Controls.Add(this.txbClimValuesPrecip);
            this.pnlClimValues.Controls.Add(this.lblTempValues);
            this.pnlClimValues.Controls.Add(this.txbClimValuesTemp);
            this.pnlClimValues.Controls.Add(this.lblPrecipValues);
            this.pnlClimValues.Location = new System.Drawing.Point(7, 166);
            this.pnlClimValues.Name = "pnlClimValues";
            this.pnlClimValues.Size = new System.Drawing.Size(380, 44);
            this.pnlClimValues.TabIndex = 6;
            // 
            // txbClimValuesPrecip
            // 
            this.txbClimValuesPrecip.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbClimValuesPrecip.Location = new System.Drawing.Point(88, 22);
            this.txbClimValuesPrecip.Name = "txbClimValuesPrecip";
            this.txbClimValuesPrecip.Size = new System.Drawing.Size(289, 20);
            this.txbClimValuesPrecip.TabIndex = 8;
            this.txbClimValuesPrecip.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // lblTempValues
            // 
            this.lblTempValues.Location = new System.Drawing.Point(4, 5);
            this.lblTempValues.Name = "lblTempValues";
            this.lblTempValues.Size = new System.Drawing.Size(82, 16);
            this.lblTempValues.TabIndex = 44;
            this.lblTempValues.Text = "Constant Temp";
            // 
            // txbClimValuesTemp
            // 
            this.txbClimValuesTemp.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbClimValuesTemp.Location = new System.Drawing.Point(88, 1);
            this.txbClimValuesTemp.Name = "txbClimValuesTemp";
            this.txbClimValuesTemp.Size = new System.Drawing.Size(289, 20);
            this.txbClimValuesTemp.TabIndex = 7;
            this.txbClimValuesTemp.TextChanged += new System.EventHandler(this.OnClimateTransitionChanged);
            // 
            // lblPrecipValues
            // 
            this.lblPrecipValues.Location = new System.Drawing.Point(4, 26);
            this.lblPrecipValues.Name = "lblPrecipValues";
            this.lblPrecipValues.Size = new System.Drawing.Size(82, 16);
            this.lblPrecipValues.TabIndex = 45;
            this.lblPrecipValues.Text = "Constant Precip";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(6, 94);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(55, 20);
            this.button2.TabIndex = 2;
            this.button2.Text = "Add";
            this.button2.Click += new System.EventHandler(this.btnClimTransitionAdd_Click);
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.label121);
            this.panel4.Controls.Add(this.checkBox2);
            this.panel4.Controls.Add(this.textBox8);
            this.panel4.Controls.Add(this.textBox9);
            this.panel4.Controls.Add(this.label122);
            this.panel4.Location = new System.Drawing.Point(7, 211);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(380, 48);
            this.panel4.TabIndex = 47;
            // 
            // label121
            // 
            this.label121.Location = new System.Drawing.Point(4, 5);
            this.label121.Name = "label121";
            this.label121.Size = new System.Drawing.Size(82, 16);
            this.label121.TabIndex = 36;
            this.label121.Text = "Min Rand Year";
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(143, 15);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(71, 17);
            this.checkBox2.TabIndex = 46;
            this.checkBox2.Text = "Replicate";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // textBox8
            // 
            this.textBox8.Location = new System.Drawing.Point(88, 1);
            this.textBox8.Name = "textBox8";
            this.textBox8.Size = new System.Drawing.Size(47, 20);
            this.textBox8.TabIndex = 34;
            // 
            // textBox9
            // 
            this.textBox9.Location = new System.Drawing.Point(88, 22);
            this.textBox9.Name = "textBox9";
            this.textBox9.Size = new System.Drawing.Size(47, 20);
            this.textBox9.TabIndex = 35;
            // 
            // label122
            // 
            this.label122.Location = new System.Drawing.Point(4, 26);
            this.label122.Name = "label122";
            this.label122.Size = new System.Drawing.Size(82, 16);
            this.label122.TabIndex = 37;
            this.label122.Text = "Max Rand Year";
            // 
            // btnClimTransitionRemove
            // 
            this.btnClimTransitionRemove.Location = new System.Drawing.Point(65, 94);
            this.btnClimTransitionRemove.Name = "btnClimTransitionRemove";
            this.btnClimTransitionRemove.Size = new System.Drawing.Size(61, 20);
            this.btnClimTransitionRemove.TabIndex = 51;
            this.btnClimTransitionRemove.Text = "Remove";
            this.btnClimTransitionRemove.Click += new System.EventHandler(this.btnClimTransitionRemove_Click);
            // 
            // panel5
            // 
            this.panel5.Controls.Add(this.textBox10);
            this.panel5.Controls.Add(this.label123);
            this.panel5.Controls.Add(this.textBox11);
            this.panel5.Controls.Add(this.label124);
            this.panel5.Location = new System.Drawing.Point(7, 166);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(380, 44);
            this.panel5.TabIndex = 48;
            // 
            // textBox10
            // 
            this.textBox10.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox10.Location = new System.Drawing.Point(88, 24);
            this.textBox10.Name = "textBox10";
            this.textBox10.Size = new System.Drawing.Size(289, 20);
            this.textBox10.TabIndex = 46;
            // 
            // label123
            // 
            this.label123.Location = new System.Drawing.Point(4, 5);
            this.label123.Name = "label123";
            this.label123.Size = new System.Drawing.Size(82, 16);
            this.label123.TabIndex = 44;
            this.label123.Text = "Constant Temp";
            // 
            // textBox11
            // 
            this.textBox11.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox11.Location = new System.Drawing.Point(88, 2);
            this.textBox11.Name = "textBox11";
            this.textBox11.Size = new System.Drawing.Size(289, 20);
            this.textBox11.TabIndex = 2;
            // 
            // label124
            // 
            this.label124.Location = new System.Drawing.Point(4, 26);
            this.label124.Name = "label124";
            this.label124.Size = new System.Drawing.Size(82, 16);
            this.label124.TabIndex = 45;
            this.label124.Text = "Constant Precip";
            // 
            // label126
            // 
            this.label126.Location = new System.Drawing.Point(170, 17);
            this.label126.Name = "label126";
            this.label126.Size = new System.Drawing.Size(74, 16);
            this.label126.TabIndex = 54;
            this.label126.Text = "Offset Type";
            // 
            // btnClimTransitionAdd
            // 
            this.btnClimTransitionAdd.Location = new System.Drawing.Point(6, 94);
            this.btnClimTransitionAdd.Name = "btnClimTransitionAdd";
            this.btnClimTransitionAdd.Size = new System.Drawing.Size(55, 20);
            this.btnClimTransitionAdd.TabIndex = 50;
            this.btnClimTransitionAdd.Text = "Add";
            this.btnClimTransitionAdd.Click += new System.EventHandler(this.btnClimTransitionAdd_Click);
            // 
            // label127
            // 
            this.label127.Location = new System.Drawing.Point(44, 17);
            this.label127.Name = "label127";
            this.label127.Size = new System.Drawing.Size(74, 16);
            this.label127.TabIndex = 53;
            this.label127.Text = "Climate Type";
            // 
            // tabStepsAndRamps
            // 
            this.tabStepsAndRamps.Controls.Add(this.tabPageSteps);
            this.tabStepsAndRamps.Controls.Add(this.tabPageRamps);
            this.tabStepsAndRamps.Location = new System.Drawing.Point(13, 502);
            this.tabStepsAndRamps.Name = "tabStepsAndRamps";
            this.tabStepsAndRamps.SelectedIndex = 0;
            this.tabStepsAndRamps.Size = new System.Drawing.Size(303, 136);
            this.tabStepsAndRamps.TabIndex = 3;
            // 
            // tabPageSteps
            // 
            this.tabPageSteps.Controls.Add(this.btnRemoveTempStep);
            this.tabPageSteps.Controls.Add(this.txbClimOffsetsTempNewStepYear);
            this.tabPageSteps.Controls.Add(this.label22);
            this.tabPageSteps.Controls.Add(this.txbClimOffsetsTempNewStepValue);
            this.tabPageSteps.Controls.Add(this.lblTempStepYear);
            this.tabPageSteps.Controls.Add(this.lstClimTempSteps);
            this.tabPageSteps.Controls.Add(this.btnAddTempStep);
            this.tabPageSteps.Controls.Add(this.txbClimOffsetsPrecipNewStepYear);
            this.tabPageSteps.Controls.Add(this.btnRemovePrecipStep);
            this.tabPageSteps.Controls.Add(this.lstClimPrecipSteps);
            this.tabPageSteps.Controls.Add(this.btnAddPrecipStep);
            this.tabPageSteps.Controls.Add(this.txbClimOffsetsPrecipNewStepValue);
            this.tabPageSteps.Controls.Add(this.label34);
            this.tabPageSteps.Controls.Add(this.label33);
            this.tabPageSteps.Location = new System.Drawing.Point(4, 22);
            this.tabPageSteps.Name = "tabPageSteps";
            this.tabPageSteps.Size = new System.Drawing.Size(295, 110);
            this.tabPageSteps.TabIndex = 1;
            this.tabPageSteps.Text = "Steps";
            this.tabPageSteps.UseVisualStyleBackColor = true;
            // 
            // btnRemoveTempStep
            // 
            this.btnRemoveTempStep.Location = new System.Drawing.Point(88, 49);
            this.btnRemoveTempStep.Name = "btnRemoveTempStep";
            this.btnRemoveTempStep.Size = new System.Drawing.Size(51, 20);
            this.btnRemoveTempStep.TabIndex = 26;
            this.btnRemoveTempStep.Text = "Delete";
            this.btnRemoveTempStep.Click += new System.EventHandler(this.btnRemoveTempStep_Click);
            // 
            // txbClimOffsetsTempNewStepYear
            // 
            this.txbClimOffsetsTempNewStepYear.Location = new System.Drawing.Point(8, 23);
            this.txbClimOffsetsTempNewStepYear.Name = "txbClimOffsetsTempNewStepYear";
            this.txbClimOffsetsTempNewStepYear.Size = new System.Drawing.Size(33, 20);
            this.txbClimOffsetsTempNewStepYear.TabIndex = 22;
            // 
            // label22
            // 
            this.label22.Location = new System.Drawing.Point(46, 9);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(38, 12);
            this.label22.TabIndex = 22;
            this.label22.Text = "Temp";
            // 
            // txbClimOffsetsTempNewStepValue
            // 
            this.txbClimOffsetsTempNewStepValue.Location = new System.Drawing.Point(46, 24);
            this.txbClimOffsetsTempNewStepValue.Name = "txbClimOffsetsTempNewStepValue";
            this.txbClimOffsetsTempNewStepValue.Size = new System.Drawing.Size(36, 20);
            this.txbClimOffsetsTempNewStepValue.TabIndex = 23;
            // 
            // lblTempStepYear
            // 
            this.lblTempStepYear.Location = new System.Drawing.Point(5, 9);
            this.lblTempStepYear.Name = "lblTempStepYear";
            this.lblTempStepYear.Size = new System.Drawing.Size(32, 16);
            this.lblTempStepYear.TabIndex = 21;
            this.lblTempStepYear.Text = "Year";
            // 
            // lstClimTempSteps
            // 
            this.lstClimTempSteps.FormattingEnabled = true;
            this.lstClimTempSteps.Location = new System.Drawing.Point(8, 48);
            this.lstClimTempSteps.Name = "lstClimTempSteps";
            this.lstClimTempSteps.Size = new System.Drawing.Size(74, 56);
            this.lstClimTempSteps.TabIndex = 25;
            // 
            // btnAddTempStep
            // 
            this.btnAddTempStep.Location = new System.Drawing.Point(88, 23);
            this.btnAddTempStep.Name = "btnAddTempStep";
            this.btnAddTempStep.Size = new System.Drawing.Size(51, 20);
            this.btnAddTempStep.TabIndex = 24;
            this.btnAddTempStep.Text = "Add";
            this.btnAddTempStep.Click += new System.EventHandler(this.btnAddTempStep_Click);
            // 
            // txbClimOffsetsPrecipNewStepYear
            // 
            this.txbClimOffsetsPrecipNewStepYear.Location = new System.Drawing.Point(155, 26);
            this.txbClimOffsetsPrecipNewStepYear.Name = "txbClimOffsetsPrecipNewStepYear";
            this.txbClimOffsetsPrecipNewStepYear.Size = new System.Drawing.Size(36, 20);
            this.txbClimOffsetsPrecipNewStepYear.TabIndex = 27;
            // 
            // btnRemovePrecipStep
            // 
            this.btnRemovePrecipStep.Location = new System.Drawing.Point(235, 48);
            this.btnRemovePrecipStep.Name = "btnRemovePrecipStep";
            this.btnRemovePrecipStep.Size = new System.Drawing.Size(52, 20);
            this.btnRemovePrecipStep.TabIndex = 31;
            this.btnRemovePrecipStep.Text = "Delete";
            this.btnRemovePrecipStep.Click += new System.EventHandler(this.btnRemovePrecipStep_Click);
            // 
            // lstClimPrecipSteps
            // 
            this.lstClimPrecipSteps.FormattingEnabled = true;
            this.lstClimPrecipSteps.Location = new System.Drawing.Point(156, 48);
            this.lstClimPrecipSteps.Name = "lstClimPrecipSteps";
            this.lstClimPrecipSteps.Size = new System.Drawing.Size(73, 56);
            this.lstClimPrecipSteps.Sorted = true;
            this.lstClimPrecipSteps.TabIndex = 30;
            // 
            // btnAddPrecipStep
            // 
            this.btnAddPrecipStep.Location = new System.Drawing.Point(235, 25);
            this.btnAddPrecipStep.Name = "btnAddPrecipStep";
            this.btnAddPrecipStep.Size = new System.Drawing.Size(52, 20);
            this.btnAddPrecipStep.TabIndex = 29;
            this.btnAddPrecipStep.Text = "Add";
            this.btnAddPrecipStep.Click += new System.EventHandler(this.btnAddPrecipStep_Click);
            // 
            // txbClimOffsetsPrecipNewStepValue
            // 
            this.txbClimOffsetsPrecipNewStepValue.Location = new System.Drawing.Point(193, 26);
            this.txbClimOffsetsPrecipNewStepValue.Name = "txbClimOffsetsPrecipNewStepValue";
            this.txbClimOffsetsPrecipNewStepValue.Size = new System.Drawing.Size(36, 20);
            this.txbClimOffsetsPrecipNewStepValue.TabIndex = 28;
            // 
            // label34
            // 
            this.label34.Location = new System.Drawing.Point(155, 11);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(32, 16);
            this.label34.TabIndex = 5;
            this.label34.Text = "Year";
            // 
            // label33
            // 
            this.label33.Location = new System.Drawing.Point(192, 11);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(48, 16);
            this.label33.TabIndex = 6;
            this.label33.Text = "Precip";
            // 
            // tabPageRamps
            // 
            this.tabPageRamps.Controls.Add(this.txbClimOffsetsPrecipNewRampYear);
            this.tabPageRamps.Controls.Add(this.btnRemovePrecipRamp);
            this.tabPageRamps.Controls.Add(this.lstClimPrecipRamps);
            this.tabPageRamps.Controls.Add(this.btnAddPrecipRamp);
            this.tabPageRamps.Controls.Add(this.label32);
            this.tabPageRamps.Controls.Add(this.label31);
            this.tabPageRamps.Controls.Add(this.txbClimOffsetsPrecipNewRampValue);
            this.tabPageRamps.Controls.Add(this.txbClimOffsetsTempNewRampYear);
            this.tabPageRamps.Controls.Add(this.btnAddTempRamp);
            this.tabPageRamps.Controls.Add(this.txbClimOffsetsTempNewRampValue);
            this.tabPageRamps.Controls.Add(this.btnRemoveTempRamp);
            this.tabPageRamps.Controls.Add(this.lstClimTempRamps);
            this.tabPageRamps.Controls.Add(this.label23);
            this.tabPageRamps.Controls.Add(this.label24);
            this.tabPageRamps.Location = new System.Drawing.Point(4, 22);
            this.tabPageRamps.Name = "tabPageRamps";
            this.tabPageRamps.Size = new System.Drawing.Size(295, 110);
            this.tabPageRamps.TabIndex = 0;
            this.tabPageRamps.Text = "Ramps";
            this.tabPageRamps.UseVisualStyleBackColor = true;
            // 
            // txbClimOffsetsPrecipNewRampYear
            // 
            this.txbClimOffsetsPrecipNewRampYear.Location = new System.Drawing.Point(160, 22);
            this.txbClimOffsetsPrecipNewRampYear.Name = "txbClimOffsetsPrecipNewRampYear";
            this.txbClimOffsetsPrecipNewRampYear.Size = new System.Drawing.Size(34, 20);
            this.txbClimOffsetsPrecipNewRampYear.TabIndex = 37;
            // 
            // btnRemovePrecipRamp
            // 
            this.btnRemovePrecipRamp.Location = new System.Drawing.Point(237, 46);
            this.btnRemovePrecipRamp.Name = "btnRemovePrecipRamp";
            this.btnRemovePrecipRamp.Size = new System.Drawing.Size(51, 20);
            this.btnRemovePrecipRamp.TabIndex = 41;
            this.btnRemovePrecipRamp.Text = "Delete";
            this.btnRemovePrecipRamp.Click += new System.EventHandler(this.btnRemovePrecipRamp_Click);
            // 
            // lstClimPrecipRamps
            // 
            this.lstClimPrecipRamps.FormattingEnabled = true;
            this.lstClimPrecipRamps.Location = new System.Drawing.Point(160, 46);
            this.lstClimPrecipRamps.Name = "lstClimPrecipRamps";
            this.lstClimPrecipRamps.Size = new System.Drawing.Size(72, 56);
            this.lstClimPrecipRamps.TabIndex = 40;
            // 
            // btnAddPrecipRamp
            // 
            this.btnAddPrecipRamp.Location = new System.Drawing.Point(237, 23);
            this.btnAddPrecipRamp.Name = "btnAddPrecipRamp";
            this.btnAddPrecipRamp.Size = new System.Drawing.Size(50, 20);
            this.btnAddPrecipRamp.TabIndex = 39;
            this.btnAddPrecipRamp.Text = "Add";
            this.btnAddPrecipRamp.Click += new System.EventHandler(this.btnAddPrecipRamp_Click);
            // 
            // label32
            // 
            this.label32.Location = new System.Drawing.Point(157, 7);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(36, 16);
            this.label32.TabIndex = 22;
            this.label32.Text = "Year";
            // 
            // label31
            // 
            this.label31.Location = new System.Drawing.Point(195, 7);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(39, 16);
            this.label31.TabIndex = 23;
            this.label31.Text = "Precip";
            // 
            // txbClimOffsetsPrecipNewRampValue
            // 
            this.txbClimOffsetsPrecipNewRampValue.Location = new System.Drawing.Point(198, 23);
            this.txbClimOffsetsPrecipNewRampValue.Name = "txbClimOffsetsPrecipNewRampValue";
            this.txbClimOffsetsPrecipNewRampValue.Size = new System.Drawing.Size(35, 20);
            this.txbClimOffsetsPrecipNewRampValue.TabIndex = 38;
            // 
            // txbClimOffsetsTempNewRampYear
            // 
            this.txbClimOffsetsTempNewRampYear.Location = new System.Drawing.Point(9, 23);
            this.txbClimOffsetsTempNewRampYear.Name = "txbClimOffsetsTempNewRampYear";
            this.txbClimOffsetsTempNewRampYear.Size = new System.Drawing.Size(33, 20);
            this.txbClimOffsetsTempNewRampYear.TabIndex = 32;
            // 
            // btnAddTempRamp
            // 
            this.btnAddTempRamp.Location = new System.Drawing.Point(86, 23);
            this.btnAddTempRamp.Name = "btnAddTempRamp";
            this.btnAddTempRamp.Size = new System.Drawing.Size(51, 20);
            this.btnAddTempRamp.TabIndex = 34;
            this.btnAddTempRamp.Text = "Add";
            this.btnAddTempRamp.Click += new System.EventHandler(this.btnAddTempRamp_Click);
            // 
            // txbClimOffsetsTempNewRampValue
            // 
            this.txbClimOffsetsTempNewRampValue.Location = new System.Drawing.Point(47, 23);
            this.txbClimOffsetsTempNewRampValue.Name = "txbClimOffsetsTempNewRampValue";
            this.txbClimOffsetsTempNewRampValue.Size = new System.Drawing.Size(34, 20);
            this.txbClimOffsetsTempNewRampValue.TabIndex = 33;
            // 
            // btnRemoveTempRamp
            // 
            this.btnRemoveTempRamp.Location = new System.Drawing.Point(86, 47);
            this.btnRemoveTempRamp.Name = "btnRemoveTempRamp";
            this.btnRemoveTempRamp.Size = new System.Drawing.Size(50, 20);
            this.btnRemoveTempRamp.TabIndex = 36;
            this.btnRemoveTempRamp.Text = "Delete";
            this.btnRemoveTempRamp.Click += new System.EventHandler(this.btnRemoveTempRamp_Click);
            // 
            // lstClimTempRamps
            // 
            this.lstClimTempRamps.FormattingEnabled = true;
            this.lstClimTempRamps.Location = new System.Drawing.Point(9, 47);
            this.lstClimTempRamps.Name = "lstClimTempRamps";
            this.lstClimTempRamps.Size = new System.Drawing.Size(72, 56);
            this.lstClimTempRamps.TabIndex = 35;
            // 
            // label23
            // 
            this.label23.Location = new System.Drawing.Point(44, 8);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(38, 12);
            this.label23.TabIndex = 13;
            this.label23.Text = "Temp";
            // 
            // label24
            // 
            this.label24.Location = new System.Drawing.Point(6, 8);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(36, 16);
            this.label24.TabIndex = 12;
            this.label24.Text = "Year";
            // 
            // chkStepsAndRampsEnabled
            // 
            this.chkStepsAndRampsEnabled.AutoSize = true;
            this.chkStepsAndRampsEnabled.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkStepsAndRampsEnabled.Location = new System.Drawing.Point(16, 479);
            this.chkStepsAndRampsEnabled.Name = "chkStepsAndRampsEnabled";
            this.chkStepsAndRampsEnabled.Size = new System.Drawing.Size(168, 17);
            this.chkStepsAndRampsEnabled.TabIndex = 2;
            this.chkStepsAndRampsEnabled.Text = "Enable Steps and Ramps";
            this.chkStepsAndRampsEnabled.UseVisualStyleBackColor = true;
            this.chkStepsAndRampsEnabled.CheckedChanged += new System.EventHandler(this.chkStepsAndRampsEnabled_CheckedChanged);
            // 
            // txbClimNumHistory
            // 
            this.txbClimNumHistory.Location = new System.Drawing.Point(101, 8);
            this.txbClimNumHistory.Name = "txbClimNumHistory";
            this.txbClimNumHistory.Size = new System.Drawing.Size(51, 20);
            this.txbClimNumHistory.TabIndex = 0;
            // 
            // label38
            // 
            this.label38.Location = new System.Drawing.Point(12, 11);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(100, 16);
            this.label38.TabIndex = 30;
            this.label38.Text = "History";
            // 
            // tabMapOutput
            // 
            this.tabMapOutput.Controls.Add(this.chkMapFireScar);
            this.tabMapOutput.Controls.Add(this.btnMapDelete);
            this.tabMapOutput.Controls.Add(this.chkMapFrame4);
            this.tabMapOutput.Controls.Add(this.chkMapFrame3);
            this.tabMapOutput.Controls.Add(this.chkMapFrame2);
            this.tabMapOutput.Controls.Add(this.chkMapFrame1);
            this.tabMapOutput.Controls.Add(this.chkMapPrecip);
            this.tabMapOutput.Controls.Add(this.chkMapTemp);
            this.tabMapOutput.Controls.Add(this.chkMapFireAge);
            this.tabMapOutput.Controls.Add(this.chkMapSub);
            this.tabMapOutput.Controls.Add(this.chkMapSite);
            this.tabMapOutput.Controls.Add(this.chkMapAge);
            this.tabMapOutput.Controls.Add(this.chkMapVeg);
            this.tabMapOutput.Controls.Add(this.chkMapARC);
            this.tabMapOutput.Controls.Add(this.chkMapHeader);
            this.tabMapOutput.Controls.Add(this.chkMapFormat);
            this.tabMapOutput.Controls.Add(this.chkMapIsOutput);
            this.tabMapOutput.Controls.Add(this.cbbMapOutputSelect);
            this.tabMapOutput.Controls.Add(this.label67);
            this.tabMapOutput.Controls.Add(this.label66);
            this.tabMapOutput.Controls.Add(this.label65);
            this.tabMapOutput.Controls.Add(this.label64);
            this.tabMapOutput.Controls.Add(this.btnMapAdd);
            this.tabMapOutput.Controls.Add(this.lblMapFlags);
            this.tabMapOutput.Controls.Add(this.txbMapYearFreq);
            this.tabMapOutput.Controls.Add(this.txbMapYearStart);
            this.tabMapOutput.Controls.Add(this.txbMapRepFreq);
            this.tabMapOutput.Controls.Add(this.txbMapRepStart);
            this.tabMapOutput.Controls.Add(this.txbMapCodes);
            this.tabMapOutput.Controls.Add(this.label61);
            this.tabMapOutput.Controls.Add(this.label59);
            this.tabMapOutput.Controls.Add(this.label58);
            this.tabMapOutput.Controls.Add(this.label57);
            this.tabMapOutput.Controls.Add(this.label53);
            this.tabMapOutput.Location = new System.Drawing.Point(4, 22);
            this.tabMapOutput.Name = "tabMapOutput";
            this.tabMapOutput.Size = new System.Drawing.Size(407, 654);
            this.tabMapOutput.TabIndex = 4;
            this.tabMapOutput.Text = "Map Output";
            // 
            // chkMapFireScar
            // 
            this.chkMapFireScar.Location = new System.Drawing.Point(180, 404);
            this.chkMapFireScar.Name = "chkMapFireScar";
            this.chkMapFireScar.Size = new System.Drawing.Size(92, 16);
            this.chkMapFireScar.TabIndex = 25;
            this.chkMapFireScar.Text = "Fire Scar";
            // 
            // btnMapDelete
            // 
            this.btnMapDelete.Location = new System.Drawing.Point(128, 64);
            this.btnMapDelete.Name = "btnMapDelete";
            this.btnMapDelete.Size = new System.Drawing.Size(52, 20);
            this.btnMapDelete.TabIndex = 2;
            this.btnMapDelete.Text = "Delete";
            this.btnMapDelete.Click += new System.EventHandler(this.btnMapDelete_Click);
            // 
            // chkMapFrame4
            // 
            this.chkMapFrame4.Location = new System.Drawing.Point(28, 448);
            this.chkMapFrame4.Name = "chkMapFrame4";
            this.chkMapFrame4.Size = new System.Drawing.Size(92, 16);
            this.chkMapFrame4.TabIndex = 15;
            this.chkMapFrame4.Text = "4";
            this.chkMapFrame4.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapFrame3
            // 
            this.chkMapFrame3.Location = new System.Drawing.Point(28, 428);
            this.chkMapFrame3.Name = "chkMapFrame3";
            this.chkMapFrame3.Size = new System.Drawing.Size(92, 16);
            this.chkMapFrame3.TabIndex = 14;
            this.chkMapFrame3.Text = "3";
            this.chkMapFrame3.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapFrame2
            // 
            this.chkMapFrame2.Location = new System.Drawing.Point(28, 408);
            this.chkMapFrame2.Name = "chkMapFrame2";
            this.chkMapFrame2.Size = new System.Drawing.Size(92, 16);
            this.chkMapFrame2.TabIndex = 13;
            this.chkMapFrame2.Text = "2";
            this.chkMapFrame2.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapFrame1
            // 
            this.chkMapFrame1.Location = new System.Drawing.Point(28, 388);
            this.chkMapFrame1.Name = "chkMapFrame1";
            this.chkMapFrame1.Size = new System.Drawing.Size(92, 16);
            this.chkMapFrame1.TabIndex = 12;
            this.chkMapFrame1.Text = "1";
            this.chkMapFrame1.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapPrecip
            // 
            this.chkMapPrecip.Location = new System.Drawing.Point(180, 444);
            this.chkMapPrecip.Name = "chkMapPrecip";
            this.chkMapPrecip.Size = new System.Drawing.Size(92, 16);
            this.chkMapPrecip.TabIndex = 22;
            this.chkMapPrecip.Text = "Precip Offset";
            this.chkMapPrecip.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapTemp
            // 
            this.chkMapTemp.Location = new System.Drawing.Point(180, 424);
            this.chkMapTemp.Name = "chkMapTemp";
            this.chkMapTemp.Size = new System.Drawing.Size(92, 16);
            this.chkMapTemp.TabIndex = 21;
            this.chkMapTemp.Text = "Temp Offset";
            this.chkMapTemp.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapFireAge
            // 
            this.chkMapFireAge.Location = new System.Drawing.Point(180, 384);
            this.chkMapFireAge.Name = "chkMapFireAge";
            this.chkMapFireAge.Size = new System.Drawing.Size(92, 16);
            this.chkMapFireAge.TabIndex = 20;
            this.chkMapFireAge.Text = "Fire Age";
            this.chkMapFireAge.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapSub
            // 
            this.chkMapSub.Location = new System.Drawing.Point(180, 364);
            this.chkMapSub.Name = "chkMapSub";
            this.chkMapSub.Size = new System.Drawing.Size(92, 16);
            this.chkMapSub.TabIndex = 19;
            this.chkMapSub.Text = "Sub Canopy";
            this.chkMapSub.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapSite
            // 
            this.chkMapSite.Location = new System.Drawing.Point(180, 344);
            this.chkMapSite.Name = "chkMapSite";
            this.chkMapSite.Size = new System.Drawing.Size(92, 16);
            this.chkMapSite.TabIndex = 18;
            this.chkMapSite.Text = "Site";
            this.chkMapSite.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapAge
            // 
            this.chkMapAge.Location = new System.Drawing.Point(180, 324);
            this.chkMapAge.Name = "chkMapAge";
            this.chkMapAge.Size = new System.Drawing.Size(92, 16);
            this.chkMapAge.TabIndex = 17;
            this.chkMapAge.Text = "Age";
            this.chkMapAge.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapVeg
            // 
            this.chkMapVeg.Location = new System.Drawing.Point(180, 304);
            this.chkMapVeg.Name = "chkMapVeg";
            this.chkMapVeg.Size = new System.Drawing.Size(92, 16);
            this.chkMapVeg.TabIndex = 16;
            this.chkMapVeg.Text = "Veg";
            this.chkMapVeg.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapARC
            // 
            this.chkMapARC.Location = new System.Drawing.Point(28, 344);
            this.chkMapARC.Name = "chkMapARC";
            this.chkMapARC.Size = new System.Drawing.Size(124, 16);
            this.chkMapARC.TabIndex = 11;
            this.chkMapARC.Text = "ARCgis Header";
            this.chkMapARC.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapHeader
            // 
            this.chkMapHeader.Location = new System.Drawing.Point(28, 324);
            this.chkMapHeader.Name = "chkMapHeader";
            this.chkMapHeader.Size = new System.Drawing.Size(124, 16);
            this.chkMapHeader.TabIndex = 10;
            this.chkMapHeader.Text = "Years/Reps Header";
            this.chkMapHeader.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapFormat
            // 
            this.chkMapFormat.Location = new System.Drawing.Point(28, 304);
            this.chkMapFormat.Name = "chkMapFormat";
            this.chkMapFormat.Size = new System.Drawing.Size(124, 16);
            this.chkMapFormat.TabIndex = 9;
            this.chkMapFormat.Text = "Format";
            this.chkMapFormat.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // chkMapIsOutput
            // 
            this.chkMapIsOutput.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkMapIsOutput.Location = new System.Drawing.Point(24, 264);
            this.chkMapIsOutput.Name = "chkMapIsOutput";
            this.chkMapIsOutput.Size = new System.Drawing.Size(92, 16);
            this.chkMapIsOutput.TabIndex = 8;
            this.chkMapIsOutput.Text = "Output Data";
            this.chkMapIsOutput.CheckedChanged += new System.EventHandler(this.FIFFlagsChanged);
            // 
            // cbbMapOutputSelect
            // 
            this.cbbMapOutputSelect.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.cbbMapOutputSelect.FormattingEnabled = true;
            this.cbbMapOutputSelect.Location = new System.Drawing.Point(16, 40);
            this.cbbMapOutputSelect.Name = "cbbMapOutputSelect";
            this.cbbMapOutputSelect.Size = new System.Drawing.Size(251, 21);
            this.cbbMapOutputSelect.TabIndex = 0;
            this.cbbMapOutputSelect.SelectionChangeCommitted += new System.EventHandler(this.cbbMapOutputSelect_SelectionChangeCommited);
            this.cbbMapOutputSelect.TextChanged += new System.EventHandler(this.cbbMapOutputSelect_TextChanged);
            this.cbbMapOutputSelect.DropDown += new System.EventHandler(this.cbbMapOutputSelect_DropDown);
            // 
            // label67
            // 
            this.label67.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label67.Location = new System.Drawing.Point(16, 20);
            this.label67.Name = "label67";
            this.label67.Size = new System.Drawing.Size(168, 16);
            this.label67.TabIndex = 24;
            this.label67.Text = "Choose a map entry to edit:";
            // 
            // label66
            // 
            this.label66.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label66.Location = new System.Drawing.Point(12, 372);
            this.label66.Name = "label66";
            this.label66.Size = new System.Drawing.Size(108, 16);
            this.label66.TabIndex = 23;
            this.label66.Text = "Map Frame";
            // 
            // label65
            // 
            this.label65.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label65.Location = new System.Drawing.Point(172, 288);
            this.label65.Name = "label65";
            this.label65.Size = new System.Drawing.Size(100, 16);
            this.label65.TabIndex = 22;
            this.label65.Text = "Map Type";
            // 
            // label64
            // 
            this.label64.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label64.Location = new System.Drawing.Point(12, 288);
            this.label64.Name = "label64";
            this.label64.Size = new System.Drawing.Size(140, 16);
            this.label64.TabIndex = 21;
            this.label64.Text = "Map Format";
            // 
            // btnMapAdd
            // 
            this.btnMapAdd.Location = new System.Drawing.Point(16, 64);
            this.btnMapAdd.Name = "btnMapAdd";
            this.btnMapAdd.Size = new System.Drawing.Size(104, 20);
            this.btnMapAdd.TabIndex = 1;
            this.btnMapAdd.Text = "Add Map Output";
            this.btnMapAdd.Click += new System.EventHandler(this.btnMapAdd_Click);
            // 
            // lblMapFlags
            // 
            this.lblMapFlags.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblMapFlags.Location = new System.Drawing.Point(16, 633);
            this.lblMapFlags.Name = "lblMapFlags";
            this.lblMapFlags.Size = new System.Drawing.Size(184, 16);
            this.lblMapFlags.TabIndex = 15;
            this.lblMapFlags.Text = "FIF Flags: 0x000";
            // 
            // txbMapYearFreq
            // 
            this.txbMapYearFreq.Location = new System.Drawing.Point(128, 208);
            this.txbMapYearFreq.Name = "txbMapYearFreq";
            this.txbMapYearFreq.Size = new System.Drawing.Size(48, 20);
            this.txbMapYearFreq.TabIndex = 7;
            // 
            // txbMapYearStart
            // 
            this.txbMapYearStart.Location = new System.Drawing.Point(128, 184);
            this.txbMapYearStart.Name = "txbMapYearStart";
            this.txbMapYearStart.Size = new System.Drawing.Size(48, 20);
            this.txbMapYearStart.TabIndex = 6;
            // 
            // txbMapRepFreq
            // 
            this.txbMapRepFreq.Location = new System.Drawing.Point(128, 160);
            this.txbMapRepFreq.Name = "txbMapRepFreq";
            this.txbMapRepFreq.Size = new System.Drawing.Size(48, 20);
            this.txbMapRepFreq.TabIndex = 5;
            // 
            // txbMapRepStart
            // 
            this.txbMapRepStart.Location = new System.Drawing.Point(128, 136);
            this.txbMapRepStart.Name = "txbMapRepStart";
            this.txbMapRepStart.Size = new System.Drawing.Size(48, 20);
            this.txbMapRepStart.TabIndex = 4;
            // 
            // txbMapCodes
            // 
            this.txbMapCodes.Location = new System.Drawing.Point(128, 112);
            this.txbMapCodes.Name = "txbMapCodes";
            this.txbMapCodes.Size = new System.Drawing.Size(56, 20);
            this.txbMapCodes.TabIndex = 3;
            // 
            // label61
            // 
            this.label61.Location = new System.Drawing.Point(16, 116);
            this.label61.Name = "label61";
            this.label61.Size = new System.Drawing.Size(100, 16);
            this.label61.TabIndex = 6;
            this.label61.Text = "Map Code";
            // 
            // label59
            // 
            this.label59.Location = new System.Drawing.Point(16, 140);
            this.label59.Name = "label59";
            this.label59.Size = new System.Drawing.Size(100, 16);
            this.label59.TabIndex = 4;
            this.label59.Text = "Map Rep Start";
            // 
            // label58
            // 
            this.label58.Location = new System.Drawing.Point(16, 164);
            this.label58.Name = "label58";
            this.label58.Size = new System.Drawing.Size(100, 16);
            this.label58.TabIndex = 3;
            this.label58.Text = "Map Rep Freq";
            // 
            // label57
            // 
            this.label57.Location = new System.Drawing.Point(16, 188);
            this.label57.Name = "label57";
            this.label57.Size = new System.Drawing.Size(100, 16);
            this.label57.TabIndex = 2;
            this.label57.Text = "Map Year Start";
            // 
            // label53
            // 
            this.label53.Location = new System.Drawing.Point(16, 212);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(100, 16);
            this.label53.TabIndex = 1;
            this.label53.Text = "Map Year Freq";
            // 
            // tabStatOutput
            // 
            this.tabStatOutput.Controls.Add(this.chkStatEvents);
            this.tabStatOutput.Controls.Add(this.chkStatData);
            this.tabStatOutput.Controls.Add(this.chkStatHist);
            this.tabStatOutput.Controls.Add(this.chkStatMax);
            this.tabStatOutput.Controls.Add(this.chkStatMin);
            this.tabStatOutput.Controls.Add(this.chkStatStdDev);
            this.tabStatOutput.Controls.Add(this.chkStatMean);
            this.tabStatOutput.Controls.Add(this.chkStatNum);
            this.tabStatOutput.Controls.Add(this.chkStatFormat);
            this.tabStatOutput.Controls.Add(this.chkStatRep);
            this.tabStatOutput.Controls.Add(this.chkStatYear);
            this.tabStatOutput.Controls.Add(this.cbbStatSelect);
            this.tabStatOutput.Controls.Add(this.txbStatSummaryFile);
            this.tabStatOutput.Controls.Add(this.label62);
            this.tabStatOutput.Controls.Add(this.label96);
            this.tabStatOutput.Location = new System.Drawing.Point(4, 22);
            this.tabStatOutput.Name = "tabStatOutput";
            this.tabStatOutput.Size = new System.Drawing.Size(407, 654);
            this.tabStatOutput.TabIndex = 5;
            this.tabStatOutput.Text = "Stat Output";
            // 
            // chkStatEvents
            // 
            this.chkStatEvents.Location = new System.Drawing.Point(96, 308);
            this.chkStatEvents.Name = "chkStatEvents";
            this.chkStatEvents.Size = new System.Drawing.Size(92, 16);
            this.chkStatEvents.TabIndex = 12;
            this.chkStatEvents.Text = "Events";
            // 
            // chkStatData
            // 
            this.chkStatData.Location = new System.Drawing.Point(96, 288);
            this.chkStatData.Name = "chkStatData";
            this.chkStatData.Size = new System.Drawing.Size(92, 16);
            this.chkStatData.TabIndex = 11;
            this.chkStatData.Text = "Data";
            // 
            // chkStatHist
            // 
            this.chkStatHist.Location = new System.Drawing.Point(96, 268);
            this.chkStatHist.Name = "chkStatHist";
            this.chkStatHist.Size = new System.Drawing.Size(92, 16);
            this.chkStatHist.TabIndex = 10;
            this.chkStatHist.Text = "Hist";
            // 
            // chkStatMax
            // 
            this.chkStatMax.Location = new System.Drawing.Point(96, 248);
            this.chkStatMax.Name = "chkStatMax";
            this.chkStatMax.Size = new System.Drawing.Size(92, 16);
            this.chkStatMax.TabIndex = 9;
            this.chkStatMax.Text = "Max";
            // 
            // chkStatMin
            // 
            this.chkStatMin.Location = new System.Drawing.Point(96, 228);
            this.chkStatMin.Name = "chkStatMin";
            this.chkStatMin.Size = new System.Drawing.Size(92, 16);
            this.chkStatMin.TabIndex = 8;
            this.chkStatMin.Text = "Min";
            // 
            // chkStatStdDev
            // 
            this.chkStatStdDev.Location = new System.Drawing.Point(96, 208);
            this.chkStatStdDev.Name = "chkStatStdDev";
            this.chkStatStdDev.Size = new System.Drawing.Size(92, 16);
            this.chkStatStdDev.TabIndex = 7;
            this.chkStatStdDev.Text = "StdDev";
            // 
            // chkStatMean
            // 
            this.chkStatMean.Location = new System.Drawing.Point(96, 188);
            this.chkStatMean.Name = "chkStatMean";
            this.chkStatMean.Size = new System.Drawing.Size(92, 16);
            this.chkStatMean.TabIndex = 6;
            this.chkStatMean.Text = "Mean";
            // 
            // chkStatNum
            // 
            this.chkStatNum.Location = new System.Drawing.Point(96, 168);
            this.chkStatNum.Name = "chkStatNum";
            this.chkStatNum.Size = new System.Drawing.Size(92, 16);
            this.chkStatNum.TabIndex = 5;
            this.chkStatNum.Text = "Num";
            // 
            // chkStatFormat
            // 
            this.chkStatFormat.Location = new System.Drawing.Point(96, 148);
            this.chkStatFormat.Name = "chkStatFormat";
            this.chkStatFormat.Size = new System.Drawing.Size(92, 16);
            this.chkStatFormat.TabIndex = 4;
            this.chkStatFormat.Text = "Format";
            // 
            // chkStatRep
            // 
            this.chkStatRep.Location = new System.Drawing.Point(96, 128);
            this.chkStatRep.Name = "chkStatRep";
            this.chkStatRep.Size = new System.Drawing.Size(92, 16);
            this.chkStatRep.TabIndex = 3;
            this.chkStatRep.Text = "Rep";
            // 
            // chkStatYear
            // 
            this.chkStatYear.Location = new System.Drawing.Point(96, 108);
            this.chkStatYear.Name = "chkStatYear";
            this.chkStatYear.Size = new System.Drawing.Size(92, 16);
            this.chkStatYear.TabIndex = 2;
            this.chkStatYear.Text = "Year";
            // 
            // cbbStatSelect
            // 
            this.cbbStatSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbbStatSelect.FormattingEnabled = true;
            this.cbbStatSelect.Items.AddRange(new object[] {
            "Fire Number",
            "Fire Size",
            "Veg Distribution",
            "Veg Residence Time",
            "Fire by Species",
            "Fire Interval"});
            this.cbbStatSelect.Location = new System.Drawing.Point(96, 80);
            this.cbbStatSelect.Name = "cbbStatSelect";
            this.cbbStatSelect.Size = new System.Drawing.Size(132, 21);
            this.cbbStatSelect.TabIndex = 1;
            this.cbbStatSelect.SelectionChangeCommitted += new System.EventHandler(this.cbbStatSelect_SelectionChangeCommitted);
            this.cbbStatSelect.DropDown += new System.EventHandler(this.cbbStatSelect_DropDown);
            // 
            // txbStatSummaryFile
            // 
            this.txbStatSummaryFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbStatSummaryFile.Location = new System.Drawing.Point(96, 24);
            this.txbStatSummaryFile.Name = "txbStatSummaryFile";
            this.txbStatSummaryFile.Size = new System.Drawing.Size(135, 20);
            this.txbStatSummaryFile.TabIndex = 0;
            // 
            // label62
            // 
            this.label62.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label62.Location = new System.Drawing.Point(8, 28);
            this.label62.Name = "label62";
            this.label62.Size = new System.Drawing.Size(100, 16);
            this.label62.TabIndex = 0;
            this.label62.Text = "Summary File";
            // 
            // label96
            // 
            this.label96.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label96.Location = new System.Drawing.Point(96, 64);
            this.label96.Name = "label96";
            this.label96.Size = new System.Drawing.Size(204, 16);
            this.label96.TabIndex = 16;
            this.label96.Text = "Choose a stat item to edit its output:";
            // 
            // tabLandscape
            // 
            this.tabLandscape.Controls.Add(this.txbLandYearOfVegAndAgeFiles);
            this.tabLandscape.Controls.Add(this.chkLandIsUsingUniqueVegAndAgePerRep);
            this.tabLandscape.Controls.Add(this.chkLandIsForcedVegTransitions);
            this.tabLandscape.Controls.Add(this.button1);
            this.tabLandscape.Controls.Add(this.label109);
            this.tabLandscape.Controls.Add(this.txbLandVegTransitionFile);
            this.tabLandscape.Controls.Add(this.btnLandTreeDensityInputFileLookup);
            this.tabLandscape.Controls.Add(this.btnLandSiteInputFileLookup);
            this.tabLandscape.Controls.Add(this.btnLandAgeInputFileLookup);
            this.tabLandscape.Controls.Add(this.btnLandVegInputFileLookup);
            this.tabLandscape.Controls.Add(this.label21);
            this.tabLandscape.Controls.Add(this.label19);
            this.tabLandscape.Controls.Add(this.label18);
            this.tabLandscape.Controls.Add(this.label17);
            this.tabLandscape.Controls.Add(this.label16);
            this.tabLandscape.Controls.Add(this.label15);
            this.tabLandscape.Controls.Add(this.label14);
            this.tabLandscape.Controls.Add(this.label13);
            this.tabLandscape.Controls.Add(this.label12);
            this.tabLandscape.Controls.Add(this.label11);
            this.tabLandscape.Controls.Add(this.label10);
            this.tabLandscape.Controls.Add(this.chkLandCropNeighbors);
            this.tabLandscape.Controls.Add(this.txbLandMaxCol);
            this.tabLandscape.Controls.Add(this.txbLandCellSize);
            this.tabLandscape.Controls.Add(this.txbLandNumSpecies);
            this.tabLandscape.Controls.Add(this.txbLandNoVeg);
            this.tabLandscape.Controls.Add(this.txbLandXllCorner);
            this.tabLandscape.Controls.Add(this.txbLandYllCorner);
            this.tabLandscape.Controls.Add(this.txbLandSiteInputFile);
            this.tabLandscape.Controls.Add(this.txbLandAgeInputFile);
            this.tabLandscape.Controls.Add(this.txbLandTreeDensityInputFile);
            this.tabLandscape.Controls.Add(this.txbLandVegInputFile);
            this.tabLandscape.Controls.Add(this.txbLandMaxRow);
            this.tabLandscape.Controls.Add(this.label37);
            this.tabLandscape.Location = new System.Drawing.Point(4, 22);
            this.tabLandscape.Name = "tabLandscape";
            this.tabLandscape.Size = new System.Drawing.Size(407, 654);
            this.tabLandscape.TabIndex = 1;
            this.tabLandscape.Text = "Landscape";
            // 
            // txbLandYearOfVegAndAgeFiles
            // 
            this.txbLandYearOfVegAndAgeFiles.Location = new System.Drawing.Point(293, 249);
            this.txbLandYearOfVegAndAgeFiles.Name = "txbLandYearOfVegAndAgeFiles";
            this.txbLandYearOfVegAndAgeFiles.Size = new System.Drawing.Size(54, 20);
            this.txbLandYearOfVegAndAgeFiles.TabIndex = 28;
            // 
            // chkLandIsUsingUniqueVegAndAgePerRep
            // 
            this.chkLandIsUsingUniqueVegAndAgePerRep.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.chkLandIsUsingUniqueVegAndAgePerRep.Location = new System.Drawing.Point(3, 236);
            this.chkLandIsUsingUniqueVegAndAgePerRep.Name = "chkLandIsUsingUniqueVegAndAgePerRep";
            this.chkLandIsUsingUniqueVegAndAgePerRep.Size = new System.Drawing.Size(142, 46);
            this.chkLandIsUsingUniqueVegAndAgePerRep.TabIndex = 27;
            this.chkLandIsUsingUniqueVegAndAgePerRep.Text = "Use Different Veg And Age Files For Each Rep?";
            this.chkLandIsUsingUniqueVegAndAgePerRep.CheckedChanged += new System.EventHandler(this.chkLandIsUsingUniqueVegAndAgePerRep_CheckedChanged);
            // 
            // chkLandIsForcedVegTransitions
            // 
            this.chkLandIsForcedVegTransitions.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.chkLandIsForcedVegTransitions.Location = new System.Drawing.Point(3, 306);
            this.chkLandIsForcedVegTransitions.Name = "chkLandIsForcedVegTransitions";
            this.chkLandIsForcedVegTransitions.Size = new System.Drawing.Size(142, 32);
            this.chkLandIsForcedVegTransitions.TabIndex = 26;
            this.chkLandIsForcedVegTransitions.Text = "Use Forced Veg Transitions?";
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.Location = new System.Drawing.Point(353, 285);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(24, 16);
            this.button1.TabIndex = 24;
            this.button1.Text = "...";
            this.button1.Click += new System.EventHandler(this.btnLandVegTransitionInputFileLookup_Click);
            // 
            // label109
            // 
            this.label109.Location = new System.Drawing.Point(4, 287);
            this.label109.Name = "label109";
            this.label109.Size = new System.Drawing.Size(92, 16);
            this.label109.TabIndex = 25;
            this.label109.Text = "Veg Transition File";
            // 
            // txbLandVegTransitionFile
            // 
            this.txbLandVegTransitionFile.Location = new System.Drawing.Point(128, 283);
            this.txbLandVegTransitionFile.Name = "txbLandVegTransitionFile";
            this.txbLandVegTransitionFile.Size = new System.Drawing.Size(219, 20);
            this.txbLandVegTransitionFile.TabIndex = 23;
            // 
            // btnLandTreeDensityInputFileLookup
            // 
            this.btnLandTreeDensityInputFileLookup.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLandTreeDensityInputFileLookup.Location = new System.Drawing.Point(353, 372);
            this.btnLandTreeDensityInputFileLookup.Name = "btnLandTreeDensityInputFileLookup";
            this.btnLandTreeDensityInputFileLookup.Size = new System.Drawing.Size(24, 16);
            this.btnLandTreeDensityInputFileLookup.TabIndex = 14;
            this.btnLandTreeDensityInputFileLookup.Text = "...";
            this.btnLandTreeDensityInputFileLookup.Click += new System.EventHandler(this.btnLandTreeDensityInputFileLookup_Click);
            // 
            // btnLandSiteInputFileLookup
            // 
            this.btnLandSiteInputFileLookup.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLandSiteInputFileLookup.Location = new System.Drawing.Point(353, 346);
            this.btnLandSiteInputFileLookup.Name = "btnLandSiteInputFileLookup";
            this.btnLandSiteInputFileLookup.Size = new System.Drawing.Size(24, 16);
            this.btnLandSiteInputFileLookup.TabIndex = 12;
            this.btnLandSiteInputFileLookup.Text = "...";
            this.btnLandSiteInputFileLookup.Click += new System.EventHandler(this.btnLandSiteInputFileLookup_Click);
            // 
            // btnLandAgeInputFileLookup
            // 
            this.btnLandAgeInputFileLookup.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLandAgeInputFileLookup.Location = new System.Drawing.Point(353, 193);
            this.btnLandAgeInputFileLookup.Name = "btnLandAgeInputFileLookup";
            this.btnLandAgeInputFileLookup.Size = new System.Drawing.Size(24, 16);
            this.btnLandAgeInputFileLookup.TabIndex = 10;
            this.btnLandAgeInputFileLookup.Text = "...";
            this.btnLandAgeInputFileLookup.Click += new System.EventHandler(this.btnLandAgeInputFileLookup_Click);
            // 
            // btnLandVegInputFileLookup
            // 
            this.btnLandVegInputFileLookup.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLandVegInputFileLookup.Location = new System.Drawing.Point(353, 217);
            this.btnLandVegInputFileLookup.Name = "btnLandVegInputFileLookup";
            this.btnLandVegInputFileLookup.Size = new System.Drawing.Size(24, 16);
            this.btnLandVegInputFileLookup.TabIndex = 8;
            this.btnLandVegInputFileLookup.Text = "...";
            this.btnLandVegInputFileLookup.Click += new System.EventHandler(this.btnLandVegInputFileLookup_Click);
            // 
            // label21
            // 
            this.label21.Location = new System.Drawing.Point(4, 374);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(120, 16);
            this.label21.TabIndex = 22;
            this.label21.Text = "Tree Density Input File";
            // 
            // label19
            // 
            this.label19.Location = new System.Drawing.Point(4, 348);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(92, 16);
            this.label19.TabIndex = 21;
            this.label19.Text = "Site Input File";
            // 
            // label18
            // 
            this.label18.Location = new System.Drawing.Point(4, 195);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(92, 16);
            this.label18.TabIndex = 20;
            this.label18.Text = "Age Input File";
            // 
            // label17
            // 
            this.label17.Location = new System.Drawing.Point(4, 219);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(92, 16);
            this.label17.TabIndex = 19;
            this.label17.Text = "Veg Input File";
            // 
            // label16
            // 
            this.label16.Location = new System.Drawing.Point(4, 160);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(92, 16);
            this.label16.TabIndex = 18;
            this.label16.Text = "YLL Corner";
            // 
            // label15
            // 
            this.label15.Location = new System.Drawing.Point(4, 136);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(92, 16);
            this.label15.TabIndex = 17;
            this.label15.Text = "XLL Corner";
            // 
            // label14
            // 
            this.label14.Location = new System.Drawing.Point(4, 112);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(92, 16);
            this.label14.TabIndex = 16;
            this.label14.Text = "No Veg";
            // 
            // label13
            // 
            this.label13.Location = new System.Drawing.Point(4, 88);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(92, 16);
            this.label13.TabIndex = 15;
            this.label13.Text = "Num Species";
            // 
            // label12
            // 
            this.label12.Location = new System.Drawing.Point(4, 64);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(92, 16);
            this.label12.TabIndex = 14;
            this.label12.Text = "Cell Size";
            // 
            // label11
            // 
            this.label11.Location = new System.Drawing.Point(4, 40);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(92, 16);
            this.label11.TabIndex = 13;
            this.label11.Text = "Max Columns";
            // 
            // label10
            // 
            this.label10.Location = new System.Drawing.Point(4, 16);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(92, 16);
            this.label10.TabIndex = 12;
            this.label10.Text = "Max Rows";
            // 
            // chkLandCropNeighbors
            // 
            this.chkLandCropNeighbors.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.chkLandCropNeighbors.Location = new System.Drawing.Point(3, 396);
            this.chkLandCropNeighbors.Name = "chkLandCropNeighbors";
            this.chkLandCropNeighbors.Size = new System.Drawing.Size(140, 24);
            this.chkLandCropNeighbors.TabIndex = 16;
            this.chkLandCropNeighbors.Text = "Crop Neighbors?";
            // 
            // txbLandMaxCol
            // 
            this.txbLandMaxCol.Location = new System.Drawing.Point(128, 36);
            this.txbLandMaxCol.Name = "txbLandMaxCol";
            this.txbLandMaxCol.Size = new System.Drawing.Size(54, 20);
            this.txbLandMaxCol.TabIndex = 1;
            // 
            // txbLandCellSize
            // 
            this.txbLandCellSize.Location = new System.Drawing.Point(128, 60);
            this.txbLandCellSize.Name = "txbLandCellSize";
            this.txbLandCellSize.Size = new System.Drawing.Size(54, 20);
            this.txbLandCellSize.TabIndex = 2;
            // 
            // txbLandNumSpecies
            // 
            this.txbLandNumSpecies.Location = new System.Drawing.Point(128, 84);
            this.txbLandNumSpecies.Name = "txbLandNumSpecies";
            this.txbLandNumSpecies.Size = new System.Drawing.Size(54, 20);
            this.txbLandNumSpecies.TabIndex = 3;
            // 
            // txbLandNoVeg
            // 
            this.txbLandNoVeg.Location = new System.Drawing.Point(128, 108);
            this.txbLandNoVeg.Name = "txbLandNoVeg";
            this.txbLandNoVeg.Size = new System.Drawing.Size(54, 20);
            this.txbLandNoVeg.TabIndex = 4;
            // 
            // txbLandXllCorner
            // 
            this.txbLandXllCorner.Location = new System.Drawing.Point(128, 132);
            this.txbLandXllCorner.Name = "txbLandXllCorner";
            this.txbLandXllCorner.Size = new System.Drawing.Size(120, 20);
            this.txbLandXllCorner.TabIndex = 5;
            // 
            // txbLandYllCorner
            // 
            this.txbLandYllCorner.Location = new System.Drawing.Point(128, 156);
            this.txbLandYllCorner.Name = "txbLandYllCorner";
            this.txbLandYllCorner.Size = new System.Drawing.Size(120, 20);
            this.txbLandYllCorner.TabIndex = 6;
            // 
            // txbLandSiteInputFile
            // 
            this.txbLandSiteInputFile.Location = new System.Drawing.Point(128, 344);
            this.txbLandSiteInputFile.Name = "txbLandSiteInputFile";
            this.txbLandSiteInputFile.Size = new System.Drawing.Size(219, 20);
            this.txbLandSiteInputFile.TabIndex = 11;
            // 
            // txbLandAgeInputFile
            // 
            this.txbLandAgeInputFile.Location = new System.Drawing.Point(128, 191);
            this.txbLandAgeInputFile.Name = "txbLandAgeInputFile";
            this.txbLandAgeInputFile.Size = new System.Drawing.Size(219, 20);
            this.txbLandAgeInputFile.TabIndex = 9;
            // 
            // txbLandTreeDensityInputFile
            // 
            this.txbLandTreeDensityInputFile.Location = new System.Drawing.Point(128, 370);
            this.txbLandTreeDensityInputFile.Name = "txbLandTreeDensityInputFile";
            this.txbLandTreeDensityInputFile.Size = new System.Drawing.Size(219, 20);
            this.txbLandTreeDensityInputFile.TabIndex = 13;
            // 
            // txbLandVegInputFile
            // 
            this.txbLandVegInputFile.Location = new System.Drawing.Point(128, 215);
            this.txbLandVegInputFile.Name = "txbLandVegInputFile";
            this.txbLandVegInputFile.Size = new System.Drawing.Size(219, 20);
            this.txbLandVegInputFile.TabIndex = 7;
            // 
            // txbLandMaxRow
            // 
            this.txbLandMaxRow.Location = new System.Drawing.Point(128, 12);
            this.txbLandMaxRow.Name = "txbLandMaxRow";
            this.txbLandMaxRow.Size = new System.Drawing.Size(54, 20);
            this.txbLandMaxRow.TabIndex = 0;
            // 
            // label37
            // 
            this.label37.Location = new System.Drawing.Point(184, 252);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(114, 16);
            this.label37.TabIndex = 29;
            this.label37.Text = "Year of Spin-up Files";
            // 
            // pnlSpecies
            // 
            this.pnlSpecies.Controls.Add(this.cmbSpecies);
            this.pnlSpecies.Controls.Add(this.label69);
            this.pnlSpecies.Controls.Add(this.pnlDecid);
            this.pnlSpecies.Controls.Add(this.pnlBSpruce);
            this.pnlSpecies.Controls.Add(this.pnlTundra);
            this.pnlSpecies.Controls.Add(this.pnlWSpruce);
            this.pnlSpecies.Location = new System.Drawing.Point(4, 22);
            this.pnlSpecies.Name = "pnlSpecies";
            this.pnlSpecies.Size = new System.Drawing.Size(407, 654);
            this.pnlSpecies.TabIndex = 6;
            this.pnlSpecies.Text = "Species";
            // 
            // cmbSpecies
            // 
            this.cmbSpecies.FormattingEnabled = true;
            this.cmbSpecies.Items.AddRange(new object[] {
            "Tundra",
            "Black Spruce",
            "White Spruce",
            "Deciduous"});
            this.cmbSpecies.Location = new System.Drawing.Point(136, 12);
            this.cmbSpecies.Name = "cmbSpecies";
            this.cmbSpecies.Size = new System.Drawing.Size(92, 21);
            this.cmbSpecies.TabIndex = 0;
            this.cmbSpecies.SelectionChangeCommitted += new System.EventHandler(this.cmbSpecies_SelectionChanged);
            // 
            // label69
            // 
            this.label69.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label69.Location = new System.Drawing.Point(8, 16);
            this.label69.Name = "label69";
            this.label69.Size = new System.Drawing.Size(124, 16);
            this.label69.TabIndex = 0;
            this.label69.Text = "Pick a species to edit :";
            // 
            // pnlDecid
            // 
            this.pnlDecid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlDecid.Controls.Add(this.txbDecidHumanFireProb);
            this.pnlDecid.Controls.Add(this.label98);
            this.pnlDecid.Controls.Add(this.txbDecidWSpruceParms);
            this.pnlDecid.Controls.Add(this.txbDecidBSpruceParms);
            this.pnlDecid.Controls.Add(this.txbDecidTundraParms);
            this.pnlDecid.Controls.Add(this.txbDecidHistory);
            this.pnlDecid.Controls.Add(this.txbDecidFireProb);
            this.pnlDecid.Controls.Add(this.txbDecidStartAgeBSpruce);
            this.pnlDecid.Controls.Add(this.txbDecidStartAgeWSpruce);
            this.pnlDecid.Controls.Add(this.label89);
            this.pnlDecid.Controls.Add(this.label88);
            this.pnlDecid.Controls.Add(this.label87);
            this.pnlDecid.Controls.Add(this.label86);
            this.pnlDecid.Controls.Add(this.label85);
            this.pnlDecid.Controls.Add(this.label81);
            this.pnlDecid.Controls.Add(this.label71);
            this.pnlDecid.Location = new System.Drawing.Point(4, 40);
            this.pnlDecid.Name = "pnlDecid";
            this.pnlDecid.Size = new System.Drawing.Size(283, 610);
            this.pnlDecid.TabIndex = 5;
            // 
            // txbDecidHumanFireProb
            // 
            this.txbDecidHumanFireProb.Location = new System.Drawing.Point(172, 88);
            this.txbDecidHumanFireProb.Name = "txbDecidHumanFireProb";
            this.txbDecidHumanFireProb.Size = new System.Drawing.Size(72, 20);
            this.txbDecidHumanFireProb.TabIndex = 9;
            // 
            // label98
            // 
            this.label98.Location = new System.Drawing.Point(8, 92);
            this.label98.Name = "label98";
            this.label98.Size = new System.Drawing.Size(164, 16);
            this.label98.TabIndex = 8;
            this.label98.Text = "Human Fire Probability";
            // 
            // txbDecidWSpruceParms
            // 
            this.txbDecidWSpruceParms.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbDecidWSpruceParms.Location = new System.Drawing.Point(172, 184);
            this.txbDecidWSpruceParms.Name = "txbDecidWSpruceParms";
            this.txbDecidWSpruceParms.Size = new System.Drawing.Size(103, 20);
            this.txbDecidWSpruceParms.TabIndex = 7;
            // 
            // txbDecidBSpruceParms
            // 
            this.txbDecidBSpruceParms.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbDecidBSpruceParms.Location = new System.Drawing.Point(172, 160);
            this.txbDecidBSpruceParms.Name = "txbDecidBSpruceParms";
            this.txbDecidBSpruceParms.Size = new System.Drawing.Size(103, 20);
            this.txbDecidBSpruceParms.TabIndex = 6;
            // 
            // txbDecidTundraParms
            // 
            this.txbDecidTundraParms.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbDecidTundraParms.Location = new System.Drawing.Point(172, 136);
            this.txbDecidTundraParms.Name = "txbDecidTundraParms";
            this.txbDecidTundraParms.Size = new System.Drawing.Size(103, 20);
            this.txbDecidTundraParms.TabIndex = 5;
            // 
            // txbDecidHistory
            // 
            this.txbDecidHistory.Location = new System.Drawing.Point(172, 112);
            this.txbDecidHistory.Name = "txbDecidHistory";
            this.txbDecidHistory.Size = new System.Drawing.Size(56, 20);
            this.txbDecidHistory.TabIndex = 4;
            // 
            // txbDecidFireProb
            // 
            this.txbDecidFireProb.Location = new System.Drawing.Point(172, 64);
            this.txbDecidFireProb.Name = "txbDecidFireProb";
            this.txbDecidFireProb.Size = new System.Drawing.Size(72, 20);
            this.txbDecidFireProb.TabIndex = 3;
            // 
            // txbDecidStartAgeBSpruce
            // 
            this.txbDecidStartAgeBSpruce.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbDecidStartAgeBSpruce.Location = new System.Drawing.Point(172, 40);
            this.txbDecidStartAgeBSpruce.Name = "txbDecidStartAgeBSpruce";
            this.txbDecidStartAgeBSpruce.Size = new System.Drawing.Size(103, 20);
            this.txbDecidStartAgeBSpruce.TabIndex = 2;
            // 
            // txbDecidStartAgeWSpruce
            // 
            this.txbDecidStartAgeWSpruce.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbDecidStartAgeWSpruce.Location = new System.Drawing.Point(172, 16);
            this.txbDecidStartAgeWSpruce.Name = "txbDecidStartAgeWSpruce";
            this.txbDecidStartAgeWSpruce.Size = new System.Drawing.Size(103, 20);
            this.txbDecidStartAgeWSpruce.TabIndex = 1;
            // 
            // label89
            // 
            this.label89.Location = new System.Drawing.Point(8, 188);
            this.label89.Name = "label89";
            this.label89.Size = new System.Drawing.Size(164, 16);
            this.label89.TabIndex = 6;
            this.label89.Text = "WSpruce Parameters";
            // 
            // label88
            // 
            this.label88.Location = new System.Drawing.Point(8, 164);
            this.label88.Name = "label88";
            this.label88.Size = new System.Drawing.Size(164, 16);
            this.label88.TabIndex = 5;
            this.label88.Text = "BSpruce Parameters";
            // 
            // label87
            // 
            this.label87.Location = new System.Drawing.Point(8, 140);
            this.label87.Name = "label87";
            this.label87.Size = new System.Drawing.Size(164, 16);
            this.label87.TabIndex = 4;
            this.label87.Text = "Tundra Parameters";
            // 
            // label86
            // 
            this.label86.Location = new System.Drawing.Point(8, 116);
            this.label86.Name = "label86";
            this.label86.Size = new System.Drawing.Size(164, 16);
            this.label86.TabIndex = 3;
            this.label86.Text = "History";
            // 
            // label85
            // 
            this.label85.Location = new System.Drawing.Point(8, 68);
            this.label85.Name = "label85";
            this.label85.Size = new System.Drawing.Size(164, 16);
            this.label85.TabIndex = 2;
            this.label85.Text = "Fire Probability";
            // 
            // label81
            // 
            this.label81.Location = new System.Drawing.Point(8, 44);
            this.label81.Name = "label81";
            this.label81.Size = new System.Drawing.Size(164, 16);
            this.label81.TabIndex = 1;
            this.label81.Text = "BSpruce Start Age Parameters";
            // 
            // label71
            // 
            this.label71.Location = new System.Drawing.Point(8, 20);
            this.label71.Name = "label71";
            this.label71.Size = new System.Drawing.Size(164, 16);
            this.label71.TabIndex = 0;
            this.label71.Text = "WSpruce Start Age Parameters";
            // 
            // pnlBSpruce
            // 
            this.pnlBSpruce.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlBSpruce.Controls.Add(this.txbBSpruceHumanFireProb);
            this.pnlBSpruce.Controls.Add(this.label101);
            this.pnlBSpruce.Controls.Add(this.txbBSpruceFireParms);
            this.pnlBSpruce.Controls.Add(this.txbBSpruceStartAge);
            this.pnlBSpruce.Controls.Add(this.label92);
            this.pnlBSpruce.Controls.Add(this.label93);
            this.pnlBSpruce.Location = new System.Drawing.Point(4, 40);
            this.pnlBSpruce.Name = "pnlBSpruce";
            this.pnlBSpruce.Size = new System.Drawing.Size(283, 610);
            this.pnlBSpruce.TabIndex = 3;
            // 
            // txbBSpruceHumanFireProb
            // 
            this.txbBSpruceHumanFireProb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbBSpruceHumanFireProb.Location = new System.Drawing.Point(132, 68);
            this.txbBSpruceHumanFireProb.Name = "txbBSpruceHumanFireProb";
            this.txbBSpruceHumanFireProb.Size = new System.Drawing.Size(35, 20);
            this.txbBSpruceHumanFireProb.TabIndex = 9;
            // 
            // label101
            // 
            this.label101.Location = new System.Drawing.Point(8, 72);
            this.label101.Name = "label101";
            this.label101.Size = new System.Drawing.Size(116, 16);
            this.label101.TabIndex = 8;
            this.label101.Text = "Huma Fire Probability";
            // 
            // txbBSpruceFireParms
            // 
            this.txbBSpruceFireParms.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbBSpruceFireParms.Location = new System.Drawing.Point(132, 44);
            this.txbBSpruceFireParms.Name = "txbBSpruceFireParms";
            this.txbBSpruceFireParms.Size = new System.Drawing.Size(131, 20);
            this.txbBSpruceFireParms.TabIndex = 7;
            // 
            // txbBSpruceStartAge
            // 
            this.txbBSpruceStartAge.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbBSpruceStartAge.Location = new System.Drawing.Point(132, 20);
            this.txbBSpruceStartAge.Name = "txbBSpruceStartAge";
            this.txbBSpruceStartAge.Size = new System.Drawing.Size(131, 20);
            this.txbBSpruceStartAge.TabIndex = 6;
            // 
            // label92
            // 
            this.label92.Location = new System.Drawing.Point(8, 48);
            this.label92.Name = "label92";
            this.label92.Size = new System.Drawing.Size(116, 16);
            this.label92.TabIndex = 5;
            this.label92.Text = "Fire Parameters";
            // 
            // label93
            // 
            this.label93.Location = new System.Drawing.Point(8, 24);
            this.label93.Name = "label93";
            this.label93.Size = new System.Drawing.Size(116, 16);
            this.label93.TabIndex = 4;
            this.label93.Text = "Start Age Parameters";
            // 
            // pnlTundra
            // 
            this.pnlTundra.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlTundra.Controls.Add(this.txbTundraHumanFireProb);
            this.pnlTundra.Controls.Add(this.label84);
            this.pnlTundra.Controls.Add(this.label83);
            this.pnlTundra.Controls.Add(this.label82);
            this.pnlTundra.Controls.Add(this.label80);
            this.pnlTundra.Controls.Add(this.label79);
            this.pnlTundra.Controls.Add(this.label78);
            this.pnlTundra.Controls.Add(this.label77);
            this.pnlTundra.Controls.Add(this.label76);
            this.pnlTundra.Controls.Add(this.label75);
            this.pnlTundra.Controls.Add(this.label74);
            this.pnlTundra.Controls.Add(this.label73);
            this.pnlTundra.Controls.Add(this.label72);
            this.pnlTundra.Controls.Add(this.label500);
            this.pnlTundra.Controls.Add(this.txbTundraSpruceBasalArea);
            this.pnlTundra.Controls.Add(this.txbTundraCalFactor);
            this.pnlTundra.Controls.Add(this.txbTundraClimGrowth);
            this.pnlTundra.Controls.Add(this.txbTundraMeanGrowth);
            this.pnlTundra.Controls.Add(this.txbTundraSeedEstParms);
            this.pnlTundra.Controls.Add(this.txbTundraHistory);
            this.pnlTundra.Controls.Add(this.txbTundraSeedlingBA);
            this.pnlTundra.Controls.Add(this.txbTundraSeedling);
            this.pnlTundra.Controls.Add(this.txbTundraSeedBasalArea);
            this.pnlTundra.Controls.Add(this.txbTundraSeedSource);
            this.pnlTundra.Controls.Add(this.txbTundraSeedRange);
            this.pnlTundra.Controls.Add(this.txbTundraSpruceEstBA);
            this.pnlTundra.Controls.Add(this.txbTundraFireProb);
            this.pnlTundra.Controls.Add(this.txbTundraStartAge);
            this.pnlTundra.Controls.Add(this.label70);
            this.pnlTundra.Controls.Add(this.label99);
            this.pnlTundra.Location = new System.Drawing.Point(4, 40);
            this.pnlTundra.Name = "pnlTundra";
            this.pnlTundra.Size = new System.Drawing.Size(283, 610);
            this.pnlTundra.TabIndex = 2;
            // 
            // txbTundraHumanFireProb
            // 
            this.txbTundraHumanFireProb.Location = new System.Drawing.Point(120, 56);
            this.txbTundraHumanFireProb.Name = "txbTundraHumanFireProb";
            this.txbTundraHumanFireProb.Size = new System.Drawing.Size(76, 20);
            this.txbTundraHumanFireProb.TabIndex = 29;
            // 
            // label84
            // 
            this.label84.Location = new System.Drawing.Point(4, 356);
            this.label84.Name = "label84";
            this.label84.Size = new System.Drawing.Size(116, 16);
            this.label84.TabIndex = 28;
            this.label84.Text = "Spruce Basal Area";
            // 
            // label83
            // 
            this.label83.Location = new System.Drawing.Point(4, 328);
            this.label83.Name = "label83";
            this.label83.Size = new System.Drawing.Size(116, 16);
            this.label83.TabIndex = 27;
            this.label83.Text = "Cal Factor";
            // 
            // label82
            // 
            this.label82.Location = new System.Drawing.Point(4, 304);
            this.label82.Name = "label82";
            this.label82.Size = new System.Drawing.Size(116, 16);
            this.label82.TabIndex = 26;
            this.label82.Text = "Climate Growth";
            // 
            // label80
            // 
            this.label80.Location = new System.Drawing.Point(4, 276);
            this.label80.Name = "label80";
            this.label80.Size = new System.Drawing.Size(116, 16);
            this.label80.TabIndex = 24;
            this.label80.Text = "Mean Growth";
            // 
            // label79
            // 
            this.label79.Location = new System.Drawing.Point(4, 252);
            this.label79.Name = "label79";
            this.label79.Size = new System.Drawing.Size(116, 16);
            this.label79.TabIndex = 23;
            this.label79.Text = "Seed Est Parms";
            // 
            // label78
            // 
            this.label78.Location = new System.Drawing.Point(4, 228);
            this.label78.Name = "label78";
            this.label78.Size = new System.Drawing.Size(116, 16);
            this.label78.TabIndex = 22;
            this.label78.Text = "History";
            // 
            // label77
            // 
            this.label77.Location = new System.Drawing.Point(4, 204);
            this.label77.Name = "label77";
            this.label77.Size = new System.Drawing.Size(116, 16);
            this.label77.TabIndex = 21;
            this.label77.Text = "Seedling BA";
            // 
            // label76
            // 
            this.label76.Location = new System.Drawing.Point(4, 180);
            this.label76.Name = "label76";
            this.label76.Size = new System.Drawing.Size(116, 16);
            this.label76.TabIndex = 20;
            this.label76.Text = "Seedling";
            // 
            // label75
            // 
            this.label75.Location = new System.Drawing.Point(4, 156);
            this.label75.Name = "label75";
            this.label75.Size = new System.Drawing.Size(116, 16);
            this.label75.TabIndex = 19;
            this.label75.Text = "Seed Basal Area";
            // 
            // label74
            // 
            this.label74.Location = new System.Drawing.Point(4, 132);
            this.label74.Name = "label74";
            this.label74.Size = new System.Drawing.Size(116, 16);
            this.label74.TabIndex = 18;
            this.label74.Text = "Seed Source";
            // 
            // label73
            // 
            this.label73.Location = new System.Drawing.Point(4, 108);
            this.label73.Name = "label73";
            this.label73.Size = new System.Drawing.Size(116, 16);
            this.label73.TabIndex = 17;
            this.label73.Text = "Seed Range";
            // 
            // label72
            // 
            this.label72.Location = new System.Drawing.Point(4, 84);
            this.label72.Name = "label72";
            this.label72.Size = new System.Drawing.Size(116, 16);
            this.label72.TabIndex = 16;
            this.label72.Text = "Spruce Estimated BA";
            // 
            // label500
            // 
            this.label500.Location = new System.Drawing.Point(4, 36);
            this.label500.Name = "label500";
            this.label500.Size = new System.Drawing.Size(116, 16);
            this.label500.TabIndex = 15;
            this.label500.Text = "Fire Prob";
            // 
            // txbTundraSpruceBasalArea
            // 
            this.txbTundraSpruceBasalArea.Location = new System.Drawing.Point(120, 352);
            this.txbTundraSpruceBasalArea.Name = "txbTundraSpruceBasalArea";
            this.txbTundraSpruceBasalArea.Size = new System.Drawing.Size(76, 20);
            this.txbTundraSpruceBasalArea.TabIndex = 14;
            // 
            // txbTundraCalFactor
            // 
            this.txbTundraCalFactor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbTundraCalFactor.Location = new System.Drawing.Point(120, 324);
            this.txbTundraCalFactor.Name = "txbTundraCalFactor";
            this.txbTundraCalFactor.Size = new System.Drawing.Size(143, 20);
            this.txbTundraCalFactor.TabIndex = 13;
            // 
            // txbTundraClimGrowth
            // 
            this.txbTundraClimGrowth.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbTundraClimGrowth.Location = new System.Drawing.Point(120, 296);
            this.txbTundraClimGrowth.Name = "txbTundraClimGrowth";
            this.txbTundraClimGrowth.Size = new System.Drawing.Size(143, 20);
            this.txbTundraClimGrowth.TabIndex = 12;
            // 
            // txbTundraMeanGrowth
            // 
            this.txbTundraMeanGrowth.Location = new System.Drawing.Point(120, 272);
            this.txbTundraMeanGrowth.Name = "txbTundraMeanGrowth";
            this.txbTundraMeanGrowth.Size = new System.Drawing.Size(76, 20);
            this.txbTundraMeanGrowth.TabIndex = 11;
            // 
            // txbTundraSeedEstParms
            // 
            this.txbTundraSeedEstParms.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbTundraSeedEstParms.Location = new System.Drawing.Point(120, 248);
            this.txbTundraSeedEstParms.Name = "txbTundraSeedEstParms";
            this.txbTundraSeedEstParms.Size = new System.Drawing.Size(143, 20);
            this.txbTundraSeedEstParms.TabIndex = 10;
            // 
            // txbTundraHistory
            // 
            this.txbTundraHistory.Location = new System.Drawing.Point(120, 224);
            this.txbTundraHistory.Name = "txbTundraHistory";
            this.txbTundraHistory.Size = new System.Drawing.Size(60, 20);
            this.txbTundraHistory.TabIndex = 9;
            // 
            // txbTundraSeedlingBA
            // 
            this.txbTundraSeedlingBA.Location = new System.Drawing.Point(120, 200);
            this.txbTundraSeedlingBA.Name = "txbTundraSeedlingBA";
            this.txbTundraSeedlingBA.Size = new System.Drawing.Size(76, 20);
            this.txbTundraSeedlingBA.TabIndex = 8;
            // 
            // txbTundraSeedling
            // 
            this.txbTundraSeedling.Location = new System.Drawing.Point(120, 176);
            this.txbTundraSeedling.Name = "txbTundraSeedling";
            this.txbTundraSeedling.Size = new System.Drawing.Size(76, 20);
            this.txbTundraSeedling.TabIndex = 7;
            // 
            // txbTundraSeedBasalArea
            // 
            this.txbTundraSeedBasalArea.Location = new System.Drawing.Point(120, 152);
            this.txbTundraSeedBasalArea.Name = "txbTundraSeedBasalArea";
            this.txbTundraSeedBasalArea.Size = new System.Drawing.Size(76, 20);
            this.txbTundraSeedBasalArea.TabIndex = 6;
            // 
            // txbTundraSeedSource
            // 
            this.txbTundraSeedSource.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbTundraSeedSource.Location = new System.Drawing.Point(120, 128);
            this.txbTundraSeedSource.Name = "txbTundraSeedSource";
            this.txbTundraSeedSource.Size = new System.Drawing.Size(143, 20);
            this.txbTundraSeedSource.TabIndex = 5;
            // 
            // txbTundraSeedRange
            // 
            this.txbTundraSeedRange.Location = new System.Drawing.Point(120, 104);
            this.txbTundraSeedRange.Name = "txbTundraSeedRange";
            this.txbTundraSeedRange.Size = new System.Drawing.Size(76, 20);
            this.txbTundraSeedRange.TabIndex = 4;
            // 
            // txbTundraSpruceEstBA
            // 
            this.txbTundraSpruceEstBA.Location = new System.Drawing.Point(120, 80);
            this.txbTundraSpruceEstBA.Name = "txbTundraSpruceEstBA";
            this.txbTundraSpruceEstBA.Size = new System.Drawing.Size(76, 20);
            this.txbTundraSpruceEstBA.TabIndex = 3;
            // 
            // txbTundraFireProb
            // 
            this.txbTundraFireProb.Location = new System.Drawing.Point(120, 32);
            this.txbTundraFireProb.Name = "txbTundraFireProb";
            this.txbTundraFireProb.Size = new System.Drawing.Size(76, 20);
            this.txbTundraFireProb.TabIndex = 2;
            // 
            // txbTundraStartAge
            // 
            this.txbTundraStartAge.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbTundraStartAge.Location = new System.Drawing.Point(120, 8);
            this.txbTundraStartAge.Name = "txbTundraStartAge";
            this.txbTundraStartAge.Size = new System.Drawing.Size(143, 20);
            this.txbTundraStartAge.TabIndex = 1;
            // 
            // label70
            // 
            this.label70.Location = new System.Drawing.Point(4, 12);
            this.label70.Name = "label70";
            this.label70.Size = new System.Drawing.Size(116, 16);
            this.label70.TabIndex = 0;
            this.label70.Text = "Start Age Parameters";
            // 
            // label99
            // 
            this.label99.Location = new System.Drawing.Point(4, 60);
            this.label99.Name = "label99";
            this.label99.Size = new System.Drawing.Size(120, 16);
            this.label99.TabIndex = 30;
            this.label99.Text = "Human Fire Probability";
            // 
            // pnlWSpruce
            // 
            this.pnlWSpruce.Controls.Add(this.txbWSpruceHumanFireProb);
            this.pnlWSpruce.Controls.Add(this.label100);
            this.pnlWSpruce.Controls.Add(this.txbWSpruceFireParms);
            this.pnlWSpruce.Controls.Add(this.txbWSpruceStartAge);
            this.pnlWSpruce.Controls.Add(this.label91);
            this.pnlWSpruce.Controls.Add(this.label90);
            this.pnlWSpruce.Location = new System.Drawing.Point(4, 40);
            this.pnlWSpruce.Name = "pnlWSpruce";
            this.pnlWSpruce.Size = new System.Drawing.Size(268, 488);
            this.pnlWSpruce.TabIndex = 4;
            // 
            // txbWSpruceHumanFireProb
            // 
            this.txbWSpruceHumanFireProb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbWSpruceHumanFireProb.Location = new System.Drawing.Point(132, 68);
            this.txbWSpruceHumanFireProb.Name = "txbWSpruceHumanFireProb";
            this.txbWSpruceHumanFireProb.Size = new System.Drawing.Size(80, 20);
            this.txbWSpruceHumanFireProb.TabIndex = 5;
            // 
            // label100
            // 
            this.label100.Location = new System.Drawing.Point(8, 72);
            this.label100.Name = "label100";
            this.label100.Size = new System.Drawing.Size(116, 16);
            this.label100.TabIndex = 4;
            this.label100.Text = "Huma Fire Probability";
            // 
            // txbWSpruceFireParms
            // 
            this.txbWSpruceFireParms.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbWSpruceFireParms.Location = new System.Drawing.Point(132, 44);
            this.txbWSpruceFireParms.Name = "txbWSpruceFireParms";
            this.txbWSpruceFireParms.Size = new System.Drawing.Size(116, 20);
            this.txbWSpruceFireParms.TabIndex = 3;
            // 
            // txbWSpruceStartAge
            // 
            this.txbWSpruceStartAge.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbWSpruceStartAge.Location = new System.Drawing.Point(132, 20);
            this.txbWSpruceStartAge.Name = "txbWSpruceStartAge";
            this.txbWSpruceStartAge.Size = new System.Drawing.Size(116, 20);
            this.txbWSpruceStartAge.TabIndex = 2;
            // 
            // label91
            // 
            this.label91.Location = new System.Drawing.Point(8, 48);
            this.label91.Name = "label91";
            this.label91.Size = new System.Drawing.Size(116, 16);
            this.label91.TabIndex = 1;
            this.label91.Text = "Fire Parameters";
            // 
            // label90
            // 
            this.label90.Location = new System.Drawing.Point(8, 24);
            this.label90.Name = "label90";
            this.label90.Size = new System.Drawing.Size(116, 16);
            this.label90.TabIndex = 0;
            this.label90.Text = "Start Age Parameters";
            // 
            // EditorFIF
            // 
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(422, 710);
            this.CloseButton = false;
            this.Controls.Add(this.fldFileName);
            this.Controls.Add(this.lblFileName);
            this.Controls.Add(this.tabFIF);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(100, 300);
            this.Name = "EditorFIF";
            this.TabText = "FIF Editor";
            this.Text = "FIF Editor";
            this.Load += new System.EventHandler(this.FIFEditor_Load);
            this.tabFIF.ResumeLayout(false);
            this.tabGeneral.ResumeLayout(false);
            this.tabGeneral.PerformLayout();
            this.tabFire.ResumeLayout(false);
            this.tabFire.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabClimate.ResumeLayout(false);
            this.tabClimate.PerformLayout();
            this.grpClimateTransitions.ResumeLayout(false);
            this.grpClimateTransitions.PerformLayout();
            this.grpClimOffsets.ResumeLayout(false);
            this.pnlOffsetsConstant.ResumeLayout(false);
            this.pnlOffsetsConstant.PerformLayout();
            this.pnlOffsetsRandom.ResumeLayout(false);
            this.pnlOffsetsRandom.PerformLayout();
            this.pnlOffsetsFile.ResumeLayout(false);
            this.pnlOffsetsFile.PerformLayout();
            this.pnlClimRand.ResumeLayout(false);
            this.pnlClimRand.PerformLayout();
            this.pnlClimValues.ResumeLayout(false);
            this.pnlClimValues.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            this.tabStepsAndRamps.ResumeLayout(false);
            this.tabPageSteps.ResumeLayout(false);
            this.tabPageSteps.PerformLayout();
            this.tabPageRamps.ResumeLayout(false);
            this.tabPageRamps.PerformLayout();
            this.tabMapOutput.ResumeLayout(false);
            this.tabMapOutput.PerformLayout();
            this.tabStatOutput.ResumeLayout(false);
            this.tabStatOutput.PerformLayout();
            this.tabLandscape.ResumeLayout(false);
            this.tabLandscape.PerformLayout();
            this.pnlSpecies.ResumeLayout(false);
            this.pnlDecid.ResumeLayout(false);
            this.pnlDecid.PerformLayout();
            this.pnlBSpruce.ResumeLayout(false);
            this.pnlBSpruce.PerformLayout();
            this.pnlTundra.ResumeLayout(false);
            this.pnlTundra.PerformLayout();
            this.pnlWSpruce.ResumeLayout(false);
            this.pnlWSpruce.PerformLayout();
            this.ResumeLayout(false);

		}
		#endregion

		private Label fldFileName;

		#endregion
		private Label lblFileName;
		private TabControl tabFIF;
		private TabPage tabGeneral;
		private TextBox txbGenMaxYears;
		private Button btnBaseDirLookup;
		private CheckBox chkGenIsLargeMemoryModel;
		private Label label9;
		private ComboBox cbbGenOutputDetailLevel;
		private ComboBox cbbGenOutputType;
		private Label label8;
		private Label label__;
		private TextBox txbGenOutputDirectory;
		private Label label7;
		private Label label20;
		private Label label5;
		private Label label4;
		private TextBox txbGenTimeStep;
		private TextBox txbGenNumHeader;
		private TextBox txbGenNoData;
		private Label label3;
		private Label label2;
		private Label label1;
		private TextBox txbGenMaxReps;
		private TextBox txbGenBaseDirectory;
		private TabPage tabFire;
		private GroupBox groupBox4;
		private TextBox txbFireSuppressionClass2;
		private CheckBox chkFireSuppressionOn;
		private TextBox txbFireSuppressionClass1;
		private Label label106;
		private Label label105;
		private TextBox txbFireSuppressionFile;
		private Label label104;
		private Label label103;
		private Label label97;
		private Label label102;
		private Label label94;
		private TextBox txbFireSuppressionClass5;
		private TextBox txbFireSuppressionClass4;
		private TextBox txbFireSuppressionClass3;
		private TextBox txbFireSuppressionThresholdIgnitions;
		private Label label107;
		private TextBox txbFireSuppressionThresholdFireSize;
		private Label label108;
		private GroupBox groupBox2;
		private Label label42;
		private Button btnFireTransitionRemove;
		private TextBox txbFireTypeTransitionYear;
		private Label label95;
		private Button btnFireTransitionAdd;
		private ListBox lstFireTypeTransitions;
		private Label label60;
		private Label label63;
		private Label label52;
		private TextBox txbFireIgnition;
		private TextBox txbFireSpatialSensitivity;
		private TextBox txbFireSensitivity;
		private TextBox txbFireSpatialIgnition;
		private Label label47;
		private Label label46;
		private Label label44;
		private Label label43;
		private ComboBox cbbFireType;
		private Label label45;
		private TextBox txbFireHistorical;
		private GroupBox groupBox1;
		private TextBox txbFireClimateC;
		private TextBox txbFireClimateB;
		private TextBox txbFireClimateA;
		private Label label56;
		private Label label55;
		private Label label54;
		private Label label51;
		private TextBox txbFireSpreadRadius;
		private TextBox txbFireSpreadStdDev;
		private TextBox txbFireSpreadMean;
		private CheckBox chkFireIgnoreFirstInterval;
		private CheckBox chkFireEnabled;
		private Label label48;
		private Label label50;
		private Label label49;
		private Label label68;
		private TextBox txbFireHumanIgnitionFile;
        private TabPage tabClimate;
		private TextBox txbClimValuesTemp;
        private TextBox txbClimNumHistory;
		private Label label39;
		private Label label38;
        private ComboBox cbbClimValuesType;
        private TextBox txbClimOffsetsTempStdDev;
        private TextBox txbClimOffsetsTempConstant;
        private TextBox txbClimOffsetsTempMean;
		private Label label30;
        private Label label29;
		private TextBox txbClimOffsetsPrecipMean;
		private TextBox txbClimOffsetsPrecipStdDev;
        private TextBox txbClimOffsetsPrecipConstant;
        private ComboBox cbbClimOffsetsType;
        private Label label35;
		private TextBox txbClimOffsetsFile;
		private TabPage tabMapOutput;
		private CheckBox chkMapFireScar;
		private Button btnMapDelete;
		private CheckBox chkMapFrame4;
		private CheckBox chkMapFrame3;
		private CheckBox chkMapFrame2;
		private CheckBox chkMapFrame1;
		private CheckBox chkMapPrecip;
		private CheckBox chkMapTemp;
		private CheckBox chkMapFireAge;
		private CheckBox chkMapSub;
		private CheckBox chkMapSite;
		private CheckBox chkMapAge;
		private CheckBox chkMapVeg;
		private CheckBox chkMapARC;
		private CheckBox chkMapHeader;
		private CheckBox chkMapFormat;
		private CheckBox chkMapIsOutput;
		private ComboBox cbbMapOutputSelect;
		private Label label67;
		private Label label66;
		private Label label65;
		private Label label64;
		private Button btnMapAdd;
		private Label lblMapFlags;
		private TextBox txbMapYearFreq;
		private TextBox txbMapYearStart;
		private TextBox txbMapRepFreq;
		private TextBox txbMapRepStart;
		private TextBox txbMapCodes;
		private Label label61;
		private Label label59;
		private Label label58;
		private Label label57;
		private Label label53;
		private TabPage tabStatOutput;
		private CheckBox chkStatEvents;
		private CheckBox chkStatData;
		private CheckBox chkStatHist;
		private CheckBox chkStatMax;
		private CheckBox chkStatMin;
		private CheckBox chkStatStdDev;
		private CheckBox chkStatMean;
		private CheckBox chkStatNum;
		private CheckBox chkStatFormat;
		private CheckBox chkStatRep;
		private CheckBox chkStatYear;
		private ComboBox cbbStatSelect;
		private TextBox txbStatSummaryFile;
		private Label label62;
		private Label label96;
		private TabPage tabLandscape;
		private Button btnLandTreeDensityInputFileLookup;
		private Button btnLandSiteInputFileLookup;
		private Button btnLandAgeInputFileLookup;
		private Button btnLandVegInputFileLookup;
		private Label label21;
		private Label label19;
		private Label label18;
		private Label label17;
		private Label label16;
		private Label label15;
		private Label label14;
		private Label label13;
		private Label label12;
		private Label label11;
		private Label label10;
		private CheckBox chkLandCropNeighbors;
		private TextBox txbLandMaxCol;
		private TextBox txbLandCellSize;
		private TextBox txbLandNumSpecies;
		private TextBox txbLandNoVeg;
		private TextBox txbLandXllCorner;
		private TextBox txbLandYllCorner;
		private TextBox txbLandSiteInputFile;
		private TextBox txbLandAgeInputFile;
		private TextBox txbLandTreeDensityInputFile;
		private TextBox txbLandVegInputFile;
		private TextBox txbLandMaxRow;
		private TabPage pnlSpecies;
		private ComboBox cmbSpecies;
		private Label label69;
		private Panel pnlDecid;
		private TextBox txbDecidHumanFireProb;
		private Label label98;
		private TextBox txbDecidWSpruceParms;
		private TextBox txbDecidBSpruceParms;
		private TextBox txbDecidTundraParms;
		private TextBox txbDecidHistory;
		private TextBox txbDecidFireProb;
		private TextBox txbDecidStartAgeBSpruce;
		private TextBox txbDecidStartAgeWSpruce;
		private Label label89;
		private Label label88;
		private Label label87;
		private Label label86;
		private Label label85;
		private Label label81;
		private Label label71;
		private Panel pnlBSpruce;
		private TextBox txbBSpruceHumanFireProb;
		private Label label101;
		private TextBox txbBSpruceFireParms;
		private TextBox txbBSpruceStartAge;
		private Label label92;
		private Label label93;
		private Panel pnlTundra;
		private TextBox txbTundraHumanFireProb;
		private Label label84;
		private Label label83;
		private Label label82;
		private Label label80;
		private Label label79;
		private Label label78;
		private Label label77;
		private Label label76;
		private Label label75;
		private Label label74;
		private Label label73;
		private Label label72;
		private Label label500;
		private TextBox txbTundraSpruceBasalArea;
		private TextBox txbTundraCalFactor;
		private TextBox txbTundraClimGrowth;
		private TextBox txbTundraMeanGrowth;
		private TextBox txbTundraSeedEstParms;
		private TextBox txbTundraHistory;
		private TextBox txbTundraSeedlingBA;
		private TextBox txbTundraSeedling;
		private TextBox txbTundraSeedBasalArea;
		private TextBox txbTundraSeedSource;
		private TextBox txbTundraSeedRange;
		private TextBox txbTundraSpruceEstBA;
		private TextBox txbTundraFireProb;
		private TextBox txbTundraStartAge;
		private Label label70;
		private Label label99;
		private Panel pnlWSpruce;
		private TextBox txbWSpruceHumanFireProb;
		private Label label100;
		private TextBox txbWSpruceFireParms;
		private TextBox txbWSpruceStartAge;
		private Label label91;
		private Label label90;
		private ComboBox cbbGenRandSeed;
		private Button btnOutputDirLookup;
		private Button button1;
		private Label label109;
		private TextBox txbLandVegTransitionFile;
		private CheckBox chkLandIsForcedVegTransitions;
        private TextBox txbClimRandExplicitMaxYear;
        private TextBox txbClimRandExplicitMinYear;
        private Label label110;
        private Label label6;
        private GroupBox grpClimOffsets;
        private Panel pnlOffsetsRandom;
        private Panel pnlOffsetsConstant;
        private Label label112;
        private Panel pnlOffsetsFile;
        private Label label113;
        private Label label26;
        private Label label25;
        private Label label111;
        private CheckBox chkClimOffsetsRandIsReplicated;
        private Label lblPrecipValues;
        private Label lblTempValues;
        private CheckBox chkClimValuesRandExplicitIsReplicated;
        private Panel pnlClimValues;
        private Panel pnlClimRand;
        private Button btnClimTransitionRemove;
        private Button btnClimTransitionAdd;
        private ListBox lstClimTransitions;
        private TabControl tabStepsAndRamps;
        private TabPage tabPageRamps;
        private TextBox txbClimOffsetsPrecipNewRampYear;
        private Button btnRemovePrecipRamp;
        private ListBox lstClimPrecipRamps;
        private Button btnAddPrecipRamp;
        private Label label32;
        private Label label31;
        private TextBox txbClimOffsetsPrecipNewRampValue;
        private TextBox txbClimOffsetsTempNewRampYear;
        private Button btnAddTempRamp;
        private TextBox txbClimOffsetsTempNewRampValue;
        private Button btnRemoveTempRamp;
        private ListBox lstClimTempRamps;
        private Label label23;
        private Label label24;
        private TabPage tabPageSteps;
        private Button btnRemoveTempStep;
        private TextBox txbClimOffsetsTempNewStepYear;
        private Label label22;
        private TextBox txbClimOffsetsTempNewStepValue;
        private Label lblTempStepYear;
        private ListBox lstClimTempSteps;
        private Button btnAddTempStep;
        private TextBox txbClimOffsetsPrecipNewStepYear;
        private Button btnRemovePrecipStep;
        private ListBox lstClimPrecipSteps;
        private Button btnAddPrecipStep;
        private TextBox txbClimOffsetsPrecipNewStepValue;
        private Label label34;
        private Label label33;
        private CheckBox chkStepsAndRampsEnabled;
        private TextBox txbClimTransitionYear;
        private Label label40;
        private GroupBox groupBox3;
        private TextBox txbClimValuesPrecip;
        private GroupBox grpClimateTransitions;
        private Label label125;
        private Label label128;
        private Label label36;
        private Button button3;
        private Button button2;
        private Panel panel4;
        private Label label121;
        private CheckBox checkBox2;
        private TextBox textBox8;
        private TextBox textBox9;
        private Label label122;
        private Panel panel5;
        private TextBox textBox10;
        private Label label123;
        private TextBox textBox11;
        private Label label124;
        private Label label126;
        private Label label127;
        private TextBox txbFireMaxEmpiricalFireSizeEvent;
        private TextBox txbFireMaxEmpiricalFireSizeEventWeight;
        private Label label27;
        private Label label28;
        private CheckBox chkLandIsUsingUniqueVegAndAgePerRep;
        private Label label37;
        private TextBox txbLandYearOfVegAndAgeFiles;

	}
}
