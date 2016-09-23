/**@file test_match.cpp
 * An example of performing regex match against a pattern with JPCRE2 and getting the
 * match count and match results.
 * Shows how to iterate over the match results to get the captured groups/substrings.
 * @include test_match.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#define PCRE2_CODE_UNIT_WIDTH 8
#include "jpcre2.hpp"


typedef jpcre2::select<char> jp;

int main(){
    jp::VecNum vec_num0;   //Vector to store numbered substring Maps.
    jp::VecNas vec_nas0;   //Vector to store named substring Maps.
    jp::VecNtN vec_nn0;    //Vector to store Named substring to Number Maps.
    
    jp::Regex re;
    
    //Compile the pattern
    re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")  //set pattern
      .setModifier("mi")                                                    //set modifier
      .addJpcre2Option(jpcre2::JIT_COMPILE)                                 //perform JIT compile
      .addPcre2Option(PCRE2_DUPNAMES)                                       //add pcre2 option
      .compile();                                                           //Finally compile it.
    
    // JIT error is a harmless error, it just means that an optimization failed.
    
    //subject string
    std::string subject = "(I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56)";
    
    size_t count = 0;
    
    count = re.initMatch()                                  //Invoke the initMatch() function
              .addModifier("g")                             //set various parameters
              .setSubject(subject)                          //...
              .setNumberedSubstringVector(&vec_num0)        //...
              .setNamedSubstringVector(&vec_nas0)           //...
              .setNameToNumberMapVector(&vec_nn0)           //...
              .addPcre2Option(0)                            //...
              .match();                                     //Finally perform the match
    
    std::cerr<<"\n"<<re.getErrorMessage();
    
    
    // re.reset(); // re-initialize re
    
    
    std::cout<<"\nTotal number of mathces: "<<count<<std::endl;
    //Now let's access the matched data
    
    //Each of these vectors contains maps.
    //Each element in the vector specifies a particular match
    //First match is the vector element 0, second is at index 1 and so forth
    //A map for a vector element, i.e for a match contains all of its substrings/capture groups
    //The first element of the map is capture group 0 i.e total match
    
    
    for(size_t i=0;i<vec_num0.size();++i){
        
        
        std::cout<< "\n################## Match no: "<<i+1<<" ####################\n";
        
        
        
        //This vector contains maps with number as the key and the corresponding substring as the value
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Numbered Substrings (number: substring) for match "<<i+1<<" ---\n";
        for(jp::MapNum::iterator ent=vec_num0[i].begin();ent!=vec_num0[i].end();++ent){
            std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
        }
        
        
        
        //This vector contains maps with name as the key and the corresponding substring as the value
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Named Substrings (name: substring) for match "<<i+1<<" ---\n";
        for(jp::MapNas::iterator ent=vec_nas0[i].begin();ent!=vec_nas0[i].end();++ent){
            std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
        }
        
        
        
        //This vector contains maps with name as the key and number as the value
        //i.e the number (of substring) can be accessed with the name for named substring.
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Name to number mapping (name: number/position) for match "<<i+1<<" ---\n";
        for(jp::MapNtN::iterator ent=vec_nn0[i].begin();ent!=vec_nn0[i].end();++ent){
            std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
        }
    }
	return 0;
}
