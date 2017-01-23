/**@file test32.cpp
 * An example of performing regex match and regex replace with std::wstring and std::u32string.
 * @include test32.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#define JPCRE2_USE_CHAR1632
#include "jpcre2.hpp"

typedef jpcre2::select<wchar_t> jpw;
#if __cplusplus >= 201103L
typedef jpcre2::select<char32_t> jpu;
#endif

int main() {
    
    //Working with wchar_t i.e std::wstring
    //In Windows this will require 16-bit library to be linked, because wchar_t in windows is 16-bit
    //Match:
    std::cout<<"\n"<<
    jpw::Regex(L"[\\w]+").match(L"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    std::wcout<<"\n"<<
    jpw::Regex(L"subject").replace(L"I am a subject",L"string"); 
    
    
    
    #if __cplusplus >= 201103L
    //Working with char32_t i.e std::u32string (>=C++11)
    
    //Match:
    std::cout<<"\n"<<
    jpu::Regex(U"[\\w]+").match(U"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    jpcre2::Convert32 conv;
    std::cout<<"\n"<<
    conv.to_bytes(jpu::Regex(U"subject").replace(U"I am a subject",U"string")); //printing by converting it to utf-8
    #endif
    
    return 0;
}
