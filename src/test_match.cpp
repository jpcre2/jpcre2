#include <iostream>
#include "jpcre2.h"



int main(){

    jpcre2::VecNum vec_num0;   ///Vector to store numbured substring Map.
    jpcre2::VecNas vec_nas0;   ///Vector to store named substring Map.
    jpcre2::VecNtN vec_nn0;    ///Vector to store Named substring to Number Map.
    
    jpcre2::Regex re;     ///An empty object is not supposed to throw any exception in normal cases.
    
    ///Compile the pattern
    try{re.compile()                                                            //Invoke the compile() function
          .pattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")     //set various parameters
          .modifiers("nJ")                                                      //...
          .jpcre2Options(jpcre2::VALIDATE_MODIFIER                              //modifier goes through validation check
                            | jpcre2::JIT_COMPILE                               //perform JIT compile
                            | jpcre2::ERROR_ALL)                                //treat warnings as errors
          .pcre2Options(0)                                                      //...
          .execute();}                                                          //Finaly execute it.               
    catch(int e){std::cerr<<re.getErrorMessage(e);}
    
    /***************************************************************************************************************
     * Always use try catch to catch any exception and avoid unexpected termination of the program.
     * All jpcre2 exceptions are of type int (integer)
     * *************************************************************************************************************/
    
    ///subject string
    std::string subject = "(I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56)";
    
    size_t count=0;
    
    try{count = re.match()                                   //Invoke the match() function
                  .modifiers("g")                            //set various parameters
                  .subject(subject)                          //...
                  .numberedSubstringVector(vec_num0)         //...
                  .namedSubstringVector(vec_nas0)            //...
                  .nameToNumberMapVector(vec_nn0)            //...
                  .jpcre2Options(jpcre2::VALIDATE_MODIFIER)  //...
                  .pcre2Options(0)                           //...
                  .execute();}                               //Finaly execute it.
    catch(int e){std::cerr<<re.getErrorMessage(e);}
    
    
    std::cout<<"\nTotal number of mathces: "<<count<<std::endl;
    ///Now let's access the matched data
    
    ///Each of these vectors contains a map
    ///and each of the maps contains all the substrings that are matched against the pattern.
    ///All the matches in all the maps combines the total match throughout the entire string.
    for(size_t i=0;i<vec_num0.size();++i){
        
        
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
