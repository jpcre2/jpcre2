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
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

//this is an example how you can use pre-defined data objects in multithreaded program.
//The logic is to wrap your objects inside another class and initialize them with constructor.
//Thus when you create objects of this wrapper class, they will have identical member objects.
//see the task() function that is using this class as a wrapper class to save 5 Regex objects.
//This has the overhead of creating new objects instead of using the same ones. If you have >=c++11
//you can avoid this wrapper class by making all Regex, RegexMatch, and RegexReplace objects thread_local,
//thus you will be able to declare these class objects just like any other vairable without any wrapper class.
struct MyRegex{
    jp::Regex re[5];
    MyRegex(){
        re[0].compile("\\w","i");
        re[1].compile("\\d","i");
        re[2].compile("\\d\\w+","i");
        re[3].compile("\\d\\w\\s","m");
        re[4].compile("[\\w\\s]+","m");
    }
};

void *task(void *arg){
    {
    MyRegex re;
    std::string sub[5] = {"subject1", "123456789", "1a2b3c", "1a 2b 3c ", "I am a string"};
    for(int i = 0;i<5; ++i){
        re.re[i].match(sub[i], "g");
    }
    }
    return 0;
}

void *thread_safe_fun1(void *arg){ //uses no global or static variable, thus thread safe.
    {
	jp::Regex re("\\w", "i"); 
	re.getMatchObject().setSubject("fdsf").setModifier("g").match();
    }
    return 0;
}

void* thread_safe_fun2(void* arg){//uses no global or static variable, thus thread safe.
    {
    jp::Regex re("\\w", "g");
    jp::RegexMatch rm(&re);
    
    //jit related functions are thread unsafe
    //~ pthread_mutex_lock( &mutex3 );
    //~ rm.setJitStackSize(32*1024,0);
    //~ pthread_mutex_unlock( &mutex3 );
    
    rm.setSubject("fdsf").setModifier("g").match();
    }
    return 0;
}

jp::Regex rec("\\w", "g");

void *thread_unsafe_fun1(void *arg){ //uses global variable 'rec', thus thread unsafe.
    //this mutex lock will not make it thread safe
    pthread_mutex_lock( &mutex2 );
    jp::RegexMatch rm(&rec);
    rm.setSubject("fdsf").setModifier("g").match();
    pthread_mutex_unlock( &mutex2);
    return 0;
}

#if __cplusplus >= 201103L
thread_local jp::Regex rec1("\\w", "g");

void *thread_safe_fun3(void *arg){ //uses thread_local global variable 'rec1', thus thread safe.
    {
    jp::RegexMatch rm(&rec1);
    rm.setSubject("fdsf").setModifier("g").match();
    }
    return 0;
}
#endif

int main(){
    pthread_t thread[20];
    
    for(size_t i = 0;i<5;++i){
        while(pthread_create( &thread[i], 0, task, 0));
        //else pthread_join(thread[i],0);
    }
    
    for(size_t i=5;i<10;++i){
        if(pthread_create( &thread[i], 0, thread_safe_fun1, 0));
        //else pthread_join(thread[5],0);
    }
    
    for(size_t i=10;i<15;++i){
        if(pthread_create( &thread[i], 0, thread_safe_fun2, 0));
        //else pthread_join(thread[5],0);
    }
    #if __cplusplus >= 201103L
    for(size_t i=15;i<20;++i){
        if(pthread_create( &thread[i], 0, thread_safe_fun3, 0));
        //else pthread_join(thread[5],0);
    }
    #endif
    
    pthread_exit((void*) 0);
    return 0;

}
