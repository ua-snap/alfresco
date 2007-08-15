using System;
using System.Collections.Generic;
using System.Text;

//TODO: Close 

namespace FRESCO_Server
{
    public class FileHandler
    {
        #region EVENTS
        public delegate void FileEditedEventHandler(object sender, EventArgs e);
        public event FileEditedEventHandler FileEdited;
        public delegate void FilePropertiesChangedEventHandler(FileHandler sender, EventArgs e);
        public event FilePropertiesChangedEventHandler FilePropertiesChanged;
        #endregion

        #region PROPERTIES
        public bool HasUnsavedChanges
        {
            get { return hasUnsavedChanges; }
            set
            {
                hasUnsavedChanges = value;
                UpdateGui();
            }

        }
        public string Name
        { 
            get { return path.Contains("\\") ? path.Substring(path.LastIndexOf('\\') + 1) : path; } 
        }
        public bool IsActive
        { 
            get { return isActive; }
            set 
            { 
                isActive = value;
                UpdateGui();
            }
        }
        public bool IsComplete
        {
            get { return isComplete; }
            set
            {
                isComplete = value;
                UpdateGui();
            }
        }
        public bool Failed
        {
            get { return failed; }
            set 
            { 
                failed = value; 
                UpdateGui();
            }
        }
        public string Path
        { get { return path; } }

        private string key;
        private bool hasUnsavedChanges;
        private bool isActive;
        private bool isComplete;
        private bool failed;
        private string path;
        #endregion

        public FileHandler(string filePath, FilePropertiesChangedEventHandler propertiesChangedHandler)
        {
            FilePropertiesChanged += new FilePropertiesChangedEventHandler(propertiesChangedHandler);
            path = filePath;
            hasUnsavedChanges = !path.Contains("\\"); //new files don't have a full path yet.
            Reset();
        }

        public void Reset()
        {
            isComplete = false;
            isActive = false;
            failed = false;
            UpdateGui();
        }

        public void Close(bool saveFirst)
        {
            if (saveFirst) Save();
        }

        public void Save()
        {
            if (!path.Contains("\\"))
            {
                SaveAs();
                return;
            }

            //TODO: guts of Save()

            hasUnsavedChanges = false;
            UpdateGui();
        }

        public void SaveAs()
        {
            System.Windows.Forms.SaveFileDialog saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            saveFileDialog.FileName = Name;
            if (path.Contains("\\"))
                saveFileDialog.InitialDirectory = path;
            saveFileDialog.Filter = "FIF files (*.fif)|*.fif|All files (*.*)|*.*";
            if (saveFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                path = saveFileDialog.FileName;
                Save();
            }
        }

        private void UpdateGui()
        {
            if (FilePropertiesChanged != null) FilePropertiesChanged(this, new EventArgs());
        }
    }
}
