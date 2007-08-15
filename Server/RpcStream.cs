using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FRESCO_Server
{
    public class RpcStream
    {
        static readonly object bufferLock = new object();

        StringBuilder buffer;
        public RpcStream()
        {
            buffer = new StringBuilder();
        }

        public void Add(string asciiText)
        {
            lock (bufferLock)
            {
                buffer.Append(asciiText);
            }
        }

        public string GetCompleteMesseges()
        { 
            string snapShot = "";
            int startIndex = 0;
            int length = 0;
            lock (bufferLock) 
            {
                snapShot = buffer.ToString();
                int endIndex = snapShot.LastIndexOf("</rpc>");
                if (endIndex > -1)
                    length = (endIndex + 5) - startIndex + 1;
                buffer.Remove(startIndex, length);
            }
            return snapShot.Substring(startIndex, length);
        }

        public int bytesAvailable()
        {
            return buffer.Length;
        }
    }
}
