using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace FRESCO_Server
{
    class ClientListViewItem : System.Windows.Forms.ListViewItem
    {
        public ClientListViewItem(ClientState state, int repsCompleted, int rep, int year, string ipAddress, int port, string computerName)
        {
            //State.
            ListViewSubItem stateItem = new ListViewSubItem(this, state.ToString());
            stateItem.Name = "state";
            this.SubItems.Add(stateItem);
            //Reps Completed.
            ListViewSubItem repsCompletedItem = new ListViewSubItem(this, " "/*repsCompleted.ToString()*/);
            repsCompletedItem.Name = "repsCompleted";
            this.SubItems.Add(repsCompletedItem);
            //Rep.
            ListViewSubItem repItem = new ListViewSubItem(this, (rep == -1) ? "" : rep.ToString());
            repItem.Name = "rep";
            this.SubItems.Add(repItem);
            //Year.
            ListViewSubItem yearItem = new ListViewSubItem(this, (year == -1) ? "" : year.ToString());
            yearItem.Name = "year";
            this.SubItems.Add(yearItem);

            //IP Address.
            ListViewSubItem ipAddressItem = new ListViewSubItem(this, ipAddress);
            ipAddressItem.Name = "ipAddress";
            this.SubItems.Add(ipAddressItem);
            //Port.
            ListViewSubItem portItem = new ListViewSubItem(this, port.ToString());
            portItem.Name = "port";
            this.SubItems.Add(portItem);
            //Computer Name.
            ListViewSubItem computerNameItem = new ListViewSubItem(this, computerName);
            computerNameItem.Name = "computerName";
            this.SubItems.Add(computerNameItem);
            //View Log Button.
            //ListViewSubItem computerNameItem = new ListViewSubItem(this, computerName);
            
            //computerNameItem.Name = "computerName";
            //this.SubItems.Add(computerNameItem);
        }
    }
}
