#include <iostream>

void fun(size_t* x){
    std::cout<<"\n"<<(*x);
}

int main(){
    int rc=0;
    fun((size_t*)&rc); //should print 0, but outputs random value
    
    size_t c=0;
    fun(&c); //prints 0
    return 0;
}
