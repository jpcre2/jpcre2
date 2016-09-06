/**@file test_replace.cpp
 * An example of doing regex replace with JPCRE2
 * @include test_replace.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"


int main(){
    jpcre2::Regex re;     /// This is not supposed to throw any exception.

    ///Compile the pattern
    try{re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")     //Set various parameters
          .addJpcre2Option(jpcre2::VALIDATE_MODIFIER)                              //...
          .setModifier("Jin")                                                      //...
          .addPcre2Option(0)                                                       //...
          .compile();}                                                             //Finally compile it.
    catch(int e){std::cerr<<re.getErrorMessage(e);}
        
    /******************************************************************************************************************
     * Use try catch block to catch any exception and avoid unexpected termination of the program in case of error
     * All jpcre2 exceptions are of type int (integer)
     * ****************************************************************************************************************/
    
    //subject string
    std::string s="I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56";
    
    try{std::cout<<"\nreplaced string: \n"<<
        re.initReplace()                                                    //Invoke the initReplace() function
          .setSubject(s)                                                    //Set various parameters
          .setReplaceWith("(replaced:$1)(replaced:$2)(replaced:${word})")   //...
          .addJpcre2Option(jpcre2::VALIDATE_MODIFIER)                       //...
          .setModifier("xE")                                                //...
          .addPcre2Option(0)                                                //...
          .replace();                                                       //Finally perform the replace operation.
    }
    catch(int e){std::cerr<<re.getErrorMessage(e);}
    
	return 0;
}
