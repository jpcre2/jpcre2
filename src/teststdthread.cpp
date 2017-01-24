/**@file teststdthread.cpp
 * A multi threaded example with std::thread
 * @include teststdthread.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include <thread>
#include <mutex>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

std::mutex mtx1, mtx2, mtx3;


//this is an example how you can use pre-defined data objects in multithreaded program.
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

void task(){
    MyRegex re;
    std::string sub[5] = {"subject1", "123456789", "1a2b3c", "1a 2b 3c ", "I am a string"};
    for(int i = 0;i<5; ++i){
        re.re[i].match(sub[i], "g");
    }
}

void thread_safe_fun1(){ //uses no global or static variable, thus thread safe.
	jp::Regex re("\\w", "i"); 
	re.getMatchObject().setSubject("fdsf").setModifier("g").match();
}

void thread_safe_fun2(){//uses no global or static variable, thus thread safe.
    {
    jp::Regex re("\\w", "g");
    jp::RegexMatch rm(&re);
    
    rm.setSubject("fdsf").setModifier("g").match();
}
}

jp::Regex rec("\\w", "g");

void thread_unsafe_fun1(){ //uses global variable 'rec', thus thread unsafe.
    //this mutex lock will not make it thread safe
    mtx1.lock();
    jp::RegexMatch rm(&rec);
    rm.setSubject("fdsf").setModifier("g").match();
    mtx1.unlock();
}


int main(){
    
    for(size_t i = 0;i<5;++i){
        std::thread th(task);
        th.detach();
    }
    
    for(size_t i=5;i<10;++i){
        std::thread th(thread_safe_fun1);
        th.detach();
    }
    
    for(size_t i=10;i<15;++i){
        std::thread th(thread_safe_fun2);
        th.detach();
    }
    
    std::cout<<"ok";
    return 0;

}
