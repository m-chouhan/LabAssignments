
/*
 * CarLoad Process
 * Loads tourists in car then sends them to
 * tourists status table
 * Author: Mahendra Chouhan(14CS60R12)
 * */


#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*Wrappers for semaphores,sharedMemory*/
#include "semwrappers.cpp"
#include "shmwrappers.cpp"

/*Global classes and variables*/
#include "GlobalVars.cpp"
/* Signal Handler class with global flag variable to determine siginterrupt*/
#include "sigterm.cpp"

using namespace std;


int main(int argc,char *argv[])
{
    signal(SIGTERM,SIGTERMHandler);
    
	GLOBAL_PROCESS_IDS *GPI = (GLOBAL_PROCESS_IDS*)
								GetSHM(sizeof(GLOBAL_PROCESS_IDS),GLOBAL_MEM);
	//Put values in global shared memory for other process to see
    GPI->CarLoad = getpid();

    int CAPACITY = GPI->CarCapacity;
            
    Queue *Q = (Queue *) GetSHM(SHMSIZE,TOURISTS_Q);
    Queue *CarTable = (Queue *) GetSHM(SHMSIZE,CAR_TABLE);
    CarTable->Queue_INIT(CAPACITY*sizeof(TouristInfo),CAR_TABLE);
 
    SignalHandler::WaitForSignal(SIGCONT);
    
    while(flag)
    {		
        //raise(SIGSTOP);
        Q->Lock();CarTable->Lock();
        
        for(int i = 0;i<CAPACITY;++i)
        {   
            sleep(1);
            int val = Q->Pop(true);
            CarTable->Push(val,true);
            kill(val,SIGCONT);
            cout<<DB<<GREEN<<val<<"\t";
        }
        
        Q->Unlock();CarTable->Unlock();
        
        GPI->CarMode = CARMODES::RUNNING;
        cout<<"\nCar Running...\n"<<flush;
        sleep(GPI->JourneyTime);//Round trip time
        
        GPI->CarMode = CARMODES::WAITING;
        kill(GPI->CarUnload,SIGCONT);
        
        cout<<DEFAULT<<argv[0]<<" trip ended..\n"<<flush;
                
        SignalHandler::WaitForSignal(SIGCONT);
    }
    cout<<RED<<"\nExiting\n"<<DEFAULT;
    
    //shmdt(Q);
	//shmdt(GPI);
    //shmdt(CarTable);
    return 0;
}
