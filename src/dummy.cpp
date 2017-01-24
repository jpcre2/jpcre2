
#include <iostream>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "jpcre2.hpp"
#include <cassert>


class A{
    int a;
    
    public:
    explicit A():a(1)
    {}
    explicit A(bool);
    explicit A(int);
    operator void* const(){
        return (a != 0)?this:0;
    }
    
};

typedef jpcre2::select<char> jp;
typedef jpcre2::select<wchar_t> jpw;

int main(){
    jp::Regex("fds").match("fds");
    A b;
    if(b);
    
     
    
    return 0;
}
