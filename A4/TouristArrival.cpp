/*
 * Tourist Arrival Process
 * Creates Tourists till Max tourists limit is reached
 * then signals car load if 4 tourists are in queue
 * Author: Mahendra Chouhan(14CS60R12)
 * */

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

/*Wrappers for semaphores,sharedMemory*/
#include "semwrappers.cpp"
#include "shmwrappers.cpp"

/*Global classes and variables*/
#include "GlobalVars.cpp"
/* Signal Handler class with global flag variable to determine siginterrupt*/
#include "sigterm.cpp"

#define R(X,Y) rand() % (Y - X) + X


using namespace std;

const int N = 4;

int main(int argc,char *argv[])
{
	srand(time(NULL));//seed for random fun
	
	GLOBAL_PROCESS_IDS *GPI = (GLOBAL_PROCESS_IDS*)
								GetSHM(sizeof(GLOBAL_PROCESS_IDS),GLOBAL_MEM);

	//Put values in global shared memory for other process to see
	GPI->TouristArrival = getpid();
	GPI->CarMode = READY;

	Queue *Q = (Queue *) GetSHM(SHMSIZE,TOURISTS_Q);	
	Q->Queue_INIT(SHMSIZE-sizeof(Queue),TOURISTS_Q);
	
	SignalHandler::WaitForSignal(SIGCONT);	
	
	int Tourists = 6;
	if(argc == 2) Tourists = stoi(argv[1]);
	cout<<"Forking.."<<Tourists<<"\n"<<flush;
	
	for(int i = 0 ;i<Tourists;++i)
	{
		sleep(R(GPI->MinDelay,GPI->MaxDelay));
		
		int pid = fork();
		if(pid == 0)
		{
			execlp("./Tourist","./Tourist",NULL);
			cout<<RED<<"ERROR:"<<getpid()<<DEFAULT<<flush;			
			return 1;
		}
		
		Q->Push(pid);
		if(Q->Size() >= GPI->CarCapacity && GPI->CarMode == READY) 
			kill(GPI->CarLoad,SIGCONT);
	}
	
	cout<<"Tourist Arrival : Tourist Creation Limit exceeded...\n"
		<<"Waiting for tourists.."<<flush;
	
	while( flag )
	{
		if(GPI->CarMode == READY && Q->Size() >= GPI->CarCapacity)
				kill(GPI->CarLoad,SIGCONT);
		sleep(1);	
	}
		
	cout<<RED<<"\nTouristArrival Exiting..\n"<<DEFAULT;
	//shmdt(Q);
	//shmdt(GPI);
	/**/ 
    return 0;
}
