using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using AssisiControls;

namespace FRESCO_Server
{
    public partial class FRESCOForm : AssisiForm
    {
		protected Global				Global;
		protected enum					EnumFormState
		{
			REFRESH,
			DISPLAY,
			EDIT
		};
		protected EnumFormState			FormState;
		
		public							FRESCOForm()
		{
			InitializeComponent();
            Global.Instance.ProgramStateEvent += new ProgramStateEventHandler(ProgramStateChanged);
            //Global.FRESCO.AfterSetup		+= new FRESCOOL.AfterSetupEventHandler(OnAfterSetup);
            FormClosed += new System.Windows.Forms.FormClosedEventHandler(OnFormClosed);
            Shown += new System.EventHandler(OnShown);
            Activated += new System.EventHandler(OnActivated);
            IsHidden = true;
		}
		public							FRESCOForm(Global G)
		{
			InitializeComponent();
			Global							= G;
			Global.ProgramStateEvent	    += new ProgramStateEventHandler(ProgramStateChanged);	
            //Global.FRESCO.AfterSetup		+= new FRESCOOL.AfterSetupEventHandler(OnAfterSetup);
			FormClosed						+= new System.Windows.Forms.FormClosedEventHandler(OnFormClosed);
            Shown							+= new System.EventHandler(OnShown);
            Activated                       += new System.EventHandler(OnActivated);
			IsHidden = true;
		}
		public virtual void				Setup()
		{ 
		}
		public virtual void				SetState()
		{ 
		}
		public virtual new void			Refresh()
		{ 
		}
		public virtual void				Clear()
		{
		}
		public override void			SetToolbar()
		{
		}
		protected virtual void			ProgramStateChanged(object sender, ProgramStateEventArgs e)
		{
		}
		protected virtual void			OnAfterSetup()
		{
		}
        public void                     OnShown(object sender, EventArgs e)
        {
            //Occurs after form is loaded and initially displayed.
            Setup();
        }
        public void                     OnActivated(object sender, EventArgs e)
        {
            //Except when passing 0 parameters, DockContent.Show calls Form.Activate instead of Form.Show.
            Setup();
        }
        protected virtual void          OnFormClosed(object sender, FormClosedEventArgs e)
		{
            //Global.FRESCO.AfterSetup -= new FRESCOOL.AfterSetupEventHandler(OnAfterSetup);
		}
	}
}
