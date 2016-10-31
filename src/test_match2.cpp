/**@file test_match2.cpp
 * Contains an example to take subject string, pattern and modifier
 * from user input and perform regex match using JPCRE2.
 * @include test_match2.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"


void getLine(std::string& a) { std::getline(std::cin,a,'\n'); }

typedef jpcre2::select<char> jp;

int main(){

    jp::VecNum vec_num;   //Vector to store numbered substring vectors.
    jp::VecNas vec_nas;   //Vector to store named substring Map.
    jp::VecNtN vec_ntn;   //Vector to store Named substring to Number Map.
    
   
    std::string pat, mod, subject, ac_mod;
    
    //create an object
    jp::Regex re;

    std::cout<<"Enter pattern: ";
    getLine(pat);
    
    while(true){
        std::cout<<"Enter compile modifiers (eijmnsuxADJSU): ";
        getLine(mod);
        
        //Compile pattern
        re.compile(pat, mod);
        
        //Check if the pattern was compiled successfully, continue the loop otherwise
        if(!re){std::cerr<<re.getErrorMessage()<<std::endl;continue;}
        break;
    }
    
    std::cout<<"\nPattern compiled with modifiers: "<<re.getModifier();

    size_t matched = 0;
    
    re.initMatch()                                //invoke the initMatch() function
      .setNumberedSubstringVector(&vec_num)       //pointer to numbered substring vector
      .setNamedSubstringVector(&vec_nas)          //pointer to named substring vector
      .setNameToNumberMapVector(&vec_ntn)         //pointer to name-to-number map vector
      //.match()                                  //Let's do the match later
      ;
		
        
    for(;;) { //forever loop
        
        std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
        getLine(subject);
        if(subject == "quit") break;
        
		std::cout<<"\nEnter action (matching) modifier (Ag): "<<std::endl;
		getLine(ac_mod);
        
        //Now let's do the match
        matched = re.getMatchObject()                           //returns a reference to the previously initialized match object
                    .setSubject(subject)						//subject
                    .addModifier(ac_mod)                        //add modifier
                    .match();                                   //Now perform the match
          
        //Now let's access the matched data

        //Each of these vectors contains maps, except the VecNum which contains vectors.
        //Each element in the vector specifies a particular match
        //First match is the vector element 0, second is at index 1 and so forth
        //A map or sub vector for a vector element, i.e for a match, contains all of its substrings/captured groups
        //The first element of the map or sub vector is capture group 0 i.e total match
        std::cout<<"\nTotal number of matches: "<<matched<<std::endl;
        if(matched){
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
        }
        else std::cout<<"\nNo match found\n";
    }
	return 0;
}
