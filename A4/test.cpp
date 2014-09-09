#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "semwrappers.cpp"
#include "shmwrappers.cpp"
#include "GlobalVars.cpp"

using namespace std;

int main()
{
    GLOBAL_PROCESS_IDS *GPI = (GLOBAL_PROCESS_IDS*)
								GetSHM(sizeof(GLOBAL_PROCESS_IDS),GLOBAL_MEM);
    
    Queue *Q = (Queue *) GetSHM(SHMSIZE,TOURISTS_Q);
    
    Queue *CarTable = (Queue *) GetSHM(SHMSIZE,CAR_TABLE);
    
    Queue *TouristStatus = (Queue *) GetSHM(SHMSIZE,TOURISTS_STAT);

    string str;
    while(1)
    {
        cout << "\033[2J";
        Q->Print("Q");
        CarTable->Print("C");
        TouristStatus->Print("TouristStat");    
        cout<<"\n"
            <<"Carload:"<<GPI->CarLoad
            <<"|CarUnload:"<<GPI->CarUnload
            <<"|CarCapacity"<<GPI->CarCapacity
            <<"|TouristArrival"<<GPI->TouristArrival
            <<endl<<
            flush;
        sleep(1);
    }
            
    return 0;
}
