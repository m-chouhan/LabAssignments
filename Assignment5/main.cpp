
/*Assignment 5:
 * Printing the longest substring using suffix tree
 * Author: Mahendra Chouhan
 *         14CS60R12
 */ 

#include <iostream>
#include <assert.h>

#include "Suffix_Tree.cpp"

using namespace std;

int main(int argc,char *argv[])
{
    assert(argc == 2);
    
    Suffix_Node S(0,0);
    S.SetString(argv[1]);
    
    Strlist list = S.PrintBranches();
    
    cout<<endl;
    S.Print();
    cout<<endl; 
    return 0 ;
}

