/**@file test_match2.cpp
 * Contains an example to take subject string, pattern and modifier
 * from user input and perform regex match using JPCRE2.
 * @include test_match2.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"


#define getLine(a) std::getline(std::cin,a,'\n')


int main(){

    jpcre2::VecNum vec_num0;   ///Vector to store numbered substring Map.
    jpcre2::VecNas vec_nas0;   ///Vector to store named substring Map.
    jpcre2::VecNtN vec_nn0;    ///Vector to store Named substring to Number Map.
    
   
    std::string pat,mod,subject;
    
    ///create an object
    jpcre2::Regex re;     /// This should not throw any exception

    std::cout<<"Enter pattern: ";
    getLine(pat);
    cp:
    std::cout<<"Enter compile modifiers (eijmnsuxADJSU): ";
    getLine(mod);
    
    ///Compile pattern
    try{re.compile(pat,mod);}
    catch(int e){std::cerr<<re.getErrorMessage(e)<<std::endl;goto cp;}
           
    /***************************************************************************************************************
     * Use try catch block to catch any exception and avoid unexpected termination of the program in case of error
     * All jpcre2 exceptions are of type int (integer)
     * *************************************************************************************************************/
    

    ///subject string
    std::cout<<"\nEnter subject string (enter quit to quit): "<<std::endl;
    getLine(subject);
    std::string ac_mod;

    size_t matched = 0;
    /// Continue loop as long as error occurs
    while(true){
		std::cout<<"\nEnter action (matching) modifier (Ag): "<<std::endl;
		getLine(ac_mod);
		if(subject=="quit")return 0;
		try{matched=re.initMatch()                                //Invoke the initMatch() function
					  .setModifier(ac_mod)                        //Set various options
					  .setNumberedSubstringVector(&vec_num0)      //...
					  .setNamedSubstringVector(&vec_nas0)         //...
					  .setNameToNumberMapVector(&vec_nn0)         //...
					  .addJpcre2Option(jpcre2::VALIDATE_MODIFIER) //...
					  .addPcre2Option(0)                          //...
					  .match();                                   //Finally do the match
		}
		catch(int e){std::cerr<<re.getErrorMessage(e);
			if(e==jpcre2::ERROR::INVALID_MODIFIER) continue;
		}
		break;
    }
    ///Now let's access the matched data

    ///Each of these vectors contains maps.
    ///Each element in the vector specifies a particular match
    ///First match is the vector element 0, second is at index 1 and so forth
    ///A map for a vector element, i.e for a match contains all of its substrings/capture groups
    ///The first element of the map is capture group 0 i.e total match
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
    //main();
	return 0;
}
