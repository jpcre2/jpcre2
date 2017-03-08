#include <iostream>
#include <pthread.h>
//~ #define JPCRE2_DISABLE_CHAR1632
//~ #include "jpcre2.hpp"
//~ #include <cstring>

//~ using namespace jpcre2;
//~ #include <unistd.h>


pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx3 = PTHREAD_MUTEX_INITIALIZER;
pthread_attr_t tha1;

void* fun1(void*){ return 0; }
void* fun2(void*){ return 0; }

int main(){
    pthread_t th1, th2;
    pthread_attr_init(&tha1); 
    pthread_attr_setdetachstate(&tha1, PTHREAD_CREATE_DETACHED);
    
    pthread_create( &th1, &tha1, fun1, 0);
    pthread_create( &th2, &tha1, fun2, 0);
    
    pthread_attr_destroy(&tha1);
    //~ pthread_exit((void*) 0);
    return 0;

}
