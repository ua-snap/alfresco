using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;


namespace FRESCO_Server
{
	public partial class Editor : FRESCOForm
	{
		public							Editor()
		{
			InitializeComponent();
		}
		public							Editor(Global G) : base(G)
		{
			this.MdiParent	= G.Main;
			InitializeComponent();
		}
		public virtual void				Save()			
		{ 
		}
		public virtual void				Undo()			
		{ 
		}
		public override void			MenuSelection(String Selection)  
		{
		}
		protected virtual void			BeginEdit(object sender, System.EventArgs e)
		{
			if (FormState==EnumFormState.DISPLAY || FormState==EnumFormState.EDIT) //|| FormState==EnumFormState.ADDNEW)
			{
				if (FormState==EnumFormState.DISPLAY) FormState = EnumFormState.EDIT;
				HasChanges = true;
				SetToolbar();
			}
		}
		protected void					ValidationDisplayError(string Message, string SystemMessage,Control Control)
		{
			Global.Main.ViewerProgress.AddSystemError(Message,SystemMessage);
			Control.Focus();
		}
	}
}
