#include <iostream>
#include "jpcre2.h"




int main(){
    jpcre2::Regex re;
    ///                       ^this is the pattern                             ^modifiers
    ///We can also use re.setPattern() and re.setModifier() to set pattern and modifier.

    ///Compile the pattern
    try{re.compile("(?:(?<name>\\d+)|(?<name>\\w+))\\s*(?<nam>\\d+d)","Jiu");}                      ///Always use try catch block to avoid                
    catch(int e){std::cout<<re.getErrorMessage(e);}     ///unexpected termination of program in case of errors
    
    //subject string
    std::string s="(I am a digit 67d আ 89d 4323d 3432D)";
    
    try{std::cout<<"\nreplaced string: "<<re.replace(s,"$1$2${name}","xEa",JPCRE2_VALIDATE_MODIFIER);} ///produces error because of: a (invalid modifier)
    ///                                       subject^  ^replacement    ^modifiers.
    ///2 is the length of the returned string. Though, it will be expanded as needed, pass a large enough value to contain the string,
    ///otherwise internal substitute function will be called twice which will eat up some additional resource.
    catch(int e){std::cout<<re.getErrorMessage(e);}
    
	return 0;
}
