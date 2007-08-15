using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace FRESCO_Server
{
    /// <summary>
    /// RemoteProcedureCall - A message container capable of translating to and 
    /// from XML for messengers to send and receive.
    /// </summary>
    public class RemoteProcedureCall
    {
        private int clientID;
        private string command;
        private string[] parameters;

        public int ClientID
        {
            get { return clientID; }
        }
        public string Command
        {
            get { return command; }
        }
        public string[] Parameters
        {
            get { return parameters; }
        }
        public string ParameterString
        {
            get 
            { 
                string s = "";
                for (int i=0; i<parameters.Length; i++)
                    s += parameters[i] +", ";
                return (s=="") ? s : s.Remove(s.LastIndexOf(','));
            }
        }

        public RemoteProcedureCall(int clientID, string command, string[] parameters)
        {
            this.clientID = clientID;
            this.command = command;
            this.parameters = (parameters == null) ? new string[0] : parameters;
        }

        /// <summary>
        /// Returns an XML formatted representation of this RemoteProcedureCall.
        /// </summary>
        /// <returns>Format of returned XML (substitue brackets with angle brackets):
        /// [?xml version="1.0" encoding="ISO-8859-1"?]
        ///      [rpc]
        ///          [clientID]1[/clientID]
        ///          [command]ExampleCommand[/command]
        ///          [parameter]exampleParameter1[/parameter]
        ///          [parameter]exampleParameter2[/parameter]
        ///          [parameter]exampleParameter3[/parameter]
        ///      [/rpc]
        /// </returns>
        public string GetXml()
        {
            StringBuilder sb = new StringBuilder();

            //Construct XML formatted RPC message.
            //sb.Append("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
            sb.Append("<rpc>");
            sb.Append("<clientID>" + clientID + "</clientID>");
            sb.Append("<cmd>" + command + "</cmd>");
            for (int i = 0; i < parameters.Length; i++)
                sb.Append("<parm>" + parameters[i] + "</parm>");
            sb.Append("</rpc>");

            return sb.ToString();
        }

        /// <summary>
        /// Parses RPC messages from XML into an array of RemoteProcedureCall objects.
        /// </summary>
        /// <param name="xmlMessage">String of XML formatted RPC messages.  Typically, 
        /// there is only one RPC message.  But it is possible and likely that some 
        /// TCP packets contains multiple messages.
        /// 
        /// Expected format of xmlMessage (substitue brackets with angle brackets):
        ///      [?xml version="1.0" encoding="ISO-8859-1"?]
        ///      [rpc]
        ///          [clientID]1[/clientID]
        ///          [command]ExampleCommand[/command]
        ///          [parameter]exampleParameter1[/parameter]
        ///          [parameter]exampleParameter2[/parameter]
        ///          [parameter]exampleParameter3[/parameter]
        ///      [/rpc]
        /// </param>
        /// <returns>Array of RemoteProcedureCall objects.</returns>
        public static RemoteProcedureCall[] ParseMessage(string xmlMessage)
        {
            //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
            // Expected format of message (can be more than one of these in xmlMessage): 
            //      <?xml version="1.0" encoding="ISO-8859-1"?>
            //      <rpc>
            //          <clientID>1</clientID>
            //          <cmd>TestCommand</cmd>
            //          <parm>testing1</parm>
            //          <parm>testing2</parm>
            //          <parm>testing3</parm>
            //      </rpc>
            //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
            string clientIDField = "clientID";
            string commandField = "cmd";
            string parameterField = "parm";
            string currentField = "";
            int clientID;
            string command;
            string[] parameters;
            RemoteProcedureCall[] rpcArray = null;
            XmlNodeList nodes;
            XmlDocument xml = new XmlDocument();
            try
            {
                //Seperate root rpc nodes.
                xmlMessage = xmlMessage.Replace("\0","");
                string[] rpcRoots = xmlMessage.Split(new string[] { "</rpc>" }, StringSplitOptions.RemoveEmptyEntries);
                rpcArray = new RemoteProcedureCall[rpcRoots.Length];
                if (rpcRoots.Length > 1)
                    clientID = -1;
                for(int r=0; r<rpcRoots.Length; r++)
                {
                    xml = new XmlDocument();
                    xml.LoadXml(rpcRoots[r]+"</rpc>");

                    //Get client ID.
                    currentField = clientIDField;
                    nodes = xml.GetElementsByTagName(clientIDField);
                    if (nodes.Count != 1)
                        throw new Exception("RemoteProcedureCall.ParseMessage() Failed: One clientID field is allowed.  No more, no less.");
                    clientID = Convert.ToInt32(nodes[0].InnerText);

                    //Get command.
                    currentField = commandField;
                    nodes = xml.GetElementsByTagName(commandField);
                    if (nodes.Count != 1)
                        throw new Exception("RemoteProcedureCall.ParseMessage() Failed: One command field per RemoteProcdureCall.  No more, no less.");
                    command = nodes[0].InnerText;

                    //Get parameters.
                    currentField = parameterField;
                    nodes = xml.GetElementsByTagName(parameterField);
                    parameters = new string[nodes.Count];
                    for (int i = 0; i < nodes.Count; i++)
                        parameters[i] = nodes[i].InnerText;
                    
                    rpcArray[r] = new RemoteProcedureCall(clientID, command, parameters);
                }
            }
            catch (Exception e)
            {
                //Failed while parsing one of the RPC messages, but we don't want to loose the messages that were parsed successfully,
                //so we pass the incomplete array in this exception.
                throw new RemoteProcedureCallParseException(
                                    rpcArray,
                                    (currentField != null) ? "RemoteProcedureCall.ParseMessage() Failed: exception occured while retreiving a " + currentField + " field.  " + e.Message : e.Message);
            }
            return rpcArray;
        }

    }   

    public class RemoteProcedureCallParseException : Exception
    {
        public RemoteProcedureCall[] IncompleteRpcArray;
        public string CustomMessage;
        public RemoteProcedureCallParseException(RemoteProcedureCall[] rpcArray, string message)
        {
            this.IncompleteRpcArray = rpcArray;
            this.CustomMessage = message;
        }
    }
}
