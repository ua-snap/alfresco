using System;
using System.IO;
using System.Text;
using System.Globalization;
using System.Collections;
using System.Collections.Generic;

namespace FRESCO_Server
{
    public class FIF
	{

		#region FIF Variables
		//Output Directory
		public string		FileName				= "";
        public string       ClientFifPath           = "";
        public string       ClientOutputBasePath    = "";
        public string       ServerOutputBasePath    = "";
        public string		RandSeed 				= "";
		public int			MaxReps 				= -1;
		public int			MaxYears 				= -1;
		#endregion

		public enum EnumFIFSettingType
		{
			BOOL,
			ENUM,
			INT,
			FLOAT,
			STRING
		};
		private string[]		words;
		private Global			Global;
		public					FIF(Global G)
		{
			Global = G;
		}

        public void             LoadLite(string filename)
        {	
			if (!File.Exists(filename))	
                throw new Exception("File " + filename + " does not exist.\n");
            this.FileName = filename;

            //Parse only values needed for server.
            string temp;
            ClientFifPath           = GetValLite("ClientFifPath");
            ClientOutputBasePath    = GetValLite("ClientOutputBasePath");
            ServerOutputBasePath    = GetValLite("ServerOutputBasePath");
            MaxYears                = Convert.ToInt32(GetValLite("MaxYears"));
            MaxReps                 = Convert.ToInt32(GetValLite("MaxReps"));
            RandSeed                = GetValLite("RandSeed");
        }

        private string GetValLite(string key)
        {
            string line = "";
            //1. Find line.
            using (StreamReader sr = new StreamReader(FileName))
            {
                bool found = false;
                while (!sr.EndOfStream) 
                {
                    line = sr.ReadLine().Trim();
                    if (line.Length >= key.Length) 
                    {
                        if (line.Substring(0, key.Length) == key) 
                        {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found)
                    throw new Exception("Field not found: " + key);
            }

            //2. Extract value(s) from line.
            if (!line.Contains(";")) throw new Exception("Invalid format.  Missing ';' in the following line: " + line);
            if (!line.Contains("=")) throw new Exception("Invalid format.  Missing '=' in the following line: " + line);
            int begin = line.IndexOf('=') + 1;
            int end = line.IndexOf(';');
            line = line.Substring(begin, end - begin).Trim();
            line = line.Replace("\"","");
            line = line.Replace("{", "");
            line = line.Replace("}", "");
            return line;
        }
		
        private string			GetVal(string Line) 
		{
			string	Setting		= "";
			string	Value		= "";
			int		Begin		= 0;
			int		End			= 0;
			Setting	= Line.Substring(0,Line.IndexOf(' ')).Trim();
			Begin	= Line.IndexOf('=')+1;
            End     = Line.IndexOfAny(new char[] {';','\t'});
			Value	= Line.Substring(Begin,End-Begin).Trim();
            if (Value.Length > 0)
			    if (Value[0]=='\"') Value = Value.Substring(1,Value.Length-2);
			return Value;
		}
		private List<string>	GetStringList(string line) 
		{
			char[]			delims	= {' ','=','{','}',',','\t'};
			List<string>	vals	= new List<string>();
			int				count	= 1;
			words = line.Split(delims);
			while(count<words.Length) {
				while (words.GetValue(count).ToString()=="") 
				{	
					count++;
					if (count>=words.Length) return vals;
				}
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(words.GetValue(count++).ToString().Replace("\"",""));
			}
			return vals;
		}
		private List<int>		GetIntList(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};
			List<int>	vals	= new List<int>();
			int			count	= 1;
			words = line.Split(delims);
			while(count<words.Length) 
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(Convert.ToInt32(words.GetValue(count++).ToString()));
			}
			return vals;
		}
		private List<int>		GetHexList(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};
			List<int>	vals	= new List<int>();
			int			count	= 1;
			words = line.Split(delims);
			while(count<words.Length) 
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(System.Int32.Parse(words[count].Substring(2),NumberStyles.HexNumber));
				count++;
				//vals.Add(Convert.ToInt32(words.GetValue(count++).ToString()));
			}
			return vals;
		}
		private List<float>		GetFloatList(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};
			List<float>	vals	= new List<float>();
			int			count	= 1;
			words = line.Split(delims);
			while(count<words.Length)
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(Convert.ToSingle(words.GetValue(count++).ToString()));
			}
			return vals;
		}
		private List<double>	GetDoubleList(string line) 
		{
			char[]			delims	= {' ','=','{','}',',','\"','\t'};
			List<double>	vals	= new List<double>();
			int				count	= 1;
			words = line.Split(delims);
			while(count<words.Length)
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(Convert.ToDouble(words.GetValue(count++).ToString()));
			}
			return vals;
		}
        private List<bool>      GetBoolList(string line)
		{
			char[]			delims	= {' ','=','{','}',',','\"','\t'};
			List<bool>	vals	= new List<bool>();
			int				count	= 1;
			words = line.Split(delims);
			while(count<words.Length)
			{
				while (words.GetValue(count).ToString()=="")	count++;
				if (String.Compare(words.GetValue(count).ToString(), 0, ";", 0, 1)==0) return vals;  //Comment has been reached and no more words should be added to return value.
				vals.Add(words.GetValue(count++).ToString() == "TRUE" ? true : false);
			}
			return vals;
        }
		private string			GetTip(string line) 
		{
			char[]		delims	= {' ','=','{','}',',','\"','\t'};		//Semicolon removed from list.
			string		tip		= "";
			int			i		= 0;
			words = line.Split(delims);									
			//Find begining of the tooltip.
			while (words.GetValue(i).ToString() != ";")			i++;			
			//Concat remaining words into the tooltip.
			for (++i; i < words.Length; i++)					tip += words.GetValue(i).ToString() + " "; 
			return tip;
		}
		public string			Format(bool Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			Return.Append(Setting?"TRUE":"FALSE");
			Return.Append(' ' ,(Setting?46:45));
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(int Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			String Value = Setting.ToString("0");
			Return.Append(Value);
			Return.Append(' ' ,50-Value.Length);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(float Setting)
		{
			return Format((double)Setting);
		}
		public string			Format(double Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			String Value = Setting.ToString();
			Return.Append(Value);
			Return.Append(' ' ,50-Value.Length);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(string Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			int				PadLength	= 0;
			//Create pad string.
			PadLength = (Setting.Length<48 ? 48 : 78) - Setting.Length;
			//Create formatted setting.
			Return.Append("\"");
			Return.Append(Setting);
			Return.Append("\"");
			Return.Append(' ',PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
        public string           Format(List<bool> Setting)
        {
            StringBuilder Return = new StringBuilder("", 255);
            StringBuilder Value = new StringBuilder("", 255);
            int PadLength = 0;
            if (Setting.Count > 0)
            {
                for (int i = 0; i < Setting.Count; i++)
                {
                    Value.Append("");
                    Value.Append(Setting[i].ToString() == bool.TrueString ? "TRUE" : "FALSE");
                    Value.Append(", ");
                }
                Value.Remove(Value.Length - 2, 2);
            }
            //Create pad string.
            PadLength = (Value.Length < 48 ? 48 - Value.Length : (Value.Length < 78 ? 78 - Value.Length : 4));
            Return.Append("{");
            Return.Append(Value);
            Return.Append("}");
            Return.Append(' ', PadLength);
            Return.Append("; ");
            return Return.ToString();
        }
        public string			Format(List<int> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("");
					Value.Append(Setting[i].ToString("0"));
					Value.Append(", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
        public string           Format(List<float> Setting)
        {
            StringBuilder Return = new StringBuilder("", 255);
            StringBuilder Value = new StringBuilder("", 255);
            int PadLength = 0;
            if (Setting.Count > 0) {
                for (int i = 0; i < Setting.Count; i++) {
                    Value.Append("");
                    Value.Append(Setting[i].ToString());
                    Value.Append(", ");
                }
                Value.Remove(Value.Length - 2, 2);
            }
            //Create pad string.
            PadLength = (Value.Length < 48 ? 48 - Value.Length : (Value.Length < 78 ? 78 - Value.Length : 4));
            Return.Append("{");
            Return.Append(Value);
            Return.Append("}");
            Return.Append(' ', PadLength);
            Return.Append("; ");
            return Return.ToString();
        }
        public string           Format(List<double> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("");
					Value.Append(Setting[i].ToString());
					Value.Append(", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(List<string> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("\"");
					Value.Append(Setting[i]);
					Value.Append("\", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			Format(string Setting,FIF.EnumFIFSettingType Type)
		{
			string	Return		= "";
			string	Spaces		= "";
			int		PadLength	= 0;
			//Create pad string.
			PadLength	= (Setting.Length<50 ? 50-Setting.Length : (Setting.Length<80 ? 80-Setting.Length : 4));
			PadLength  -= (Type==FIF.EnumFIFSettingType.STRING ? 2 : 0);
			for (int i=0;i<PadLength;i++) Spaces += " ";
			//Create formatted setting.
			switch (Type) {
			case FIF.EnumFIFSettingType.BOOL :
				goto case FIF.EnumFIFSettingType.FLOAT;
			case FIF.EnumFIFSettingType.ENUM :
				goto case FIF.EnumFIFSettingType.FLOAT;
			case FIF.EnumFIFSettingType.INT :
				goto case FIF.EnumFIFSettingType.FLOAT;
			case FIF.EnumFIFSettingType.FLOAT :
				Return = Setting + Spaces;
				break;
			case FIF.EnumFIFSettingType.STRING : 			
				Return = "\"" + Setting + "\"" + Spaces;
				break;
			}
			Return += "; ";
			return Return;
		}
		public string			FormatHex(int Setting)
		{
			StringBuilder Return = new StringBuilder("", 255);
			String Value = Setting.ToString("x");
			Return.Append("0x");
			Return.Append(Value);
			Return.Append(' ' ,48-Value.Length);
			Return.Append("; ");
			return Return.ToString();
		}
		public string			FormatHex(List<int> Setting)
		{
			StringBuilder	Return		= new StringBuilder("", 255);
			StringBuilder	Value		= new StringBuilder("", 255);
			int				PadLength	= 0;
			if (Setting.Count>0) {
				for(int i=0; i<Setting.Count; i++) {
					Value.Append("0x");
					Value.Append(Setting[i].ToString("x"));
					Value.Append(", ");
				} 
				Value.Remove(Value.Length-2,2);
			}
			//Create pad string.
			PadLength	= (Value.Length<48 ? 48-Value.Length : (Value.Length<78 ? 78-Value.Length : 4));
			Return.Append("{");
			Return.Append(Value);
			Return.Append("}");
			Return.Append(' ' ,PadLength);
			Return.Append("; ");
			return Return.ToString();
		}
	}
}
