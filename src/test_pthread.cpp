/**@file test_pthread.cpp
 * A multi threaded example with POSIX pthread
 * @include test_pthread.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include <pthread.h>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx3 = PTHREAD_MUTEX_INITIALIZER;
pthread_attr_t thattr;

void sleep(double sec){
    clock_t st = clock();
    while(((double)(clock()-st)/CLOCKS_PER_SEC) < sec);
}


//This is an example how you can use pre-defined data objects in multithreaded program which
//will act independently of one another without any lock:
//The logic is to wrap your objects inside another class and initialize them with constructor.
//Thus when you create objects of this wrapper class, they will have identical member objects.
//see the task() function that is using this class as a wrapper class to save 5 Regex objects.
//This has the overhead of creating new objects instead of using the same ones.
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

void* thread_safe_fun1(void*){
    MyRegex re;
    std::string sub[5] = {"subject1", "123456789", "1a2b3c", "1a 2b 3c ", "I am a string"};
    for(int i = 0;i<5; ++i){
        re.re[i].match(sub[i], "g");
    }
    int c=0;
    while(c++<4){
        pthread_mutex_lock(&mtx2);
        std::cout<<"\t1";
        pthread_mutex_unlock(&mtx2);
        sleep(0.01);
    }
    return 0;
}

void* thread_safe_fun2(void*){ //uses no global or static variable, thus thread safe.
	jp::Regex re("\\w", "i"); 
    jp::RegexMatch rm(&re);
	rm.setSubject("fdsf").setModifier("g").match();
    int c=0;
    while(c++<4){
        pthread_mutex_lock(&mtx2);
        std::cout<<"\t2";
        pthread_mutex_unlock(&mtx2);
        sleep(0.009);
    }
    return 0;
}

void* thread_safe_fun3(void*){//uses no global or static variable, thus thread safe.
    jp::Regex re("\\w", "g");
    jp::RegexMatch rm(&re);
    rm.setSubject("fdsf").setModifier("g").match();
    int c=0;
    while(c++<4){
        pthread_mutex_lock(&mtx2);
        std::cout<<"\t3";
        pthread_mutex_unlock(&mtx2);
        sleep(0.0095);
    }
    return 0;
}

jp::Regex rec("\\w", "g");

void* thread_safe_fun4(void*){
    //uses global variable 'rec', but uses
    //mutex lock, thus thread safe when the thread is joined with the main thread.
    //But when thread is detached from the main thread, it won't be thread safe any more,
    //because, the main thread will destroy the rec object while possibly being used by the detached child thread.
    pthread_mutex_lock(&mtx1);
    jp::RegexMatch rm(&rec);
    rm.setSubject("fdsf").setModifier("g").match();
    pthread_mutex_unlock(&mtx1);
    int c=0;
    while(c++<4){
        pthread_mutex_lock(&mtx2);
        std::cout<<"\t4";
        pthread_mutex_unlock(&mtx2);
        sleep(0.008);
    }
    return 0;
}

int main(){
    pthread_t th1, th2, th3, th4;
    pthread_attr_init(&thattr);
    pthread_attr_setdetachstate(&thattr, PTHREAD_CREATE_DETACHED);
    
    if(pthread_create( &th1, &thattr, thread_safe_fun1, 0));
    if(pthread_create( &th2, &thattr, thread_safe_fun2, 0));
    if(pthread_create( &th3, &thattr, thread_safe_fun3, 0));
    if(pthread_create( &th4, 0, thread_safe_fun4, 0));
    else pthread_join(th4,0); //detach is unsafe for this one.

    pthread_attr_destroy(&thattr);
    pthread_exit((void*) 0);
    return 0;

}
