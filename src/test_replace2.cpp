/**@file test_replace2.cpp
 * Contains an example to take subject string, replacement string, modifier and pattern
 * from user input and perform regex replace with JPCRE2
 * @include test_replace2.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */


#include <iostream>
#define PCRE2_CODE_UNIT_WIDTH 8
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

#define getLine(a) std::getline(std::cin,a,'\n')


int main(){
    std::string pat,mod,subject,repl,repl_mod;

    std::cout<<"\nEnter pattern: ";
    getLine(pat);

    std::cout<<"\nEnter compile modifiers (eijmnsuxADJSU): ";
    getLine(mod);
    jp::Regex re;   

    // Compile the pattern
    re.compile(pat,mod);
    
    //check if it was a success
    if(!re){std::cerr<<re.getErrorMessage();} 
    
    //if(re) is only available for >=C++11, use if(!!re) as an alternative
    
    /* // >= C++11
    if(re) std::cout<<"\n Success";
    else std::cout<<"\n Failure";
    */
    
    if(!!re) std::cout<<"\n Compile Success";
    else std::cout<<"\n Compile Failure";

    // subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    if(subject=="quit")return 0;
    
     //replacement string
    std::cout<<"\nEnter replacement string: "<<std::endl;
    getLine(repl);

    std::cout<<"\nEnter action (replacement) modifiers (eEgx): ";
    getLine(repl_mod);

    //perform replace

    std::cout<<"\nreplaced string: "<<
        re.initReplace()
          .setSubject(subject)
          .setReplaceWith(repl)
          .addModifier(repl_mod)
          .replace();

	return 0;
}
