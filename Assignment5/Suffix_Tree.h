
//#ifndef SUFFIX_TREE
//#define SUFFIX_TREE

#include <vector>
#include <queue>
#include <string>
#include <algorithm>


class Suffix_Node;
typedef std::vector<Suffix_Node *> Nodelist;
typedef Nodelist::iterator NIter;

typedef std::vector<std::string> Strlist;
typedef Strlist::iterator SIter;

struct Wrapper{
        int depth;
        Suffix_Node *node;
        Wrapper(int d,Suffix_Node *n) : depth(d),node(n) {}
    };

class Suffix_Node{
        
        Nodelist Childs;
        int start,end,id;
        /*This members will be shared by all instances of the class*/
        static int ID;
        static std::string String;
        static Strlist List;
        static std::queue<Wrapper> Q;
         
        bool IsLeaf()
        {
            NIter it;
            for( it = Childs.begin();it !=Childs.end();++it)
                if( *it == NULL) break;
            
            return ( Childs.begin() == Childs.end() || it != Childs.end() );
        }
        
        /* Pushes string into childs or as one of its child */
        void add(Suffix_Node * node)
        {
            int index1 = node->start,index2 = start;
            
            while(String[index1] == String[index2] && 
                    index2 < end && index1 < node->end) 
                    index1++,index2++;

            /*TODO:  Splitting when index2 != end */            
            if( index2 < end )
            {                
                Suffix_Node * node2 = new Suffix_Node(index2,end);
                end = index2;
                
                while(Childs.size())
                {
                    node2->Childs.push_back(Childs.back());
                    Childs.pop_back();
                }
                                    
                if(index1 == node->end ) Childs.push_back(NULL);
                else 
                {
                    node->start = index1;
                    Childs.push_back(node);
                }
                Childs.push_back(node2);                
                return;     
            } 
                                        
            /*Exec this if index2 == end ie
             * we need to search childs for common strings
             * */
            node->start = index1;
            for(NIter it = Childs.begin();it != Childs.end();++it)
            {
                    if( String[(*it)->start] == String[index1] )
                    {    
                        (*it)->add(node);
                        return;
                    }
            }
            
            Childs.push_back(node);
            return;
        }
        /*Prints Strings of nodes with branches Using DFS*/
        void PrintBranches(std::string prefix )
        {
            std::string substr = String.substr(start,end-start);
            
            if(Childs.size() >= 1 && id )
            {
                std::cout<<"\nID:"<<id<<"\t["
                    <<start<<","<<end<<"]"<<substr
                    <<"\nWholestrings:"<<std::flush;
                    
                int i = 1;                   
                while(i<=substr.length())
                {
                    List.push_back(prefix + substr.substr(0,i));
                    std::cout<<List.back()<<"\t";
                    i++;
                }    
                std::cout<<"\n";        
            }
            
            for(NIter it = Childs.begin();it != Childs.end();++it)
            {
                if(*it != NULL) (*it)->PrintBranches(prefix+substr);
            }
            
        }
        
        public:
        Suffix_Node(int start,int end)
        {
            this->start = start;
            this->end = end;
            id = ID++;
        }

        void SetString(std::string str)
        {
                String = str;
                //const char *ptr = String.c_str();
                int i = 0,length = String.length();
                
                /*Loop over to create diff suffixes and try to  push them*/
                while(i<length)
                {
                    Suffix_Node *node = new Suffix_Node(i,length);
                    NIter it = Childs.begin();
                    /*Check if its already present
                     * if yes then add it to that child*/
                    for(;it != Childs.end();++it)
                    {
                        if( String[(*it)->start] == String[node->start] )
                        {
                            (*it)->add(node);
                            break;
                        }
                        
                    }                    
                    /*No Child contains this substrings
                     * so create a new child*/
                    if(it == Childs.end())
                        Childs.push_back(node);                        
                    i++;
                } 
            return;
        }
        
        /*Prints Tree with dfs */
        void Print()
        {            
            std::cout<<"\nID:"<<id<<"\t["
                    <<start<<","<<end<<"]"<<String.substr(start,end-start)
                    <<"\tIsLeaf:"<<IsLeaf()<<"\tSize:"<<Childs.size()
                    <<std::flush;
            /**/       
            for(NIter it = Childs.begin();it != Childs.end();++it)
            {
                if(*it != NULL) 
                    (*it)->Print();
            }
        }
        
        void PrintBFS(int level = 0)
        {
            std::cout<<"\nLevel:"<<level<<"\tID:"<<id<<"\t["
                    <<start<<","<<end<<"]"<<String.substr(start,end-start)
                    <<"\tSize:"<<Childs.size()
                    <<std::flush;
            for(NIter it = Childs.begin();it != Childs.end();++it)
                if(*it != NULL) 
                    Q.push(Wrapper(level+1,*it));
                
            if(!level)
            {
                while(Q.size())
                {
                    Wrapper obj = Q.front();
                    Q.pop();
                    obj.node->PrintBFS(obj.depth);
                }
            }    
        }
        
        /*Prints Leaves of tree Using DFS*/
        void PrintLeaves(std::string prefix = "")
        {
            prefix += String.substr(start,end-start);
            
            if(IsLeaf())
                std::cout<<prefix<<"\n"<<std::flush;
    
            for(NIter it = Childs.begin();it != Childs.end();++it)
                if(*it != NULL) (*it)->PrintLeaves(prefix);
        }
        
        /*(Wrappers)*/
        /*Prints Strings of nodes with branches Using DFS*/        
        Strlist PrintSubstrings()
        {
            PrintBranches("");
            return List;
        }
        
        Strlist& getList()
        {
                return List;
        }
                    
    };

int Suffix_Node::ID = 0;
std::string Suffix_Node::String = "";
Strlist Suffix_Node::List;
std::queue<Wrapper> Suffix_Node::Q;
