/**@file test.cpp
 * A miscellaneous example experimenting on different situations (test suit).
 * This test will try to reach every nook and cranny of the library
 * and test all kinds of situations.
 * Do not take codes in this file as reference, because it includes
 * code regardless of good or bad practices, efficiency etc...
 * 
 * You will often notice that these codes make no sense whatsoever.
 * 
 * Primary goals of this test:
 * 
 * 1. Check for memory leak
 * 2. Check for thread safety
 * 3. Cover almost 100% codes
 * 
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include <iostream>
#include "jpcre2.hpp"

#define PS

int err(size_t line, const std::string& s=""){
    std::cerr<<"Error at line: "<<line<<"\n"+s;
    return 1;
}

typedef jpcre2::select<char> jp;

int main(){
    std::string big_text(PS"\n\
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

    jp::Regex re;
    //testing various situations
    re = jp::Regex();
    re = jp::Regex("[\\S]+"); //check copy assignment is done
    re = jp::Regex("[.]+", "eijmnsuxADJUS"); //check modifiers
    re = jp::Regex(".*", PCRE2_ANCHORED);
    re = jp::Regex(".*", PCRE2_ANCHORED, jpcre2::JIT_COMPILE);
    
    //check pointer versions
    re = jp::Regex(&big_text); //check copy assignment is done
    re = jp::Regex(&big_text, "eijmnsuxADJUS"); //check modifiers
    re = jp::Regex(&big_text, PCRE2_ANCHORED);
    re = jp::Regex(&big_text, PCRE2_ANCHORED, jpcre2::JIT_COMPILE);
    
    jp::Regex re2(re); //check on copy constructor
    
    re2 = jp::Regex(re);
    
    re2.setLocale(LC_ALL, "en_US.UTF-8").compile();
    
    jp::Regex re3(re2);
    
    re = jp::Regex(re3);
    //check bollean operator
    if(re);
    if(!re) err(__LINE__);
    
    re.resetErrors();
    re.reset();
    re.addModifier("eijmnsuxADJUS");
    re.addPcre2Option(PCRE2_ANCHORED);
    re.addJpcre2Option(jpcre2::JIT_COMPILE);
    re.getLocale();
    re.getLocaleTypeId();
    re.getErrorMessage();
    re.getErrorNumber();
    re.getErrorOffset();
    re.getPattern();
    re.getPcre2Option();
    re.getJpcre2Option();
    re.getModifier();
    
    re.initMatch();
    re.getMatchObject();
    re.initMatch();
    re.getMatchObject();
    
    re3 = re;
    re2 = re;
    re = re3;
    
    re.compile("(?<name>\\d+)(?<name>\\w+)", ""); //regex error, dupname.
    re.getErrorMessage();
    if(re) err(__LINE__, "dupname error not realized");
    re.compile("(?<name>\\d+)(?<name>)\\w+","J"); //dupname allowed
    if(!re) err(__LINE__, "J modifier not reqlized");
    
    re.getMatchObject().reset().resetErrors();
    re.initMatch().reset().resetErrors();
    jp::RegexMatch& m = re.getMatchObject();
    
    m.match(); re.match();
    m.match("s"); re.match("s");
    m.match("s", "g"); re.match("s", "g");
    m.match(&big_text); re.match(&big_text);
    m.match(&big_text, "g"); re.match(&big_text, "g");
    m.match(&big_text, "g", 2); re.match(&big_text, "g", 2);
    
    jp::RegexMatch rm;
    jp::VecNum vec_num;
    jp::VecNtN vec_ntn;
    jp::VecNas vec_nas;
    
    rm = jp::RegexMatch(&re);
    rm.setSubject("d");
    rm.setSubject(&big_text);
    jp::RegexMatch rm2 = rm;
    jp::RegexMatch rm3 = jp::RegexMatch(rm2);
    rm2.reset();
    rm.resetErrors();
    
    rm.setNumberedSubstringVector(&vec_num);
    rm.setNamedSubstringVector(&vec_nas).setNameToNumberMapVector(&vec_ntn);
    jp::Regex re4(PS"\\w+");
    rm.setRegexObject(&re4);
    size_t c = rm.match(&big_text, "g", 0);
    if(c == 0) err(__LINE__, "match should have been successful");
    
    vec_num[0][0]; //should not give segfault.
    
    re.initMatchFrom(rm);
    re.getMatchObject().match();
    re.getMatchObject().match();
    re.initMatchFrom(rm2);
    re.initMatch();
    re.match();
    
    
    rm.getErrorMessage();
    rm.getErrorNumber();
    rm.getErrorOffset();
    rm.getPcre2Option();
    rm.getJpcre2Option();
    rm.getModifier();
    rm.getSubject();
    
    rm.addPcre2Option(0);
    rm.addModifier("g");
    rm.addJpcre2Option(jpcre2::FIND_ALL);
    rm.setFindAll();
    
    
    
    
    
    //check replace
    
    jp::RegexReplace rr, rr2, rr3;
    
    rr = jp::RegexReplace(&re2);
    
    rr2 = rr;
    rr2 = jp::RegexReplace(0);
    rr2.setRegexObject(&re);
    rr = rr2;
    
    rr.setBufferSize(2);
    
    rr.replace();
    rr2.replace();
    
    re.initReplace();
    
    rr = rr2;
    rr3 = jp::RegexReplace(&re2);
    
    rr.replace(); re.replace();
    rr.replace("fds"); re.replace("fds");
    rr.replace("fds", "$$"); re.replace("fds", "$$");
    rr.replace("fds", "$$", "g"); re.replace("fds", "$$", "g");
    rr.replace(&big_text); re.replace(&big_text);
    rr.replace(&big_text, "$$"); re.replace(&big_text, "$$");
    rr.replace(&big_text, "$$", "g"); re.replace(&big_text, "$$", "g");
    rr.replace(&big_text, &big_text); re.replace(&big_text, &big_text);
    rr.replace(&big_text, &big_text, "g"); re.replace(&big_text, &big_text, "g");
    
    
    rr.getErrorMessage();
    rr.getErrorNumber();
    rr.getErrorOffset();
    rr.getPcre2Option();
    rr.getJpcre2Option();
    rr.getModifier();
    rr.getSubject();
    rr.getReplaceWith();
    
    rr.addModifier("g");
    rr.addPcre2Option(0);
    rr.addJpcre2Option(0);
    
    re = *rr.getRegexObject();
    
    
    
    return 0;
}
