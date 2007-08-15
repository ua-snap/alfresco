using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DevExpress.XtraEditors.Controls;
using Uaf.Controls;
using WeifenLuo.WinFormsUI;

//TODO: UpdateMenu()
//TODO: Open editor with double click, enter or button.
//TODO: Avoid text overlap on top of icon.
//TODO: Update main menu.
namespace FRESCO_Server
{
    public partial class FileListView : DockContent
    {
        #region PROPERTIES
        List<FileHandler> filesList = new List<FileHandler>();
        public int Count
        {
            get { return filesList.Count; }
        }
        #endregion

        public FileListView()
        {
            InitializeComponent();
            boundListView.DataSource = filesList;
            boundListView.DisplayMember = "Name";
            boundListView.DrawItem += new DevExpress.XtraEditors.ListBoxDrawItemEventHandler(boundListView_DrawItem);

            UpdateMenu(false);
            Global.Instance.ProgramStateEvent += new ProgramStateEventHandler(ProgramStateChanged);
        }

        public void ProgramStateChanged(Object sender, ProgramStateEventArgs e)
        {
            SafeInvokeHelper.Invoke(this, "UpdateMenu",false);
        }
        
        public void UpdateMenu(bool updateParentWindow)
        {
            boundListView.Refresh();
         
            EnumProgramState ps = Global.Instance.ProgramState;
            bool isSimulationBusy = ps != EnumProgramState.NOT_LOADED;
            bool hasItems = (filesList.Count > 0);
            bool hasSelectedItems = (boundListView.SelectedIndices.Count > 0);
            bool isTopItemSelected = boundListView.SelectedIndices.Contains(0);
            bool isBottomItemSelected = boundListView.SelectedIndices.Contains(filesList.Count-1);

            btnNew.Enabled = !isSimulationBusy;
            btnOpen.Enabled = !isSimulationBusy;
            btnSave.Enabled = !isSimulationBusy && hasItems && hasSelectedItems;
            btnSaveAll.Enabled = !isSimulationBusy && hasItems;
            btnRemove.Enabled = !isSimulationBusy && hasItems && hasSelectedItems;
            btnMoveDown.Enabled = !isSimulationBusy && hasItems && hasSelectedItems && !isBottomItemSelected;
            btnMoveUp.Enabled = !isSimulationBusy && hasItems && hasSelectedItems && !isTopItemSelected;
            
            if (updateParentWindow) Global.Instance.Main.SetMenu();
        }

        public List<FileHandler>.Enumerator GetEnumerator()
        {
            return filesList.GetEnumerator();
        }

        public void addNewFile()
        {
            //Choose available name.
            int newCount = 0;
            string name = "New.fif";
            while (nameIsDuplicate(name))
                name = "New" + ++newCount + ".fif";
            //Create and add new file handler.
            FileHandler newFile = new FileHandler(name, filePropertiesChanged);
            filesList.Add(newFile);
            //Synchronize.
            UpdateMenu(true);
        }

        public void addExistingFile()
        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                //First sort the selected files.
                System.Collections.Generic.SortedList<string, string> sortedFileNames = new SortedList<string, string>();
                foreach (string f in openFileDialog.FileNames)
                {
                    sortedFileNames.Add(f, f);
                }
                //Add them to the list.
                foreach (string filename in sortedFileNames.Values)
                {
                    string path = filename;
                    string name = path;
                    if (path.Contains("\\")) name = path.Substring(path.LastIndexOf('\\') + 1);
                    FileHandler newFile = new FileHandler(path, filePropertiesChanged);
                    filesList.Add(newFile);
                }
            }
            UpdateMenu(true);
        }

        public void removeSelected()
        {
                DevExpress.XtraEditors.BaseImageListBoxControl.SelectedIndexCollection selectedIndices;
                selectedIndices = boundListView.SelectedIndices;

                if (selectedIndices.Count > 0)
                {
                    for (int i = selectedIndices.Count - 1; i >= 0; i--)
                    {
                        bool saveBeforeRemove = false;
                        int index = selectedIndices[i];
                        FileHandler file = filesList[index];
                        if (file.HasUnsavedChanges)
                        {
                            DialogResult result = MessageBox.Show("Save changes before removing " + file.Name + "?", "Save?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                            switch (result)
                            {
                                case DialogResult.Yes: saveBeforeRemove = true; break;
                                case DialogResult.No: saveBeforeRemove = false; break;
                                case DialogResult.Cancel: return;
                            }
                        }
                        file.Close(saveBeforeRemove);
                        boundListView.SetSelected(index, false);
                        filesList.RemoveAt(index);
                    }
                }
                UpdateMenu(true);
        }

        public void reset()
        {
            foreach (FileHandler file in filesList)
            {
                file.Reset();
            }
        }

        private bool nameIsDuplicate(string name)
        {
            foreach (FileHandler file in filesList)
            {
                string n = file.Name;
                if (n[n.Length - 1] == '*') n = n.Remove(n.Length - 1);
                n = n.Trim();
                if (n.Equals(name)) return true;
            }
            return false;
        }

        public void filePropertiesChanged(FileHandler sender, EventArgs e)
        {
             boundListView.Refresh();
        }

        #region GUI EVENTS
        private void addNewScenario_Click(object sender, EventArgs e)
        {
            addNewFile();
        }

        private void addExistingScenario_Click(object sender, EventArgs e)
        {
            addExistingFile();
        }
        
        private void saveScenario_Click(object sender, EventArgs e)
        {
            foreach (int fileIndex in boundListView.SelectedIndices)
                filesList[fileIndex].Save();
        }

        private void saveAllScenarios(object sender, EventArgs e)
        {
            foreach (FileHandler file in filesList)
                file.Save();
        }

        private void remove_Click(object sender, EventArgs e)
        {
            removeSelected();
        }

        //private void resize(object sender, EventArgs e)
        //{
        //    list.Columns[0].Width = list.Width;
        //}

        private void list_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateMenu(true);
        }

        private void list_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.ToString() == "\b" || e.KeyChar.ToString() == Keys.Delete.ToString())
            {
                if (btnRemove.Enabled)
                {
                    removeSelected();
                    e.Handled = true;
                }
            }
        }

        void boundListView_DrawItem(object sender, DevExpress.XtraEditors.ListBoxDrawItemEventArgs e)
        {
            FileHandler file = filesList[e.Index];
            //Is this item selected?
            bool isSelected = false;
            foreach (int index in boundListView.SelectedIndices) {
                if (index == e.Index) {
                    isSelected = true;
                    break;
                }
            }
            //Set to default appearance.
            e.Appearance.ForeColor = Color.Black;
            e.Appearance.BackColor = Color.White;
            e.Appearance.BackColor2 = Color.White;
            e.Appearance.Font = new Font(e.Appearance.Font, FontStyle.Regular);

            //Set selection appearance.
            if (isSelected) {
                e.Appearance.BackColor = Color.LightBlue;
                e.Appearance.BackColor2 = Color.LightBlue;
            }

            //Set file status indications.
            //.Value = sender.Name + (file.HasUnsavedChanges ? " *" : "");
            if (file.IsComplete) { e.Appearance.BackColor = Color.LawnGreen; e.Appearance.BackColor2 = Color.LawnGreen; }
            else if (file.Failed) { e.Appearance.BackColor = Color.Red; e.Appearance.BackColor2 = Color.Tomato; }
            if (file.IsActive) e.Appearance.Font = new Font(e.Appearance.Font, FontStyle.Bold);

            //Set selection appearance.
            if (isSelected) {
                e.Appearance.BackColor2 = Color.LightBlue;
            }

        }

        private void moveDown_Click(object sender, EventArgs e)
        {
            //Copy and sort selected indices.
            List<int> selectedIndices = new List<int>();
            foreach (int i in boundListView.SelectedIndices)
                selectedIndices.Add(i);
            selectedIndices.Sort();

            for (int i = selectedIndices.Count - 1; i >= 0; i--)
            {
                int oldIndex = selectedIndices[i];
                int newIndex = oldIndex + 1;
                if (newIndex >= filesList.Count) return;

                //Save, remove, insert.
                FileHandler swapFile = filesList[oldIndex];
                filesList.RemoveAt(oldIndex);
                filesList.Insert(newIndex, swapFile);
                boundListView.SetSelected(oldIndex, false);
                boundListView.SetSelected(newIndex, true);
            }

            UpdateMenu(false);
        }

        private void moveUp_Click(object sender, EventArgs e)
        {
            //Copy and sort selected indices.
            List<int> selectedIndices = new List<int>();
            foreach (int i in boundListView.SelectedIndices)
                selectedIndices.Add(i);
            selectedIndices.Sort();

            for (int i = 0; i < selectedIndices.Count; i++)
            {
                int oldIndex = selectedIndices[i];
                int newIndex = oldIndex - 1;
                if (newIndex < 0) return;

                //Save, remove, insert.
                FileHandler swapFile = filesList[oldIndex];
                filesList.RemoveAt(oldIndex);
                filesList.Insert(newIndex, swapFile);
                boundListView.SetSelected(oldIndex, false);
                boundListView.SetSelected(newIndex, true);
            }

            UpdateMenu(false);
        }

        #endregion

    }
}
