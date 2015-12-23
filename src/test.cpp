#include <iostream>
#include "jpcre2.h"




int main(){

    jpcre2::VecNum vec_num0;   ///Vector to store numbured substring Map.
    jpcre2::VecNas vec_nas0;   ///Vector to store named substring Map.
    jpcre2::VecNtN vec_nn0;    ///Vector to store Named substring to Number Map.
    
    jpcre2::Regex re;
    ///                       ^this is the pattern                             ^this is the modifier
    ///We can also use re.setPattern() and re.setModifier() to set pattern and modifier.
    
    ///Compile the pattern
    try{re.compile("(?:(?<name>\\d+)|(?<name>\\w+))\\s*(?<nam>\\d+d)","Jiu");}                                  ///Always use try catch block to avoid                
    catch(int e){std::cout<<re.getErrorMessage(e);}     ///unexpected termination of program in case of errors
    
    ///subject string
    std::string s="(I am a digit 67d আ 89d 4323d 3432D)";
    ///############ Replacement
    try{std::cout<<"\nreplaced string: "<<re.replace(s,"$1$22${name}","xE",5);}
    ///                                       subject^  ^replacement    ^modifiers.
    ///2 is the length of the returned string. Though, it will be expanded as needed, pass a large enough value to contain the string,
    ///otherwise internal substitute function will be called twice which will eat up some additional resource.
    catch(int e){std::cout<<re.getErrorMessage(e);}
    
    
    ///########### Match
    try{re.match(s,vec_num0,vec_nas0,vec_nn0,true);}          ///true makes it to find all matches
    catch(int e){std::cout<<re.getErrorMessage(e);}  
    
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
