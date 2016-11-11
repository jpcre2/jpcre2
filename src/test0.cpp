/**@file test0.cpp
 * An example of handling multi code unit width.
 * @include test0.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp8;
typedef jpcre2::select<wchar_t> jp32;

   
int main(){
    jp8::Regex   re8;
    jp32::Regex  re32;
    
    re8.setPattern("\\d+");
    re32.setPattern(L"\\d+");


    re8.getMatchObject().setModifier("fdsafsd");
    std::cout<<re8.getMatchObject().getErrorMessage();

    jp32::VecNum vec_num32;
    re32.getMatchObject()
        .setStartOffset(28) //match will start at offset 28.
        .setNumberedSubstringVector(&vec_num32);

    size_t count = re32.match(L"I am a subject with digits 32 43 44", "g");
    std::cout<<"\nMatch count: "<<count;
    std::wcout<<"\nFirst match: "<<vec_num32[0][0];
    std::cout<<"\nMatch ended at offset: "<<re32.getMatchObject().getEndOffset();

    std::cout<<"\n--------------------------------\n";
    
    //////////////////////////////////////////////////
    //Using new match and replace object
    #ifndef __DOXYGEN__
    jp8::RegexMatch rm(&re8);
    jp8::RegexReplace rr(&re8);
    #else
    //This is for doxygen documentation.
    jp8::RegexMatch rm; //compile error
    jp8::RegexReplace rr; //compile error
    #endif
    
    

    jp8::VecNum vec_num8;
    rm.setSubject("I am a subject with digits 3343242 4433243 443244")
      .setModifier("g")
      .setNumberedSubstringVector(&vec_num8)
      .match();
     
    std::cout<<"\nFirst match: " + vec_num8[0][0];
    
    jp8::Regex re8_2("[\\S]+");
    rm.setRegexObject(&re8_2)
      .match();
    std::cout<<"\nFirst match: " + vec_num8[0][0];
    
    std::cout<<"\nReplace: " + 
            rr.setSubject("I am a subject with digits 3343242 4433243 443244")
              .setReplaceWith("@")
              .setModifier("g")
              .replace();
    
    
    std::cout<<"\nReplace2: " + 
            rr.setSubject("I am a subject with digits 3343242 4433243 443244")
              .setReplaceWith("@")
              .setModifier("g")
              .setRegexObject(&re8_2)
              .replace();
   
   return 0;
   }
