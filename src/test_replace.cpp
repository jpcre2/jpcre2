#include <iostream>
#include "jpcre2.h"




int main(){
    jpcre2::Regex re;     ///An empty object is not supposed to throw any exception in normal cases.

    ///Compile the pattern
    try{re.compile()                                                            //Invoke the compile() function
          .pattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")     //Set various parameters
          .modifiers("Jin")                                                    //...
          .jpcre2Options(jpcre2::VALIDATE_MODIFIER)                             //...
          .pcre2Options(0)                                                      //...
          .execute();}                                                          //Finally execute it.               
    catch(int e){std::cout<<re.getErrorMessage(e);}
        
    /***************************************************************************************************************
     * Always use try catch block to catch any exception and avoid unexpected termination of the program.
     * All jpcre2 exceptions are of type int (integer)
     * *************************************************************************************************************/
    
    //subject string
    std::string s="I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56";
    
    try{std::cout<<"\nreplaced string: \n"<<
        re.replace()                                                        //Invoke the replace() function
          .subject(s)                                                       //Set various parameters
          .replaceWith("(replaced:$1)(replaced:$2)(replaced:${word})")      //...
          .modifiers("xE")                                                  //...
          .jpcre2Options(jpcre2::VALIDATE_MODIFIER)                         //...
          .pcre2Options(0)                                                  //...
          .execute();                                                       //Finally execute it.
    }
    catch(int e){std::cout<<re.getErrorMessage(e);}
    
	return 0;
}
