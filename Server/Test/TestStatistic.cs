using System;
using System.Collections.Generic;
using System.Text;

namespace FRESCO_Server
{
    public class TestStatistic
    {
        public TestStatistic() { }
        public void runTests() 
        {
            testOverloadSingleRepYear();
        }

        
        void testOverloadSingleRepYear()
        {
            try
            {
                Statistic s = new Statistic("test", 100, 100, 1, 0x831, false);
                while (true)
                {
                    s.Add(0, 0, 1, 0);
                }
            }
            catch (Exception e)
            {
                Global.Instance.RaiseSimulationFailed(this, new SimulationFailedEventArgs(e));
            }
        }
    }
}
