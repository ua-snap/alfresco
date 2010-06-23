using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FRESCO_Server
{
    struct FireSizeEvent
    {
        public double Value;
        public int Cause;
        public int LowCount;
        public int ModCount;
        public int HighLssCount;
        public int HighHssCount;
    };

    class FireSizeStatistic
    {
        //Variables
        public long tally;                                  //For totaling incremental types.
        private string title;                               //Title of the stat contents.
        private int maxYears;								//Maintain a local copy for optimization.
        private int maxReps;								//Maintain a local copy for optimization.
        private int timeStep;								//Maintain a local copy for optimization
        private int outFlags;								//Indicates which data to output when the object is written.
        private List<List<double>> data;
        private List<BasicStatistic> basicStatYear;	        //Maintains the statistics across replicates
        private List<BasicStatistic> basicStatRep;		    //Maintains the statistics within a replicate
        private LinkedList<List<FireSizeEvent>>[,] eventLists;  //A bucket system is used to avoid finding contiguous memory for all events.  Instead a list of buckets containing FireSizeEvents is maintained.
        private const int BUCKET_CAPACITY = 10000;


        /// <summary>
        /// Constructor.  Initializes members and allocates memory.
        /// </summary>
        /// <param name="title"></param>
        /// <param name="maxYears"></param>
        /// <param name="maxReps"></param>
        /// <param name="timeStep"></param>
        /// <param name="outflags"></param>
        public FireSizeStatistic(string title, int maxYears, int maxReps, int timeStep, int outflags)
        {
            //Setup the default values.
            this.tally = 0;
            this.title = title;
            this.maxYears = maxYears;
            this.maxReps = maxReps;
            this.timeStep = timeStep;
            this.outFlags = outflags;

            int numYears = maxYears / timeStep + 1;

            //Allocate memory.
            basicStatYear = new List<BasicStatistic>(numYears);  //Init capacity to include the last year in the sequence
            for (int i = 0; i < basicStatYear.Capacity; i++)
                basicStatYear.Add(new BasicStatistic());

            basicStatRep = new List<BasicStatistic>(maxReps);
            for (int i = 0; i < basicStatRep.Capacity; i++)
                basicStatRep.Add(new BasicStatistic());

            data = new List<List<double>>(numYears);
            for (int y = 0; y < data.Capacity; y++)
            {
                data.Add(new List<double>(maxReps));
                for (int r = 0; r < data[y].Capacity; r++)
                    data[y].Add(0.0);
            }

            eventLists = new LinkedList<List<FireSizeEvent>>[numYears, maxReps];
            for (int y = 0; y < numYears; y++)
            {
                for (int r = 0; r < maxReps; r++)
                {
                    eventLists[y, r] = new LinkedList<List<FireSizeEvent>>();
                    eventLists[y, r].AddLast(new List<FireSizeEvent>());
                }
            }
        }

        //Functions
        /// <summary>
        /// Resets the stat to values at creation.
        /// </summary>
        public void Clear()
        {
            tally = 0;
            //Reset year and rep stats.
            basicStatYear.Clear();
            basicStatRep.Clear();
            //Reset data array.
            for (int i = 0; i < data.Count; i++)
                data[i].Clear();
            data.Clear();
            ClearEvents();
        }

        /// <summary>
        /// Adds a given statistic to the year and replicate.  If an invalid Year/Rep is specified, an error is thrown.
        /// </summary>
        /// <param name="year"></param>
        /// <param name="rep"></param>
        /// <param name="data"></param>
        /// <param name="cause"></param>
        public void Add(int year, int rep, double value, int cause, int low, int mod, int hiLSS, int hiHSS)
        {
            //Error if year or rep is out of bounds.
            if (year > maxYears || year < 0) throw new Exception("Invalid year specified when adding a statisitc.\n");
            if (rep >= maxReps || rep < 0) throw new Exception("Invalid replicate specified when adding a statisitc.\n");
            //Add data to the by year statistic.
            basicStatYear[year / timeStep].Add(value);
            //Add data to the by rep statistic.
            basicStatRep[rep].Add(value);
            //Add data to the summation for year and rep.
            data[year / timeStep][rep] += value;
            //Add data to the event list. 
            if ((outFlags & (int)OutFlags.outEvents) > 0)
                AddEvent(year, rep, value, cause, low, mod, hiLSS, hiHSS);
        }

        /// <summary>
        /// Increment the tally by one.
        /// </summary>
        /// <param name="a"></param>
        /// <returns></returns>
        public static FireSizeStatistic operator ++(FireSizeStatistic a)
        {
            a.tally++;
            return a;
        }

        /// <summary>
        /// Decrement the tally by one.
        /// </summary>
        /// <param name="a"></param>
        /// <returns></returns>
        public static FireSizeStatistic operator --(FireSizeStatistic a)
        {
            a.tally--;
            return a;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="year"></param>
        /// <param name="rep"></param>
        /// <returns></returns>
        public double SumOfYearRep(int year, int rep)
        {
            return data[year / timeStep][rep];
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public double SumAcrossYears()
        {
            double value = 0;
            for (int i = 0; i < basicStatYear.Count; i++)
                value += basicStatYear[i].Sum;
            return value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public double SumAcrossReps()
        {
            double value = 0;
            for (int i = 0; i < basicStatRep.Count; i++)
                value += basicStatRep[i].Sum;
            return value;
        }

        /// <summary>
        /// Writes this object's stats to the stream according to the outFlags.
        /// </summary>
        /// <param name="stream">The stream to be written to.</param>
        public void Save()
        {
            //Output data.
            if ((outFlags & (int)OutFlags.outData) > 0)
            {
                System.IO.StreamWriter file = new System.IO.StreamWriter(Path.Combine(Global.Instance.StatOutputDirectory, title + ".txt"), false);
                file.Write("Year");
                for (int r = 0; r < maxReps; r++) file.Write("\tRep " + r);
                file.WriteLine("");
                for (int y = 0; y <= maxYears / timeStep; y++)
                {
                    file.Write(y + "\t");
                    for (int r = 0; r < maxReps; r++)
                    {
                        file.Write(data[y][r] + "\t");
                    }
                    file.WriteLine("");
                    file.Flush();
                }
                file.WriteLine("");
                file.Close();
            }

            //Output events.
            if ((outFlags & (int)OutFlags.outEvents) > 0)
            {
                System.IO.FileStream file = new System.IO.FileStream(Path.Combine(Global.Instance.StatOutputDirectory, title + "Events.txt"), System.IO.FileMode.Create);
                System.IO.StreamWriter stream = new System.IO.StreamWriter(file);
                stream.WriteLine("Year\tRep\tValue\tCause\tLow\tMod\tHighLSS\tHighHSS");
                string yearAndRep;
                for (int r = 0; r < maxReps; r++)
                {
                    for (int y = 0; y <= maxYears / timeStep; y++)
                    {
                        yearAndRep = y + "\t" + r + "\t";
                        foreach (List<FireSizeEvent> eventList in eventLists[y, r])
                        {
                            foreach (FireSizeEvent e in eventList)
                            {
                                stream.WriteLine(yearAndRep + e.Value + "\t" + e.Cause + "\t" + e.LowCount + "\t" + e.ModCount + "\t" + e.HighLssCount + "\t" + e.HighHssCount);
                            }
                        }
                    }
                }
                stream.Flush();
                stream.Close();
                file.Close();
            }
        }

        private void ClearEvents()
        {
            for (int y = 0; y < maxYears / timeStep + 1; y++)
            {
                for (int r = 0; r < maxReps; r++)
                {
                    foreach (List<FireSizeEvent> eventList in eventLists[y, r])
                    {
                        eventList.Clear();
                        eventList.Capacity = 0;
                    }
                    eventLists[y, r].Clear();
                    eventLists[y, r].AddLast(new List<FireSizeEvent>());  //To return to state after Statistic constructor.
                }
            }
        }

        private void AddEvent(int year, int rep, double value, int cause, int low, int mod, int hiLSS, int hiHSS)
        {
            try
            {
                FireSizeEvent se;
                se.Value = value;
                se.Cause = cause;
                se.LowCount = low;
                se.ModCount = mod;
                se.HighLssCount = hiLSS;
                se.HighHssCount = hiHSS;

                List<FireSizeEvent> curentBucket = eventLists[year, rep].Last.Value;
                try { curentBucket.Add(se); }
                catch (System.OutOfMemoryException)
                {
                    //Ideally all new buckets are created when checking capacity below.
                    curentBucket = eventLists[year, rep].AddLast(new List<FireSizeEvent>(BUCKET_CAPACITY)).Value;
                    curentBucket.Add(se);
                }
                // Check capacity and start new bucket if needed (First bucket might not be == 'capacity').
                // ?lock?
                if (curentBucket.Count == curentBucket.Capacity && curentBucket.Count >= BUCKET_CAPACITY)
                {
                    curentBucket = eventLists[year, rep].AddLast(new List<FireSizeEvent>(BUCKET_CAPACITY)).Value;
                }
            }
            catch (System.OutOfMemoryException e)
            {
                string m = "";
                m += "Failed adding statistical event to " + title + " stat due to System.OutOfMemoryException.\n";
                m += "Event year=" + year + " and rep=" + rep + ".  \n";
                m += "Number of buckets for this year and rep: " + eventLists[year, rep].Count + "\n";
                m += "Current bucket's count=" + eventLists[year, rep].Last.Value.Count;
                m += " and capacity=" + eventLists[year, rep].Last.Value.Capacity + "\n";
                throw new Exception(m, e);
            }
        }
    }
}