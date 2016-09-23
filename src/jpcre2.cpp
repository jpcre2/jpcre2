/* *****************************************************************************
 * ******************* C++ wrapper for PCRE2 Library ****************************
 * *****************************************************************************
 *            Copyright (c) 2015-2016 Md. Jahidul Hamid
 * 
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 *     * The names of its contributors may be used to endorse or promote 
 *       products derived from this software without specific prior written
 *       permission.
 *       
 * Disclaimer:
 * 
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *     IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *     ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *     CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *     SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *     INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *     ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 * */

/**
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include "jpcre2.hpp"

#include <cstdio>   // snprintf
#include <limits>   // std::numeric_limits
#include <cwchar>   // wcslen, std::mbstate_t
#include <cstring>  // strlen
#include <clocale>  // std::setlocale

#if PCRE2_CODE_UNIT_WIDTH != 8 && PCRE2_CODE_UNIT_WIDTH != 16 && PCRE2_CODE_UNIT_WIDTH !=32
#error This cpp file needs PCRE2_CODE_UNIT_WIDTH to be defined either 8 or 16 or 32. if you want 0, link with libraries instead.
#endif

#define JPCRE2_JOIN(a,b) a ## b
#define JPCRE2_GLUE(a,b) JPCRE2_JOIN(a,b)
#define JPCRE2_SUFFIX(a) JPCRE2_GLUE(a,PCRE2_CODE_UNIT_WIDTH)
#define select JPCRE2_SUFFIX(select)


const std::string jpcre2::INFO::NAME("JPCRE2");
const std::string jpcre2::INFO::FULL_VERSION("10.27.01");
const std::string jpcre2::INFO::VERSION_GENRE("10");
const std::string jpcre2::INFO::VERSION_MAJOR("27");
const std::string jpcre2::INFO::VERSION_MINOR("01");
const std::string jpcre2::INFO::VERSION_PRE_RELEASE("");


const std::string jpcre2::LOCALE_NONE = "JPCRE2_NONE";                           ///< Nothing to be done on locale
const std::string jpcre2::LOCALE_DEFAULT = LOCALE_NONE;                          ///< Default local to be used
/// Max of int is used as the initial size of replaced string by default
const jpcre2::SIZE_T jpcre2::SUBSTITUTE_RESULT_INIT_SIZE = std::numeric_limits<int>::max();


template<>
const typename jpcre2::select<char>::String jpcre2::select<char>::JPCRE2_EMPTY_STRING("");
template<>
const typename jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::JPCRE2_EMPTY_STRING(L"");
#if __cplusplus >= 201103L
template<>
const typename jpcre2::select<char16_t>::String jpcre2::select<char16_t>::JPCRE2_EMPTY_STRING(u"");
template<>
const typename jpcre2::select<char32_t>::String jpcre2::select<char32_t>::JPCRE2_EMPTY_STRING(U"");
#endif

// Define modifiers for compile
// Every modifier needs to be unique in this block
const std::string jpcre2::MOD::C_N("eijmnsuxADJU");
const jpcre2::Uint jpcre2::MOD::C_V[12] = { PCRE2_MATCH_UNSET_BACKREF,                  // Modifier e
                                            PCRE2_CASELESS,                             // Modifier i
                                            PCRE2_ALT_BSUX | PCRE2_MATCH_UNSET_BACKREF, // Modifier j
                                            PCRE2_MULTILINE,                            // Modifier m
                                            PCRE2_UTF | PCRE2_UCP,                      // Modifier n (includes u)
                                            PCRE2_DOTALL,                               // Modifier s
                                            PCRE2_UTF,                                  // Modifier u
                                            PCRE2_EXTENDED,                             // Modifier x
                                            PCRE2_ANCHORED,                             // Modifier A
                                            PCRE2_DOLLAR_ENDONLY,                       // Modifier D
                                            PCRE2_DUPNAMES,                             // Modifier J
                                            PCRE2_UNGREEDY                              // Modifier U
                                          };
                                          

const std::string jpcre2::MOD::CJ_N("S");
const jpcre2::Uint jpcre2::MOD::CJ_V[1] = { JIT_COMPILE,                                // Modifier S
                                          };


// Define modifiers for replace
// Every modifier needs to be unique in this block
const std::string jpcre2::MOD::R_N("eEgx");
const jpcre2::Uint jpcre2::MOD::R_V[4] = { PCRE2_SUBSTITUTE_UNSET_EMPTY,                // Modifier  e
                                           PCRE2_SUBSTITUTE_UNKNOWN_UNSET | PCRE2_SUBSTITUTE_UNSET_EMPTY,   // Modifier E (includes e)
                                           PCRE2_SUBSTITUTE_GLOBAL,                     // Modifier g
                                           PCRE2_SUBSTITUTE_EXTENDED                    // Modifier x
                                         };


const std::string jpcre2::MOD::RJ_N("");
const jpcre2::Uint jpcre2::MOD::RJ_V[1] = { NONE
                                          };
//Explicit

// Define modifiers for match
// Every modifier needs to be unique in this block

const std::string jpcre2::MOD::M_N("A");
const jpcre2::Uint jpcre2::MOD::M_V[1] = { PCRE2_ANCHORED                               // Modifier  A
                                         };


const std::string jpcre2::MOD::MJ_N("g");
const jpcre2::Uint jpcre2::MOD::MJ_V[1] = { FIND_ALL,                                   // Modifier  g
                                          };



template<class Char_T>
typename jpcre2::select<Char_T>::String jpcre2::select<Char_T>::toString(const Char_T* a) {
	if (a)
		return String(a);
	else
		return JPCRE2_EMPTY_STRING;
}
//Explicit instantiation
template jpcre2::select<char>::String jpcre2::select<char>::toString(const char* a);
template jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::toString(const wchar_t* a);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::String jpcre2::select<char16_t>::toString(const char16_t* a);
template jpcre2::select<char32_t>::String jpcre2::select<char32_t>::toString(const char32_t* a);
#endif


template<class Char_T>
typename jpcre2::select<Char_T>::String jpcre2::select<Char_T>::toString(Char_T a) {
	if (a)
		return String(1, a);
	else
		return JPCRE2_EMPTY_STRING;
}
//Explicit inst...
template jpcre2::select<char>::String jpcre2::select<char>::toString(char a);
template jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::toString(wchar_t a);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::String jpcre2::select<char16_t>::toString(char16_t a);
template jpcre2::select<char32_t>::String jpcre2::select<char32_t>::toString(char32_t a);
#endif

namespace jpcre2{
template<>
void select<char>::Sprintf(char * buf, int length, int x) {
    snprintf(buf, length, "%d", x);
}
template<>
void select<wchar_t>::Sprintf(wchar_t * buf, int length, int x) {
    swprintf(buf, length + 1, L"%d", x);
}
/*
template<>
SIZE_T jpcre2::select<char>::Strlen(const char* a){
    return strlen(a);
}

template<>
SIZE_T jpcre2::select<wchar_t>::Strlen(const wchar_t* a){
    return wcslen(a);
}
*/


template<class Char_T>
typename jpcre2::select<Char_T>::String jpcre2::select<Char_T>::toString(int x) {
	int length = snprintf(0, 0, "%d", x);
	assert(length >= 0);
	Char* buf = new Char[length + 1];
    Sprintf(buf, length +1, x);
	String str(buf);
	delete[] buf;
	return str;
}
//Explicit inst..
template jpcre2::select<char>::String jpcre2::select<char>::toString(int x);
template jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::toString(int x);

#if __cplusplus >= 201103L
template<>
typename jpcre2::select<char16_t>::String jpcre2::select<char16_t>::toString(int x) {
	std::string s = std::to_string(x);
    std::u16string us = convert16.from_bytes(s);
    return us;
}
template<>
typename jpcre2::select<char32_t>::String jpcre2::select<char32_t>::toString(int x) {
	std::string s = std::to_string(x);
    std::u32string us = convert32.from_bytes(s);
    return us;
}
#endif

}


template<class Char_T>
typename jpcre2::select<Char_T>::String jpcre2::select<Char_T>::toString(PCRE2_UCHAR* a) {
	if (a)
		return String((Char*) a);
	else
		return JPCRE2_EMPTY_STRING;
}
//Explicit
template jpcre2::select<char>::String jpcre2::select<char>::toString(PCRE2_UCHAR* a);
template jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::toString(PCRE2_UCHAR* a);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::String jpcre2::select<char16_t>::toString(PCRE2_UCHAR* a);
template jpcre2::select<char32_t>::String jpcre2::select<char32_t>::toString(PCRE2_UCHAR* a);
#endif



template<class Char_T>
typename jpcre2::select<Char_T>::String jpcre2::select<Char_T>::getPcre2ErrorMessage(int err_num) {
	PCRE2_UCHAR buffer[8048];
	pcre2_get_error_message(err_num, buffer, sizeof(buffer));
	return toString((PCRE2_UCHAR*) buffer);
}
//Explicit inst...
template jpcre2::select<char>::String jpcre2::select<char>::getPcre2ErrorMessage(int err_num);
template jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::getPcre2ErrorMessage(int err_num);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::String jpcre2::select<char16_t>::getPcre2ErrorMessage(int err_num);
template jpcre2::select<char32_t>::String jpcre2::select<char32_t>::getPcre2ErrorMessage(int err_num);
#endif


template<class Char_T>
typename jpcre2::select<Char_T>::String jpcre2::select<Char_T>::getErrorMessage(int err_num, int err_off) {
	if (err_num == (int)ERROR::JIT_COMPILE_FAILED) {
		return getPcre2ErrorMessage((int) err_off);
	} else if(err_num != 0) {
		return getPcre2ErrorMessage((int) err_num) + toString((int) err_off);
	}
    else return JPCRE2_EMPTY_STRING;
}
//Explicit inst...
template jpcre2::select<char>::String jpcre2::select<char>::getErrorMessage(int err_num, int err_off);
template jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::getErrorMessage(int err_num, int err_off);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::String jpcre2::select<char16_t>::getErrorMessage(int err_num, int err_off);
template jpcre2::select<char32_t>::String jpcre2::select<char32_t>::getErrorMessage(int err_num, int err_off);
#endif


template<class Char_T>
std::string jpcre2::select<Char_T>::Regex::getModifier(){
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < MOD::C_N.length(); ++i){
        if( (MOD::C_V[i] & compile_opts) != 0 && 
            (MOD::C_V[i] & compile_opts) == MOD::C_V[i]) //One option can include other
            temp += MOD::C_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < MOD::CJ_N.length(); ++i){
        if( (MOD::CJ_V[i] & jpcre2_compile_opts) != 0 && 
            (MOD::CJ_V[i] & jpcre2_compile_opts) == MOD::CJ_V[i]) //One option can include other
            temp += MOD::CJ_N[i];
    }
    return temp;
}
//Explicit inst ...
template std::string jpcre2::select<char>::Regex::getModifier();
template std::string jpcre2::select<wchar_t>::Regex::getModifier();
#if __cplusplus >= 201103L
template std::string jpcre2::select<char16_t>::Regex::getModifier();
template std::string jpcre2::select<char32_t>::Regex::getModifier();
#endif



template<class Char_T>
std::string jpcre2::select<Char_T>::RegexMatch::getModifier(){
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < MOD::M_N.length(); ++i){
        if( (MOD::M_V[i] & match_opts) != 0 && 
            (MOD::M_V[i] & match_opts) == MOD::M_V[i]) //One option can include other
            temp += MOD::M_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < MOD::MJ_N.length(); ++i){
        if( (MOD::MJ_V[i] & jpcre2_match_opts) != 0 && 
            (MOD::MJ_V[i] & jpcre2_match_opts) == MOD::MJ_V[i]) //One option can include other
            temp += MOD::MJ_N[i];
    }
    return temp;
}
//Explicit inst ...
template std::string jpcre2::select<char>::RegexMatch::getModifier();
template std::string jpcre2::select<wchar_t>::RegexMatch::getModifier();
#if __cplusplus >= 201103L
template std::string jpcre2::select<char16_t>::RegexMatch::getModifier();
template std::string jpcre2::select<char32_t>::RegexMatch::getModifier();
#endif


template<class Char_T>
std::string jpcre2::select<Char_T>::RegexReplace::getModifier(){
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < MOD::R_N.length(); ++i){
        if( (MOD::R_V[i] & replace_opts) != 0 &&
            (MOD::R_V[i] & replace_opts) == MOD::R_V[i]) //One option can include other
            temp += MOD::R_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < MOD::RJ_N.length(); ++i){
        if( (MOD::RJ_V[i] & jpcre2_replace_opts) != 0 &&
            (MOD::RJ_V[i] & jpcre2_replace_opts) == MOD::RJ_V[i]) //One option can include other
            temp += MOD::RJ_N[i];
    }
    return temp;
}
//Explicit
template std::string jpcre2::select<char>::RegexReplace::getModifier();
template std::string jpcre2::select<wchar_t>::RegexReplace::getModifier();
#if __cplusplus >= 201103L
template std::string jpcre2::select<char16_t>::RegexReplace::getModifier();
template std::string jpcre2::select<char32_t>::RegexReplace::getModifier();
#endif

/////////

// Regex class




template<class Char_T>
void jpcre2::select<Char_T>::Regex::deepCopy(const Regex& r) {
	//Copy #code if it is non-null
	if (r.code) {
        ///First release memory of #code from current object if it is non-NULL
		freeRegexMemory();
		/// Copy compiled memory of #code to #code of current object using pcre2_code_copy() 
		code = pcre2_code_copy(r.code);
		/// Perform JIT compilation (if enabled) as pcre2_code_copy() doesn't copy JIT memory
		if ((jpcre2_compile_opts & JIT_COMPILE) != 0) {
			//Perform JIT compilation:
			int jit_ret = pcre2_jit_compile(code, PCRE2_JIT_COMPLETE);
			if (jit_ret != 0) {
				error_number = (int)ERROR::JIT_COMPILE_FAILED;
                error_offset = jit_ret;
                //JIT error should be ignored.
			}
		}
	} else
		code = 0;
	///Don't copy #rm, just set it to NULL
	//and delete the other one
	delete rm;
	rm = 0;
	delete r.rm;
	///Don't copy #rr, just set it to NULL
	//and delete the other one
	delete rr;
	rr = 0;
	delete r.rr;
}
//Explicit
template void jpcre2::select<char>::Regex::deepCopy(const Regex& r);
template void jpcre2::select<wchar_t>::Regex::deepCopy(const Regex& r);
#if __cplusplus >= 201103L
template void jpcre2::select<char16_t>::Regex::deepCopy(const Regex& r);
template void jpcre2::select<char32_t>::Regex::deepCopy(const Regex& r);
#endif


template<class Char_T>
typename jpcre2::select<Char_T>::Regex& jpcre2::select<Char_T>::Regex::changeModifier(const std::string& mod, bool x) {
	//loop through mod
	for (SIZE_T i = 0; i < mod.length(); ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < MOD::CJ_N.length(); ++j){
            if(MOD::CJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::CJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< MOD::C_N.length(); ++j){
            if(MOD::C_N[j] == mod[i]){
                changePcre2Option(MOD::C_V[j], x);
                goto endfor;
            }
        }
        
        //Modifier didn't match, invalid modifier
        #ifdef JPCRE2_ASSERT_INVALID_MODIFIER
        assert(std::string("Invalid modifier") == std::string(1,mod[i]));
        #endif
        
        endfor:;
	}
    return *this;
}
//Explicit
template jpcre2::select<char>::Regex& jpcre2::select<char>::Regex::changeModifier(const std::string& mod, bool x);
template jpcre2::select<wchar_t>::Regex& jpcre2::select<wchar_t>::Regex::changeModifier(const std::string& mod, bool x);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::Regex& jpcre2::select<char16_t>::Regex::changeModifier(const std::string& mod, bool x);
template jpcre2::select<char32_t>::Regex& jpcre2::select<char32_t>::Regex::changeModifier(const std::string& mod, bool x);
#endif


template<class Char_T>
void jpcre2::select<Char_T>::Regex::compile() {
	//Get c_str of pattern
	PCRE2_SPTR c_pattern = (PCRE2_SPTR) pat_str.c_str();

	/**************************************************************************
	 * Compile the regular expression pattern, and handle 
	 * any errors that are detected.                                          
	 *************************************************************************/

	pcre2_compile_context *ccontext = pcre2_compile_context_create(0);

	if (mylocale != LOCALE_NONE) {
		std::string loc_old;
		loc_old = std::setlocale(LC_CTYPE, mylocale.c_str());//mylocal needs to be std::string
		const unsigned char *tables = pcre2_maketables(0);
		pcre2_set_character_tables(ccontext, tables);
		std::setlocale(LC_CTYPE, (char*)loc_old.c_str());
	}

	code = pcre2_compile(c_pattern,     /* the pattern */
                PCRE2_ZERO_TERMINATED,  /* indicates pattern is zero-terminated */
                compile_opts,           /* default options */
                &error_number,          /* for error number */
                &error_offset,          /* for error offset */
                ccontext);              /* use compile context */

	if (code == 0) {
		/* Compilation failed */
		//must not free regex memory, the only function has that right is the destructor
        return;
	} else if ((jpcre2_compile_opts & JIT_COMPILE) != 0) {
		///perform JIT compilation it it's enabled
		int jit_ret = pcre2_jit_compile(code, PCRE2_JIT_COMPLETE);
		if (jit_ret != 0) {
            error_number = (int)ERROR::JIT_COMPILE_FAILED;
            error_offset = jit_ret;
            //Do not return, JIT error should be ignored.
		}
	}
    error_number = error_offset = 0;
}
//Explicit
template void jpcre2::select<char>::Regex::compile();
template void jpcre2::select<wchar_t>::Regex::compile();
#if __cplusplus >= 201103L
template void jpcre2::select<char16_t>::Regex::compile();
template void jpcre2::select<char32_t>::Regex::compile();
#endif


//////////////////


// RegexReplace class



template<class Char_T>
typename jpcre2::select<Char_T>::RegexReplace& jpcre2::select<Char_T>::RegexReplace::resetErrors() {
    re->error_number = 0;
    re->error_offset = 0;
    return *this;
}
//Explicit
template jpcre2::select<char>::RegexReplace& jpcre2::select<char>::RegexReplace::resetErrors();
template jpcre2::select<wchar_t>::RegexReplace& jpcre2::select<wchar_t>::RegexReplace::resetErrors();
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::RegexReplace& jpcre2::select<char16_t>::RegexReplace::resetErrors();
template jpcre2::select<char32_t>::RegexReplace& jpcre2::select<char32_t>::RegexReplace::resetErrors();
#endif


template<class Char_T>
typename jpcre2::select<Char_T>::RegexReplace& jpcre2::select<Char_T>::RegexReplace::changeModifier(const std::string& mod, bool x) {
	//loop through mod
	for (SIZE_T i = 0; i < mod.length(); ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < MOD::RJ_N.length(); ++j){
            if(MOD::RJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::RJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< MOD::R_N.length(); ++j){
            if(MOD::R_N[j] == mod[i]){
                changePcre2Option(MOD::R_V[j], x);
                goto endfor;
            }
        }
        
        //Modifier didn't match, invalid modifier
        ///If this macro is defined, invalid modifier will terminate the program immediately with a failed assertion
        #ifdef JPCRE2_ASSERT_INVALID_MODIFIER
        assert(std::string("Invalid modifier") == std::string(1,mod[i]));
        #endif
        
        endfor:;
	}
    return *this;
}
//Explicit
template jpcre2::select<char>::RegexReplace& jpcre2::select<char>::RegexReplace::changeModifier(const std::string& mod, bool x);
template jpcre2::select<wchar_t>::RegexReplace& jpcre2::select<wchar_t>::RegexReplace::changeModifier(const std::string& mod, bool x);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::RegexReplace& jpcre2::select<char16_t>::RegexReplace::changeModifier(const std::string& mod, bool x);
template jpcre2::select<char32_t>::RegexReplace& jpcre2::select<char32_t>::RegexReplace::changeModifier(const std::string& mod, bool x);
#endif


template<class Char_T>
typename jpcre2::select<Char_T>::String jpcre2::select<Char_T>::RegexReplace::replace() {

	// If code is null, return the subject string unmodified.
	if (re->code == 0)
		return r_subject;

	PCRE2_SPTR subject = (PCRE2_SPTR) r_subject.c_str();
	PCRE2_SIZE subject_length = r_subject.length() /* Strlen((Char *) subject) */;
	PCRE2_SPTR replace = (PCRE2_SPTR) r_replw.c_str();
	PCRE2_SIZE replace_length = r_replw.length() /* Strlen((Char *) replace) */;
	PCRE2_SIZE outlengthptr = (PCRE2_SIZE) buffer_size;
	int ret = 0;
	bool retry = true;
	PCRE2_UCHAR* output_buffer;
	output_buffer = (PCRE2_UCHAR*) malloc(outlengthptr * sizeof(PCRE2_UCHAR));

	while (true) {
		ret = pcre2_substitute(re->code,    /*Points to the compiled pattern*/
                    subject,                /*Points to the subject string*/
                    subject_length,         /*Length of the subject string*/
                    0,                      /*Offset in the subject at which to start matching*/
                    replace_opts,           /*Option bits*/
                    0,                      /*Points to a match data block, or is NULL*/
                    0,                      /*Points to a match context, or is NULL*/
                    replace,                /*Points to the replacement string*/
                    replace_length,         /*Length of the replacement string*/
                    output_buffer,          /*Points to the output buffer*/
                    &outlengthptr           /*Points to the length of the output buffer*/
                    );

		if (ret < 0) {
			//Handle errors
			if ((replace_opts & PCRE2_SUBSTITUTE_OVERFLOW_LENGTH) != 0
					&& ret == (int) PCRE2_ERROR_NOMEMORY && retry) {
				retry = false;
				outlengthptr++;  // It was changed to required length
                /// If initial #buffer_size wasn't big enough for resultant string, 
                /// we will try once more with a new buffer size adjusted to the length of the resultant string.
				output_buffer = (PCRE2_UCHAR*) realloc(output_buffer,
						outlengthptr * sizeof(PCRE2_UCHAR));
				// Go and try to perform the substitute again
				continue;
			} else {
				::free(output_buffer);
				re->error_number = re->error_offset = ret;
                return r_subject;
			}
		}
		//If everything's ok exit the loop
		break;
	}
	String result = toString((Char*) output_buffer);
	::free(output_buffer);
	return result;
}
//Explicit
template jpcre2::select<char>::String jpcre2::select<char>::RegexReplace::replace();
template jpcre2::select<wchar_t>::String jpcre2::select<wchar_t>::RegexReplace::replace();
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::String jpcre2::select<char16_t>::RegexReplace::replace();
template jpcre2::select<char32_t>::String jpcre2::select<char32_t>::RegexReplace::replace();
#endif


/////////////////

// RegexMatch class




template<class Char_T>
typename jpcre2::select<Char_T>::RegexMatch& jpcre2::select<Char_T>::RegexMatch::resetErrors() {
    re->error_number = 0;
    re->error_offset = 0;
    return *this;
}
//Explicit
template jpcre2::select<char>::RegexMatch& jpcre2::select<char>::RegexMatch::resetErrors();
template jpcre2::select<wchar_t>::RegexMatch& jpcre2::select<wchar_t>::RegexMatch::resetErrors();
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::RegexMatch& jpcre2::select<char16_t>::RegexMatch::resetErrors();
template jpcre2::select<char32_t>::RegexMatch& jpcre2::select<char32_t>::RegexMatch::resetErrors();
#endif


template<class Char_T>
typename jpcre2::select<Char_T>::RegexMatch& jpcre2::select<Char_T>::RegexMatch::changeModifier(const std::string& mod, bool x) {
	//loop through mod
	for (SIZE_T i = 0; i < mod.length(); ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < MOD::MJ_N.length(); ++j){
            if(MOD::MJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::MJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< MOD::M_N.length(); ++j){
            if(MOD::M_N[j] == mod[i]){
                changePcre2Option(MOD::M_V[j], x);
                goto endfor;
            }
        }
        
        //Modifier didn't match, invalid modifier
        #ifdef JPCRE2_ASSERT_INVALID_MODIFIER
        assert(std::string("Invalid modifier") == std::string(1,mod[i]));
        #endif
        
        endfor:;
	}
    return *this;
}
//Explicit
template jpcre2::select<char>::RegexMatch& jpcre2::select<char>::RegexMatch::changeModifier(const std::string& mod, bool x);
template jpcre2::select<wchar_t>::RegexMatch& jpcre2::select<wchar_t>::RegexMatch::changeModifier(const std::string& mod, bool x);
#if __cplusplus >= 201103L
template jpcre2::select<char16_t>::RegexMatch& jpcre2::select<char16_t>::RegexMatch::changeModifier(const std::string& mod, bool x);
template jpcre2::select<char32_t>::RegexMatch& jpcre2::select<char32_t>::RegexMatch::changeModifier(const std::string& mod, bool x);
#endif


template<class Char_T>
bool jpcre2::select<Char_T>::RegexMatch::getNumberedSubstrings(int rc, pcre2_match_data *match_data) {
	for (int i = 0; i < rc; i++) {
		String value;
		//If we use pcre2_substring_get_bynumber(),
		//we will have to deal with returned error codes and memory
		PCRE2_UCHAR **bufferptr;
		PCRE2_SIZE bufflen;
		pcre2_substring_length_bynumber(match_data, (Uint) i, &bufflen);
		bufferptr = (PCRE2_UCHAR**) malloc(bufflen * sizeof(PCRE2_UCHAR));
		int ret = pcre2_substring_get_bynumber(match_data, (Uint) i, bufferptr,
				&bufflen);
		if (ret < 0) {
			switch (ret) {
			case PCRE2_ERROR_NOMEMORY:
				re->error_number = re->error_offset = ret;
                return false;
			default:
				break;   ///Errors other than PCRE2_ERROR_NOMEMORY error are ignored
			}
		}
		value = toString((Char*) *bufferptr);
		//pcre2_substring_free(*bufferptr);
		::free(bufferptr);                  //must free memory
		if (num_map0)
			(*num_map0)[i] = value; //This null check is paranoid, this function shouldn't be called if this map is null
	}
    return true;
}
//Explicit
template bool jpcre2::select<char>::RegexMatch::getNumberedSubstrings(int rc, pcre2_match_data *match_data);
template bool jpcre2::select<wchar_t>::RegexMatch::getNumberedSubstrings(int rc, pcre2_match_data *match_data);
#if __cplusplus >= 201103L
template bool jpcre2::select<char16_t>::RegexMatch::getNumberedSubstrings(int rc, pcre2_match_data *match_data);
template bool jpcre2::select<char32_t>::RegexMatch::getNumberedSubstrings(int rc, pcre2_match_data *match_data);
#endif


template<class Char_T>
bool jpcre2::select<Char_T>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size, PCRE2_SPTR tabptr, pcre2_match_data *match_data) {

	for (int i = 0; i < namecount; i++) {
		String key, value, value1;
        #if PCRE2_CODE_UNIT_WIDTH == 8
            int n = (int)((tabptr[0] << 8) | tabptr[1]);
            key = toString((Char*) (tabptr + 2));
        #elif PCRE2_CODE_UNIT_WIDTH == 16 || PCRE2_CODE_UNIT_WIDTH == 32
            int n = (int)tabptr[0];
            key = toString((Char*) (tabptr + 1));
        #endif
		PCRE2_UCHAR **bufferptr;
		PCRE2_SIZE bufflen;
		pcre2_substring_length_byname(match_data, (PCRE2_SPTR) key.c_str(),
				&bufflen);
		bufferptr = (PCRE2_UCHAR **) malloc(
				(bufflen + 1) * sizeof(PCRE2_UCHAR));
		int ret = pcre2_substring_get_byname(match_data,
				(PCRE2_SPTR) key.c_str(), bufferptr, &bufflen);
		if (ret < 0) {
			switch (ret) {
			case PCRE2_ERROR_NOMEMORY:
				re->error_number = re->error_offset = ret;
                return false;
			default:
				break;   ///Errors other than PCRE2_ERROR_NOMEMORY error are ignored
			}
		}
		value = toString((Char *) *bufferptr);

		//Let's get the value again, this time with number
		//We will match this value with the previous one.
		//If they match, we got the right one.
		//Otherwise the number is not valid for the corresponding name and
		//we will skip this iteration.
		//Don't use pcre2_substring_number_from_name() to get the number for the name (It's messy with dupnames).
		::free(bufferptr);
		pcre2_substring_length_bynumber(match_data, (Uint) n, &bufflen);
		bufferptr = (PCRE2_UCHAR **) malloc(bufflen * sizeof(PCRE2_UCHAR));
		ret = pcre2_substring_get_bynumber(match_data, (Uint) n, bufferptr,
				&bufflen);
		if (ret < 0) {
			switch (ret) {
			case PCRE2_ERROR_NOMEMORY:
				re->error_number = re->error_offset = ret;
                return false;
			default:
				break;   ///Errors other than PCRE2_ERROR_NOMEMORY error are ignored
			}
		}
		value1 = toString((Char *) *bufferptr);

		//pcre2_substring_free(*bufferptr);
		//Instead use free() to free the memory
		::free(bufferptr);                  //must free memory
		if (value != value1) {
			tabptr += name_entry_size;
			continue;
		}
		if (nas_map0)
			(*nas_map0)[key] = value;  //must check for null
		if (ntn_map0)
			(*ntn_map0)[key] = n;      //must check for null
		tabptr += name_entry_size;
	}
    return true;
}
//Explicit
template bool jpcre2::select<char>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size, PCRE2_SPTR tabptr, pcre2_match_data *match_data);
template bool jpcre2::select<wchar_t>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size, PCRE2_SPTR tabptr, pcre2_match_data *match_data);
#if __cplusplus >= 201103L
template bool jpcre2::select<char16_t>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size, PCRE2_SPTR tabptr, pcre2_match_data *match_data);
template bool jpcre2::select<char32_t>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size, PCRE2_SPTR tabptr, pcre2_match_data *match_data);
#endif


 template<class Char_T>
jpcre2::SIZE_T jpcre2::select<Char_T>::RegexMatch::match() {

	/// If Regex::code is null, return 0 as the match count
	if (re->code == 0)
		return 0;

	PCRE2_SPTR subject = (PCRE2_SPTR) m_subject.c_str();
	PCRE2_SPTR name_table;
	int crlf_is_newline;
	int namecount;
	int name_entry_size;
	int rc;
	int utf;
	SIZE_T count = 0;
	Uint option_bits;
	Uint newline;
	PCRE2_SIZE *ovector;
	SIZE_T subject_length;
	pcre2_match_data *match_data;
	subject_length = m_subject.length()/* Strlen((Char *) subject) */;
    

	/// Clear all (passed) vectors and initialize associated maps
    /// No memory will be allocated for a map if its associated vector is't passed.
	if (vec_num) {
		vec_num->clear();
		num_map0 = new MapNum();
	}
	if (vec_nas) {
		vec_nas->clear();
		nas_map0 = new MapNas();
	}
	if (vec_ntn) {
		vec_ntn->clear();
		ntn_map0 = new MapNtN();
	}

	/* Using this function ensures that the block is exactly the right size for
	 the number of capturing parentheses in the pattern. */

	match_data = pcre2_match_data_create_from_pattern(re->code, 0);

	rc = pcre2_match(re->code,  /* the compiled pattern */
                subject,        /* the subject string */
                subject_length, /* the length of the subject */
                0,              /* start at offset 0 in the subject */
                match_opts,     /* default options */
                match_data,     /* block for storing the result */
                0);             /* use default match context */

	/* Matching failed: handle error cases */

	if (rc < 0) {
		pcre2_match_data_free(match_data); /* Release memory used for the match */
		//must not free code. This function has no right to modify regex
		switch (rc) {
            case PCRE2_ERROR_NOMATCH:
                return count;
                /*
                 Handle other special cases if you like
                 */
            default:;
		}
        re->error_number = re->error_offset = rc;
		return count;
	}

	++count; //Increment the counter
	/* Match succeded. Get a pointer to the output vector, where string offsets are
	 stored. */
	ovector = pcre2_get_ovector_pointer(match_data);

	/************************************************************************//*
	 * We have found the first match within the subject string. If the output *
	 * vector wasn't big enough, say so. Then output any substrings that were *
	 * captured.                                                              *
	 *************************************************************************/

	/* The output vector wasn't big enough. This should not happen, because we used
	 pcre2_match_data_create_from_pattern() above. */

	if (rc == 0) {
		//ovector was not big enough for all the captured substrings;
		return count;

	}

	/// Get numbered substrings if #num_map0 isn't null
	if (num_map0) {
		if(!getNumberedSubstrings(rc, match_data))
            return count;
    }

	/*************************************************************************//*
	 * That concludes the basic part of this demonstration program. We have    *
	 * compiled a pattern, and performed a single match. The code that follows *
	 * shows first how to access named substrings, and then how to code for    *
	 * repeated matches on the same subject.                                   *
	 **************************************************************************/

	/* See if there are any named substrings, and if so, show them by name. First
	 we have to extract the count of named parentheses from the pattern. */

	(void) pcre2_pattern_info(re->code, /* the compiled pattern */
	PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
	&namecount); /* where to put the answer */

	if (namecount <= 0)
		; /*No named substrings*/

	else {
		PCRE2_SPTR tabptr;

		/* Before we can access the substrings, we must extract the table for
		 translating names to numbers, and the size of each entry in the table. */

		(void) pcre2_pattern_info(re->code, /* the compiled pattern */
		PCRE2_INFO_NAMETABLE, /* address of the table */
		&name_table); /* where to put the answer */

		(void) pcre2_pattern_info(re->code, /* the compiled pattern */
		PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
		&name_entry_size); /* where to put the answer */

		/* Now we can scan the table and, for each entry, print the number, the name,
		 and the substring itself. In the 8-bit library the number is held in two
		 bytes, most significant first. */

		tabptr = name_table;

		/// Get named substrings if #nas_map0 isn't null.
        /// Get name to number map if #ntn_map0 isn't null.
		if (nas_map0 || ntn_map0) {
			if(!getNamedSubstrings(namecount, name_entry_size, tabptr, match_data))
                return count;
        }

	}

	/// Populate vectors with their associated maps.
	pushMapsIntoVectors();

	/************************************************************************//*
	 * If the "-g" option was given on the command line, we want to continue  *
	 * to search for additional matches in the subject string, in a similar   *
	 * way to the /g option in Perl. This turns out to be trickier than you   *
	 * might think because of the possibility of matching an empty string.    *
	 * What happens is as follows:                                            *
	 *                                                                        *
	 * If the previous match was NOT for an empty string, we can just start   *
	 * the next match at the end of the previous one.                         *
	 *                                                                        *
	 * If the previous match WAS for an empty string, we can't do that, as it *
	 * would lead to an infinite loop. Instead, a call of pcre2_match() is    *
	 * made with the PCRE2_NOTEMPTY_ATSTART and PCRE2_ANCHORED flags set. The *
	 * first of these tells PCRE2 that an empty string at the start of the    *
	 * subject is not a valid match; other possibilities must be tried. The   *
	 * second flag restricts PCRE2 to one match attempt at the initial string *
	 * position. If this match succeeds, an alternative to the empty string   *
	 * match has been found, and we can print it and proceed round the loop,  *
	 * advancing by the length of whatever was found. If this match does not  *
	 * succeed, we still stay in the loop, advancing by just one character.   *
	 * In UTF-8 mode, which can be set by (*UTF) in the pattern, this may be  *
	 * more than one byte.                                                    *
	 *                                                                        *
	 * However, there is a complication concerned with newlines. When the     *
	 * newline convention is such that CRLF is a valid newline, we must       *
	 * advance by two characters rather than one. The newline convention can  *
	 * be set in the regex by (*CR), etc.; if not, we must find the default.  *
	 *************************************************************************/

	if ((jpcre2_match_opts & FIND_ALL) == 0) {
		pcre2_match_data_free(match_data); /* Release the memory that was used */
		//pcre2_code_free(re);                  /// Don't do this. This function has no right to modify regex.
		return count; /* Exit the program. */
	}

	/* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
	 sequence. First, find the options with which the regex was compiled and extract
	 the UTF state. */

	(void) pcre2_pattern_info(re->code, PCRE2_INFO_ALLOPTIONS, &option_bits);
	utf = (option_bits & PCRE2_UTF) != 0;

	/* Now find the newline convention and see whether CRLF is a valid newline
	 sequence. */

	(void) pcre2_pattern_info(re->code, PCRE2_INFO_NEWLINE, &newline);
	crlf_is_newline = newline == PCRE2_NEWLINE_ANY
			|| newline == PCRE2_NEWLINE_CRLF
			|| newline == PCRE2_NEWLINE_ANYCRLF;

	/** We got the first match. Now loop for second and subsequent matches. */

	for (;;) {
		/// Clear maps before filling it with new values
		if (num_map0)
			num_map0->clear();
		if (nas_map0)
			nas_map0->clear();
		if (ntn_map0)
			ntn_map0->clear();

		Uint options = match_opts; /* Normally no options */
		PCRE2_SIZE start_offset = ovector[1]; /* Start at end of previous match */

		/* If the previous match was for an empty string, we are finished if we are
		 at the end of the subject. Otherwise, arrange to run another match at the
		 same point to see if a non-empty match can be found. */

		if (ovector[0] == ovector[1]) {
			if (ovector[0] == subject_length)
				break;
			options |= PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
		}

		/** Run the next matching operation */

		rc = pcre2_match(re->code,  /* the compiled pattern */
                    subject,        /* the subject string */
                    subject_length, /* the length of the subject */
                    start_offset,   /* starting offset in the subject */
                    options,        /* options */
                    match_data,     /* block for storing the result */
                    0);             /* use default match context */

		/* This time, a result of NOMATCH isn't an error. If the value in "options"
		 is zero, it just means we have found all possible matches, so the loop ends.
		 Otherwise, it means we have failed to find a non-empty-string match at a
		 point where there was a previous empty-string match. In this case, we do what
		 Perl does: advance the matching position by one character, and continue. We
		 do this by setting the "end of previous match" offset, because that is picked
		 up at the top of the loop as the point at which to start again.

		 There are two complications: (a) When CRLF is a valid newline sequence, and
		 the current position is just before it, advance by an extra byte. (b)
		 Otherwise we must ensure that we skip an entire UTF character if we are in
		 UTF mode. */

		if (rc == PCRE2_ERROR_NOMATCH) {
			if (options == 0)
				break; /* All matches found */
			ovector[1] = start_offset + 1; /* Advance one code unit */
			if (crlf_is_newline && /* If CRLF is newline & */
			start_offset < subject_length - 1 && /* we are at CRLF, */
			subject[start_offset] == '\r' && subject[start_offset + 1] == '\n')
				ovector[1] += 1; /* Advance by one more. */
			else if (utf) { /* advance a whole UTF (8 or 16), for UTF-32, it's not needed */
				while (ovector[1] < subject_length) {
                    #if PCRE2_CODE_UNIT_WIDTH == 8
                        if ((subject[ovector[1]] & 0xc0) != 0x80)
                    #elif PCRE2_CODE_UNIT_WIDTH == 16
                        if ((subject[ovector[1]] & 0xfc00) != 0xdc00)
                    #endif
                    // This is a dummy loop for UTF-32 (no iteration)
                            break;
					ovector[1] += 1;
				}
			}
			continue; /* Go round the loop again */
		}

		/* Other matching errors are not recoverable. */

		if (rc < 0) {
			pcre2_match_data_free(match_data);
			//pcre2_code_free(code);           //must not do this. This function has no right to modify regex.
            re->error_number = re->error_offset = rc;
			return count;
		}

		/* match succeeded */
		++count; //Increment the counter

		if (rc == 0) {
			/* The match succeeded, but the output vector wasn't big enough. This
			 should not happen. */
			return count;
		}

		/* As before, get substrings stored in the output vector by number, and then
		 also any named substrings. */

		/// Get numbered substrings if #num_map0 isn't null
		if (num_map0) {
			if(!getNumberedSubstrings(rc, match_data))
                return count;
        }

		if (namecount <= 0)
			; /*No named substrings*/
		else {
			PCRE2_SPTR tabptr = name_table;

            /// Get named substrings if #nas_map0 isn't null.
            /// Get name to number map if #ntn_map0 isn't null.
			if (nas_map0 || ntn_map0) {
				if(!getNamedSubstrings(namecount, name_entry_size, tabptr, match_data))
                    return count;
            }
		}

		/// Populate vectors with their associated maps.
		pushMapsIntoVectors();

	} /* End of loop to find second and subsequent matches */

	pcre2_match_data_free(match_data);
	// Must not free pcre2_code* code. This function has no right to modify regex.
	return count;
}
//Explicit
template jpcre2::SIZE_T jpcre2::select<char>::RegexMatch::match();
template jpcre2::SIZE_T jpcre2::select<wchar_t>::RegexMatch::match();
#if __cplusplus >= 201103L
template jpcre2::SIZE_T jpcre2::select<char16_t>::RegexMatch::match();
template jpcre2::SIZE_T jpcre2::select<char32_t>::RegexMatch::match();
#endif


#undef JPCRE2_GLUE
#undef JPCRE2_JOIN
#undef JPCRE2_SUFFIX
#undef select
