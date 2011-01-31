using System.IO;
using System.Net.Sockets;

namespace FRESCO_Server
{
    public enum EnumProgramState
    {
        NOT_LOADED=1,					//FIF not loaded.
        LOADED,							//FIF loaded.
        SAVING,							//FIF being saved.
        CLEARING,						//Run being cleared.
        //SIMULATING and below all imply a simulation has been started.
        SIMULATING,						//Simulating a run.
        STOPPING,						//Stopping a simulation.
        STOPPED,						//Simulation stopped.
        COMPLETED						//Simulation completed.
    };
    public enum ClientState
    {
        UNKNOWN,                        //Not yet known.
        DISCONNECTED,                   //Socket connection is disconnected.
        BUSY,                           //Busy running a different server's run.
        ERROR,                          //Client experienced an error and cannot recover.
        AVAILABLE,                      //Ready to process a run.
        CLEARING,                       //Clearing a run.
        SETTING_UP,                     //Getting init data.
        SETUP,                          //Done setting up.
        //SIMULATING and below all imply a simulation has been started.
        SIMULATING,						//Simulating a run.
        STOPPING,						//Stopping a simulation.
        STOPPED,						//Simulation stopped.
        COMPLETED						//Simulation completed.
    }
    public enum OutgoingCommand
    {
        VERSION_ASSERTION,
        SET_CLIENT_ID,
        REP_ASSIGNMENT,
        CLIENT_STATE_REQUEST,
        CLIENT_NAME_REQUEST,
        SERVER_STATE_REPORT,
        SERVER_DISCONNECTING_REPORT,
        FIF_PATH_REPORT
    }

    /// <summary>
    /// Global is a singleton class.
    /// </summary>
    public sealed class Global
    {
        //Singleton code.
        static readonly object  padlock = new object();
        private static Global   instance = null;
        public static Global    Instance
        {
            get
            {
                //Retreive the single instance.
                //Mutual exclusion for thread safety.
                lock (padlock)
                {
                    if (instance == null)
                        instance = new Global();
                    return instance;
                }
            }
        }
        private                 Global()
        {
            programState = EnumProgramState.NOT_LOADED;
            FIF = new FIF(this);
            WorkingDirectory = Directory.GetCurrentDirectory();
        }


        /// <summary>
        /// Appends a string to the end of a client log.
        /// </summary>
        /// <param name="message">A client log entry.</param>
        public void AddClientLogEntry(string message, Socket client)
        {
            AddLogEntry(message, client, false);
            //ClientOutputEvent(instance, new ClientOutputEventArgs(message, -1, false));
        }
        
        /// <summary>
        /// Appends a string, prefixed with "DEBUG: ", to the end of a client log.
        /// </summary>
        /// <param name="message">A client log entry.</param>
        public void AddDebugToClientLog(string message, Socket client)
        {
            AddLogEntry(message, client, true);
//          ClientOutputEvent(instance, new ClientOutputEventArgs("DEBUG: " + message, -1, false));
        }
        
        private void AddLogEntry(string message, Socket client, bool isDebugMessage)
        {
            if (!IsDebugMode && isDebugMessage)
                return;
            else if (IsDebugMode && isDebugMessage)
                message = "DEBUG: " + message;

            int clientID = -1;      //To be set by pass as reference call below.
            if (Main.clientMonitor.clientList.Contains((System.Net.IPEndPoint)client.RemoteEndPoint, ref clientID)) {
                //Client already in the ClientList.  Write message to its ClientLog.
                Main.clientMonitor.clientList.ByID(clientID).Log.Write(ref message, false);
            } 
            else {
                //Client not in ClientList.  Write message to ProgressViewer.
                SafeInvokeHelper.Invoke(Main.ViewerProgress, "AddWarning", message);
                //Main.ViewerProgress.AddWarning(message);
            }
        }
 
        public void     RaiseSimulationStopped(object sender, System.EventArgs e)
        {
            if (Global.Instance.SimulationStoppedEvent != null)
                Global.Instance.SimulationStoppedEvent(sender, e);
        }
        public void     RaiseSimulationCleared(object sender, System.EventArgs e)
        {
            if (Global.Instance.SimulationClearedEvent != null)
                Global.Instance.SimulationClearedEvent();
        }
        public void     RaiseSimulationCompleted(object sender, SimulationCompletedEventArgs e)
        {
            if (Global.Instance.SimulationCompletedEvent != null)
                Global.Instance.SimulationCompletedEvent(sender, e);
        }
        public void     RaiseSimulationFailed(object sender, SimulationFailedEventArgs e)
        {
            if (Global.Instance.SimulationFailedEvent != null)
                Global.Instance.SimulationFailedEvent(sender, e);
        }

        private readonly string Copyright = "University of Alaska, Department of Forest Sciences";
        public bool IsDebugMode = false;
        private EnumProgramState programState;
        //Global variables.
        public Server Main;
        public FIF FIF;
        public EnumProgramState ProgramState
        {
            get { return programState; }
            set 
            {
                programState = value;
                if (ProgramStateEvent != null) ProgramStateEvent(this, new ProgramStateEventArgs(programState));
            }
        }
        public bool IsRunningLocal;
        public string WorkingDirectory;

        public string TimeStampedOutputPath;
        public string LogOutputDirectory
        {
            get 
            {
                if (Global.Instance.FIF.ServerOutputBasePath == null || Global.Instance.TimeStampedOutputPath == null)
                    return "";
                string o = Path.Combine(Global.Instance.FIF.ServerOutputBasePath, Global.Instance.TimeStampedOutputPath);
                return Path.Combine(o, "Logs");
            }
        }
        public string MapOutputDirectory
        {
            get
            {
                string result = "";
                string seperator = "\\";
                if (Global.Instance.FIF.ClientOutputBasePath != null)
                    if (Global.Instance.FIF.ClientOutputBasePath != "")
                    {
                        if (Global.Instance.FIF.ClientOutputBasePath.Contains("/"))
                            seperator = "/";
                        result = Global.Instance.FIF.ClientOutputBasePath + seperator + Global.Instance.TimeStampedOutputPath + seperator + "Maps";
                    }
                return result;
            }
        }
        public string StatOutputDirectory
        {
            get
            {
                return Path.Combine(Global.Instance.FIF.ServerOutputBasePath, Global.Instance.TimeStampedOutputPath);
            }
        }
        public event ProgramStateEventHandler ProgramStateEvent;
        public event SimulationStoppedEventHandler SimulationStoppedEvent;
        public event SimulationClearedEventHandler SimulationClearedEvent;
        public event SimulationCompletedEventHandler SimulationCompletedEvent;
        public event SimulationFailedEventHandler SimulationFailedEvent;
        public event ClientOutputEventHandler ClientOutputEvent;
        public readonly string Title = "FRESCO Server";
        public readonly string Version = "1.0.11";
        public readonly int UdpPort = 9050;
        public int TcpPort = 9051;
        public readonly short asNull = -1;
        public readonly float PI = 3.1415927f;
    }

    //General.
	public enum EDetailLevel	{MINIMAL, MODERATE, MAXIMUM};
	public enum EOutputType		{DELETEOUTPUTDIRECTORY,OVERWRITE,APPENDDATETIME};

    //Climate.
	public enum EClimateValuesType		{VTCONSTANT, VTSPATIAL, VTEXPLICIT, VTRANDEXPLICIT};
    public enum EClimateOffsetsType     {OTNONE, OTCONSTANT, OTFILE, OTRANDOM};
    public enum EClimateOffsetsApplied  {OADIRECT, OASTOCHASTIC, OAREPLICATED};
    public enum EClimateClimateType     {TEMP, PRECIP};
    public enum EClimateOffsetType      {STEP, RAMP};										
    public struct SClimateTransition	 
    {
        //public SClimateTransition()
        //{
        //    ValuesType = EClimateValuesType.VTCONSTANT;
        //    OffsetsType = EClimateOffsetsType.OTCONSTANT;
        //}
        public int						Year;
        //Values
        public EClimateValuesType ValuesType;
        public float ConstantTemp;
        public float ConstantPrecip;
        public string SpatialTempFile;
        public string SpatialPrecipFile;
        public int RandExplicitMinYear;
        public int RandExplicitMaxYear;
        public bool IsRandExplicitReplicated;
        //Offsets
        public EClimateOffsetsType OffsetsType;
        public float ConstantTempOffset;
        public float ConstantPrecipOffset;
        public string OffsetsFile;
        public float RandomOffsetsTempMean;
        public float RandomOffsetsTempStdDev;
        public float RandomOffsetsPrecipMean;
        public float RandomOffsetsPrecipStdDev;
        public bool IsRandOffsetReplicated;
        //SClimateTransition() : Year(0), 
        //    ValuesType(CClimate::VTCONSTANT), ConstantTemp(0), ConstantPrecip(0), SpatialTempFile(""), SpatialPrecipFile(""), RandExplicitMinYear(0), RandExplicitMaxYear(0), IsRandExplicitReplicated(false),
        //    OffsetsType(CClimate::OTCONSTANT), 	ConstantTempOffset(0), ConstantPrecipOffset(0), OffsetsFile(""), RandomOffsetsTempMean(0), RandomOffsetsTempStdDev(0), RandomOffsetsPrecipMean(0), RandomOffsetsPrecipStdDev(0), IsRandOffsetReplicated(false) {};
    };    
    
    //Fire.
	public enum EFireType		{FIXED, SPATIAL, HISTORICAL};
	public enum EFireCause		{NATURAL, HUMAN};
    public struct SFireTransition	 
    {
        //public SFireTransition()
        //{
        //    Type = EFireType.FIXED;
        //}

        public int Year;
        public EFireType Type;
        public float Ignition;
        public float Sensitivity;
        public string SpatialIgnitionFile;
        public string SpatialSensitivityFile;
        public string HistoricalFile;
    };
}
