#include <iostream>
#include "jpcre2.h"

#define getLine(a) std::getline(std::cin,a,'\n');


int main(){
    std::string pat,mod,subject,repl,repl_mod;
    
    std::cout<<"\nEnter pattern: ";
    getLine(pat);
    
    std::cout<<"\nEnter compile modifiers (eijmsuxADJSXU): ";
    getLine(mod);
    jpcre2::Regex re;
    
    
    ///Compile the pattern
    try{re.compile(pat,mod);}                      ///Always use try catch block to avoid                
    catch(int e){std::cout<<re.getErrorMessage(e);}     ///unexpected termination of program in case of errors
    
    
    ///subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    if(subject=="quit")return 0;
     ///replacement string
    std::cout<<"\nEnter replacement string: "<<std::endl;
    getLine(repl);
    
    std::cout<<"\nEnter action (replacement) modifiers (eEgx): ";
    getLine(repl_mod);
    
    //perform replace
    
    try{std::cout<<"\nreplaced string: "<<re.replace(subject,repl,repl_mod,2,jpcre2::VALIDATE_MODIFIER);}
    ///2 is the length of the returned string. Though, it will be expanded as needed, pass a large enough value to contain the string,
    ///otherwise internal substitute function will be called twice which will eat up some additional resource.
    catch(int e){std::cout<<re.getErrorMessage(e);}
    std::cout<<"\n\n--------------------------------------------------\n";
    main();
	return 0;
}
