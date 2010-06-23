using System;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;


namespace FRESCO_Server
{
	public partial class ViewerProgress : Viewer
	{
		public						ViewerProgress()
		{
			InitializeComponent();		
		}
		public						ViewerProgress(Global G) : base(G)
		{
			InitializeComponent();
		}
        public override void        Setup()
        {
            List<int> Tabs = new List<int>(new int[] { 25 });
            txbOutput.SetDefaults("Arial", 9, false, false, Color.Black, Color.White, HorizontalAlignment.Left, false, false, Tabs);
            txbOutput.RightMargin = 3000;
            Clear();
            base.Setup();
        }
		public override void		MenuSelection(String Selection)
		{
			switch (Selection)
			{
			case "Clear" :
				Clear();
				break;
			}
		}
		public override void		Clear()
		{   
			txbOutput.Clear();
            AddHeading1(Global.Instance.Title + " " + Global.Instance.Version + "\n");
            string msg = "\nUsing port " + Global.Instance.TcpPort.ToString() + "\n";
            AddText(msg, "Arial", 9, true, false, Color.FromArgb(0, 0, 0));
        }
        public void                 Save(string filePath)
        {
            AddText("\tSaving  " + filePath + "... ");
            try { 
                txbOutput.SaveFile(filePath);
                AddText("done\n");
            }
            catch (Exception) {
                AddError("failed\n");                
            }
        }
		public void					AddHeading(string Heading)
		{
			AddText("\n" + Heading, "Arial",16,true,true,Color.Black);
		}
		public void					Add(string Heading)
		{
			AddText(Heading);
		}
		public void					AddError(string Error)
		{
			txbOutput.AddError(Error);
		}
		public void					AddWarning(string Warning)
		{
			txbOutput.AddWarning(Warning);
		}

        //SafeAddWarning(string Warning)
        //{
        //    SafeInvokeHelper.Invoke(this,
        //}

		public void					AddSystemMessage(string Message)
		{
			txbOutput.AddSystemMessage(Message);
		}
		public void					AddSystemError(string Message, string SystemMessage)
		{
			txbOutput.AddSystemError(Message,SystemMessage);
		}
		
		#region AddText() Functions
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold, bool IsUnderline, Color Color, Color BackColor, HorizontalAlignment Alignment, bool IsBullet, bool IsItalic, List<int> Tabs)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold,IsUnderline,Color,BackColor,Alignment,IsBullet,IsItalic, Tabs);
		}
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold, bool IsUnderline, Color Color, Color BackColor, HorizontalAlignment Alignment, bool IsBullet, bool IsItalic)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold,IsUnderline,Color,BackColor,Alignment,IsBullet,IsItalic);
		}
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold, bool IsUnderline, Color Color, Color BackColor, HorizontalAlignment Alignment, bool IsBullet)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold,IsUnderline,Color,BackColor,Alignment,IsBullet);
		}
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold, bool IsUnderline, Color Color, Color BackColor, HorizontalAlignment Alignment)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold,IsUnderline,Color,BackColor,Alignment);
		}
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold, bool IsUnderline, Color Color, Color BackColor)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold,IsUnderline,Color,BackColor);
		}
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold, bool IsUnderline, Color Color)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold,IsUnderline,Color);
		}
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold, bool IsUnderline)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold,IsUnderline);
		}
		public void					AddText(String Text, String FontName, int FontSize, bool IsBold)
		{
			txbOutput.AddText(Text,FontName,FontSize,IsBold);
		}
		public void					AddText(String Text, String FontName, int FontSize)
		{
			txbOutput.AddText(Text,FontName,FontSize);
		}
		public void					AddText(String Text, String FontName)
		{
			txbOutput.AddText(Text,FontName);
		}
		public void					AddText(String Text)
		{
			txbOutput.AddText(Text);
		}
		public void					AddText(String NewText, bool IsBold, bool IsUnderline)
		{
			txbOutput.AddText(NewText,IsBold,IsUnderline);
		}
		public void					AddText(String NewText, int FontSize, bool IsBold)
		{
			txbOutput.AddText(NewText,FontSize,IsBold);
		}
		public void					AddText(String NewText, bool IsBold, Color Color)
		{
			txbOutput.AddText(NewText,IsBold,Color);
		}
		public void					AddText(String NewText, bool IsBold)
		{
			txbOutput.AddText(NewText,IsBold);
		}


        public void                 AddHeading1(String Text)
        {
            txbOutput.AddText(Text, "Arial", 12, true, true);
        }
        public void                 AddHeading2(String Text)
        {
            txbOutput.AddText(Text, "Arial", 10, true);
        }
		#endregion


        private void txbOutput_LinkClicked(object sender, LinkClickedEventArgs e)
        {
            try {
                System.Diagnostics.Process.Start(e.LinkText);
            } catch (Exception error) {
                System.Windows.Forms.MessageBox.Show(error.Message, "Error");
            }
        }
	}
}
