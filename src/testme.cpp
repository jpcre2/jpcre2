/**@file test.cpp
 * A test experimenting on different situations (sanity check).
 * 
 * The goal of this test is to go through every nook and cranny of the library
 * and touch every bit of code. Most of this test is just to see if there's any
 * major error like segfault or memory leak. It is also to measure the
 * efficiency of various blocks of code.
 * 
 * Primary goals of this test:
 * 
 * 1. Check for memory leak
 * 2. Check for segfault.
 * 3. Some test on input/output (assert).
 * 4. Estimate code coverage.
 * 
 * **Notes:**
 * 
 * * This test is not intended for client review, it's primarily for developers to make sure nothing is broken.
 * * This test doesn't  check for input/output (mostly) i.e only calls are made, validity of output is not tested. See testio.cpp for input/output test.
 * 
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"
#include <cassert>

typedef jpcre2::select<char> jp;
typedef jp::String String;
typedef jp::NumSub NumSub;
typedef jp::MapNas MapNas;
typedef jp::MapNtN MapNtN;

class A{
    int a;
    public:
    
    A& fun(){
        a = 6;
        std::cout<<"\nA"<<a;
        return *this;
        }
    virtual ~A(){}
    };
class B: public A{
    public:
    void fun2(){
        std::cout<<"B";
        }
    };

template<typename T1 = const NumSub&, typename T2 = void, typename T3 = void>
struct MatchEvaluatorCallBack{
    typedef std::function<String (const NumSub&)> CallBack;
};

template<>
struct MatchEvaluatorCallBack<const NumSub&, const MapNas&, void>{
    typedef std::function<String (const NumSub&, const MapNas&)> CallBack;
};

template<>
struct MatchEvaluatorCallBack<const NumSub&, const MapNas&, const MapNtN&>{
    typedef std::function<String (const NumSub&, const MapNas&, const MapNtN&)> CallBack;
};

template<>
struct MatchEvaluatorCallBack<void, const MapNas&, void>{
    typedef std::function<String (const MapNas&)> CallBack;
};

template<>
struct MatchEvaluatorCallBack<void, const MapNas&, const MapNtN&>{
    typedef std::function<String (const MapNas&, const MapNtN&)> CallBack;
};

template<>
struct MatchEvaluatorCallBack<void, void, const MapNtN&>{
    typedef std::function<String (const MapNtN&)> CallBack;
};

template<typename T1, typename T2, typename T3>
String callfcn(T1 a, T2 b, T3 c ){
    String s;
    
    return s;
}


int main(){
    String s = "fds";
    const char * s1 = "123456789";
    char * s2 = new char[12];
    std::memcpy(s2,s1,5);
    std::memcpy(s2+5,s1+8,1);
    std::cout<<s2;
    B b;
    b.fun();
    jp::RegexMatch rm;
    jp::MatchEvaluator me(
        [](const jp::NumSub& m, void*, void*){
            return "fdsfsdfsd";
        }
        );
    jp::Regex re("\\w+", "n");
    jp::RegexReplace rr(&re);
    
    String s3 = "I am ঋ আা a string 879879 fdsjkll ১ ২ ৩ ৪ অ আ ক খ গ ঘ আমার লোনার বাংলা";
    
    std::cout<<"\nlength: "<<s3.length();
    
    //~ for(size_t i=0;i<s3.length();++i){
        //~ std::cout<<"\n"<<s3[i];
        
    //~ }
    
    rr.setSubject(&s3)
      .setPcre2Option(PCRE2_SUBSTITUTE_GLOBAL);
    
    std::cout<<"\n"<<rr.nreplace(jp::MatchEvaluator(
        [](const jp::NumSub& m, void*, void*){
            return "("+m[0]+")";
        }
        ));
    
    return 0;
}
