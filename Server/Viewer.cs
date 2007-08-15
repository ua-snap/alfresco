using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;

using System.Windows.Forms;


namespace FRESCO_Server
{
	public partial class Viewer : Editor
	{
		public							Viewer()
		{
			InitializeComponent();
		}
		public							Viewer(Global G) : base(G)
		{
			InitializeComponent();
			//Global.FRESCO.AfterYearEnd		+= new FRESCOOL.AfterYearEndEventHandler(OnAfterYearEnd);
		}
		protected virtual void			OnAfterYearEnd()
		{
			Refresh();
		}

		protected override void			OnFormClosed(object sender, FormClosedEventArgs e)
		{
			//Global.FRESCO.AfterYearEnd		-= new FRESCOOL.AfterYearEndEventHandler(OnAfterYearEnd);
		}
	}
}

