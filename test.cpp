#include <iostream>
#include "jpcre2.hpp"




int main(){

    jpcre2::VecNum vec_num0;   ///Vector to store numbured substring Map.
    jpcre2::VecNas vec_nas0;   ///Vector to store named substring Map.
    jpcre2::VecNtN vec_nn0;    ///Vector to store Named substring to Number Map.
    
    jpcre2::Pcre2Regex re("(?:(?<name>\\d+)|(?<name>\\w+))\\s*(?<nam>\\d+d)","Jiu");
    ///                       ^this is the pattern                             ^this is the modifier
    ///We can also use re.setPattern() and re.setModifier() to set pattern and modifier.
    
    ///Compile the pattern
    try{re.compile();}                      ///Always use try catch block to avoid                
    catch(std::string e){std::cout<<e;}     ///unexpected termination of program in case of errors
    
    ///subject string
    std::string s="(I am a digit 67d আ 89d 4323d 3432D)";
    
    try{std::cout<<"\nrepl: "<<re.replace(s,"$1$22${name}","xE",2);}
    catch(std::string e){std::cout<<e;}
    
    try{re.match(s,vec_num0,vec_nas0,vec_nn0,true);}
    catch(std::string e){std::cout<< e<<re.getErrorCode();}
    
    ///Now let's access the matched data
    for(int i=0;i<(int)vec_num0.size();i++){
        std::cout<< "\n####################################\n";
        for(auto const& ent : vec_num0[i]){
            std::cout<<"\n"<<ent.first<<": "<<ent.second<<"\n";
        }
        std::cout<< "\n----------------------------------\n";
        for(auto const& ent : vec_nas0[i]){
            std::cout<<"\n"<<ent.first<<": "<<ent.second<<"\n";
        }
        
        std::cout<< "\n----------------------------------\n";
        for(auto const& ent : vec_nn0[i]){
            std::cout<<"\n"<<ent.first<<": "<<ent.second<<"\n";
        }
    }
	return 0;
}
