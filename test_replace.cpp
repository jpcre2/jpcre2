#include <iostream>
#include "jpcre2.hpp"




int main(){

    jpcre2::Pcre2Regex re("(?:(?<name>\\d+)|(?<name>\\w+))\\s*(?<nam>\\d+d)","Jiu");

    ///Compile the pattern
    try{re.compile();}
    catch(std::string e){std::cout<<e;}
    
    std::string s="(I am a digit 67d আ 89d 4323d 3432D)";
    
    try{std::cout<<"\nrepl: "<<re.replace(s,"$1$22${name}","xE",2);}
    catch(std::string e){std::cout<<e;}
    
	return 0;
}
