#include <iostream>
#include "jpcre2.hpp"




int main(){

    jpcre2::Pcre2Regex re("(?:(?<name>\\d+)|(?<name>\\w+))\\s*(?<nam>\\d+d)","Jiu");
    ///                       ^this is the pattern                             ^modifiers
    ///We can also use re.setPattern() and re.setModifier() to set pattern and modifier.

    ///Compile the pattern
    try{re.compile();}                      ///Always use try catch block to avoid                
    catch(std::string e){std::cout<<e;}     ///unexpected termination of program in case of errors
    
    std::string s="(I am a digit 67d আ 89d 4323d 3432D)";
    
    try{std::cout<<"\nreplaced string: "<<re.replace(s,"$1$22${name}","xE",2);}
    ///                            subject^  ^replacement    ^modifiers.
    ///2 is the length of the returned string. Though, it will be expanded as needed, pass a large enough value to contain the string,
    ///otherwise internal substitute function will be called twice which will eat up some additional resource.
    
    catch(std::string e){std::cout<<e;}
    
	return 0;
}
