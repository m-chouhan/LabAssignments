#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctime>

#include "semwrappers.cpp"
#include "shmwrappers.cpp"
#include "GlobalVars.cpp"

using namespace std;

int main(int argc,char *argv[])
{
    assert(argc == 7);
    
    GLOBAL_PROCESS_IDS *GPI = (GLOBAL_PROCESS_IDS*)
								GetSHM(sizeof(GLOBAL_PROCESS_IDS),GLOBAL_MEM,IPC_CREAT);
    
    GPI->CarCapacity = atoi(argv[1]);
    GPI->JourneyTime = atoi(argv[2]);
    GPI->MinDelay = atoi(argv[3]);
    GPI->MaxDelay = atoi(argv[4]);
    GPI->Probabilty = atoi(argv[5]);
    strcpy(GPI->Emotion,argv[6]);
    
    string str;
    cout<<"\nPress Enter to start...";
    getline(cin,str);
    
    Queue *Q = (Queue *) GetSHM(SHMSIZE,TOURISTS_Q);
    
    //int size = GPI->CarCapacity*sizeof(TouristInfo);
    Queue *CarTable = (Queue *) GetSHM(SHMSIZE,CAR_TABLE);
    
    Queue *TouristStatus = (Queue *) GetSHM(SHMSIZE,TOURISTS_STAT);

    
    while(1)
    {
        getline(cin,str);
        cout << "\033[2J";
        
        Print_Q(Q);
        Print_Q(CarTable);
        Print_Q(TouristStatus);    
        
        cout<<"\n"
            <<"Carload:"<<GPI->CarLoad
            <<"|CarUnload:"<<GPI->CarUnload
            <<"|CarCapacity"<<GPI->CarCapacity
            <<"|TouristArrival"<<GPI->TouristArrival
            <<endl<<flush;
        if(str == "s")
                kill(GPI->TouristArrival,SIGCONT);
        if(str == "q")
        {
        	/*CLEANUP*/
            cout<<"Main:Cleaning Up";
            Q->Lock();
            while( Q->Size() > 0)
                    kill(Q->Pop(true),SIGKILL);
            Q->Unlock();
	
            kill(GPI->CarLoad,SIGTERM);
            kill(GPI->CarUnload,SIGTERM);		
            kill(GPI->TouristArrival,SIGTERM);
            sleep(2);
            Q->Destroy();
            CarTable->Destroy();
            TouristStatus->Destroy();
            DetachMemory(GLOBAL_MEM);
            
            string str;
            getline(cin,str);
            break;            
        }    
        //sleep(1);
    }
            
    return 0;
}

