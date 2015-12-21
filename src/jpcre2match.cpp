#include <iostream>
#include "jpcre2.h"

#define getLine(a) std::getline(std::cin,a,'\n');


int main(){

    jpcre2::VecNum vec_num0;   ///Vector to store numbured substring Map.
    jpcre2::VecNas vec_nas0;   ///Vector to store named substring Map.
    jpcre2::VecNtN vec_nn0;    ///Vector to store Named substring to Number Map.
    
   
    std::string pat,mod,subject;
    {
    jpcre2::Pcre2Regex re;
}
 loop:
{
   jpcre2::Pcre2Regex re;
    std::cout<<"Enter pattern: ";
    getLine(pat);
    cp:
    std::cout<<"Enter modifiers (eimsuxADJSU): ";
    getLine(mod);
    
    ///Compile the pattern
    try{re.compile(pat,mod);}                                  ///Always use try catch block to avoid                
    catch(int e){std::cout<<re.getErrorMessage(e)<<std::endl;goto cp;}     ///unexpected termination of program in case of errors
    

    ///subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    if(subject=="quit")return 0;
    
    try{re.match(subject,vec_num0,vec_nas0,vec_nn0,true);}          ///true makes it to find all matches
    catch(int e){std::cout<<re.getErrorMessage(e);goto loop;}   
}
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
    if(subject!="quit")goto loop;
	return 0;
}
