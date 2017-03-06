/* *****************************************************************************
 * ******************* C++ wrapper for PCRE2 Library ****************************
 * *****************************************************************************
 *            Copyright (c) 2015-2017 Md. Jahidul Hamid
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
 * It includes the `pcre2.h` header, therefore you shouldn't include `pcre2.h`, neither should you define `PCRE2_CODE_UNIT_WIDTH` before including
 * `jpcre2.hpp`.
 * If your `pcre2.h` header is not in standard include paths, you may include `pcre2.h` with correct path before including `jpcre2.hpp`
 * manually. In this case you will have to define `PCRE2_CODE_UNIT_WIDTH` before including `pcre2.h`.
 * Make sure to link required PCRE2 libraries when compiling.
 * 
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 */

#ifndef JPCRE2_HPP
#define JPCRE2_HPP

#ifndef PCRE2_CODE_UNIT_WIDTH

///@def PCRE2_CODE_UNIT_WIDTH
///This macro does not have any significance in JPCRE2 context.
///It is defined as 0 by default. Defining it before including jpcre2.hpp
///will override the default (discouraged as it will make it harder for you to detect problems),
///but still it will have no effect in a JPCRE2 perspective.
///Defining it with an invalid value will yield to compile error.
#define PCRE2_CODE_UNIT_WIDTH 0
#endif

//previous inclusion of pcre2.h will be respected and we won't try to include it twice.
//Thus one can pre-include pcre2.h from an arbitrary/non-standard path.
#ifndef PCRE2_MAJOR
    #include <pcre2.h>  // pcre2 header
#endif
#include <string>       // std::string, std::wstring
#include <vector>       // std::vector
#include <map>          // std::map
#include <cstdio>       // std::sprintf
#include <cwchar>       // std::mbstate_t, std::swprintf
//~ #include <climits>      // CHAR_BIT //[redundant] limits.h is included by pcre2.h

#if __cplusplus >= 201103L
    #if !defined JPCRE2_DISABLE_CHAR1632
        #include <locale>       // std::codecvt, std::wstring_convert
    #endif
    #ifndef JPCRE2_USE_FUNCTION_POINTER_CALLBACK
        #include <functional>   // std::function
    #endif
#endif

#define JPCRE2_UNUSED(x) ((void)(x))

#ifndef NDEBUG
    #define JPCRE2_ASSERT(cond, msg) jpcre2::assert(cond, msg, __FILE__, __LINE__)
    #define JPCRE2_VECTOR_DATA_ASSERT(cond, name) \
    jpcre2::assert(cond, \
    std::string("ValueError: \n\
    Required data vector of type ")+std::string(name)+" is empty.\n\
    Your MatchEvaluator callback function is not\n\
    compatible with existing data!!\n\
    You are trying to use a vector that does not\n\
    have any match data. Either call nreplace()\n\
    with true or perform a match with appropriate\n\
    callback function. For more details, refer to\n\
    the doc in MatchEvaluator section.", __FILE__, __LINE__)
#else
    #define JPCRE2_ASSERT(cond, msg) ((void)0)
    #define JPCRE2_VECTOR_DATA_ASSERT(cond, name) ((void)0)
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


///Define for JPCRE2 version.
///It can be used to support changes in different versions of the lib.
#define JPCRE2_VERSION 102903L

/** @namespace jpcre2::INFO
 *  Namespace to provide information about JPCRE2 library itself.
 *  Contains constant Strings with version info.
 */
namespace INFO {
    static const char NAME[] = "JPCRE2";               ///< Name of the project
    static const char FULL_VERSION[] = "10.29.03";     ///< Full version string
    static const char VERSION_GENRE[] = "10";          ///< Generation, depends on original PCRE2 version
    static const char VERSION_MAJOR[] = "29";          ///< Major version, updated when API change is made
    static const char VERSION_MINOR[] = "03";          ///< Minor version, includes bug fix or minor feature upgrade
    static const char VERSION_PRE_RELEASE[] = "";      ///< Alpha or beta (testing) release version
}


typedef PCRE2_SIZE SIZE_T;                          ///< Used for match count and vector size
typedef uint32_t Uint;                              ///< Used for options (bitwise operation)
typedef uint8_t Ush;                                ///< 8 bit unsigned integer.
typedef std::vector<SIZE_T> VecOff;                 ///< vector of size_t

/// @namespace jpcre2::ERROR
/// Namespace for error codes.
namespace ERROR {
    /** Error numbers for JPCRE2.
     *  JPCRE2 error numbers are positive integers while
     *  PCRE2 error numbers are negative integers.
     */
    enum {
        INVALID_MODIFIER        = 2,  ///< Invalid modifier was detected
        INSUFFICIENT_OVECTOR    = 3   ///< Ovector was not big enough during a match
    };
}


/** These constants provide JPCRE2 options.
 */
enum {
    NONE                    = 0x0000000u,           ///< Option 0 (zero)
    FIND_ALL                = 0x0000002u,           ///< Find all during match (global match)
    JIT_COMPILE             = 0x0000004u            ///< Perform JIT compilation for optimization
};

///JPCRE2 assert function.
///Aborts with an error message if condition fails.
///@param cond boolean condition
///@param msg message (std::string)
///@param file file where assert was called.
///@param line line number where assert was called.
void assert(bool cond, std::string msg, std::string file, size_t line){
    if(!cond) {
        std::cerr<<"\nE: AssertionFailure: "<<msg<<"\nAssertion failed in File: "<<file<<"\tat Line: "<<line<<"\n";
        ::abort(); //abort is defined in stdlib.h which is included by pcre2.h
    }
}

//enableif and is_same implementation
template<bool B, typename T = void>
struct EnableIf{};
template<typename T>
struct EnableIf<true, T>{typedef T Type;};

template<typename T1, typename T2>
struct IsSame{ static const bool value = false; };
template<typename T>
struct IsSame<T,T>{ static const bool value = true; };


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
    typedef pcre2_jit_callback_8 JitCallback;
    typedef pcre2_jit_stack_8 JitStack;
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
    typedef pcre2_jit_callback_16 JitCallback;
    typedef pcre2_jit_stack_16 JitStack;
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
    typedef pcre2_jit_callback_32 JitCallback;
    typedef pcre2_jit_stack_32 JitStack;
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
    static Pcre2Type<8>::CompileContext* compile_context_copy(Pcre2Type<8>::CompileContext* ccontext){
    return pcre2_compile_context_copy_8(ccontext); 
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
    //~ static typename Pcre2Type<8>::Pcre2Code * code_copy(const typename Pcre2Type<8>::Pcre2Code *code){
        //~ return pcre2_code_copy_8(code);
    //~ }
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
    static void jit_stack_assign(typename Pcre2Type<8>::MatchContext *mcontext, 
                                 typename Pcre2Type<8>::JitCallback callback_function,
                                 void *callback_data){
        pcre2_jit_stack_assign_8(mcontext, callback_function, callback_data);
    }
    static typename Pcre2Type<8>::JitStack *jit_stack_create(PCRE2_SIZE startsize, PCRE2_SIZE maxsize,
                                                             typename Pcre2Type<8>::GeneralContext *gcontext){
    return pcre2_jit_stack_create_8(startsize, maxsize, gcontext);
    }
    static void jit_stack_free(typename Pcre2Type<8>::JitStack *jit_stack){
        pcre2_jit_stack_free_8(jit_stack);
    }
    static void jit_free_unused_memory(typename Pcre2Type<8>::GeneralContext *gcontext){
        pcre2_jit_free_unused_memory_8(gcontext);
    }
    static typename Pcre2Type<8>::MatchContext *match_context_create(typename Pcre2Type<8>::GeneralContext *gcontext){
        return pcre2_match_context_create_8(gcontext);
    }
    static typename Pcre2Type<8>::MatchContext *match_context_copy(typename Pcre2Type<8>::MatchContext *mcontext){
        return pcre2_match_context_copy_8(mcontext);
    }
    static void match_context_free(typename Pcre2Type<8>::MatchContext *mcontext){
        pcre2_match_context_free_8(mcontext);
    }
    static uint32_t get_ovector_count(typename Pcre2Type<8>::MatchData *match_data){
        return pcre2_get_ovector_count_8(match_data);
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
    static Pcre2Type<16>::CompileContext* compile_context_copy(Pcre2Type<16>::CompileContext* ccontext){
    return pcre2_compile_context_copy_16(ccontext); 
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
    //~ static typename Pcre2Type<16>::Pcre2Code * code_copy(const typename Pcre2Type<16>::Pcre2Code *code){
        //~ return pcre2_code_copy_16(code);
    //~ }
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
    static void jit_stack_assign(typename Pcre2Type<16>::MatchContext *mcontext, 
                                 typename Pcre2Type<16>::JitCallback callback_function,
                                 void *callback_data){
        pcre2_jit_stack_assign_16(mcontext, callback_function, callback_data);
    }
    static typename Pcre2Type<16>::JitStack *jit_stack_create(PCRE2_SIZE startsize, PCRE2_SIZE maxsize,
                                                             typename Pcre2Type<16>::GeneralContext *gcontext){
    return pcre2_jit_stack_create_16(startsize, maxsize, gcontext);
    }
    static void jit_stack_free(typename Pcre2Type<16>::JitStack *jit_stack){
        pcre2_jit_stack_free_16(jit_stack);
    }
    static void jit_free_unused_memory(typename Pcre2Type<16>::GeneralContext *gcontext){
        pcre2_jit_free_unused_memory_16(gcontext);
    }
    static typename Pcre2Type<16>::MatchContext *match_context_create(typename Pcre2Type<16>::GeneralContext *gcontext){
        return pcre2_match_context_create_16(gcontext);
    }
    static typename Pcre2Type<16>::MatchContext *match_context_copy(typename Pcre2Type<16>::MatchContext *mcontext){
        return pcre2_match_context_copy_16(mcontext);
    }
    static void match_context_free(typename Pcre2Type<16>::MatchContext *mcontext){
        pcre2_match_context_free_16(mcontext);
    }
    static uint32_t get_ovector_count(typename Pcre2Type<16>::MatchData *match_data){
        return pcre2_get_ovector_count_16(match_data);
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
    static Pcre2Type<32>::CompileContext* compile_context_copy(Pcre2Type<32>::CompileContext* ccontext){
    return pcre2_compile_context_copy_32(ccontext); 
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
    //~ static typename Pcre2Type<32>::Pcre2Code * code_copy(const typename Pcre2Type<32>::Pcre2Code *code){
        //~ return pcre2_code_copy_32(code);
    //~ }
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
    
    
    static void jit_stack_assign(typename Pcre2Type<32>::MatchContext *mcontext, 
                                 typename Pcre2Type<32>::JitCallback callback_function,
                                 void *callback_data){
        pcre2_jit_stack_assign_32(mcontext, callback_function, callback_data);
    }
    static typename Pcre2Type<32>::JitStack *jit_stack_create(PCRE2_SIZE startsize, PCRE2_SIZE maxsize,
                                                             typename Pcre2Type<32>::GeneralContext *gcontext){
    return pcre2_jit_stack_create_32(startsize, maxsize, gcontext);
    }
    static void jit_stack_free(typename Pcre2Type<32>::JitStack *jit_stack){
        pcre2_jit_stack_free_32(jit_stack);
    }
    static void jit_free_unused_memory(typename Pcre2Type<32>::GeneralContext *gcontext){
        pcre2_jit_free_unused_memory_32(gcontext);
    }
    
    static typename Pcre2Type<32>::MatchContext *match_context_create(typename Pcre2Type<32>::GeneralContext *gcontext){
        return pcre2_match_context_create_32(gcontext);
    }
    static typename Pcre2Type<32>::MatchContext *match_context_copy(typename Pcre2Type<32>::MatchContext *mcontext){
        return pcre2_match_context_copy_32(mcontext);
    }
    static void match_context_free(typename Pcre2Type<32>::MatchContext *mcontext){
        pcre2_match_context_free_32(mcontext);
    }
    static uint32_t get_ovector_count(typename Pcre2Type<32>::MatchData *match_data){
        return pcre2_get_ovector_count_32(match_data);
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
    // Uint is being used in getModifier() in for loop to get the number of element in this array,
    // be sure to chnage there if you change here.
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
    static const jpcre2::Uint RJ_V[1] = { NONE  //placeholder
                                              };

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
    static std::basic_string<Char_T> INSUFFICIENT_OVECTOR(void);
};
//specialization
template<> inline std::basic_string<char> MSG<char>::INVALID_MODIFIER(){ return "Invalid modifier: "; }
template<> inline std::basic_string<wchar_t> MSG<wchar_t>::INVALID_MODIFIER(){ return L"Invalid modifier: "; }
template<> inline std::basic_string<char> MSG<char>::INSUFFICIENT_OVECTOR(){ return "ovector wasn't big enough"; }
template<> inline std::basic_string<wchar_t> MSG<wchar_t>::INSUFFICIENT_OVECTOR(){ return L"ovector wasn't big enough"; }
#if __cplusplus >= 201103L && !defined JPCRE2_DISABLE_CHAR1632
template<> inline std::basic_string<char16_t> MSG<char16_t>::INVALID_MODIFIER(){ return u"Invalid modifier: "; }
template<> inline std::basic_string<char32_t> MSG<char32_t>::INVALID_MODIFIER(){ return U"Invalid modifier: "; }
template<> inline std::basic_string<char16_t> MSG<char16_t>::INSUFFICIENT_OVECTOR(){ return u"ovector wasn't big enough"; }
template<> inline std::basic_string<char32_t> MSG<char32_t>::INSUFFICIENT_OVECTOR(){ return U"ovector wasn't big enough"; }
#endif


#if __cplusplus >= 201103L && !defined JPCRE2_DISABLE_CHAR1632

//Convenience wrapper of std::codecvt (`>=C++11`)
template <class internT, class externT, class stateT>
struct Codecvt : std::codecvt<internT,externT,stateT>
{ ~Codecvt(){} };

///@struct ConvUTF
///`UTF-8 <> UTF-16` and `UTF-8 <> UTF32` converter (>=C++11).
///
///Convert `UTF-16 <> UTF-8`:
/// ```cpp
/// ConvUTF<char16_t>::Converter conv;
/// //UTF-16 to UTF-8
/// std::string s = conv.to_bytes(utf16string);
/// //UTF-8 to UTF-16
/// std::u16string us = conv.from_bytes(utf8string);
/// ```
///Convert `UTF-8 <> UTF-16`:
/// ```cpp
/// ConvUTF<char32_t>::Converter conv;
/// //UTF-8 to UTF-32
/// std::u16string us = conv.from_bytes(utf8string);
/// //UTF-32 to UTF-8
/// std::string s = conv.to_bytes(utf32string);
/// ```
template<typename Char_T>
struct ConvUTF { typedef std::wstring_convert<Codecvt<Char_T, char, std::mbstate_t>, Char_T> Converter; };

///This is a convenience typedef (>=C++11) to convert between UTF-8 <> UTF-16.
///Convert UTF-16 to UTF-8:
///```cpp
///Convert16 conv;
///std::string s = conv.to_bytes(utf16string);
///```
///Convert UTF-8 to UTF-16:
///```cpp
///std::u16string us = conv.from_bytes(utf8string);
///```
typedef ConvUTF<char16_t>::Converter Convert16;

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
typedef ConvUTF<char32_t>::Converter Convert32;
#endif


///@struct ConvInt
///Contains a function to convert integer to string.
///Integer is converted to `std::basic_string<Char_T>`
///@tparam Char_T Basic character type (`char`, `wchar`, `char16_t`, `char32_t`).
template<typename Char_T, typename T = Char_T> struct ConvInt{
    ///Converts an integer to string.
    ///String may be `std::string`, `std::wstring`
    ///(+ `std::u16string`, `std::u32string` if `>=C++11` is used)
    ///@param x the integer to convert
    ///@return `std::string`/`std::wstring`/`std::u16string`/`std::u32string` from the integer
    std::basic_string<Char_T> toString(int x);
};

template<typename Char_T> struct ConvInt<Char_T, typename EnableIf<
IsSame<Char_T, char>::value|IsSame<Char_T, wchar_t>::value, Char_T>::Type>{
    //wrapper of sprintf or swprintf
    static int mysprint(Char_T*, SIZE_T size, int x);
    
    ///Converts an integer to std::string/std::wstring
    ///@param x the integer to convert
    ///@return std::string/std::wstring from the integer
    static std::basic_string<Char_T> toString(int x){
        Char_T buf[sizeof(int)*CHAR_BIT]; //sizeof(int)*CHAR_BIT should always be sufficient
        int written = mysprint(buf, sizeof(buf)/sizeof(Char_T), x);
        JPCRE2_ASSERT(written > 0, "IOError: Failed to write into buffer during int to string conversion.");
        JPCRE2_UNUSED(written); //production happy
        return std::basic_string<Char_T>(buf);
    }
};

template<> inline int ConvInt<char>::mysprint(char* buf, SIZE_T size, int x){
    return std::sprintf(buf, "%d", x);
}
template<> inline int ConvInt<wchar_t>::mysprint(wchar_t* buf, SIZE_T size, int x){
    return std::swprintf(buf, size, L"%d", x);
}


#if __cplusplus >= 201103L && !defined JPCRE2_DISABLE_CHAR1632

template<typename Char_T> struct ConvInt<Char_T, typename EnableIf<
IsSame<Char_T, char16_t>::value|IsSame<Char_T, char32_t>::value, Char_T>::Type>{
    
    ///Converts integer to std::u16string/std::u32string.
    ///Uses `std::codecvt` for conversion.
    ///@param x int to convert
    ///@return std::u16string/std::u32string from the integer
    static std::basic_string<Char_T> toString(int x){
        std::string s = std::to_string(x);
        typename ConvUTF<Char_T>::Converter conv;
        return conv.from_bytes(s);
    }
};
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
///If BS is not given, i.e it is called like `jpcre2::select<Char_T>`.
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
///We will use the following typedef throughout this doc:
///```cpp
///typedef jpcre2::select<Char_T> jp;
///```
template<typename Char_T, Ush BS = sizeof( Char_T ) * CHAR_BIT> 
struct select{

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
    
    //These are to shorten the code
    typedef typename Pcre2Type<BS>::Pcre2Uchar Pcre2Uchar;
    typedef typename Pcre2Type<BS>::Pcre2Sptr Pcre2Sptr;
    typedef typename Pcre2Type<BS>::Pcre2Code Pcre2Code;
    typedef typename Pcre2Type<BS>::CompileContext CompileContext;
    typedef typename Pcre2Type<BS>::MatchData MatchData;
    typedef typename Pcre2Type<BS>::GeneralContext GeneralContext;
    typedef typename Pcre2Type<BS>::MatchContext MatchContext;
    typedef typename Pcre2Type<BS>::JitCallback JitCallback;
    typedef typename Pcre2Type<BS>::JitStack JitStack;
    
    template<typename T>
    static String toString(T); //prevent implicit type conversion of T
    
    ///Converts a Char_T (char, wchar_t, char16_t, char32_t) to jpcre2::select::String
    ///@param a Char_T
    ///@return jpcre2::select::String
    static String toString(Char a){
        if (a) return String(1, a);
        else return String();
    }                     
    
    ///@overload
    ///...
    ///Converts a const Char_T* (char*, wchar_t*, char16_t*, char32_t*) to jpcre2::select::String
    ///@param a const Char_T*
    ///@return jpcre2::select::String
    static String toString(const Char* a){
        if (a) return String(a);
        else return String();
    }
    
    ///@overload
    ///...
    ///Converts a Char_T* (char*, wchar_t*, char16_t*, char32_t*) to jpcre2::select::String
    ///@param a const Char_T*
    ///@return jpcre2::select::String
    static String toString(Char* a){
        if (a) return String(a);
        else return String();
    }              
    
    ///@overload
    ///...
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
    static String getPcre2ErrorMessage(int err_num) {
        Pcre2Uchar buffer[sizeof(Char)*CHAR_BIT*1024];
        Pcre2Func<BS>::get_error_message(err_num, buffer, sizeof(buffer));
        return toString((Pcre2Uchar*) buffer);
    }          
    
    ///Returns error message (either JPCRE2 or PCRE2) from error number and error offset
    ///@param err_num error number (negative for PCRE2, positive for JPCRE2)
    ///@param err_off error offset
    ///@return message as jpcre2::select::String.
    static String getErrorMessage(int err_num, int err_off)  {
        if(err_num == (int)ERROR::INVALID_MODIFIER){
            return MSG<Char>::INVALID_MODIFIER() + toString((Char)err_off);
        } else if(err_num == (int)ERROR::INSUFFICIENT_OVECTOR){
            return MSG<Char>::INSUFFICIENT_OVECTOR();
        } else if(err_num != 0) {
            return getPcre2ErrorMessage((int) err_num) + ConvInt<Char>::toString((int) err_off);
        } else return String();
    }

    //forward declaration
    class Regex;
    class RegexMatch;
    class RegexReplace;
    
    /** Provides public constructors to create RegexMatch objects.
     * Every RegexMatch object should be associated with a Regex object.
     * This class stores a pointer to its' associated Regex object, thus when
     * the content of the associated Regex object is changed, there's no need to
     * set the pointer again.
     * 
     * Examples:
     * 
     * ```cpp
     * jp::Regex re;
     * jp::RegexMatch rm;
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
        MatchContext *mcontext;
        //Managing jit stack spoils thread safety
        //~ JitStack *jit_stack;
        //~ PCRE2_SIZE jit_stack_startsize;
        //~ PCRE2_SIZE jit_stack_maxsize;
        
        PCRE2_SIZE _start_offset; //name collision, use _ at start

        VecNum* vec_num;        
        VecNas* vec_nas;        
        VecNtN* vec_ntn;        
        
        VecOff* vec_soff;
        VecOff* vec_eoff;

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
        //~ void createJitStack(){
            //~ freeJitStack();
            //~ if(jit_stack_startsize) jit_stack = Pcre2Func<BS>::jit_stack_create(jit_stack_startsize, jit_stack_maxsize, 0);
        //~ }
        //~ void createMatchContext(){
            //~ freeMatchContext();
            //~ mcontext = Pcre2Func<BS>::match_context_create(0);
        //~ }
        //~ //this function creates jit_stack if not available.
        //~ void assignJitStack(){
            //~ if(!jit_stack) createJitStack();
            //~ if(jit_stack){//not else
                //~ if(!mcontext) createMatchContext();
                //~ if(mcontext) Pcre2Func<BS>::jit_stack_assign(mcontext, 0, jit_stack);
            //~ }
        //~ }
        //~ void freeJitStack(){
            //~ if(jit_stack) Pcre2Func<BS>::jit_stack_free(jit_stack);
            //~ jit_stack = 0;
        //~ }
        //~ void freeMatchContext(){
            //~ if(mcontext) Pcre2Func<BS>::match_context_free(mcontext);
            //~ mcontext = 0;
        //~ }
        
        void init_vars() {
            re = 0;
            vec_num = 0; 
            vec_nas = 0; 
            vec_ntn = 0;
            vec_soff = 0;
            vec_eoff = 0;
            num_sub = 0; 
            nas_map = 0; 
            ntn_map = 0; 
            match_opts = 0; 
            jpcre2_match_opts = 0; 
            error_number = 0;
            error_offset = 0;
            _start_offset = 0;
            m_subject_ptr = &m_subject;
            mcontext = 0;
            //~ jit_stack = 0;
            //~ jit_stack_startsize = 0;
            //~ jit_stack_maxsize = 0;
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
                        
            //underlying data of vectors are not handled by RegexMatch
            //thus it's safe to just copy the pointers.
            vec_num = rm.vec_num;
            vec_nas = rm.vec_nas;
            vec_ntn = rm.vec_ntn;
            
            vec_soff = rm.vec_soff;
            vec_eoff = rm.vec_eoff;
            
            //maps should be null, no copy needed
            
            match_opts = rm.match_opts;
            jpcre2_match_opts = rm.jpcre2_match_opts;
            error_number = rm.error_number;
            error_offset = rm.error_offset;
            _start_offset = rm._start_offset;
            
            //~ freeMatchContext();
            //~ if(rm.mcontext) mcontext = Pcre2Func<BS>::match_context_copy(rm.mcontext);
            mcontext = rm.mcontext;
            //no need to copy jit_stack, it will be created if needed
            //~ setJitStackSize(rm.jit_stack_startsize, rm.jit_stack_maxsize);
            
        }

        friend class Regex;

        protected:
        
        RegexMatch& setErrorNumber(int x){
            error_number = x;
            return *this;
        }

    public: 
    
        ///Default constructor.
        RegexMatch(){
            init_vars();
        }
    
        ///@overload
        ///...
        ///Creates a RegexMatch object associating a Regex object.
        ///@param r pointer to a Regex object
        RegexMatch(const Regex * r) {
            init_vars();
            re = r;
        }
        
        ///@overload
        ///...
        ///Copy constructor. Performs deep copy.
        ///@param rm Reference to RegexMatch object
        RegexMatch(const RegexMatch& rm){
            init_vars();
            deepCopy(rm);
        }
        
        ///Overloaded copy-assignment operator.
        ///Allows assigning objects:
        ///```cpp
        ///jp::RegexMatch rm;
        ///rm = jp::RegexMatch(&re);
        ///```
        ///@param rm RegexMatch object
        ///@return A reference to the calling RegexMatch object.
        virtual RegexMatch& operator=(const RegexMatch& rm){
            if(this == &rm) return *this;
            deepCopy(rm);
            return *this;
        }
        
        ///Destructor
        ///Frees all internal memories that were used.
        virtual ~RegexMatch() { 
            delete num_sub; 
            delete nas_map; 
            delete ntn_map;
            //~ freeMatchContext();
            //~ freeJitStack();
        } 

        /** Reset all class variables to its default (initial) state.
         * Data in the vectors will retain (It won't delete previous data in vectors)
         * You will need to pass vector pointers again after calling this function to get match results.
         * 
         * @return Reference to the calling RegexMatch object.
         * */
        virtual RegexMatch& reset() { 
            resetMaps();
            m_subject.clear(); //not ptr , external string won't be modified.
            //~ freeMatchContext();
            //~ freeJitStack();
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
        virtual RegexMatch& resetErrors(){
            error_number = 0;
            error_offset = 0;
            return *this;
        }
        
        /// Returns the last error number
        ///@return Last error number
        virtual int getErrorNumber() const { 
            return error_number; 
        } 

        /// Returns the last error offset
        ///@return Last error offset
        virtual int getErrorOffset() const  { 
            return (int)error_offset; 
        } 
        
        /// Returns the last error message
        ///@return Last error message
        virtual String getErrorMessage() const  { 
            return select<Char, BS>::getErrorMessage(error_number, error_offset); 
        } 
        
        ///Get subject string.
        ///@return subject string
        ///@see RegexReplace::getSubject()
        virtual String getSubject() const  { 
            return *m_subject_ptr; 
        } 
        
        ///Get pointer to subject string.
        ///Data can not be changed with this pointer.
        ///@return subject string pointer 
        ///@see RegexReplace::getSubjectPointer()
        virtual const String* getSubjectPointer() const  { 
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
        virtual std::string getModifier() const ; 
        
        
        /**Get PCRE2 option
         * @return PCRE2 option for match operation
         * @see Regex::getPcre2Option()
         * @see RegexReplace::getPcre2Option()
         * */
        virtual Uint getPcre2Option() const  { 
            return match_opts; 
        } 

        /** Get JPCRE2 option
         * @return JPCRE2 options for math operation
         * @see Regex::getJpcre2Option()
         * @see RegexReplace::getJpcre2Option()
         * */
        virtual Uint getJpcre2Option() const  { 
            return jpcre2_match_opts; 
        } 
        
        /// Get offset from where match will start in the subject.
        /// @return Start offset
        virtual PCRE2_SIZE getStartOffset() const  {
            return _start_offset;
        }
        
        ///Get match start offset vector pointer.
        ///@return pointer to the match start offset vector
        virtual VecOff* getMatchStartOffsetVector() const {
            return vec_soff;
        }
        
        ///Get match end offset vector pointer.
        ///@return pointer to the end start offset vector
        virtual VecOff* getMatchEndOffsetVector() const {
            return vec_eoff;
        }
        
        ///Get a pointer to the associated Regex object.
        ///If no actual Regex object is associated, null is returned.
        ///@return A pointer to the associated Regex object or null.
        virtual const Regex* getRegexObject() const {
            return re;
        }
        
        ///Get pointer to numbered substring vector.
        ///@return Pointer to numbered substring vector.
        virtual VecNum* getNumberedSubstringVector() const {
            return vec_num;
        }
        
        ///Get pointer to named substring vector.
        ///@return Pointer to named substring vector.
        virtual VecNas* getNamedSubstringVector() const {
            return vec_nas;
        }
        
        ///Get pointer to name to number map vector.
        ///@return Pointer to name to number map vector.
        virtual VecNtN* getNameToNumberMapVector() const {
            return vec_ntn;
        }
        
        ///Set pointer to the associated regex object.
        ///@param r Pointer to a Regex object.
        ///@return Reference to the calling RegexMatch object.
        virtual RegexMatch& setRegexObject(const Regex* r){
            re = r;
            return *this;
        }

        /// Set a pointer to the numbered substring vector.
        /// This vector will be filled with numbered (indexed) captured groups.
        /// @param v pointer to the numbered substring vector
        /// @return Reference to the calling RegexMatch object
        virtual RegexMatch& setNumberedSubstringVector(VecNum* v) { 
            vec_num = v; 
            return *this; 
        } 

        /// Set a pointer to the named substring vector.
        /// This vector will be populated with named captured groups.
        /// @param v pointer to the named substring vector
        /// @return Reference to the calling RegexMatch object
        virtual RegexMatch& setNamedSubstringVector(VecNas* v) { 
            vec_nas = v; 
            return *this; 
        } 

        /// Set a pointer to the name to number map vector.
        /// This vector will be populated with name to number map for captured groups.
        /// @param v pointer to the name to number map vector
        /// @return Reference to the calling RegexMatch object
        virtual RegexMatch& setNameToNumberMapVector(VecNtN* v) { 
            vec_ntn = v; 
            return *this; 
        }
        
        /// Set the pointer to a vector to store the offsets where matches
        /// start in the subject.
        /// @param v Pointer to a jpcre2::VecOff vector (std::vector<size_t>)
        /// @return Reference to the calling RegexMatch object
        virtual RegexMatch& setMatchStartOffsetVector(VecOff* v){
            vec_soff = v;
            return *this;
        }
        
        /// Set the pointer to a vector to store the offsets where matches
        /// end in the subject.
        /// @param v Pointer to a VecOff vector (std::vector<size_t>)
        /// @return Reference to the calling RegexMatch object
        virtual RegexMatch& setMatchEndOffsetVector(VecOff* v){
            vec_eoff = v;
            return *this;
        }

        ///Set the subject string for match.
        ///This makes a copy of the subject string. If a copy is not desirable
        ///or you are working with huge text, consider passing a pointer instead
        ///of a constant reference.
        /// @param s Subject string
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setSubject()
        virtual RegexMatch& setSubject(const String& s) { 
            m_subject = s;
            m_subject_ptr = &m_subject; //must overwrite
            return *this; 
        } 

        ///@overload
        ///...
        ///Set pointer to the subject string for match.
        /// @param s Pointer to subject string
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setSubject()
        virtual RegexMatch& setSubject(const String* s) { 
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
        virtual RegexMatch& setModifier(const std::string& s) { 
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
        virtual RegexMatch& setJpcre2Option(Uint x) { 
            jpcre2_match_opts = x; 
            return *this; 
        } 

        ///Set PCRE2 option match (overwrite existing option)
        /// @param x Option value
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::setPcre2Option()
        /// @see Regex::setPcre2Option()
        virtual RegexMatch& setPcre2Option(Uint x) {
            match_opts = x; 
            return *this; 
        } 
        
        /// Set whether to perform global match
        /// @param x True or False
        /// @return Reference to the calling RegexMatch object
        virtual RegexMatch& setFindAll(bool x) { 
            if (x) 
                jpcre2_match_opts |= FIND_ALL; 
            else 
                jpcre2_match_opts &= ~FIND_ALL; 
            return *this; 
        } 

        ///@overload
        ///...
        ///This function just calls RegexMatch::setFindAll(bool x) with `true` as the parameter
        ///@return Reference to the calling RegexMatch object
        virtual RegexMatch& setFindAll() { 
            return setFindAll(true); 
        }

        /// Set offset from where match starts.
        /// When FIND_ALL is set, a global match would not be performed on all positions on the subject,
        /// rather it will be performed from the start offset and onwards.
        /// @param offset Start offset
        /// @return Reference to the calling RegexMatch object
        virtual RegexMatch& setStartOffset(PCRE2_SIZE offset) {
            _start_offset = offset;
            return *this;
        }
        
        ///Set the match context.
        ///You can create match context using the native PCRE2 API.
        ///The memory is not handled by RegexMatch object and not freed.
        ///User will be responsible for freeing the memory of the match context.
        ///@param match_context Pointer to the match context.
        ///@return Reference to the calling RegexMatch object
        virtual RegexMatch& setMatchContext(MatchContext *match_context){
            mcontext = match_context;
            return *this;
        }
        
        //the following is not thread safe. (pcre2_jit_stack_create function is not thread safe.)
        //~ ///Set JIT stack size.
        //~ ///Some large or complicated pattern may need more than the default stack size (32K).
        //~ ///A call to this function will create a new JIT memory on machine stack exclusively for this match object.
        //~ ///Any and all copies from this match object will also hold their respective exclusive JIT stack.
        //~ ///If this match object is copied into multiple other match objects, it may have a significant memory cost.
        //~ ///You can change/reset it to its default state by setting the startsize (first argument) to 0.
        //~ ///A call to `RegexMatch::reset()` will reset it to default along with others.
        //~ ///
        //~ ///Thread safety:
        //~ /// 
        //~ ///1. MT unsafe if called with non-zero value.
        //~ ///2. Also the next matching operations (RegexMatch::match()) will be MT unsafe.
        //~ ///3. Dummy call to this function is (C) MT safe i.e calling it with zero-value when there was no previous custom JIT stack.
        //~ ///
        //~ ///@param startsize Starting JIT stack size (usually 32*1024).
        //~ ///@param maxsize Maximum size of JIT stack (512*1024 or 1024*1024 should be more than enough). A wrong value, such as less than the startsize will be corrected to startsize.
        //~ ///@return Reference to the calling RegexMatch object
        //~ virtual RegexMatch& setJitStackSize(PCRE2_SIZE startsize, PCRE2_SIZE maxsize){
            //~ jit_stack_startsize = startsize;
            //~ jit_stack_maxsize = maxsize;
            //~ if(jit_stack_maxsize < jit_stack_startsize) jit_stack_maxsize = jit_stack_startsize;
            //~ createJitStack();
            //~ return *this;
        //~ }
        
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
        virtual RegexMatch& changeModifier(const std::string& mod, bool x); 

        /// Parse modifier and add/remove equivalent PCRE2 and JPCRE2 options.
        /// Add or remove a JPCRE2 option
        /// @param opt JPCRE2 option value
        /// @param x Add the option if it's true, remove otherwise.
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::changeJpcre2Option()
        /// @see Regex::changeJpcre2Option()
        virtual RegexMatch& changeJpcre2Option(Uint opt, bool x) { 
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
        virtual RegexMatch& changePcre2Option(Uint opt, bool x) { 
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
        virtual RegexMatch& addModifier(const std::string& mod){ 
            return changeModifier(mod, true); 
        } 

        /// Add option to existing JPCRE2 options for match
        /// @param x Option value
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::addJpcre2Option()
        /// @see Regex::addJpcre2Option()
        virtual RegexMatch& addJpcre2Option(Uint x) { 
            jpcre2_match_opts |= x; 
            return *this; 
        } 

        /// Add option to existing PCRE2 options for match
        /// @param x Option value
        /// @return Reference to the calling RegexMatch object
        /// @see RegexReplace::addPcre2Option()
        /// @see Regex::addPcre2Option()
        virtual RegexMatch& addPcre2Option(Uint x) { 
            match_opts |= x; 
            return *this; 
        }
        
        //~ ///Free unused JIT memory.
        //~ ///Thread safety: MT unsafe
        //~ virtual RegexMatch& freeUnusedJitMemory(){
            //~ Pcre2Func<BS>::jit_free_unused_memory(0);
            //~ return *this;
        //~ }

        /// Perform match operation using info from class variables and return the match count and
        /// store the results in specified vectors.
        /// 
        /// Note: This function uses pcre2_match() function to do the match.
        ///@return Match count
        virtual SIZE_T match(void);
    }; 

    
    ///This class contains a typedef of a function pointer or a templated function wrapper (`std::function`)
    ///to provide callback function to the `MatchEvaluator`.
    ///`std::function` is used when `>=C++11` is being used , otherwise function pointer is used.
    ///You can force using function pointer instead of `std::function` when `>=C++11` is used by defining  the macro
    ///`JPCRE2_USE_FUNCTION_POINTER_CALLBACK` before including jpcre2.hpp.
    ///If you are using lambda function with capture, you must use the `std::function` approach.
    ///
    ///The callback function takes exactly three positional arguments:
    ///@tparam T1 The first argument must be `const jp::NumSub&` aka `const std::vector<String>&` (or `void*` if not needed).
    ///@tparam T2 The second argument must be `const jp::MapNas&` aka `const std::map<String, size_t>&` (or `void*` if not needed).
    ///@tparam T3 The third argument must be `const jp::MapNtN&` aka `const std::map<String, String>&` (or `void*` if not needed).
    ///
    /// **Examples:**
    /// ```cpp
    /// typedef jpcre2::select<char> jp;
    /// jp::String myCallback1(const jp::NumSub& m1, void*, void*){
    ///     return "("+m1[0]+")";
    /// }
    /// 
    /// jp::String myCallback2(const jp::NumSub& m1, const jp::MapNas& m2, void*){
    ///     return "("+m1[0]+"/"+m2.at("total")+")";
    /// }
    /// //Now you can pass these functions in MatchEvaluator constructors to create a match evaluator
    /// jp::MatchEvaluator me1(myCallback1); 
    ///
    /// //Examples with lambda (>=C++11)
    /// jp::MatchEvaluator me2([](const jp::NumSub& m1, void*, void*)
    ///                         {
    ///                             return "("+m1[0]+")";
    ///                         });
    /// ```
    ///@see MatchEvaluator
    template<typename T1, typename T2, typename T3>
    struct MatchEvaluatorCallback{
        #if !defined JPCRE2_USE_FUNCTION_POINTER_CALLBACK && __cplusplus >= 201103L
        typedef std::function<String (T1,T2,T3)> Callback;
        #else
        typedef String (*Callback)(T1,T2,T3);
        #endif
    };

    ///Provides some default static callback functions.
    ///The primary goal of this class is to provide default
    ///callback function to MatchEvaluator default constructor which is
    ///essentially callback::erase.
    ///This class does not allow object instantiation.
    struct callback{
        ///Callback funcition that removes the matched part/s in the subject string
        /// and takes all match vectors as argument.
        ///Even though this function itself does not use the vectors, it still takes them
        ///so that the caller can perform a match and populate all the match data to perform
        ///further evaluation of other callback functions without doing the match again.
        ///@param num jp::NumSub vector.
        ///@param nas jp::MapNas map.
        ///@param ntn jp::MapNtN map.
        ///@return empty string.
        static String eraseFill(const NumSub& num, const MapNas& nas, const MapNtN& ntn){
            return String();
        }
        
        ///Callback function that removes the matched part/s in the subject string
        ///and does not take any match vector.
        ///This is a minimum cost pattern deleting callback function.
        ///
        ///It's the default callback function when you Instantiate
        ///a MatchEvaluator object with its default constructor:
        ///```cpp
        ///MatchEvaluator me;
        ///```
        ///@return empty string.
        static String erase(void*, void*, void*){
            return String();
        }
        
        ///Callback function for populating match vectors that does not modify the subject string.
        ///It always returns the total matched part and thus the subject string remains the same.
        ///@param num jp::NumSub vector.
        ///@param nas jp::MapNas map.
        ///@param ntn jp::MapNtN map.
        ///@return total match (group 0) of current match.
        static String fill(const NumSub& num, const MapNas& nas, const MapNtn& ntn){
            return num[0];
        }
        
        private:
        //preven onject instantiation.
        callback();
        callback(const callback&);
        ~callback();
    };

    ///This class inherits RegexMatch and provides a similar functionality.
    ///All public member functions from RegexMatch class are publicly available except the following:
    ///* setNumberedSubstringVector
    ///* setNamedSubstringVector
    ///* setNameToNumberMapVector
    ///* setMatchStartOffsetVector
    ///* setMatchEndOffsetVector
    ///
    ///The use of above functions is not allowed as the vectors are created according to the callback function you pass.
    ///
    ///Each constructor of this class takes a callback function as argument (see `MatchEvaluatorCallback`).
    ///
    ///It provides a MatchEvaluator::nreplace() function to perform replace operation.
    ///
    ///An instance of this class can also be passed with `RegexReplace::nreplace()` function to perform replacement
    ///according to this match evaluator.
    ///
    ///Match data is stored in vectors, and the vectors are populated according to the callback functions.
    ///Populated vector data is never deleted but they get overwritten. Vector data can be manually zeroed out
    ///by calling `MatchEvaluator::clearMatchDataVectors()`.
    ///
    /// # Compatibility of callback function with Match Data
    /// A match data populated with a callback function that takes only a jp::NumSub vector is not compatible
    /// with the data created according to callback function with a jp::MapNas vector.
    /// Because, for this later callback, jp::MapNas data is required but is not available (only jp::NumSub is available).
    /// In such cases, previous Match data can not be used to perform a new replacment operation with this second callback function.
    ///
    /// To populate the match vectors, one must call the `MatchEvaluator::match()` or `MatchEvaluator::nreplace()` function, they will populated
    /// vectors with match data according to call back function.
    ///
    /// ## Example:
    ///
    /// ```cpp
    /// jp::String callback5(const NumSub& m, void*, const MapNtn& n){
    ///     return m[0];
    /// }
    /// jp::String callback4(void*, void*, const MapNtn& n){
    ///     return jpcre2::ConvInt<char>::toString(n.at("name")); //position of group 'name'.
    /// }
    /// jp::String callback2(void*, const MapNas& m, void*){
    ///     return m.at('name'); //substring by name
    /// }
    ///
    /// jp::MatchEvaluator me;
    /// me.setRegexObject(&re).setSubject("string").setMatchEvaluatorCallback(callback5).nreplace();
    /// //In above, nreplace() populates jp::NumSub and jp::MapNtn with match data.
    ///
    /// me.setMatchEvaluatorCallback(callback4).nreplace(false);
    /// //the above uses previous match result (note the 'false') which is OK, 
    /// //because, callback4 requires jp::MapNtn which was made available in the previous operation.
    ///
    /// //but the following is not OK: (assertion failure)
    /// me.setMatchEvaluatorCallback(callback2).nreplace(false);
    /// //because, callback2 requires jp::MapNas data which is not available.
    /// //now, this is OK:
    /// me.setMatchEvaluatorCallback(callback2).nreplace();
    /// //because, it will recreate those match data including this one (jp::MapNas).
    /// ```
    ///@see MatchEvaluatorCallback
    ///@see RegexReplace::nreplace()
    class MatchEvaluator: virtual public RegexMatch{
        private:
        friend class RegexReplace;
        
        VecNum vec_num;
        VecNas vec_nas;
        VecNtN vec_ntn;
        VecOff vec_soff;
        VecOff vec_eoff;
        int callbackn;
        typename MatchEvaluatorCallback<void*, void*, void*>::Callback callback0;
        typename MatchEvaluatorCallback<const NumSub&, void*, void*>::Callback callback1;
        typename MatchEvaluatorCallback<void*, const MapNas&, void*>::Callback callback2;
        typename MatchEvaluatorCallback<const NumSub&, const MapNas&, void*>::Callback callback3;
        typename MatchEvaluatorCallback<void*, void*, const MapNtN&>::Callback callback4;
        typename MatchEvaluatorCallback<const NumSub&, void*, const MapNtN&>::Callback callback5;
        typename MatchEvaluatorCallback<void*, const MapNas&, const MapNtN&>::Callback callback6;
        typename MatchEvaluatorCallback<const NumSub&, const MapNas&, const MapNtN&>::Callback callback7;
        //Q: Why the callback names seem random? is it random?
        //A: No, it's not random, NumSub = 1, MapNas = 2, MapNtn = 4, thus:
        //     NumSub + MapNas = 3
        //     NumSub + MapNtn = 5
        //     MapNas + MapNtn = 6
        //     NumSub + MapNas + MapNtn = 7
        //Q: Why is it like this?
        //A: It's historical. Once, there was not this many callback declaration, there was only one (a templated one).
        //   The nreplace function itself used to calculate a mode value according to available vectors
        //   and determine what kind of callback function needed to be called.
        //Q: Why the history changed?
        //A: We had some compatibility issues with the single templated callback.
        //   Also, this approach proved to be more readable and robust.
        
        
        void init(){
            callbackn = 0;
            callback0 = 0;
            callback1 = 0;
            callback2 = 0;
            callback3 = 0;
            callback4 = 0;
            callback5 = 0;
            callback6 = 0;
            callback7 = 0;
            setMatchStartOffsetVector(&vec_soff);
            setMatchEndOffsetVector(&vec_eoff);
        }
        
        void setVectorPointersAccordingToCallback(){
            switch(callbackn){
                case 0: break;
                case 1: setNumberedSubstringVector(&vec_num);break;
                case 2: setNamedSubstringVector(&vec_nas);break;
                case 3: setNumberedSubstringVector(&vec_num).setNamedSubstringVector(&vec_nas);break;
                case 4: setNameToNumberMapVector(&vec_ntn);break;
                case 5: setNumberedSubstringVector(&vec_num).setNameToNumberMapVector(&vec_ntn);break;
                case 6: setNamedSubstringVector(&vec_nas).setNameToNumberMapVector(&vec_ntn);break;
                case 7: setNumberedSubstringVector(&vec_num).setNamedSubstringVector(&vec_nas).setNameToNumberMapVector(&vec_ntn);break;
            }
        }
        
        void deepCopy(const MatchEvaluator& me) {
            callbackn = me.callbackn;
            callback0 = me.callback0;
            callback1 = me.callback1;
            callback2 = me.callback2;
            callback3 = me.callback3;
            callback4 = me.callback4;
            callback5 = me.callback5;
            callback6 = me.callback6;
            callback7 = me.callback7;
            //must update the pointers to point to this class vectors.
            setVectorPointersAccordingToCallback(); 
            vec_num = me.vec_num;
            vec_nas = me.vec_nas;
            vec_ntn = me.vec_ntn;
            vec_soff = me.vec_soff;
            vec_eoff = me.vec_eoff;
        }
        
        //prevent public access to some funcitons
        MatchEvaluator& setNumberedSubstringVector(VecNum* v){
            RegexMatch::setNumberedSubstringVector(v);
            return *this;
        }
        MatchEvaluator& setNamedSubstringVector(VecNas* v){
            RegexMatch::setNamedSubstringVector(v);
            return *this;
        }
        MatchEvaluator& setNameToNumberMapVector(VecNtN* v){
            RegexMatch::setNameToNumberMapVector(v);
            return *this;
        }
        MatchEvaluator& setMatchStartOffsetVector(VecOff* v){
            RegexMatch::setMatchStartOffsetVector(v);
            return *this;
        }
        MatchEvaluator& setMatchEndOffsetVector(VecOff* v){
            RegexMatch::setMatchEndOffsetVector(v);
            return *this;
        }
        
        public:
        
        ///Default constructor.
        ///Sets callback::erase as the callback function.
        ///Removes matched part/s from the subject string if the callback is not
        ///changed.
        /// ```cpp
        /// jp::Regex re("\s*string");
        /// jp::MatchEvaluator me;
        /// std::cout<<
        /// me.setRegexObject(&re);
        ///   .setSubject("I am a   string");
        ///   .nreplace();
        /// //The above will delete '   string' from the subject
        /// //thus the result will be 'I am a'
        /// ```
        explicit
        MatchEvaluator():RegexMatch(){
            init();
            setMatchEvaluatorCallback(callback::erase);
        }
        
        ///@overload
        ///...
        ///Constructor taking a Regex object pointer.
        ///It sets the associated Regex object and
        ///initializes the MatchEvaluator object with
        ///callback::erase callback function.
        ///@param r constant Regex pointer.
        explicit
        MatchEvaluator(const Regex *r):RegexMatch(r){
            init();
            setMatchEvaluatorCallback(callback::erase);
        }
        
        ///@overload
        ///...
        ///Constructor taking a callback function.
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<void*, void*, void*>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        ///@overload
        /// ...
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<const NumSub&, void*, void*>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        ///@overload
        /// ...
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<const NumSub&, const MapNas&, void*>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        ///@overload
        /// ...
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<const NumSub&, void*,  const MapNtN&>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        ///@overload
        /// ...
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<const NumSub&, const MapNas&, const MapNtN&>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        ///@overload
        /// ...
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<void*, const MapNas&, void*>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        
        ///@overload
        /// ...
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<void*, const MapNas&,  const MapNtN&>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        
        
        ///@overload
        /// ...
        ///It calls a corresponding MatchEvaluator::setMatchEvaluatorCallback() function to set the callback function.
        ///@param mef Callback function.
        explicit
        MatchEvaluator(typename MatchEvaluatorCallback<void*, void*,  const MapNtN&>::Callback mef): RegexMatch(){
            init();
            setMatchEvaluatorCallback(mef);
        }
        
        
        
        ///@overload
        /// ...
        ///Copy constructor. Performs deep copy.
        ///@param me Reference to MatchEvaluator object
        MatchEvaluator(const MatchEvaluator& me): RegexMatch(me){
            init();
            deepCopy(me);
        }
        
        ///Overloaded copy-assignment operator
        ///@param me MatchEvaluator object
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& operator=(const MatchEvaluator& me){
            if(this == &me) return *this;
            RegexMatch::operator=(me);
            deepCopy(me);
            return *this;
        }
        
        virtual ~MatchEvaluator(){}
        
        
        ///Member function to set a callback function with no vector reference.
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<void*, void*, void*>::Callback mef){
            callback0 = mef;
            callbackn = 0;
            return *this;
        }
        
        ///@overload
        /// ...
        ///Sets a callback function with a jp::NumSub vector.
        ///You will be working with a constant reference to the vector.
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<const NumSub&, void*, void*>::Callback mef){
            callback1 = mef;
            callbackn = 1;
            setNumberedSubstringVector(&vec_num);
            return *this;
        }
        
        ///@overload
        /// ...
        ///Sets a callback function with a jp::NumSub and jp::MapNas.
        ///You will be working with constant references of the vectors.
        ///For maps, you won't be able to use `[]` operator on constant reference, use at() instead:
        ///```cpp
        ///map_nas["word"]; //wrong
        ///map_nas.at("word"); //ok 
        ///```
        ///If you want to use `[]` operator with maps, make a copy:
        ///```cpp
        ///jp::MapNas mn = map_nas;
        ///mn["word"]; //ok
        ///```
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<const NumSub&, const MapNas&, void*>::Callback mef){
            callback3 = mef;
            callbackn = 3;
            setNumberedSubstringVector(&vec_num);
            setNamedSubstringVector(&vec_nas);
            return *this;
        }
        
        ///@overload
        /// ...
        ///Sets a callback function with a jp::NumSub and jp::MapNtN.
        ///You will be working with constant references of the vectors.
        ///For maps, you won't be able to use `[]` operator on constant reference, use at() instead:
        ///```cpp
        ///map_ntn["word"]; //wrong
        ///map_ntn.at("word"); //ok 
        ///```
        ///If you want to use `[]` operator with maps, make a copy:
        ///```cpp
        ///jp::MapNtN mn = map_ntn;
        ///mn["word"]; //ok
        ///```
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<const NumSub&, void*,  const MapNtN&>::Callback mef){
            callback5 = mef;
            callbackn = 5;
            setNumberedSubstringVector(&vec_num);
            setNameToNumberMapVector(&vec_ntn);
            return *this;
        }
        
        
        ///@overload
        /// ...
        ///Sets a callback function with a jp::NumSub, jp::MapNas, jp::MapNtN.
        ///You will be working with constant references of the vectors.
        ///For maps, you won't be able to use `[]` operator on constant reference, use at() instead:
        ///```cpp
        ///map_nas["word"]; //wrong
        ///map_nas.at("word"); //ok 
        ///```
        ///If you want to use `[]` operator with maps, make a copy:
        ///```cpp
        ///jp::MapNas mn = map_nas;
        ///mn["word"]; //ok
        ///```
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<const NumSub&, const MapNas&, const MapNtN&>::Callback mef){
            callback7 = mef;
            callbackn = 7;
            setNumberedSubstringVector(&vec_num);
            setNamedSubstringVector(&vec_nas);
            setNameToNumberMapVector(&vec_ntn);
            return *this;
        }
        
        ///@overload
        /// ...
        ///Sets a callback function with a jp::MapNas.
        ///You will be working with constant reference of the vector.
        ///For maps, you won't be able to use `[]` operator on constant reference, use at() instead:
        ///```cpp
        ///map_nas["word"]; //wrong
        ///map_nas.at("word"); //ok 
        ///```
        ///If you want to use `[]` operator with maps, make a copy:
        ///```cpp
        ///jp::MapNas mn = map_nas;
        ///mn["word"]; //ok
        ///```
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<void*, const MapNas&, void*>::Callback mef){
            callback2 = mef;
            callbackn = 2;
            setNamedSubstringVector(&vec_nas);
            return *this;
        }
        
        ///@overload
        /// ...
        ///Sets a callback function with a jp::MapNas, jp::MapNtN.
        ///You will be working with constant reference of the vector.
        ///For maps, you won't be able to use `[]` operator on constant reference, use at() instead:
        ///```cpp
        ///map_nas["word"]; //wrong
        ///map_nas.at("word"); //ok 
        ///```
        ///If you want to use `[]` operator with maps, make a copy:
        ///```cpp
        ///jp::MapNas mn = map_nas;
        ///mn["word"]; //ok
        ///```
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<void*, const MapNas&,  const MapNtN&>::Callback mef){
            callback6 = mef;
            callbackn = 6;
            setNamedSubstringVector(&vec_nas);
            setNameToNumberMapVector(&vec_ntn);
            return *this;
        }
        
        ///@overload
        /// ...
        ///Sets a callback function with a jp::MapNtN.
        ///You will be working with constant references of the vectors.
        ///For maps, you won't be able to use `[]` operator on constant reference, use at() instead:
        ///```cpp
        ///map_ntn["word"]; //wrong
        ///map_ntn.at("word"); //ok 
        ///```
        ///If you want to use `[]` operator with maps, make a copy:
        ///```cpp
        ///jp::MapNtN mn = map_ntn;
        ///mn["word"]; //ok
        ///```
        ///@param mef Callback function.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchEvaluatorCallback(typename MatchEvaluatorCallback<void*, void*,  const MapNtN&>::Callback mef){
            callback4 = mef;
            callbackn = 4;
            setNameToNumberMapVector(&vec_ntn);
            return *this;
        }
        
        ///Clear match data.
        ///It clears all match data from all vectors.
        ///A call to `match()`  or nreplace() will be required to produce match data again.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& clearMatchDataVectors(){
            vec_num.clear();
            vec_nas.clear();
            vec_ntn.clear();
            vec_soff.clear();
            vec_eoff.clear();
            return *this;
        }
        
        ///Reset MatchEvaluator retaining callback function.
        ///It calls the base `RegexMatch::reset()` function and then 
        ///updates the vector pointers (internal use).
        ///It clears all match data in all vectors.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& reset(){
            RegexMatch::reset();
            clearMatchDataVectors();
            //just like a new object with default constructor:
            init();
            setMatchEvaluatorCallback(callback::erase);
            return *this;
        }
        
        ///Call RegexMatch::resetErrors().
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& resetErrors(){
            RegexMatch::resetErrors();
            return *this;
        }
        
        ///Call RegexMatch::setRegexObject(r).
        ///@param r constant Regex object pointer
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setRegexObject (const Regex *r){
            RegexMatch::setRegexObject(r);
            return *this;
        }
        
        ///Call RegexMatch::setSubject(const String &s).
        ///@param s subject string
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setSubject (const String &s){
            RegexMatch::setSubject(s);
            return *this;
        }
        
        ///Call RegexMatch::setSubject(const String *s).
        ///@param s subject string
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setSubject (const String *s){
            RegexMatch::setSubject(s);
            return *this;
        }
        
        ///Call RegexMatch::setModifier(const std::string &s).
        ///@param s modifier string.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setModifier (const std::string &s){
            RegexMatch::setModifier(s);
            return *this;
        }
        
        ///Call RegexMatch::setJpcre2Option(Uint x).
        ///@param x JPCRE2 option value.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setJpcre2Option (Uint x){
            RegexMatch::setJpcre2Option(x);
            return *this;
        }
        
        ///Call RegexMatch::setPcre2Option (Uint x).
        ///@param x PCRE2 option value.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setPcre2Option (Uint x){
            RegexMatch::setPcre2Option(x);
            return *this;
        }
        
        ///Call RegexMatch::setFindAll(bool x).
        ///@param x true if global match, false otherwise.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setFindAll (bool x){
            RegexMatch::setFindAll(x);
            return *this;
        }
        
        ///Call RegexMatch::setFindAll().
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setFindAll(){
            RegexMatch::setFindAll();
            return *this;
        }
        
        ///Call RegexMatch::setStartOffset (PCRE2_SIZE offset).
        ///@param offset match start offset in the subject.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setStartOffset (PCRE2_SIZE offset){
            RegexMatch::setStartOffset(offset);
            return *this;
        }
        
        ///Call RegexMatch::setMatchContext(MatchContext *match_context).
        ///@param match_context pointer to match context.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& setMatchContext (MatchContext *match_context){
            RegexMatch::setMatchContext(match_context);
            return *this;
        }
        
        ///Call RegexMatch::changeModifier(const std::string &mod, bool x).
        ///@param mod modifier string.
        ///@param x true (add) or false (remove).
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& changeModifier (const std::string &mod, bool x){
            RegexMatch::changeModifier(mod, x);
            return *this;
        }
        
        ///Call RegexMatch::changeJpcre2Option(Uint opt, bool x).
        ///@param opt JPCRE2 option
        ///@param x true (add) or false (remove).
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& changeJpcre2Option (Uint opt, bool x){
            RegexMatch::changeJpcre2Option(opt, x);
            return *this;
        }
        
        ///Call RegexMatch::changePcre2Option(Uint opt, bool x).
        ///@param opt PCRE2 option.
        ///@param x true (add) or false (remove).
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& changePcre2Option (Uint opt, bool x){
            RegexMatch::changePcre2Option(opt, x);
            return *this;
        }
        
        ///Call RegexMatch::addModifier(const std::string &mod).
        ///@param mod modifier string.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& addModifier (const std::string &mod){
            RegexMatch::addModifier(mod);
            return *this;
        }
        
        ///Call RegexMatch::addJpcre2Option(Uint x).
        ///@param x JPCRE2 option.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& addJpcre2Option (Uint x){
            RegexMatch::addJpcre2Option(x);
            return *this;
        }
        
        ///Call RegexMatch::addPcre2Option(Uint x).
        ///@param x PCRE2 option.
        ///@return A reference to the calling MatchEvaluator object.
        MatchEvaluator& addPcre2Option (Uint x){
            RegexMatch::addPcre2Option(x);
            return *this;
        }
        
        ///Perform match and return the match count.
        ///This function modifies matching options that are considered
        ///bad options for replacement operation and then calls the original
        ///RegexMatch::match() to perform the match.
        ///
        ///This function checks for null
        ///Regex pointer and if no Regex object is set, it gives out
        ///assertion failure.
        ///@return match count.
        SIZE_T match(void){
            JPCRE2_ASSERT(RegexMatch::getRegexObject() != 0, "NullPointerError:\n\
    MatchEvaluator object contains null Regex pointer.\n\
    May be you forgot to setRegexObject!!!");
            //remove bad matching options
            RegexMatch::changePcre2Option(PCRE2_PARTIAL_HARD|PCRE2_PARTIAL_SOFT, false);
            return RegexMatch::match();
        }
        
        ///Perform regex replace with this match evaluator.
        ///This is a JPCRE2 native replace function (thus the name nreplace).
        ///It uses the `MatchEvaluatorCallback` function that was set with a constructor or `MatchEvaluator::setMatchEvaluatorCallback()` function
        ///to generate the replacement strings on the fly.
        ///The string returned by the callback function will be treated as literal and will
        ///not go through any further processing.
        ///
        ///This function performs a new match everytime it is called unless it is passed with a boolean `false` as argument.
        ///To use existing match data that was created by a previous `MatchEvaluator::nreplace()` or `MatchEvaluator::match()`, call this
        ///function with boolean `false` as argument.
        ///@param do_match Perform a new matching operation if true, otherwise use existing match data.
        ///@return resultant string after replace.
        ///@see MatchEvaluator.
        ///@see MatchEvaluatorCallback.
        String nreplace(bool do_match=true);
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
     * jp::Regex re;
     * jp::RegexReplace rr;
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
        PCRE2_SIZE _start_offset;
        MatchData *mdata;
        MatchContext *mcontext;
        
        void init_vars() {
            re = 0;
            r_subject_ptr = &r_subject;
            r_replw_ptr = &r_replw;
            replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; 
            jpcre2_replace_opts = 0; 
            buffer_size = 0; 
            error_number = 0;
            error_offset = 0;
            _start_offset = 0;
            mdata = 0;
            mcontext = 0;
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
            _start_offset = rr._start_offset;
            mdata = rr.mdata;
            mcontext = rr.mcontext;
        }
        
        friend class Regex;

    public: 

        ///Default constructor
        RegexReplace(){
            init_vars();
        }

        ///@overload
        /// ...
        ///Creates a RegexReplace object associating a Regex object.
        ///@param r pointer to a Regex object
        RegexReplace(const Regex * r) { 
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
        ///Allows object assignment:
        ///```cpp
        ///jp::RegexReplace rr;
        ///rr = jp::RegexReplace(&re);
        ///```
        ///@param rr RegexReplace object reference
        ///@return A reference to the calling RegexReplace object
        RegexReplace& operator=(const RegexReplace& rr){
            if(this == &rr) return *this;
            deepCopy(rr);
            return *this;
        }
        
        virtual ~RegexReplace() {} 
    
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
        int getErrorNumber() const { 
            return error_number; 
        } 

        /// Returns the last error offset
        ///@return Last error offset
        int getErrorOffset() const  { 
            return (int)error_offset; 
        } 
        
        /// Returns the last error message
        ///@return Last error message
        String getErrorMessage() const  { 
            return select<Char, BS>::getErrorMessage(error_number, error_offset); 
        } 
        
        /// Get replacement string
        ///@return replacement string
        String getReplaceWith() const  { 
            return *r_replw_ptr; 
        } 
        
        /// Get pointer to replacement string
        ///@return pointer to replacement string
        const String* getReplaceWithPointer() const  { 
            return r_replw_ptr; 
        } 

        /// Get subject string
        ///@return subject string
        ///@see RegexMatch::getSubject()
        String getSubject() const  { 
            return *r_subject_ptr; 
        } 

        /// Get pointer to subject string
        ///@return Pointer to subject string
        ///@see RegexMatch::getSubjectPointer()
        const String* getSubjectPointer() const  { 
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
        std::string getModifier() const;
        
        ///Get start offset.
        ///@return the start offset where matching starts for replace operation
        PCRE2_SIZE getStartOffset() const {
            return _start_offset;
        }
        
        /** Get PCRE2 option
         * @return PCRE2 option for replace
         * @see Regex::getPcre2Option()
         * @see RegexMatch::getPcre2Option()
         * */
        Uint getPcre2Option() const  { 
            return replace_opts; 
        } 

        /** Get JPCRE2 option
         * @return JPCRE2 option  for replace
         * @see Regex::getJpcre2Option()
         * @see RegexMatch::getJpcre2Option()
         * */
        Uint getJpcre2Option() const  { 
            return jpcre2_replace_opts; 
        }
        
        ///Get a pointer to the associated Regex object.
        ///If no actual Regex object is associated, null is returned
        ///@return A pointer to the associated Regex object or null
        const Regex* getRegexObject() const {
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
         *...
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
         * `$` is a special character which implies captured group.
         *
         * 1. A numbered substring can be referenced with `$n` or `${n}` where n is the group number.
         * 2. A named substring can be referenced with `${name}`, where 'name' is the group name.
         * 3. A literal `$` can be given as `$$`.
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
         *...
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
        
        ///Set start offset.
        ///Set the offset where matching starts for replace operation
        ///@param start_offset The offset where matching starts for replace operation
        ///@return Reference to the calling RegexReplace object
        RegexReplace& setStartOffset(PCRE2_SIZE start_offset){
            _start_offset = start_offset;
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
        
        ///Set the match context to be used.
        ///Native PCRE2 API may be used to create match context.
        ///The memory of the match context is not handled by RegexReplace object and not freed.
        ///User will be responsible for freeing memory.
        ///@param match_context Pointer to match context.
        ///@return Reference to the calling RegexReplace object.
        RegexReplace& setMatchContext(MatchContext * match_context){
            mcontext = match_context;
            return *this;
        }
        
        ///Set the match data to be used.
        ///Native PCRE2 API may be used to create match data.
        ///The memory of the match data is not handled by RegexReplace object and not freed.
        ///User will be responsible for freeing memory.
        ///@param match_data Pointer to match data.
        ///@return Reference to the calling RegexReplace object.
        RegexReplace& setMatchData(MatchData *match_data){
            mdata = match_data;
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
        /// Note: This function calls pcre2_substitute() to do the replacement. 
        ///@return Replaced string
        String replace(void);
        
        ///JPCRE2 native replace function.
        ///A different name is adopted to
        ///distinguish itself from the regular replace() function which
        ///uses pcre2_substitute() to do the replacement; contrary to that,
        ///it will provide a JPCRE2 native way of replacement operation.
        ///It takes a MatchEvaluator object which provides a callback function that is used
        ///to generate replacement string on the fly. Any replacement string set with
        ///`RegexReplace::setReplaceWith()` function will have no effect.
        ///The string returned by the callback function will be treated as literal and will
        ///not go through any further processing.
        ///
        ///This function works on a copy of the MatchEvaluator, and thus makes no changes
        ///to the original. The copy is modified (if the second param is true) as below:
        ///
        ///1. Global replacement will set FIND_ALL for match, unset otherwise.
        ///2. Bad matching options such as `PCRE2_PARTIAL_HARD|PCRE2_PARTIAL_SOFT` will be removed.
        ///3. subject, start_offset and Regex object will change according to the RegexReplace object.
        ///
        ///It calls MatchEvaluator::nreplace() on the MatchEvaluator object to perform the replacement.
        ///@param me A MatchEvaluator object.
        ///@param do_match Perform a new match operation if true, otherwise use existing match result.
        ///@return The resultant string after replacement.
        ///@see MatchEvaluator::nreplace()
        ///@see MatchEvaluator
        ///@see MatchEvaluatorCallback
        String nreplace(MatchEvaluator me, bool do_match = true);
    }; 
 
 
    /** Provides public constructors to create Regex object.
     * Each regex pattern needs an object of this class and each pattern needs to be compiled.
     * Pattern compilation can be done using one of its' overloaded constructors or the `Regex::compile()`
     * member function.
     * 
     * This class can contain a RegexMatch or RegexReplace object if desired. RegexMatch
     * object can be created either with Regex::initMatch() or Regex::getMatchObject() member function.
     * Similar functions are available for RegexReplace.
     * 
     * Examples:
     * 
     * ```cpp
     * jp::Regex re;
     * re.compile("pattern", "modifier");
     * jp::Regex re2("pattern", "modifier");
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
        int error_number;
        PCRE2_SIZE error_offset;

        CompileContext *ccontext;
        std::vector<unsigned char> tabv;
        
        
        void init_vars() { 
            jpcre2_compile_opts = 0; 
            compile_opts = 0; 
            error_number = 0; 
            error_offset = 0; 
            code = 0;
            rm = 0;
            rr = 0; 
            pat_str_ptr = &pat_str;
            ccontext = 0;
        } 

        void freeRegexMemory(void) {
            Pcre2Func<BS>::code_free(code);
            code = 0; //we may use it again
        }
        
        void freeCompileContext(){
            Pcre2Func<BS>::compile_context_free(ccontext);
            ccontext = 0;
        }

        friend class RegexMatch;    
        friend class RegexReplace; 

        void deepCopy(const Regex& r) {
            pat_str = r.pat_str; //must not use setPattern() here
            //r.pat_str_ptr may point to other user data
            if(r.pat_str_ptr == &r.pat_str) pat_str_ptr = &pat_str; //not r.pat_str
            else pat_str_ptr = r.pat_str_ptr; //other user data
            
            compile_opts = r.compile_opts; 
            jpcre2_compile_opts = r.jpcre2_compile_opts; 
            error_number = r.error_number; 
            error_offset = r.error_offset; 
            
            //copy tables
            tabv = r.tabv;
            //copy ccontext if it's not null
            freeCompileContext();
            if(r.ccontext){
                ccontext = Pcre2Func<BS>::compile_context_copy(r.ccontext);
                //if tabv is not empty and ccontext is ok (not null) set the table pointer to ccontext
                if(!tabv.empty()){
                    Pcre2Func<BS>::set_character_tables(ccontext, &tabv[0]);
                }
            } else {
                ccontext = r.ccontext; //r.ccontext is null
            }
            
            //table pointer must be updated in the compiled code itself
            //copy is not going to work, we need a recompile.
            //as all vars are already copied, we can just call compile()
            if(r.code) compile();
            else freeRegexMemory();
            
            //~ //Copy #code if it is non-null
            //~ ///First release memory of #code from current object if it is non-NULL
            //~ freeRegexMemory();
            //~ if (r.code) {
                //~ /// Copy compiled memory of #code to #code of current object using pcre2_code_copy() 
                //~ code = Pcre2Func<BS>::code_copy(r.code);
                //~ /// Perform JIT compilation (if enabled) as pcre2_code_copy() doesn't copy JIT memory
                //~ if ((jpcre2_compile_opts & JIT_COMPILE) != 0) {
                    //~ //Perform JIT compilation:
                    //~ int jit_ret = Pcre2Func<BS>::jit_compile(code, PCRE2_JIT_COMPLETE);
                    //~ if(jit_ret < 0) error_number = jit_ret;
                //~ }
            //~ } //else code is already null
            
            //use copy assignment for rm and rr
            delete rm;
            rm = 0;
            if(r.rm) {
                rm = new RegexMatch(*(r.rm)); 
                rm->re = this; //associated Regex object needs to be this one
            }
            
            delete rr;
            rr = 0;
            if(r.rr){
                rr = new RegexReplace(*(r.rr)); 
                rr->re = this; //associated Regex object needs to be this one
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
         *  Thread safety: same as `Regex::compile()`.
         *  @param re Pattern string
         * */
        Regex(const String& re) {
            init_vars();
            compile(re); 
        } 

        /** Compile pattern with initialization.
         *  Thread safety: same as `Regex::compile()`.
         *  @param re Pointer to pattern string
         * */
        Regex(const String* re) {
            init_vars();
            compile(re); 
        } 
        
        /**@overload
         *...
         *  Compile pattern with initialization.
         *  Thread safety: same as `Regex::compile()`.
         *  @param re Pattern string
         *  @param mod Modifier string
         * */
        Regex(const String& re, const std::string& mod) {
            init_vars();
            compile(re, mod); 
        } 
        
        /**@overload
         *...
         *  Compile pattern with initialization.
         *  Thread safety: same as `Regex::compile()`.
         *  @param re Pointer to pattern string
         *  @param mod Modifier string
         * */
        Regex(const String* re, const std::string& mod) {
            init_vars();
            compile(re, mod); 
        }

        /**@overload
         *...
         *  Compile pattern with initialization.
         *  Thread safety: same as `Regex::compile()`.
         *  @param re Pattern string
         *  @param pcre2_opts PCRE2 option value
         * */
        Regex(const String& re, Uint pcre2_opts) {
            init_vars();
            compile(re, pcre2_opts); 
        } 

        /**@overload
         *...
         *  Compile pattern with initialization.
         *  Thread safety: same as `Regex::compile()`.
         *  @param re Pointer to pattern string
         *  @param pcre2_opts PCRE2 option value
         * */
        Regex(const String* re, Uint pcre2_opts) {
            init_vars();
            compile(re, pcre2_opts); 
        } 

        /**@overload
         *...
         *  Compile pattern with initialization.
         *  Thread safety: same as `Regex::compile()`.
         *  @param re Pattern string
         *  @param pcre2_opts    PCRE2 option value
         *  @param opt_bits        JPCRE2 option value
         * */
        Regex(const String& re, Uint pcre2_opts, Uint opt_bits) {
            init_vars();
            compile(re, pcre2_opts, opt_bits); 
        } 

        /**@overload
         *...
         *  Compile pattern with initialization.
         *  Thread safety: same as `Regex::compile()`.
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
        /// A separate and new compile is performed from the copied options.
        ///
        ///Thread safety: same as `Regex::compile()`.
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
        /// Allows assigning objects:
        /// ```cpp
        /// jp::Regex re2;
        /// re2 = re;
        /// ```
        ///Thread safety: same as `Regex::compile()`.
        /// @param r const Regex&
        /// @return *this
        Regex& operator=(const Regex& r) { 
            if (this == &r) return *this;
            deepCopy(r); 
            return *this; 
        } 
        
        #if __cplusplus >= 201103L
        
        /** Provides boolean check for the status of the object.
         *  This overloaded boolean operator needs to be declared
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
        virtual ~Regex() {
            freeRegexMemory();
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

        /// Reset character tables used by PCRE2.
        /// You should call this function after changing the locale to remake the
        /// character tables according to the new locale.
        /// These character tables are used to compile the regex and used by match
        /// and replace operation. A separate call to compile() will be required
        /// to apply the new character tables.
        /// @return Reference to the calling Regex object.
        Regex& resetCharacterTables() {
            const unsigned char* tables = Pcre2Func<BS>::maketables(0); //must pass 0, we are using free() to free the tables.
            tabv = std::vector<unsigned char>(tables, tables+1088);
            ::free((void*)tables); //must free memory
            if(!ccontext)
                ccontext = Pcre2Func<BS>::compile_context_create(0);
            Pcre2Func<BS>::set_character_tables(ccontext, &tabv[0]);
            return *this; 
        } 

        ///@deprecated 
        /**Create and initialize a new match object and return a reference to it
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

        ///@deprecated 
        /**Creates a new RegexReplace object and returns its reference.
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

        ///@deprecated 
        ///Copy the match object into this Regex object.
        ///The match object passed is copied into the Regex object leaving the
        ///original unchanged.
        ///After copying, the associated Regex object for this new internal
        ///match object is set (corrected) to this object.
        ///@param rmo Constant reference to a RegexMatch object
        ///@return Reference to the calling Regex object
        Regex& initMatchFrom(const RegexMatch& rmo){
            delete rm;
            rm = new RegexMatch(rmo);
            rm->re = this;
            return *this;
        }

        ///@deprecated 
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
        String getPattern() const  { 
            return *pat_str_ptr; 
        }
        
        /** Get pointer to pattern string
         * @return Pointer to pattern string
         * */
        const String* getPatternPointer() const  { 
            return pat_str_ptr; 
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
        std::string getModifier() const ; 

        /** Get PCRE2 option
         *  @return Compile time PCRE2 option value
         * @see RegexReplace::getPcre2Option()
         * @see RegexMatch::getPcre2Option()
         * */
        Uint getPcre2Option() const  { 
            return compile_opts; 
        } 

        /** Get JPCRE2 option
         *  @return Compile time JPCRE2 option value
         * @see RegexReplace::getJpcre2Option()
         * @see RegexMatch::getJpcre2Option()
         * */
        Uint getJpcre2Option() const  { 
            return jpcre2_compile_opts; 
        } 

        /// Returns the last error number
        ///@return Last error number
        int getErrorNumber() const { 
            return error_number; 
        } 

        /// Returns the last error offset
        ///@return Last error offset
        int getErrorOffset() const  { 
            return (int)error_offset; 
        } 
        
        /// Returns the last error message
        ///@return Last error message
        String getErrorMessage() const  { 
            return select<Char, BS>::getErrorMessage(error_number, error_offset); 
        } 
      
        ///@deprecated 
        ///Returns a reference to existing match object.
        /// If there was no match object, it will create a new and act similarly to Regex::initMatch()
        /// @return Reference to a RegexMatch object
        ///@see Regex::initMatch()
        RegexMatch& getMatchObject() { 
            if(rm) return *rm; 
            else return initMatch(); 
        } 
        
        ///@deprecated 
        ///Returns a reference to the existing RegexReplace object.
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
        Uint getNewLine() {
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
         * **Thread safety:** 
         * 
         * 1. (C) MT safe if JIT compile is not done.
         * 2. MT unsafe if JIT compile is done (e.g passing the 'S' modifier)
         *
         * 
         * @see Regex::compile(const String& re, Uint po, Uint jo)
         * @see Regex::compile(const String& re, Uint po)
         * @see Regex::compile(const String& re, const std::string& mod)
         * @see Regex::compile(const String& re)
         * */
        void compile(void); 

        /**@overload
         *...
         * Thread safety: same as `Regex::compile()`.
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

        /**@overload
         *...
         * Thread safety: same as `Regex::compile()`.
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
        
        /**@overload
         *...
         * Thread safety: same as `Regex::compile()`.
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pattern string
         *  @param po PCRE2 option
         * */
        void compile(const String& re, Uint po) { 
            setPattern(re).setPcre2Option(po);
            compile(); 
        } 
        
        /**@overload
         *...
         * Thread safety: same as `Regex::compile()`.
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
         * Thread safety: same as `Regex::compile()`.
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

        /**@overload
         *...
         * Thread safety: same as `Regex::compile()`.
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

        /**@overload
         *...
         * Thread safety: same as `Regex::compile()`.
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pattern string
         * */
        void compile(const String& re) { 
            setPattern(re);
            compile(); 
        } 

        /**@overload
         *...
         * Thread safety: same as `Regex::compile()`.
         *
         *
         *  Set the specified parameters, then compile the pattern using options from class variables.
         *  @param re Pointer to pattern string
         * */
        void compile(const String* re) {
            setPattern(re);
            compile(); 
        } 
        
        /** Perform regex match and return match count using a temporary match object.
         *  No previously set options are used or changed. A match is performed with just the options
         *  that were passed as parameters.
         * 
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         * 
         * 
         *  @param s Subject string.
         *  @param mod Modifier string.
         *  @param start_offset Offset from where matching will start in the subject string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s, const std::string& mod, PCRE2_SIZE start_offset) {
            return RegexMatch(this).setStartOffset(start_offset).setSubject(s).setModifier(mod).match(); 
        } 
        
        ///@overload
        ///...
        ///This action doesn not affect any class variables.
        ///The temporary object that is created is not further usable.
        ///@param s Pointer to subject string.
        ///@param mod Modifier string.
        ///@param start_offset Offset from where matching will start in the subject string.
        ///@return Match count
        SIZE_T match(const String* s, const std::string& mod, PCRE2_SIZE start_offset) {
            return RegexMatch(this).setStartOffset(start_offset).setSubject(s).setModifier(mod).match(); 
        }
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param s Subject string.
         *  @param mod Modifier string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s, const std::string& mod) { 
            return RegexMatch(this).setSubject(s).setModifier(mod).match(); 
        } 
        
        
        ///@overload
        ///...
        ///This action doesn not affect any class variables.
        ///The temporary object that is created is not further usable.
        ///
        ///@param s Pointer to subject string.
        ///@param mod Modifier string.
        ///@return Match count
        ///@see RegexMatch::match()
        SIZE_T match(const String* s, const std::string& mod) { 
            return RegexMatch(this).setSubject(s).setModifier(mod).match(); 
        } 

        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         * 
         *  @param s Subject string
         *  @param start_offset Offset from where matching will start in the subject string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s,  PCRE2_SIZE start_offset) { 
            return RegexMatch(this).setStartOffset(start_offset).setSubject(s).match(); 
        } 

        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param s Pointer to subject string
         *  @param start_offset Offset from where matching will start in the subject string.
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String* s,  PCRE2_SIZE start_offset) { 
            return RegexMatch(this).setStartOffset(start_offset).setSubject(s).match(); 
        }
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         * 
         *  @param s Subject string
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String& s) { 
            return RegexMatch(this).setSubject(s).match(); 
        }
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param s Pointer to subject string
         *  @return Match count
         *  @see RegexMatch::match()
         * */
        SIZE_T match(const String* s) { 
            return RegexMatch(this).setSubject(s).match(); 
        }
        
        ///@deprecated 
        ///Shorthand for getMatchObject().match()
        ///This uses previously initiated match object i.e call RegexMatch::match() with all previous options intact.
        ///@return Match count
        ///@see RegexMatch::match()
        SIZE_T match(){
            return getMatchObject().match();
        }
        
        /** Perform regex replace and return the replaced string using a temporary replace object.
         *  This function takes the parameters, creates a temporary replace object and
         *  then sets the parameters to the newly created RegexReplace object and calls
         *  RegexReplace::replace() which returns the result.
         * 
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Subject string
         *  @param repl String to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String& repl, const std::string& mod) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Pointer to subject string
         *  @param repl String to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String& repl, const std::string& mod) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Subject string
         *  @param repl Pointer to string to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String* repl, const std::string& mod) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Pointer to subject string
         *  @param repl Pointer to string to replace with
         *  @param mod Modifier string (std::string)
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String* repl, const std::string& mod) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); 
        } 

        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Subject string
         *  @param repl String to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String& repl) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).replace(); 
        } 
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Pointer to subject string
         *  @param repl String to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String& repl) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).replace(); 
        } 
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Subject string
         *  @param repl Pointer to string to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String& mains, const String* repl) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).replace(); 
        } 
        
        /**@overload
         *...
         *  This action doesn not affect any class variables.
         *  The temporary object that is created is not further usable.
         *  @param mains Pointer to subject string
         *  @param repl Pointer to string to replace with
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace(const String* mains, const String* repl) { 
            return RegexReplace(this).setSubject(mains).setReplaceWith(repl).replace(); 
        }

        ///@deprecated 
        /** Shorthand for getReplaceObject().replace()
         *  All previously set options will be used. It's just a short hand
         *  for calling `re.getReplaceObject().replace()`
         *  @return Resultant string after regex replace
         *  @see RegexReplace::replace()
         * */
        String replace() { 
            return getReplaceObject().replace(); 
        } 
    };
    
    private:
    //prevent object instantiation of select class
    select();
    select(const select&);
    ~select();
};//struct select
}//jpcre2 namespace

    
template<typename Char_T, jpcre2::Ush BS>
std::string jpcre2::select<Char_T, BS>::Regex::getModifier() const {
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < sizeof(MOD::C_V)/sizeof(Uint); ++i){
        if( (MOD::C_V[i] & compile_opts) != 0 && 
            (MOD::C_V[i] & compile_opts) == MOD::C_V[i]) //One option can include other
            temp += MOD::C_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < sizeof(MOD::CJ_V)/sizeof(Uint); ++i){
        if( (MOD::CJ_V[i] & jpcre2_compile_opts) != 0 && 
            (MOD::CJ_V[i] & jpcre2_compile_opts) == MOD::CJ_V[i]) //One option can include other
            temp += MOD::CJ_N[i];
    }
    return temp;
}



template<typename Char_T, jpcre2::Ush BS>
std::string jpcre2::select<Char_T, BS>::RegexMatch::getModifier() const {
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < sizeof(MOD::M_V)/sizeof(Uint); ++i){
        if( (MOD::M_V[i] & match_opts) != 0 && 
            (MOD::M_V[i] & match_opts) == MOD::M_V[i]) //One option can include other
            temp += MOD::M_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < sizeof(MOD::MJ_V)/sizeof(Uint); ++i){
        if( (MOD::MJ_V[i] & jpcre2_match_opts) != 0 && 
            (MOD::MJ_V[i] & jpcre2_match_opts) == MOD::MJ_V[i]) //One option can include other
            temp += MOD::MJ_N[i];
    }
    return temp;
}

template<typename Char_T, jpcre2::Ush BS>
std::string jpcre2::select<Char_T, BS>::RegexReplace::getModifier() const {
    //Calculate PCRE2 mod
    std::string temp("");
    for(SIZE_T i = 0; i < sizeof(MOD::R_V)/sizeof(Uint); ++i){
        if( (MOD::R_V[i] & replace_opts) != 0 &&
            (MOD::R_V[i] & replace_opts) == MOD::R_V[i]) //One option can include other
            temp += MOD::R_N[i];
    }
    //Calculate JPCRE2 mod
    for(SIZE_T i = 0; i < sizeof(MOD::RJ_V)/sizeof(Uint); ++i){
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
    SIZE_T n = mod.length();
    for (SIZE_T i = 0; i < n; ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < sizeof(MOD::CJ_V)/sizeof(Uint); ++j){
            if(MOD::CJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::CJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< sizeof(MOD::C_V)/sizeof(Uint); ++j){
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
    SIZE_T n = mod.length();
    for (SIZE_T i = 0; i < n; ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < sizeof(MOD::RJ_V)/sizeof(Uint); ++j){
            if(MOD::RJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::RJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< sizeof(MOD::R_V)/sizeof(Uint); ++j){
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
typename jpcre2::select<Char_T, BS>::String jpcre2::select<Char_T, BS>::MatchEvaluator::nreplace(bool do_match){
    if(do_match) match();
    SIZE_T mcount = vec_soff.size();
    // if mcount is 0, return the subject string.
    if(mcount == 0) return RegexMatch::getSubject();
    SIZE_T current_offset = 0;
    String res;
    //loop through the matches
    for(SIZE_T i=0;i<mcount;++i){
        //first copy the unmatched part.
        //Matches that use \K to end before they start are not supported.
        if(vec_soff[i] < current_offset){
            RegexMatch::setErrorNumber(PCRE2_ERROR_BADSUBSPATTERN);
            return RegexMatch::getSubject();
        } else {
            res += RegexMatch::getSubjectPointer()->substr(current_offset, vec_soff[i]-current_offset);
        }
        //now process the matched part
        switch(callbackn){
            case 0: res += callback0((void*)0, (void*)0, (void*)0); break;
            case 1: JPCRE2_VECTOR_DATA_ASSERT(vec_num.size() == mcount, "VecNum");
                    res += callback1(vec_num[i], (void*)0, (void*)0); break;
            case 2: JPCRE2_VECTOR_DATA_ASSERT(vec_nas.size() == mcount, "VecNas");
                    res += callback2((void*)0, vec_nas[i], (void*)0); break;
            case 3: JPCRE2_VECTOR_DATA_ASSERT(vec_num.size() == mcount && vec_nas.size() == mcount, "VecNum or VecNas");
                    res += callback3(vec_num[i], vec_nas[i], (void*)0); break;
            case 4: JPCRE2_VECTOR_DATA_ASSERT(vec_ntn.size() == mcount, "VecNtn");
                    res += callback4((void*)0, (void*)0, vec_ntn[i]); break;
            case 5: JPCRE2_VECTOR_DATA_ASSERT(vec_num.size() == mcount && vec_ntn.size() == mcount, "VecNum or VecNtn");
                    res += callback5(vec_num[i], (void*)0, vec_ntn[i]); break;
            case 6: JPCRE2_VECTOR_DATA_ASSERT(vec_nas.size() == mcount && vec_ntn.size() == mcount, "VecNas or VecNtn");
                    res += callback6((void*)0, vec_nas[i], vec_ntn[i]); break;
            case 7: JPCRE2_VECTOR_DATA_ASSERT(vec_num.size() == mcount && vec_nas.size() == mcount && vec_ntn.size() == mcount, "VecNum\n or VecNas or VecNtn");
                    res += callback7(vec_num[i], vec_nas[i], vec_ntn[i]); break;
        }
        //reset the current offset
        current_offset = vec_eoff[i];
    }
    //All matched parts have been dealt with.
    //now copy rest of the string from current_offset
    res += RegexMatch::getSubjectPointer()->substr(current_offset, String::npos);
    return res;
}


template<typename Char_T, jpcre2::Ush BS>
typename jpcre2::select<Char_T, BS>::String jpcre2::select<Char_T, BS>::RegexReplace::nreplace(MatchEvaluator me, bool do_match){
    // If re or re->code is null, return the subject string unmodified.
    if (!re || re->code == 0) return getSubject();
    if(do_match){
        //set the re object to point to the re object corresponding to replace object
        me.setRegexObject(re);
        //set subject and start offset
        me.setSubject(getSubjectPointer()).setStartOffset(getStartOffset());
        //global replacement will force global match and vice versa
        if((replace_opts & PCRE2_SUBSTITUTE_GLOBAL)!=0) me.setFindAll(); //true is default
        else me.setFindAll(false);
    }
    return me.nreplace(do_match);
}


template<typename Char_T, jpcre2::Ush BS>
typename jpcre2::select<Char_T, BS>::String jpcre2::select<Char_T, BS>::RegexReplace::replace() {

    // If re or re->code is null, return the subject string unmodified.
    if (!re || re->code == 0)
        return *r_subject_ptr;

    Pcre2Sptr subject = (Pcre2Sptr) r_subject_ptr->c_str();
    PCRE2_SIZE subject_length = r_subject_ptr->length();
    Pcre2Sptr replace = (Pcre2Sptr) r_replw_ptr->c_str();
    PCRE2_SIZE replace_length = r_replw_ptr->length();
    PCRE2_SIZE outlengthptr = (PCRE2_SIZE) buffer_size;
    bool retry = true;
    int ret = 0;
    Pcre2Uchar* output_buffer = new Pcre2Uchar[outlengthptr + 1]();

    while (true) {
        ret = Pcre2Func<BS>::substitute(
                    re->code,               /*Points to the compiled pattern*/
                    subject,                /*Points to the subject string*/
                    subject_length,         /*Length of the subject string*/
                    _start_offset,          /*Offset in the subject at which to start matching*/
                    replace_opts,           /*Option bits*/
                    mdata,                  /*Points to a match data block, or is NULL*/
                    mcontext,               /*Points to a match context, or is NULL*/
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
                output_buffer = new Pcre2Uchar[outlengthptr + 1]();
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
    SIZE_T n = mod.length();
    for (SIZE_T i = 0; i < n; ++i) {
        //First check for JPCRE2 mods
        for(SIZE_T j = 0; j < sizeof(MOD::MJ_V)/sizeof(Uint); ++j){
            if(MOD::MJ_N[j] == mod[i]) {
                changeJpcre2Option(MOD::MJ_V[j], x);
                goto endfor;
            }
        }
        //Now check for PCRE2 mods
        for(SIZE_T j = 0; j< sizeof(MOD::M_V)/sizeof(Uint); ++j){
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
        } else {
            value = toString((Char*) buffer);
            Pcre2Func<BS>::substring_free(buffer);     //must free memory
        }
        buffer = 0; //we are going to use it again.
        //if (num_sub)   //This null check is paranoid, this function shouldn't be called if this vector is null
        num_sub->push_back(value); 
    }
    return true;
}



template<typename Char_T, jpcre2::Ush BS>
bool jpcre2::select<Char_T, BS>::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size,
                                                            Pcre2Sptr name_table, MatchData *match_data) {
    Pcre2Sptr tabptr = name_table;
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
        } else {
            value = toString((Char *) buffer);
            Pcre2Func<BS>::substring_free(buffer);     //must free memory
        }
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
            } else {
                value1 = toString((Char *) buffer);
                Pcre2Func<BS>::substring_free(buffer);     //must free memory
            }
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
    if(vec_soff) vec_soff->clear();
    if(vec_eoff) vec_eoff->clear();

    //~ //check if jit code is available and assign jit stack if user wants it.
    //~ SIZE_T jit_size = 0;
    //~ Pcre2Func<BS>::pattern_info(re->code, PCRE2_INFO_JITSIZE, &jit_size);
    //~ if(jit_size && jit_stack_startsize) assignJitStack();

    /* Using this function ensures that the block is exactly the right size for
     the number of capturing parentheses in the pattern. */

    match_data = Pcre2Func<BS>::match_data_create_from_pattern(re->code, 0);

    rc = Pcre2Func<BS>::match(  re->code,       /* the compiled pattern */
                                subject,        /* the subject string */
                                subject_length, /* the length of the subject */
                                _start_offset,  /* start at offset 'start_offset' in the subject */
                                match_opts,     /* default options */
                                match_data,     /* block for storing the result */
                                mcontext);      /* use default match context */

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
        error_number = ERROR::INSUFFICIENT_OVECTOR;
        rc = Pcre2Func<BS>::get_ovector_count(match_data);
    }
    //match succeeded at offset ovector[0]
    if(vec_soff) vec_soff->push_back(ovector[0]);
    if(vec_eoff) vec_eoff->push_back(ovector[1]);

    // Get numbered substrings if #num_sub isn't null
    if (num_sub) { //must do null check
        if(!getNumberedSubstrings(rc, match_data))
            return count;
    }
    
    //get named substrings if either nas_map or ntn_map is given.
    if (nas_map || ntn_map) {
        /* See if there are any named substrings, and if so, show them by name. First
         we have to extract the count of named parentheses from the pattern. */

        (void) Pcre2Func<BS>::pattern_info( re->code,               /* the compiled pattern */
                                            PCRE2_INFO_NAMECOUNT,   /* get the number of named substrings */
                                            &namecount);            /* where to put the answer */

        if (namecount <= 0); /*No named substrings*/

        else {
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


            // Get named substrings if #nas_map isn't null.
            // Get name to number map if #ntn_map isn't null.
            
            if(!getNamedSubstrings(namecount, name_entry_size, name_table, match_data))
                return count;
        }
    }

    // Populate vectors with their associated maps.
    pushMapsIntoVectors();

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
                                    mcontext);      /* use match context */

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
            error_number = ERROR::INSUFFICIENT_OVECTOR;
            rc = Pcre2Func<BS>::get_ovector_count(match_data);
        }
        
        //match succeded at ovector[0]
        if(vec_soff) vec_soff->push_back(ovector[0]);
        if(vec_eoff) vec_eoff->push_back(ovector[1]);
        
        /* As before, get substrings stored in the output vector by number, and then
         also any named substrings. */

        /// Get numbered substrings if #num_sub isn't null
        if (num_sub) { //must do null check
            if(!getNumberedSubstrings(rc, match_data))
                return count;
        }

        if (nas_map || ntn_map) {
            if (namecount <= 0); /*No named substrings*/
            else {
                /// Get named substrings if #nas_map isn't null.
                /// Get name to number map if #ntn_map isn't null.
                if(!getNamedSubstrings(namecount, name_entry_size, name_table, match_data))
                    return count;
            }
        }

        /// Populate vectors with their associated maps.
        pushMapsIntoVectors();
        
    } /* End of loop to find second and subsequent matches */

    Pcre2Func<BS>::match_data_free(match_data);
    // Must not free code. This function has no right to modify regex.
    return count;
}

#undef JPCRE2_VECTOR_DATA_ASSERT
#undef JPCRE2_UNUSED

//some macro documentation for doxygen

#ifdef __DOXYGEN__

#ifndef JPCRE2_DISABLE_CHAR1632
#define JPCRE2_DISABLE_CHAR1632
#endif
#ifndef JPCRE2_USE_FUNCTION_POINTER_CALLBACK
#define JPCRE2_USE_FUNCTION_POINTER_CALLBACK
#endif
#ifndef JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION
#define JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION
#endif

///@def JPCRE2_DISABLE_CHAR1632
///Disable support for `char16_t` and `char32_t`.
///Older compiler which supposedly supports C++11
///but does not have `std::wstring_convert` will not work with `char16_t` and `char32_t`, in this case
///define this macro before including `jpcre2.hpp` to disable `char16_t` and `char32_t` support.

///@def JPCRE2_USE_FUNCTION_POINTER_CALLBACK
///Use function pointer in all cases for MatchEvaluatorCallback function.
///By default function pointer is used for callback in MatchEvaluator when using <C++11 compiler, but for
///`>=C++11` compiler `std::function` instead of function pointer is used.
///If this macro is defined before including jpcre2.hpp, function pointer will be used in all cases.
///It you are using lambda function with captures, stick with `std::function`, on the other hand, if
///you are using older compilers, you might want to use function pointer instead.
///
///For example, with gcc-4.7, `std::function` will give compile error in C++11 mode, in such cases where full C++11
///support is not available, use function pointer.

///@def JPCRE2_DISABLE_CODE_UNIT_WIDTH_VALIDATION
///By default JPCRE2 checks if the code unit width equals to
///sizeof(Char_T)*CHAR_BIT, if not,it will produce compile error.
///This check can be disabled by defining this macro.


///@def JPCRE2_ASSERT(cond, msg)
///Macro to call `jpcre2::assert()` with file path and line number.
///When NDEBUG is defined before including this header, this macro will
///be defined as `((void)0)` thus eliminating this assertion.
///@param cond condtion (boolean)
///@param msg message

#endif


#endif
