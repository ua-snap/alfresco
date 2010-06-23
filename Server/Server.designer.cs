using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;  //For ATL Events. (might not be needed)
using WeifenLuo.WinFormsUI;

namespace FRESCO_Server
{
	partial class Server
	{
		#region Designer Code

		private System.Windows.Forms.ImageList ToolbarImages;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.ToolBarButton toolBarButton1;
		private System.Windows.Forms.StatusBar stbStatusBar;
        private System.Windows.Forms.StatusBarPanel stbRep;
		private System.Windows.Forms.StatusBarPanel stbSpacer;
		private System.Windows.Forms.StatusBarPanel stbTime;
        public WeifenLuo.WinFormsUI.Docking.DockPanel DockManager;
		private System.ComponentModel.IContainer components;


		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
                PrepareForClose();
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}
		
		#region Windows Form Designer generated code
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.ToolStrip tblSimulation;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Server));
            WeifenLuo.WinFormsUI.Docking.DockPanelSkin dockPanelSkin1 = new WeifenLuo.WinFormsUI.Docking.DockPanelSkin();
            WeifenLuo.WinFormsUI.Docking.AutoHideStripSkin autoHideStripSkin1 = new WeifenLuo.WinFormsUI.Docking.AutoHideStripSkin();
            WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient1 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient1 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPaneStripSkin dockPaneStripSkin1 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripSkin();
            WeifenLuo.WinFormsUI.Docking.DockPaneStripGradient dockPaneStripGradient1 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient2 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient2 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient3 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPaneStripToolWindowGradient dockPaneStripToolWindowGradient1 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripToolWindowGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient4 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient5 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient3 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient6 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient7 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.btnSimulationRun = new System.Windows.Forms.ToolStripButton();
            this.btnSimulationStop = new System.Windows.Forms.ToolStripButton();
            this.btnSimulationClear = new System.Windows.Forms.ToolStripButton();
            this.ToolbarImages = new System.Windows.Forms.ImageList(this.components);
            this.mainMenu1 = new System.Windows.Forms.MainMenu(this.components);
            this.toolBarButton1 = new System.Windows.Forms.ToolBarButton();
            this.stbStatusBar = new System.Windows.Forms.StatusBar();
            this.stbRep = new System.Windows.Forms.StatusBarPanel();
            this.stbSpacer = new System.Windows.Forms.StatusBarPanel();
            this.stbTime = new System.Windows.Forms.StatusBarPanel();
            this.DockManager = new WeifenLuo.WinFormsUI.Docking.DockPanel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.mnuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.addScenarioToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuFileExit = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSimulation = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSimulationClear = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSimulationRun = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSimulationStop = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuRunTests = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.changeSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            tblSimulation = new System.Windows.Forms.ToolStrip();
            tblSimulation.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.stbRep)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.stbSpacer)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.stbTime)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.toolStripContainer1.ContentPanel.SuspendLayout();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tblSimulation
            // 
            tblSimulation.Dock = System.Windows.Forms.DockStyle.None;
            tblSimulation.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            tblSimulation.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSeparator3,
            this.btnSimulationRun,
            this.btnSimulationStop,
            this.btnSimulationClear});
            tblSimulation.Location = new System.Drawing.Point(0, 24);
            tblSimulation.Name = "tblSimulation";
            tblSimulation.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            tblSimulation.Size = new System.Drawing.Size(992, 25);
            tblSimulation.Stretch = true;
            tblSimulation.TabIndex = 17;
            tblSimulation.Text = "s";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // btnSimulationRun
            // 
            this.btnSimulationRun.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSimulationRun.Image = global::FRESCO_Server.Properties.Resources.SimulationRun;
            this.btnSimulationRun.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(250)))), ((int)(((byte)(225)))));
            this.btnSimulationRun.Name = "btnSimulationRun";
            this.btnSimulationRun.Size = new System.Drawing.Size(23, 22);
            this.btnSimulationRun.Text = "Run Simulation";
            this.btnSimulationRun.Click += new System.EventHandler(this.SimulationRun_Click);
            // 
            // btnSimulationStop
            // 
            this.btnSimulationStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSimulationStop.Image = global::FRESCO_Server.Properties.Resources.SimulationStop;
            this.btnSimulationStop.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(250)))), ((int)(((byte)(225)))));
            this.btnSimulationStop.Name = "btnSimulationStop";
            this.btnSimulationStop.Size = new System.Drawing.Size(23, 22);
            this.btnSimulationStop.Text = "Stop Simulation";
            this.btnSimulationStop.Click += new System.EventHandler(this.SimulationStop_Click);
            // 
            // btnSimulationClear
            // 
            this.btnSimulationClear.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnSimulationClear.Image = global::FRESCO_Server.Properties.Resources.SimulationClear;
            this.btnSimulationClear.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(250)))), ((int)(((byte)(225)))));
            this.btnSimulationClear.Name = "btnSimulationClear";
            this.btnSimulationClear.Size = new System.Drawing.Size(23, 22);
            this.btnSimulationClear.Text = "Clear Simulation";
            this.btnSimulationClear.Click += new System.EventHandler(this.SimulationClear_Click);
            // 
            // ToolbarImages
            // 
            this.ToolbarImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ToolbarImages.ImageStream")));
            this.ToolbarImages.TransparentColor = System.Drawing.Color.Transparent;
            this.ToolbarImages.Images.SetKeyName(0, "Open");
            this.ToolbarImages.Images.SetKeyName(1, "Save");
            this.ToolbarImages.Images.SetKeyName(2, "ViewerProgress");
            this.ToolbarImages.Images.SetKeyName(3, "EditorFIF");
            this.ToolbarImages.Images.SetKeyName(4, "ViewerFire");
            this.ToolbarImages.Images.SetKeyName(5, "ViewerFireAge");
            this.ToolbarImages.Images.SetKeyName(6, "ViewerFireSupression");
            this.ToolbarImages.Images.SetKeyName(7, "ViewerAge");
            this.ToolbarImages.Images.SetKeyName(8, "ViewerSite");
            this.ToolbarImages.Images.SetKeyName(9, "ViewerSubCanopy");
            this.ToolbarImages.Images.SetKeyName(10, "ViewerVegetation");
            this.ToolbarImages.Images.SetKeyName(11, "ViewerReplay");
            this.ToolbarImages.Images.SetKeyName(12, "Clear");
            this.ToolbarImages.Images.SetKeyName(13, "Setup");
            this.ToolbarImages.Images.SetKeyName(14, "Run");
            this.ToolbarImages.Images.SetKeyName(15, "Stop");
            // 
            // toolBarButton1
            // 
            this.toolBarButton1.Name = "toolBarButton1";
            this.toolBarButton1.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
            // 
            // stbStatusBar
            // 
            this.stbStatusBar.Location = new System.Drawing.Point(0, 592);
            this.stbStatusBar.Name = "stbStatusBar";
            this.stbStatusBar.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
            this.stbRep,
            this.stbSpacer,
            this.stbTime});
            this.stbStatusBar.ShowPanels = true;
            this.stbStatusBar.Size = new System.Drawing.Size(992, 26);
            this.stbStatusBar.TabIndex = 8;
            // 
            // stbRep
            // 
            this.stbRep.BorderStyle = System.Windows.Forms.StatusBarPanelBorderStyle.None;
            this.stbRep.MinWidth = 75;
            this.stbRep.Name = "stbRep";
            this.stbRep.Width = 150;
            // 
            // stbSpacer
            // 
            this.stbSpacer.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring;
            this.stbSpacer.BorderStyle = System.Windows.Forms.StatusBarPanelBorderStyle.None;
            this.stbSpacer.Name = "stbSpacer";
            this.stbSpacer.Style = System.Windows.Forms.StatusBarPanelStyle.OwnerDraw;
            this.stbSpacer.Width = 676;
            // 
            // stbTime
            // 
            this.stbTime.Alignment = System.Windows.Forms.HorizontalAlignment.Right;
            this.stbTime.BorderStyle = System.Windows.Forms.StatusBarPanelBorderStyle.None;
            this.stbTime.MinWidth = 100;
            this.stbTime.Name = "stbTime";
            this.stbTime.Width = 150;
            // 
            // DockManager
            // 
            this.DockManager.ActiveAutoHideContent = null;
            this.DockManager.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DockManager.DockBackColor = System.Drawing.SystemColors.Control;
            this.DockManager.DocumentStyle = WeifenLuo.WinFormsUI.Docking.DocumentStyle.DockingWindow;
            this.DockManager.Font = new System.Drawing.Font("Tahoma", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.World);
            this.DockManager.Location = new System.Drawing.Point(0, 0);
            this.DockManager.Name = "DockManager";
            this.DockManager.ShowDocumentIcon = true;
            this.DockManager.Size = new System.Drawing.Size(992, 543);
            dockPanelGradient1.EndColor = System.Drawing.SystemColors.ControlLight;
            dockPanelGradient1.StartColor = System.Drawing.SystemColors.ControlLight;
            autoHideStripSkin1.DockStripGradient = dockPanelGradient1;
            tabGradient1.EndColor = System.Drawing.SystemColors.Control;
            tabGradient1.StartColor = System.Drawing.SystemColors.Control;
            tabGradient1.TextColor = System.Drawing.SystemColors.ControlDarkDark;
            autoHideStripSkin1.TabGradient = tabGradient1;
            dockPanelSkin1.AutoHideStripSkin = autoHideStripSkin1;
            tabGradient2.EndColor = System.Drawing.SystemColors.ControlLightLight;
            tabGradient2.StartColor = System.Drawing.SystemColors.ControlLightLight;
            tabGradient2.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripGradient1.ActiveTabGradient = tabGradient2;
            dockPanelGradient2.EndColor = System.Drawing.SystemColors.Control;
            dockPanelGradient2.StartColor = System.Drawing.SystemColors.Control;
            dockPaneStripGradient1.DockStripGradient = dockPanelGradient2;
            tabGradient3.EndColor = System.Drawing.SystemColors.ControlLight;
            tabGradient3.StartColor = System.Drawing.SystemColors.ControlLight;
            tabGradient3.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripGradient1.InactiveTabGradient = tabGradient3;
            dockPaneStripSkin1.DocumentGradient = dockPaneStripGradient1;
            tabGradient4.EndColor = System.Drawing.SystemColors.ActiveCaption;
            tabGradient4.LinearGradientMode = System.Drawing.Drawing2D.LinearGradientMode.Vertical;
            tabGradient4.StartColor = System.Drawing.SystemColors.GradientActiveCaption;
            tabGradient4.TextColor = System.Drawing.SystemColors.ActiveCaptionText;
            dockPaneStripToolWindowGradient1.ActiveCaptionGradient = tabGradient4;
            tabGradient5.EndColor = System.Drawing.SystemColors.Control;
            tabGradient5.StartColor = System.Drawing.SystemColors.Control;
            tabGradient5.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripToolWindowGradient1.ActiveTabGradient = tabGradient5;
            dockPanelGradient3.EndColor = System.Drawing.SystemColors.ControlLight;
            dockPanelGradient3.StartColor = System.Drawing.SystemColors.ControlLight;
            dockPaneStripToolWindowGradient1.DockStripGradient = dockPanelGradient3;
            tabGradient6.EndColor = System.Drawing.SystemColors.GradientInactiveCaption;
            tabGradient6.LinearGradientMode = System.Drawing.Drawing2D.LinearGradientMode.Vertical;
            tabGradient6.StartColor = System.Drawing.SystemColors.GradientInactiveCaption;
            tabGradient6.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripToolWindowGradient1.InactiveCaptionGradient = tabGradient6;
            tabGradient7.EndColor = System.Drawing.Color.Transparent;
            tabGradient7.StartColor = System.Drawing.Color.Transparent;
            tabGradient7.TextColor = System.Drawing.SystemColors.ControlDarkDark;
            dockPaneStripToolWindowGradient1.InactiveTabGradient = tabGradient7;
            dockPaneStripSkin1.ToolWindowGradient = dockPaneStripToolWindowGradient1;
            dockPanelSkin1.DockPaneStripSkin = dockPaneStripSkin1;
            this.DockManager.Skin = dockPanelSkin1;
            this.DockManager.TabIndex = 10;
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.SystemColors.Control;
            this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuFile,
            this.mnuSimulation,
            this.settingsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(992, 24);
            this.menuStrip1.TabIndex = 20;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // mnuFile
            // 
            this.mnuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addScenarioToolStripMenuItem,
            this.toolStripSeparator1,
            this.mnuFileExit});
            this.mnuFile.Name = "mnuFile";
            this.mnuFile.Size = new System.Drawing.Size(35, 20);
            this.mnuFile.Text = "File";
            // 
            // addScenarioToolStripMenuItem
            // 
            this.addScenarioToolStripMenuItem.Image = global::FRESCO_Server.Properties.Resources.document_open;
            this.addScenarioToolStripMenuItem.Name = "addScenarioToolStripMenuItem";
            this.addScenarioToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.addScenarioToolStripMenuItem.Text = "Add Scenario";
            this.addScenarioToolStripMenuItem.Click += new System.EventHandler(this.FileOpen_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(145, 6);
            // 
            // mnuFileExit
            // 
            this.mnuFileExit.Name = "mnuFileExit";
            this.mnuFileExit.Size = new System.Drawing.Size(148, 22);
            this.mnuFileExit.Text = "Exit";
            this.mnuFileExit.Click += new System.EventHandler(this.FileExit_Click);
            // 
            // mnuSimulation
            // 
            this.mnuSimulation.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuSimulationClear,
            this.mnuSimulationRun,
            this.mnuSimulationStop,
            this.mnuRunTests});
            this.mnuSimulation.Name = "mnuSimulation";
            this.mnuSimulation.Size = new System.Drawing.Size(67, 20);
            this.mnuSimulation.Text = "Simulation";
            // 
            // mnuSimulationClear
            // 
            this.mnuSimulationClear.Image = global::FRESCO_Server.Properties.Resources.SimulationClear;
            this.mnuSimulationClear.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(250)))), ((int)(((byte)(225)))));
            this.mnuSimulationClear.Name = "mnuSimulationClear";
            this.mnuSimulationClear.Size = new System.Drawing.Size(133, 22);
            this.mnuSimulationClear.Text = "Clear";
            this.mnuSimulationClear.Click += new System.EventHandler(this.SimulationClear_Click);
            // 
            // mnuSimulationRun
            // 
            this.mnuSimulationRun.Image = global::FRESCO_Server.Properties.Resources.SimulationRun;
            this.mnuSimulationRun.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(250)))), ((int)(((byte)(225)))));
            this.mnuSimulationRun.Name = "mnuSimulationRun";
            this.mnuSimulationRun.Size = new System.Drawing.Size(133, 22);
            this.mnuSimulationRun.Text = "Run";
            this.mnuSimulationRun.Click += new System.EventHandler(this.SimulationRun_Click);
            // 
            // mnuSimulationStop
            // 
            this.mnuSimulationStop.Image = global::FRESCO_Server.Properties.Resources.SimulationStop;
            this.mnuSimulationStop.ImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(250)))), ((int)(((byte)(225)))));
            this.mnuSimulationStop.Name = "mnuSimulationStop";
            this.mnuSimulationStop.Size = new System.Drawing.Size(133, 22);
            this.mnuSimulationStop.Text = "Stop";
            this.mnuSimulationStop.Click += new System.EventHandler(this.SimulationStop_Click);
            // 
            // mnuRunTests
            // 
            this.mnuRunTests.Name = "mnuRunTests";
            this.mnuRunTests.Size = new System.Drawing.Size(133, 22);
            this.mnuRunTests.Text = "Run Tests";
            this.mnuRunTests.Visible = false;
            // 
            // settingsToolStripMenuItem
            // 
            this.settingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.changeSettingsToolStripMenuItem});
            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(58, 20);
            this.settingsToolStripMenuItem.Text = "Settings";
            // 
            // changeSettingsToolStripMenuItem
            // 
            this.changeSettingsToolStripMenuItem.Name = "changeSettingsToolStripMenuItem";
            this.changeSettingsToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.changeSettingsToolStripMenuItem.Text = "Edit Settings...";
            this.changeSettingsToolStripMenuItem.Click += new System.EventHandler(this.changeSettingsToolStripMenuItem_Click);
            // 
            // toolStripContainer1
            // 
            this.toolStripContainer1.BottomToolStripPanelVisible = false;
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Controls.Add(this.DockManager);
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(992, 543);
            this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer1.LeftToolStripPanelVisible = false;
            this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.RightToolStripPanelVisible = false;
            this.toolStripContainer1.Size = new System.Drawing.Size(992, 592);
            this.toolStripContainer1.TabIndex = 23;
            this.toolStripContainer1.Text = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.BackColor = System.Drawing.SystemColors.Control;
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.menuStrip1);
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(tblSimulation);
            // 
            // progressBar
            // 
            this.progressBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar.Location = new System.Drawing.Point(700, 600);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(121, 12);
            this.progressBar.TabIndex = 25;
            // 
            // Server
            // 
            this.ClientSize = new System.Drawing.Size(992, 618);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.toolStripContainer1);
            this.Controls.Add(this.stbStatusBar);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.Menu = this.mainMenu1;
            this.MinimumSize = new System.Drawing.Size(300, 300);
            this.Name = "Server";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FRESCO Server";
            this.Shown += new System.EventHandler(this.Server_Shown);
            this.Closing += new System.ComponentModel.CancelEventHandler(this.Server_Closing);
            tblSimulation.ResumeLayout(false);
            tblSimulation.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.stbRep)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.stbSpacer)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.stbTime)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.toolStripContainer1.ContentPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.ResumeLayout(false);

		}
		#endregion

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Server());
        }
		#endregion

        private ToolStripButton btnSimulationClear;
		private ToolStripButton btnSimulationRun;
        private ToolStripButton btnSimulationStop;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem mnuFile;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripMenuItem mnuFileExit;
        private ToolStripMenuItem mnuSimulation;
        private ToolStripMenuItem mnuSimulationClear;
        private ToolStripMenuItem mnuSimulationRun;
        private ToolStripMenuItem mnuSimulationStop;
        private ToolStripContainer toolStripContainer1;
        private ToolStripSeparator toolStripSeparator3;
        private ProgressBar progressBar;
        private ToolStripMenuItem mnuRunTests;
        private ToolStripMenuItem addScenarioToolStripMenuItem;
        private ToolStripMenuItem settingsToolStripMenuItem;
        private ToolStripMenuItem changeSettingsToolStripMenuItem;
	}
}