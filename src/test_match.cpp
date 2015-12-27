#include <iostream>
#include "jpcre2.h"

///This example requires C++11, see pcre2match.cpp for a non c++11 example.


int main(){

    jpcre2::VecNum vec_num0;   ///Vector to store numbured substring Map.
    jpcre2::VecNas vec_nas0;   ///Vector to store named substring Map.
    jpcre2::VecNtN vec_nn0;    ///Vector to store Named substring to Number Map.
    
    jpcre2::Regex re;
    ///                       ^this is the pattern                             ^this is the modifier
    ///We can also use re.setPattern() and re.setModifier() to set pattern and modifier.
    
    ///Compile the pattern
    try{re.compile("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)","JiuX");} ///Always use try catch block to avoid                
    catch(int e){std::cout<<re.getErrorMessage(e);}     ///unexpected termination of program in case of errors
    
    ///subject string
    std::string s="(I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56)";
    int count=0;
    try{count=re.match(s,vec_num0,vec_nas0,vec_nn0,true);}          ///true makes it to find all matches
    catch(int e){std::cout<<re.getErrorMessage(e);}   
    
    std::cout<<"\nTotal number of mathces: "<<count<<std::endl;
    ///Now let's access the matched data
    
    ///Each of these vectors contains a map
    ///and each of the maps contains all the substrings that are matched against the pattern.
    ///All the matches in all the maps combines the total match throughout the entire string.
    for(int i=0;i<(int)vec_num0.size();i++){
        
        
        std::cout<< "\n################## Match no: "<<i+1<<" ####################\n";
        
        
        
        ///This vector contains maps with number as the key and the corresponding substring as the value
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Numbered Substrings (number: substring) for match "<<i+1<<" ---\n";
        for(auto const& ent : vec_num0[i]){
            std::cout<<"\n\t"<<ent.first<<": "<<ent.second<<"\n";
        }
        
        
        
        ///This vector contains maps with name as the key and the corresponding substring as the value
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Named Substrings (name: substring) for match "<<i+1<<" ---\n";
        for(auto const& ent : vec_nas0[i]){
            std::cout<<"\n\t"<<ent.first<<": "<<ent.second<<"\n";
        }
        
        
        
        ///This vector contains maps with name as the key and number as the value
        ///i.e the number (of substring) can be accessed with the name for named substring.
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Name to number mapping (name: number/position) for match "<<i+1<<" ---\n";
        for(auto const& ent : vec_nn0[i]){
            std::cout<<"\n\t"<<ent.first<<": "<<ent.second<<"\n";
        }
    }
	return 0;
}
