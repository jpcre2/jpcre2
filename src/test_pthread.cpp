/**@file test_pthread.cpp
 * A multi threaded example with POSIX pthread
 * @include test_pthread.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include <pthread.h>
#include "jpcre2.hpp"
#include <unistd.h>

typedef jpcre2::select<char> jp;

pthread_mutex_t mutex[20] = PTHREAD_MUTEX_INITIALIZER;

struct MyRegex{
    jp::Regex re[5];
};

void *task(void *arg){
    pthread_mutex_lock( &mutex[0] );
    MyRegex re;
    re.re[0].compile("\\w","Si");
    re.re[1].compile("\\d","Si");
    re.re[2].compile("\\d\\w+","i");
    re.re[3].compile("\\d\\w\\s","m");
    re.re[4].compile("[\\w\\s]+","m");
    std::string sub[5] = {"subject1", "123456789", "1a2b3c", "1a 2b 3c ", "I am a string"};
    for(int i = 0;i<5; ++i){
        re.re[i].match(sub[i], "g");
    }
    pthread_mutex_unlock( &mutex[0] );
    return 0;
}

void *thread_safe_fun1(void *arg){ //uses no global or static variable, thus thread safe.
	jp::Regex re("\\w", "i"); //It's a local variable
	re.getMatchObject().setSubject("fdsf").setModifier("g").match();
    return 0;
}

jp::Regex rec("\\w", "g");

void *thread_pseudo_unsafe_fun1(void *arg){ //uses global variable 'rec', thus thread unsafe.
    //making it thread safe with mutex lock
    jp::Regex re = rec;
    pthread_mutex_lock( &mutex[1] );
    jp::RegexMatch rm(&re); //It's a local variable
    rm.setSubject("fdsf").setModifier("g").match();
    pthread_mutex_unlock( &mutex[1]);
    return 0;
}

void* thread_safe_fun2(void* arg){//uses no global or static variable, thus thread safe.
    jp::Regex re("\\w", "g");
    jp::RegexMatch rm(&re); //It's a local variable
    rm.setSubject("fdsf").setModifier("g").match();
    return 0;
}

int main(){
    pthread_t thread[30];
    
    for(size_t i = 0;i<5;++i){
        while(pthread_create( &thread[i], 0, task, 0));
        //else pthread_join(thread[i],0);
    }
    
    for(size_t i=5;i<10;++i){
        if(pthread_create( &thread[i], 0, thread_safe_fun1, 0));
        //else pthread_join(thread[5],0);
    }
    
    for(size_t i=10;i<15;++i){
        if(pthread_create( &thread[i], 0, thread_pseudo_unsafe_fun1, 0));
        //else pthread_join(thread[5],0);
    }
    
    for(size_t i=15;i<20;++i){
        if(pthread_create( &thread[i], 0, thread_safe_fun2, 0));
        //else pthread_join(thread[5],0);
    }
    
    
    
    pthread_exit((void*) 0);
    return 0;

}
