#include <iostream>
#include <thread>

void fun(void){
    int a = 0;
}

int main(){
    std::thread th1(fun);
    th1.join();
    return 0;
}
