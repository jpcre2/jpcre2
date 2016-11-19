#include <iostream>
#include <pthread.h>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

const char s[] = "";
const int a[3] = {2,3,0};

typedef int Int;

void fun(){
    
    std::cout<<"\n"<<sizeof(s)/sizeof(char);
    std::cout<<"\n"<<sizeof(a)/sizeof(Int);
    }

int main(void){
    fun();
    
    std::cout<<s[0];
    if(s[0]==0)std::cout<<"fd";
    
    return 0;
}
