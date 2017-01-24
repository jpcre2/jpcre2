/**@file testme.cpp
 * Examples of using MatchEvaluator to perform replace.
 * 
 * @include testme.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
//~ #define JPCRE2_USE_FUNCTIONAL
#include "jpcre2.hpp"
#include <cassert>

typedef jpcre2::select<char> jp;
typedef jp::String String;

String toString (size_t x){
    return jpcre2::ConvInt<char>::toString((int)x);
}

String myme1(const jp::NumSub& m1, void*, void*){
    return "("+m1[0]+")";
}

String myme2(void*, const jp::MapNas& m2, void*){
    return "("+m2.at("total")+")";
}

String myme3(const jp::NumSub& m1,const jp::MapNas& m2, void*){
    jp::MapNas mn2 = m2;
    return "("+m1[0]+"/"+mn2["total"]+")";
}

String myme4(void*, void*, const jp::MapNtN& m3){
    jp::MapNtN mn3 = m3;
    return "("+toString(mn3["total"])+")";
}

String myme5(const jp::NumSub& m1, void*, const jp::MapNtN& m3){
    jp::MapNtN mn = m3;
    return "("+m1[0]+"/"+toString(int(mn["total"]))+")";
}

String myme6(void*, const jp::MapNas& m2, const jp::MapNtn& m3){
    jp::MapNas mn2 = m2;
    jp::MapNtN mn3 = m3;
    return "("+mn2["total"]+"/"+toString(mn3["total"])+")";
}

String myme7(const jp::NumSub& m1, const jp::MapNas& m2, const jp::MapNtn& m3){
    jp::MapNas mn2 = m2;
    jp::MapNtN mn3 = m3;
    return "("+m1[0]+"/"+mn2["total"]+"/"+toString(mn3["total"])+")";
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
                jp::MatchEvaluator(
                    [](const jp::NumSub& m1, const jp::MapNas& m2, void*){
                        return "("+m1[0]+"/"+m2.at("total")+")";
                    }
                ));
    #endif
    
    std::cout<<"\n\n### 1\n"<<rr.nreplace(jp::MatchEvaluator(myme1));
    std::cout<<"\n\n### 2\n"<<rr.nreplace(jp::MatchEvaluator(myme2));
    std::cout<<"\n\n### 3\n"<<rr.nreplace(jp::MatchEvaluator(myme3));
    std::cout<<"\n\n### 4\n"<<rr.nreplace(jp::MatchEvaluator(myme4));
    std::cout<<"\n\n### 5\n"<<rr.nreplace(jp::MatchEvaluator(myme5));
    std::cout<<"\n\n### 6\n"<<rr.nreplace(jp::MatchEvaluator(myme6));
    std::cout<<"\n\n### 7\n"<<rr.nreplace(jp::MatchEvaluator(myme7));
    
    
    // Some random sanity check
    rr.setRegexObject(0);
    assert(rr.nreplace(jp::MatchEvaluator(myme1))==s3);
    
    rr.setRegexObject(&re).setPcre2Option(0).nreplace(jp::MatchEvaluator(myme2));
    
    
    #if __cplusplus >= 201103L
    jp::MatchEvaluator me1([](const jp::NumSub& m1, const jp::MapNas& m2, void*)
                {
                    return "("+m1[0]+"/"+m2.at("total")+")";
                });
    
    jp::MatchEvaluator me2 = me1;
    jp::MatchEvaluator me3 = me2;
    
    me1 = jp::MatchEvaluator([](const jp::NumSub& m1, const jp::MapNas& m2, const jp::MapNtN& m3)
                {
                    jp::MapNas mn2 = m2;
                    return "("+m1[0]+"/"+mn2["total"]+")";
                });
    me2 = me1;
    me1 = jp::MatchEvaluator([](void*, const jp::MapNas& m2, const jp::MapNtN& m3)
                {
                    jp::MapNas mn2 = m2;
                    return "("+mn2["total"]+")";
                });
                
    me3 = me1;
    me1 = jp::MatchEvaluator([](void*, void*, const jp::MapNtN& m3)
                {
                    return "("+toString(m3.at("total"))+")";
                });
    
    me2 = me1;
    me1 = me3;
    
    #endif
    return 0;
}
