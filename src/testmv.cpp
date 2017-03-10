#include <iostream>
#include <pthread.h>
#define JPCRE2_DISABLE_CHAR1632
#include "jpcre2.hpp"
#include <cstring>

using namespace jpcre2;
#include <unistd.h>


    std::string const * m;

void fun(std::string const & s){
     m = &s;
    }

void fun2(){
    
    fun("fsdfsd");
    }

int main(){
    
    std::string s = 0;
    
    fun2();
    
    std::cout<<*m;
    std::cout<<*m;
    std::cout<<*m;
    return 0;

}
