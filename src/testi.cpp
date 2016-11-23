#include <iostream>
#include <pthread.h>
#include "jpcre2.hpp"
#include <cstring>

typedef jpcre2::select<char> jp;

const char s[] = "";
const int a[3] = {2,3,0};

typedef int Int;

void fun(){
    
    std::cout<<"\n"<<sizeof(s)/sizeof(char);
    std::cout<<"\n"<<sizeof(a)/sizeof(Int);
    }

int main(void){
    const unsigned char* tables = pcre2_maketables_8(0);
    
    std::vector<unsigned char> tabv(1088);
    std::memcpy(&tabv[0], tables, 1088);
    
    for(int i=0;i<=1088;i++)std::cout<<tabv[i];
    
    return 0;
}
