#include <iostream>
#include "jpcre2.hpp"




int main(){
    jpcre2::Regex re;     ///An empty object is not supposed to throw any exception in normal cases.

    ///Compile the pattern
    try{re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")     //Set various parameters
          .setModifier("Jin")                                                     //...
          .addJpcre2Option(jpcre2::VALIDATE_MODIFIER)                             //...
          .addPcre2Option(0)                                                      //...
          .compile();}                                                          //Finally execute it.               
    catch(int e){std::cout<<re.getErrorMessage(e);}
        
    /***************************************************************************************************************
     * Always use try catch block to catch any exception and avoid unexpected termination of the program.
     * All jpcre2 exceptions are of type int (integer)
     * *************************************************************************************************************/
    
    //subject string
    std::string s="I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56";
    
    try{std::cout<<"\nreplaced string: \n"<<
        re.initReplace()                                                        //Invoke the replace() function
          .setSubject(s)                                                       //Set various parameters
          .setReplaceWith("(replaced:$1)(replaced:$2)(replaced:${word})")      //...
          .setModifier("xE")                                                  //...
          .addJpcre2Option(jpcre2::VALIDATE_MODIFIER)                         //...
          .addPcre2Option(0)                                                  //...
          .replace();                                                       //Finally execute it.
    }
    catch(int e){std::cout<<re.getErrorMessage(e);}
    
	return 0;
}
