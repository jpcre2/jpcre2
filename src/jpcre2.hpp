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

#ifndef PCRE2_CODE_UNIT_WIDTH

///@def PCRE2_CODE_UNIT_WIDTH
///This macro does not have any significance in JPCRE2 context.
///It is defined as 0 by default. Defining it before including jpcre2.hpp
///will override the default (0), but still it will have no effect in a 
///JPCRE2 perspective.
///Defining it with an invalid value will yield to compile error.
#define PCRE2_CODE_UNIT_WIDTH 0
#endif

#include <pcre2.h>      // pcre2 header
#include <string>       // std::string, std::wstring
#include <vector>       // std::vector
#include <map>          // std::map
#include <cstdio>       // snprintf
#include <cwchar>       // std::mbstate_t
#include <cstring>      // strlen
#include <clocale>      // std::setlocale
#include <climits>      // CHAR_BIT


#if __cplusplus >= 201103L
    //These will be included if >=C++11 is used
    #include <codecvt>
    #include <locale>  // std::wstring_convert
#endif

#ifdef JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION
    template<bool B, class T = void>
    struct Validate_Code_Unit_Width { typedef T ValidChar; };
#else
    template<bool B, class T=void>
    struct Validate_Code_Unit_Width {};
     
    template<class T>
    struct Validate_Code_Unit_Width<true, T> { typedef T ValidChar; };
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


/** @namespace jpcre2::INFO
 *  Namespace to provide information about JPCRE2 library itself.
 *  Contains constant Strings with version info.
 */
namespace INFO {
    static const char NAME[] = "JPCRE2";               ///< Name of the project
    static const char FULL_VERSION[] = "10.28.06";     ///< Full version string
    static const char VERSION_GENRE[] = "10";          ///< Generation, depends on original PCRE2 version
    static const char VERSION_MAJOR[] = "28";          ///< Major version, updated when API change is made
    static const char VERSION_MINOR[] = "06";          ///< Minor version, includes bug fix or minor feature upgrade
    static const char VERSION_PRE_RELEASE[] = "";      ///< Alpha or beta (testing) release version
}


typedef PCRE2_SIZE SIZE_T;                          ///< Used for match count and vector size
typedef uint32_t Uint;                              ///< Used for options (bitwise operation)
typedef uint8_t Ush;                                ///< 8 bit unsigned integer.

/// @namespace jpcre2::ERROR
/// Namespace for error codes.
namespace ERROR {
    /** Error numbers for JPCRE2.
     *  JPCRE2 error numbers are positive integers while
     *  PCRE2 error numbers are negative integers.
     */
    enum {
        INVALID_MODIFIER        = 2                ///< Error number implying that invalid modifier was detected
    };
}


/** These constants provide JPCRE2 options.
 */
enum {
    NONE                    = 0x0000000u,           ///< Option 0 (zero)
    FIND_ALL                = 0x0000002u,           ///< Find all during match (global match)
    JIT_COMPILE             = 0x0000004u            ///< Perform JIT compilation for optimization
};

/// Used by default to provide big enough initial buffer for replaced string.
/// 0 or insufficient values will force a second call to pcre2_substitute(),
/// If that is not desirable, pass a big enough buffer size with
/// RegexReplace::setBufferSize() function to make it a single call operation.
static const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE = 0;  


#if __cplusplus >= 201103L
///@struct Codecvt
///Convenience wrapper of `std::codecvt`.
///Only available for >=C++11.
template <class internT, class externT, class stateT>
struct Codecvt : std::codecvt<internT,externT,stateT>
{ ~Codecvt(){} };

///This is a convenience typedef (>=C++11) to convert between UTF-8 <> UTF-16.
///Convert UTF-16 to UTF-8
///```cpp
///Convert16 conv;
///std::string s = conv.to_bytes(utf16string);
///```
///Convert UTF-8 to UTF-16
///```cpp
///std::u16string us = conv.from_bytes(utf8string);
///```
typedef std::wstring_convert<Codecvt<char16_t, char, std::mbstate_t>,char16_t> Convert16;

///This is a convenience typedef (>=C++11) to convert between UTF-8 <> UTF-32.
///Convert UTF-32 to UTF-8
///```cpp
///Convert32 conv;
///std::string s = conv.to_bytes(utf32string);
///```
///Convert UTF-8 to UTF-32
///```cpp
///std::u32string us = conv.from_bytes(utf8string);
///```
typedef std::wstring_convert<Codecvt<char32_t, char, std::mbstate_t>,char32_t> Convert32;

#endif


////////////////////////// The following are type and function mappings from PCRE2 interface to JPCRE2 interface /////////////////////////

//forward decalration

template<Ush BS> struct Pcre2Type;
template<Ush BS> struct Pcre2Func;

//PCRE2 types
//These templated types will be used in place of actual types
template<Ush BS> struct Pcre2Type {};

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

//wrappers for PCRE2 functions
template<Ush BS> struct Pcre2Func{};

//8-bit version
template<> struct Pcre2Func<8> {
    static typename Pcre2Type<8>::CompileContext* compile_context_create(typename Pcre2Type<8>::GeneralContext *gcontext){
        return pcre2_compile_context_create_8(gcontext);
    }
    static void compile_context_free(typename Pcre2Type<8>::CompileContext *ccontext){
        pcre2_compile_context_free_8(ccontext);
    }
    static const unsigned char * maketables(typename Pcre2Type<8>::GeneralContext* gcontext){
        return pcre2_maketables_8(gcontext);
    }
    static int set_character_tables(typename Pcre2Type<8>::CompileContext * ccontext, const unsigned char * table){
        return pcre2_set_character_tables_8(ccontext, table);
    }
    static typename Pcre2Type<8>::Pcre2Code * compile( typename Pcre2Type<8>::Pcre2Sptr pattern,
                                     PCRE2_SIZE length,
                                     uint32_t options,
                                     int *errorcode,
                                     PCRE2_SIZE *erroroffset,
                                     typename Pcre2Type<8>::CompileContext *ccontext){
        return pcre2_compile_8(pattern, length, options, errorcode, erroroffset, ccontext);
    }
    static int jit_compile(typename Pcre2Type<8>::Pcre2Code *code, uint32_t options){
        return pcre2_jit_compile_8(code, options);
    }
    static int substitute( const typename Pcre2Type<8>::Pcre2Code *code,
                    typename Pcre2Type<8>::Pcre2Sptr subject, 
                    PCRE2_SIZE length, 
                    PCRE2_SIZE startoffset, 
                    uint32_t options, 
                    typename Pcre2Type<8>::MatchData *match_data, 
                    typename Pcre2Type<8>::MatchContext *mcontext, 
                    typename Pcre2Type<8>::Pcre2Sptr replacement, 
                    PCRE2_SIZE rlength, 
                    typename Pcre2Type<8>::Pcre2Uchar *outputbuffer, 
                    PCRE2_SIZE *outlengthptr){
        return pcre2_substitute_8( code, subject, length, startoffset, options, match_data,
                                   mcontext, replacement, rlength, outputbuffer, outlengthptr);
    } 
    static int substring_get_bynumber(typename Pcre2Type<8>::MatchData *match_data,
                                        uint32_t number, 
                                        typename Pcre2Type<8>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen){
        return pcre2_substring_get_bynumber_8(match_data, number, bufferptr, bufflen);
    }
    static void substring_free(typename Pcre2Type<8>::Pcre2Uchar *buffer){
        pcre2_substring_free_8(buffer);
    }
    static typename Pcre2Type<8>::Pcre2Code * code_copy(const typename Pcre2Type<8>::Pcre2Code *code){
        return pcre2_code_copy_8(code);
    }
    static void code_free(typename Pcre2Type<8>::Pcre2Code *code){
        pcre2_code_free_8(code);
    }
    static int get_error_message(  int errorcode,
                            typename Pcre2Type<8>::Pcre2Uchar *buffer,
                            PCRE2_SIZE bufflen){
        return pcre2_get_error_message_8(errorcode, buffer, bufflen);
    }
    static int substring_get_byname(typename Pcre2Type<8>::MatchData *match_data,
                                        typename Pcre2Type<8>::Pcre2Sptr name, 
                                        typename Pcre2Type<8>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen){
        return pcre2_substring_get_byname_8(match_data, name, bufferptr, bufflen);
    }
    static typename Pcre2Type<8>::MatchData * match_data_create_from_pattern(
                              const typename Pcre2Type<8>::Pcre2Code *code,
                              typename Pcre2Type<8>::GeneralContext *gcontext){
        return pcre2_match_data_create_from_pattern_8(code, gcontext);
                                
    }
    static int match(  const typename Pcre2Type<8>::Pcre2Code *code, 
                            typename Pcre2Type<8>::Pcre2Sptr subject, 
                            PCRE2_SIZE length, 
                            PCRE2_SIZE startoffset, 
                            uint32_t options, 
                            typename Pcre2Type<8>::MatchData *match_data, 
                            typename Pcre2Type<8>::MatchContext *mcontext){
        return pcre2_match_8(code, subject, length, startoffset, options, match_data, mcontext);
    }
    static void match_data_free(typename Pcre2Type<8>::MatchData *match_data){
        pcre2_match_data_free_8(match_data);
    }
    static PCRE2_SIZE * get_ovector_pointer(typename Pcre2Type<8>::MatchData *match_data){
        return pcre2_get_ovector_pointer_8(match_data);
    }
    static int pattern_info(const typename Pcre2Type<8>::Pcre2Code *code, uint32_t what, void *where){
        return pcre2_pattern_info_8(code, what, where);
    }
    static int set_newline(typename Pcre2Type<8>::CompileContext *ccontext, uint32_t value){
        return pcre2_set_newline_8(ccontext, value); 
    }
};

//16-bit version
template<> struct Pcre2Func<16> {
    static typename Pcre2Type<16>::CompileContext* compile_context_create(typename Pcre2Type<16>::GeneralContext *gcontext){
        return pcre2_compile_context_create_16(gcontext);
    }
    static void compile_context_free(typename Pcre2Type<16>::CompileContext *ccontext){
        pcre2_compile_context_free_16(ccontext);
    }
    static const unsigned char * maketables(typename Pcre2Type<16>::GeneralContext* gcontext){
        return pcre2_maketables_16(gcontext);
    }
    static int set_character_tables(typename Pcre2Type<16>::CompileContext * ccontext, const unsigned char * table){
        return pcre2_set_character_tables_16(ccontext, table);
    }
    static typename Pcre2Type<16>::Pcre2Code * compile( typename Pcre2Type<16>::Pcre2Sptr pattern,
                                     PCRE2_SIZE length,
                                     uint32_t options,
                                     int *errorcode,
                                     PCRE2_SIZE *erroroffset,
                                     typename Pcre2Type<16>::CompileContext *ccontext){
        return pcre2_compile_16(pattern, length, options, errorcode, erroroffset, ccontext);
    }
    static int jit_compile(typename Pcre2Type<16>::Pcre2Code *code, uint32_t options){
        return pcre2_jit_compile_16(code, options);
    }
    static int substitute( const typename Pcre2Type<16>::Pcre2Code *code,
                    typename Pcre2Type<16>::Pcre2Sptr subject, 
                    PCRE2_SIZE length, 
                    PCRE2_SIZE startoffset, 
                    uint32_t options, 
                    typename Pcre2Type<16>::MatchData *match_data, 
                    typename Pcre2Type<16>::MatchContext *mcontext, 
                    typename Pcre2Type<16>::Pcre2Sptr replacement, 
                    PCRE2_SIZE rlength, 
                    typename Pcre2Type<16>::Pcre2Uchar *outputbuffer, 
                    PCRE2_SIZE *outlengthptr){
        return pcre2_substitute_16( code, subject, length, startoffset, options, match_data,
                                   mcontext, replacement, rlength, outputbuffer, outlengthptr);
    } 
    static int substring_get_bynumber(typename Pcre2Type<16>::MatchData *match_data,
                                        uint32_t number, 
                                        typename Pcre2Type<16>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen){
        return pcre2_substring_get_bynumber_16(match_data, number, bufferptr, bufflen);
    }
    static void substring_free(typename Pcre2Type<16>::Pcre2Uchar *buffer){
        pcre2_substring_free_16(buffer);
    }
    static typename Pcre2Type<16>::Pcre2Code * code_copy(const typename Pcre2Type<16>::Pcre2Code *code){
        return pcre2_code_copy_16(code);
    }
    static void code_free(typename Pcre2Type<16>::Pcre2Code *code){
        pcre2_code_free_16(code);
    }
    static int get_error_message(  int errorcode,
                            typename Pcre2Type<16>::Pcre2Uchar *buffer,
                            PCRE2_SIZE bufflen){
        return pcre2_get_error_message_16(errorcode, buffer, bufflen);
    }
    static int substring_get_byname(typename Pcre2Type<16>::MatchData *match_data,
                                        typename Pcre2Type<16>::Pcre2Sptr name, 
                                        typename Pcre2Type<16>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen){
        return pcre2_substring_get_byname_16(match_data, name, bufferptr, bufflen);
    }
    static typename Pcre2Type<16>::MatchData * match_data_create_from_pattern(
                              const typename Pcre2Type<16>::Pcre2Code *code,
                              typename Pcre2Type<16>::GeneralContext *gcontext){
        return pcre2_match_data_create_from_pattern_16(code, gcontext);
                                
    }
    static int match(  const typename Pcre2Type<16>::Pcre2Code *code, 
                            typename Pcre2Type<16>::Pcre2Sptr subject, 
                            PCRE2_SIZE length, 
                            PCRE2_SIZE startoffset, 
                            uint32_t options, 
                            typename Pcre2Type<16>::MatchData *match_data, 
                            typename Pcre2Type<16>::MatchContext *mcontext){
        return pcre2_match_16(code, subject, length, startoffset, options, match_data, mcontext);
    }
    static void match_data_free(typename Pcre2Type<16>::MatchData *match_data){
        pcre2_match_data_free_16(match_data);
    }
    static PCRE2_SIZE * get_ovector_pointer(typename Pcre2Type<16>::MatchData *match_data){
        return pcre2_get_ovector_pointer_16(match_data);
    }
    static int pattern_info(const typename Pcre2Type<16>::Pcre2Code *code, uint32_t what, void *where){
        return pcre2_pattern_info_16(code, what, where);
    }
    static int set_newline(typename Pcre2Type<16>::CompileContext *ccontext, uint32_t value){
        return pcre2_set_newline_16(ccontext, value); 
    }
};

//32-bit version
template<> struct Pcre2Func<32> {
    static typename Pcre2Type<32>::CompileContext* compile_context_create(typename Pcre2Type<32>::GeneralContext *gcontext){
        return pcre2_compile_context_create_32(gcontext);
    }
    static void compile_context_free(typename Pcre2Type<32>::CompileContext *ccontext){
        pcre2_compile_context_free_32(ccontext);
    }
    static const unsigned char * maketables(typename Pcre2Type<32>::GeneralContext* gcontext){
        return pcre2_maketables_32(gcontext);
    }
    static int set_character_tables(typename Pcre2Type<32>::CompileContext * ccontext, const unsigned char * table){
        return pcre2_set_character_tables_32(ccontext, table);
    }
    static typename Pcre2Type<32>::Pcre2Code * compile( typename Pcre2Type<32>::Pcre2Sptr pattern,
                                     PCRE2_SIZE length,
                                     uint32_t options,
                                     int *errorcode,
                                     PCRE2_SIZE *erroroffset,
                                     typename Pcre2Type<32>::CompileContext *ccontext){
        return pcre2_compile_32(pattern, length, options, errorcode, erroroffset, ccontext);
    }
    static int jit_compile(typename Pcre2Type<32>::Pcre2Code *code, uint32_t options){
        return pcre2_jit_compile_32(code, options);
    }
    static int substitute( const typename Pcre2Type<32>::Pcre2Code *code,
                    typename Pcre2Type<32>::Pcre2Sptr subject, 
                    PCRE2_SIZE length, 
                    PCRE2_SIZE startoffset, 
                    uint32_t options, 
                    typename Pcre2Type<32>::MatchData *match_data, 
                    typename Pcre2Type<32>::MatchContext *mcontext, 
                    typename Pcre2Type<32>::Pcre2Sptr replacement, 
                    PCRE2_SIZE rlength, 
                    typename Pcre2Type<32>::Pcre2Uchar *outputbuffer, 
                    PCRE2_SIZE *outlengthptr){
        return pcre2_substitute_32( code, subject, length, startoffset, options, match_data,
                                   mcontext, replacement, rlength, outputbuffer, outlengthptr);
    } 
    static int substring_get_bynumber(typename Pcre2Type<32>::MatchData *match_data,
                                        uint32_t number, 
                                        typename Pcre2Type<32>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen){
        return pcre2_substring_get_bynumber_32(match_data, number, bufferptr, bufflen);
    }
    static void substring_free(typename Pcre2Type<32>::Pcre2Uchar *buffer){
        pcre2_substring_free_32(buffer);
    }
    static typename Pcre2Type<32>::Pcre2Code * code_copy(const typename Pcre2Type<32>::Pcre2Code *code){
        return pcre2_code_copy_32(code);
    }
    static void code_free(typename Pcre2Type<32>::Pcre2Code *code){
        pcre2_code_free_32(code);
    }
    static int get_error_message(  int errorcode,
                            typename Pcre2Type<32>::Pcre2Uchar *buffer,
                            PCRE2_SIZE bufflen){
        return pcre2_get_error_message_32(errorcode, buffer, bufflen);
    }
    static int substring_get_byname(typename Pcre2Type<32>::MatchData *match_data,
                                        typename Pcre2Type<32>::Pcre2Sptr name, 
                                        typename Pcre2Type<32>::Pcre2Uchar **bufferptr, 
                                        PCRE2_SIZE *bufflen){
        return pcre2_substring_get_byname_32(match_data, name, bufferptr, bufflen);
    }
    static typename Pcre2Type<32>::MatchData * match_data_create_from_pattern(
                              const typename Pcre2Type<32>::Pcre2Code *code,
                              typename Pcre2Type<32>::GeneralContext *gcontext){
        return pcre2_match_data_create_from_pattern_32(code, gcontext);
                                
    }
    static int match(  const typename Pcre2Type<32>::Pcre2Code *code, 
                            typename Pcre2Type<32>::Pcre2Sptr subject, 
                            PCRE2_SIZE length, 
                            PCRE2_SIZE startoffset, 
                            uint32_t options, 
                            typename Pcre2Type<32>::MatchData *match_data, 
                            typename Pcre2Type<32>::MatchContext *mcontext){
        return pcre2_match_32(code, subject, length, startoffset, options, match_data, mcontext);
    }
    static void match_data_free(typename Pcre2Type<32>::MatchData *match_data){
        pcre2_match_data_free_32(match_data);
    }
    static PCRE2_SIZE * get_ovector_pointer(typename Pcre2Type<32>::MatchData *match_data){
        return pcre2_get_ovector_pointer_32(match_data);
    }
    static int pattern_info(const typename Pcre2Type<32>::Pcre2Code *code, uint32_t what, void *where){
        return pcre2_pattern_info_32(code, what, where);
    }
    static int set_newline(typename Pcre2Type<32>::CompileContext *ccontext, uint32_t value){
        return pcre2_set_newline_32(ccontext, value); 
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Namespace for modifier constants.
// For each modifier constant there is a jpcre2::Uint option value.
// Some modifiers may have multiple values set together (ORed in bitwise operation) and
// thus they may include other modifiers. Such an example is the 'n' modifier. It is combined together with 'u'.
namespace MOD {

    // Define modifiers for compile
    // String of compile modifier characters for PCRE2 options
    static const char C_N[] = "eijmnsuxADJU";
    // Array of compile modifier values for PCRE2 options
    static const jpcre2::Uint C_V[12] = {   PCRE2_MATCH_UNSET_BACKREF,                  // Modifier e
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
    static const char CJ_N[] = "S";
    // Array of compile modifier values for JPCRE2 options
    static const jpcre2::Uint CJ_V[1] = { JIT_COMPILE,                                // Modifier S
                                              };


    // Define modifiers for replace
    // String of action (replace) modifier characters for PCRE2 options
    static const char R_N[] = "eEgx";
    // Array of action (replace) modifier values for PCRE2 options
    static const jpcre2::Uint R_V[4] = {   PCRE2_SUBSTITUTE_UNSET_EMPTY,                // Modifier  e
                                           PCRE2_SUBSTITUTE_UNKNOWN_UNSET | PCRE2_SUBSTITUTE_UNSET_EMPTY,   // Modifier E (includes e)
                                           PCRE2_SUBSTITUTE_GLOBAL,                     // Modifier g
                                           PCRE2_SUBSTITUTE_EXTENDED                    // Modifier x
                                 };


    // String of action (replace) modifier characters for JPCRE2 options
    static const char RJ_N[] = "";
    // Array of action (replace) modifier values for JPCRE2 options
    static const jpcre2::Uint RJ_V[1] = { NONE
                                              };
    //Explicit

    // Define modifiers for match
    // String of action (match) modifier characters for PCRE2 options
    static const char M_N[] = "A";
    // Array of action (match) modifier values for PCRE2 options
    static const jpcre2::Uint M_V[1] = { PCRE2_ANCHORED                               // Modifier  A
                                             };


    // String of action (match) modifier characters for JPCRE2 options
    static const char MJ_N[] = "g";
    // Array of action (match) modifier values for JPCRE2 options
    static const jpcre2::Uint MJ_V[1] = { FIND_ALL,                                   // Modifier  g
                                              };

}

//These message strings are used for error/warning message construction.
//take care to prevent multiple definition
template<typename Char_T> struct MSG{
    static std::basic_string<Char_T> INVALID_MODIFIER(void);
};
//specialization
template<> inline std::basic_string<char> MSG<char>::INVALID_MODIFIER(){ return "Invalid modifier: "; }
template<> inline std::basic_string<wchar_t> MSG<wchar_t>::INVALID_MODIFIER(){ return L"Invalid modifier: "; }
#if __cplusplus >= 201103L
template<> inline std::basic_string<char16_t> MSG<char16_t>::INVALID_MODIFIER(){ return u"Invalid modifier: "; }
template<> inline std::basic_string<char32_t> MSG<char32_t>::INVALID_MODIFIER(){ return U"Invalid modifier: "; }
#endif


///@struct ParseInt
///Contains function that parses integer.
template<typename Char_T> struct ParseInt{
    static std::basic_string<Char_T> toString(int x);
};

///Converts integer to std::string
///@param x the integer to convert
///@return std::string from the integer
template<> inline std::string ParseInt<char>::toString(int x){
    int length = snprintf(0, 0, "%d", x);
    if(length <= 0) return std::string();
    char* buf = new char[length + 1];
    snprintf(buf, length +1, "%d", x);
    std::string str(buf);
    delete[] buf;
    return str;
}

///@overload
///
///
///Converts integer to std::wstring
///@param x the integer to convert
///@return std::wstring from the integer
template<> inline std::wstring ParseInt<wchar_t>::toString(int x){
    int length = 2; //most ints coming here are 2 digits long.
    wchar_t* buf = 0;
    do{
        delete[] buf;
        buf = new wchar_t[++length + 1];
    }while(swprintf(buf, length + 1, L"%d", x) < 0);
    std::wstring str(buf);
    delete[] buf;
    return str;
}
    
#if __cplusplus >= 201103L

///@overload
///
///
///Converts integer to std::u16string.
///Uses codecvt to convert to utf16 from utf8
///@param x int to convert
///@return std::u16string from the integer
template<> inline std::u16string ParseInt<char16_t>::toString(int x) {
    std::string s = std::to_string(x);
    Convert16 conv;
    std::u16string us = conv.from_bytes(s);
    return us;
}

///@overload
///
///
///Converts integer to std::u32string.
///Uses codecvt to convert to utf32 from utf8
///@param x int to convert
///@return std::u32string from the integer
template<> inline std::u32string ParseInt<char32_t>::toString(int x) {
    std::string s = std::to_string(x);
    Convert32 conv;
    std::u32string us = conv.from_bytes(s);
    return us;
}
#endif


///struct to select code unit width and the character type. 
///
///Usage: `jpcre2::select<Char_T, BS>`.
///
///The character type (`Char_T`) must be in accordance with its' code unit width (BS).
///If it doesn't match with the character type, compile error will be produced if not suppressed by
///defining the macro `JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION`.
///@tparam BS Code unit width (8, 16 or 32)
///@tparam Char_T Character type (`char`, `wchar_t`, `char16_t`, `char32_t`)
///
///If BS is not given, i.e it is called like
///```cpp
///jpcre2::select<Char_T>
///```
///then `sizeof(Char_T)*CHAR_BIT` will be taken as the value for BS.
///
///It is possible to write portable code by using `jpcre2::select<Char_T>`
///i.e by not defining the bit size explicitly.
///
///If you want to fix the code unit width for a character type, pass
///the value as the second templated parameter for the selector. e.g:
///```cpp
///jpcre2::select<char, 8>::Regex re;
///```
template<typename Char_T, Ush BS = sizeof( Char_T ) * CHAR_BIT> 
struct select{
    virtual ~select(){}    //allow subclassing

    ///Typedef for character (either one of `char`, `wchar_t`, `char16_t`, `char32_t`)
    typedef typename Validate_Code_Unit_Width<BS == sizeof( Char_T ) * CHAR_BIT, Char_T>::ValidChar Char;
    
    //typedef Char_T Char;
    ///Typedef for string (either one of `std::string`, `std::wstring`, `std::u16string`, `std::u32string`).
    ///Defined as `std::basic_string<Char_T>`.
    ///May be this list will make more sense:
    ///Character  | String
    ///---------  | -------
    ///char | std::string
    ///wchar_t | std::wstring
    ///char16_t | std::u16string (>=C++11)
    ///char32_t | std::u32string (>=C++11) 
    typedef typename std::basic_string<Char_T> String;
    
    ///Map for Named substrings.
    typedef typename std::map<String, String> MapNas;
    ///Vector for Numbered substrings (Sub container).
    typedef typename std::vector<String> NumSub;     
    ///Substring name to Substring number map.
    typedef typename std::map<String, SIZE_T> MapNtN;
    ///Allow spelling mistake of MapNtN as MapNtn.
    typedef MapNtN MapNtn;
    ///Vector of matches with named substrings.
    typedef typename std::vector<MapNas> VecNas;   
    ///Vector of substring name to substring number map.       
    typedef typename std::vector<MapNtN> VecNtN;
    ///Allow spelling mistake of VecNtN as VecNtn.            
    typedef VecNtN VecNtn;
    ///Vector of matches with numbered substrings.  
    typedef typename std::vector<NumSub> VecNum;         
    
    //This is to shorten the code
    typedef typename Pcre2Type<BS>::Pcre2Uchar Pcre2Uchar;
    typedef typename Pcre2Type<BS>::Pcre2Sptr Pcre2Sptr;
    typedef typename Pcre2Type<BS>::Pcre2Code Pcre2Code;
    typedef typename Pcre2Type<BS>::CompileContext CompileContext;
    typedef typename Pcre2Type<BS>::MatchData MatchData;
    typedef typename Pcre2Type<BS>::GeneralContext GeneralContext;
    typedef typename Pcre2Type<BS>::MatchContext MatchContext; 
    
    ///Converts a Char_T (char, wchar_t, char16_t, char32_t) to jpcre2::select::String
    ///@param a Char_T
    ///@return jpcre2::select::String
    static String toString(Char a){
        if (a) return String(1, a);
        else return String();
    }                     
    
    ///@overload
    ///
    ///
    ///Converts a Char_T* (char*, wchar_t*, char16_t*, char32_t*) to jpcre2::select::String
    ///@param a const Char_T*
    ///@return jpcre2::select::String
    static String toString(const Char* a){
        if (a) return String(a);
        else return String();
    }              
    
    ///@overload
    ///
    ///
    ///Converts a PCRE2_UCHAR to String
    ///@param a PCRE2_UCHAR
    ///@return jpcre2::select::String
    static String toString(Pcre2Uchar* a) {
        if (a) return String((Char*) a);
        else return String();
    }
    
    ///Retruns error message from PCRE2 error number
    ///@param err_num error number (negative)
    ///@return message as jpcre2::select::String.
    static String getPcre2ErrorMessage(int err_num){
        Pcre2Uchar buffer[8048];
        Pcre2Func<BS>::get_error_message(err_num, buffer, sizeof(buffer));
        return toString((Pcre2Uchar*) buffer);
    }          
    
    ///Retruns error message (either JPCRE2 or PCRE2) from error number and error offset
    ///@param err_num error number (negative for PCRE2, positive for JPCRE2)
    ///@param err_off error offset
    ///@return message as jpcre2::select::String.
    static String getErrorMessage(int err_num, int err_off) {
        if(err_num == (int)ERROR::INVALID_MODIFIER){
            return MSG<Char>::INVALID_MODIFIER() + toString((Char)err_off);
        } else if(err_num != 0) {
            return getPcre2ErrorMessage((int) err_num) + ParseInt<Char>::toString((int) err_off);
        } else return String();
    }

    //forward declaration
    class Regex; 
    
    /** Provides public constructors to create RegexMatch objects.
     * Every RegexMatch object should be associated with a Regex object.
     * This class stores a pointer to its' associated Regex object, thus when
     * the content of the associated Regex object is changed, there's no need to
     * set the pointer again.
     * 
     * Examples:
     * 
     * ```cpp
     * Regex re;
     * RegexMatch rm;
     * rm.setRegexObject(&re);
     * rm.match("subject", "g");  // 0 match
     * re.compile("\\w");
     * rm.match();  // 7 matches
     * ```
     */
    class RegexMatch { 

    private: 

        const Regex* re;              

        String m_subject;
        const String* m_subject_ptr; //for huge text avoid copy
        Uint match_opts;        
        Uint jpcre2_match_opts; 
        int error_number;
        PCRE2_SIZE error_offset;
        
        PCRE2_SIZE _start_offset; //name collision, use _ at start
        PCRE2_SIZE _end_offset;

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
            re = 0;
            vec_num = 0; 
            vec_nas = 0; 
            vec_ntn = 0; 
            num_sub = 0; 
            nas_map = 0; 
            ntn_map = 0; 
            match_opts = 0; 
            jpcre2_match_opts = 0; 
            error_number = 0;
            error_offset = 0;
            _start_offset = 0;
            _end_offset = 0;
            m_subject_ptr = &m_subject;
        }
        
        void resetMaps(){
            delete num_sub; 
            delete nas_map; 
            delete ntn_map;
            num_sub = 0;
            nas_map = 0;
            ntn_map = 0;
        }
        
        void deepCopy(const RegexMatch& rm){
            re = rm.re; //only pointer should be copied
            
            m_subject = rm.m_subject;
            //pointer to subject may point to m_subject or other user data
            if(rm.m_subject_ptr == &rm.m_subject) m_subject_ptr = &m_subject; //not &rm.m_subject
            else m_subject_ptr = rm.m_subject_ptr;
                        
            //undelying data of vectors are not handled by RegexMatch
            //thus it's safe to just copy the pointers.
            vec_num = rm.vec_num;
            vec_nas = rm.vec_nas;
            vec_ntn = rm.vec_ntn;
            
            //maps should be null, no copy needed
            
            match_opts = rm.match_opts;
            jpcre2_match_opts = rm.jpcre2_match_opts;
            error_number = rm.error_number;
            error_offset = rm.error_offset;
            _start_offset = rm._start_offset;
            _end_offset = rm._end_offset;
            
        }

        friend class Regex;

    public: 
    
        ///Default constructor.
        RegexMatch(){
            init_vars();
        }
    
        ///@overload
        ///
        ///
        ///Creates a RegexMatch object associating a Regex object.
        ///@param r pointer to a Regex object
        RegexMatch(Regex * r) {
            init_vars();
            re = r;
        }
        
        ///@overload
        ///
        ///
        ///Copy constructor. Performs deep copy.
        ///@param rm Reference to RegexMatch object
        RegexMatch(const RegexMatch& rm){
            init_vars();
            deepCopy(rm);
        }
        
        ///Overloaded copy-assignment operator
        ///Allowes assigning objects like this:
        ///```cpp
        ///RegexMatch rm;
        ///rm = RegexMatch(&re);
        ///```
        ///@param rm RegexMatch object
        ///@return A reference to the calling RegexMatch object.
        RegexMatch& operator=(const RegexMatch& rm){
            if(this == &rm) return *this;
            deepCopy(rm);
            return *this;
        }
        
        ~RegexMatch() { 
            delete num_sub; 
            delete nas_map; 
            delete ntn_map; 
        } 

        /** Reset all class variables to its default (initial) state.
         * Data in the vectors will retain (It won't delete previous data in vectors)
         * You will need to pass vector pointers again after calling this function to get match results.
         * @return Reference to the calling RegexMatch object.
         * */
        RegexMatch& reset() { 
            resetMaps();
            m_subject.clear();
            init_vars();
            return *this; 
        } 
        
        ///Reset match related errors to zero.
        ///If you want to examine the error status of a function call in the method chain,
        ///add this function just before your target function so that the error is set to zero
        ///before that target function is called, and leave everything out after the target
        ///function so that there will be no additional errors from other function calls.
        ///@return A reference to the RegexMatch object
        ///@see Regex::resetErrors()
        ///@see RegexReplace::resetErrors()
        RegexMatch& resetErrors(){
            error_number = 0;
            error_offset = 0;
            return *this;
        }
        
        /// Returns the last error number
        ///@return Last error number
        int getErrorNumber(){ 
            return error_number; 
        } 

        /// Returns the last error offset
        ///@return Last error offset
        int getErrorOffset() { 
            return (int)error_offset; 
        } 
        
        /// Returns the last error message
        ///@return Last error message
        String getErrorMessage() { 
            return select<Char, BS>::getErrorMessage(error_number, error_offset); 
        } 
        
        ///Get subject string.
        ///@return subject string
        ///@see RegexReplace::getSubject()
        String getSubject() { 
            return *m_subject_ptr; 
        } 
        
        ///Get pointer to subject string.
        ///Data can not be changed with this pointer.
        ///@return subject string pointer 
        ///@see RegexReplace::getSubjectPointer()
        const String* getSubjectPointer() { 
            return m_subject_ptr;
        } 


        /** Calculate modifier string from PCRE2 and JPCRE2 options and return it.
         *
         *  Do remember that modifiers (or PCRE2 and JPCRE2 options) do not change or get initialized
         *  as long as you don't do that explicitly. Calling RegexMatch::setModifier() will re-set them.
         *
         *  **Mixed or combined modifier**.
         *
         *  Some modifier may include other modifiers i.e they have the same meaning of some modifiers
         *  combined together. For example, the 'n' modifier includes the 'u' modifier and together they
         *  are equivalent to `PCRE2_UTF | PCRE2_UCP`. When you set a modifier like this, both options
         *  get set, and when you remove the 'n' modifier (with `RegexMatch::changeModifier()`), both will get removed.
         * @return Calculated modifier string (std::string)
         * @see Regex::getModifier()
         * @see RegexReplace::getModifier()
         * */
        std::string getModifier(); 
        
        
        /**Get PCRE2 option
         * @return PCRE2 option for match operation
         * @see Regex::getPcre2Option()
         * @see RegexReplace::getPcre2Option()
         * */
        Uint getPcre2Option() { 
            return match_opts; 
        } 

        /** Get JPCRE2 option
         * @return JPCRE2 options for math operation
         * @see Regex::getJpcre2Option()
         * @see RegexReplace::getJpcre2Option()
         * */
        Uint getJpcre2Option() { 
            return jpcre2_match_opts; 
        } 
        
        /// Get offset from where match will start in the subject.
        /// @return Start offset
        PCRE2_SIZE getStartOffset() {
            return _start_offset;
        }
        
        /// Get offset where last match operation has ended
        /// @return End offset
        PCRE2_SIZE getEndOffset() {
            return _end_offset;
        }
        
        ///Get a pointer to the associated Regex object.
        ///If no actual Regex object is associated, null is returned.
        ///@return A pointer to the associated Regex object or null.
        const Regex* getRegexObject(){
            return re;
        }
        
        ///Set pointer to the associated regex object.
        ///@param r Pointer to a Regex object.
        ///@return Reference to the calling RegexMatch object.
        RegexMatch& setRegexObject(const Regex* r){
            re = r;
            return *this;
        }

        /// Set a pointer to the numbered substring vector.
        /// This vector will be filled with numbered (indexed) captured groups.
        /// @param v pointer to the numbered substring vector
        /// @return Reference to the calling RegexMatch object
        RegexMatch& setNumberedSubstringVector(VecNum* v) { 
            vec_num = v; 
            return *this; 
        } 

        /// Set a pointer to the named substring vector.
        /// This vector will be populated with named captured groups.
        /// @param v pointer to the named substring vector
        /// @return Reference to the calling RegexMatch object
        RegexMatch& setNamedSubstringVector(VecNas* v) { 
            vec_nas = v; 
            return *this; 
        } 

        /// Set a pointer to the name to number map vector.
        /// This vector will be populated with name to number map for captured groups.
        /// @param v pointer to the name to number map vector
        /// @return Reference to the calling RegexMatch object
        RegexMatch& setNameToNumberMapVector(VecNtN* v) { 
            vec_ntn = v; 
            return *this; 
        } 

        ///Set the subject string for match.
        ///This makes a copy of the subject string. If a copy is not desirable
        ///or you are working with huge text, consider passing a pointer instead
        ///of a constant reference.
        /// @param s Subject string
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setSubject()
        RegexMatch& setSubject(const String& s) { 
            m_subject = s;
            m_subject_ptr = &m_subject; //must overwrite
            return *this; 
        } 

        ///@overload
        ///
        ///
        ///Set pointer to the subject string for match.
        /// @param s Pointer to subject string
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setSubject()
        RegexMatch& setSubject(const String* s) { 
            m_subject_ptr = s; 
            return *this; 
        } 


        /// Set the modifier (resets all JPCRE2 and PCRE2 options) by calling RegexMatch::changeModifier().
        /// Re-initializes the option bits for PCRE2 and JPCRE2 options, then parses the modifier to set their equivalent options.
        ///
        /// **Note:** If speed of operation is very crucial, use RegexMatch::setJpcre2Option() and
        /// RegexMatch::setPcre2Option() with equivalent options. It will be faster that way.
        /// @param s Modifier string
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setModifier()
        /// @see Regex::setModifier()
        RegexMatch& setModifier(const std::string& s) { 
            match_opts = 0; 
            jpcre2_match_opts = 0; 
            changeModifier(s, true); 
            return *this; 
        } 

        /// Set JPCRE2 option for match (overwrite existing option)
        /// @param x Option value
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setJpcre2Option()
        /// @see Regex::setJpcre2Option()
        RegexMatch& setJpcre2Option(Uint x) { 
            jpcre2_match_opts = x; 
            return *this; 
        } 

        ///Set PCRE2 option match (overwrite existing option)
        /// @param x Option value
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setPcre2Option()
        /// @see Regex::setPcre2Option()
        RegexMatch& setPcre2Option(Uint x) { 
            match_opts = x; 
            return *this; 
        } 
        
        /// Set whether to perform global match
        /// @param x True or False
        /// @return Reference to the calling RegexMatch object
        RegexMatch& setFindAll(bool x) { 
            if (x) 
                jpcre2_match_opts |= FIND_ALL; 
            else 
                jpcre2_match_opts &= ~FIND_ALL; 
            return *this; 
        } 

        ///@overload
        ///
        ///
        ///This function just calls RegexMatch::setFindAll(bool x) with `true` as the parameter
        ///@return Reference to the calling RegexMatch object
        RegexMatch& setFindAll() { 
            return setFindAll(true); 
        }

        /// Set offset from where match starts.
        /// When FIND_ALL is set, a global match would not be performed on all positions on the subject,
        /// rather it will be performed from the start offset and onwards.
        /// @param offset Start offset
        /// @return Reference to the calling RegexMatch object
        RegexMatch& setStartOffset(PCRE2_SIZE offset) {
            _start_offset = offset;
            return *this;
        }

        /// After a call to this function PCRE2 and JPCRE2 options will be properly set.
        /// This function does not initialize or re-initialize options.
        /// If you want to set options from scratch, initialize them to 0 before calling this function.
        ///
        /// **Note:** If speed of operation is very crucial, use RegexMatch::changeJpcre2Option() and
        /// RegexMatch::changePcre2Option() with equivalent options. It will be faster that way.
        ///
        /// If invalid modifier is detected, then the error number for the RegexMatch
        /// object will be jpcre2::ERROR::INVALID_MODIFIER and error offset will be the modifier character.
        /// You can get the message with RegexMatch::getErrorMessage() function.
        /// @param mod Modifier string
        /// @param x Whether to add or remove option
        /// @return Reference to the RegexMatch object
        /// @see Regex::changeModifier()
        /// @see RegexReplace::changeModifier()
        RegexMatch& changeModifier(const std::string& mod, bool x); 

        /// Parse modifier and add/remove equivalent PCRE2 and JPCRE2 options.
        /// Add or remove a JPCRE2 option
        /// @param opt JPCRE2 option value
        /// @param x Add the option if it's true, remove otherwise.
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::changeJpcre2Option()
        /// @see Regex::changeJpcre2Option()
        RegexMatch& changeJpcre2Option(Uint opt, bool x) { 
            if(x) 
                jpcre2_match_opts |= opt; 
            else 
                jpcre2_match_opts &= ~opt; 
            return *this; 
        } 

        /// Add or remove a PCRE2 option
        /// @param opt PCRE2 option value
        /// @param x Add the option if it's true, remove otherwise.
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::changePcre2Option()
        /// @see Regex::changePcre2Option()
        RegexMatch& changePcre2Option(Uint opt, bool x) { 
            if(x) 
                match_opts |= opt; 
            else 
                match_opts &= ~opt; 
            return *this; 
        } 
        
        /// Parse modifier string and add equivalent PCRE2 and JPCRE2 options.
        /// This is just a wrapper of the original function RegexMatch::changeModifier()
        /// provided for convenience.
        ///
        /// **Note:** If speed of operation is very crucial, use RegexMatch::addJpcre2Option() and RegexMatch::addPcre2Option()
        /// with equivalent options. It will be faster that way.
        /// @param mod Modifier string
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::addModifier()
        /// @see Regex::addModifier()
        RegexMatch& addModifier(const std::string& mod){ 
            return changeModifier(mod, true); 
        } 

        /// Add option to existing JPCRE2 options for match
        /// @param x Option value
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::addJpcre2Option()
        /// @see Regex::addJpcre2Option()
        RegexMatch& addJpcre2Option(Uint x) { 
            jpcre2_match_opts |= x; 
            return *this; 
        } 

        /// Add option to existing PCRE2 options for match
        /// @param x Option value
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::addPcre2Option()
        /// @see Regex::addPcre2Option()
        RegexMatch& addPcre2Option(Uint x) { 
            match_opts |= x; 
            return *this; 
        }

        /// Perform match operaton using info from class variables and return the match count and
        /// store the results in specified vectors.
        ///@return Match count
        SIZE_T match(void);
        
        ///@overload
        ///
        ///
        ///Overwrites start offset before match
        ///@param start_offset Start offset to start the match from
        ///@return Match count
        SIZE_T match(PCRE2_SIZE start_offset){
            return setStartOffset(start_offset).match();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites subject before match
        ///@param s Subject string
        ///@return Match count
        SIZE_T match(const String& s){
            return setSubject(s).match();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites pointer to subject before match
        ///@param s Pointer to subject string
        ///@return Match count
        SIZE_T match(const String* s){
            return setSubject(s).match();
        }
        
        ///@overload
        ///
        ///
        ///* Overwrites subject string before match
        ///* Resets all JPCRE2 and PCRE2 options and initializes them according to new modifier string
        ///
        ///@param s Subject string
        ///@param mod Modifier string
        ///@return Match count
        SIZE_T match(const String& s, const std::string& mod){
            return setSubject(s).setModifier(mod).match();
        }
        
        ///@overload
        ///
        ///
        ///* Overwrites pointer to subject string before match
        ///* Resets all JPCRE2 and PCRE2 options and initializes them according to new modifier string
        ///
        ///@param s Pointer to subject string
        ///@param mod Modifier string
        ///@return Match count
        SIZE_T match(const String* s, const std::string& mod){
            return setSubject(s).setModifier(mod).match();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites subject string and start offset before match
        ///@param s Subject string
        ///@param start_offset Start offset to start the match from
        ///@return Match count
        SIZE_T match(const String& s, PCRE2_SIZE start_offset){
            return setSubject(s).setStartOffset(start_offset).match();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites start offset and pointer to subject string before match
        ///@param s Pointer to subject string
        ///@param start_offset Start offset to start the match from
        ///@return Match count
        SIZE_T match(const String* s, PCRE2_SIZE start_offset){
            return setSubject(s).setStartOffset(start_offset).match();
        }
        
        ///@overload
        ///
        ///
        ///* Overwrites subject string and start offset before match
        ///* Resets all JPCRE2 and PCRE2 options and initializes them according to new modifier string.
        ///
        ///@param s Subject string
        ///@param mod Modifier string
        ///@param start_offset Start offset to start the match from
        ///@return Match count
        SIZE_T match(const String& s, const std::string& mod, PCRE2_SIZE start_offset){
            return setSubject(s).setModifier(mod).setStartOffset(start_offset).match();
        }
        
        ///@overload
        ///
        ///
        ///* Overwrites start offset and pointer to subject string before match
        ///* Resets all JPCRE2 and PCRE2 options and initializes them according to new modifier string.
        ///
        ///@param s Pointer to subject string
        ///@param mod Modifier string
        ///@param start_offset Start offset to start the match from
        ///@return Match count
        SIZE_T match(const String* s, const std::string& mod, PCRE2_SIZE start_offset){
            return setSubject(s).setModifier(mod).setStartOffset(start_offset).match();
        }
        
    }; 

    
    /** Provides public constructors to create RegexReplace objects.
     * Every RegexReplace object should be associated with a Regex object.
     * This class stores a pointer to its' associated Regex object, thus when
     * the content of the associated Regex object is changed, there's no need to
     * set the pointer again.
     * 
     * Examples:
     * 
     * ```cpp
     * Regex re;
     * RegexReplace rr;
     * rr.setRegexObject(&re);
     * rr.replace("subject", "me");  // returns 'subject'
     * re.compile("\\w+");
     * rr.replace();  // replaces 'subject' with 'me' i.e returns 'me'
     * ```
     */
    class RegexReplace { 

    private: 

        const Regex* re;      

        String r_subject;
        const String* r_subject_ptr;
        String r_replw;
        const String* r_replw_ptr;
        Uint replace_opts;          
        Uint jpcre2_replace_opts;  
        PCRE2_SIZE buffer_size;     
        int error_number;
        PCRE2_SIZE error_offset;
        
        void init_vars() {
            re = 0;
            r_subject_ptr = &r_subject;
            r_replw_ptr = &r_replw;
            replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; 
            jpcre2_replace_opts = 0; 
            buffer_size = SUBSTITUTE_RESULT_INIT_SIZE; 
            error_number = 0;
            error_offset = 0;
        }

        void deepCopy(const RegexReplace& rr){
            re = rr.re; //only pointer should be copied.
            
            r_subject = rr.r_subject;
            //rr.r_subject_ptr may point to rr.r_subject or other user data
            if(rr.r_subject_ptr == &rr.r_subject) r_subject_ptr = &r_subject; //not rr.r_subject
            else r_subject_ptr = rr.r_subject_ptr; //other user data
            
            r_replw = rr.r_replw;
            //rr.r_replw_ptr may point to rr.r_replw or other user data
            if(rr.r_replw_ptr == &rr.r_replw) r_replw_ptr = &r_replw; //not rr.r_replw
            else r_replw_ptr = rr.r_replw_ptr; //other user data
            
            replace_opts = rr.replace_opts;
            jpcre2_replace_opts = rr.jpcre2_replace_opts;
            buffer_size = rr.buffer_size;
            error_number = rr.error_number;
            error_offset = rr.error_offset;
        }
        
        friend class Regex;

    public: 

        ///Default constructor
        RegexReplace(){
            init_vars();
        }

        ///@overload
        ///
        ///
        ///Creates a RegexReplace object associating a Regex object.
        ///@param r pointer to a Regex object
        RegexReplace(Regex * r) { 
            init_vars();
            re = r;
        }
        
        ///@overload
        ///
        ///
        ///Copy constructor\. Performs a deep copy.
        ///@param rr RegexReplace object reference
        RegexReplace(const RegexReplace& rr){
            init_vars();
            deepCopy(rr);
        }
        
        ///Overloaded Copy assignment operator.
        ///Allows object assignment like this:
        ///```cpp
        ///RegexReplace rr;
        ///rr = RegexReplace(&re);
        ///```
        ///@param rr RegexReplace object reference
        ///@return A reference to the calling RegexReplace object
        RegexReplace& operator=(const RegexReplace& rr){
            if(this == &rr) return *this;
            deepCopy(rr);
            return *this;
        }
        
        ~RegexReplace() {} 
    
        /** Reset all class variables to its default (initial) state.
         * @return Reference to the calling RegexReplace object.
         * */     
        RegexReplace& reset() { 
            r_subject.clear();
            r_replw.clear();
            init_vars(); 
            return *this; 
        } 
        
        /// Reset replace related errors to zero.
        ///If you want to examine the error status of a function call in the method chain,
        ///add this function just before your target function so that the error is set to zero
        ///before that target function is called, and leave everything out after the target
        ///function so that there will be no additional errors from other function calls.
        ///@return Reference to the calling RegexReplace object
        ///@see Regex::resetErrors()
        ///@see RegexMatch::resetErrors()
        RegexReplace& resetErrors(){
            error_number = 0;
            error_offset = 0;
            return *this;
        }

        /// Returns the last error number
        ///@return Last error number
        int getErrorNumber(){ 
            return error_number; 
        } 

        /// Returns the last error offset
        ///@return Last error offset
        int getErrorOffset() { 
            return (int)error_offset; 
        } 
        
        /// Returns the last error message
        ///@return Last error message
        String getErrorMessage() { 
            return select<Char, BS>::getErrorMessage(error_number, error_offset); 
        } 
        
        /// Get replacement string
        ///@return replacement string
        String getReplaceWith() { 
            return *r_replw_ptr; 
        } 
        
        /// Get pointer to replacement string
        ///@return pointer to replacement string
        const String* getReplaceWithPointer() { 
            return r_replw_ptr; 
        } 

        /// Get subject string
        ///@return subject string
        ///@see RegexMatch::getSubject()
        String getSubject() { 
            return *r_subject_ptr; 
        } 

        /// Get pointer to subject string
        ///@return Pointer to subject string
        ///@see RegexMatch::getSubjectPointer()
        const String* getSubjectPointer() { 
            return r_subject_ptr; 
        } 

 
         /** Calculate modifier string from PCRE2 and JPCRE2 options and return it.
         *
         *  Do remember that modifiers (or PCRE2 and JPCRE2 options) do not change or get initialized
         *  as long as you don't do that explicitly. Calling RegexReplace::setModifier() will re-set them.
         *
         *  **Mixed or combined modifier**.
         *
         *  Some modifier may include other modifiers i.e they have the same meaning of some modifiers
         *  combined together. For example, the 'n' modifier includes the 'u' modifier and together they
         *  are equivalent to `PCRE2_UTF | PCRE2_UCP`. When you set a modifier like this, both options
         *  get set, and when you remove the 'n' modifier (with `RegexReplace::changeModifier()`), both will get removed.
         *  @return Calculated modifier string (std::string)
         * @see RegexMatch::getModifier()
         * @see Regex::getModifier()
         * */
        std::string getModifier(); 
        
        /** Get PCRE2 option
         * @return PCRE2 option for replace
         * @see Regex::getPcre2Option()
         * @see RegexMatch::getPcre2Option()
         * */
        Uint getPcre2Option() { 
            return replace_opts; 
        } 

        /** Get JPCRE2 option
         * @return JPCRE2 option  for replace
         * @see Regex::getJpcre2Option()
         * @see RegexMatch::getJpcre2Option()
         * */
        Uint getJpcre2Option() { 
            return jpcre2_replace_opts; 
        }
        
        ///Get a pointer to the associated Regex object.
        ///If no actual Regex object is associated, null is returned
        ///@return A pointer to the associated Regex object or null
        const Regex* getRegexObject(){
            return re;
        }
        
        ///Set pointer to the associated Regex object.
        ///@param r Pointer to a Regex object.
        ///@return Reference to the calling RegexReplace object.
        RegexReplace& setRegexObject(const Regex* r){
            re = r;
            return *this;
        }
    
        /** Set the subject string for replace.
         * This makes a copy of the string. If no copy is desired or you are working
         * with big text, consider passing pointer to the subject string.
         * @param s Subject string
         * @return Reference to the calling RegexReplace object
         * @see RegexMatch::setSubject()
         * */
        RegexReplace& setSubject(const String& s) { 
            r_subject = s; 
            r_subject_ptr = &r_subject; //must overwrite
            return *this; 
        } 
    
        /**@overload
         * 
         * 
         *  Set pointer to the subject string for replace
         * @param s Pointer to subject string
         * @return Reference to the calling RegexReplace object
         * @see RegexMatch::setSubject()
         * */
        RegexReplace& setSubject(const String* s) { 
            r_subject_ptr = s; 
            return *this; 
        }
         
        /** Set the replacement string.
         * `$` is a special char. It denotes a captured group, e.g `$0` is the whole match,
         * $1 is the caputred group no. 1, `${name}` is the captured group `name`.
         * To isolate a capture group, wrap the number with `{}` e.g ${12} is the 12th
         * captured group.
         * To pass a literal `$` you must use `$$`.
         * 
         * **Note:** This function makes a copy of the string. If no copy is desired or
         * you are working with big text, consider passing the string with pointer.
         * 
         * @param s String to replace with
         * @return Reference to the calling RegexReplace object
         * */
        RegexReplace& setReplaceWith(const String& s) { 
            r_replw = s;
            r_replw_ptr = &r_replw; //must overwrite
            return *this; 
        }   
        
        /**@overload
         * 
         * 
         * @param s Pointer to the string to replace with
         * @return Reference to the calling RegexReplace object
         * */
        RegexReplace& setReplaceWith(const String* s) { 
            r_replw_ptr = s; 
            return *this; 
        }    
        
        /** Set the modifier string (resets all JPCRE2 and PCRE2 options) by calling RegexReplace::changeModifier().
         *
         * **Note:** If speed of operation is very crucial, use RegexReplace::setJpcre2Option() and RegexReplace::setPcre2Option()
         * with equivalent options. It will be faster that way.
         * @param s Modifier string
         * @return Reference to the calling RegexReplace object
         * @see RegexMatch::setModifier()
         * @see Regex::setModifier()
         */
        RegexReplace& setModifier(const std::string& s) { 
            replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; /* must not be initialized to 0 */ 
            jpcre2_replace_opts = 0; 
            return changeModifier(s, true); 
        } 

        /** Set the initial buffer size to be allocated for replaced string (used by PCRE2)
         * @param x Buffer size
         * @return Reference to the calling RegexReplace object
         */
        RegexReplace& setBufferSize(PCRE2_SIZE x) { 
            buffer_size = x; 
            return *this; 
        } 

        /** Set JPCRE2 option for replace (overwrite existing option)
         * @param x Option value
         * @return Reference to the calling RegexReplace object
         * @see RegexMatch::setJpcre2Option()
         * @see Regex::setJpcre2Option()
         */
        RegexReplace& setJpcre2Option(Uint x) { 
            jpcre2_replace_opts = x; 
            return *this; 
        } 

        /** Set PCRE2 option replace (overwrite existing option)
         * @param x Option value
         * @return Reference to the calling RegexReplace object
         * @see RegexMatch::setPcre2Option()
         * @see Regex::setPcre2Option()
         */
        RegexReplace& setPcre2Option(Uint x) { 
            replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH | x; 
            return *this; 
        } 

        /// After a call to this function PCRE2 and JPCRE2 options will be properly set.
        /// This function does not initialize or re-initialize options.
        /// If you want to set options from scratch, initialize them to 0 before calling this function.
        ///
        /// **Note:** If speed of operation is very crucial, use RegexReplace::changeJpcre2Option() and
        /// RegexReplace::changePcre2Option() with equivalent options. It will be faster that way.
        ///
        /// If invalid modifier is detected, then the error number for the RegexReplace
        /// object will be jpcre2::ERROR::INVALID_MODIFIER and error offset will be the modifier character.
        /// You can get the message with RegexReplace::getErrorMessage() function.
        /// @param mod Modifier string
        /// @param x Whether to add or remove option
        /// @return Reference to the RegexReplace object
        /// @see Regex::changeModifier()
        /// @see RegexMatch::changeModifier()
        RegexReplace& changeModifier(const std::string& mod, bool);
         
        /// Parse modifier and add/remove equivalent PCRE2 and JPCRE2 options.
        /// Add or remove a JPCRE2 option
        /// @param opt JPCRE2 option value
        /// @param x Add the option if it's true, remove otherwise.
        /// @return Reference to the calling RegexReplace object
        /// @see RegexMatch::changeJpcre2Option()
        /// @see Regex::changeJpcre2Option()
        RegexReplace& changeJpcre2Option(Uint opt, bool x) { 
            if(x) 
                jpcre2_replace_opts |= opt; 
            else 
                jpcre2_replace_opts &= ~opt; 
            return *this; 
        } 

        /// Add or remove a PCRE2 option
        /// @param opt PCRE2 option value
        /// @param x Add the option if it's true, remove otherwise.
        /// @return Reference to the calling RegexReplace object
        /// @see RegexMatch::changePcre2Option()
        /// @see Regex::changePcre2Option()
        RegexReplace& changePcre2Option(Uint opt, bool x) { 
            if(x) 
                replace_opts |= opt; 
            else 
                replace_opts &= ~opt; 
                //replace_opts |= PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; /* It's important, but let user override it. */ 
            return *this; 
        } 
        
        /// Parse modifier string and add equivalent PCRE2 and JPCRE2 options.
        /// This is just a wrapper of the original function RegexReplace::changeModifier()
        /// provided for convenience.
        ///
        /// **Note:** If speed of operation is very crucial, use RegexReplace::addJpcre2Option() and
        /// RegexReplace::addPcre2Option() with equivalent options. It will be faster that way.
        /// @param mod Modifier string
        /// @return Reference to the calling RegexReplace object
        /// @see RegexMatch::addModifier()
        /// @see Regex::addModifier()
        RegexReplace& addModifier(const std::string& mod){ 
            return changeModifier(mod, true); 
        } 

        /** Add specified JPCRE2 option to existing options for replace.
         * @param x Option value
         * @return Reference to the calling RegexReplace object
         * @see RegexMatch::addJpcre2Option()
         * @see Regex::addJpcre2Option()
         * */
        RegexReplace& addJpcre2Option(Uint x) { 
            jpcre2_replace_opts |= x; 
            return *this; 
        } 
        
        /** Add specified PCRE2 option to existing options for replace
         * @param x Option value
         * @return Reference to the calling RegexReplace object
         * @see RegexMatch::addPcre2Option()
         * @see Regex::addPcre2Option()
         * */
        RegexReplace& addPcre2Option(Uint x) { 
            replace_opts |= x; 
            return *this; 
        } 
        
        /// Perform regex replace by retrieving subject string, replacement string, modifier and other options from class variables.
        /// In the replacement string (see RegexReplace::setReplaceWith()) `$` is a special character which implies captured group.
        /// 1. A numbered substring can be referenced with `$n` or `${n}` where n is the group number.
        /// 2. A named substring can be referenced with `${name}`, where 'name' is the group name.
        /// 3. A literal `$` can be given as `$$`.
        ///
        ///@return Replaced string
        String replace(void);
        
        ///@overload
        ///
        ///
        ///Overwrites the subject before replace.
        ///@param mains Subject string
        ///@return replaced string
        String replace(const String& mains){
            return setSubject(mains).replace();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites pointer to the subject before replace.
        ///@param mains Pointer to subject string
        ///@return replaced string
        String replace(const String* mains){
            return setSubject(mains).replace();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites the subject and replacement string
        ///@param mains Subject string
        ///@param repl Replacement string
        ///@return replaced string
        String replace(const String& mains, const String& repl){
            return setSubject(mains).setReplaceWith(repl).replace();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites replacement string and pointer to the subject string
        ///@param mains Pointer to subject string
        ///@param repl Replacement string
        ///@return replaced string
        String replace(const String* mains, const String& repl){
            return setSubject(mains).setReplaceWith(repl).replace();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites  the subject string and pointer to replacement string
        ///@param mains Subject string
        ///@param repl Pointer to replacement string
        ///@return replaced string
        String replace(const String& mains, const String* repl){
            return setSubject(mains).setReplaceWith(repl).replace();
        }
        
        ///@overload
        ///
        ///
        ///Overwrites pointer to replacement and the subject string
        ///@param mains Pointer to subject string
        ///@param repl Pointer to replacement string
        ///@return replaced string
        String replace(const String* mains, const String* repl){
            return setSubject(mains).setReplaceWith(repl).replace();
        }
        
        ///@overload
        ///
        ///
        ///* Resets all JPCRE2 and PCRE2 options and resets them according to new modifier string.
        ///* Overwrites the subject and replacement string
        ///
        ///@param mains Subject string
        ///@param repl Replacement string
        ///@param mod Modifier string
        ///@return replaced string
        String replace(const String& mains, const String& repl, const std::string& mod){
            return setSubject(mains).setReplaceWith(repl).setModifier(mod).replace();
        }
        
        ///@overload
        ///@param mains Pointer to subject string
        ///@param repl Replacement string
        ///@param mod Modifier string
        ///@return replaced string
        String replace(const String* mains, const String& repl, const std::string& mod){
            return setSubject(mains).setReplaceWith(repl).setModifier(mod).replace();
        }
        
        ///@overload
        ///@param mains Subject string
        ///@param repl Pointer to replacement string
        ///@param mod Modifier string
        ///@return replaced string
        String replace(const String& mains, const String* repl, const std::string& mod){
            return setSubject(mains).setReplaceWith(repl).setModifier(mod).replace();
        }
        
        ///@overload
        ///@param mains Pointer to subject string
        ///@param repl Pointer to replacement string
        ///@param mod Modifier string
        ///@return replaced string
        String replace(const String* mains, const String* repl, const std::string& mod){
            return setSubject(mains).setReplaceWith(repl).setModifier(mod).replace();
        }
    }; 
 
 
    /** Provides public constructos to create Regex object.
     * Each regex pattern needs an object of this class and each pattern needs to be compiled.
     * Pattern compilation can be done using one of its' overloaded constructors or the `Regex::compile()`
     * memeber function.
     * 
     * This class can contain a RegexMatch or RegexReplace object if desired. RegexMatch
     * object can be created either with Regex::initMatch() or Regex::getMatchObject() member function.
     * Similar functions are available for RegexReplace.
     * 
     * Examples:
     * 
     * ```cpp
     * Regex re;
     * re.compile("pattern", "modifier");
     * Regex re2("pattern", "modifier");
     * re.getReplaceObject().setReplaceWith("$0");
     * ```
     *
     */
    class Regex { 

    private: 
    
        RegexMatch *rm;
        RegexReplace *rr;

        String pat_str;
        const String* pat_str_ptr;
        Pcre2Code *code;            
        Uint compile_opts;         
        Uint jpcre2_compile_opts;    
        std::string mylocale;            
        int error_number;
        PCRE2_SIZE error_offset;
        int ltype; //local category

        CompileContext *ccontext;
        const unsigned char* tables;
        
        
        void init_vars() { 
            jpcre2_compile_opts = 0; 
            compile_opts = 0; 
            error_number = 0; 
            error_offset = 0; 
            code = 0;
            rm = 0;
            rr = 0; 
            pat_str_ptr = &pat_str;
            ltype = 0;
            ccontext = 0;
            tables = 0;
        } 

        void freeRegexMemory(void) {
            Pcre2Func<BS>::code_free(code);
            code = 0; //we may use it again
        }
        
        void freeCompileContext(){
            Pcre2Func<BS>::compile_context_free(ccontext);
            ccontext = 0;
        }
        
        void freeCharTables(){
            ::free((void*)tables); //malloc was used by PCRE2
            tables = 0;
        }

        friend class RegexMatch;    
        friend class RegexReplace; 

        void deepCopy(const Regex& r) {
            pat_str = r.pat_str; //must not use setPattern() here
            //r.pat_str_ptr may point to other user data
            if(r.pat_str_ptr == &r.pat_str) pat_str_ptr = &pat_str; //not r.pat_str
            else pat_str_ptr = r.pat_str_ptr; //other user data
            
            mylocale = r.mylocale;
            ltype = r.ltype;
            //recreate ccontext and tables:
            if(!mylocale.empty()) 
                setLocale(ltype, mylocale);
            
            compile_opts = r.compile_opts; 
            jpcre2_compile_opts = r.jpcre2_compile_opts; 
            error_number = r.error_number; 
            error_offset = r.error_offset; 
            
            //Copy #code if it is non-null
            ///First release memory of #code from current object if it is non-NULL
            freeRegexMemory();
            if (r.code) {
                /// Copy compiled memory of #code to #code of current object using pcre2_code_copy() 
                code = Pcre2Func<BS>::code_copy(r.code);
                /// Perform JIT compilation (if enabled) as pcre2_code_copy() doesn't copy JIT memory
                if ((jpcre2_compile_opts & JIT_COMPILE) != 0) {
                    //Perform JIT compilation:
                    int jit_ret = Pcre2Func<BS>::jit_compile(code, PCRE2_JIT_COMPLETE);
                    if(jit_ret < 0) error_number = jit_ret;
                }
            } //else code is already null
            
            //use copy assignment for rm and rr
            delete rm;
            if(r.rm) {
                rm = new RegexMatch(*(r.rm)); 
                rm->re = this; //associated Regex object needs to be this one
            } else {
                rm = 0;
            }
            
            delete rr;
            if(r.rr){
                rr = new RegexReplace(*(r.rr)); 
                rr->re = this; //associated Regex object needs to be this one
            } else {
                rr = 0;
            }
        }

    public: 

        /// Default Constructor.
        /// Initializes all class variables to defaults.
        /// Does not perform any pattern compilation.
        Regex() { 
            init_vars();
        } 

        /** Compile pattern with initialization.
         *  @param re Pattern string
         * */
        Regex(const String& re) {
            init_vars();
            compile(re); 
        } 

        /** Compile pattern with initialization.
         *  @param re Pointer to pattern string
         * */
        Regex(const String* re) {
            init_vars();
            compile(re); 
        } 
        
        /** @overload
         *
         *
         *  Compile pattern with initialization.
         *  @param re Pattern string
         *  @param mod Modifier string
         * */
        Regex(const String& re, const std::string& mod) {
            init_vars();
            compile(re, mod); 
        } 
        
        /** @overload
         *
         *
         *  Compile pattern with initialization.
         *  @param re Pointer to pattern string
         *  @param mod Modifier string
         * */
        Regex(const String* re, const std::string& mod) {
            init_vars();
            compile(re, mod); 
        }

        /** @overload
         *
         *
         *  Compile pattern with initialization.
         *  @param re Pattern string
         *  @param pcre2_opts PCRE2 option value
         * */
        Regex(const String& re, Uint pcre2_opts) {
            init_vars();
            compile(re, pcre2_opts); 
        } 

        /** @overload
         *
         *
         *  Compile pattern with initialization.
         *  @param re Pointer to pattern string
         *  @param pcre2_opts PCRE2 option value
         * */
        Regex(const String* re, Uint pcre2_opts) {
            init_vars();
            compile(re, pcre2_opts); 
        } 

        /** @overload
         *
         *
         *  Compile pattern with initialization.
         *  @param re Pattern string
         *  @param pcre2_opts    PCRE2 option value
         *  @param opt_bits        JPCRE2 option value
         * */
        Regex(const String& re, Uint pcre2_opts, Uint opt_bits) {
            init_vars();
            compile(re, pcre2_opts, opt_bits); 
        } 

        /** @overload
         *
         *
         *  Compile pattern with initialization.
         *  @param re Pointer to pattern string
         *  @param pcre2_opts    PCRE2 option value
         *  @param opt_bits        JPCRE2 option value
         * */
        Regex(const String* re, Uint pcre2_opts, Uint opt_bits) {
            init_vars();
            compile(re, pcre2_opts, opt_bits); 
        } 

        /// @overload
        ///
        ///
        /// Copy constructor\. Performs a deep copy.
        /// The associated RegexMatch and RegexReplace objects (if any) are copied
        /// and their associated Regex object is set to this Regex Object.
        /// No change is made to the original Regex object or their associated
        /// RegexMatch and RegexReplace objects.
        /// @param r Constant reference to a Regex object.
        Regex(const Regex& r) {
            init_vars();
            deepCopy(r);
        } 
        
        /// Overloaded assignment operator.
        /// Performs a deep copy.
        ///
        /// The associated RegexMatch and RegexReplace objects (if any) are copied
        /// and their associated Regex object is set to this Regex Object.
        /// No change is made to the original Regex object or their associated
        /// RegexMatch and RegexReplace objects.
        ///
        /// Allows assigning objects like this:
        /// ```cpp
        /// Regex re;
        /// re = Regex("new pattern");
        /// ```
        /// However, use of this method is discouraged (Use Regex::compile() instead), because a call to this function
        /// requires an additional call to PCRE2 internal function pcre2_code_copy().
        /// If the pattern was JIT compiled, it requires another additional JIT compilation because
        /// JIT memory was not copied by pcre2_code_copy().
        ///
        /// **Memory management:** Old JIT memory will be released along with the old compiled code.
        /// @param r const Regex&
        /// @return *this
        Regex& operator=(const Regex& r) { 
            if (this == &r) return *this;
            deepCopy(r); 
            return *this; 
        } 
        
        #if __cplusplus >= 201103L
        
        /** Provides boolean check for the status of the object.
         *  This overlaoded boolean operator needs to be declared
         *  explicit to prevent implicit conversion and overloading issues.
         *
         *  We will only enable it if >=C++11 is being used, as the explicit keyword
         *  for a function other than constructor is not supported in older compilers.
         *
         *  If you are dealing with legacy code/compilers use the Double bang trick mentioned
         *  in Regex::operator!().
         *
         *  This helps us to check the status of the compiled regex like this:
         *
         *  ```
         *  jpcre2::select<char>::Regex re("pat", "mod");
         *  if(re) {
         *      std::cout<<"Compile success";
         *  } else {
         *      std::cout<<"Compile failed";
         *  }
         *  ```
         * @return true if regex compiled successfully, false otherwise.
         * */
        explicit operator bool() const { 
            return (code != 0); 
        }
        #endif
            
        /** Provides boolean check for the status of the object.
         *  This is a safe boolean approach (no implicit conversion  or overloading).
         *  We don't need the explicit keyword here and thus it's the preferable method
         *  to check for object status that will work well with older compilers.
         *  e.g:
         *
         *  ```
         *  jpcre2::select<char>::Regex re("pat","mod");
         *  if(!re) {
         *      std::cout<<"Compile failed";
         *  } else {
         *      std::cout<<"Compiled successfully";
         *  }
         *  ```
         *  Double bang trick:
         *
         *  ```
         *  jpcre2::select<char>::Regex re("pat","mod");
         *  if(!!re) {
         *      std::cout<<"Compiled successfully";
         *  } else {
         *      std::cout<<"Compile failed";
         *  }
         *  ```
         *  @return true if regex compile failed, false otherwise.
         * */
        bool operator!() const { 
            return (code == 0); 
        } 

        /// Destructor.
        /// Deletes all memory used by Regex, RegexMatch and RegexReplace object including compiled code and JIT memory.
        ~Regex() { 
            freeRegexMemory();
            freeCharTables();
            freeCompileContext();
            delete rm; /* Deleting null pointer is perfectly safe, no check needed. */ 
            delete rr; /* Deleting null pointer is perfectly safe, no check needed. */ 
        } 

        /** Reset all class variables to its default (initial) state.
         * Release any memory used by existing compiled pattern, RegexMatch, RegexReplace objects etc..
         * @return Reference to the calling Regex object.
         * */
        Regex& reset() { 
            freeRegexMemory(); 
            freeCharTables();
            freeCompileContext();
            pat_str.clear();
            delete rm;  /* deleting null pointer is safe. */ 
            delete rr; 
            init_vars();
            return *this; 
        } 

        /// Reset regex compile related errors to zero.
        ///If you want to examine the error status of a function call in the method chain,
        ///add this function just before your target function so that the error is set to zero
        ///before that target function is called, and leave everything out after the target
        ///function so that there will be no additional errors from other function calls.
        ///@return A reference to the Regex object
        ///@see  RegexReplace::resetErrors()
        ///@see  RegexMatch::resetErrors()
        Regex& resetErrors() { 
            error_number = 0; 
            error_offset = 0; 
            return *this; 
        } 

        /** Create and initialize a new match object and return a reference to it
         *
         * Options can be set with the setter functions of RegexMatch class
         * in-between the Regex::initMatch() and RegexMatch::match() call.
         *
         * @return Reference to a new RegexMatch object
         * @see Regex::getMatchObject()
         * */
        RegexMatch& initMatch() {
            delete rm; /* rm is either occupied or NULL, double deletion won't happen */ 
            rm = new RegexMatch(this);
            return *rm; 
        } 

        /** Creates a new RegexReplace object and returns its reference.
         * Options can be set with the setter functions of RegexReplace class
         * in-between the Regex::initReplace() and RegexReplace::replace() call.
         * @return Reference to a new RegexReplace object.
         * @see Regex::getReplaceObject()
         * */
        RegexReplace& initReplace() {
            delete rr; /* rr is either occupied or NULL, double deletion won't happen */ 
            rr = new RegexReplace(this);
            return *rr; 
        } 

        ///Copy the match object into this Regex object.
        ///The match object passed is copied into the Regex object leaving the
        ///original unchanged.
        ///After copying, the associated Regex object for this new internal
        ///match object is changed to this object.
        ///@param rmo Constant reference to a RegexMatch object
        ///@return Reference to the calling Regex object
        Regex& initMatchFrom(const RegexMatch& rmo){
            delete rm;
            rm = new RegexMatch(rmo);
            rm->re = this;
            return *this;
        }

        ///Copy the replace object into this Regex object.
        ///The replace object passed is copied into the Regex object leaving the
        ///original unchanged.
        ///After copying, the associated Regex object for this new internal
        ///replace object is changed to this object.
        ///@param rro Constant reference to a RegexReplace object
        ///@return Reference to the calling Regex object
        Regex& initReplaceFrom(const RegexReplace& rro){
            delete rr;
            rr = new RegexReplace(rro);
            rr->re = this;
            return *this;
        }

        /** Get pattern string
         * @return pattern string of type jpcre2::select::String
         * */
        String getPattern() { 
            return *pat_str_ptr; 
        }
        
        /** Get pointer to pattern string
         * @return Pointer to pattern string
         * */
        const String* getPatternPointer() { 
            return pat_str_ptr; 
        }
        
        /** Get locale as a string.
         *  To get the type id, call Regex::getLocaleTypeId()
         *  @return locale as std::string
         * */
        std::string getLocale() { 
            return mylocale; 
        }
        
        ///Get the category/type identifier for the locale
        ///that is being used to compile, match and replace.
        ///@return Locale Type identifier.
        int getLocaleTypeId(){
            return ltype;
        }


        /** Calculate modifier string from PCRE2 and JPCRE2 options and return it.
         *
         *  Do remember that modifiers (or PCRE2 and JPCRE2 options) do not change or get initialized
         *  as long as you don't do that explicitly. Calling Regex::setModifier() will re-set them.
         *
         *  **Mixed or combined modifier**.
         *
         *  Some modifier may include other modifiers i.e they have the same meaning of some modifiers
         *  combined together. For example, the 'n' modifier includes the 'u' modifier and together they
         *  are equivalent to `PCRE2_UTF | PCRE2_UCP`. When you set a modifier like this, both options
         *  get set, and when you remove the 'n' modifier (with `Regex::changeModifier()`), both will get removed.
         * @tparam Char_T Character type
         *  @return Calculated modifier string (std::string)
         * @see RegexMatch::getModifier()
         * @see RegexReplace::getModifier()
         * */
        std::string getModifier(); 

        /** Get PCRE2 option
         *  @return Compile time PCRE2 option value
         * @see RegexReplace::getPcre2Option()
         * @see RegexMatch::getPcre2Option()
         * */
        Uint getPcre2Option() { 
            return compile_opts; 
        } 

        /** Get JPCRE2 option
         *  @return Compile time JPCRE2 option value
         * @see RegexReplace::getJpcre2Option()
         * @see RegexMatch::getJpcre2Option()
         * */
        Uint getJpcre2Option() { 
            return jpcre2_compile_opts; 
        } 

        /// Returns the last error number
        ///@return Last error number
        int getErrorNumber(){ 
            return error_number; 
        } 

        /// Returns the last error offset
        ///@return Last error offset
        int getErrorOffset() { 
            return (int)error_offset; 
        } 
        
        /// Returns the last error message
        ///@return Last error message
        String getErrorMessage() { 
            return select<Char, BS>::getErrorMessage(error_number, error_offset); 
        } 
      
        /// Returns a reference to existing match object.
        /// If there was no match object, it will create a new and act similarly to Regex::initMatch()
        /// @return Reference to a RegexMatch object
        ///@see Regex::initMatch()
        RegexMatch& getMatchObject(){ 
            if(rm) return *rm; 
            else return initMatch(); 
        } 
        
        /// Returns a reference to the existing RegexReplace object.
        ///If there was no replace object, it will create a new one
        /// and act similarly to Regex::initReplace().
        ///@return reference to a RegexReplace object
        ///@see Regex::initReplace()
        RegexReplace& getReplaceObject() { 
            if(rr) return *rr; 
            else return initReplace(); 
        }
        
        ///Get new line convention from compiled code.
        ///@return New line option value or 0.
        ///```
        ///PCRE2_NEWLINE_CR        Carriage return only
        ///PCRE2_NEWLINE_LF        Linefeed only
        ///PCRE2_NEWLINE_CRLF      CR followed by LF only
        ///PCRE2_NEWLINE_ANYCRLF   Any of the above
        ///PCRE2_NEWLINE_ANY       Any Unicode newline sequence
        ///```
        Uint getNewLine(){
            if(!code) return 0;
            Uint newline = 0;
            int ret = Pcre2Func<BS>::pattern_info(code, PCRE2_INFO_NEWLINE, &newline);
            if(ret < 0) error_number = ret;
            return newline;
        }
        
        
        ///Set new line convention.
        ///@param value New line option value.
        ///```
        ///PCRE2_NEWLINE_CR        Carriage return only
        ///PCRE2_NEWLINE_LF        Linefeed only
        ///PCRE2_NEWLINE_CRLF      CR followed by LF only
        ///PCRE2_NEWLINE_ANYCRLF   Any of the above
        ///PCRE2_NEWLINE_ANY       Any Unicode newline sequence
        ///```
        ///@return Reference to the calling Regex object
        Regex& setNewLine(Uint value){
            if(!ccontext)
                ccontext = Pcre2Func<BS>::compile_context_create(0);
            int ret = Pcre2Func<BS>::set_newline(ccontext, value);
            if(ret < 0) error_number = ret;
            return *this;
        }
        
        /// Set the pattern string to compile
        /// @param re Pattern string
        /// @return Reference to the calling Regex object.
        Regex& setPattern(const String& re) { 
            pat_str = re;
            pat_str_ptr = &pat_str; //must overwrite
            return *this; 
        } 
        
        /// Set pointer to the pattern string to compile
        /// @param re Pattern string
        /// @return Reference to the calling Regex object.
        Regex& setPattern(const String* re) { 
            pat_str_ptr = re; 
            return *this;
        }

        /// set the modifier (resets all JPCRE2 and PCRE2 options) by calling Regex::changeModifier().
        /// Re-initializes the option bits for PCRE2 and JPCRE2 options, then parses the modifier and sets
        /// equivalent PCRE2 and JPCRE2 options.
        ///
        /// **Note:** If speed of operation is very crucial, use Regex::setJpcre2Option() and
        /// Regex::setPcre2Option() with equivalent options. It will be faster that way.
        /// @param x Modifier string
        /// @return Reference to the calling Regex object.
        /// @see RegexMatch::setModifier()
        /// @see RegexReplace::setModifier()
        Regex& setModifier(const std::string& x) { 
            compile_opts = 0; 
            jpcre2_compile_opts = 0; 
            return changeModifier(x, true); 
        } 

        /// Set the locale.
        /// Locale is set temporarily to create character tables.
        /// After creating character tables, locale is restored to its' prior state.
        /// **Note:** This function may or may not be thread safe (depends on std::setlocale()).
        /// @param category  locale category identifier, one of the LC_xxx macros. May be 0. 
        /// @param locale   system-specific locale identifier. Can be "" for the user-preferred locale or "C" for the minimal locale.
        /// @return Reference to the calling Regex object.
        Regex& setLocale(int category, const std::string& locale) {
            ltype = category;  //store
            mylocale = locale; //store
            freeCharTables();
            const char* loc_old = std::setlocale(ltype, 0);
            std::setlocale(ltype, mylocale.c_str());
            tables = Pcre2Func<BS>::maketables(0); //must pass 0, we are using free() to free the tables.
            if(!ccontext)
                ccontext = Pcre2Func<BS>::compile_context_create(0);
            Pcre2Func<BS>::set_character_tables(ccontext, tables);
            std::setlocale(ltype, loc_old);
            return *this; 
        } 
        
        /// Set JPCRE2 option for compile (overwrites existing option)
        /// @param x Option value
        /// @return Reference to the calling Regex object.
        /// @see RegexMatch::setJpcre2Option()
        /// @see RegexReplace::setJpcre2Option()
        Regex& setJpcre2Option(Uint x) { 
            jpcre2_compile_opts = x; 
            return *this; 
        } 

        ///  Set PCRE2 option for compile (overwrites existing option)
        /// @param x Option value
        /// @return Reference to the calling Regex object.
        /// @see RegexMatch::setPcre2Option()
        /// @see RegexReplace::setPcre2Option()
        Regex& setPcre2Option(Uint x) { 
            compile_opts = x; 
            return *this; 
        } 

        /// Parse modifier and add/remove equivalent PCRE2 and JPCRE2 options.
        /// This function does not initialize or re-initialize options.
        /// If you want to set options from scratch, initialize them to 0 before calling this function.
        ///
        /// **Note:** If speed of operation is very crucial, use Regex::changeJpcre2Option() and
        /// Regex::changePcre2Option() with equivalent options. It will be faster that way.
        ///
        /// If invalid modifier is detected, then the error number for the Regex
        /// object will be jpcre2::ERROR::INVALID_MODIFIER and error offset will be the modifier character.
        /// You can get the message with Regex::getErrorMessage() function.
        /// @param mod Modifier string
        /// @param x Whether to add or remove option
        /// @return Reference to the calling Regex object
        /// @see RegexMatch::changeModifier()
        /// @see RegexReplace::changeModifier()
        Regex& changeModifier(const std::string&, bool); 

        ///  Add or remove a JPCRE2 option
        /// @param opt JPCRE2 option value
        /// @param x Add the option if it's true, remove otherwise.
        /// @return Reference to the calling Regex object
        /// @see RegexMatch::changeJpcre2Option()
        /// @see RegexReplace::changeJpcre2Option()
        Regex& changeJpcre2Option(Uint opt, bool x) { 
            if(x) 
                jpcre2_compile_opts |= opt; 
            else 
                jpcre2_compile_opts &= ~opt; 
            return *this; 
        } 
     
        /// Add or remove a PCRE2 option
        /// @param opt PCRE2 option value
        /// @param x Add the option if it's true, remove otherwise.
        /// @return Reference to the calling Regex object
        /// @see RegexMatch::changePcre2Option()
        /// @see RegexReplace::changePcre2Option()
        Regex& changePcre2Option(Uint opt, bool x) { 
            if(x) 
                compile_opts |= opt; 
            else 
                compile_opts &= ~opt; 
            return *this; 
        } 
        
        /// Parse modifier string and add equivalent PCRE2 and JPCRE2 options.
        /// This is just a wrapper of the original function Regex::changeModifier()
        /// provided for convenience.
        ///
        /// **Note:** If speed of operation is very crucial, use Regex::addJpcre2Option() and
        /// Regex::addPcre2Option() with equivalent options. It will be faster that way.
        /// @param mod Modifier string
        /// @return Reference to the calling Regex object
        /// @see RegexMatch::addModifier()
        /// @see RegexReplace::addModifier()
        Regex& addModifier(const std::string& mod){ 
            return changeModifier(mod, true); 
        } 

        /// Add option to existing JPCRE2 options for compile
        /// @param x Option value
        /// @return Reference to the calling Regex object
        /// @see RegexMatch::addJpcre2Option()
        /// @see RegexReplace::addJpcre2Option()
        Regex& addJpcre2Option(Uint x) { 
            jpcre2_compile_opts |= x; 
            return *this; 
        } 

        ///  Add option to existing PCRE2 options for compile
        /// @param x Option value
        /// @return Reference to the calling Regex object
        /// @see RegexMatch::addPcre2Option()
        /// @see RegexReplace::addPcre2Option()
        Regex& addPcre2Option(Uint x) { 
            compile_opts |= x; 
            return *this; 
        } 

        /**Compile pattern using info from class variables.
         *
         * Prefer using one of its variants when compiling pattern for an already declared Regex object.
         * A use of
         * ```cpp
         * jpcre2::select<char>::Regex re;
         * re = jpcre2::select<char>::Regex("pattern");
         * ```
         * (or such) is discouraged. see `Regex::operator=(const Regex& r)` for details.
         * @see Regex::compile(const String& re, Uint po, Uint jo)
         * @see Regex::compile(const String& re, Uint po)
         * @see Regex::compile(const String& re, const std::string& mod)
         * @see Regex::compile(const String& re)
         * */
        void compile(void); 

        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using information from class variables.
         *  @param re Pattern string
         *  @param po PCRE2 option
         *  @param jo JPCRE2 option
         * */
        void compile(const String& re, Uint po, Uint jo) { 
            setPattern(re).setPcre2Option(po).setJpcre2Option(jo);
            compile(); 
        } 

        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using information from class variables.
         *  @param re Pointer to pattern string
         *  @param po PCRE2 option
         *  @param jo JPCRE2 option
         * */
        void compile(const String* re, Uint po, Uint jo) { 
            setPattern(re).setPcre2Option(po).setJpcre2Option(jo);
            compile(); 
        } 
        
        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pattern string
         *  @param po PCRE2 option
         * */
        void compile(const String& re, Uint po) { 
            setPattern(re).setPcre2Option(po);
            compile(); 
        } 
        
        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re  Pointer to pattern string
         *  @param po PCRE2 option
         * */
        void compile(const String* re, Uint po) { 
            setPattern(re).setPcre2Option(po);
            compile(); 
        } 

        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pattern string
         *  @param mod Modifier string
         * */
        void compile(const String& re, const std::string& mod) { 
            setPattern(re).setModifier(mod);
            compile(); 
        } 

        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pointer to pattern string
         *  @param mod Modifier string
         * */
        void compile(const String* re, const std::string& mod) {
            setPattern(re).setModifier(mod);
            compile(); 
        } 

        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pattern string
         * */
        void compile(const String& re) { 
            setPattern(re);
            compile(); 
        } 

        /** @overload
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pointer to pattern string
         * */
        void compile(const String* re) {
            setPattern(re);
            compile(); 
        } 
        
        /** Perform regex match and return match count.
         *  This function takes the parameters, then sets the parameters to RegexMatch class and calls
         *  RegexMatch::match() which returns the result.
         *
         *  1. It makes use of any previously initialized match object i.e it uses Regex::getMatchObject() function to get a reference to the match object.
         *  2. It retains all options that are set previously unless overridden.
         *  3. Resets all JPCRE2 and PCRE2 options and re-initializes them according to modifier string.
         *  4. Overwrites subject string for match.
         * 
         *  @param s Subject string.
         *  @param mod Modifier string.
         *  @param start_offset Offset from where matching will start in the subject string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s, const std::string& mod, PCRE2_SIZE start_offset) {
            return getMatchObject().setStartOffset(start_offset).setSubject(s).setModifier(mod).match(); 
        } 
        
        ///@overload
        ///
        ///
        ///@param s Pointer to subject string.
        ///@param mod Modifier string.
        ///@param start_offset Offset from where matching will start in the subject string.
        ///@return Match count
        SIZE_T match(const String* s, const std::string& mod, PCRE2_SIZE start_offset) {
            return getMatchObject().setStartOffset(start_offset).setSubject(s).setModifier(mod).match(); 
        } 
        
        /** @overload
         * 
         * 
         *  1. Uses previously set start offset and other options.
         *  2. Resets all JPCRE2 and PCRE2 options and re-initializes them according to modifier string.
         *  3. Overwrites subject string for match.
         *  4. Be ware that previous value of start offset is used.
         * 
         *  @param s Subject string.
         *  @param mod Modifier string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s, const std::string& mod) { 
            return getMatchObject().setSubject(s).setModifier(mod).match(); 
        } 
        
        
        ///@overload
        ///
        ///
        ///@param s Pointer to subject string.
        ///@param mod Modifier string.
        ///@return Match count
        ///@see RegexMatch::match()
        SIZE_T match(const String* s, const std::string& mod) { 
            return getMatchObject().setSubject(s).setModifier(mod).match(); 
        } 

        /** @overload
         * 
         * 
         *  * Overwrites subject string and start offset.
         *  * Uses previously set options.
         * 
         *  @param s Subject string
         *  @param start_offset Offset from where matching will start in the subject string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s,  PCRE2_SIZE start_offset) { 
            return getMatchObject().setStartOffset(start_offset).setSubject(s).match(); 
        } 

        /** @overload
         * 
         *  @param s Pointer to subject string
         *  @param start_offset Offset from where matching will start in the subject string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String* s,  PCRE2_SIZE start_offset) { 
            return getMatchObject().setStartOffset(start_offset).setSubject(s).match(); 
        }
        
        /** @overload
         * 
         * 
         *  1. Overwrites subject string for match.
         *  2. Uses previously set start offset and other options.
         * 
         *  @param s Subject string
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s) { 
            return getMatchObject().setSubject(s).match(); 
        }
        
        /** @overload
         * 
         *  @param s Pointer to subject string
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String* s) { 
            return getMatchObject().setSubject(s).match(); 
        }
        
        /** @overload
         * 
         * 
         *  @param start_offset Offset from where matching will start in the subject string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(PCRE2_SIZE start_offset) { 
            return getMatchObject().setStartOffset(start_offset).match(); 
        }
        
        /** @overload
         * 
         * 
         *  Uses previously set start offset and other options.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match() { 
            return getMatchObject().match(); 
        }
        
        /** Perform regex replace and return the replaced string.
         *  This function takes the parameters, then sets the parameters to RegexReplace class and calls
         *  RegexReplace::replace() which returns the result.
         *
         *  1. It makes use of any previously initialized replace object i.e it uses Regex::getReplaceObject() function to get a reference to the replace object.
         *  2. Resets all JPCRE2 and PCRE2 options and re-initializes them according to modifier string.
         * 
         *  @param mains Subject string
         *  @param repl String to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String& repl, const std::string& mod) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 
        
        /**@overload
         *  @param mains Pointer to subject string
         *  @param repl String to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String& repl, const std::string& mod) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 
        
        /**@overload
         *  @param mains Subject string
         *  @param repl Pointer to string to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String* repl, const std::string& mod) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 
        
        /**@overload
         *  @param mains Pointer to subject string
         *  @param repl Pointer to string to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String* repl, const std::string& mod) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 

        /** @overload
         *  @param mains Subject string
         *  @param repl String to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String& repl) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).replace(); 
        } 
        
        /** @overload
         *  @param mains Pointer to subject string
         *  @param repl String to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String& repl) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).replace(); 
        } 
        
        /** @overload
         *  @param mains Subject string
         *  @param repl Pointer to string to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String* repl) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).replace(); 
        } 
        
        /** @overload
         *  @param mains Pointer to subject string
         *  @param repl Pointer to string to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String* repl) { 
            return getReplaceObject().setSubject(mains).setReplaceWith(repl).replace(); 
        } 

        /** @overload
         *  @param mains Subject string
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains) { 
            return getReplaceObject().setSubject(mains).replace();
        } 

        /** @overload
         *  @param mains Pointer to subject string
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains) { 
            return getReplaceObject().setSubject(mains).replace();
        } 

        /** @overload
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace() { 
            return getReplaceObject().replace(); 
        } 
    };
    
};//struct select
}//jpcre2 namespace

    
template<typename Char_T, jpcre2::Ush BS>
std::string jpcre2::select<Char_T, BS>::Regex::getModifier(){
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < strlen(MOD::C_N); ++i){
        if( (MOD::C_V[i] & compile_opts) != 0 && 
            (MOD::C_V[i] & compile_opts) == MOD::C_V[i]) //One option can include other
            temp += MOD::C_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < strlen(MOD::CJ_N); ++i){
        if( (MOD::CJ_V[i] & jpcre2_compile_opts) != 0 && 
            (MOD::CJ_V[i] & jpcre2_compile_opts) == MOD::CJ_V[i]) //One option can include other
            temp += MOD::CJ_N[i];
    }
    return temp;
}



template<typename Char_T, jpcre2::Ush BS>
std::string jpcre2::select<Char_T, BS>::RegexMatch::getModifier(){
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < strlen(MOD::M_N); ++i){
        if( (MOD::M_V[i] & match_opts) != 0 && 
            (MOD::M_V[i] & match_opts) == MOD::M_V[i]) //One option can include other
            temp += MOD::M_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < strlen(MOD::MJ_N); ++i){
        if( (MOD::MJ_V[i] & jpcre2_match_opts) != 0 && 
            (MOD::MJ_V[i] & jpcre2_match_opts) == MOD::MJ_V[i]) //One option can include other
            temp += MOD::MJ_N[i];
    }
    return temp;
}

template<typename Char_T, jpcre2::Ush BS>
std::string jpcre2::select<Char_T, BS>::RegexReplace::getModifier(){
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < strlen(MOD::R_N); ++i){
        if( (MOD::R_V[i] & replace_opts) != 0 &&
            (MOD::R_V[i] & replace_opts) == MOD::R_V[i]) //One option can include other
            temp += MOD::R_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < strlen(MOD::RJ_N); ++i){
        if( (MOD::RJ_V[i] & jpcre2_replace_opts) != 0 &&
            (MOD::RJ_V[i] & jpcre2_replace_opts) == MOD::RJ_V[i]) //One option can include other
            temp += MOD::RJ_N[i];
    }
    return temp;
}


template<typename Char_T, jpcre2::Ush BS>
typename jpcre2::select<Char_T, BS>::Regex& 
            jpcre2::select<Char_T, BS>::Regex::
                changeModifier(const std::string& mod, bool x) {
    //loop through mod
    for (SIZE_T i = 0; i < mod.length(); ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < strlen(MOD::CJ_N); ++j){
            if(MOD::CJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::CJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< strlen(MOD::C_N); ++j){
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




template<typename Char_T, jpcre2::Ush BS>
void jpcre2::select<Char_T, BS>::Regex::compile() {
    //Get c_str of pattern
    Pcre2Sptr c_pattern = (Pcre2Sptr) pat_str_ptr->c_str();

    /**************************************************************************
     * Compile the regular expression pattern, and handle 
     * any errors that are detected.                                          
     *************************************************************************/

    //first release any previous memory
    freeRegexMemory();
    code = Pcre2Func<BS>::compile(  c_pattern,              /* the pattern */
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
        int jit_ret = Pcre2Func<BS>::jit_compile(code, PCRE2_JIT_COMPLETE);
        if(jit_ret < 0) error_number = jit_ret;
    }
    //everythings OK
    error_number = 0;
    error_offset = 0;
}



//////////////////


// RegexReplace class


template<typename Char_T, jpcre2::Ush BS>
typename jpcre2::select<Char_T, BS>::RegexReplace&
            jpcre2::select<Char_T, BS>::RegexReplace::
                changeModifier(const std::string& mod, bool x) {
    //loop through mod
    for (SIZE_T i = 0; i < mod.length(); ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < strlen(MOD::RJ_N); ++j){
            if(MOD::RJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::RJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< strlen(MOD::R_N); ++j){
            if(MOD::R_N[j] == mod[i]){
                changePcre2Option(MOD::R_V[j], x);
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



template<typename Char_T, jpcre2::Ush BS>
typename jpcre2::select<Char_T, BS>::String jpcre2::select<Char_T, BS>::RegexReplace::replace() {

    // If code is null, return the subject string unmodified.
    if (!re || re->code == 0)
        return *r_subject_ptr;

    Pcre2Sptr subject = (Pcre2Sptr) r_subject_ptr->c_str();
    PCRE2_SIZE subject_length = r_subject_ptr->length() /* Strlen((Char *) subject) */;
    Pcre2Sptr replace = (Pcre2Sptr) r_replw_ptr->c_str();
    PCRE2_SIZE replace_length = r_replw_ptr->length() /* Strlen((Char *) replace) */;
    PCRE2_SIZE outlengthptr = (PCRE2_SIZE) buffer_size;
    bool retry = true;
    int ret = 0;
    Pcre2Uchar* output_buffer = new Pcre2Uchar[outlengthptr + 1];

    while (true) {
        ret = Pcre2Func<BS>::substitute(re->code,    /*Points to the compiled pattern*/
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
                /// If initial #buffer_size wasn't big enough for resultant string, 
                /// we will try once more with a new buffer size adjusted to the length of the resultant string.
                delete[] output_buffer;
                output_buffer = new Pcre2Uchar[outlengthptr + 1];
                // Go and try to perform the substitute again
                continue;
            } else {
                error_number = ret;
                delete[] output_buffer;
                return *r_subject_ptr;
            }
        }
        //If everything's ok exit the loop
        break;
    }
    String result = toString((Char*) output_buffer);
    delete[] output_buffer;
    return result;
}



/////////////////

// RegexMatch class

template<typename Char_T, jpcre2::Ush BS>
typename jpcre2::select<Char_T, BS>::RegexMatch& jpcre2::select<Char_T, BS>::RegexMatch::changeModifier(const std::string& mod, bool x) {
    //loop through mod
    for (SIZE_T i = 0; i < mod.length(); ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < strlen(MOD::MJ_N); ++j){
            if(MOD::MJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::MJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< strlen(MOD::M_N); ++j){
            if(MOD::M_N[j] == mod[i]){
                changePcre2Option(MOD::M_V[j], x);
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


template<typename Char_T, jpcre2::Ush BS>
bool jpcre2::select<Char_T, BS>::RegexMatch::getNumberedSubstrings(int rc, MatchData *match_data) {
    String value;
    PCRE2_SIZE bufflen = 0;
    Pcre2Uchar *buffer = 0;
    int ret = 0;
    
    for (int i = 0; i < rc; i++) {
        //If we use pcre2_substring_get_bynumber(),
        //we will have to deal with returned error codes and memory
        ret = Pcre2Func<BS>::substring_get_bynumber(match_data, (Uint) i, &buffer,
                &bufflen);
        if (ret < 0) {
            switch (ret) {
            case PCRE2_ERROR_NOMEMORY:
                error_number = ret;
                return false;
            default:
                break;   ///Errors other than PCRE2_ERROR_NOMEMORY error are ignored
            }
        }
        value = toString((Char*) buffer);
        Pcre2Func<BS>::substring_free(buffer);     //must free memory
        buffer = 0; //we are going to use it again.
        //if (num_sub)   //This null check is paranoid, this function shouldn't be called if this vector is null
        num_sub->push_back(value); 
    }
    return true;
}



template<typename Char_T, jpcre2::Ush BS>
bool jpcre2::select<Char_T, BS>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size,
                                                            Pcre2Sptr tabptr, MatchData *match_data) {

    String key, value, value1;
    PCRE2_SIZE bufflen = 0;
    Pcre2Uchar *buffer = 0;
    int ret = 0;
    
    for (int i = 0; i < namecount; i++) {
        int n;
        if(BS == 8){
            n = (int)((tabptr[0] << 8) | tabptr[1]);
            key = toString((Char*) (tabptr + 2));
        }
        else{
            n = (int)tabptr[0];
            key = toString((Char*) (tabptr + 1));
        }
        //Use of tabptr is finished for this iteration, let's increment it now.
        tabptr += name_entry_size;
        
        ret = Pcre2Func<BS>::substring_get_byname(match_data,
                (Pcre2Sptr) key.c_str(), &buffer, &bufflen);
        if (ret < 0) {
            switch (ret) {
            case PCRE2_ERROR_NOMEMORY:
                error_number = ret;
                return false;
            default:
                break;   ///Errors other than PCRE2_ERROR_NOMEMORY error are ignored
            }
        }
        value = toString((Char *) buffer);
        Pcre2Func<BS>::substring_free(buffer);     //must free memory
        buffer = 0; //we may use this pointer again, better initialize it.
        

        if(ntn_map) {
            //Let's get the value again, this time with number
            //We will match this value with the previous one.
            //If they match, we got the right one.
            //Otherwise the number is not valid for the corresponding name and
            //we will skip this iteration.
            
            //Don't use pcre2_substring_number_from_name() to get the number for the name (It's messy with dupnames).
            ret = Pcre2Func<BS>::substring_get_bynumber(match_data, (Uint) n, &buffer,
                    &bufflen);
            if (ret < 0) {
                switch (ret) {
                case PCRE2_ERROR_NOMEMORY:
                    error_number = ret;
                    return false;
                default:
                    break;   ///Errors other than PCRE2_ERROR_NOMEMORY error are ignored
                }
            }
            value1 = toString((Char *) buffer);
            Pcre2Func<BS>::substring_free(buffer);     //must free memory
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



template<typename Char_T, jpcre2::Ush BS>
jpcre2::SIZE_T jpcre2::select<Char_T, BS>::RegexMatch::match() {

    // If re or re->code is null, return 0 as the match count
    if (!re || re->code == 0)
        return 0;

    Pcre2Sptr subject = (Pcre2Sptr) m_subject_ptr->c_str();
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
    subject_length = m_subject_ptr->length();
    

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

    match_data = Pcre2Func<BS>::match_data_create_from_pattern(re->code, 0);

    rc = Pcre2Func<BS>::match(  re->code,       /* the compiled pattern */
                                subject,        /* the subject string */
                                subject_length, /* the length of the subject */
                                _start_offset,  /* start at offset 'start_offset' in the subject */
                                match_opts,     /* default options */
                                match_data,     /* block for storing the result */
                                0);             /* use default match context */

    /* Matching failed: handle error cases */

    if (rc < 0) {
        Pcre2Func<BS>::match_data_free(match_data); /* Release memory used for the match */
        //must not free code. This function has no right to modify regex
        switch (rc) {
            case PCRE2_ERROR_NOMATCH:
                return count;
                /*
                 Handle other special cases if you like
                 */
            default:;
        }
        error_number = rc;
        return count;
    }

    ++count; //Increment the counter
    /* Match succeded. Get a pointer to the output vector, where string offsets are
     stored. */
    ovector = Pcre2Func<BS>::get_ovector_pointer(match_data);

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

    // Get numbered substrings if #num_sub isn't null
    if (num_sub) { //must do null check
        if(!getNumberedSubstrings(rc, match_data))
            return count;
    }

    /* See if there are any named substrings, and if so, show them by name. First
     we have to extract the count of named parentheses from the pattern. */

    (void) Pcre2Func<BS>::pattern_info( re->code,               /* the compiled pattern */
                                        PCRE2_INFO_NAMECOUNT,   /* get the number of named substrings */
                                        &namecount);            /* where to put the answer */

    if (namecount <= 0); /*No named substrings*/

    else {
        Pcre2Sptr tabptr;

        /* Before we can access the substrings, we must extract the table for
         translating names to numbers, and the size of each entry in the table. */

        (void) Pcre2Func<BS>::pattern_info( re->code,               /* the compiled pattern */
                                            PCRE2_INFO_NAMETABLE,   /* address of the table */
                                            &name_table);           /* where to put the answer */

        (void) Pcre2Func<BS>::pattern_info( re->code,                   /* the compiled pattern */
                                            PCRE2_INFO_NAMEENTRYSIZE,   /* size of each entry in the table */
                                            &name_entry_size);          /* where to put the answer */

        /* Now we can scan the table and, for each entry, print the number, the name,
         and the substring itself. In the 8-bit library the number is held in two
         bytes, most significant first. */

        tabptr = name_table;

        // Get named substrings if #nas_map isn't null.
        // Get name to number map if #ntn_map isn't null.
        if (nas_map || ntn_map) {
            if(!getNamedSubstrings(namecount, name_entry_size, tabptr, match_data))
                return count;
        }

    }

    // Populate vectors with their associated maps.
    pushMapsIntoVectors();
    
    _end_offset = ovector[1]; //store where current match has ended, it may be the last one.

    /***********************************************************************//*
     * If the "g" modifier was given, we want to continue                     *
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
        Pcre2Func<BS>::match_data_free(match_data); /* Release the memory that was used */
        // Must not free code. This function has no right to modify regex.
        return count; /* Exit the program. */
    }

    /* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
     sequence. First, find the options with which the regex was compiled and extract
     the UTF state. */

    (void) Pcre2Func<BS>::pattern_info(re->code, PCRE2_INFO_ALLOPTIONS, &option_bits);
    utf = ((option_bits & PCRE2_UTF) != 0);
    
    /* Now find the newline convention and see whether CRLF is a valid newline
     sequence. */

    (void) Pcre2Func<BS>::pattern_info(re->code, PCRE2_INFO_NEWLINE, &newline);
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

        rc = Pcre2Func<BS>::match(  re->code,       /* the compiled pattern */
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
                break;                          /* All matches found */
            ovector[1] = start_offset + 1; /* Advance one code unit */
            if (crlf_is_newline &&                      /* If CRLF is newline & */
                start_offset < subject_length - 1 &&    /* we are at CRLF, */
                subject[start_offset] == '\r' && subject[start_offset + 1] == '\n')
                ovector[1] += 1;                        /* Advance by one more. */
            else if (utf) { /* advance a whole UTF (8 or 16), for UTF-32, it's not needed */
                while (ovector[1] < subject_length) {
                    if(BS == 8 && (subject[ovector[1]] & 0xc0) != 0x80) break;
                    else if(BS == 16 && (subject[ovector[1]] & 0xfc00) != 0xdc00) break;
                    else if(BS == 32) break; //must be else if
                    ovector[1] += 1;
                }
            }
            continue; /* Go round the loop again */
        }

        /* Other matching errors are not recoverable. */

        if (rc < 0) {
            Pcre2Func<BS>::match_data_free(match_data);
            // Must not free code. This function has no right to modify regex.
            error_number = rc;
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

        _end_offset = ovector[1]; //store where current match has ended, overwrite to contain the last one.
        
    } /* End of loop to find second and subsequent matches */

    Pcre2Func<BS>::match_data_free(match_data);
    // Must not free code. This function has no right to modify regex.
    return count;
}

#ifdef JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION
#undef JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION
#endif

//some macro documentation for doxygen

#ifdef __DOXYGEN__

///@def JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION
///By default JPCRE2 checks if the code unit width equals to
///sizeof(Char_T)*CHAR_BIT, if not,it will produce compile error.
///This check can be disabled by defining this macro.
#define JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION

#endif


#endif
