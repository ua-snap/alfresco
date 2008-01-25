using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices; //For ATL Events. (might not be needed)

namespace FRESCO_Server
{
	partial class ViewerProgress
	{
		#region Designer Code

		private void ToolBar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)				{MenuSelection(e.Button.Tag.ToString());}

		protected override void Dispose( bool disposing )
		{
			if (disposing && (components != null))
			{
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ViewerProgress));
            this.txbOutput = new AssisiControls.AssisiRichTextBox();
            this.ImageList = new System.Windows.Forms.ImageList(this.components);
            this.ToolBar = new System.Windows.Forms.ToolBar();
            this.btnClear = new System.Windows.Forms.ToolBarButton();
            this.SuspendLayout();
            // 
            // txbOutput
            // 
            this.txbOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txbOutput.HideSelection = false;
            this.txbOutput.IsRequired = false;
            this.txbOutput.Location = new System.Drawing.Point(2, 30);
            this.txbOutput.Margin = new System.Windows.Forms.Padding(0);
            this.txbOutput.Name = "txbOutput";
            this.txbOutput.Size = new System.Drawing.Size(443, 178);
            this.txbOutput.TabIndex = 0;
            this.txbOutput.Text = "";
            this.txbOutput.WordWrap = false;
            this.txbOutput.LinkClicked += new System.Windows.Forms.LinkClickedEventHandler(this.txbOutput_LinkClicked);
            // 
            // ImageList
            // 
            this.ImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageList.ImageStream")));
            this.ImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.ImageList.Images.SetKeyName(0, "SimulationClear.bmp");
            // 
            // ToolBar
            // 
            this.ToolBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.ToolBar.Appearance = System.Windows.Forms.ToolBarAppearance.Flat;
            this.ToolBar.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
            this.btnClear});
            this.ToolBar.ButtonSize = new System.Drawing.Size(16, 16);
            this.ToolBar.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.ToolBar.Dock = System.Windows.Forms.DockStyle.None;
            this.ToolBar.DropDownArrows = true;
            this.ToolBar.ImageList = this.ImageList;
            this.ToolBar.Location = new System.Drawing.Point(2, 0);
            this.ToolBar.Margin = new System.Windows.Forms.Padding(0);
            this.ToolBar.Name = "ToolBar";
            this.ToolBar.ShowToolTips = true;
            this.ToolBar.Size = new System.Drawing.Size(443, 28);
            this.ToolBar.TabIndex = 9;
            this.ToolBar.Wrappable = false;
            this.ToolBar.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.ToolBar_ButtonClick);
            // 
            // btnClear
            // 
            this.btnClear.ImageKey = "SimulationClear.bmp";
            this.btnClear.Name = "btnClear";
            this.btnClear.Tag = "Clear";
            this.btnClear.ToolTipText = "Clear Viewer";
            // 
            // ViewerProgress
            // 
            this.ClientSize = new System.Drawing.Size(447, 210);
            this.CloseButton = false;
            this.Controls.Add(this.ToolBar);
            this.Controls.Add(this.txbOutput);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Location = new System.Drawing.Point(0, 500);
            this.Name = "ViewerProgress";
            this.Padding = new System.Windows.Forms.Padding(2, 0, 2, 2);
            this.TabText = "Progress Viewer";
            this.Text = "Progress Viewer";
            this.ResumeLayout(false);
            this.PerformLayout();

        }
		#endregion
		
		private AssisiControls.AssisiRichTextBox txbOutput;

		#endregion
		private ImageList ImageList;
		private IContainer components;
		private ToolBar ToolBar;
		private ToolBarButton btnClear;

	}
}
