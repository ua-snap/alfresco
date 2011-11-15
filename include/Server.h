#ifndef Fresco_Server_h
#define Fresco_Server_h


class Server
{
//Data
public:
    enum ServerState
    {
        S_UNKNOWN,
        S_NOT_LOADED,      				//FIF not loaded.
        S_LOADED,						//FIF loaded.
        S_SAVING,						//FIF being saved.
        S_CLEARING,						//Run being cleared.
        //SIMULATING and below all imply a simulation has been started.
        S_SIMULATING,					//Simulating a run.
        S_STOPPING,						//Stopping a simulation.
        S_STOPPED,						//Simulation stopped.
        S_COMPLETED						//Simulation completed.
    };    


//Functions
public:
    Server(void)  { }
    ~Server(void) { }
};


#endif
