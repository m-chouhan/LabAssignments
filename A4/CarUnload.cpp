
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <fstream>
#include <limits>
#include <string.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define R(X) rand() % X 

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
    srand(time(NULL));
    signal(SIGTERM,SIGTERMHandler);

	GLOBAL_PROCESS_IDS *GPI = (GLOBAL_PROCESS_IDS*)
								GetSHM(sizeof(GLOBAL_PROCESS_IDS),GLOBAL_MEM);

    //Put values in global shared memory for other process to see
    GPI->CarUnload = getpid();
    
    Queue *Q = (Queue *) GetSHM(SHMSIZE,TOURISTS_Q);
    
    int size = GPI->CarCapacity*sizeof(TouristInfo);
    Queue *CarTable = (Queue *) GetSHM(size+sizeof(Queue),CAR_TABLE);

    Queue *TouristStatus = (Queue *) GetSHM(SHMSIZE,TOURISTS_STAT);
    TouristStatus->Queue_INIT(SHMSIZE-sizeof(Queue),TOURISTS_STAT);

    SignalHandler::WaitForSignal(SIGCONT);
    
    while(flag)
    {            
        CarTable->Lock();
        cout<<"Locked";
        while(CarTable->Size() > 0)
        {
            sleep(1);
            int elem = CarTable->Pop(true);
            kill(elem,SIGCONT);
            cout<<RED<<"\nRemoving User:"<<elem<<DEFAULT<<flush;
            
            //if(false)
            if( R(100) < GPI->Probabilty)
            {
                Q->Push(elem);
                kill(elem,SIGCONT);
                cout<<GREEN<<"\nInserting User:"<<elem<<" back to Queue."<<DEFAULT<<flush;
            }
            else kill(elem,SIGTERM);
            TouristStatus->Push(elem);    
        }    
        
        CarTable->Unlock();
        
        //sleep(2);//simulate working....
        GPI->CarMode = CARMODES::READY;
        cout<<GREEN<<"\nCarMode READY\n"<<DEFAULT<<flush;
        SignalHandler::WaitForSignal(SIGCONT);
    }
    cout<<RED<<endl<<argv[0]<<" Exiting..\n"<<DEFAULT<<flush;
    
    //shmdt(Q);
	//shmdt(GPI);
    //shmdt(CarTable);
    //shmdt(TouristStatus);
    
    return 0;
}
