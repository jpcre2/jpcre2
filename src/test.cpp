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
#include <cassert>
#include "jpcre2.hpp"


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
     \
    jp::Regex re2(re); /*//check on copy constructor*/ \
     \
    re2 = jp::Regex(re); \
     \
    std::setlocale(LC_CTYPE, "en_US"); \
    re2.resetCharacterTables().compile(); \
     \
    jp::Regex re3(re2); \
    re = jp::Regex(re3); \
    /*//check boolean operator*/ \
    RE_TEST \
     \
    re.resetErrors(); \
    re.reset(); \
    re.addModifier("eijmnsuxADJUS"); \
    re.addPcre2Option(PCRE2_ANCHORED); \
    re.addJpcre2Option(jpcre2::JIT_COMPILE); \
    std::setlocale(LC_CTYPE, "fr_FR"); \
    re.resetCharacterTables().compile(); \
    re.resetCharacterTables().compile(); \
    re.resetCharacterTables().compile(); \
    re.getErrorMessage(); \
    re.getErrorNumber(); \
    re.getErrorOffset(); \
    re.getPattern(); \
    re.getPatternPointer(); \
    re.getPcre2Option(); \
    re.getJpcre2Option(); \
    re.getModifier(); \
     \
    re.resetErrors(); \
    re.reset().clear(); \
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
    re.reset().resetErrors(); \
    re.reset().resetErrors(); \
     \
    re.match(PAT); \
    re.match(PAT, 0); \
    re.match(PAT, "g"); \
    re.match(PAT, "g", 2); \
    re.match(&text); \
    re.match(&text, 21); \
    re.match(&text, "g"); \
    re.match(&text, "g", 22); \
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
    rm2.reset().clear(); \
    rm.resetErrors(); \
     \
    rm.setNumberedSubstringVector(&vec_num); \
    rm.setNamedSubstringVector(&vec_nas).setNameToNumberMapVector(&vec_ntn); \
    rm.setNumberedSubstringVector(vec_num); \
    rm.setNamedSubstringVector(vec_nas).setNameToNumberMapVector(vec_ntn); \
    jpcre2::VecOff vec_soff; \
    jpcre2::VecOff vec_eoff; \
    rm.setMatchStartOffsetVector(&vec_soff); \
    rm.setMatchEndOffsetVector(&vec_eoff); \
    rm.setMatchStartOffsetVector(vec_soff); \
    rm.setMatchEndOffsetVector(vec_eoff); \
    re = jp::Regex(PAT, "in"); \
    rm.setRegexObject(&re); \
    rm.setMatchContext(0); \
    rm.setSubject(jp::String()).setSubject(&text).setSubject(text).setModifier("g").match(); \
    jp::Regex re4(PAT, "niJS"); \
    rm.setRegexObject(&re4); \
    size_t count = rm.setSubject(&text).setModifier("g").setStartOffset(0).match(); \
    if(count);\
    count = rm.setSubject(&text).setModifier("gA").match(); \
    count = rm.setSubject(&text).setModifier("A").match(); \
     \
     \
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
    rm.getNumberedSubstringVector(); \
    rm.getNamedSubstringVector(); \
    rm.getNameToNumberMapVector(); \
    rm.changePcre2Option(PCRE2_ANCHORED, true); \
    rm.changePcre2Option(PCRE2_ANCHORED, false); \
    rm.addModifier("g"); \
    rm.addJpcre2Option(jpcre2::FIND_ALL); \
    rm.changeJpcre2Option(jpcre2::FIND_ALL,false); \
    rm.changeJpcre2Option(jpcre2::FIND_ALL,true); \
    rm.setFindAll(); \
    rm.setFindAll(false); \
    rm.changeModifier("gAgfdsf", false); \
    jp::getErrorMessage(jpcre2::ERROR::INSUFFICIENT_OVECTOR, 0); \
    rm.getMatchEndOffsetVector();\
    rm.getMatchStartOffsetVector();\
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
     \
    rr = rr2; \
    rr3 = jp::RegexReplace(&re2); \
     \
    re.replace(TEXT, TEXT); \
    re.replace(TEXT, &text); \
    re.replace(TEXT, TEXT, "g"); \
    re.replace(TEXT, &text, "g"); \
    re.replace(&text, TEXT); \
    re.replace(&text, TEXT, "g"); \
    re.replace(&text, &text); \
    re.replace(&text, &text, "g"); \
     \
    rr.resetErrors(); \
    rr.reset().clear(); \
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
    rr.setMatchContext(0); \
    rr.setMatchDataBlock(0); \
    rr.changeJpcre2Option(0, true); \
    rr.changeJpcre2Option(0, false); \
    rr.changePcre2Option(0, true); \
    rr.changePcre2Option(0, false); \
    rr.changeModifier("gfdsf", false); \
     \
    rr.reset().setSubject(TEXT).setReplaceWith(TEXT).replace(); \
    rr.changePcre2Option(PCRE2_SUBSTITUTE_OVERFLOW_LENGTH, false); \
    rr.setRegexObject(&re); \
    rr.setSubject(TEXT).setReplaceWith(TEXT).replace(); /* replace error: */ \
     \
    const jp::Regex *rep = rr.getRegexObject(); \
    if(rep); /*//rep is not null*/ \
     \
     \
     \
    /*//checking the string converter with null input*/ \
    assert(jp::toString((jp::Char)0) == jp::String()); \
    assert(jp::toString((jp::Char*)0) == jp::String()); \
    assert(jp::toString((jp::Pcre2Uchar*)0) == jp::String());\
    jp::MatchEvaluator me;\
    assert(me.match()==0);\
    assert(me.nreplace()==jp::String()); \
    me.setRegexObject(0); \
    me.getErrorMessage(); \
    me.getErrorNumber(); \
    me.getErrorOffset(); \
    me.getPcre2Option(); \
    me.getJpcre2Option(); \
    me.getModifier(); \
    me.getStartOffset(); \
    me.getSubject(); \
    me.getSubjectPointer(); \
    me.getRegexObject(); \
    me.setJpcre2Option(0); \
    me.setPcre2Option(0); \
    me.changeJpcre2Option(0, true); \
    me.changeJpcre2Option(0, false); \
    me.changePcre2Option(0, true); \
    me.changePcre2Option(0, false); \
     \
    me.addPcre2Option(0); \
    me.getNumberedSubstringVector(); \
    me.getNamedSubstringVector(); \
    me.getNameToNumberMapVector(); \
    me.changePcre2Option(PCRE2_ANCHORED, true); \
    me.changePcre2Option(PCRE2_ANCHORED, false); \
    me.addModifier("g"); \
    me.addJpcre2Option(jpcre2::FIND_ALL); \
    me.changeJpcre2Option(jpcre2::FIND_ALL,false); \
    me.changeJpcre2Option(jpcre2::FIND_ALL,true); \
    me.setFindAll(); \
    me.setFindAll(false); \
    me.changeModifier("gAgfdsf", false); \
    jp::getErrorMessage(jpcre2::ERROR::INSUFFICIENT_OVECTOR, 0); \
    me.getMatchEndOffsetVector();\
    me.getMatchStartOffsetVector();\
    
    
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
#define vec_soff JPCRE2_SUFFIX(vec_soff)
#define vec_eoff JPCRE2_SUFFIX(vec_eoff)
#define me JPCRE2_SUFFIX(me)

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
