using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;

using System.Windows.Forms;


namespace FRESCO_Server
{
	partial class Viewer
	{
		#region Designer Code
		private	System.ComponentModel.IContainer components = null;
		private void				InitializeComponent()
		{
            this.SuspendLayout();
            // 
            // Viewer
            // 
            this.ClientSize = new System.Drawing.Size(292, 273);
            this.Name = "Viewer";
            this.TabText = "Viewer";
            this.Text = "Viewer";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
            this.ResumeLayout(false);

		}
		protected override void		Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}
		#endregion
	}
}

