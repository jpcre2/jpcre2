/**@file testmd.cpp
 * 
 * Examples of working with modifier table.
 * 
 * @include testmd.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */
//~ #define NDEBUG
#include <iostream>
#define JPCRE2_DISABLE_CHAR1632 //being compatible with older compilers like gcc >=4.8 while using c++11
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;
typedef jp::String String;


int main(){
    
    const char* nametab = "IJMS"; //arbitrary modifier character.
    //now the option values sequentially
    jpcre2::Uint valtab[] = { PCRE2_CASELESS, PCRE2_DUPNAMES, PCRE2_MULTILINE, jpcre2::JIT_COMPILE };
    //if the above two doesn't have the same number of elements, the behavior is undefined.
    
    //init ModifierTable
    jpcre2::ModifierTable mdt; //creates default table.
    
    //change the Compile modifier table only:
    mdt.setCompileModifierTable(nametab, valtab);
    
    jp::Regex re;
    
    re.setModifierTable(mdt).compile("JPCRE2","I");
    
    jp::RegexMatch rm(re);
    
    std::cout<<rm.setSubject("jpcre2").match(); //capital I is now working as caseless modifier.
    //and the small i is now an invalid modifier :
    re.compile("JPCRE2", "i");
    std::cout<<rm.match(); //doesn't match, small i is not a valid modifier now.
    
    
    
    return 0;
}
