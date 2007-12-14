using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI;

namespace FRESCO_Server
{
    public partial class ClientMonitor : FRESCOForm
    {
        #region Events
        public delegate void AfterYearEndEventHandler(object sender, EventArgs e);
        public event AfterYearEndEventHandler AfterYearEnd;
        public delegate void AfterRepEndEventHandler(object sender, EventArgs e);
        public event AfterRepEndEventHandler AfterRepEnd;
        #endregion

        #region Variables
        public int setupID = 1;
        public string HostName
        {
            get { return this.hostName; }
        }
        public string FifPath
        {
            get { return fifPath; }
            set 
            { 
                fifPath = value;
            }
        }
        private string hostName;
        private string fifPath;
        private Messenger messenger;
        public ClientList clientList;
        private static readonly object clientLogLock = new Object();
        private static readonly object nextRepLock = new Object();
        private static readonly object statLock = new Object();
        private OrderedDictionary stats;
        private Random randSeedGenerator;
        private int selectedClientID;
        private bool isAcceptingNewConnections;
        #endregion

        public          ClientMonitor()
        {
            InitializeComponent();
            this.hostName = Dns.GetHostName();
            this.clientList = new ClientList();
            clientGrid.DataSource = clientList;
            this.messenger = new Messenger();
            messenger.NewClientConnectedEvent += new NewClientConnectedEventHandler(Messenger_NewClientConnected);
            messenger.RPCReceivedEvent += new RPCReceivedEventHandler(Messenger_RPCReceived);
            Global.Instance.ClientOutputEvent += new ClientOutputEventHandler(OnClientOutput);
            stats = new OrderedDictionary(25);  //TODO: Statition class?
            selectedClientID = -1;
            splitContainer.Panel2Collapsed = true;
        }
        private void    ClientManager_Shown(object sender, EventArgs e)
        {
            lblNoClientsConnected.BringToFront();
            clientLogDisplay.SetDefaults("Courier New", 8, false, false, Color.Black, Color.White, HorizontalAlignment.Left, false, false, new List<int>(new int[] { 25 }));
            clientLogDisplay.RightMargin = 3000;

            StartReceivingClientConnections();
        }
        private void    DoDisposeWork()
        {
            //Called from Dispose in the designer code.
            for (int i=0; i<clientList.Count; i++)
                clientList[i].Log.Close();
            ClientLog.DeleteTempDirectory();
        }
        //User interaction and UI support.
        private void    RefreshGrid()
        {
            SafeInvokeHelper.Invoke(clientGrid, "RefreshDataSource");
        }
        private void    HideLog()
        {
            splitContainer.Panel2Collapsed = true;
            //Stop listening to previous selected client.
            if (selectedClientID >= 0)
                clientList.ByID(selectedClientID).Log.ClearSubscriptions();
            selectedClientID = -1;
        }
        private void    ViewLogOfSelectedClient()
        {
            if (gridView1.GetSelectedRows().Length > 0)
            {
                //Stop listening to previous selected client.
                if (selectedClientID >= 0)
                    clientList.ByID(selectedClientID).Log.ClearSubscriptions();
                //Show saved log for newly selected client.
                selectedClientID = GetSelectedClientID();
                clientLogDisplay.Clear();
                clientLogDisplay.AddText(clientList.ByID(selectedClientID).Log.GetContents().ToString());
                //Start listening to currently selected client.
                clientList.ByID(selectedClientID).Log.LogEntryEvent += new ClientOutputEventHandler(OnLogEntryDisplay);
                //Update label.
                lblClientLog.Text = "Log of Client " + GetSelectedClientID() + ": ";
                splitContainer.Panel2Collapsed = false;
            }
        }
        private int     GetSelectedClientID()
        {
            return Convert.ToInt32(gridView1.GetRowCellDisplayText(gridView1.GetSelectedRows()[0], gridView1.Columns["ID"]));
        }
        public void     SimulationClear()
        {
            for (int i=0; i<clientList.Count; i++)
            {
                clientList[i].SimulationClear();
            }
            RefreshGrid();
            ClearStats();
            HideLog();
        }
        public bool     HasIdleClients()
        {
            for (int i=0; i<clientList.Count; i++) 
            {
                if (clientList[i].State == ClientState.AVAILABLE) 
                {
                    return true;
                }
            }
            return false;
        }
        public bool     HasStoppedClients()
        {
            for (int i = 0; i < clientList.Count; i++)
            {
                if (clientList[i].State == ClientState.STOPPED)
                {
                    return true;
                }
            }
            return false;        
        }
        public void     InitRandomSeedGenerator(string masterSeed)
        {
            if (masterSeed == "CURRENTTIME")
            {
                randSeedGenerator = new Random();
            }
            else 
            {
                int seed = 0;
                try { seed = Convert.ToInt32(masterSeed); }
                catch (Exception) { throw new Exception("Cannot convert RandomSeed text (" + masterSeed + " } to an integer.  Set the RandomSeed FIF field to \"CURRENTTIME\" to use the current time as the seed."); }
                randSeedGenerator = new Random(seed);
            }
        }
        public int      GetNextRandomSeed()
        {
            int seed = -1;
            if (randSeedGenerator != null)
                seed = randSeedGenerator.Next(int.MaxValue);
            return seed;
        }
        //Connectiong and Disconnecting with Clients.
        public void     StartReceivingClientConnections()
        {
            isAcceptingNewConnections = true;
            messenger.StartReceivingClientConnections();
        }
        private void    Messenger_NewClientConnected(Object sender, NewClientConnectedEventArgs e)
        {
            SafeInvokeHelper.Invoke(this, "SetupNewClient", e.socket);
        }
        public void     SetupNewClient(Socket client)
        {
            int clientID = clientList.Add(client);
            Global.Instance.AddDebugToClientLog("ClientManager: New client connected.\tClientID=" + clientID + "\tEndPoint=" + clientList.ByID(clientID).EndPoint.ToString() + "\n", client);
            //AddClientLogEntry(clientID, "Connected.\n", false);
            
            //Start listening to client.
            messenger.BeginReceiveDataFrom(clientList.ByID(clientID).Socket);

            //Setup and request info from client until we get response.
            int attemptCount = 0;
            bool isClientSetup = false;
            while (!isClientSetup && attemptCount < 1)
            {
                attemptCount++;
                isClientSetup = SetupNewClientHelper(clientID);
            }
        }
        public bool     SetupNewClientHelper(int clientID)
        {
            //Set clientID to client.
            RemoteProcedureCall idRpc = new RemoteProcedureCall(clientID, OutgoingCommand.SET_CLIENT_ID.ToString(), null);
            messenger.SendData(idRpc, clientList.ByID(clientID).Socket);
            //Request client's state.
            RemoteProcedureCall stateRpc = new RemoteProcedureCall(clientID, OutgoingCommand.CLIENT_STATE_REQUEST.ToString(), null);
            messenger.SendData(stateRpc, clientList.ByID(clientID).Socket);
            //Request client's hostname.
            RemoteProcedureCall nameRpc = new RemoteProcedureCall(clientID, OutgoingCommand.CLIENT_NAME_REQUEST.ToString(), null);
            messenger.SendData(nameRpc, clientList.ByID(clientID).Socket);
            //Assert client's version number is same as server.  Client throws error if not. 
            string[] parameter = { Global.Instance.Version };
            RemoteProcedureCall versionRpc = new RemoteProcedureCall(clientID, OutgoingCommand.VERSION_ASSERTION.ToString(), parameter);
            messenger.SendData(versionRpc, clientList.ByID(clientID).Socket);
            //Update client list.
            lblNoClientsConnected.SendToBack();
            RefreshGrid();

            return !(clientList.ByID(clientID).State == ClientState.UNKNOWN);  //Returns true if response is received from client.
        }
        public void     DisconnectClients()
        {
            messenger.StopReceivingClientConnections();
            Client c;
            for (int i=0; i<clientList.Count; i++)
            {
                c = clientList[i];
                if (c.Socket.Connected)
                {
                    messenger.SendData(new RemoteProcedureCall(c.ID, OutgoingCommand.SERVER_DISCONNECTING_REPORT.ToString(), null), c.Socket);
                    System.Threading.Thread.Sleep(500);
                    c.Socket.Close();
                }
            }
        }
        public void     DisconnectClient(int clientID)
        {
            AddClientLogEntry(clientID, "Disconnected.", false);
            Client client = clientList.ByID(clientID);
            client.State = ClientState.DISCONNECTED;
            client.IsParticipating = false;
            RefreshGrid();
            client.Socket.Close();
        }
        //Incoming commands from clients.
        public void     SelectProcedure(RemoteProcedureCall incomingRPC)
        {
            int id = incomingRPC.ClientID;
            Client client = clientList.ByID(id);
            string[] parms = incomingRPC.Parameters;
            //Using the RPC object passed in by the Messenger, choose which procedure to call.
            switch (incomingRPC.Command)
            {
                case "CLIENT_STATE_REPORT":
                    ClientState oldState = client.State;
                    client.State = (ClientState)Convert.ToInt32(incomingRPC.Parameters[0]);
                    RefreshGrid();

                    switch (client.State) 
                    {
                        case ClientState.ERROR:
                            Global.Instance.RaiseSimulationFailed(this, new SimulationFailedEventArgs(new Exception("See file:" + Path.Combine(Global.Instance.LogOutputDirectory, "clientLog"+client.ID+".txt") + " for details.")));
                            break;
                        case ClientState.AVAILABLE:
                            client.IsCompleted = false;
                            if (oldState == ClientState.CLEARING)
                            {
                                bool allClientsAreCleared = true;
                                foreach (Client c in clientList) {
                                    if (c.IsParticipating) {
                                        if (c.State != ClientState.AVAILABLE)
                                            allClientsAreCleared = false;
                                    }
                                }
                                if (allClientsAreCleared)
                                    Global.Instance.RaiseSimulationCleared(this, new EventArgs());
                            }
                            break;
                        case ClientState.STOPPED:
                            bool allClientsAreStopped = true;
                            foreach (Client c in clientList) {
                                if (c.State != ClientState.STOPPED)
                                    if (c.IsParticipating) allClientsAreStopped = false;
                            }
                            if (allClientsAreStopped) Global.Instance.RaiseSimulationStopped(this, new EventArgs());
                            break;
                        case ClientState.COMPLETED:
                            client.IsCompleted = true;
                            client.RandomSeed = -1;
                            bool allClientsCompleted = true;
                            foreach (Client c in clientList) {
                                if (c.IsParticipating && !c.IsCompleted)
                                    allClientsCompleted = false;
                            }
                            if (allClientsCompleted) {
                                SaveStats();
                                Global.Instance.RaiseSimulationCompleted(this, new SimulationCompletedEventArgs());
                            }
                            break;
                    }
                    RefreshGrid();
                    break;
                case "COMPUTER_NAME_REPORT":
                    client.HostName = incomingRPC.Parameters[0];
                    RefreshGrid();
                    break;
                case "REP_REPORT":
                    client.Rep = Convert.ToInt32(incomingRPC.Parameters[0]);
                    RefreshGrid();
                    break;
                case "YEAR_REPORT":
                    client.Year = Convert.ToInt32(incomingRPC.Parameters[0]);
                    RefreshGrid();
                    break;
                case "REP_COMPLETED":
                    client.RepsCompleted++;
                    RefreshGrid();
                    SafeInvokeHelper.Invoke(this, "RaiseAfterRepEnd");
                    break;
                case "YEAR_COMPLETED":
                    SafeInvokeHelper.Invoke(this, "RaiseAfterYearEnd");
                    break;
                case "DISCONNECT_REPORT":
                    DisconnectClient(id);
                    break;
                case "CLIENT_LOG_ENTRY":
                    AddClientLogEntry(id, incomingRPC.Parameters[0], (incomingRPC.Parameters[1] == "true" ? true : false));
                    //SetupNewClientHelper(setupID++);
                    break;
                case "STAT_SETUP":
                    SetupStat(parms[0], Convert.ToInt32(parms[1]), Convert.ToInt32(parms[2]), Convert.ToInt32(parms[3]), Convert.ToInt32(parms[4]), Convert.ToBoolean(parms[5]));
                    break;
                case "STAT_ADD":
                    AddStat(parms[0], Convert.ToInt32(parms[1]), Convert.ToInt32(parms[2]), Convert.ToDouble(parms[3]));
                    break;
                case "FIRESIZE_STAT_ADD":
                    AddFireSizeStat(parms[0], Convert.ToInt32(parms[1]), Convert.ToInt32(parms[2]), Convert.ToDouble(parms[3]), Convert.ToInt32(parms[4]), Convert.ToInt32(parms[5]), Convert.ToInt32(parms[6]), Convert.ToInt32(parms[7]), Convert.ToInt32(parms[8]));
                    break;
                default:
                    throw new Exception("SelectProcedure Failed: invalid command.");
            }
        }
        public void     AddClientLogEntry(int clientID, string text, bool isError)
        {
            clientList.ByID(clientID).Log.Write(ref text, isError);
        }
        public void     UnsafeClientLogEntryDisplay(string text)
        {
            clientLogDisplay.AddText(text);
        }
        public void     SetupStat(string title, int maxYear, int maxRep, int timeStep, int outFlags, bool isFireSizeStat)
        {
            lock (statLock)
            {
                if (!stats.Contains(title))
                {
                    if (isFireSizeStat)
                        stats.Add(title, new FireSizeStatistic(title, maxYear, maxRep, timeStep, outFlags));
                    else
                        stats.Add(title, new Statistic(title, maxYear, maxRep, timeStep, outFlags));
                }
                //else, it was already setup and is ready for any client to add to.
            }
        }
        public void     AddStat(string title, int year, int rep, double value)
        {
            lock (statLock)
            {
                if (stats.Contains(title))
                    ((Statistic)stats[title]).Add(year, rep, value);
                else
                    throw new Exception("Client attempted to add to a non-existant statistic titled " + title);
            }
        }
        public void     AddFireSizeStat(string title, int year, int rep, double value, int cause, int low, int mod, int hiLSS, int hiHSS)
        {
            lock (statLock)
            {
                if (stats.Contains(title))
                    ((FireSizeStatistic)stats[title]).Add(year, rep, value, cause, low, mod, hiLSS, hiHSS);
                else
                    throw new Exception("Client attempted to add to a non-existant statistic titled " + title);
            }
        }
        public void     SaveStats()
        {
            for (int i = 0; i < stats.Count; i++)
            {
                if (stats[i].GetType().Name == "FireSizeStatistic")
                    ((FireSizeStatistic)stats[i]).Save();
                else
                    ((Statistic)stats[i]).Save();
            }
        }
        public void     ClearStats()
        {
            for (int i = 0; i < stats.Count; i++)
            {
                if (stats[i].GetType().Name == "FireSizeStatistic")
                    ((FireSizeStatistic)stats[i]).Clear();
                else
                    ((Statistic)stats[i]).Clear();
            }
            stats.Clear();
        }
        public void     RaiseAfterYearEnd()
        {
            if (AfterYearEnd != null) AfterYearEnd(this, new EventArgs());
        }
        public void     RaiseAfterRepEnd()
        {
            if (AfterRepEnd != null) AfterRepEnd(this, new EventArgs());
        }
        //Outgoing commands to clients.
        public void     UpdateServerStateOnClients(EnumProgramState state)
        { 
            for (int i=0; i<clientList.Count; i++)
                messenger.SendData(new RemoteProcedureCall(clientList[i].ID, OutgoingCommand.SERVER_STATE_REPORT.ToString(), new string[] { ((int)state).ToString() }), clientList[i].Socket);
        }
        public void     SendFifPathToClients()
        {
            for (int i=1; i<=clientList.Count; i++)
                SendFifPathTo(i);
        }
        public void     SendFifPathTo(int clientID)
        {
            if (clientList.ByID(clientID).RandomSeed == -1)
                clientList.ByID(clientID).RandomSeed = GetNextRandomSeed();
            messenger.SendData(new RemoteProcedureCall(clientID, OutgoingCommand.FIF_PATH_REPORT.ToString(), new string[] { fifPath.Substring(fifPath.LastIndexOf("\\") + 1), Global.Instance.FIF.ClusterBaseDirectory, Global.Instance.OutputDirectory, clientList.ByID(clientID).RandomSeed.ToString() }), clientList.ByID(clientID).Socket);
        }
        public void     CreateOutputDirectory(string dir, EOutputType type)
        {
            string baseDir = Global.Instance.FIF.BaseDirectory + "\\";
            switch (type)
            {
                case EOutputType.DELETEOUTPUTDIRECTORY:
                    //Delete existing output tree and create new tree.
                    Directory.Delete(baseDir + dir, true);
                    Directory.CreateDirectory(baseDir + dir);
                    Directory.CreateDirectory(baseDir + dir + "\\Maps");
                    break;
                case EOutputType.OVERWRITE:
                    //Make sure output directories exist and replace any existing files.
                    Directory.CreateDirectory(baseDir + dir);
                    Directory.CreateDirectory(baseDir + dir + "\\Maps");
                    break;
                case EOutputType.APPENDDATETIME:
                    //Create new output directory by appending time.
                    dir += "\\" + DateTime.Now.ToString("yyyy-MM-dd_ttHHmm");
                    if (Directory.Exists(baseDir + dir))
                    {
                        //Folder for this date-time already exists, append "(count)".
                        int length = dir.Length;
                        int count = 2;
                        while (Directory.Exists(baseDir + dir))
                            dir = dir.Substring(0, length) + "_(" + (count++).ToString() + ")";
                    }
                    Directory.CreateDirectory(baseDir + dir);
                    Directory.CreateDirectory(baseDir + dir + "\\Maps");
                    break;
                default:
                    throw new Exception("Invalid output type.\n");
            }
            Global.Instance.OutputDirectory = dir;
        }
        public void     SendRepAssignmentsToClients(int numReps)
        {
            //Divi up reps between all available clients.
            int numClients = clientList.Count;
            int numRepsPerClient = numReps / numClients;
            int remainder = numReps % numClients;
            int repStart = 0;
            int repStop = numRepsPerClient + (remainder>0?1:0);
            remainder--;
            for (int i=0; i<numClients; i++) {
                if (clientList[i].State == ClientState.AVAILABLE) {
                    clientList[i].IsParticipating = true;
                    //Set local copy.
                    clientList[i].RepStart = repStart;
                    clientList[i].RepStop = repStop;
                    //Set remote client.
                    string[] parms = { repStart.ToString(), repStop.ToString() };
                    RemoteProcedureCall rpc = new RemoteProcedureCall(clientList[i].ID, OutgoingCommand.REP_ASSIGNMENT.ToString(), parms);
                    messenger.SendData(rpc, clientList[i].Socket);
                    //Prepare for next client.
                    repStart = repStop;
                    repStop += numRepsPerClient + (remainder > 0 ? 1 : 0);
                    remainder--;
                }
                else
                    clientList[i].IsParticipating = false;
            }
        }
        //Events.
        private void    Messenger_RPCReceived(Object sender, RPCReceivedEventArgs e)
        {
            SelectProcedure(e.RemoteProcedureCall);
        }
        private void    OnClientOutput(Object sender, ClientOutputEventArgs e)
        {
            if (clientLogDisplay.InvokeRequired) SafeInvokeHelper.Invoke(clientLogDisplay, "AppendText", e.Message);
            else clientLogDisplay.AppendText(e.Message);
        }
        private void    OnLogEntryDisplay(object sender, ClientOutputEventArgs e)
        {
            if (e.IsError)
                SafeInvokeHelper.Invoke(clientLogDisplay,"AddSystemError","Error on client " + e.ClientID, e.Message);
            else
                SafeInvokeHelper.Invoke(this, "UnsafeClientLogEntryDisplay", e.Message);
        }
        private void    mnuViewLog_Click(object sender, EventArgs e)
        {
            ViewLogOfSelectedClient();
        }
        private void    logHideButton_Click(object sender, EventArgs e)
        {
            HideLog();
        }
        private void    clientGrid_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (selectedClientID == GetSelectedClientID())
                    HideLog();
                else
                    ViewLogOfSelectedClient();
            }
        }
        private void    clientGrid_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right  &&  e.Y > 18)
            {
                clientContextMenu.Show(new Point(MousePosition.X - clientGrid.Location.X, MousePosition.Y - clientGrid.Location.Y));
            }
        }
        private void    clientGrid_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (e.Y > 18) //Ensure not clicking on header.  Column headers are 18 units heigh.
            {
                if (selectedClientID == GetSelectedClientID())
                    HideLog();
                else
                    ViewLogOfSelectedClient();
            }
        }
    }
}
