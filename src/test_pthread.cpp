/**@file test_pthread.cpp
 * A multi threaded example with POSIX pthread
 * @include test_pthread.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include <pthread.h>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

struct MyRegex{
    jp::Regex re[5];
};

void *task(void *arg){
    std::cout<<"\n*** Thread started ***\n";
    MyRegex* re = (MyRegex*) arg;
    std::string sub[5] = {"subject1", "123456789", "1a2b3c", "1a 2b 3c ", "I am a string"};
    pthread_mutex_lock( &mutex1 );
    for(int i = 0;i<5; ++i){
        std::cout<<"\nMatch count for re["<<i<<"]:\t"<<re->re[i].match(sub[i], "g");
    }
    pthread_mutex_unlock( &mutex1 );
    std::cout<<"\n\n*** Thread finished ***\n";
    return 0;
}

    
int main(){
    pthread_t thread[5];
    
    MyRegex re;
    re.re[0].compile("\\w","Si");
    re.re[1].compile("\\d","Si");
    re.re[2].compile("\\d\\w+","i");
    re.re[3].compile("\\d\\w\\s","m");
    re.re[4].compile("[\\w\\s]+","m");
    
    for(size_t i = 0;i<5;++i){
        if(pthread_create( &thread[0], 0, task, &re)) std::cerr<<"Error creating thread";
        else pthread_join(thread[0],0);
    }
    
    pthread_exit((void*) 0);
    return 0;

}
