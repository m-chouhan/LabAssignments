
#include <signal.h>
#include <string.h>


#define TOURISTS_Q 10
#define TOURISTS_Q_S 20*sizeof(TouristInfo)

#define TOURISTS_STAT 11
#define TOURISTS_STAT_S 40*sizeof(TouristInfo)

#define CAR_TABLE 12
//#define CAR_TABLE_S CAPACITY*sizeof(TouristInfo)

#define GLOBAL_MEM 13

#define SEMKEY 14
#define SHMKEY 15
#define SHMSIZE 2056

/*Color Codes :P*/
#define BLUE "\033["<<34<<";1m" //BLUE
#define RED "\033["<<31<<";1m" //RED
#define GREEN "\033["<<32<<";1m" //GREEN
#define DEFAULT "\033["<<39<<";0m" //DEFAULT
#define DB "\033["<<39<<";1m" //DEFAULT BOLD
#define CYAN "\033["<<36<<";1m" //CYAN
#define YELLOW "\033["<<33<<";1m" //YELLOW


enum CARMODES
{
    READY,RUNNING,WAITING
};

struct GLOBAL_PROCESS_IDS
{
    CARMODES CarMode;
    int TouristArrival;
    int CarLoad;
    int CarCapacity;
    int CarUnload;
    int JourneyTime;
    int MinDelay;
    int MaxDelay;
    int Probabilty;
    char Emotion[10];
};

struct TouristInfo
{
    private:
    int ID;
    char Arrival[26];
    char Start[26];
    char End[26];
    char Waiting[26];
    struct tm *Time()
    {
        time_t t = time(NULL);
        return localtime(&t);
    }
    public:    
    void set(int id)
    {
        ID = id;
        asctime_r(Time(),Arrival);
    }
    
    void setEnd()
    {}
    void setStart()
    {}
    void setWaiting()
    {}
    
    std::string getArr()
    {
        //char time[15] = "";
        //strftime(time,15,"%T",&Arrival);
        return Arrival;            
    }
    int getId()
    {
        return ID;
    }
};

/*
 * Implements a shared memory queue using mutex for syncronized access
 * TODO:
 * use mutex during push and pop***
 * getter for current queue size
 * Testing **
 * */
struct Queue
{
        private:
        int MAX_SIZE,KEY,SEM_ID;
        int beg,end,size;
        TouristInfo array[];
        public:
        void Queue_INIT(int Size,int key);
        int Pop(bool locked = false);
        void Push(int value,bool locked = false);
        void Lock()
        {
            LockSemaphore(SEM_ID,0);
        }
        void Unlock()
        {
            UnlockSemaphore(SEM_ID,0);
        }
        
        /*Pushes value in Queue or waits for the queue to get empty
         * for pushing 
         * BLOCKING CALL
         */ 
        
        std::string Get(int i)
        {
            assert(i<size);
            
            i = beg + i % MAX_SIZE;
            
            return std::string(std::to_string(array[i].getId())+
                                "\t"+array[i].getArr());
        }
        
        int Size()
        {
            return size;
        }
        
        void Print(const char * str,bool locked);
        void Destroy()
        {
            DeleteSemaphoreSet(SEM_ID);
            DetachMemory(SEM_ID);
        }
};

void Queue::Queue_INIT(int Size,int key) 
        {
            //array = (int *) GetSHM(size,key);
            //SEM_ID = array[0]; //First elem will b semaphore id to be used to lock varas
            //array++;// array starts from here
            
            //array = new int[size];
            MAX_SIZE = Size/sizeof(TouristInfo);
            KEY = key;
            short unsigned val = 1;
            SEM_ID = CreateSemaphoreSet(1,&val,key);
            beg = 0;end = 0;
            size = 0;
            
        }
        
void Queue::Push(int value,bool locked)
        {
            /*
            if(size == MAX_SIZE) 
            {
                std::cout<<"\nQueue full["<<size<<"]\nWaiting for pop..\n"
                    <<std::flush;
                while(size == MAX_SIZE) sleep(1);
            }/**/
            if(!locked) Lock();
            
            assert(size < MAX_SIZE);                        
            array[end++].set(value);
            end = end % MAX_SIZE;
            size++;          
            
            if(!locked) Unlock();  
        }
        
        /*Pops value from Queue or waits for the queue to get empty
         * for pushing 
         * BLOCKING CALL
         */         
int Queue::Pop(bool locked)
        {   
            /*
            if(size == 0) 
            {
                std::cout<<"\nQueue Empty["<<size<<"]\nWaiting for push..\n"
                    <<std::flush;
                while(size == 0) sleep(1);
            }/**/
            if(!locked) Lock();

            assert(size > 0);                
            int elem = array[beg++].getId();
            beg = beg % MAX_SIZE;
            size--;
            
            if(!locked) Unlock();

            return elem;
        }

void Queue::Print(const char * str,bool locked = false)
        {
            if(!locked) Lock();
            
            std::cout<<DB<<GREEN<<str<<":\n"
                <<"Process_ID\tArrival Time\n";
            
            for(int i = 0;i<size;++i)
                std::cout<<DB<<BLUE<<Get(i)<<"\n";
            std::cout<<"\nsize:"<<size<<"|Max:"<<MAX_SIZE<<std::endl
                    <<DEFAULT<<std::flush;
            
            if(!locked) Unlock();
        }

#define Print_Q(X) X->Print(#X)
