using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using WeifenLuo.WinFormsUI;

namespace FRESCO_Server
{
    public class Client
    {
        #region Public Data Accessors/Mutators
        public int          ID
        {
            get { return id; }
        }
        /// <summary>
        /// Returns the percent (in non-decimal format, ex: 0-100) 
        /// of workload completed.
        /// </summary>
        public ClientState  State
        {
            get { return state; }
            set 
            {
                state = value;
                if (state == ClientState.SIMULATING) {
                    isCleared = false;
                }
            }
        }
        public string       StateAsString
        {
            get { return State.ToString(); }
        }
        public int          PercentCompleted
        {
            get 
            {
                if (!isCleared  && rep>=0 && year>=0) {
                    float value = 0.0f;
                    int repsTotal = RepStop - RepStart;
                    int repsDone = rep - RepStart;
                    int yearsPerRep = Global.Instance.FIF.LastYear - Global.Instance.FIF.FirstYear + 1;
                    if (repsTotal > 0) {
                        int yearsTotal = yearsPerRep * repsTotal;
                        int yearsDone = year - Global.Instance.FIF.FirstYear + 1 + (yearsPerRep * repsDone);
                        value = (float)yearsDone / yearsTotal * 100;
                    }
                    percentCompleted = (int)value;
                }
                return percentCompleted;
            }
        }
        public int          RepsCompleted
        {
            get { return repsCompleted; }
            set { repsCompleted = value; }
        }
        public string       RepsCompletedAsString
        {
            get
            {
                if (isCleared)
                {
                    return "";
                }
                else
                {
                    int rc = RepsCompleted;
                    return rc.ToString()+" of "+(RepStop-RepStart).ToString();
                }
            }
        }
        public int          Rep
        {
            get { return rep; }
            set { rep = value; }
        }
        public string       RepAsString
        {
            get { return (rep<0 ? "" : rep.ToString()); }
        }
        public int          Year
        {
            get { return year; }
            set { year = value; }
        }
        public string       YearAsString
        {
            get { return (year<0 ? "" : year.ToString()); }
        }
        public string       IPAddress
        {
            get { return endPoint.Address.ToString(); }
        }
        public int          Port
        {
            get { return endPoint.Port; }
        }
        public string       HostName
        {
            get { return hostName;  }
            set { hostName = value; }
        }
        public Socket       Socket
        {
            get { return socket; }
        }
        public EndPoint     EndPoint
        {
            get { return this.endPoint; }
        }
        #endregion
        
        #region Public Data Members
        public RpcStream    Stream;
        public ClientLog    Log;
        public bool         IsParticipating;
        public bool         IsCompleted;
        public int          RepStart;   //Inclusive.
        public int          RepStop;    //Non-inclusive.
        public int          RandomSeed;
        #endregion
         
        #region Private Data Members
        private int         id;
        private ClientState state;
        private bool        isCleared;
        private int         percentCompleted;
        private int         repsCompleted;
        private int         rep;
        private int         year;
        private Socket      socket;
        private IPEndPoint  endPoint;
        private string      hostName;
        private DateTime    tcpStartTime;
        #endregion

        public          Client(int id, Socket clientSocket)
        {
            this.id = id;
            State = ClientState.UNKNOWN;
            isCleared = true;
            percentCompleted = 0;
            RepsCompleted = 0;
            Rep = -1;
            Year = -1;
            socket = clientSocket;
            endPoint = (IPEndPoint)clientSocket.RemoteEndPoint;
            tcpStartTime = DateTime.Now;
            Stream = new RpcStream();

            HostName = "Unknown";
            IsParticipating = false;
            IsCompleted = false;
            RandomSeed = -1;

            Log = new ClientLog(this.id);
        }

        public void     SimulationClear()
        {
            isCleared = true;
            percentCompleted = 0;
            repsCompleted = 0;
            Log.Clear();
        }
    }
}
