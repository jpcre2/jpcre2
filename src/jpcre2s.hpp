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

/** @file jpcre2.hpp
 * @brief Main header file for JPCRE2 library to be included by programs that uses its functionalities.
 * It includes the pcre2.h header, therefore you shouldn't include pcre2.h
 * separately in your program. Make sure to link PCRE2 library when compiling.
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 */

#ifndef JPCRE2_HPP
#define JPCRE2_HPP

#include <pcre2.h>
#include <string>       // std::string, std::wstring
#include <vector>       // std::vector
#include <map>          // std::map
#include <cstdio>       // snprintf
#include <cwchar>       // wcslen, std::mbstate_t
#include <cstring>      // strlen
#include <clocale>      // std::setlocale


#if __cplusplus >= 201103L
    //These will be included if >=C++11 is used
	#include <codecvt>
	#include <locale>  // std::wstring_convert
#endif


/** @namespace jpcre2
 *  Top level namespace of JPCRE2.
 *
 *  All functions, classes/structs, constants, enums that are provided by JPCRE2 belong to this namespace while
 *  **PCRE2** structs, functions, constants remain outside of its scope.
 *
 *  If you want to use any PCRE2 functions or constants,
 *  remember that they are in the global scope and should be used as such.
 */

namespace jpcre2 {


typedef PCRE2_SIZE SIZE_T;                          ///< Used for match count and vector size
typedef uint32_t Uint;                              ///< Used for options (bitwise operation)

/// @namespace jpcre2::ERROR
/// Namespace for error codes.
namespace ERROR {
	/** ERROR numbers.
	 *  JPCRE2 error numbers are positive integers while
	 *  PCRE2 error numbers are negative integers.
	 */
	enum {
		INVALID_MODIFIER        = 2,                ///< Error number implying that invalid modifier was detected
		JIT_COMPILE_FAILED      = 3                 ///< Error number implying that JIT compile failed
	};
}


/** These constants provide JPCRE2 options.
 */
enum {
	NONE                    = 0x0000000u,           ///< Option 0 (zero)
	FIND_ALL                = 0x0000002u,           ///< Find all during match (global match)
	JIT_COMPILE             = 0x0000004u            ///< Perform JIT compilation for optimization
};


//define some constants, we can't put these in the cpp file, 
//because a program may use multiple libraries which will end up in
//multiple definitions.

/// Used by default to provide big enough initial buffer for replaced string.
/// 0 or insufficient values will force a second call to pcre2_substitute(),
/// If that is not desirable, pass a big enough buffer size with
/// RegexReplace::setBufferSize() function to make it a single call operation.
const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE = 0;
/// Don't do anything about locale if it is set to jpcre2::LOCALE_NONE
const std::string LOCALE_NONE = "JPCRE2_NONE";
/// Default locale
const std::string LOCALE_DEFAULT = LOCALE_NONE;   


#if __cplusplus >= 201103L
///@struct Codecvt
///Convenience wrapper of `std::codecvt`.
///Only available for >=C++11.
template <class internT, class externT, class stateT>
struct Codecvt : std::codecvt<internT,externT,stateT>
{ ~Codecvt(){} };

///@var convert16
///This is a convenience object (>=C++11) to convert between UTF-8 <> UTF-16.
///Convert UTF-16 to UTF-8
///```cpp
///convert16.to_bytes(utf16string)
///```
///Convert UTF-8 to UTF-16
///```cpp
///convert16.from_bytes(utf8string)
///```
static thread_local std::wstring_convert<Codecvt<char16_t,char,std::mbstate_t>,char16_t> convert16;
///@var convert32
///This is a convenience object (>=C++11) to convert between UTF-8 <> UTF-32.
///Convert UTF-32 to UTF-8:
///```cpp
///convert32.to_bytes(utf32string)
///```
///Convert UTF-8 to UTF-32..
///```cpp
///convert32.from_bytes(utf8string)
///```
static thread_local std::wstring_convert<Codecvt<char32_t,char,std::mbstate_t>,char32_t> convert32;

#endif


////////////////////////// The following are type and function mappings from PCRE2 interface to JPCRE2 interface /////////////////////////

//forward decalration

template<int S> struct Pcre2Type;
template<int S> struct Pcre2FuncPtr;
template<int S> struct Pcre2Func;

//PCRE2 types
template<int S> struct Pcre2Type {};

template<> struct Pcre2Type<8>{
    //typedefs used
    typedef PCRE2_UCHAR8 Pcre2Uchar;
    typedef PCRE2_SPTR8 Pcre2Sptr;
    typedef pcre2_code_8 Pcre2Code;
    typedef pcre2_compile_context_8 CompileContext;
    typedef pcre2_match_data_8 MatchData;
    typedef pcre2_general_context_8 GeneralContext;
    typedef pcre2_match_context_8 MatchContext;
};
template<> struct Pcre2Type<16>{
    //typedefs used
    typedef PCRE2_UCHAR16 Pcre2Uchar;
    typedef PCRE2_SPTR16 Pcre2Sptr;
    typedef pcre2_code_16 Pcre2Code;
    typedef pcre2_compile_context_16 CompileContext;
    typedef pcre2_match_data_16 MatchData;
    typedef pcre2_general_context_16 GeneralContext;
    typedef pcre2_match_context_16 MatchContext;
};
template<> struct Pcre2Type<32>{
    //typedefs used
    typedef PCRE2_UCHAR32 Pcre2Uchar;
    typedef PCRE2_SPTR32 Pcre2Sptr;
    typedef pcre2_code_32 Pcre2Code;
    typedef pcre2_compile_context_32 CompileContext;
    typedef pcre2_match_data_32 MatchData;
    typedef pcre2_general_context_32 GeneralContext;
    typedef pcre2_match_context_32 MatchContext;
};


//Function pointers for pcre2 functions
template<int S> struct Pcre2FuncPtr {
    typedef typename Pcre2Type<S>::CompileContext* (*Compile_context_create)(typename Pcre2Type<S>::GeneralContext*);
    typedef const unsigned char * (*Maketables)(typename Pcre2Type<S>::GeneralContext*);
    typedef int (*Set_character_tables)(typename Pcre2Type<S>::CompileContext *, const unsigned char *);
    typedef  typename Pcre2Type<S>::Pcre2Code * (*Compile)( typename Pcre2Type<S>::Pcre2Sptr pattern,
                                     PCRE2_SIZE length,
                                     uint32_t options,
                                     int *errorcode,
                                     PCRE2_SIZE *erroroffset,
                                     typename Pcre2Type<S>::CompileContext *ccontext);
    typedef int (*Jit_compile)(typename Pcre2Type<S>::Pcre2Code *, uint32_t);
    typedef int (*Substitute)(  const typename Pcre2Type<S>::Pcre2Code *code,
                                typename Pcre2Type<S>::Pcre2Sptr subject, 
                                PCRE2_SIZE length, 
                                PCRE2_SIZE startoffset, 
                                uint32_t options, 
                                typename Pcre2Type<S>::MatchData *match_data, 
                                typename Pcre2Type<S>::MatchContext *mcontext, 
                                typename Pcre2Type<S>::Pcre2Sptr replacement, 
                                PCRE2_SIZE rlength, 
                                typename Pcre2Type<S>::Pcre2Uchar *outputbuffer, 
                                PCRE2_SIZE *outlengthptr); 
    typedef int (*Substring_get_bynumber)(typename Pcre2Type<S>::MatchData *match_data,
                                        uint32_t number, 
                                        typename Pcre2Type<S>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen);
    typedef void (*Substring_free)(typename Pcre2Type<S>::Pcre2Uchar *buffer);
    typedef typename Pcre2Type<S>::Pcre2Code * (*Code_copy)(const typename Pcre2Type<S>::Pcre2Code *code);
    typedef void (*Code_free)(typename Pcre2Type<S>::Pcre2Code *code);
    typedef int (*Get_error_message)(  int errorcode,
                                        typename Pcre2Type<S>::Pcre2Uchar *buffer,
                                        PCRE2_SIZE bufflen);
    typedef int (*Substring_get_byname)(typename Pcre2Type<S>::MatchData *match_data,
                                        typename Pcre2Type<S>::Pcre2Sptr name, 
                                        typename Pcre2Type<S>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen); 
    typedef typename Pcre2Type<S>::MatchData * (*Match_data_create_from_pattern)(const typename Pcre2Type<S>::Pcre2Code *code,
                                                          typename Pcre2Type<S>::GeneralContext *gcontext); 
    typedef int (*Match)(  const typename Pcre2Type<S>::Pcre2Code *code, 
                            typename Pcre2Type<S>::Pcre2Sptr subject, 
                            PCRE2_SIZE length, 
                            PCRE2_SIZE startoffset, 
                            uint32_t options, 
                            typename Pcre2Type<S>::MatchData *match_data, 
                            typename Pcre2Type<S>::MatchContext *mcontext);
    typedef void (*Match_data_free)(typename Pcre2Type<S>::MatchData *match_data);
    typedef PCRE2_SIZE * (*Get_ovector_pointer)(typename Pcre2Type<S>::MatchData *match_data);
    typedef int (*Pattern_info)(const typename Pcre2Type<S>::Pcre2Code *code, uint32_t what, void *where);
};


//Function pointer typedefs of PCRE2 functions
template<int S> struct Pcre2Func {
    static typename Pcre2FuncPtr<S>::Compile_context_create compile_context_create;
    static typename Pcre2FuncPtr<S>::Maketables maketables;
    static typename Pcre2FuncPtr<S>::Set_character_tables set_character_tables;
    static typename Pcre2FuncPtr<S>::Compile compile;
    static typename Pcre2FuncPtr<S>::Jit_compile jit_compile;
    static typename Pcre2FuncPtr<S>::Substitute substitute;
    static typename Pcre2FuncPtr<S>::Substring_get_bynumber substring_get_bynumber;
    static typename Pcre2FuncPtr<S>::Substring_free substring_free;
    static typename Pcre2FuncPtr<S>::Code_copy code_copy;
    static typename Pcre2FuncPtr<S>::Code_free code_free;
    static typename Pcre2FuncPtr<S>::Get_error_message get_error_message;
    static typename Pcre2FuncPtr<S>::Substring_get_byname substring_get_byname;
    static typename Pcre2FuncPtr<S>::Match_data_create_from_pattern match_data_create_from_pattern;
    static typename Pcre2FuncPtr<S>::Match match;
    static typename Pcre2FuncPtr<S>::Match_data_free match_data_free;
    static typename Pcre2FuncPtr<S>::Get_ovector_pointer get_ovector_pointer;
    static typename Pcre2FuncPtr<S>::Pattern_info pattern_info;
 };

//8-bit versions
template<>
typename Pcre2FuncPtr<8>::Compile_context_create Pcre2Func<8>::compile_context_create = &pcre2_compile_context_create_8;
template<>
typename Pcre2FuncPtr<8>::Maketables Pcre2Func<8>::maketables = &pcre2_maketables_8;
template<>
typename Pcre2FuncPtr<8>::Set_character_tables Pcre2Func<8>::set_character_tables = &pcre2_set_character_tables_8;
template<>
typename Pcre2FuncPtr<8>::Compile Pcre2Func<8>::compile = &pcre2_compile_8;
template<>
typename Pcre2FuncPtr<8>::Jit_compile Pcre2Func<8>::jit_compile = pcre2_jit_compile_8;
template<>
typename Pcre2FuncPtr<8>::Substitute Pcre2Func<8>::substitute = &pcre2_substitute_8;
template<>
typename Pcre2FuncPtr<8>::Substring_get_bynumber Pcre2Func<8>::substring_get_bynumber = &pcre2_substring_get_bynumber_8;
template<>
typename Pcre2FuncPtr<8>::Substring_free Pcre2Func<8>::substring_free = &pcre2_substring_free_8;
template<>
typename Pcre2FuncPtr<8>::Code_copy Pcre2Func<8>::code_copy = &pcre2_code_copy_8;
template<>
typename Pcre2FuncPtr<8>::Code_free Pcre2Func<8>::code_free = &pcre2_code_free_8;
template<>
typename Pcre2FuncPtr<8>::Get_error_message Pcre2Func<8>::get_error_message = &pcre2_get_error_message_8;
template<>
typename Pcre2FuncPtr<8>::Substring_get_byname Pcre2Func<8>::substring_get_byname = &pcre2_substring_get_byname_8;
template<>
typename Pcre2FuncPtr<8>::Match_data_create_from_pattern Pcre2Func<8>::match_data_create_from_pattern =  
                                        &pcre2_match_data_create_from_pattern_8;
template<>
typename Pcre2FuncPtr<8>::Match Pcre2Func<8>::match = &pcre2_match_8;
template<>
typename Pcre2FuncPtr<8>::Match_data_free Pcre2Func<8>::match_data_free = &pcre2_match_data_free_8;
template<>
typename Pcre2FuncPtr<8>::Get_ovector_pointer Pcre2Func<8>::get_ovector_pointer = &pcre2_get_ovector_pointer_8;
template<>
typename Pcre2FuncPtr<8>::Pattern_info Pcre2Func<8>::pattern_info = &pcre2_pattern_info_8;

//16-bit versions
template<>
typename Pcre2FuncPtr<16>::Compile_context_create Pcre2Func<16>::compile_context_create = &pcre2_compile_context_create_16;
template<>
typename Pcre2FuncPtr<16>::Maketables Pcre2Func<16>::maketables = &pcre2_maketables_16;
template<>
typename Pcre2FuncPtr<16>::Set_character_tables Pcre2Func<16>::set_character_tables = &pcre2_set_character_tables_16;
template<>
typename Pcre2FuncPtr<16>::Compile Pcre2Func<16>::compile = &pcre2_compile_16;
template<>
typename Pcre2FuncPtr<16>::Jit_compile Pcre2Func<16>::jit_compile = pcre2_jit_compile_16;
template<>
typename Pcre2FuncPtr<16>::Substitute Pcre2Func<16>::substitute = &pcre2_substitute_16;
template<>
typename Pcre2FuncPtr<16>::Substring_get_bynumber Pcre2Func<16>::substring_get_bynumber = &pcre2_substring_get_bynumber_16;
template<>
typename Pcre2FuncPtr<16>::Substring_free Pcre2Func<16>::substring_free = &pcre2_substring_free_16;
template<>
typename Pcre2FuncPtr<16>::Code_copy Pcre2Func<16>::code_copy = &pcre2_code_copy_16;
template<>
typename Pcre2FuncPtr<16>::Code_free Pcre2Func<16>::code_free = &pcre2_code_free_16;
template<>
typename Pcre2FuncPtr<16>::Get_error_message Pcre2Func<16>::get_error_message = &pcre2_get_error_message_16;
template<>
typename Pcre2FuncPtr<16>::Substring_get_byname Pcre2Func<16>::substring_get_byname = &pcre2_substring_get_byname_16;
template<>
typename Pcre2FuncPtr<16>::Match_data_create_from_pattern Pcre2Func<16>::match_data_create_from_pattern =  
                                        &pcre2_match_data_create_from_pattern_16;
template<>
typename Pcre2FuncPtr<16>::Match Pcre2Func<16>::match = &pcre2_match_16;
template<>
typename Pcre2FuncPtr<16>::Match_data_free Pcre2Func<16>::match_data_free = &pcre2_match_data_free_16;
template<>
typename Pcre2FuncPtr<16>::Get_ovector_pointer Pcre2Func<16>::get_ovector_pointer = &pcre2_get_ovector_pointer_16;
template<>
typename Pcre2FuncPtr<16>::Pattern_info Pcre2Func<16>::pattern_info = &pcre2_pattern_info_16;

//32-bit versions
template<>
typename Pcre2FuncPtr<32>::Compile_context_create Pcre2Func<32>::compile_context_create = &pcre2_compile_context_create_32;
template<>
typename Pcre2FuncPtr<32>::Maketables Pcre2Func<32>::maketables = &pcre2_maketables_32;
template<>
typename Pcre2FuncPtr<32>::Set_character_tables Pcre2Func<32>::set_character_tables = &pcre2_set_character_tables_32;
template<>
typename Pcre2FuncPtr<32>::Compile Pcre2Func<32>::compile = &pcre2_compile_32;
template<>
typename Pcre2FuncPtr<32>::Jit_compile Pcre2Func<32>::jit_compile = pcre2_jit_compile_32;
template<>
typename Pcre2FuncPtr<32>::Substitute Pcre2Func<32>::substitute = &pcre2_substitute_32;
template<>
typename Pcre2FuncPtr<32>::Substring_get_bynumber Pcre2Func<32>::substring_get_bynumber = &pcre2_substring_get_bynumber_32;
template<>
typename Pcre2FuncPtr<32>::Substring_free Pcre2Func<32>::substring_free = &pcre2_substring_free_32;
template<>
typename Pcre2FuncPtr<32>::Code_copy Pcre2Func<32>::code_copy = &pcre2_code_copy_32;
template<>
typename Pcre2FuncPtr<32>::Code_free Pcre2Func<32>::code_free = &pcre2_code_free_32;
template<>
typename Pcre2FuncPtr<32>::Get_error_message Pcre2Func<32>::get_error_message = &pcre2_get_error_message_32;
template<>
typename Pcre2FuncPtr<32>::Substring_get_byname Pcre2Func<32>::substring_get_byname = &pcre2_substring_get_byname_32;
template<>
typename Pcre2FuncPtr<32>::Match_data_create_from_pattern Pcre2Func<32>::match_data_create_from_pattern =  
                                        &pcre2_match_data_create_from_pattern_32;
template<>
typename Pcre2FuncPtr<32>::Match Pcre2Func<32>::match = &pcre2_match_32;
template<>
typename Pcre2FuncPtr<32>::Match_data_free Pcre2Func<32>::match_data_free = &pcre2_match_data_free_32;
template<>
typename Pcre2FuncPtr<32>::Get_ovector_pointer Pcre2Func<32>::get_ovector_pointer = &pcre2_get_ovector_pointer_32;
template<>
typename Pcre2FuncPtr<32>::Pattern_info Pcre2Func<32>::pattern_info = &pcre2_pattern_info_32;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @namespace jpcre2::INFO
 *  Namespace to provide information about JPCRE2 library itself.
 *  Contains constant Strings with version info.
 */
namespace INFO {
	const std::string NAME = "JPCRE2";               ///< Name of the project
	const std::string FULL_VERSION = "10.28.01";     ///< Full version string
	const std::string VERSION_GENRE = "10";          ///< Generation, depends on original PCRE2 version
	const std::string VERSION_MAJOR = "28";          ///< Major version, updated when API change is made
	const std::string VERSION_MINOR = "01";          ///< Minor version, includes bug fix or minor feature upgrade
	const std::string VERSION_PRE_RELEASE = "";      ///< Alpha or beta (testing) release version
}



// Namespace for modifier constants.
// For each modifier constant there is a jpcre2::Uint option value.
// Some modifiers may have multiple values set together (ORed in bitwise operation) and
// thus they may include other modifiers. Such an example is the 'n' modifier. It is combined together with 'u'.
namespace MOD {

    // Define modifiers for compile
    // Every modifier needs to be unique in this block
    // String of compile modifier characters for PCRE2 options
    const std::string C_N("eijmnsuxADJU");
    // Array of compile modifier values for PCRE2 options
    const jpcre2::Uint C_V[12] = {  PCRE2_MATCH_UNSET_BACKREF,                  // Modifier e
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
                                              

    // String of compile modifier characters for JPCRE2 options
    const std::string CJ_N("S");
    // Array of compile modifier values for JPCRE2 options
    const jpcre2::Uint CJ_V[1] = { JIT_COMPILE,                                // Modifier S
                                              };


    // Define modifiers for replace
    // Every modifier needs to be unique in this block
    // String of action (replace) modifier characters for PCRE2 options
    const std::string R_N("eEgx");
    // Array of action (replace) modifier values for PCRE2 options
    const jpcre2::Uint R_V[4] = {  PCRE2_SUBSTITUTE_UNSET_EMPTY,                // Modifier  e
                                   PCRE2_SUBSTITUTE_UNKNOWN_UNSET | PCRE2_SUBSTITUTE_UNSET_EMPTY,   // Modifier E (includes e)
                                   PCRE2_SUBSTITUTE_GLOBAL,                     // Modifier g
                                   PCRE2_SUBSTITUTE_EXTENDED                    // Modifier x
                                 };


    // String of action (replace) modifier characters for JPCRE2 options
    const std::string RJ_N("");
    // Array of action (replace) modifier values for JPCRE2 options
    const jpcre2::Uint RJ_V[1] = { NONE
                                              };
    //Explicit

    // Define modifiers for match
    // Every modifier needs to be unique in this block

    // String of action (match) modifier characters for PCRE2 options
    const std::string M_N("A");
    // Array of action (match) modifier values for PCRE2 options
    const jpcre2::Uint M_V[1] = { PCRE2_ANCHORED                               // Modifier  A
                                             };


    // String of action (match) modifier characters for JPCRE2 options
    const std::string MJ_N("g");
    // Array of action (match) modifier values for JPCRE2 options
    const jpcre2::Uint MJ_V[1] = { FIND_ALL,                                   // Modifier  g
                                              };

}


///@struct MSG
///JPCRE2 messages as String.
template<typename Char_T> struct MSG{
    static const std::basic_string<Char_T> INVALID_MODIFIER;
};

template<> const std::basic_string<char> MSG<char>::INVALID_MODIFIER = "Invalid modifier: ";
template<> const std::basic_string<wchar_t> MSG<wchar_t>::INVALID_MODIFIER = L"Invalid modifier: ";

#if __cplusplus >= 201103L
template<> const std::basic_string<char16_t> MSG<char16_t>::INVALID_MODIFIER = u"Invalid modifier: ";
template<> const std::basic_string<char32_t> MSG<char32_t>::INVALID_MODIFIER = U"Invalid modifier: ";
#endif


///@struct ParseInt
///Contains function that parses integer.
template<typename Char_T> struct ParseInt{
    static std::basic_string<Char_T> toString(int x);
};

///Converts integer to std::string
///@param x the integer to convert
///@return std::string from the integer
template<> std::string ParseInt<char>::toString(int x){
    //this function can only convert to std::string and std::wstring
    int length = snprintf(0, 0, "%d", x);
    //assert(length >= 0);
    char* buf = new char[length + 1];
    snprintf(buf, length +1, "%d", x);
    std::string str(buf);
    delete[] buf;
    return str;
}

///@overload
///
///Converts integer to std::wstring
///@param x the integer to convert
///@return std::wstring from the integer
template<> std::wstring ParseInt<wchar_t>::toString(int x){
    //this function can only convert to std::string and std::wstring
    int length = swprintf(0, 0, L"%d", x);
    //assert(length >= 0);
    wchar_t* buf = new wchar_t[length + 1];
    swprintf(buf, length +1, L"%d", x);
    std::wstring str(buf);
    delete[] buf;
    return str;
}

    
#if __cplusplus >= 201103L

///@overload
///
///Converts integer to std::u16string.
///Uses codecvt to convert to utf16 from utf8
///@param x int to convert
///@return std::u16string from the integer
template<> std::u16string ParseInt<char16_t>::toString(int x) {
    std::string s = std::to_string(x);
    std::u16string us = convert16.from_bytes(s);
    return us;
}

///@overload
///
///Converts integer to std::u32string.
///Uses codecvt to convert to utf32 from utf8
///@param x int to convert
///@return std::u32string from the integer
template<> std::u32string ParseInt<char32_t>::toString(int x) {
    std::string s = std::to_string(x);
    std::u32string us = convert32.from_bytes(s);
    return us;
}
#endif




//The select struct
template<int S, typename Char_T> 
struct select{
    virtual ~select(){}	//allow subclassing

    typedef Char_T Char;
    typedef typename std::basic_string<Char_T> String;

    typedef typename std::map<String, String> MapNas;
    typedef typename std::vector<String> NumSub;     
    typedef typename std::map<String, SIZE_T> MapNtN;
    typedef MapNtN MapNtn;
    typedef typename std::vector<MapNas> VecNas;            
    typedef typename std::vector<MapNtN> VecNtN;            
    typedef VecNtN VecNtn;
    typedef typename std::vector<NumSub> VecNum;         
    
    //This is to shorten the code
    typedef typename Pcre2Type<S>::Pcre2Uchar Pcre2Uchar;
    typedef typename Pcre2Type<S>::Pcre2Sptr Pcre2Sptr;
    typedef typename Pcre2Type<S>::Pcre2Code Pcre2Code;
    typedef typename Pcre2Type<S>::CompileContext CompileContext;
    typedef typename Pcre2Type<S>::MatchData MatchData;
    typedef typename Pcre2Type<S>::GeneralContext GeneralContext;
    typedef typename Pcre2Type<S>::MatchContext MatchContext; 
    
    ///@overload
    ///
    ///Converts a Char_T (char, wchar_t, char16_t, char32_t) to String
    ///@param a Char_T
    ///@return String
    static String toString(Char a){
        if (a) return String(1, a);
        else return String();
    }                     
    
    ///@overload
    ///
    ///Converts a Char_T* (char*, wchar_t*, char16_t*, char32_t*) to String
    ///@param a Char_T*
    ///@return String
    static String toString(const Char* a){
        if (a) return String(a);
        else return String();
    }              
    
    ///@overload
    ///
    ///Converts a PCRE2_UCHAR to String
    ///@param a Char_T
    ///@return String
    static String toString(Pcre2Uchar* a) {
        if (a) return String((Char*) a);
        else return String();
    }
    
    ///Retruns error message from PCRE2 error number
    ///@param err_num error number (negative)
    ///@return message as String.
    static String getPcre2ErrorMessage(int err_num){
        Pcre2Uchar buffer[8048];
        Pcre2Func<S>::get_error_message(err_num, buffer, sizeof(buffer));
        return toString((Pcre2Uchar*) buffer);
    }          
    
    ///Retruns error message (either JPCRE2 or PCRE2) from error number and error offset
    ///@param err_num error number (negative for PCRE2, positive for JPCRE2)
    ///@param err_off error offset
    ///@return message as String.
    static String getErrorMessage(int err_num, int err_off) {
        if (err_num == (int)ERROR::JIT_COMPILE_FAILED) {
            return getPcre2ErrorMessage((int) err_off);
        } else if(err_num == (int)ERROR::INVALID_MODIFIER){
            return MSG<Char>::INVALID_MODIFIER + toString((Char)err_off);
        } else if(err_num != 0) {
            return getPcre2ErrorMessage((int) err_num) + ParseInt<Char>::toString((int) err_off);
        }
        else return String();
    }

    class Regex; 

    class RegexMatch { 

    private: 

        Regex* re;              

        String m_subject;       
        Uint match_opts;        
        Uint jpcre2_match_opts; 

        VecNum* vec_num;        
        VecNas* vec_nas;        
        VecNtN* vec_ntn;        

        NumSub* num_sub;       
        MapNas* nas_map;       
        MapNtN* ntn_map;       

        bool getNumberedSubstrings(int, MatchData *);   

        bool getNamedSubstrings(int, int, Pcre2Sptr, MatchData *);   
        
        void pushMapsIntoVectors(void){ 
            if (vec_num) 
                vec_num->push_back(*num_sub); 
            if (vec_nas) 
                vec_nas->push_back(*nas_map); 
            if (vec_ntn) 
                vec_ntn->push_back(*ntn_map); 
        } 
        
        void init_vars() { 
            vec_num = 0; 
            vec_nas = 0; 
            vec_ntn = 0; 
            num_sub = 0; 
            nas_map = 0; 
            ntn_map = 0; 
            match_opts = 0; 
            jpcre2_match_opts = 0; 
        } 

        RegexMatch() { 
            init_vars(); 
        } 
        RegexMatch(const RegexMatch&) { 
            init_vars(); 
        } 
        ~RegexMatch() { 
            delete num_sub; 
            delete nas_map; 
            delete ntn_map; 
        } 

        friend class Regex; 

    public: 

        RegexMatch& reset() { 
            delete num_sub; 
            delete nas_map; 
            delete ntn_map; 
            init_vars(); 
            m_subject.clear(); 
            return *this; 
        } 
        String getSubject() { 
            return m_subject; 
        } 

        std::string getModifier(); 
        
        Uint getPcre2Option() { 
            return match_opts; 
        } 

        Uint getJpcre2Option() { 
            return jpcre2_match_opts; 
        } 

        RegexMatch& resetErrors(); 

        RegexMatch& setNumberedSubstringVector(VecNum* v) { 
            vec_num = v; 
            return *this; 
        } 

        RegexMatch& setNamedSubstringVector(VecNas* v) { 
            vec_nas = v; 
            return *this; 
        } 

        RegexMatch& setNameToNumberMapVector(VecNtN* v) { 
            vec_ntn = v; 
            return *this; 
        } 

        RegexMatch& setSubject(const String& s) { 
            m_subject = s; 
            return *this; 
        } 

        RegexMatch& setModifier(const std::string& s) { 
            match_opts = 0; 
            jpcre2_match_opts = 0; 
            changeModifier(s, true); 
            return *this; 
        } 

        RegexMatch& setJpcre2Option(Uint x) { 
            jpcre2_match_opts = x; 
            return *this; 
        } 

        RegexMatch& setPcre2Option(Uint x) { 
            match_opts = x; 
            return *this; 
        } 
        RegexMatch& setFindAll(bool x) { 
            if (x) 
                jpcre2_match_opts |= FIND_ALL; 
            else 
                jpcre2_match_opts &= ~FIND_ALL; 
            return *this; 
        } 

        RegexMatch& setFindAll() { 
            return setFindAll(true); 
        } 

        RegexMatch& changeModifier(const std::string&, bool); 

        RegexMatch& changeJpcre2Option(Uint opt, bool x) { 
            if(x) 
                jpcre2_match_opts |= opt; 
            else 
                jpcre2_match_opts &= ~opt; 
            return *this; 
        } 

        RegexMatch& changePcre2Option(Uint opt, bool x) { 
            if(x) 
                match_opts |= opt; 
            else 
                match_opts &= ~opt; 
            return *this; 
        } 
        
        RegexMatch& addModifier(const std::string& mod){ 
            return changeModifier(mod, true); 
        } 

        RegexMatch& addJpcre2Option(Uint x) { 
            jpcre2_match_opts |= x; 
            return *this; 
        } 

        RegexMatch& addPcre2Option(Uint x) { 
            match_opts |= x; 
            return *this; 
        }

        SIZE_T match(void); 
    }; 

    class RegexReplace { 

    private: 

        Regex* re;      

        String r_subject;           
        String r_replw;             
        Uint replace_opts;          
        Uint jpcre2_replace_opts;  
        PCRE2_SIZE buffer_size;     
        
        void init_vars() { 
            replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; 
            jpcre2_replace_opts = 0; 
            buffer_size = SUBSTITUTE_RESULT_INIT_SIZE; 
        }

        RegexReplace() { 
            init_vars(); 
        } 

        RegexReplace(const RegexReplace&) { 
            init_vars(); 
        } 

        ~RegexReplace() {} 

        friend class Regex; 

    public: 

        RegexReplace& reset() { 
            init_vars(); 
            r_subject.clear(); 
            r_replw.clear(); 
            return *this; 
        } 
        String getReplaceWith() { 
            return r_replw; 
        } 

        String getSubject() { 
            return r_subject; 
        } 

        std::string getModifier(); 
        
        Uint getPcre2Option() { 
            return replace_opts; 
        } 

        Uint getJpcre2Option() { 
            return jpcre2_replace_opts; 
        } 

        RegexReplace& resetErrors(); 

        RegexReplace& setSubject(const String& s) { 
            r_subject = s; 
            return *this; 
        } 

        RegexReplace& setReplaceWith(const String& s) { 
            r_replw = s; 
            return *this; 
        } 
        RegexReplace& setModifier(const std::string& s) { 
            replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; /* must not be initialized to 0 */ 
            jpcre2_replace_opts = 0; 
            return changeModifier(s, true); 
        } 

        RegexReplace& setBufferSize(PCRE2_SIZE x) { 
            buffer_size = x; 
            return *this; 
        } 

        RegexReplace& setJpcre2Option(Uint x) { 
            jpcre2_replace_opts = x; 
            return *this; 
        } 

        RegexReplace& setPcre2Option(Uint x) { 
            replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH | x; 
            return *this; 
        } 

        RegexReplace& changeModifier(const std::string&, bool); 
        RegexReplace& changeJpcre2Option(Uint opt, bool x) { 
            if(x) 
                jpcre2_replace_opts |= opt; 
            else 
                jpcre2_replace_opts &= ~opt; 
            return *this; 
        } 

        RegexReplace& changePcre2Option(Uint opt, bool x) { 
            if(x) 
                replace_opts |= opt; 
            else 
                replace_opts &= ~opt; 
                replace_opts |= PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; /* It's important, this option must not be removed */ 
            return *this; 
        } 
        
        RegexReplace& addModifier(const std::string& mod){ 
            return changeModifier(mod, true); 
        } 

        RegexReplace& addJpcre2Option(Uint x) { 
            jpcre2_replace_opts |= x; 
            return *this; 
        } 
        RegexReplace& addPcre2Option(Uint x) { 
            replace_opts |= x; 
            return *this; 
        } 
        
        String replace(void); 
    }; 

    class Regex { 

    private: 

        RegexMatch *rm;		        
        RegexReplace *rr;	        

        String pat_str;		        
        Pcre2Code *code;	        
        Uint compile_opts;         
        Uint jpcre2_compile_opts;	
        std::string mylocale;	        
        int error_number;          
        PCRE2_SIZE error_offset;    

        void init_vars() {
            mylocale = LOCALE_DEFAULT; 
            jpcre2_compile_opts = 0; 
            compile_opts = 0; 
            error_number = 0; 
            error_offset = 0; 
            code = 0; 
            rr = 0; 
            rm = 0; 
        } 

        void init() { 
            init_vars(); 
        } 

        void init(const String& re, const std::string& mod) { 
            init_vars(); 
            pat_str = re; 
            changeModifier(mod, true); 
        } 

        void init(const String& re, Uint po, Uint jo) { 
            init_vars(); 
            pat_str = re; 
            compile_opts = po; 
            jpcre2_compile_opts = jo; 
        } 

        void freeRegexMemory(void) { 
            if (code) /* We don't need to free code if it's null */ 
                Pcre2Func<S>::code_free(code); 
        }

        friend class RegexMatch;	
        friend class RegexReplace; 

        void shallowCopy(const Regex& r) { 
            pat_str = r.pat_str; 
            mylocale = r.mylocale; 
            compile_opts = r.compile_opts; 
            jpcre2_compile_opts = r.jpcre2_compile_opts; 
            error_number = r.error_number; 
            error_offset = r.error_offset; 
        } 

        void deepCopy(const Regex&); 

    public: 

        Regex() { 
            init(); 
        } 

        Regex(const String& re) { 
            compile(re, 0, 0); 
        } 
        Regex(const String& re, const std::string& mod) { 
            compile(re, mod); 
        } 

        Regex(const String& re, Uint pcre2_opts) { 
            compile(re, pcre2_opts, 0); 
        } 

        Regex(const String& re, Uint pcre2_opts, Uint opt_bits) { 
            compile(re, pcre2_opts, opt_bits); 
        } 

        Regex(const Regex& r) { 
            shallowCopy(r); /* shallow copy must be performed **before** deep copy */ 
            deepCopy(r); 
        } 
        Regex& operator=(const Regex& r) { 
            if (this == &r) 
                return *this; 
            shallowCopy(r); /* shallow copy must be performed before deep copy */ 
            deepCopy(r); 
            return *this; 
        } 
        
        #if __cplusplus >= 201103L
        explicit operator bool() const { 
            return (code != 0); 
        }
        #endif
            
        bool operator!() const { 
            return (code == 0); 
        } 

        ~Regex() { 
            freeRegexMemory(); 
            delete rm; /* Deleting null pointer is perfectly safe, no check needed. */ 
            delete rr; /* Deleting null pointer is perfectly safe, no check needed. */ 
        } 

        Regex& reset() { 
            freeRegexMemory(); 
            init(); 
            delete rm; rm = 0; /* deleting null pointer is safe. */ 
            delete rr; rr = 0; 
            return *this; 
        } 

        Regex& resetErrors() { 
            error_number = 0; 
            error_offset = 0; 
            return *this; 
        } 

        String getPattern() { 
            return pat_str; 
        } 
        std::string getLocale() { 
            return mylocale; 
        } 

        std::string getModifier(); 

        Uint getPcre2Option() { 
            return compile_opts; 
        } 

        Uint getJpcre2Option() { 
            return jpcre2_compile_opts; 
        } 

        int getErrorNumber(){ 
            return error_number; 
        } 

        int getErrorOffset() { 
            return (int)error_offset; 
        } 
        String getErrorMessage() { 
            return select<S,Char>::getErrorMessage(error_number, error_offset); 
        } 
        Regex& setPattern(const String& re) { 
            pat_str = re; 
            return *this; 
        } 

        Regex& setModifier(const std::string& x) { 
            compile_opts = 0; 
            jpcre2_compile_opts = 0; 
            return changeModifier(x, true); 
        } 

        Regex& setLocale(const std::string& x) { 
            mylocale = x; 
            return *this; 
        } 
        Regex& setJpcre2Option(Uint x) { 
            jpcre2_compile_opts = x; 
            return *this; 
        } 

        Regex& setPcre2Option(Uint x) { 
            compile_opts = x; 
            return *this; 
        } 

        Regex& changeModifier(const std::string&, bool); 

        Regex& changeJpcre2Option(Uint opt, bool x) { 
            if(x) 
                jpcre2_compile_opts |= opt; 
            else 
                jpcre2_compile_opts &= ~opt; 
            return *this; 
        } 
     
        Regex& changePcre2Option(Uint opt, bool x) { 
            if(x) 
                compile_opts |= opt; 
            else 
                compile_opts &= ~opt; 
            return *this; 
        } 
        Regex& addModifier(const std::string& mod){ 
            return changeModifier(mod, true); 
        } 

        Regex& addJpcre2Option(Uint x) { 
            jpcre2_compile_opts |= x; 
            return *this; 
        } 

        Regex& addPcre2Option(Uint x) { 
            compile_opts |= x; 
            return *this; 
        } 

        void compile(void); 

        void compile(const String& re, Uint po, Uint jo) { 
            init(re, po, jo); 
            compile(); 
        } 
        void compile(const String& re, Uint po) { 
            init(re, po, 0); 
            compile(); 
        } 

        void compile(const String& re, const std::string& mod) { 
            init(re, mod); 
            compile(); 
        } 

        void compile(const String& re) { 
            init(re, 0, 0); 
            compile(); 
        } 
        SIZE_T match(const String& s, const std::string& mod) { 
            return getMatchObject().setSubject(s).setModifier(mod).match(); 
        } 

        SIZE_T match(const String& s) { 
            return getMatchObject().setSubject(s).match(); 
        } 

        RegexMatch& initMatch() { 
            delete rm; /* rm is either occupied or NULL, double deletion won't happen */ 
            rm = new RegexMatch(); 
            rm->re = this; 
            return *rm; 
        } 
      
        RegexMatch& getMatchObject(){ 
            if(rm != 0) return *rm; 
            else return initMatch(); 
        } 
        String replace(const String& mains, const String& repl, const std::string& mod) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 

        String replace(const String& mains, const String& repl) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).replace(); 
        } 

        RegexReplace& initReplace() { 
            delete rr; /* rr is either occupied or NULL, double deletion won't happen */ 
            rr = new RegexReplace(); 
            rr->re = this; 
            return *rr; 
        } 
        
        RegexReplace& getReplaceObject() { 
            if(rr != 0) return *rr; 
            else return initReplace(); 
        } 
    };
    
};//struct select

    
    ////////////////////////// Definitions //////////////////////////////////
    


}//jpcre2 namespace

    
template<int S, typename Char_T>
std::string jpcre2::select<S, Char_T>::Regex::getModifier(){
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



template<int S, typename Char_T>
std::string jpcre2::select<S, Char_T>::RegexMatch::getModifier(){
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

template<int S, typename Char_T>
std::string jpcre2::select<S, Char_T>::RegexReplace::getModifier(){
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



template<int S, typename Char_T>
void jpcre2::select<S, Char_T>::Regex::deepCopy(const Regex& r) {
	//Copy #code if it is non-null
	if (r.code) {
        ///First release memory of #code from current object if it is non-NULL
		freeRegexMemory();
		/// Copy compiled memory of #code to #code of current object using pcre2_code_copy() 
		code = Pcre2Func<S>::code_copy(r.code);
		/// Perform JIT compilation (if enabled) as pcre2_code_copy() doesn't copy JIT memory
		if ((jpcre2_compile_opts & JIT_COMPILE) != 0) {
			//Perform JIT compilation:
			int jit_ret = Pcre2Func<S>::jit_compile(code, PCRE2_JIT_COMPLETE);
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

template<int S, typename Char_T>
typename jpcre2::select<S, Char_T>::Regex& 
            jpcre2::select<S, Char_T>::Regex::
                changeModifier(const std::string& mod, bool x) {
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
        error_number = (int)ERROR::INVALID_MODIFIER;
        error_offset = (int)mod[i];
        
        endfor:;
	}
    return *this;
}




template<int S, typename Char_T>
void jpcre2::select<S, Char_T>::Regex::compile() {
	//Get c_str of pattern
	Pcre2Sptr c_pattern = (Pcre2Sptr) pat_str.c_str();

	/**************************************************************************
	 * Compile the regular expression pattern, and handle 
	 * any errors that are detected.                                          
	 *************************************************************************/

	CompileContext *ccontext = Pcre2Func<S>::compile_context_create(0);

	if (mylocale != LOCALE_NONE) {
		std::string loc_old;
		loc_old = std::setlocale(LC_CTYPE, mylocale.c_str());//mylocal needs to be std::string
		const unsigned char *tables = Pcre2Func<S>::maketables(0);
		Pcre2Func<S>::set_character_tables(ccontext, tables);
		std::setlocale(LC_CTYPE, (char*)loc_old.c_str());
	}

	code = Pcre2Func<S>::compile(c_pattern,     /* the pattern */
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
		int jit_ret = Pcre2Func<S>::jit_compile(code, PCRE2_JIT_COMPLETE);
		if (jit_ret != 0) {
            error_number = (int)ERROR::JIT_COMPILE_FAILED;
            error_offset = jit_ret;
            //Do not return, JIT error should be ignored.
		}
	}
    error_number = error_offset = 0;
}



//////////////////


// RegexReplace class


template<int S, typename Char_T>
typename jpcre2::select<S, Char_T>::RegexReplace& jpcre2::select<S, Char_T>::RegexReplace::resetErrors() {
    re->error_number = 0;
    re->error_offset = 0;
    return *this;
}



template<int S, typename Char_T>
typename jpcre2::select<S, Char_T>::RegexReplace&
            jpcre2::select<S, Char_T>::RegexReplace::
                changeModifier(const std::string& mod, bool x) {
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
        re->error_number = (int)ERROR::INVALID_MODIFIER;
        re->error_offset = (int)mod[i];
        
        endfor:;
	}
    return *this;
}



template<int S, typename Char_T>
typename jpcre2::select<S, Char_T>::String jpcre2::select<S, Char_T>::RegexReplace::replace() {

	// If code is null, return the subject string unmodified.
	if (re->code == 0)
		return r_subject;

	Pcre2Sptr subject = (Pcre2Sptr) r_subject.c_str();
	PCRE2_SIZE subject_length = r_subject.length() /* Strlen((Char *) subject) */;
	Pcre2Sptr replace = (Pcre2Sptr) r_replw.c_str();
	PCRE2_SIZE replace_length = r_replw.length() /* Strlen((Char *) replace) */;
	PCRE2_SIZE outlengthptr = (PCRE2_SIZE) buffer_size;
	int ret = 0;
	bool retry = true;
	Pcre2Uchar* output_buffer;
	output_buffer = (Pcre2Uchar*) malloc(outlengthptr * sizeof(Pcre2Uchar));

	while (true) {
		ret = Pcre2Func<S>::substitute(re->code,    /*Points to the compiled pattern*/
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
				output_buffer = (Pcre2Uchar*) realloc(output_buffer,
						outlengthptr * sizeof(Pcre2Uchar));
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



/////////////////

// RegexMatch class




template<int S, typename Char_T>
typename jpcre2::select<S, Char_T>::RegexMatch& jpcre2::select<S, Char_T>::RegexMatch::resetErrors() {
    re->error_number = 0;
    re->error_offset = 0;
    return *this;
}



template<int S, typename Char_T>
typename jpcre2::select<S, Char_T>::RegexMatch& jpcre2::select<S, Char_T>::RegexMatch::changeModifier(const std::string& mod, bool x) {
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
        re->error_number = (int)ERROR::INVALID_MODIFIER;
        re->error_offset = (int)mod[i];
        
        endfor:;
	}
    return *this;
}


template<int S, typename Char_T>
bool jpcre2::select<S, Char_T>::RegexMatch::getNumberedSubstrings(int rc, MatchData *match_data) {
    String value;
    PCRE2_SIZE bufflen = 0;
    Pcre2Uchar *buffer = 0;
    int ret = 0;
    
	for (int i = 0; i < rc; i++) {
		//If we use pcre2_substring_get_bynumber(),
		//we will have to deal with returned error codes and memory
		ret = Pcre2Func<S>::substring_get_bynumber(match_data, (Uint) i, &buffer,
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
		value = toString((Char*) buffer);
		Pcre2Func<S>::substring_free(buffer);     //must free memory
        //::free(buffer);
        buffer = 0; //we are going to use it again.
		//if (num_sub)   //This null check is paranoid, this function shouldn't be called if this vector is null
        num_sub->push_back(value); 
	}
    return true;
}



template<int S, typename Char_T>
bool jpcre2::select<S, Char_T>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size,
                                                            Pcre2Sptr tabptr, MatchData *match_data) {

    String key, value, value1;
    PCRE2_SIZE bufflen = 0;
    Pcre2Uchar *buffer = 0;
    int ret = 0;
    
	for (int i = 0; i < namecount; i++) {
        int n;
        if(S==8){
            n = (int)((tabptr[0] << 8) | tabptr[1]);
            key = toString((Char*) (tabptr + 2));
        }
        else{
            n = (int)tabptr[0];
            key = toString((Char*) (tabptr + 1));
        }
        //Use of tabptr is finished for this iteration, let's increment it now.
        tabptr += name_entry_size;
        
		ret = Pcre2Func<S>::substring_get_byname(match_data,
				(Pcre2Sptr) key.c_str(), &buffer, &bufflen);
		if (ret < 0) {
			switch (ret) {
			case PCRE2_ERROR_NOMEMORY:
				re->error_number = re->error_offset = ret;
                return false;
			default:
				break;   ///Errors other than PCRE2_ERROR_NOMEMORY error are ignored
			}
		}
		value = toString((Char *) buffer);
		Pcre2Func<S>::substring_free(buffer);     //must free memory
        //::free(buffer);
        buffer = 0; //we may use this pointer again, better initialize it.
        

        if(ntn_map) {
            //Let's get the value again, this time with number
            //We will match this value with the previous one.
            //If they match, we got the right one.
            //Otherwise the number is not valid for the corresponding name and
            //we will skip this iteration.
            
            //Don't use pcre2_substring_number_from_name() to get the number for the name (It's messy with dupnames).
            ret = Pcre2Func<S>::substring_get_bynumber(match_data, (Uint) n, &buffer,
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
            value1 = toString((Char *) buffer);
            Pcre2Func<S>::substring_free(buffer);     //must free memory
            //::free(buffer);
            buffer = 0;
            
            if (value != value1) continue;
            
            (*ntn_map)[key] = n; //this is inside ntn_map null check
        }
        
        if (nas_map)
            (*nas_map)[key] = value;  //must check for null
            //The above assignment will execute multiple times for same value with same key when there are dupnames and
            //ntn_map is null, therefore keeping it below ntn_map will be more efficient.
            //In this way, when ntn_map is non-Null, it will execute only once for all dupnames.
            
            
	}
    return true;
}



template<int S, typename Char_T>
jpcre2::SIZE_T jpcre2::select<S, Char_T>::RegexMatch::match() {

	/// If Regex::code is null, return 0 as the match count
	if (re->code == 0)
		return 0;

	Pcre2Sptr subject = (Pcre2Sptr) m_subject.c_str();
	Pcre2Sptr name_table;
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
	MatchData *match_data;
	subject_length = m_subject.length();
    

	/// Clear all (passed) vectors and initialize associated maps
    /// No memory will be allocated for a map if its associated vector is't passed.
	if (vec_num) {
		vec_num->clear();
        delete num_sub;
		num_sub = new NumSub();
	}
	if (vec_nas) {
		vec_nas->clear();
        delete nas_map;
		nas_map = new MapNas();
	}
	if (vec_ntn) {
		vec_ntn->clear();
        delete ntn_map;
		ntn_map = new MapNtN();
	}

	/* Using this function ensures that the block is exactly the right size for
	 the number of capturing parentheses in the pattern. */

	match_data = Pcre2Func<S>::match_data_create_from_pattern(re->code, 0);

	rc = Pcre2Func<S>::match(re->code,  /* the compiled pattern */
                subject,        /* the subject string */
                subject_length, /* the length of the subject */
                0,              /* start at offset 0 in the subject */
                match_opts,     /* default options */
                match_data,     /* block for storing the result */
                0);             /* use default match context */

	/* Matching failed: handle error cases */

	if (rc < 0) {
		Pcre2Func<S>::match_data_free(match_data); /* Release memory used for the match */
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
	ovector = Pcre2Func<S>::get_ovector_pointer(match_data);

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

	/// Get numbered substrings if #num_sub isn't null
	if (num_sub) { //must do null check
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

	(void) Pcre2Func<S>::pattern_info(re->code, /* the compiled pattern */
	PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
	&namecount); /* where to put the answer */

	if (namecount <= 0)
		; /*No named substrings*/

	else {
		Pcre2Sptr tabptr;

		/* Before we can access the substrings, we must extract the table for
		 translating names to numbers, and the size of each entry in the table. */

		(void) Pcre2Func<S>::pattern_info(re->code, /* the compiled pattern */
		PCRE2_INFO_NAMETABLE, /* address of the table */
		&name_table); /* where to put the answer */

		(void) Pcre2Func<S>::pattern_info(re->code, /* the compiled pattern */
		PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
		&name_entry_size); /* where to put the answer */

		/* Now we can scan the table and, for each entry, print the number, the name,
		 and the substring itself. In the 8-bit library the number is held in two
		 bytes, most significant first. */

		tabptr = name_table;

		/// Get named substrings if #nas_map isn't null.
        /// Get name to number map if #ntn_map isn't null.
		if (nas_map || ntn_map) {
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
		Pcre2Func<S>::match_data_free(match_data); /* Release the memory that was used */
		//Pcre2Func<S>::code_free(re);                  /// Don't do this. This function has no right to modify regex.
		return count; /* Exit the program. */
	}

	/* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
	 sequence. First, find the options with which the regex was compiled and extract
	 the UTF state. */

	(void) Pcre2Func<S>::pattern_info(re->code, PCRE2_INFO_ALLOPTIONS, &option_bits);
	utf = (option_bits & PCRE2_UTF) != 0;

	/* Now find the newline convention and see whether CRLF is a valid newline
	 sequence. */

	(void) Pcre2Func<S>::pattern_info(re->code, PCRE2_INFO_NEWLINE, &newline);
	crlf_is_newline = newline == PCRE2_NEWLINE_ANY
			|| newline == PCRE2_NEWLINE_CRLF
			|| newline == PCRE2_NEWLINE_ANYCRLF;

	/** We got the first match. Now loop for second and subsequent matches. */

	for (;;) {
		/// Clear maps before filling it with new values
		if (num_sub)
			num_sub->clear();
		if (nas_map)
			nas_map->clear();
		if (ntn_map)
			ntn_map->clear();

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

		rc = Pcre2Func<S>::match(re->code,  /* the compiled pattern */
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
                    if(S == 8 && (subject[ovector[1]] & 0xc0) != 0x80) break;
                    else if(S == 16 && (subject[ovector[1]] & 0xfc00) != 0xdc00) break;
                    else if(S == 32) break;
					ovector[1] += 1;
				}
			}
			continue; /* Go round the loop again */
		}

		/* Other matching errors are not recoverable. */

		if (rc < 0) {
			Pcre2Func<S>::match_data_free(match_data);
			//Pcre2Func<S>::code_free(code);           //must not do this. This function has no right to modify regex.
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

		/// Get numbered substrings if #num_sub isn't null
		if (num_sub) { //must do null check
			if(!getNumberedSubstrings(rc, match_data))
                return count;
        }

		if (namecount <= 0)
			; /*No named substrings*/
		else {
			Pcre2Sptr tabptr = name_table;

            /// Get named substrings if #nas_map isn't null.
            /// Get name to number map if #ntn_map isn't null.
			if (nas_map || ntn_map) {
				if(!getNamedSubstrings(namecount, name_entry_size, tabptr, match_data))
                    return count;
            }
		}

		/// Populate vectors with their associated maps.
		pushMapsIntoVectors();

	} /* End of loop to find second and subsequent matches */

	Pcre2Func<S>::match_data_free(match_data);
	// Must not free code. This function has no right to modify regex.
	return count;
}



//some macro documentation for doxygen

#ifdef __DOXYGEN__
/**@def PCRE2_CODE_UNIT_WIDTH
 * This macro must be defined to either 0, 8, 16 or 32 before including jpcre2.hpp.
 * 0 means you will be using multiple code unit width in your program.
 * 
 * The code unit width must match with the bit size of the character type you are going to use.
 */
 #define PCRE2_CODE_UNIT_WIDTH
#endif


#endif
