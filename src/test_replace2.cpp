#include <iostream>
#include "jpcre2.h"
#define getLine(a) std::getline(std::cin,a,'\n');


int main(){
    std::string pat,mod,subject,repl,repl_mod;
    
    std::cout<<"\nEnter pattern: ";
    getLine(pat);
    
    std::cout<<"\nEnter compile modifiers (eijmnsuxADJSU): ";
    getLine(mod);
    jpcre2::Regex re;     ///An empty object is not supposed to throw any exception in normal cases.
    
    
    ///Compile the pattern
    try{re.compile(pat,mod).execute();}  
    catch(int e){std::cout<<re.getErrorMessage(e);}
    
           
    /***************************************************************************************************************
     * Always use try catch block to catch any exception and avoid unexpected termination of the program.
     * All jpcre2 exceptions are of type int (integer)
     * *************************************************************************************************************/
    
    
    ///subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    if(subject=="quit")return 0;
     ///replacement string
    std::cout<<"\nEnter replacement string: "<<std::endl;
    getLine(repl);
    
    loop2:
    std::cout<<"\nEnter action (replacement) modifiers (eEgx): ";
    getLine(repl_mod);
    
    //perform replace
    
    try{std::cout<<"\nreplaced string: "<<re.replace(subject,repl)
                                            .modifiers(repl_mod)
                                            .jpcre2Options(jpcre2::VALIDATE_MODIFIER)
                                            .execute();}
    ///2 is the length of the returned string. Though, it will be expanded as needed, pass a large enough value to contain the string,
    ///otherwise internal substitute function will be called twice which will eat up some additional resource.
    catch(int e){std::cout<<re.getErrorMessage(e);
        if(e==jpcre2::ERROR::INVALID_MODIFIER) goto loop2;
    }
    std::cout<<"\n\n--------------------------------------------------\n";
    main();
	return 0;
}
