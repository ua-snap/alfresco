namespace FRESCO_Server
{
    partial class ClientMonitor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                DoDisposeWork();
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ClientMonitor));
            this.lblClientLog = new System.Windows.Forms.Label();
            this.clientContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.mnuViewLog = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.lblNoClientsConnected = new System.Windows.Forms.Label();
            this.clientGrid = new DevExpress.XtraGrid.GridControl();
            this.clientListBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.gridView1 = new DevExpress.XtraGrid.Views.Grid.GridView();
            this.colID = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colStateAsString = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colRepsCompletedAsString = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colRepAsString = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colYearAsString = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colIPAddress = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colPort = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colHostName = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colPercentCompleted = new DevExpress.XtraGrid.Columns.GridColumn();
            this.repositoryItemProgressBar3 = new DevExpress.XtraEditors.Repository.RepositoryItemProgressBar();
            this.repositoryItemProgressBar1 = new DevExpress.XtraEditors.Repository.RepositoryItemProgressBar();
            this.repositoryItemProgressBar2 = new DevExpress.XtraEditors.Repository.RepositoryItemProgressBar();
            this.clientLogHideButton = new System.Windows.Forms.Button();
            this.clientLogDisplay = new AssisiControls.AssisiRichTextBox();
            this.clientContextMenu.SuspendLayout();
            this.toolStripContainer1.ContentPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.clientGrid)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.clientListBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemProgressBar3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemProgressBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemProgressBar2)).BeginInit();
            this.SuspendLayout();
            // 
            // lblClientLog
            // 
            this.lblClientLog.AutoSize = true;
            this.lblClientLog.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblClientLog.Location = new System.Drawing.Point(3, 8);
            this.lblClientLog.Name = "lblClientLog";
            this.lblClientLog.Size = new System.Drawing.Size(68, 13);
            this.lblClientLog.TabIndex = 1;
            this.lblClientLog.Text = "Client Log:";
            // 
            // clientContextMenu
            // 
            this.clientContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuViewLog});
            this.clientContextMenu.Name = "clientContextMenu";
            this.clientContextMenu.Size = new System.Drawing.Size(128, 26);
            // 
            // mnuViewLog
            // 
            this.mnuViewLog.Name = "mnuViewLog";
            this.mnuViewLog.Size = new System.Drawing.Size(127, 22);
            this.mnuViewLog.Text = "View Log";
            this.mnuViewLog.Click += new System.EventHandler(this.mnuViewLog_Click);
            // 
            // toolStripContainer1
            // 
            this.toolStripContainer1.BottomToolStripPanelVisible = false;
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Controls.Add(this.splitContainer);
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(748, 434);
            this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer1.LeftToolStripPanelVisible = false;
            this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.RightToolStripPanelVisible = false;
            this.toolStripContainer1.Size = new System.Drawing.Size(748, 459);
            this.toolStripContainer1.TabIndex = 3;
            this.toolStripContainer1.Text = "toolStripContainer1";
            // 
            // splitContainer
            // 
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 0);
            this.splitContainer.Name = "splitContainer";
            this.splitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.lblNoClientsConnected);
            this.splitContainer.Panel1.Controls.Add(this.clientGrid);
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.clientLogHideButton);
            this.splitContainer.Panel2.Controls.Add(this.clientLogDisplay);
            this.splitContainer.Panel2.Controls.Add(this.lblClientLog);
            this.splitContainer.Size = new System.Drawing.Size(748, 434);
            this.splitContainer.SplitterDistance = 249;
            this.splitContainer.TabIndex = 3;
            // 
            // lblNoClientsConnected
            // 
            this.lblNoClientsConnected.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblNoClientsConnected.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.lblNoClientsConnected.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblNoClientsConnected.ForeColor = System.Drawing.Color.DarkRed;
            this.lblNoClientsConnected.Location = new System.Drawing.Point(6, 62);
            this.lblNoClientsConnected.Name = "lblNoClientsConnected";
            this.lblNoClientsConnected.Size = new System.Drawing.Size(730, 18);
            this.lblNoClientsConnected.TabIndex = 4;
            this.lblNoClientsConnected.Text = "No Clients Connected";
            this.lblNoClientsConnected.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // clientGrid
            // 
            this.clientGrid.DataSource = this.clientListBindingSource;
            this.clientGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.clientGrid.EmbeddedNavigator.Name = "";
            this.clientGrid.Location = new System.Drawing.Point(0, 0);
            this.clientGrid.LookAndFeel.Style = DevExpress.LookAndFeel.LookAndFeelStyle.Flat;
            this.clientGrid.LookAndFeel.UseDefaultLookAndFeel = false;
            this.clientGrid.LookAndFeel.UseWindowsXPTheme = true;
            this.clientGrid.MainView = this.gridView1;
            this.clientGrid.Name = "clientGrid";
            this.clientGrid.RepositoryItems.AddRange(new DevExpress.XtraEditors.Repository.RepositoryItem[] {
            this.repositoryItemProgressBar1,
            this.repositoryItemProgressBar2,
            this.repositoryItemProgressBar3});
            this.clientGrid.Size = new System.Drawing.Size(748, 249);
            this.clientGrid.TabIndex = 3;
            this.clientGrid.ViewCollection.AddRange(new DevExpress.XtraGrid.Views.Base.BaseView[] {
            this.gridView1});
            this.clientGrid.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.clientGrid_MouseDoubleClick);
            this.clientGrid.MouseClick += new System.Windows.Forms.MouseEventHandler(this.clientGrid_MouseClick);
            this.clientGrid.KeyDown += new System.Windows.Forms.KeyEventHandler(this.clientGrid_KeyDown);
            // 
            // clientListBindingSource
            // 
            this.clientListBindingSource.DataSource = typeof(FRESCO_Server.ClientList);
            // 
            // gridView1
            // 
            this.gridView1.Columns.AddRange(new DevExpress.XtraGrid.Columns.GridColumn[] {
            this.colID,
            this.colStateAsString,
            this.colRepsCompletedAsString,
            this.colRepAsString,
            this.colYearAsString,
            this.colIPAddress,
            this.colPort,
            this.colHostName,
            this.colPercentCompleted});
            this.gridView1.GridControl = this.clientGrid;
            this.gridView1.Name = "gridView1";
            this.gridView1.OptionsBehavior.AutoSelectAllInEditor = false;
            this.gridView1.OptionsBehavior.Editable = false;
            this.gridView1.OptionsCustomization.AllowFilter = false;
            this.gridView1.OptionsCustomization.AllowGroup = false;
            this.gridView1.OptionsCustomization.AllowSort = false;
            this.gridView1.OptionsMenu.EnableColumnMenu = false;
            this.gridView1.OptionsMenu.EnableGroupPanelMenu = false;
            this.gridView1.OptionsNavigation.AutoMoveRowFocus = false;
            this.gridView1.OptionsNavigation.UseTabKey = false;
            this.gridView1.OptionsView.ShowGroupPanel = false;
            this.gridView1.OptionsView.ShowIndicator = false;
            // 
            // colID
            // 
            this.colID.Caption = "Client ID";
            this.colID.FieldName = "ID";
            this.colID.Name = "colID";
            this.colID.OptionsColumn.AllowFocus = false;
            this.colID.OptionsColumn.FixedWidth = true;
            this.colID.OptionsColumn.ReadOnly = true;
            this.colID.Visible = true;
            this.colID.VisibleIndex = 0;
            this.colID.Width = 54;
            // 
            // colStateAsString
            // 
            this.colStateAsString.Caption = "Status";
            this.colStateAsString.FieldName = "StateAsString";
            this.colStateAsString.Name = "colStateAsString";
            this.colStateAsString.OptionsColumn.AllowFocus = false;
            this.colStateAsString.OptionsColumn.FixedWidth = true;
            this.colStateAsString.OptionsColumn.ReadOnly = true;
            this.colStateAsString.Visible = true;
            this.colStateAsString.VisibleIndex = 1;
            this.colStateAsString.Width = 98;
            // 
            // colRepsCompletedAsString
            // 
            this.colRepsCompletedAsString.Caption = "Reps Completed";
            this.colRepsCompletedAsString.FieldName = "RepsCompletedAsString";
            this.colRepsCompletedAsString.Name = "colRepsCompletedAsString";
            this.colRepsCompletedAsString.OptionsColumn.AllowFocus = false;
            this.colRepsCompletedAsString.OptionsColumn.FixedWidth = true;
            this.colRepsCompletedAsString.OptionsColumn.ReadOnly = true;
            this.colRepsCompletedAsString.Visible = true;
            this.colRepsCompletedAsString.VisibleIndex = 2;
            this.colRepsCompletedAsString.Width = 88;
            // 
            // colRepAsString
            // 
            this.colRepAsString.Caption = "Rep";
            this.colRepAsString.FieldName = "RepAsString";
            this.colRepAsString.Name = "colRepAsString";
            this.colRepAsString.OptionsColumn.AllowFocus = false;
            this.colRepAsString.OptionsColumn.FixedWidth = true;
            this.colRepAsString.OptionsColumn.ReadOnly = true;
            this.colRepAsString.Visible = true;
            this.colRepAsString.VisibleIndex = 3;
            this.colRepAsString.Width = 39;
            // 
            // colYearAsString
            // 
            this.colYearAsString.Caption = "Year";
            this.colYearAsString.FieldName = "YearAsString";
            this.colYearAsString.Name = "colYearAsString";
            this.colYearAsString.OptionsColumn.AllowFocus = false;
            this.colYearAsString.OptionsColumn.FixedWidth = true;
            this.colYearAsString.OptionsColumn.ReadOnly = true;
            this.colYearAsString.Visible = true;
            this.colYearAsString.VisibleIndex = 4;
            this.colYearAsString.Width = 42;
            // 
            // colIPAddress
            // 
            this.colIPAddress.Caption = "IP Address";
            this.colIPAddress.FieldName = "IPAddress";
            this.colIPAddress.Name = "colIPAddress";
            this.colIPAddress.OptionsColumn.AllowFocus = false;
            this.colIPAddress.OptionsColumn.FixedWidth = true;
            this.colIPAddress.OptionsColumn.ReadOnly = true;
            this.colIPAddress.Visible = true;
            this.colIPAddress.VisibleIndex = 5;
            this.colIPAddress.Width = 90;
            // 
            // colPort
            // 
            this.colPort.Caption = "Port";
            this.colPort.FieldName = "Port";
            this.colPort.Name = "colPort";
            this.colPort.OptionsColumn.AllowFocus = false;
            this.colPort.OptionsColumn.FixedWidth = true;
            this.colPort.OptionsColumn.ReadOnly = true;
            this.colPort.Visible = true;
            this.colPort.VisibleIndex = 6;
            this.colPort.Width = 48;
            // 
            // colHostName
            // 
            this.colHostName.Caption = "Hostname";
            this.colHostName.FieldName = "HostName";
            this.colHostName.Name = "colHostName";
            this.colHostName.OptionsColumn.AllowFocus = false;
            this.colHostName.OptionsColumn.FixedWidth = true;
            this.colHostName.Visible = true;
            this.colHostName.VisibleIndex = 7;
            this.colHostName.Width = 130;
            // 
            // colPercentCompleted
            // 
            this.colPercentCompleted.Caption = "Progress";
            this.colPercentCompleted.ColumnEdit = this.repositoryItemProgressBar3;
            this.colPercentCompleted.FieldName = "PercentCompleted";
            this.colPercentCompleted.Name = "colPercentCompleted";
            this.colPercentCompleted.OptionsColumn.AllowEdit = false;
            this.colPercentCompleted.OptionsColumn.AllowFocus = false;
            this.colPercentCompleted.OptionsColumn.ReadOnly = true;
            this.colPercentCompleted.Visible = true;
            this.colPercentCompleted.VisibleIndex = 8;
            this.colPercentCompleted.Width = 157;
            // 
            // repositoryItemProgressBar3
            // 
            this.repositoryItemProgressBar3.LookAndFeel.Style = DevExpress.LookAndFeel.LookAndFeelStyle.Flat;
            this.repositoryItemProgressBar3.Name = "repositoryItemProgressBar3";
            // 
            // repositoryItemProgressBar1
            // 
            this.repositoryItemProgressBar1.Name = "repositoryItemProgressBar1";
            // 
            // repositoryItemProgressBar2
            // 
            this.repositoryItemProgressBar2.Name = "repositoryItemProgressBar2";
            // 
            // clientLogHideButton
            // 
            this.clientLogHideButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.clientLogHideButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.clientLogHideButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F);
            this.clientLogHideButton.Location = new System.Drawing.Point(674, 1);
            this.clientLogHideButton.Name = "clientLogHideButton";
            this.clientLogHideButton.Size = new System.Drawing.Size(65, 23);
            this.clientLogHideButton.TabIndex = 4;
            this.clientLogHideButton.Text = "Hide Log";
            this.clientLogHideButton.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.clientLogHideButton.UseVisualStyleBackColor = true;
            this.clientLogHideButton.Click += new System.EventHandler(this.logHideButton_Click);
            // 
            // clientLogDisplay
            // 
            this.clientLogDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.clientLogDisplay.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.clientLogDisplay.IsRequired = false;
            this.clientLogDisplay.Location = new System.Drawing.Point(3, 26);
            this.clientLogDisplay.Name = "clientLogDisplay";
            this.clientLogDisplay.Size = new System.Drawing.Size(742, 152);
            this.clientLogDisplay.TabIndex = 3;
            this.clientLogDisplay.Text = "";
            this.clientLogDisplay.WordWrap = false;
            // 
            // ClientMonitor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(748, 459);
            this.CloseButton = false;
            this.Controls.Add(this.toolStripContainer1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ClientMonitor";
            this.TabText = "Client Manager";
            this.Text = "Client Manager";
            this.Shown += new System.EventHandler(this.ClientManager_Shown);
            this.clientContextMenu.ResumeLayout(false);
            this.toolStripContainer1.ContentPanel.ResumeLayout(false);
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.Panel2.PerformLayout();
            this.splitContainer.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.clientGrid)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.clientListBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemProgressBar3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemProgressBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemProgressBar2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label lblClientLog;
        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.SplitContainer splitContainer;
        private System.Windows.Forms.ContextMenuStrip clientContextMenu;
        private System.Windows.Forms.ToolStripMenuItem mnuViewLog;
        private AssisiControls.AssisiRichTextBox clientLogDisplay;
        private System.Windows.Forms.Button clientLogHideButton;
        private DevExpress.XtraGrid.GridControl clientGrid;
        private System.Windows.Forms.BindingSource clientListBindingSource;
        private DevExpress.XtraEditors.Repository.RepositoryItemProgressBar repositoryItemProgressBar1;
        private DevExpress.XtraGrid.Views.Grid.GridView gridView1;
        private DevExpress.XtraEditors.Repository.RepositoryItemProgressBar repositoryItemProgressBar2;
        private DevExpress.XtraGrid.Columns.GridColumn colID;
        private DevExpress.XtraGrid.Columns.GridColumn colStateAsString;
        private DevExpress.XtraGrid.Columns.GridColumn colPercentCompleted;
        private DevExpress.XtraEditors.Repository.RepositoryItemProgressBar repositoryItemProgressBar3;
        private DevExpress.XtraGrid.Columns.GridColumn colRepsCompletedAsString;
        private DevExpress.XtraGrid.Columns.GridColumn colRepAsString;
        private DevExpress.XtraGrid.Columns.GridColumn colYearAsString;
        private DevExpress.XtraGrid.Columns.GridColumn colIPAddress;
        private DevExpress.XtraGrid.Columns.GridColumn colPort;
        private DevExpress.XtraGrid.Columns.GridColumn colHostName;
        private System.Windows.Forms.Label lblNoClientsConnected;
    }
}