#include <iostream>


#define PCRE2_CODE_UNIT_WIDTH 8
#define JPCRE2_DISABLE_CODE_UNIT_VALIDATION
#include "jpcre2.cpp"

int main(){
   
   jpcre2::select<char>::Regex("\\d");
    
    
    }
