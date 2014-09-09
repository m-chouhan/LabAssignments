
#include <assert.h>

bool flag = true;

class SignalHandler
{
        static int signum;
        public:
        static bool flag;
        static void SigHandler(int sig)
        {
            assert(sig == signum);
            flag = false;
        }
        static void WaitForSignal(int sig)
        {
            flag = true;
            signum = sig;
            signal(signum,SignalHandler::SigHandler);
            
            std::cout<<"\n"<<getpid()<<"|SIGHANDLER[Waiting for "<<strsignal(sig)<<":"<<sig<<"]"
                        <<"\n"<<std::flush;
            while(flag && ::flag) 
                sleep(1);
            std::cout<<"Waking Up...\n"<<std::flush;    
        }
};

bool SignalHandler::flag = false;
int SignalHandler::signum = 0;

void SIGTERMHandler(int signum)
{
    assert(signum == SIGTERM);
    std::cout<<"\n["<<getpid()<<"]SigtermReceived:"<<signum<<std::flush;
    
    SignalHandler::flag = flag = false;    
}

