/**@file test0.cpp
 * An example of handling multi code unit width.
 * @include test0.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "test.cpp"
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp8;
typedef jpcre2::select<wchar_t, 32> jp32;

   
int main(){
   jp8::Regex   re8 ( "\\d+");
   jp32::Regex  re32(L"\\d+");
   
   
   re8.getMatchObject().setModifier("fdsafsd");
   std::cout<<re8.getMatchObject().getErrorMessage();
   
   size_t count = re32.match(L"I am a subject with digits 32 43 44", "g");
   std::cout<<"\nMatch count: "<<count;
   
   return 0;
   }
