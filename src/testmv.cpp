#include <iostream>
#include <pthread.h>
#define JPCRE2_DISABLE_CHAR1632
#include "jpcre2.hpp"
#include <cstring>

#include <unistd.h>


typedef jpcre2::select<char>  jp;

int main(){
    Modifier a("");

    jp::RegexReplace rr;
    std::cout<<rr.getLastReplaceCount(); //should print 0

    jp::Regex re("\\d");
    rr.setSubject("123456789")
      .setRegexObject(&re)
      .setReplaceWith("#")
      .addModifier("g")
      .replace();

    std::cout<<rr.getLastReplaceCount(); //should print 9
    
    return 0;

}
