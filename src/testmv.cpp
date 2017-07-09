#include <iostream>
#include <pthread.h>
#define JPCRE2_DISABLE_CHAR1632
#include "jpcre2.hpp"
#include <cstring>

#include <unistd.h>

class Modifier{
    std::string mod;
    
    public:
    ///Default constructor.
    Modifier(){}
    
    ///Constructor that takes a std::string.
    ///@param x std::string as a reference.
    Modifier(std::string const& x):mod(x){}
    
    ///Constructor that takes char const * (null safety is provided by this one)
    ///@param x char const *
    Modifier(char const *x):mod(x?x:""){ std::cout<<"called";}
    
    ///Returns the modifier string
    ///@return modifier string (std::string)
    std::string str() const { return mod; }
    
    ///Returns the c_str() of modifier string
    ///@return char const *
    char const * c_str() const { return mod.c_str(); }
    
    ///Returns the length of the modifier string
    ///@return length
    const size_t length() const{ return mod.length(); }
    
    ///operator[] overload to access character by index.
    ///@param i index
    ///@return character at index i.
    const char operator[](size_t i) const { return mod[i]; }
};

int main(){
    Modifier a("");
    
    return 0;

}
