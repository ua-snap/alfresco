using System;
using System.Collections.Generic;
using System.Text;

namespace FRESCO_Server
{
    struct SMean 
    {
        float Mean;
        float StandardDeviation;
    };


    class BasicStatistic
    //The statistical gathering functions are housed in two classes.  The first (CBasicStat) 
    //is the basic statistical routines which store enough data to provide basic stats on
    //whatever data is stored in the object.  The second (CStat) provides a framework by which
    //it is possible to automatically store data across replicates and time steps automatically.
    //This is done by maintaining arrays of objects of CBasicStat and updating the relevent
    //objects as necessary.
    {
        private long num        = 0;
        private double sum	    = 0;
        private double sumSqr	= 0;
        private double min		= 0;
        private double max		= 0;
    
        /// <summary>
        /// Set all values to zero.
        /// </summary>
        public void Clear()
        {
            num     = 0;
            sum	    = 0;
            sumSqr	= 0;
            min		= 0;
            max		= 0;
        }

        /// <summary>
        /// Add a statistic to the data storage.
        /// </summary>
        /// <param name="value">Value</param>
        /// <returns>Returns the number of entries.</returns>
        public long Add(double value)
        {
            sum += value;
            sumSqr += (value * value);
            if (num != 0)
            {
                min = (value < min) ? value : min;
                max = (value > max) ? value : max;
            }
            else
            {
                min = value;
                max = value;
            }
            return ++num;
        }

        /// <summary>
        /// Return the mean.
        /// </summary>
        /// <returns>Mean</returns>
        public double Mean()
        {
            double result = 0;
            if (num == 0) result = 0;
            else result = sum / (double)num;
            return result;
        }

        public double StdDev
        {
            get
            {
                double result = 0;
                if (num - 1 <= 0) result = 0;
                else result = System.Math.Sqrt((sumSqr - (sum * sum / (double)num)) / (double)(num - 1));
                return result;
            }
        }

        public long Num
        {
            get
            {
                return num;
            }
        }
        public double Min
        {
            get
            {
                return min;
            }
        }
        public double Max
        {
            get
            {
                return max;
            }
        }
        public double Sum
        {
            get
            {
                return sum;
            }
        }
    }
}