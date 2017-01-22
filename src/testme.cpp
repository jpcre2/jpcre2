/**@file testme.cpp
 * Examples of using MatchEvaluator to perform replace.
 * 
 * @include testme.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"
#include <cassert>

typedef jpcre2::select<char> jp;
typedef jp::String String;

String toString (size_t x){
    return jpcre2::ConvInt<char>::toString((int)x);
}

String myme1(jp::NumSub m1, void*, void*){
    return "("+m1[0]+")";
}

String myme2(void*, jp::MapNas m2, void*){
    return "("+m2["total"]+")";
}

String myme3(jp::NumSub m1,jp::MapNas m2, void*){
    return "("+m1[0]+"/"+m2.at("total")+")";
}

String myme4(void*, void*, jp::MapNtN m3){
    return "("+toString(m3["total"])+")";
}

String myme5(jp::NumSub m1, void*, jp::MapNtN m3){
    return "("+m1[0]+"/"+toString(int(m3["total"]))+")";
}

String myme6(void*, jp::MapNas m2, jp::MapNtn m3){
    return "("+m2["total"]+"/"+toString(m3["total"])+")";
}

String myme7(jp::NumSub m1, jp::MapNas m2, jp::MapNtn m3){
    return "("+m1[0]+"/"+m2["total"]+"/"+toString(m3["total"])+")";
}

int main(){
    jp::Regex re("(?<total>\\w+)", "n");
    jp::RegexReplace rr(&re);
    
    String s3 = "I am ঋ আা a string 879879 fdsjkll ১ ২ ৩ ৪ অ আ ক খ গ ঘ আমার সোনার বাংলা";
    
    
    rr.setSubject(&s3)
      .setPcre2Option(PCRE2_SUBSTITUTE_GLOBAL);
      
    rr.setSubject(s3); //this is allowed too
    
    #if __cplusplus >= 201103L
    //example with lambda
    std::cout<<"\n\n### Lambda\n"<<rr.nreplace(
                jp::MatchEvaluator([](jp::NumSub m1, jp::MapNas m2, void*)
                {
                    return "("+m1[0]+"/"+m2["total"]+")";
                }));
    #endif
    
    std::cout<<"\n\n### 1\n"<<rr.nreplace(jp::MatchEvaluator(myme1));
    std::cout<<"\n\n### 2\n"<<rr.nreplace(myme2);
    std::cout<<"\n\n### 3\n"<<rr.nreplace(myme3);
    std::cout<<"\n\n### 4\n"<<rr.nreplace(myme4);
    std::cout<<"\n\n### 5\n"<<rr.nreplace(myme5);
    std::cout<<"\n\n### 6\n"<<rr.nreplace(myme6);
    std::cout<<"\n\n### 7\n"<<rr.nreplace(myme7);
    
    
    // Some random sanity check
    rr.setRegexObject(0);
    assert(rr.nreplace(myme1)==s3);
    
    rr.setRegexObject(&re).setPcre2Option(0).nreplace(myme2);
    
    jp::MatchEvaluator me1([](jp::NumSub m1, jp::MapNas m2, void*)
                {
                    return "("+m1[0]+"/"+m2["total"]+")";
                });
    
    jp::MatchEvaluator me2 = me1;
    jp::MatchEvaluator me3 = me2;
    
    me1 = jp::MatchEvaluator([](jp::NumSub m1, jp::MapNas m2, jp::MapNtN m3)
                {
                    return "("+m1[0]+"/"+m2["total"]+")";
                });
    me2 = me1;
    me1 = jp::MatchEvaluator([](void*, jp::MapNas m2, jp::MapNtN m3)
                {
                    return "("+m2["total"]+")";
                });
                
    me3 = me1;
    me1 = jp::MatchEvaluator([](void*, void*, jp::MapNtN m3)
                {
                    return "("+toString(m3["total"])+")";
                });
    
    me2 = me1;
    me1 = me3;
    
    return 0;
}
