/**@file test.cpp
 * A miscellaneous example experimenting on different situations.
 * @include test.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"


typedef jpcre2::select<char> jp;

int main(){
    std::string big_text("\n\
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
I am a sample big string. I am a sample big string. I am a sample big string. I am a sample big string.\n \
                        \n");

    jp::Regex re("[\\S]+");
    jp::RegexMatch rm;
    
    rm.setSubject(&big_text).setModifier("g");
    
    std::cout<<"\nMatch: "<<rm.match(); // 0 match, because no regex was specified.
    
    rm = jp::RegexMatch(&re); //equivalent efficient code: rm.reset().setRegexObject(&re);
    
    //It's more efficient to pass big text using pointer so that no copy is made:
    rm.setSubject(&big_text).setModifier("g");
    
    std::cout<<"\n---------------";
    
    jp::Regex re2;
    
    re2.setPattern("[amp]+").setModifier("i").compile();
    
    re2.initMatchFrom(rm); //makes a copy of rm.
    
    rm.setRegexObject(&re2);
    
    std::cout<<"\n-----\nThe following uses same regex and different but similar match object:";
    std::cout<<"\nMatch2: "<<rm.match();  //uses re for regex
    std::cout<<"\nMatch2: "<<re2.match(); //uses a copy of rm as match object
    
    
    rm.setStartOffset(20);
    
    std::cout<<"\n-----\nThe following uses same regex but different match object:";
    std::cout<<"\nMatch3: "<<rm.match();
    std::cout<<"\nMatch3: "<<re2.match(); //change in rm didn't affect re2, because re2 is working on a separate copy
    
    //change in re2 will affect rm, because, rm is working on re2 directly (no copy)
    std::cout<<"\n-----\nChange in associated regex object reflects on the match:";
    re2.setPattern("[\\s]+").compile();
    rm.setStartOffset(0);
    std::cout<<"\nMatch4: "<<rm.match();
    std::cout<<"\nMatch4: "<<re2.match();
    
    jp::RegexReplace rr;
    
    std::cout<<"\n"<<rr.setSubject(&big_text).setReplaceWith("$$").replace(); //no replacement, because regex was not given.
    
    std::cout<<"\n"<<rr.setRegexObject(&re2).setModifier("g").replace();
    
    re2.setPattern("[ampi]+").compile();
    
    std::cout<<"\n\n\n------\nChange in regex affects replace object because it's working on the regex object directly (no copy): \n-----\n";
    std::cout<<"\n"<<rr.replace();
    
    std::cout<<"reset: "<<rr.reset().replace(); //empty string
    
    
    return 0;
}
