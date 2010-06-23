using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI;

namespace FRESCO_Server
{
    public partial class ClientLogViewer : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        public ClientLogViewer(ClientLog log)
        {
            InitializeComponent();
            this.Hide();
            log.LogEntryEvent += new ClientOutputEventHandler(OnLogEntry);

            textBox.AppendText(log.GetContents().ToString());
        }


        void OnLogEntry(object sender, ClientOutputEventArgs e)
        {
            if (e.IsError)
            {
                textBox.AppendText("\n============================\n");
                textBox.AppendText("ERROR: " + e.Message + "\n");
                textBox.AppendText("\n============================\n");
            }
            else
                textBox.AppendText(e.Message);
        }
    }
}