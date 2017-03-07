#include <iostream>
#define JPCRE2_DISABLE_CHAR1632
#include "jpcre2.hpp"

class A{
    std::string s;
    std::vector<std::string> v;
    int* a;
    std::string* sp;
    public:
    //other ctors
    
    //move ctor
    A(A&& a){
        
    }
    //others
};

int main(){
   std::string s;
    return 0;
}
