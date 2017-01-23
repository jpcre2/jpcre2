
#include <iostream>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "jpcre2.hpp"
#include <cassert>

typedef jpcre2::select<char> jp;
typedef jpcre2::select<wchar_t> jpw;

int main(){
    jp::Regex("fds").match("fds");
    
    return 0;
}
