/**@file testio.cpp
 * A test suit to check for code validity.
 * 
 * Goals:
 * 
 * 1. Check Validity of output for different inputs
 * 2. Memory check
 * 3. Segfault check
 * 4. coverage
 * 
 * This program will exit with 0 if all test passes, otherwise exit with
 * a non-zero status when error occurs.
 * 
 * @include testio.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */
#include <iostream>
#if __cplusplus >= 201103L
#define JPCRE2_USE_CHAR1632
#endif
#include "jpcre2.hpp"
#include <cassert>

typedef jpcre2::select<char> jpc;
typedef jpcre2::select<wchar_t> jpw;
#ifdef JPCRE2_USE_CHAR1632
typedef jpcre2::select<char16_t> jp16;
typedef jpcre2::select<char32_t> jp32;
#endif

int main(){
    ////////////////////// Check with std::string //////////////////////
    jpc::Regex rec;
    jpc::VecNum vec_numc;
    jpc::VecNas vec_nasc;
    jpc::VecNtN vec_ntnc;
    
    rec.setPattern("(?<digit>\\d)").compile();
    rec.getMatchObject()
       .setNumberedSubstringVector(&vec_numc)
       .setNamedSubstringVector(&vec_nasc)
       .setNameToNumberMapVector(&vec_ntnc)
       .setSubject("123456789")
       .setModifier("g");
    
    //check for validity of match
    assert(rec.match() == 9);
    assert(vec_numc[8][0] == "9");
    assert(vec_numc[8][1] == "9");
    assert(vec_nasc[8]["digit"] == "9");
    assert(vec_ntnc[8]["digit"] == 1);
    
    //check for validity of replace
    assert(rec.replace("123456789", "d$1") == "d123456789");
    assert(rec.replace("123456789", "d$1", "g") == "d1d2d3d4d5d6d7d8d9");
    
    ////////////////////////////////////////////////////////////////////
    
    //////////////////////// Check with std::wstring ///////////////////
    jpw::Regex rew;
    jpw::VecNum vec_numw;
    jpw::VecNas vec_nasw;
    jpw::VecNtN vec_ntnw;
    
    rew.setPattern(L"(?<digit>\\d)").compile();
    rew.getMatchObject()
       .setNumberedSubstringVector(&vec_numw)
       .setNamedSubstringVector(&vec_nasw)
       .setNameToNumberMapVector(&vec_ntnw)
       .setSubject(L"123456789")
       .setModifier("g");
    
    //check for validity of match
    assert(rew.match() == 9);
    assert(vec_numw[8][0] == L"9");
    assert(vec_numw[8][1] == L"9");
    assert(vec_nasw[8][L"digit"] == L"9");
    assert(vec_ntnw[8][L"digit"] == 1);
    
    //check for validity of replace
    assert(rew.replace(L"123456789", L"d$1") == L"d123456789");
    assert(rew.replace(L"123456789", L"d$1", "g") == L"d1d2d3d4d5d6d7d8d9");
    ////////////////////////////////////////////////////////////////////
    
    #ifdef JPCRE2_USE_CHAR1632
    //////////////////////// Check with std::u16string ///////////////////
    jp16::Regex re16;
    jp16::VecNum vec_num16;
    jp16::VecNas vec_nas16;
    jp16::VecNtN vec_ntn16;
    
    re16.setPattern(u"(?<digit>\\d)").compile();
    re16.getMatchObject()
        .setNumberedSubstringVector(&vec_num16)
        .setNamedSubstringVector(&vec_nas16)
        .setNameToNumberMapVector(&vec_ntn16)
        .setSubject(u"123456789")
        .setModifier("g");
    
    //check for validity of match
    assert(re16.match() == 9);
    assert(vec_num16[8][0] == u"9");
    assert(vec_num16[8][1] == u"9");
    assert(vec_nas16[8][u"digit"] == u"9");
    assert(vec_ntn16[8][u"digit"] == 1);
    
    //check for validity of replace
    assert(re16.replace(u"123456789", u"d$1") == u"d123456789");
    assert(re16.replace(u"123456789", u"d$1", "g") == u"d1d2d3d4d5d6d7d8d9");
    ////////////////////////////////////////////////////////////////////
    
    //////////////////////// Check with std::u32string ///////////////////
    jp32::Regex re32;
    jp32::VecNum vec_num32;
    jp32::VecNas vec_nas32;
    jp32::VecNtN vec_ntn32;
    
    re32.setPattern(U"(?<digit>\\d)").compile();
    re32.getMatchObject()
        .setNumberedSubstringVector(&vec_num32)
        .setNamedSubstringVector(&vec_nas32)
        .setNameToNumberMapVector(&vec_ntn32)
        .setSubject(U"123456789")
        .setModifier("g");
    
    //check for validity of match
    assert(re32.match() == 9);
    assert(vec_num32[8][0] == U"9");
    assert(vec_num32[8][1] == U"9");
    assert(vec_nas32[8][U"digit"] == U"9");
    assert(vec_ntn32[8][U"digit"] == 1);
    
    //check for validity of replace
    assert(re32.replace(U"123456789", U"d$1") == U"d123456789");
    assert(re32.replace(U"123456789", U"d$1", "g") == U"d1d2d3d4d5d6d7d8d9");
    ////////////////////////////////////////////////////////////////////
    #endif
    
    return 0;
}
