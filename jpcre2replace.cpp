#include <iostream>
#include "header-only/jpcre2.h"

#define getLine(a) std::getline(std::cin,a,'\n');


int main(){
    std::string pat,mod,subject,repl,repl_mod;
    
    std::cout<<"Enter pattern: "<<std::endl;
    getLine(pat);
    
    std::cout<<"Enter modifiers (eimsuxADJSU): "<<std::endl;
    getLine(mod);
    jpcre2::Pcre2Regex re(pat,mod);
    
    
    ///Compile the pattern
    try{re.compile();}                      ///Always use try catch block to avoid                
    catch(int e){std::cout<<re.getErrorMessage(e);}     ///unexpected termination of program in case of errors
    
    
    loop:
    ///subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    if(subject=="quit")return 0;
     ///replacement string
    std::cout<<"\nEnter replacement string: "<<std::endl;
    getLine(repl);
    
    std::cout<<"\nEnter replacement modifiers: "<<std::endl;
    getLine(repl_mod);
    
    //perform replace
    
    try{std::cout<<"\nreplaced string: "<<re.replace(subject,repl,repl_mod);}
    ///2 is the length of the returned string. Though, it will be expanded as needed, pass a large enough value to contain the string,
    ///otherwise internal substitute function will be called twice which will eat up some additional resource.
    catch(int e){std::cout<<re.getErrorMessage(e);}
    
    if(subject!="quit")goto loop;
	return 0;
}
