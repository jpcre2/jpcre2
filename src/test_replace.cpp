/**@file test_replace.cpp
 * An example of doing regex replace with JPCRE2
 * @include test_replace.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;


int main(){
    jp::Regex re; 

    //Compile the pattern
    re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")     //Set various parameters
      .addModifier("Jin")                                                      //
      .addPcre2Option(0)                                                       //...
      .compile();                                                              //Finally compile it.
          
    if(!re){std::cerr<<re.getErrorMessage();}
        
    
    //subject string
    std::string s="I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56";
    
    std::cout<<"\nreplaced string: \n"<<
    re.initReplace()                                                    //create replace object
      .setSubject(s)                                                    //Set various parameters
      .setReplaceWith("(replaced:$1)(replaced:$2)(replaced:${word})")   //...
      .addModifier("xE")                                                //
      .addPcre2Option(0)                                                //...
      .replace();                                                       //Finally perform the replace operation.
    
    if(re.getReplaceObject().getErrorNumber() != 0)
        std::cerr<<re.getReplaceObject().getErrorMessage();
    
	return 0;
}
