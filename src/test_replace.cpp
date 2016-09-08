/**@file test_replace.cpp
 * An example of doing regex replace with JPCRE2
 * @include test_replace.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"


int main(){
    jpcre2::Regex re;     // This is not supposed to throw any exception.

    //Compile the pattern
    try{re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")     //Set various parameters
          .addModifier("&Jin")                                                     //modifier & == jpcre2::VALIDATE_MODIFIER
          .addPcre2Option(0)                                                       //...
          .compile();}                                                             //Finally compile it.
    catch(jpcre2::Except& e){std::cerr<<e.getErrorMessage();}
        
    /******************************************************************************************************************
     * All jpcre2 exceptions are of type jpcre2::Except
     * ****************************************************************************************************************/
    
    //subject string
    std::string s="I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56";
    
    try{std::cout<<"\nreplaced string: \n"<<
        re.initReplace()                                                    //Invoke the initReplace() function
          .setSubject(s)                                                    //Set various parameters
          .setReplaceWith("(replaced:$1)(replaced:$2)(replaced:${word})")   //...
          .addModifier("~xE")                                               //modifier ~ == jpcre2::ERROR_ALL
          .addPcre2Option(0)                                                //...
          .replace();                                                       //Finally perform the replace operation.
    }
    catch(jpcre2::Except& e){std::cerr<<e.getErrorMessage();}
    
	return 0;
}
