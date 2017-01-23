/**@file test16.cpp
 * An example of performing regex match and regex replace with std::wstring and std::u16string.
 * @include test16.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#define JPCRE2_USE_CHAR1632
#include "jpcre2.hpp"

typedef jpcre2::select<wchar_t> jpw; //for windows it's 16
#if __cplusplus >= 201103L
typedef jpcre2::select<char16_t> jpu; //sizeof(char16_t)*CHAR_BIT will be taken as default value for BS
#endif

int main() {
    
    //Working with wchar_t i.e std::wstring
    //In Linux this will require 32-bit library to be linked, because, wchar_t is 32 bit in Linux.
    //Match:
    std::cout<<"\n"<<
    jpw::Regex(L"[\\w]+").match(L"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    std::wcout<<"\n"<<
    jpw::Regex(L"subject").replace(L"I am a subject",L"string"); 
    
    
    #if __cplusplus >= 201103L
    //Working with char16_t i.e std::u16string (>=C++11)
    
    //Match:
    std::cout<<"\n"<<
    jpu::Regex(u"[\\w]+").match(u"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    jpcre2::Convert16 conv;
    std::cout<<"\n"<<
    conv.to_bytes(jpu::Regex(u"subject").replace(u"I am a subject",u"string")); //print by converting it to utf-8
    #endif
    
    return 0;
}
