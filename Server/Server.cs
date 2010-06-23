using System;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using WeifenLuo.WinFormsUI;

namespace FRESCO_Server
{
    public partial class Server : Form
    {

        #region Public Data Members
        public FileListView             fileList;
        public ClientMonitor            clientMonitor;
        //public EditorFIF                EditorFIF;
        public ViewerProgress           ViewerProgress;
        #endregion


        #region Private Data Members
        private Global					Global;
		private string					FIFName;
        private int                     repsCompleted;
        private int                     yearsCompleted;
        private DateTime                TimeStart;
        private System.Collections.Generic.IEnumerator<FileHandler> scenarioFiles;
        delegate void                   ClearCallback();
        static ClearCallback            clearCallback;
        private WeifenLuo.WinFormsUI.Docking.DeserializeDockContent MyDeserializeDockContent;
        private bool                    allScenariosCompleted = false;
        #endregion


        #region Init and Exit Code...
        public                          Server()
		{
            MyDeserializeDockContent = new WeifenLuo.WinFormsUI.Docking.DeserializeDockContent(GetContentFromPersistString);
			InitializeComponent();
		}
		void					        Server_Shown(object sender, EventArgs e)
		{
            AppDomain currentDomain = AppDomain.CurrentDomain;
            currentDomain.UnhandledException += new UnhandledExceptionEventHandler(LastChanceHandler);

            progressBar.Visible = false;
            Global = Global.Instance;
            Global.Main = this;

            try
            {
                Global.TcpPort = FRESCO_Server.Properties.Settings.Default.port;
            }
            catch (Exception)
            {
                string invalid = FRESCO_Server.Properties.Settings.Default.port.ToString();
                string msg = "Invalid value for the \"port\" setting.  The default port 9051 will be used for now.\n\n" +
                    "Invalid value: " + invalid + "\n" +
                    "Expected an integer.\n";
                MessageBox.Show(msg, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            Global.ProgramStateEvent        += new ProgramStateEventHandler(ProgramStateChanged);
            Global.SimulationStoppedEvent   += new SimulationStoppedEventHandler(SimulationStopped);
            Global.SimulationClearedEvent   += new SimulationClearedEventHandler(SimulationCleared);
            Global.SimulationCompletedEvent += new SimulationCompletedEventHandler(SimulationCompleted);
            Global.SimulationFailedEvent    += new SimulationFailedEventHandler(SimulationFailed);
            ViewFileList();
            ViewClientMonitor();
            Global.ProgramState = EnumProgramState.NOT_LOADED;
            ViewProgress();
		}
		void					        Server_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
            PrepareForClose();
		}
        void                            PrepareForClose()
        {
            if (Global.ProgramState == EnumProgramState.SIMULATING)
                SimulationStop();
            clientMonitor.DisconnectClients();
            LayoutSave();
        }
        void                            LayoutLoad()
        {
            //Load window docking layout if layout file exists for current FIF file.
            string path = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "config");
            path = Path.Combine(path, Path.GetFileNameWithoutExtension(FIFName) + ".layout");

            if (File.Exists(path))
            {
                DockManager.SuspendLayout(true);

                CloseAllForms();  //In order to load layout from XML, we need to close all the DockContent forms.
                DockManager.LoadFromXml(path, MyDeserializeDockContent);

                DockManager.ResumeLayout(true, true);
            }
        }
        void                            LayoutSave()
        {
            string path = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "config");
            if (!Directory.Exists(path))
                Directory.CreateDirectory(path);

            string filename = Path.GetFileNameWithoutExtension(FIFName);
            if (!String.IsNullOrEmpty(filename))
            {
                path = Path.Combine(path, filename + ".layout");
                DockManager.SaveAsXml(path);
            }
        }
        WeifenLuo.WinFormsUI.Docking.IDockContent GetContentFromPersistString(string persistString)
        {
            switch (persistString)
            {
                //case "FRESCO_Server.EditorFIF": EditFIF(); return EditorFIF;
                case "FRESCO_Server.clientMonitor": ViewClientMonitor(); return clientMonitor;
                //case "FRESCO_Server.EditorBatchRuns":            EditBatchRuns();            return EditorBatchRuns;
                case "FRESCO_Server.ViewerProgress": ViewProgress(); return ViewerProgress;
                //case "FRESCO_Server.ViewerMapFire":              ViewFire();                 return ViewerMapFire;
                //case "FRESCO_Server.ViewerMapFireAge":           ViewFireAge();              return ViewerMapFireAge;
                //case "FRESCO_Server.ViewerMapFireSuppression":   ViewFireSuppression();      return ViewerMapFireSuppression;
                //case "FRESCO_Server.ViewerMapAge":               ViewAge();                  return ViewerMapAge;            
                //case "FRESCO_Server.ViewerMapVegetation":        ViewVegetation();           return ViewerMapVegetation;
                //case "FRESCO_Server.ViewerMapReplay":            ViewReplay();               return ViewerMapReplay;
                default: return null;
            }
        }
        void                            CloseAllForms()
        {
            WeifenLuo.WinFormsUI.Docking.IDockContent[] c = new WeifenLuo.WinFormsUI.Docking.IDockContent[DockManager.Contents.Count];
            int i = 0;

            //Must build an array from enumerator, because the Close() call interupts the enumerator.
            foreach (WeifenLuo.WinFormsUI.Docking.IDockContent content in DockManager.Contents)
                c[i++] = content;
            for (i = 0; i < c.Length; i++)
                c[i].DockHandler.Close();  //Raises OnFormClosed event for clean up.
        }
        #endregion


        #region Program State Code...
        public void                     ProgramStateChanged(Object sender, ProgramStateEventArgs e)
        {
            SafeInvokeHelper.Invoke(this, "SetMenu");
            clientMonitor.UpdateServerStateOnClients(Global.ProgramState);
        }
        public void                     SetMenu()
        {
            EnumProgramState ps = Global.ProgramState;
            //File.
            //mnuFileOpen.Enabled = (ps != EnumProgramState.SIMULATING && ps != EnumProgramState.STOPPING);
            //btnFileOpen.Enabled = mnuFileOpen.Enabled;

            //mnuFileSave.Enabled = (ps != EnumProgramState.NOT_LOADED && ps != EnumProgramState.SAVING && ps != EnumProgramState.SIMULATING);
            //btnFileSave.Enabled = mnuFileSave.Enabled;

            mnuFileExit.Enabled = true;

            //btnViewClientMonitor.Checked = (clientMonitor == null) ? false : !clientMonitor.IsHidden;

            //Simulation.
            mnuSimulationClear.Enabled = (allScenariosCompleted || ps == EnumProgramState.COMPLETED || ps == EnumProgramState.STOPPED);
            btnSimulationClear.Enabled = mnuSimulationClear.Enabled;

            //mnuSimulationRun.Enabled = (ps == EnumProgramState.LOADED || ps == EnumProgramState.STOPPED);
            mnuSimulationRun.Enabled = (fileList.Count > 0 && !allScenariosCompleted && (ps == EnumProgramState.NOT_LOADED || ps == EnumProgramState.LOADED || ps == EnumProgramState.STOPPED));
            btnSimulationRun.Enabled = mnuSimulationRun.Enabled;
        
            mnuSimulationRun.Checked = (ps == EnumProgramState.SIMULATING);
            btnSimulationRun.Checked = mnuSimulationRun.Checked;

            mnuSimulationStop.Enabled = (ps == EnumProgramState.SIMULATING);
            btnSimulationStop.Enabled = mnuSimulationStop.Enabled;
        }
        #endregion


        void                            FileOpen()
        {
            //Let user pick file and open it.
            OpenFileDialog OpenFileDialogBox = new OpenFileDialog();
            OpenFileDialogBox.Filter = "FIF files (*.fif)|*.fif|All files (*.*)|*.*";
            if (OpenFileDialogBox.ShowDialog() == DialogResult.OK)
                FileOpen(OpenFileDialogBox.FileName);
            else
                ViewerProgress.Clear();
        }
        bool                            FileOpen(string filepath)
        {
            bool isFileOpened = false;
            if (ViewerProgress == null) ViewProgress();
            FIFName = filepath;
            clientMonitor.FifPath = filepath;
            try
            {
                Global.FIF.LoadLite(FIFName);
                //Global.FIF.Load(FIFName);
                //if (EditorFIF == null) EditFIF();
                //else EditorFIF.Refresh();
                Global.ProgramState = EnumProgramState.LOADED;
                ViewerProgress.AddText("\tFIF Input\t\tfile:" + FIFName.Replace(" ","%20") + "\n");
                this.Text = Global.Title + " - " + FIFName.Substring(FIFName.LastIndexOf("\\") + 1);
                isFileOpened = true;
            }
            catch (Exception E)
            {
                isFileOpened = false;
                ViewerProgress.AddError("Load FIF failed.  File: " + FIFName + ".\n" + E.Message);
                Global.ProgramState = EnumProgramState.NOT_LOADED;
                FIFName = "";
            }
            return isFileOpened;
        }
        void                            FileSave()
        {
            EnumProgramState bookMark = Global.ProgramState;
            Global.ProgramState = EnumProgramState.SAVING;
            MessageBox.Show("Pretend that the FIF file was saved.");
            //TODO EditorFIF.Save();
            Global.ProgramState = bookMark;
        }
		void					        FileExit()
		{
			this.Close();
		}

        void                            ViewFileList()
        {
            if (fileList == null)
            {
                fileList = new FileListView();
                fileList.Load += new System.EventHandler(this.MDIClient_Load);
                fileList.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FileList_FormClosed);
                fileList.HideOnClose = true;
                fileList.Show(DockManager, WeifenLuo.WinFormsUI.Docking.DockState.DockLeft);
            }
            else if (fileList.IsHidden)
                fileList.Show(DockManager);
            else
                fileList.Hide();
        }
        void					        ViewProgress()
        {
            if (ViewerProgress==null) 
            {
                ViewerProgress				= new ViewerProgress();		
                //ViewerProgress.Load			+= new System.EventHandler(this.MDIClient_Load); 
                ViewerProgress.FormClosed	+= new System.Windows.Forms.FormClosedEventHandler(this.ViewerProgress_FormClosed);
                
                //Global.ProgramStateEvent += new ProgramStateEventHandler(ProgramStateChanged);
                //ViewerProgress.FormClosed += new System.Windows.Forms.FormClosedEventHandler(ViewerProgress.OnFormClosed);
                //ViewerProgress.Shown += new System.EventHandler(ViewerProgress.OnShown);
                //ViewerProgress.Activated += new System.EventHandler(ViewerProgress.OnActivated);
                //ViewerProgress.IsHidden = true;

                //ViewerProgress.Show(DockManager, DockState.DockBottom);
                //ViewerProgress.MdiParent = this;
                //ViewerProgress.Show(();
                //ViewerProgress.Show(DockManager);
                //ViewerProgress.MdiParent = this;
                //ViewerProgress.Show();
                ViewerProgress.Show(DockManager.Panes[1], WeifenLuo.WinFormsUI.Docking.DockAlignment.Bottom, 0.5);
                //ViewerProgress.Clear();
            }
            else
                ViewerProgress.Close();
        }
        void                            ViewClientMonitor()
        {
            if (clientMonitor == null)
            {
                clientMonitor = new ClientMonitor();
                clientMonitor.Load += new System.EventHandler(this.MDIClient_Load);
                clientMonitor.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ClientMonitor_FormClosed);
                clientMonitor.AfterYearEnd += new ClientMonitor.AfterYearEndEventHandler(AfterYearEnd);
                clientMonitor.AfterRepEnd += new ClientMonitor.AfterRepEndEventHandler(AfterRepEnd);
                clientMonitor.HideOnClose = true;
                clientMonitor.Show(DockManager);
            }
            else if (clientMonitor.IsHidden)
                clientMonitor.Show(DockManager);
            else
                clientMonitor.Hide();
        }

        void                            SimulationRunAll()
        {
            allScenariosCompleted = false; SetMenu();
            //Check for unsaved changes.
            List<FileHandler> unsaved = new List<FileHandler>();
            foreach (FileHandler file in fileList)
                if (file.HasUnsavedChanges)
                    unsaved.Add(file);
            if (unsaved.Count > 0)
            {
                DialogResult result = MessageBox.Show("Some of your documents have unsaved changes.  Would you like to save them before continuing?", global::AssemblyInfo.Version, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                switch (result)
                { 
                    case DialogResult.Yes:
                        foreach (FileHandler file in unsaved)
                            file.Save();
                        break;
                    case DialogResult.No: break;
                    case DialogResult.Cancel: return;
                }
            }
            //Start first scenario.
            ResetScenarioEnumerator();
            StartNextScenario();
        }
        void                            ResetScenarioEnumerator()
        {

            scenarioFiles = fileList.GetEnumerator();
            scenarioFiles.Reset();
        }
        FileHandler                     CurrentScenario()
        {
            if (scenarioFiles == null) return null;
            return scenarioFiles.Current; 
        }
        void                            StartNextScenario()
        {
            if (scenarioFiles.MoveNext())
            {
                StartScenario(scenarioFiles.Current);
            }
            else {
                allScenariosCompleted = true; SetMenu();
                int doneCount = 0;
                int failCount = 0;
                foreach (FileHandler file in fileList)
                {
                    if (file.IsComplete)
                        doneCount++;
                    else
                        failCount++;
                }
                ViewerProgress.AddHeading2("\n" + doneCount + " done, " + failCount + " failed\n");
            }
        }
        void                            StartScenario(FileHandler file)
        {
            ViewerProgress.AddHeading2("\nStarting " + scenarioFiles.Current.Name + "\n");
            bool isFileLoaded = FileOpen(file.Path);
            if (isFileLoaded)
            {
                file.IsActive = true;
                SimulationRun();
            }
        }
        void                            DoNothing()
        {
        }
        void                            SimulationRun()
        {
            try
            {
                if (Global.ProgramState == EnumProgramState.STOPPED)
                {
                    Global.ProgramState = EnumProgramState.SIMULATING;
                }
                else
                {
                    TimeStart = System.DateTime.Now;
                    stbRep.Text = "";
                    stbTime.Text = "";
                    progressBar.Value = 0;
                    progressBar.Visible = true;
                    yearsCompleted = 0;
                    repsCompleted = 0;
                    clientMonitor.CreateOutputDirectory();

                    ViewerProgress.AddText("\tStat Output\t\tfile:" + Global.Instance.StatOutputDirectory + "\n");
                    ViewerProgress.AddText("\tMap Output\t\tfile:" + Global.Instance.MapOutputDirectory + "\n");
                    ViewerProgress.AddText("\tLog Output\t\tfile:" + Global.Instance.LogOutputDirectory + "\n");
                    ViewerProgress.AddText("\tStarted \t\t" + TimeStart.ToString("M/d/yyyy h:mm:ss tt") + "\n");
                    stbRep.Text = "  Reps Completed: " + repsCompleted + " of " + Global.FIF.MaxReps;
                    
                    clientMonitor.InitRandomSeedGenerator(Global.Instance.FIF.RandSeed);
                    clientMonitor.SendFifPathToClients();
                    clientMonitor.SendRepAssignmentsToClients(Global.Instance.FIF.MaxReps);
                    Global.ProgramState = EnumProgramState.SIMULATING;
                }
            }
            catch (Exception e) {
                Global.Instance.RaiseSimulationFailed(this, new SimulationFailedEventArgs(e));
            }
        }
        void                            SimulationStop()
        {
            Global.ProgramState = EnumProgramState.STOPPING;
            //SimulationStoppedEvent is reaised when all participating clients have stopped.
        }

        void                            SimulationBeginClear(ClearCallback callback)
        {
            ViewerProgress.AddText("\tClearing\n");
            stbRep.Text = "";
            stbTime.Text = "";
            progressBar.Value = 0;
            progressBar.Visible = false;
            clearCallback = callback;  // will be called in SimulationEndClear()
            Global.ProgramState = EnumProgramState.CLEARING;
            clientMonitor.SimulationClear();
            
            //Now wait for clientMonitor to decide all clients are cleared and call SimulationEndClear()
        }
        public void                     SimulationEndClear()
        {
            Global.ProgramState = EnumProgramState.NOT_LOADED;
            //Update progress viewer.
            //ViewerProgress.AddText("\tDone.\n");
            FIFName = "";

            if (clearCallback != null)
            {
                clearCallback.Method.Invoke(this, null);  //for appropriate follow up.
                clearCallback = null;
            }
        }

        //Simulation Events
        void                            SimulationStopped(object sender, EventArgs e)
        {
            Global.ProgramState = EnumProgramState.STOPPED;
        }
        void                            SimulationCleared()
        {
            SafeInvokeHelper.Invoke(this, "SimulationEndClear");
        }
        void                            SimulationCompleted(object sender, SimulationCompletedEventArgs e)
        {
            SafeInvokeHelper.Invoke(this, "UnsafeSimulationCompleted");
        }
        public void                     UnsafeSimulationCompleted()
        {
            Global.ProgramState = EnumProgramState.COMPLETED;
            //Update status bar.
            TimeSpan timeSoFar;
            timeSoFar = new TimeSpan(System.DateTime.Now.Ticks - TimeStart.Ticks);
            stbTime.Text = "Total Time " + timeSoFar.Hours + ":" + timeSoFar.Minutes.ToString("00") + ":" + timeSoFar.Seconds.ToString("00") + "    ";
            //Update progress viewer.
            ViewerProgress.AddText("\tCompleted\t\t" + DateTime.Now.ToString("M/d/yyyy h:mm:ss tt") + "\n");
            ViewerProgress.AddText("\tTotal Time\t\t" + timeSoFar.Hours + ":" + timeSoFar.Minutes.ToString("00") + ":" + timeSoFar.Seconds.ToString("00") + "\n");
            //Update file list.
            FileHandler file = CurrentScenario();
            file.IsActive = false;
            file.IsComplete = true;

            SimulationBeginClear(StartNextScenario);
        }
        void                            SimulationFailed(object sender, SimulationFailedEventArgs e)
        {
            SafeInvokeHelper.Invoke(this, "UnsafeSimulationFailed", e);
        }
        public void                     UnsafeSimulationFailed(SimulationFailedEventArgs e)
        {
            //Update file list.
            FileHandler file = CurrentScenario();
            if (file != null)
            {
                file.IsActive = false;
                file.IsComplete = false;
                file.Failed = true;
            }
            //Update Progress Viewer.
            ViewerProgress.AddError("\tSimulation Failed: " + e.Exception.Message + "\n");
            if (e.Exception.InnerException != null)
                ViewerProgress.AddError("\tInner Exception Details: " + e.Exception.InnerException.Message + "\n");

            //if (Global.Instance.OutputDirectory != null)
            //    ViewerProgress.Save(Path.Combine(Global.Instance.OutputDirectory, "ProgressViewerLog.rtf"));
            if (Global.ProgramState == EnumProgramState.SIMULATING)
                SimulationStop();

            if (file != null)
                SimulationBeginClear(StartNextScenario);
        }
        public void                     AfterYearEnd(object sender, EventArgs e)
        {
            yearsCompleted++;
            int yearsTotal = Global.FIF.MaxReps * (Global.FIF.MaxYears + 1);
            
            //Calculate an estimation of time remaining.
            TimeSpan timeSoFar      = new TimeSpan(System.DateTime.Now.Ticks - TimeStart.Ticks);
            TimeSpan avgTimePerYear = new TimeSpan(timeSoFar.Ticks / (yearsCompleted+1));
            TimeSpan timeToFinish   = new TimeSpan(avgTimePerYear.Ticks * (yearsTotal - (yearsCompleted+1)));
            TimeSpan totalTime      = new TimeSpan(avgTimePerYear.Ticks * yearsTotal);
            stbTime.Text = "Time Remaining " + timeToFinish.Hours + ":" + timeToFinish.Minutes.ToString("00") + ":" + timeToFinish.Seconds.ToString("00") + "    ";
            
            //Set progress bar percentage.
            float percentDone = (float)yearsCompleted / yearsTotal * 100;
            percentDone = System.Math.Max(percentDone, 0);
            percentDone = System.Math.Min(percentDone, 100);
            progressBar.Value = (int)percentDone;
        }
        public void                     AfterRepEnd(object sender, EventArgs e)
        {
            repsCompleted++;
            stbRep.Text = "  Reps Completed: "+ repsCompleted + " of " + Global.FIF.MaxReps;
        }


        void LastChanceHandler(object sender,  UnhandledExceptionEventArgs args)
        {
            bool isProgressViewerSaved = false;
            string logFile = "";
            try
            {
                if (ViewerProgress != null)
                {
                    if (!Directory.Exists(Global.LogOutputDirectory))
                        Directory.CreateDirectory(Global.LogOutputDirectory);
                    logFile = Global.LogOutputDirectory + "\\ProgressViewerLog--" + System.DateTime.Now.ToString("yyyy-MM-dd_ttHHmm") + ".rtf";
                    ViewerProgress.Save(logFile);
                    isProgressViewerSaved = true;
                }
            }
            catch (Exception e)
            { 
                // oh well, let's continue
            }

            Exception ex = ((Exception)(args.ExceptionObject));
            string msg = "Unhandled Error in FRESCO Server: " + ex.Message + "\n\n";
            if (isProgressViewerSaved) msg += "Progress viewer saved to " + logFile + "\n\n";
            msg += "It might be a good idea to take a screenshot of this message to send in for debugging: \n\n";
            msg += ex.ToString();
            MessageBox.Show(msg, "Unhandled Error in FRESCO Server", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        #region Form Events
        //Loading and Closing Child Forms
        void MDIClient_Load(object O, System.EventArgs EventArgs)												  {SetMenu();}
        void CloseDockContent<D>(ref D dockContent) where D : WeifenLuo.WinFormsUI.Docking.DockContent
        {
            dockContent.Hide();
            dockContent.DockPanel = null;
            dockContent = null; 
            SetMenu(); 
        }
        void FileList_FormClosed(object O, System.Windows.Forms.FormClosedEventArgs EventArgs)                   {CloseDockContent<FileListView>(ref fileList); }
        void ClientMonitor_FormClosed(object O, System.Windows.Forms.FormClosedEventArgs EventArgs)              {CloseDockContent<ClientMonitor>(ref clientMonitor); }
        void ViewerProgress_FormClosed(object O, System.Windows.Forms.FormClosedEventArgs EventArgs)             {CloseDockContent<ViewerProgress>(ref ViewerProgress);}

        //File
        void FileOpen_Click(object sender, EventArgs e)
        {
            fileList.addExistingFile();
        }
        void FileSave_Click(object sender, EventArgs e)
        {
            //FileSave();
        }
        void FileExit_Click(object sender, EventArgs e)
        {
            FileExit();
        }

        //Simulation
        void SimulationRun_Click(object sender, EventArgs e)
        {
            if (Global.ProgramState == EnumProgramState.STOPPED)
            {
                if (!clientMonitor.HasStoppedClients())
                    MessageBox.Show("No clients are stopped and ready to resume the simulation.", Global.Title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                else
                    SimulationRun();
            }
            else
            {
                if (!clientMonitor.HasIdleClients())
                    MessageBox.Show("No clients are ready for a new simulation.", Global.Title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                else
                {
                    fileList.reset();
                    SimulationRunAll();
                }
            }
        }
		void SimulationStop_Click(object sender, EventArgs e)
		{
            SimulationStop();
        }
		void SimulationClear_Click(object sender, EventArgs e)
		{
            SimulationBeginClear(new ClearCallback(DoNothing));
            allScenariosCompleted = false;
            //Update file list.
            foreach (FileHandler file in fileList)
            {
                file.IsActive = false;
                file.IsComplete = false;
                file.Failed = false;
            }
        }
        void RunTests_Click(object sender, EventArgs e)
        {
            try
            {
                ViewerProgress.AddHeading1("\nTesting\n");
                ViewerProgress.AddText("\tStatistic.\n");
                TestStatistic t = new TestStatistic();
                t.runTests();
            }
            catch (Exception ex)
            {
                Global.RaiseSimulationFailed(this, new SimulationFailedEventArgs(ex));
            }
        }
        #endregion

        private void changeSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SettingsEditor editor = new SettingsEditor();
            editor.ShowDialog();
        }
	}
}