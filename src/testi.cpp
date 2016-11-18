#include <iostream>
#include <pthread.h>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

struct MyRegex{
    jp::Regex re[5];
};

void *task(void *arg){
    pthread_mutex_lock( &mutex1 );
    std::cout<<"\nStarting thread...\n";
    MyRegex* re = (MyRegex*) arg;
    std::string sub[5] = {"subject1", "123456789", "1a2b3c", "1a 2b 3c ", "I am a string"};
    for(int i = 0;i<5; ++i){
        std::cout<<"\nMatch count for re["<<i<<"]:\t"<<re->re[i].match(sub[i], "g");
    }
    std::cout<<"\n\nThread finished\n";
    pthread_mutex_unlock( &mutex1 );
    return 0;
}

    
int main(void){
    //~ pthread_t thread[5];
    
    //~ MyRegex re;
    //~ re.re[0].compile("\\w","Si");
    //~ re.re[1].compile("\\d","Si");
    //~ re.re[2].compile("\\d\\w+","i");
    //~ re.re[3].compile("\\d\\w\\s","m");
    //~ re.re[4].compile("[\\w\\s]+","m");
    
    //~ for(size_t i = 0;i<5;++i){
        //~ if(pthread_create( &thread[0], NULL, task, &re)) std::cerr<<"Error creating thread";
        //~ else pthread_join(thread[0],NULL);
    //~ }
    
    //~ pthread_exit((void*) 0);
    
    unsigned char* c = (unsigned char*)malloc(1088);
    
    std::cout<<sizeof(unsigned char);
    
    for(int i = 0; i<1088;++i) std::cout<<c[i];
    
    return 0;

}
