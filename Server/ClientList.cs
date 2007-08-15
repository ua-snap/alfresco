using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace FRESCO_Server
{
    /// <summary>
    /// A collection of clients.
    /// </summary>
    public class ClientList : System.Collections.CollectionBase
    {
        public Client this[int index]
        {
            //List is zero-based, but Client IDs are one-based.
            get { return (Client)(List[index]); }
            set { List[index] = value; }
        }
        public Client ByID(int clientID)
        { 
            int index = clientID-1;
            if (index >= 0 && index < List.Count)
                return this[index];
            else
                return null;

        }

        public int Add(Client value)
        {
            return List.Add(value);
        }
        /// <summary>
        /// Add a new Client to the list.
        /// </summary>
        /// <param name="clientSocket">The client's connected TCP socket.</param>
        /// <returns>ClientID of new Client.</returns>
        public int Add(Socket clientSocket)
        {
            int newID = List.Count + 1;
            this.Add(new Client(newID, clientSocket));
            return newID;
        }

        public void Insert(int index, Client value)
        {
            List.Insert(index, value);
        }

        public int IndexOf(Client value)
        {
            return List.IndexOf(value);
        }
        public int IndexOf(IPEndPoint ipEndPoint)
        {
            string ipAddress = ipEndPoint.Address.ToString();
            for (int i=0; i<this.Count; i++)
                if (this[i].IPAddress.Equals(ipAddress))
                    if (this[i].Port == ipEndPoint.Port)
                        return i;
            return -1;
        }

        public bool Contains(Client value)
        {
            return List.Contains(value);
        }
        public bool Contains(string ipAddress)
        {
            for (int i=0; i<List.Count; i++)
            {
                if (this[i].IPAddress.Equals(ipAddress))
                    return true;
            }
            return false;
        }
        public bool Contains(IPEndPoint ipEndPoint, ref int clientID)
        {
            string ipAddress = ipEndPoint.Address.ToString();
            for (int i=0; i<List.Count; i++)
            {
                if (this[i].IPAddress.Equals(ipAddress))
                    if (this[i].Port == ipEndPoint.Port)
                    {
                        clientID = i+1; //Adjust for 1-based index.
                        return true;
                    }
            }
            return false;
        }

        public void Remove(Client value)
        {
            List.Remove(value);
        }

        public void CopyTo(Client[] array, int index)
        {
            List.CopyTo(array, index);
        }









    }
}
