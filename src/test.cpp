#include <iostream>
#include <climits>


template<bool B, class T=void>
struct validate_code_unit_width {};
 
template<class T>
struct validate_code_unit_width<true, T> { typedef T ValidCodeUnit; };

template<int S, class Char_T, typename = typename validate_code_unit_width<S == sizeof( Char_T ) * CHAR_BIT, Char_T>::ValidCodeUnit> struct A{};

int main(){
    
    A<32, wchar_t> a;
    
    return 0;
    }
