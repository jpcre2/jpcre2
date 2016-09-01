#include <iostream>
#include "jpcre2.h"

#define getLine(a) std::getline(std::cin,a,'\n');


int main(){

    jpcre2::VecNum vec_num0;   ///Vector to store numbured substring Map.
    jpcre2::VecNas vec_nas0;   ///Vector to store named substring Map.
    jpcre2::VecNtN vec_nn0;    ///Vector to store Named substring to Number Map.
    
   
    std::string pat,mod,subject;
    
    ///create an object
    jpcre2::Regex re;     ///An empty object is not supposed to throw any exception in normal cases.

    std::cout<<"Enter pattern: ";
    getLine(pat);
    cp:
    std::cout<<"Enter compile modifiers (eijmnsuxADJSU): ";
    getLine(mod);
    
    ///Compile pattern
    try{re.compile(pat,mod).execute();}
    catch(int e){std::cout<<re.getErrorMessage(e)<<std::endl;goto cp;}
           
    /***************************************************************************************************************
     * Always use try catch block to catch any exception and avoid unexpected termination of the program.
     * All jpcre2 exceptions are of type int (integer)
     * *************************************************************************************************************/
    

    ///subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    std::string ac_mod;
    loop2:
    std::cout<<"\nEnter action (matching) modifier (Ag): "<<std::endl;
    getLine(ac_mod);
    if(subject=="quit")return 0;
    size_t matched=0;
    try{matched=re.match(subject)                            //Invoke the match() function
                  .modifiers(ac_mod)                         //Set various options
                  .numberedSubstringVector(vec_num0)         //...
                  .namedSubstringVector(vec_nas0)            //...
                  .nameToNumberMapVector(vec_nn0)            //...
                  .jpcre2Options(jpcre2::VALIDATE_MODIFIER)  //...
                  .pcre2Options(0)                           //...
                  .execute();                                //Finally execute it.
    }                               
    catch(int e){std::cout<<re.getErrorMessage(e);
        if(e==jpcre2::ERROR::INVALID_MODIFIER) goto loop2;
    }  

    ///Now let's access the matched data
    
    ///Each of these vectors contains a map
    ///and each of the maps contains all the substrings that are matched against the pattern.
    ///All the matches in all the maps combines the total match throughout the entire string.
    std::cout<<"\nTotal number of matches: "<<matched<<std::endl;
    if(matched){
        for(size_t i=0;i<vec_num0.size();++i){
            
            
            std::cout<< "\n################## Match no: "<<i+1<<" ####################\n";
            
            
            
            ///This vector contains maps with number as the key and the corresponding substring as the value
            std::cout<<"\n-------------------------------------------------------------------------";
            std::cout<< "\n--- Numbered Substrings (number: substring) for match "<<i+1<<" ---\n";
            for(jpcre2::MapNum::iterator ent=vec_num0[i].begin();ent!=vec_num0[i].end();++ent){
                std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
            }
            
            
            
            ///This vector contains maps with name as the key and the corresponding substring as the value
            std::cout<<"\n-------------------------------------------------------------------------";
            std::cout<< "\n--- Named Substrings (name: substring) for match "<<i+1<<" ---\n";
            for(jpcre2::MapNas::iterator ent=vec_nas0[i].begin();ent!=vec_nas0[i].end();++ent){
                std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
            }
            
            
            
            ///This vector contains maps with name as the key and number as the value
            ///i.e the number (of substring) can be accessed with the name for named substring.
            std::cout<<"\n-------------------------------------------------------------------------";
            std::cout<< "\n--- Name to number mapping (name: number/position) for match "<<i+1<<" ---\n";
            for(jpcre2::MapNtN::iterator ent=vec_nn0[i].begin();ent!=vec_nn0[i].end();++ent){
                std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
            }
        }
    }
    else std::cout<<"\nNo match found\n";
    main();
	return 0;
}
