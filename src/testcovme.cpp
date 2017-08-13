/**@file testcovme.cpp
 * Simulating and discussing various erroneous situations.
 * 
 * This program will always exit with non-zero exit status.
 * 
 * @include testcovme.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */
//~ #define NDEBUG
#include <iostream>
#include "jpcre2.hpp"


typedef jpcre2::select<char> jp;
typedef jp::String String;

String toString (int x){
    char buf[128];
    int written = std::sprintf(buf, "%d", x);
    JPCRE2_ASSERT(written > 0, "IOError: Failed to write into buffer during int to string conversion.");
    return String(buf);
}

String callback0(void*, void*, void*){
    return "dummy";
}

String callback1(const jp::NumSub& m1, void*, void*){
    return "("+m1[0]+")";
}

String callback2(void*, const jp::MapNas& m2, void*){
    return "("+m2.at("total")+")";
}

String callback3(const jp::NumSub& m1,const jp::MapNas& m2, void*){
    jp::MapNas mn2 = m2;
    return "("+m1[0]+"/"+mn2["total"]+")";
}

String callback4(void*, void*, const jp::MapNtN& m3){
    jp::MapNtN mn3 = m3;
    return "("+toString(mn3["total"])+")";
}

String callback5(const jp::NumSub& m1, void*, const jp::MapNtN& m3){
    jp::MapNtN mn = m3;
    return "("+m1[0]+"/"+toString(mn["total"])+")";
}

String callback6(void*, const jp::MapNas& m2, const jp::MapNtn& m3){
    jp::MapNas mn2 = m2;
    jp::MapNtN mn3 = m3;
    return "("+mn2["total"]+"/"+toString(mn3["total"])+")";
}

String callback7(const jp::NumSub& m1, const jp::MapNas& m2, const jp::MapNtn& m3){
    jp::MapNas mn2 = m2;
    jp::MapNtN mn3 = m3;
    return "("+m1[0]+"/"+mn2["total"]+"/"+toString(mn3["total"])+")";
}

int main(){
    
	jp::Regex re("(?<total>\\w+)", "n");

	String s3 = "I am a string 879879 fdsjkll ১ ২ ৩ ৪ অ আ ক খ গ ঘ";
    
    jp::MatchEvaluator cme(callback1);
    cme.setSubject(&s3).setRegexObject(&re).setFindAll().match();
    
    //the following nreplace() performs a new match populating NumSub and MapNas because of callback3:
    std::cout<<"\n\n### callback3: \n"<<cme.setCallback(callback3).nreplace();
    
    //Now you can use either one of callback1, callback2 or callback3 without performing a new match,
    //as the required vectors NumSub and MapNas are both populated.
    std::cout<<"\n\n### callback2: \n"<<cme.setCallback(callback2).nreplace(false);
    
    //jp::callback::eraseFill function initiates all vectors (kinda like callback7). Thus, the following
    //populates all vectors. It erases the matched part/s from the subject string.
    std::cout<<"\n\n### default callback: \n"<<cme.setCallback(jp::callback::eraseFill).nreplace();
    //After populating all vectors, you can use any type of callback without performing the match again.
    
    
    //The following (uncomment if you wanna test) will give you assertion failure, because the callback1 only populates NumSub vector,
    //but callback2 requires pre-exisiting (due to the 'false' argument to nreplace()) MapNas data:
    cme.reset().setSubject(&s3).setRegexObject(&re).setFindAll().setCallback(callback1).nreplace();
    std::cout<<"\n\n### callback2: \n"<<cme.setCallback(callback2).nreplace(false); //Assertion failure.
    
    return 0;
}
