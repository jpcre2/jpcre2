#include <iostream>
#include "jpcre2.h"




int main(){
    jpcre2::Regex re;
    ///                       ^this is the pattern                             ^modifiers
    ///We can also use re.setPattern() and re.setModifier() to set pattern and modifier.

    ///Compile the pattern
    try{re.compile("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)","JiuX");} ///Always use try catch block to avoid                
    catch(int e){std::cout<<re.getErrorMessage(e);}     ///unexpected termination of program in case of errors
    
    //subject string
    std::string s="I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56";
    
    try{std::cout<<"\nreplaced string: \n"<<
        re.replace(s,"(replaced:$1)(replaced:$2)(replaced:${word})","xEav",jpcre2::VALIDATE_MODIFIER);}
    ///                                       subject^  ^replacement    ^modifiers.
    ///2 is the length of the returned string. Though, it will be expanded as needed, pass a large enough value to contain the string,
    ///otherwise internal substitute function will be called twice which will eat up some additional resource.
    catch(int e){std::cout<<re.getErrorMessage(e);}
    
	return 0;
}
