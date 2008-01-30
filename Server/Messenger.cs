using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace FRESCO_Server
{
    #region Event Classes
    public class RPCReceivedEventArgs : EventArgs
    {
        public RemoteProcedureCall RemoteProcedureCall;
        public RPCReceivedEventArgs(RemoteProcedureCall rpc)
        {
            this.RemoteProcedureCall = rpc;
        }
    }
    public delegate void RPCReceivedEventHandler(object sender, RPCReceivedEventArgs e);

    public class NewClientConnectedEventArgs : EventArgs
    {
        public Socket socket;
        public NewClientConnectedEventArgs(Socket client)
        {
            this.socket = client;
        }
    }
    public delegate void NewClientConnectedEventHandler(object sender, NewClientConnectedEventArgs e);
    #endregion

    public class StateObject
    {
        public StateObject(Socket socket)
        {
            this.socket = socket;
        }
        public Socket socket = null;
        public const int BUFFER_SIZE = 4096;
        public byte[] buffer = new byte[BUFFER_SIZE];
    }


    class Messenger
    {
        public event RPCReceivedEventHandler RPCReceivedEvent;
        public event NewClientConnectedEventHandler NewClientConnectedEvent;
        static readonly object  receiveLock = new object();
        private Socket          server;
        private byte[]          incomingData;
        private byte[]          outgoingData;
        private int             bufferSize = 4096;//1024;
        private bool            isReceivingNewConnections;
        private Global          global;

        public                  Messenger()
        {
            global = Global.Instance;
            server = null;
            incomingData = new byte[bufferSize];
            outgoingData = new byte[bufferSize];
            //rpcStream = new RpcStream();
        }
        //Connecting
        public void             StartReceivingClientConnections()
        {
            //Listen for client TCP connections.
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint serverTCPEndPoint = new IPEndPoint(IPAddress.Any, Global.Instance.TcpPort);
            server.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            server.Bind(serverTCPEndPoint);
            server.Listen(10); //TODO: Research this backlog parameter.

            isReceivingNewConnections = true;
            server.BeginAccept(new AsyncCallback(AcceptConnection), server);
        }
        public void             AcceptConnection(IAsyncResult iar)
        {
            if (isReceivingNewConnections)
            {
                server = (Socket)iar.AsyncState;
                Socket client = server.EndAccept(iar);
                server.BeginAccept(new AsyncCallback(AcceptConnection), server); //Continue listening for more clients.

                try
                {
                    //
                    // Shake hands and play nice.
                    //
                    //1. Extend hand to client.
                    byte[] byteData = Encoding.ASCII.GetBytes("FS-" + Global.Instance.Version + "&");  //FS-1.0.1&
                    client.BeginSend(byteData, 0, byteData.Length, SocketFlags.None, new AsyncCallback(SentData), client);
                    //2. Receive client hand.
                    const int size = 20;
                    byte[] buffer = new byte[size];
                    int bookmark = client.ReceiveTimeout;
                    client.ReceiveTimeout = 4000;
                    int bytesReceived = client.Receive(buffer, 0, size, SocketFlags.None);
                    string asciiData = Encoding.ASCII.GetString(buffer, 0, bytesReceived);
                    client.ReceiveTimeout = bookmark;
                    if (asciiData.Contains("FC-" + Global.Instance.Version))
                    {
                        OnNewClientConnected(client);
                    }
                    else client.Close();  //No client hand received, so not worth keeping.
                }
                catch (Exception)
                {
                    client.Close();  //No client hand received, so not worth keeping.
                }
            }
        }
        protected virtual void  OnNewClientConnected(Socket client)
        {
            //Raise event to notify all observers (aka subscribers) to react.
            NewClientConnectedEvent(this, new NewClientConnectedEventArgs(client));
        }
        public void             StopReceivingClientConnections()
        {
            isReceivingNewConnections = false;
            //if (server != null)
            //{
            //    if (server.Connected)
            //        server.
            //        //server.Disconnect(false);
            //}
        }
        //Receiving
        public void             BeginReceiveDataFrom(Socket socket)
        {
            try
            {
                bool isConnected = false;
                try {isConnected = socket.Connected;} catch (Exception e) {
                    //Do nothing.
                }
                if (isConnected) {
                    StateObject so = new StateObject(socket);
                    socket.BeginReceive(so.buffer, 0, StateObject.BUFFER_SIZE, SocketFlags.None, new AsyncCallback(EndReceiveDataFrom), so);
                }
            }
            catch (Exception e)
            {
                string message = "Failed to retreive data from a client.  Check client connections.  ";
                message += e.Message;
                Global.Instance.RaiseSimulationFailed(this, new SimulationFailedEventArgs(new Exception(message, e)));
            }
        }
        public void             EndReceiveDataFrom(IAsyncResult asyncResult)
        {
            lock (receiveLock)
            {
                Socket client = null;
                RemoteProcedureCall[] rpcArray;
                RemoteProcedureCall rpc;
                string asciiData = "";

                //Get data from socket.
                try
                {
                    StateObject so = (StateObject)asyncResult.AsyncState;
                    client = so.socket;
                    if (!client.Connected)
                        return;
                    int bytesReceived = client.EndReceive(asyncResult);
                    asciiData = Encoding.ASCII.GetString(so.buffer, 0, bytesReceived);
                }
                catch (SocketException e)
                {
                    Global.Instance.AddClientLogEntry("Failed to retreive data from socket: " + e.Message + "\n", client);
                }

                //Add data to appropriate client stream.
                int clientID = global.Main.clientMonitor.clientList.IndexOf((IPEndPoint)client.RemoteEndPoint);
                global.Main.clientMonitor.clientList[clientID].Stream.Add(asciiData);
                asciiData = global.Main.clientMonitor.clientList[clientID].Stream.GetCompleteMesseges();

                //Parse all complete RPCs from client stream, leave uncompleted for later.
                try
                {
                    rpcArray = RemoteProcedureCall.ParseMessage(asciiData);
                }
                catch (RemoteProcedureCallParseException e)
                {
                    rpcArray = e.IncompleteRpcArray;  //Retreive any successful 
                    Exception wrapperException = new Exception("Failed to parse data received from " + client.RemoteEndPoint.ToString() + ": \n" + e.CustomMessage + "\n" + e.ToString() + "\n\n");
                    Global.Instance.RaiseSimulationFailed(this, new SimulationFailedEventArgs(wrapperException));
                }

                //Advertise the RPCs parsed from above.
                if (rpcArray != null)
                {
                    for (int i = 0; i < rpcArray.Length; i++)
                    {
                        rpc = rpcArray[i];
                        if (rpc != null)
                        {
                            //Global.Instance.AddDebugToClientLog("Received " + rpc.Command.PadRight(rpc.Command.Length + (24 - rpc.Command.Length), ' ') + "from client " + rpc.ClientID + "\tto   server " + 1 + "\twith parameters=" + rpc.ParameterString + "\n", client);
                            OnRPCReceived(rpc);
                        }
                    }
                }

                //Resume receiving data.
                BeginReceiveDataFrom(client);
            }
        }
        protected virtual void  OnRPCReceived(RemoteProcedureCall rpc)
        {
            //Raise event to notify all observers (aka subscribers) to react.
            RPCReceivedEvent(this, new RPCReceivedEventArgs(rpc));
        }
        //Sending
        public void             SendData(RemoteProcedureCall rpc, Socket client)
        {
            if (client.Connected)
            {
                byte[] byteData = Encoding.ASCII.GetBytes(rpc.GetXml()+"&");
                client.BeginSend(byteData, 0, byteData.Length, SocketFlags.None, new AsyncCallback(SentData), client);

                //Global.Instance.AddDebugToClientLog("Sent     " + rpc.Command.PadRight(rpc.Command.Length + (24 - rpc.Command.Length), ' ') + "to   client " + rpc.ClientID + "\tfrom server " + 1 + "\twith parameters=" + rpc.ParameterString + "\n", client);
            }
        }
        public void             SentData(IAsyncResult asyncResult)
        {
            Socket client = (Socket)asyncResult.AsyncState;
            if (client.Connected)
                client.EndSend(asyncResult);
        }
    }
}