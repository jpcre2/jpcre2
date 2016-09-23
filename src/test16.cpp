/**@file test16.cpp
 * An example of performing regex match and regex replace with std::wstring and std::u16string
 * for 16 bit code unit width
 * @include test16.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#define JPCRE2_ENABLE_CODE_UNIT_VALIDATION
#define PCRE2_CODE_UNIT_WIDTH 16
#include "jpcre2.hpp"

typedef jpcre2::select<wchar_t> jpw;
typedef jpcre2::select<char16_t> jpu;

int main() {
    
    //Working with wchar_t i.e std::wstring
    
    /* //In Linux this will give a runtime error because wchar_t in Linux is 32 bit but 16 bit code unit is defined
     * (JPCRE2_ENABLE_CODE_UNIT_VALIDATION)
    //Match:
    std::cout<<"\n"<<
    jpw::Regex(L"[\\w]+").match(L"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    std::wcout<<"\n"<<
    jpw::Regex(L"subject").replace(L"I am a subject",L"string"); 
    */
    
    
    
    //Working with char16_t i.e std::u16string (>=C++11)
    
    //Match:
    std::cout<<"\n"<<
    jpu::Regex(u"[\\w]+").match(u"I am a subject with 7 matches", "g"); //modifier is always std::string
    
    //Replace
    std::cout<<"\n"<<
    jpcre2::convert16.to_bytes(jpu::Regex(u"subject").replace(u"I am a subject",u"string")); //print by converting it to utf-8
    
    
    return 0;
}