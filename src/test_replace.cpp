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

    int lrc = jp::Regex::getLastReplaceCount();
    if(lrc != 0) { std::cerr << "\n" << "jp::Regex::LastReplaceCount should be 0 but instead is " << lrc << "\n"; }
    else { std::cout << "\n" << "jp::Regex::LastReplaceCount correctly found to be " << lrc << "\n"; }

    //Compile the pattern
    re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")     //Set various parameters
      .addModifier("Jin")                                                      //
      .addPcre2Option(0)                                                       //...
      .compile();                                                              //Finally compile it.
          
    if(!re){std::cerr<<re.getErrorMessage();}
        
    
    //subject string
    std::string s="I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56";
    
    jp::RegexReplace rr;
    std::cout<<"\nreplaced string: \n"<<
    rr.setRegexObject(&re)                                               //set associated Regex object
      .setSubject(&s)                                                    //Set various parameters
      .setReplaceWith("(replaced:$1)(replaced:$2)(replaced:${word})")    //...
      .addModifier("xEafds")                                             //
      .addPcre2Option(0)                                                 //...
      .replace();                                                        //Finally perform the replace operation.
    
    if(rr.getErrorNumber() != 0)
        std::cerr<<"\n"<<rr.getErrorMessage();

    lrc = jp::Regex::getLastReplaceCount();
    if(lrc != 1) { std::cerr << "\n" << "jp::Regex::LastReplaceCount should be 1 but instead is " << lrc << "\n"; }
    else { std::cout << "\n" << "jp::Regex::LastReplaceCount correctly found to be " << lrc << "\n"; }

	return 0;
}
