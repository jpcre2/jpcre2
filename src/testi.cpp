#include <iostream>
#include <cassert>
#include "jpcre2.hpp"


int main(){
    
    std::cout<<"\nstring: "<<ConvInt<char>::toString(-48);
    std::wcout<<"\nstring: "<<ConvInt<wchar_t>::toString(-2147483647);
    #if __cplusplus >= 201103L
    ConvUTF<char16_t>::Converter conv;
    ConvUTF<char32_t>::Converter conv2;
    char a[131072];
    std::cout<<"\nu16string: "<<conv.to_bytes(ConvInt<char16_t>::toString(-48));
    std::cout<<"\nu32string: "<<conv2.to_bytes(ConvInt<char32_t>::toString(-48));
    #endif
    return 0;
}
