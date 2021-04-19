/**@file test0.cpp
 * An example of handling std::string and std::wstring
 * @include test0.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jpc;
typedef jpcre2::select<wchar_t> jpw;

#ifdef JPCRE2_UNSET_CAPTURES_NULL
#define VEC_NUM8 *vec_num8
#define VEC_NUM32 *vec_num32
#else
#define VEC_NUM8 vec_num8
#define VEC_NUM32 vec_num32
#endif
   
int main(){
    jpc::Regex   rec;
    jpw::Regex  rew;
    
    rec.setPattern("\\d+").compile();
    rew.setPattern(L"\\d+").compile();

    jpw::VecNum vec_num32;
    jpcre2::VecOff vec_eoff;
    
    jpw::RegexMatch rmw;
    size_t count =
    rmw.setRegexObject(&rew)
       .setSubject(L"123456789")
       .setModifier("g")
       .setNumberedSubstringVector(&vec_num32)
       .setMatchEndOffsetVector(&vec_eoff)
       .match();
    
    std::cout<<"\nMatch count: "<<count;
    std::wcout<<"\nFirst match: "<<VEC_NUM32[0][0];
    std::cout<<"\nMatch ended at offset: "<<vec_eoff[vec_eoff.size()-1];

    std::cout<<"\n--------------------------------\n";


    jpc::RegexMatch rm;
    jpc::RegexReplace rr;
    
    rm.setRegexObject(&rec);
    rr.setRegexObject(&rec);
    

    jpc::VecNum vec_num8;
    rm.setSubject("I am a subject with digits 3343242 4433243 443244")
      .setModifier("g")
      .setNumberedSubstringVector(&vec_num8)
      .match();
     
    std::cout<<"\nFirst match: " + VEC_NUM8[0][0];
    
    jpc::Regex rec_2("[\\S]+");
    rm.setRegexObject(&rec_2)
      .setSubject("I am subject")
      .setNumberedSubstringVector(&vec_num8)
      .match();
    std::cout<<"\nFirst match: " + VEC_NUM8[0][0];
    
    std::cout<<"\nReplace: " + 
            rr.setSubject("I am a subject with digits 3343242 4433243 443244")
              .setReplaceWith("@")
              .setModifier("g")
              .replace();
    
    
    std::cout<<"\nReplace2: " + 
            rr.setSubject("I am a subject with digits 3343242 4433243 443244")
              .setReplaceWith("@")
              .setModifier("g")
              .setRegexObject(&rec_2)
              .replace();
   
   return 0;
   }
