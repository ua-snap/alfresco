using System;

namespace FRESCO_Server
{
    public class ProgramStateEventArgs : EventArgs
    {
        public readonly EnumProgramState ProgramState;
        public ProgramStateEventArgs(EnumProgramState ps)
        {
            ProgramState = ps;
        }
    }
    public delegate void ProgramStateEventHandler(object sender, ProgramStateEventArgs e);


    public class SimulationClearedEventArgs : EventArgs
    {
        public SimulationClearedEventArgs() { }
    }
    public delegate void SimulationClearedEventHandler();


    public delegate void SimulationStoppedEventHandler(object sender, EventArgs e);


    public class SimulationCompletedEventArgs : EventArgs
    {
        public SimulationCompletedEventArgs() { }
    }
    public delegate void SimulationCompletedEventHandler(object sender, SimulationCompletedEventArgs e);


    public class SimulationFailedEventArgs : EventArgs
    {
        public Exception Exception;
        public SimulationFailedEventArgs(Exception e) { Exception = e; }
    }
    public delegate void SimulationFailedEventHandler(object sender, SimulationFailedEventArgs e);


    //Used for the messenger to inform the client monitor.
    public class ClientOutputEventArgs : EventArgs
    {
        public string Message;
        public bool IsError;
        public int ClientID;
        public ClientOutputEventArgs(string message, int clientID, bool isError)
        {
            Message = message;
            ClientID = clientID;
            IsError = isError;
        }
    }
    public delegate void ClientOutputEventHandler(object sender, ClientOutputEventArgs e);

    public class ClientLogEntryEventArgs : EventArgs
    {
        public string Message;
        public ClientLogEntryEventArgs(string message) { Message = message; }
    }
    public delegate void ClientLogEntryEventHandler(object sender, ClientLogEntryEventArgs e);
}
