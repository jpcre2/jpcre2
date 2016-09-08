/**@file test_replace2.cpp
 * Contains an example to take subject string, replacement string, modifier and pattern
 * from user input and perform regex replace with JPCRE2
 * @include test_replace2.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */


#include <iostream>
#include "jpcre2.hpp"


#define getLine(a) std::getline(std::cin,a,'\n')


int main(){
    std::string pat,mod,subject,repl,repl_mod;

    std::cout<<"\nEnter pattern: ";
    getLine(pat);

    std::cout<<"\nEnter compile modifiers (eijmnsuxADJSU): ";
    getLine(mod);
    jpcre2::Regex re;     // This is not supposed to throw any exception.


    // Compile the pattern
    try{re.compile(pat,mod);}
    catch(jpcre2::Except& e){std::cerr<<e.getErrorMessage();}


    /***************************************************************************************************************
     * All jpcre2 exceptions are of type jpcre2::Except
     * *************************************************************************************************************/


    // subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    if(subject=="quit")return 0;
     //replacement string
    std::cout<<"\nEnter replacement string: "<<std::endl;
    getLine(repl);

    // Continue loop as long as error occurs
    while(true){
		std::cout<<"\nEnter action (replacement) modifiers (eEgx): ";
		getLine(repl_mod);

		//perform replace

		try{std::cout<<"\nreplaced string: "<<re.initReplace()
												.setSubject(subject)
												.setReplaceWith(repl)
												.addJpcre2Option(jpcre2::VALIDATE_MODIFIER)
												.addModifier(repl_mod)
												.replace();}
		catch(jpcre2::Except& e){std::cerr<<e.getErrorMessage();
			if(e.getErrorNumber()==jpcre2::ERROR::INVALID_MODIFIER) continue;
		}
		break;
    }
    std::cout<<"\n\n--------------------------------------------------\n";

	return 0;
}
