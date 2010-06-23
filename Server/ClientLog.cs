using System;
using System.Collections.Generic;
using System.IO;
using System.Text;


namespace FRESCO_Server
{
    public class ClientLog
    {
        public event ClientOutputEventHandler   LogEntryEvent;

        static readonly object                  fileLock = new object();
        static string                           TEMP_FOLDER_NAME = "Temporary";
        static string                           tempDirectory;
        private int                             clientID;
        private string                          logFilePath;
        private System.IO.StreamWriter          logWriter;


        public                  ClientLog(int clientID)
        {
            this.clientID = clientID;
            TEMP_FOLDER_NAME = "TemporaryForPort" + FRESCO_Server.Properties.Settings.Default.port.ToString();
            try
            {
                //Create temp directory.
                tempDirectory = Path.Combine(Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath), TEMP_FOLDER_NAME);
                if (!Directory.Exists(tempDirectory)) Directory.CreateDirectory(tempDirectory);

                //Create file within temp directory.
                logFilePath = Path.Combine(tempDirectory, clientID + ".txt");
                if (File.Exists(logFilePath)) File.Delete(logFilePath);
                logWriter = File.CreateText(logFilePath);
            }
            catch (Exception e) {
                Global.Instance.RaiseSimulationFailed(this, new SimulationFailedEventArgs(e));
            }
        }

        /// <summary>
        /// Write text to the log.
        /// </summary>
        /// <param name="text"></param>
        /// <param name="isError">If true, text is highlighted.</param>
        public void             Write(ref string text, bool isError)
        {
            text.Replace("\n", "\r\n");
            lock (fileLock) {
                try
                {
                    if (isError)
                    {
                        logWriter.WriteLine("\n============================\n");
                        logWriter.WriteLine("ERROR: " + text + "\n");
                        logWriter.WriteLine("\n============================\n");
                    }
                    else logWriter.WriteLine(text);
                    logWriter.Flush();
                    if (LogEntryEvent != null)  //Notify subscribers (ClientLogViewer).
                        LogEntryEvent(this, new ClientOutputEventArgs(text, clientID, isError));
                }
                catch (Exception e)
                {
                    //exit and continue
                }
            }
        }

        public StringBuilder    GetContents()
        {
            //Stream log file into 'contents'.
            StringBuilder contents = new StringBuilder();
            
            lock (fileLock)
            {
                logWriter.Flush(); logWriter.Close(); logWriter.Dispose();      //Close writer (can only have one stream pointing to the file).
                using (StreamReader logReader = new StreamReader(logFilePath))
                {
                    contents.Append(logReader.ReadToEnd().Replace("\n\r\n", "\n"));
                    logReader.Close();
                }
                logWriter = new StreamWriter(logFilePath, true);                //Reopen writer in append mode.
            }
            return contents;
        }

        public void             ClearSubscriptions()
        {
            LogEntryEvent = null;
        }

        public void             Clear()
        {
            //Start over with a new empty log.
            string logFilePath = Path.Combine(tempDirectory, clientID + ".txt");
            if (Directory.Exists(tempDirectory) && File.Exists(logFilePath))
            {
                Close();
                bool isCompleted = MoveTo(Global.Instance.LogOutputDirectory);
                logWriter = File.CreateText(logFilePath);
            }
        }

        public void             Close()
        {
            lock (fileLock)
            {
                logWriter.Flush();
                logWriter.Close();
            }
        }

        public bool             MoveTo(string destinationFolder)
        {
            if (!File.Exists(logFilePath)) 
                return false;
            if (String.Empty == destinationFolder)
                return false;
            if (!Directory.Exists(destinationFolder)) 
                Directory.CreateDirectory(destinationFolder);
            //Move.
            string destinationPath = Path.Combine(destinationFolder, "clientLog" + Path.GetFileName(logFilePath));
            if (!File.Exists(destinationPath)) 
                File.Move(logFilePath, destinationPath);
            //Validate.
            bool isSuccess = false;
            if (File.Exists(destinationPath) && !File.Exists(logFilePath))
                isSuccess = true;
            return isSuccess;
        }

        public static void      DeleteTempDirectory()
        {
            string dir = Path.Combine(System.Environment.CurrentDirectory, TEMP_FOLDER_NAME);
            if (Directory.Exists(dir)) {
                //Delete all files within.
                string[] files = Directory.GetFiles(dir);
                foreach (string file in files) {
                    File.Delete(file);
                }
                Directory.Delete(dir, true);
            }
        }
    }
}
