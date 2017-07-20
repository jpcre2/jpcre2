/**@file testmd.cpp
 * 
 * Examples of working with modifier table.
 * 
 * @include testmd.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */
//~ #define NDEBUG
#include <iostream>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;
typedef jp::String String;


int main(){
    
            /* ***************************
             * Compile modifier table 
             * ***************************/
             
    //character table is either std::string or const char* (not jp::String)
    std::string nametab = "IJMS"; //arbitrary modifier characters.
    //now the option values sequentially
    jpcre2::Uint valtab[] = { PCRE2_CASELESS, PCRE2_DUPNAMES, PCRE2_MULTILINE, jpcre2::JIT_COMPILE };
    //if the above two doesn't have the same number of elements, the behavior is undefined.
    
    //init ModifierTable
    jpcre2::ModifierTable mdt; //creates empty table.
    
    //change the Compile modifier table only:
    mdt.setCompileModifierTable(nametab, valtab);
    
    //now bind the table with the object
    jp::Regex re;
    re.setModifierTable(&mdt);
    
    //let's perform a compile
    re.compile("JPCRE2","I");
    
    //let's see it in action with a match
    jp::RegexMatch rm(&re);
    
    std::cout<<rm.setSubject("jpcre2").match(); //capital I is now working as caseless modifier.
    //and the small i is now an invalid modifier :
    re.compile("JPCRE2", "i");
    std::cout<<rm.match(); //doesn't match, small i is not a valid modifier now.
    std::cout<<"\n"<<re.getErrorMessage();
    
    
    
            /* ***************************
             * Match modifier table 
             * ***************************/
    
    
    std::string namtabm = "Ga";
    uint32_t valtabm[]= {jpcre2::FIND_ALL, PCRE2_ANCHORED};
    
    mdt.setMatchModifierTable(namtabm, valtabm);
    
    rm.setModifierTable(&mdt);
    
    rm.setModifier("g"); //modifier error, it's capital G now.
    std::cout<<"\n"<<rm.getErrorMessage();
    
    std::cout<<"\nmatched: "<<
    jp::Regex("\\w").setModifierTable(&mdt).match("jpcre2", "G");
    
    
    
            /* ***************************
             * Replace modifier table 
             * ***************************/
            
    
    
    std::string nametabr = "Ge";
    uint32_t valtabr[] = {PCRE2_SUBSTITUTE_GLOBAL, PCRE2_SUBSTITUTE_UNSET_EMPTY};
    mdt.setReplaceModifierTable(nametabr, valtabr);
    
    jp::RegexReplace rr(&re);
    
    rr.setModifierTable(&mdt);
    re.compile("\\d");
    std::cout<<"\nreplace: "<<
    rr.setSubject("12345").setReplaceWith("($0)").setModifier("G").replace();
    
    std::cout<<"\nmod: "<<rr.getModifier(); //G modifier was passed only.
    
    
    /* *****************************************************************
     * Note:
     *   Even though it may seem easy to pass the tables as arrays,
     *   it's kinda unsafe for the loophole that makes it an undefined
     *   behavior when the arrays are not of the same size. Thus passing
     *   them as vectors is preferable. If vectors are not of the same
     *   size you will get clear assertion error and be able to look for
     *   the problem and fix it for good.
     * ****************************************************************/
    
    std::string nametabrs = "Ge";
    #if __cpluscplus >= 201103L
    std::vector<uint32_t> valtabs = {PCRE2_SUBSTITUTE_GLOBAL, PCRE2_SUBSTITUTE_UNSET_EMPTY};
    #else
    std::vector<uint32_t> valtabs;
    valtabs.push_back(PCRE2_SUBSTITUTE_GLOBAL);
    valtabs.push_back(PCRE2_SUBSTITUTE_UNSET_EMPTY);
    #endif
    
    mdt.setReplaceModifierTable(nametabrs, valtabs);
    std::cout<<"\nreplace: "<<
    rr.setSubject("12345").setReplaceWith("($0)").setModifier("G").replace();
    
    
    
    /* *****************************************************************
     *              Some random sanity checks
     * ****************************************************************/
    
    
    mdt.clear().reset();
    mdt.setAllToDefault();
    mdt.setMatchModifierTable(nametabrs, valtabs);
    mdt.setCompileModifierTable(nametabrs, valtabs);
    
    jpcre2::ModifierTable mdt1;
    
    mdt1 = mdt;
    mdt = mdt1;
    mdt1 = mdt;
    
    jpcre2::ModifierTable mdt2(mdt1);
    
    jpcre2::ModifierTable mdt3(true); //create default table.
    
    re.getModifier();
    rm.getModifier();
    
    mdt.setMatchModifierTable(0,0); //clear
    mdt.setReplaceModifierTable(0,0); //clear
    mdt.setCompileModifierTable(0,0);
    mdt.setCompileModifierTable(&nametabrs[0],0);
    mdt.setCompileModifierTable(&nametabrs[0],&valtabs[0]);
    mdt.setMatchModifierTable(&nametabrs[0],0);
    mdt.setMatchModifierTable(&nametabrs[0],&valtabs[0]);
    mdt.setReplaceModifierTable(&nametabrs[0],0);
    mdt.setReplaceModifierTable(&nametabrs[0],&valtabs[0]);
    mdt.setMatchModifierTable(nametabrs,0);
    mdt.setReplaceModifierTable(nametabrs,0);
    mdt.setCompileModifierTable(nametabrs,0);
    
    mdt = rm.getModifierTable();
    mdt = re.getModifierTable();
    mdt = rr.getModifierTable();
    
    
    
    
    
    return 0;
}
