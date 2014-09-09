
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fstream>
#include <limits>
#include <string.h>

#define R(X) rand() % X + 1

/*Color Codes :P*/
#define BLUE "\033["<<34<<";1m" //BLUE
#define RED "\033["<<31<<";1m" //RED
#define GREEN "\033["<<32<<";1m" //GREEN
#define DEFAULT "\033["<<39<<";0m" //DEFAULT
#define DB "\033["<<39<<";1m" //DEFAULT BOLD
#define CYAN "\033["<<36<<";1m" //CYAN
#define YELLOW "\033["<<33<<";1m" //YELLOW

/*Wrappers for semaphores,sharedMemory*/
#include "semwrappers.cpp"
#include "shmwrappers.cpp"

/*Global classes and variables*/
#include "GlobalVars.cpp"
/* Signal Handler class with global flag variable to determine siginterrupt*/
#include "sigterm.cpp"

using namespace std;

const int MAX_LINES = 6;
string NAME;


ifstream& GotoLine(ifstream& file, unsigned int num){
    file.seekg(ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

int main(int argc,char *argv[])
{
        signal(SIGTERM,SIGTERMHandler);
        srand(time(NULL));
        
        GLOBAL_PROCESS_IDS *GPI = (GLOBAL_PROCESS_IDS*)
                            GetSHM(sizeof(GLOBAL_PROCESS_IDS),GLOBAL_MEM,IPC_CREAT);
        
        NAME = argv[0];
        
        int randm,trip = 0,pid = getpid();
        ifstream emotion(GPI->Emotion);
        string filename = "touristdump/"+to_string(pid)+".txt"; 
        //cout<<"Filename:"<<filename<<endl;
                   
        SignalHandler::WaitForSignal(SIGCONT);
     
        ofstream out(filename.c_str());
        out<<pid<<"\n";
        string INITIAL_EM,FINAL_EM,BUFFER="";
        while(flag)
        {
            trip++;
            randm = R(MAX_LINES);
            GotoLine(emotion,randm); 
            getline(emotion,INITIAL_EM);
            cout<<DB<<GREEN<<"\n"<<pid<<":trip:"<<trip<<endl//<<"random:"<<randm<<endl;    
                    <<INITIAL_EM<<endl<<DEFAULT<<flush;
            
            SignalHandler::WaitForSignal(SIGCONT);
            
            randm = R(MAX_LINES);
            GotoLine(emotion,randm);
            getline(emotion,FINAL_EM);
            //cout<<"random:"<<randm<<endl;    
            cout<<DB<<YELLOW<<pid<<":Trip:"<<trip<<":\nFinal_EM:"<<FINAL_EM
                <<endl<<DEFAULT<<flush;
            
            //BUFFER += INITIAL_EM + "\n" + FINAL_EM + "\n";
            out<<"Trip no:"<<trip<<endl<<"Initial:"<<INITIAL_EM<<endl
                <<"Final:"<<FINAL_EM<<endl<<endl<<flush;
                
            SignalHandler::WaitForSignal(SIGCONT);
        }
        //out.open(filename.c_str());
        out.close();
        emotion.close();
        cout<<RED<<"\nTourist:"<<pid<<" Exiting.."<<DEFAULT<<flush;
        return 0;
}
