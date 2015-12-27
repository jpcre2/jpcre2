#include <iostream>

#define Glue(a,b) a##b


int main(){
    std::string s="অআইঈউঊএঐওঔকখগঘঝ";
    for(char c: s)std::cout<<c<<std::endl;
    
    
    return 0;
    }
