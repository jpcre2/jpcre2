/**@file test32.cpp
 * An example of performing regex match and regex replace with std::wstring and std::u32string
 * for 32 bit code unit width
 * @include test32.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#define PCRE2_CODE_UNIT_WIDTH 32
#include "jpcre2.hpp"

typedef jpcre2::select<wchar_t> jpw;
typedef jpcre2::select<char32_t> jpu;

int main() {
    
    //Working with wchar_t i.e std::wstring
    
    //In Windows this will give compile time errors because wchar_t in Windows is 16 bit but 32 bit code unit is defined
    //Match:
    std::cout<<"\n"<<
    jpw::Regex(L"[\\w]+").match(L"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    std::wcout<<"\n"<<
    jpw::Regex(L"subject").replace(L"I am a subject",L"string"); 
    
    
    
    
    //Working with char32_t i.e std::u32string (>=C++11)
    
    //Match:
    std::cout<<"\n"<<
    jpu::Regex(U"[\\w]+").match(U"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    std::cout<<"\n"<<
    jpcre2::convert32.to_bytes(jpu::Regex(U"subject").replace(U"I am a subject",U"string")); //printing by converting it to utf-8
    
    
    return 0;
}
