
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Suffix_Node;
typedef vector<Suffix_Node *> Nodelist;
typedef Nodelist::iterator NIter;

class Suffix_Node{
        
        Nodelist Childs;
        int start,end,id;
        static int ID;
        static string String; 
        Suffix_Node(int start,int end)
        {
            this->start = start;
            this->end = end;
            id = ID++;
        }
        /* Pushes string into childs or as one of its child */
        void add(Suffix_Node * node)
        {
            int index1 = node->start,index2 = start;
            
            while(String[index1] == String[index2]) index1++,index2++;
            
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
        }
        
        /*Prints Strings Using DFS*/
        void Print()
        {
            
        }
        
        void SetString(string str)
        {
                String = str;
                const char *ptr = String.c_str();
                int i = 0,length = String.length();
                
                /*Loop over to create diff suffixes and try to  push them*/
                while(i<length)
                {
                    Suffix_Node *node = new Suffix_Node(i,length-1);
                    NIter it = Childs.begin();
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
                    if(it == Childs.end());    
                    i++;
                } 
        }
        
    };

int Suffix_Node::ID = 0;
string Suffix_Node::String = "";

int main(int argc,char *argv[])
{
    return 0 ;
}

