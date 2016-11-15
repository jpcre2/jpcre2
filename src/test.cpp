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
typedef jpcre2::select<char> jpc;
typedef jpcre2::select<wchar_t> jpw;
#if __cplusplus >= 201103L
typedef jpcre2::select<char16_t> jp16;
typedef jpcre2::select<char32_t> jp32;
#define RE_TEST if(re);
#else
#define RE_TEST
#endif

int main(){
    #define FUNKY_CODE \
    jp::String text(TEXT); \
     \
    jp::Regex re; \
    /*//testing various situations*/ \
    re = jp::Regex(); \
    re = jp::Regex(PAT); /*check copy assignment is done*/ \
    re = jp::Regex(&text); \
    re = jp::Regex(PAT, "eijmnsuxADJUS"); /*//check modifiers*/ \
    re = jp::Regex(PAT, PCRE2_ANCHORED); \
    re = jp::Regex(PAT, PCRE2_ANCHORED, jpcre2::JIT_COMPILE); \
     \
    /*//check pointer versions*/ \
    re = jp::Regex(&text); /*//check copy assignment is done*/ \
    re = jp::Regex(&text, "eijmnsuxADJUS"); /*//check modifiers*/ \
    re = jp::Regex(&text, PCRE2_ANCHORED); \
    re = jp::Regex(&text, PCRE2_ANCHORED, jpcre2::JIT_COMPILE); \
     \
    re.initReplace(); \
     \
    jp::Regex re2(re); /*//check on copy constructor*/ \
     \
    re2 = jp::Regex(re); \
     \
    re2.setLocale(LC_ALL, "en_US.UTF-8").compile(); \
     \
    jp::Regex re3(re2); \
     \
    re = jp::Regex(re3); \
    /*//check bollean operator*/ \
    RE_TEST \
     \
    re.resetErrors(); \
    re.reset(); \
    re.addModifier("eijmnsuxADJUS"); \
    re.addPcre2Option(PCRE2_ANCHORED); \
    re.addJpcre2Option(jpcre2::JIT_COMPILE); \
    re.getLocale(); \
    re.getLocaleTypeId(); \
    re.getErrorMessage(); \
    re.getErrorNumber(); \
    re.getErrorOffset(); \
    re.getPattern(); \
    re.getPatternPointer(); \
    re.getPcre2Option(); \
    re.getJpcre2Option(); \
    re.getModifier(); \
     \
    re.initMatch(); \
    re.getMatchObject(); \
    re.initMatch(); \
    re.getMatchObject(); \
    re.resetErrors(); \
    re.reset(); \
    re.getMatchObject(); \
    re.getReplaceObject(); \
    re.changeJpcre2Option(0, false); \
    re.changePcre2Option(0, false); \
    re.changeModifier("i", false); \
     \
    re3 = re; \
    re2 = re; \
    re = re3; \
     \
    re.compile(PAT, "gfdsf");  \
    re.getErrorMessage(); \
    RE_TEST \
    re.compile(PAT,"J");  \
    if(!re); \
     \
    re.setModifier("fdsfsd"); \
    assert(re.getErrorNumber() == jpcre2::ERROR::INVALID_MODIFIER); \
    re.getErrorMessage(); \
     \
    re.getMatchObject().reset().resetErrors(); \
    re.initMatch().reset().resetErrors(); \
    jp::RegexMatch& m = re.getMatchObject(); \
     \
    m.match(); re.match(); \
    m.match(3); re.match(3); \
    m.match(PAT); re.match(PAT); \
    m.match(PAT, 0); re.match(PAT, 0); \
    m.match(PAT, "g"); re.match(PAT, "g"); \
    m.match(PAT, "g", 2); re.match(PAT, "g", 2); \
    m.match(&text); re.match(&text); \
    m.match(&text, 21); re.match(&text, 21); \
    m.match(&text, "g"); re.match(&text, "g"); \
    m.match(&text, "g", 22); re.match(&text, "g", 22); \
     \
    jp::RegexMatch rm; \
    jp::VecNum vec_num; \
    jp::VecNtN vec_ntn; \
    jp::VecNas vec_nas; \
     \
    rm = jp::RegexMatch(&re); \
    rm.setSubject(TEXT); \
    rm.setSubject(&text); \
    jp::RegexMatch rm2 = rm; \
    jp::RegexMatch rm3 = jp::RegexMatch(rm2); \
    rm2.reset(); \
    rm.resetErrors(); \
     \
    rm.setNumberedSubstringVector(&vec_num); \
    rm.setNamedSubstringVector(&vec_nas).setNameToNumberMapVector(&vec_ntn); \
    re = jp::Regex(PAT, "in"); \
    rm.setRegexObject(&re); \
    rm.match(&text, "g"); \
    jp::Regex re4(PAT, "niJS"); \
    rm.setRegexObject(&re4); \
    size_t count = rm.match(&text, "g", 0); \
    if(count);\
    count = rm.match(&text, "gA"); \
    count = rm.match(&text, "A"); \
     \
     \
    re.initMatchFrom(rm); \
    re.getMatchObject().match(); \
    re.getMatchObject().match(); \
    re.initMatchFrom(rm2); \
    re.initMatch(); \
    re.match(); \
    rm.addModifier("E"); \
    \
    re.reset().setNewLine(PCRE2_NEWLINE_CRLF); \
    re.compile(PAT,"J"); \
    assert(PCRE2_NEWLINE_CRLF == re.getNewLine()); \
     \
    rm.getErrorMessage(); \
    rm.getErrorNumber(); \
    rm.getErrorOffset(); \
    rm.getPcre2Option(); \
    rm.getJpcre2Option(); \
    rm.getModifier(); \
    rm.getStartOffset(); \
    rm.getEndOffset(); \
    rm.getSubject(); \
    rm.getSubjectPointer(); \
    rm.getRegexObject(); \
    rm.setJpcre2Option(0); \
    rm.setPcre2Option(0); \
    rm.changeJpcre2Option(0, true); \
    rm.changeJpcre2Option(0, false); \
    rm.changePcre2Option(0, true); \
    rm.changePcre2Option(0, false); \
     \
    rm.addPcre2Option(0); \
    rm.changePcre2Option(PCRE2_ANCHORED, true); \
    rm.changePcre2Option(PCRE2_ANCHORED, false); \
    rm.addModifier("g"); \
    rm.addJpcre2Option(jpcre2::FIND_ALL); \
    rm.changeJpcre2Option(jpcre2::FIND_ALL,false); \
    rm.changeJpcre2Option(jpcre2::FIND_ALL,true); \
    rm.setFindAll(); \
    rm.setFindAll(false); \
    rm.changeModifier("gAgfdsf", false); \
     \
     \
     \
     \
     \
    /*//check replace*/ \
     \
    jp::RegexReplace rr, rr2, rr3; \
     \
    rr = jp::RegexReplace(&re2); \
     \
    rr2 = rr; \
    rr.setSubject(&text).setReplaceWith(&text); \
    rr2 = jp::RegexReplace(rr); \
    rr2.setRegexObject(&re); \
    rr = rr2; \
     \
    rr.setBufferSize(2); \
     \
    rr.replace(); \
    rr2.replace(); \
     \
    re.initReplace(); \
     \
    rr = rr2; \
    rr3 = jp::RegexReplace(&re2); \
     \
    rr.replace(); re.replace(); \
    rr.replace(TEXT); re.replace(TEXT); \
    rr.replace(TEXT, TEXT); re.replace(TEXT, TEXT); \
    rr.replace(TEXT, &text); re.replace(TEXT, &text); \
    rr.replace(TEXT, TEXT, "g"); re.replace(TEXT, TEXT, "g"); \
    rr.replace(TEXT, &text, "g"); re.replace(TEXT, &text, "g"); \
    rr.replace(&text); re.replace(&text); \
    rr.replace(&text, TEXT); re.replace(&text, TEXT); \
    rr.replace(&text, TEXT, "g"); re.replace(&text, TEXT, "g"); \
    rr.replace(&text, &text); re.replace(&text, &text); \
    rr.replace(&text, &text, "g"); re.replace(&text, &text, "g"); \
     \
    rr.resetErrors(); \
    rr.reset(); \
    rr.getErrorMessage(); \
    rr.getErrorNumber(); \
    rr.getErrorOffset(); \
    rr.getPcre2Option(); \
    rr.getJpcre2Option(); \
    rr.addModifier("E"); \
    rr.getModifier(); \
    rr.getSubject(); \
    rr.getSubjectPointer(); \
    rr.getReplaceWith(); \
    rr.getReplaceWithPointer(); \
     \
    rr.addModifier("g"); \
    rr.addPcre2Option(0); \
    rr.addJpcre2Option(0); \
    rr.setJpcre2Option(0); \
    rr.setPcre2Option(0); \
    rr.changeJpcre2Option(0, true); \
    rr.changeJpcre2Option(0, false); \
    rr.changePcre2Option(0, true); \
    rr.changePcre2Option(0, false); \
    rr.changeModifier("gfdsf", false); \
     \
    rr.reset().replace(TEXT, TEXT); \
    rr.changePcre2Option(PCRE2_SUBSTITUTE_OVERFLOW_LENGTH, false); \
    rr.setRegexObject(&re); \
    rr.replace(TEXT, TEXT); /* replace error: */ \
     \
    const jp::Regex *rep = rr.getRegexObject(); \
    if(rep); /*//rep is not null*/ \
     \
    re.initMatchFrom(rm); \
    re.initReplaceFrom(rr); \
     \
     \
    /*//checking the string converter with null input*/ \
    assert(jp::toString((jp::Char)0) == jp::String()); \
    assert(jp::toString((jp::Char*)0) == jp::String()); \
    assert(jp::toString((jp::Pcre2Uchar*)0) == jp::String());
    
    
#define JPCRE2_JOIN(a,b) a ## b
#define JPCRE2_GLUE(a,b) JPCRE2_JOIN(a,b)
#define JPCRE2_SUFFIX(a) JPCRE2_GLUE(a,JPCRE2_LOCAL_CHAR)

#define jp JPCRE2_SUFFIX(jp)
#define rr JPCRE2_SUFFIX(rr)
#define rr2 JPCRE2_SUFFIX(rr2)
#define rr3 JPCRE2_SUFFIX(rr3)
#define rr4 JPCRE2_SUFFIX(rr4)
#define rm JPCRE2_SUFFIX(rm)
#define m JPCRE2_SUFFIX(m)
#define rm2 JPCRE2_SUFFIX(rm2)
#define rm3 JPCRE2_SUFFIX(rm3)
#define rm4 JPCRE2_SUFFIX(rm4)
#define re JPCRE2_SUFFIX(re)
#define re2 JPCRE2_SUFFIX(re2)
#define re3 JPCRE2_SUFFIX(re3)
#define re4 JPCRE2_SUFFIX(re4)
#define rep JPCRE2_SUFFIX(rep)
#define text JPCRE2_SUFFIX(text)
#define count JPCRE2_SUFFIX(count)
#define vec_num JPCRE2_SUFFIX(vec_num)
#define vec_ntn JPCRE2_SUFFIX(vec_ntn)
#define vec_nas JPCRE2_SUFFIX(vec_nas)

#define JPCRE2_LOCAL_CHAR c
#define TEXT "I am a simple\r\n text অ\r\n আ ক \nখ গ ঘ\n"
#define PAT "(?<name1>\\w+)(?<name2>\\s+)(?<name1>\\w+)"
FUNKY_CODE
#undef TEXT
#undef PAT
#undef JPCRE2_LOCAL_CHAR
    
#define JPCRE2_LOCAL_CHAR w
#define TEXT L"I am a simple\r\n text অ\r\n আ ক \nখ গ ঘ\n"
#define PAT L"(?<name1>\\w+)(?<name2>\\s+)(?<name1>\\w+)"
FUNKY_CODE
#undef TEXT
#undef PAT
#undef JPCRE2_LOCAL_CHAR

#if __cplusplus >= 201103L
#define JPCRE2_LOCAL_CHAR 16
#define TEXT u"I am a simple\r\n text অ\r\n আ ক \nখ গ ঘ\n"
#define PAT u"(?<name1>\\w+)(?<name2>\\s+)(?<name1>\\w+)"
FUNKY_CODE
#undef TEXT
#undef PAT
#undef JPCRE2_LOCAL_CHAR

#define JPCRE2_LOCAL_CHAR 32
#define TEXT U"I am a simple\r\n text অ\r\n আ ক \nখ গ ঘ\r\n"
#define PAT U"(*CRLF)(?<name1>\\w+)(?<name2>\\s+)(?<name1>\\w+)"
FUNKY_CODE
#undef TEXT
#undef PAT
#undef JPCRE2_LOCAL_CHAR
#endif

    
    
    return 0;
}
