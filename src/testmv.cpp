#include <iostream>
#define JPCRE2_DISABLE_CHAR1632
#include "jpcre2.hpp"
#include <cstring>

using namespace jpcre2;






int main(){
    
    //~ size_t sj = sizeof(MOD::MJ_V)/sizeof(Uint);
    //~ size_t s =  sizeof(MOD::M_V)/sizeof(Uint);
    Uint po=0,jo=0;
    int en=0;
    size_t eo=0;
    Modifier("").toOptionM(true, &po,&jo,&en,&eo);
    
    std::cout<<en<<(char)eo<<po;
    
    std::vector<std::string> v;
    v.push_back("fds");
    
    std::vector<std::string> v1;
    v1 = std::move(v);
    std::vector<std::string>().swap(v);
   
    return 0;
}
