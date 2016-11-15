/**@file test_match.cpp
 * An example of performing regex match against a pattern with JPCRE2 and getting the
 * match count and match results.
 * Shows how to iterate over the match results to get the captured groups/substrings.
 * @include test_match.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"


typedef jpcre2::select<char> jp;

int main(){
    jp::VecNum vec_num;   //Vector to store numbered substring vectors.
    jp::VecNas vec_nas;   //Vector to store named substring Maps.
    jp::VecNtN vec_ntn;   //Vector to store Named substring to Number Maps.
    
    jp::Regex re;
    
    //Compile the pattern
    re.setPattern("(?:(?<w_s>[.?#@]+)|(?<w_s>\\w+))\\s*(?<digit>\\d+)")  //set pattern
      .setModifier("minJ")                                               //set modifier
      .addJpcre2Option(jpcre2::JIT_COMPILE)                              //perform JIT compile
      .addPcre2Option(0)                                                 //add pcre2 option
      .compile();                                                        //Finally compile it.
      re.setModifier("fdsfsd"); //creating a invalid modifier error
    std::cerr<<re.getErrorMessage()<<re.getErrorNumber();
    // JIT error is a harmless, it just means that an optimization failed.
    
    //subject string
    std::string subject = "(I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56)";
    
    size_t count = 0;
    
    count = re.initMatch()                                  //create a match object
              .addModifier("gi")                            //set various parameters
              //'invalid modifier: i' error (i is a compile modifier)
              //modifier error is harmless
              .setSubject(subject)                          //...
              .setNumberedSubstringVector(&vec_num)         //...
              .setNamedSubstringVector(&vec_nas)            //...
              .setNameToNumberMapVector(&vec_ntn)           //...
              .addPcre2Option(0)                            //...
              .match();                                     //Finally perform the match
    
    std::cerr<<"\n"<<re.getMatchObject().getErrorMessage();
    
    
    // re.reset(); // re-initialize re
    
    
    std::cout<<"\nTotal number of mathces: "<<count<<std::endl;
    //Now let's access the matched data
    
    //Each of these vectors contains maps, except the VecNum which contains vectors.
    //Each element in the vector specifies a particular match
    //First match is the vector element 0, second is at index 1 and so forth
    //A map or sub vector for a vector element, i.e for a match, contains all of its substrings/captured groups
    //The first element of the map or sub vector is capture group 0 i.e total match
    
    
    for(size_t i=0;i<vec_num.size();++i){
        
        
        std::cout<< "\n################## Match no: "<<i+1<<" ####################\n";
        
        
        
        //This vector contains vectors of substrings or captured group index by index.
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Numbered Substrings (number: substring) for match "<<i+1<<" ---\n";
        for(size_t j=0;j<vec_num[i].size();++j){
            std::cout<<"\n\t"<<j<<": "<<vec_num[i][j]<<"\n";
        }
        
        
        
        //This vector contains maps with name as the key and the corresponding substring as the value
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Named Substrings (name: substring) for match "<<i+1<<" ---\n";
        for(jp::MapNas::iterator ent=vec_nas[i].begin();ent!=vec_nas[i].end();++ent){
            std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
        }
        
        
        
        //This vector contains maps with name as the key and number as the value
        //i.e the number (of substring) can be accessed with the name for named substring.
        std::cout<<"\n-------------------------------------------------------------------------";
        std::cout<< "\n--- Name to number mapping (name: number/position) for match "<<i+1<<" ---\n";
        for(jp::MapNtN::iterator ent=vec_ntn[i].begin();ent!=vec_ntn[i].end();++ent){
            std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
        }
    }
	return 0;
}
