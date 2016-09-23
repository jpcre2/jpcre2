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
 * separately in your program. Make sure to link both JPCRE2 and PCRE2 library when compiling.
 *
 * If you are using JPCRE2 with all of its source files, you won't need to link it with JPCRE2 library, but do remember that you
 * still need to link with  PCRE2 library.
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 */

#ifndef JPCRE2_HPP
#define JPCRE2_HPP



#ifndef PCRE2_CODE_UNIT_WIDTH
    #ifdef JPCRE2_CODE_UNIT_WIDTH //JPCRE2_CODE_UNIT_WIDTH should not be used, it's provided just for convenience
        #define PCRE2_CODE_UNIT_WIDTH JPCRE2_CODE_UNIT_WIDTH
    //#else
    //    #define PCRE2_CODE_UNIT_WIDTH 8 ///< 8 bit code by default (for testing)
	#else
	#error PCRE2_CODE_UNIT_WIDTH must be defined before including jpcre2.hpp
    #endif
#endif

#if PCRE2_CODE_UNIT_WIDTH == 0
	#define JPCRE2_DISABLE_CODE_UNIT_VALIDATION //when code unit is 0, this validation method becomes invalid
#endif

/// Disable all assertions
#ifdef JPCRE2_DISABLE_ERROR
    /// Disable assert() to prevent terminating program in case error/undefined behavior occurs
	#ifdef JPCRE2_ENABLE_CODE_UNIT_VALIDATION
		//Disable naive implementation of code unit validation
    	#undef JPCRE2_ENABLE_CODE_UNIT_VALIDATION
	#endif
    #define NDEBUG
#endif



#include <pcre2.h> 
//#include <stdint.h> 	// uint32_t		//pcre2 itself includes this
//#include <cstddef>  	// std::size_t
#include <string>  		// std::string, std::wstring
#include <vector>   	// std::vector
#include <map>      	// std::map
#include <cassert>  // assert
#include <climits>  // CHAR_BIT
#if __cplusplus >= 201103L
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


typedef PCRE2_SIZE SIZE_T;                      ///< Used for match count and vector size
typedef uint32_t Uint;                          ///< Used for options (bitwise operation)

/// @namespace jpcre2::ERROR
/// Namespace for error codes.
namespace ERROR {
	/** ERROR numbers.
	 *  JPCRE2 error numbers are positive integers while
	 *  PCRE2 error numbers are negative integers.
	 */
	enum {
		INVALID_MODIFIER        = 2,        ///< Error number implying that invalid modifier was detected
		JIT_COMPILE_FAILED      = 3         ///< Error number implying that JIT compile failed
	};
}


extern const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE;    ///< Used by default to provide big enough initial buffer for replaced string
extern const std::string LOCALE_NONE;               ///< Don't do anything about locale if it is set to jpcre2::LOCALE_NONE
extern const std::string LOCALE_DEFAULT;            ///< Default locale


/** These constants provide JPCRE2 options.
 */
enum {
	NONE                    = 0x0000000u,   ///< Option 0 (zero)
	FIND_ALL                = 0x0000002u,   ///< Find all during match (global match)
	JIT_COMPILE             = 0x0000004u    ///< Perform JIT compilation for optimization
};

/** @namespace jpcre2::INFO
 *  Namespace to provide information about JPCRE2 library itself.
 *  Contains constant Strings with version info.
 */
namespace INFO {
	extern const std::string NAME;					///< Name of the project
	extern const std::string FULL_VERSION;			///< Full version string
	extern const std::string VERSION_GENRE;			///< Generation, depends on original PCRE2 version
	extern const std::string VERSION_MAJOR;			///< Major version, updated when API change is made
	extern const std::string VERSION_MINOR;			///< Minor version, includes bug fix or minor feature upgrade
	extern const std::string VERSION_PRE_RELEASE;	///< Alpha or beta (testing) release version
}

/** @namespace jpcre2::MOD
 *  Namespace for modifier constants.
 *  For each modifier constant there is a jpcre2::Uint option value.
 *  Some modifiers may have multiple values set together (ORed in bitwise operation) and
 *  thus they may include other modifiers. Such an example is the 'n' modifier. It is combined together with 'u'.
 */
namespace MOD {
    extern const std::string C_N; 	///< String of compile modifier characters for PCRE2 options
    extern const Uint C_V[]; 		///< Array of compile modifier values for PCRE2 options
    extern const std::string CJ_N; 	///< String of compile modifier characters for JPCRE2 options
    extern const Uint CJ_V[]; 		///< Array of compile modifier values for JPCRE2 options
    extern const std::string M_N; 	///< String of action (match) modifier characters for PCRE2 options
    extern const Uint M_V[];        ///< Array of action (match) modifier values for PCRE2 options
    extern const std::string MJ_N;  ///< String of action (match) modifier characters for JPCRE2 options
    extern const Uint MJ_V[];       ///< Array of action (match) modifier values for JPCRE2 options
    extern const std::string R_N;   ///< String of action (replace) modifier characters for PCRE2 options
    extern const Uint R_V[];        ///< Array of action (replace) modifier values for PCRE2 options
    extern const std::string RJ_N;  ///< String of action (replace) modifier characters for JPCRE2 options
    extern const Uint RJ_V[];       ///< Array of action (replace) modifier values for JPCRE2 options
}


#if __cplusplus >= 201103L
///@struct Codecvt
///Convenience wrapper of `std::codecvt`
template <class internT, class externT, class stateT>
struct Codecvt : std::codecvt<internT,externT,stateT>
{ ~Codecvt(){} };

///@var convert16
///This is a convenience object to convert between UTF-8 <> UTF-16.
///`convert16.to_bytes(utf16string)` will convert UTF-16 to UTF-8.
///`convert16.from_bytes(utf8string)` will convert UTF-8 to UTF-16.
static thread_local std::wstring_convert<Codecvt<char16_t,char,std::mbstate_t>,char16_t> convert16;
///@var convert32
///This is a convenience object to convert between UTF-8 <> UTF-32.
///`convert32.to_bytes(utf32string)` will convert UTF-32 to UTF-8.
///`convert32.from_bytes(utf8string)` will convert UTF-8 to UTF-32.
static thread_local std::wstring_convert<Codecvt<char32_t,char,std::mbstate_t>,char32_t> convert32;

#endif




#define JPCRE2_SELECT_FIRST \
template<class Char_T> \
struct select { \
virtual ~select(){}	\
\
typedef Char_T Char; \
typedef typename std::basic_string<Char_T> String; \
\
typedef typename std::map<String, String> MapNas; \
typedef typename std::map<SIZE_T, String> MapNum; \
typedef typename std::map<String, SIZE_T> MapNtN; \
typedef MapNtN MapNtn; \
typedef typename std::vector<MapNas> VecNas;             \
typedef typename std::vector<MapNtN> VecNtN;             \
typedef VecNtN VecNtn; \
typedef typename std::vector<MapNum> VecNum;             \
\
static const String JPCRE2_EMPTY_STRING; \
\
static String toString(int);    \
static String toString(Char);                     \
static String toString(const Char*);              \
static String toString(PCRE2_UCHAR*);             \
static String getPcre2ErrorMessage(int);          \
static String getErrorMessage(int, int);          \
static void Sprintf(Char*, int, int); \
\
\
class Regex; \
\
class RegexMatch { \
\
private: \
\
	Regex* re;              \
\
	String m_subject;       \
	Uint match_opts;        \
	Uint jpcre2_match_opts; \
\
	VecNum* vec_num;        \
	VecNas* vec_nas;        \
	VecNtN* vec_ntn;        \
\
	MapNum* num_map0;       \
	MapNas* nas_map0;       \
	MapNtN* ntn_map0;       \
\
	bool getNumberedSubstrings(int, pcre2_match_data *);   \
\
	bool getNamedSubstrings(int, int, PCRE2_SPTR, pcre2_match_data *);   \
    \
    void pushMapsIntoVectors(void){ \
        if (vec_num) \
            vec_num->push_back(*num_map0); \
        if (vec_nas) \
            vec_nas->push_back(*nas_map0); \
        if (vec_ntn) \
            vec_ntn->push_back(*ntn_map0); \
    } \
    \
	void init_vars() { \
		vec_num = 0; \
		vec_nas = 0; \
		vec_ntn = 0; \
		num_map0 = 0; \
		nas_map0 = 0; \
		ntn_map0 = 0; \
		match_opts = 0; \
		jpcre2_match_opts = 0; \
	} \
\
	RegexMatch() { \
		init_vars(); \
	} \
	RegexMatch(const RegexMatch&) { \
		init_vars(); \
	} \
	~RegexMatch() { \
		delete num_map0; \
		delete nas_map0; \
		delete ntn_map0; \
	} \
\
	friend class Regex; \
\
public: \
\
	RegexMatch& reset() { \
		delete num_map0; \
		delete nas_map0; \
		delete ntn_map0; \
		init_vars(); \
		m_subject = JPCRE2_EMPTY_STRING; \
		return *this; \
	} \
    String getSubject() { \
        return m_subject; \
    } \
\
	std::string getModifier(); \
    \
	Uint getPcre2Option() { \
		return match_opts; \
	} \
\
	Uint getJpcre2Option() { \
		return jpcre2_match_opts; \
	} \
\
    RegexMatch& resetErrors(); \
\
	RegexMatch& setNumberedSubstringVector(VecNum* v) { \
		vec_num = v; \
		return *this; \
	} \
\
	RegexMatch& setNamedSubstringVector(VecNas* v) { \
		vec_nas = v; \
		return *this; \
	} \
\
	RegexMatch& setNameToNumberMapVector(VecNtN* v) { \
		vec_ntn = v; \
		return *this; \
	} \
\
	RegexMatch& setSubject(const String& s) { \
		m_subject = s; \
		return *this; \
	} \
\
	RegexMatch& setModifier(const std::string& s) { \
		match_opts = 0; \
        jpcre2_match_opts = 0; \
        changeModifier(s, true); \
		return *this; \
	} \
\
	RegexMatch& setJpcre2Option(Uint x) { \
		jpcre2_match_opts = x; \
		return *this; \
	} \
\
	RegexMatch& setPcre2Option(Uint x) { \
		match_opts = x; \
		return *this; \
	} \
	RegexMatch& setFindAll(bool x) { \
		if (x) \
			jpcre2_match_opts |= FIND_ALL; \
		else \
			jpcre2_match_opts &= ~FIND_ALL; \
		return *this; \
	} \
\
	RegexMatch& setFindAll() { \
		return setFindAll(true); \
	} \
\
	RegexMatch& changeModifier(const std::string&, bool); \
\
    RegexMatch& changeJpcre2Option(Uint opt, bool x) { \
        if(x) \
            jpcre2_match_opts |= opt; \
        else \
            jpcre2_match_opts &= ~opt; \
        return *this; \
    } \
\
    RegexMatch& changePcre2Option(Uint opt, bool x) { \
        if(x) \
            match_opts |= opt; \
        else \
            match_opts &= ~opt; \
        return *this; \
    } \
    \
    RegexMatch& addModifier(const std::string& mod){ \
        return changeModifier(mod, true); \
    } \
\
	RegexMatch& addJpcre2Option(Uint x) { \
		jpcre2_match_opts |= x; \
		return *this; \
	} \
\
	RegexMatch& addPcre2Option(Uint x) { \
		match_opts |= x; \
		return *this; \
	}\
\
	SIZE_T match(void); \
}; \
\
class RegexReplace { \
\
private: \
\
	Regex* re;      \
\
	String r_subject;           \
	String r_replw;             \
	Uint replace_opts;          \
	Uint jpcre2_replace_opts;  \
	PCRE2_SIZE buffer_size;     \
	\
	void init_vars() { \
        replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; \
        jpcre2_replace_opts = 0; \
		buffer_size = SUBSTITUTE_RESULT_INIT_SIZE; \
	}\
\
	RegexReplace() { \
		init_vars(); \
	} \
\
	RegexReplace(const RegexReplace&) { \
		init_vars(); \
	} \
\
	~RegexReplace() {} \
\
	friend class Regex; \
\
public: \
\
	RegexReplace& reset() { \
		init_vars(); \
		r_subject = JPCRE2_EMPTY_STRING; \
		r_replw = JPCRE2_EMPTY_STRING; \
		return *this; \
	} \
    String getReplaceWith() { \
        return r_replw; \
    } \
\
    String getSubject() { \
        return r_subject; \
    } \
\
	std::string getModifier(); \
    \
	Uint getPcre2Option() { \
		return replace_opts; \
	} \
\
	Uint getJpcre2Option() { \
		return jpcre2_replace_opts; \
	} \
\
    RegexReplace& resetErrors(); \
\
	RegexReplace& setSubject(const String& s) { \
		r_subject = s; \
		return *this; \
	} \
\
	RegexReplace& setReplaceWith(const String& s) { \
		r_replw = s; \
		return *this; \
	} \
	RegexReplace& setModifier(const std::string& s) { \
		replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; /* must not be initialized to 0 */ \
        jpcre2_replace_opts = 0; \
        return changeModifier(s, true); \
	} \
\
	RegexReplace& setBufferSize(PCRE2_SIZE x) { \
		buffer_size = x; \
		return *this; \
	} \
\
	RegexReplace& setJpcre2Option(Uint x) { \
		jpcre2_replace_opts = x; \
		return *this; \
	} \
\
	RegexReplace& setPcre2Option(Uint x) { \
		replace_opts = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH | x; \
		return *this; \
	} \
\
    RegexReplace& changeModifier(const std::string&, bool); \
    RegexReplace& changeJpcre2Option(Uint opt, bool x) { \
        if(x) \
            jpcre2_replace_opts |= opt; \
        else \
            jpcre2_replace_opts &= ~opt; \
        return *this; \
    } \
\
    RegexReplace& changePcre2Option(Uint opt, bool x) { \
        if(x) \
            replace_opts |= opt; \
        else \
            replace_opts &= ~opt; \
            replace_opts |= PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; /* It's important, this option must not be removed */ \
        return *this; \
    } \
    \
    RegexReplace& addModifier(const std::string& mod){ \
        return changeModifier(mod, true); \
    } \
\
	RegexReplace& addJpcre2Option(Uint x) { \
		jpcre2_replace_opts |= x; \
		return *this; \
	} \
	RegexReplace& addPcre2Option(Uint x) { \
		replace_opts |= x; \
		return *this; \
	} \
    \
	String replace(void); \
}; \
\
class Regex { \
\
private: \
\
	RegexMatch *rm;		        \
	RegexReplace *rr;	        \
\
	String pat_str;		        \
	pcre2_code *code;	        \
	Uint compile_opts;         \
	Uint jpcre2_compile_opts;	\
	std::string mylocale;	        \
    int error_number;          \
    PCRE2_SIZE error_offset;    \
\
	void init_vars() { \
        JPCRE2_CODE_UNIT_ASSERT  \
		mylocale = LOCALE_DEFAULT; \
		jpcre2_compile_opts = 0; \
		compile_opts = 0; \
        error_number = 0; \
        error_offset = 0; \
		code = 0; \
		rr = 0; \
		rm = 0; \
	} \
\
	void init() { \
		init_vars(); \
	} \
\
	void init(const String& re, const std::string& mod) { \
		init_vars(); \
		pat_str = re; \
        changeModifier(mod, true); \
	} \
\
	void init(const String& re, Uint po, Uint jo) { \
		init_vars(); \
		pat_str = re; \
		compile_opts = po; \
		jpcre2_compile_opts = jo; \
	} \
\
	void freeRegexMemory(void) { \
		if (code) /* We don't need to free code if it's null */ \
			pcre2_code_free(code); \
	}\
\
	friend class RegexMatch;	\
	friend class RegexReplace; \
\
	void shallowCopy(const Regex& r) { \
		pat_str = r.pat_str; \
		mylocale = r.mylocale; \
		compile_opts = r.compile_opts; \
		jpcre2_compile_opts = r.jpcre2_compile_opts; \
		error_number = r.error_number; \
        error_offset = r.error_offset; \
	} \
\
	void deepCopy(const Regex&); \
\
public: \
\
	Regex() { \
		init(); \
	} \
\
	Regex(const String& re) { \
		compile(re, 0, 0); \
	} \
	Regex(const String& re, const std::string& mod) { \
		compile(re, mod); \
	} \
\
	Regex(const String& re, Uint pcre2_opts) { \
		compile(re, pcre2_opts, 0); \
	} \
\
	Regex(const String& re, Uint pcre2_opts, Uint opt_bits) { \
		compile(re, pcre2_opts, opt_bits); \
	} \
\
	Regex(const Regex& r) { \
		shallowCopy(r); /* shallow copy must be performed **before** deep copy */ \
		deepCopy(r); \
	} \
	Regex& operator=(const Regex& r) { \
		if (this == &r) \
			return *this; \
		shallowCopy(r); /* shallow copy must be performed before deep copy */ \
		deepCopy(r); \
		return *this; \
	}
    
    /*
	#if __cplusplus >= 201103L
    explicit operator bool() const {
        return (code != 0);
    }
    #endif
    */
    
    #define JPCRE2_SELECT_LAST \
    bool operator!() const { \
        return (code == 0); \
    } \
\
	~Regex() { \
		freeRegexMemory(); \
		delete rm; /* Deleting null pointer is perfectly safe, no check needed. */ \
		delete rr; /* Deleting null pointer is perfectly safe, no check needed. */ \
	} \
\
	Regex& reset() { \
		freeRegexMemory(); \
		init(); \
        delete rm; rm = 0; /* deleting null pointer is safe. */ \
        delete rr; rr = 0; \
		return *this; \
	} \
\
    Regex& resetErrors() { \
        error_number = 0; \
        error_offset = 0; \
        return *this; \
    } \
\
	String getPattern() { \
		return pat_str; \
	} \
	std::string getLocale() { \
		return mylocale; \
	} \
\
	std::string getModifier(); \
\
	Uint getPcre2Option() { \
		return compile_opts; \
	} \
\
	Uint getJpcre2Option() { \
		return jpcre2_compile_opts; \
	} \
\
    int getErrorNumber(){ \
        return error_number; \
    } \
\
    int getErrorOffset() { \
        return (int)error_offset; \
    } \
    String getErrorMessage() { \
        return select<Char>::getErrorMessage(error_number, error_offset); \
    } \
	Regex& setPattern(const String& re) { \
		pat_str = re; \
		return *this; \
	} \
\
	Regex& setModifier(const std::string& x) { \
		compile_opts = 0; \
        jpcre2_compile_opts = 0; \
		return changeModifier(x, true); \
	} \
\
	Regex& setLocale(const std::string& x) { \
		mylocale = x; \
		return *this; \
	} \
	Regex& setJpcre2Option(Uint x) { \
		jpcre2_compile_opts = x; \
		return *this; \
	} \
\
	Regex& setPcre2Option(Uint x) { \
		compile_opts = x; \
		return *this; \
	} \
\
    Regex& changeModifier(const std::string&, bool); \
\
    Regex& changeJpcre2Option(Uint opt, bool x) { \
        if(x) \
            jpcre2_compile_opts |= opt; \
        else \
            jpcre2_compile_opts &= ~opt; \
        return *this; \
    } \
 \
    Regex& changePcre2Option(Uint opt, bool x) { \
        if(x) \
            compile_opts |= opt; \
        else \
            compile_opts &= ~opt; \
        return *this; \
    } \
    Regex& addModifier(const std::string& mod){ \
        return changeModifier(mod, true); \
    } \
\
	Regex& addJpcre2Option(Uint x) { \
		jpcre2_compile_opts |= x; \
		return *this; \
	} \
\
	Regex& addPcre2Option(Uint x) { \
		compile_opts |= x; \
		return *this; \
	} \
\
	void compile(void); \
\
	void compile(const String& re, Uint po, Uint jo) { \
		init(re, po, jo); \
		compile(); \
	} \
	void compile(const String& re, Uint po) { \
		init(re, po, 0); \
		compile(); \
	} \
\
	void compile(const String& re, const std::string& mod) { \
		init(re, mod); \
		compile(); \
	} \
\
	void compile(const String& re) { \
		init(re, 0, 0); \
		compile(); \
	} \
	SIZE_T match(const String& s, const std::string& mod) { \
		return getMatchObject().setSubject(s).setModifier(mod).match(); \
	} \
\
	SIZE_T match(const String& s) { \
		return getMatchObject().setSubject(s).match(); \
	} \
\
	RegexMatch& initMatch() { \
		delete rm; /* rm is either occupied or NULL, double deletion won't happen */ \
		rm = new RegexMatch(); \
		rm->re = this; \
		return *rm; \
	} \
  \
    RegexMatch& getMatchObject(){ \
        if(rm != 0) return *rm; \
        else return initMatch(); \
    } \
	String replace(const String& mains, const String& repl, const std::string& mod) { \
		return getReplaceObject().setSubject(mains).setReplaceWith(repl).setModifier(mod).replace(); \
	} \
\
	String replace(const String& mains, const String& repl) { \
		return getReplaceObject().setSubject(mains).setReplaceWith(repl).replace(); \
	} \
\
	RegexReplace& initReplace() { \
		delete rr; /* rr is either occupied or NULL, double deletion won't happen */ \
		rr = new RegexReplace(); \
		rr->re = this; \
		return *rr; \
	} \
    \
    RegexReplace& getReplaceObject() { \
        if(rr != 0) return *rr; \
        else return initReplace(); \
    } \
}; \
}; 
/* struct select */


#define JPCRE2_JOIN(a,b) a ## b
#define JPCRE2_GLUE(a,b) JPCRE2_JOIN(a,b)
#define JPCRE2_SUFFIX(a) JPCRE2_GLUE(a,JPCRE2_LOCAL_WIDTH)

///Enable a naive validation check on Code unit width and the selected character type
#ifdef JPCRE2_ENABLE_CODE_UNIT_VALIDATION
	#define JPCRE2_CODE_UNIT_ASSERT \
	assert(PCRE2_CODE_UNIT_WIDTH == sizeof(Char)*CHAR_BIT);
#else
	#define JPCRE2_CODE_UNIT_ASSERT
#endif



#ifdef select
#define JPCRE2_SELECT_BKP select
#undef select
#endif

#define select JPCRE2_SUFFIX(select)

//Create three sets of declaration for select8, select16 and select32


#define JPCRE2_LOCAL_WIDTH 8
// Expand
JPCRE2_SELECT_FIRST

	#if __cplusplus >= 201103L
    explicit operator bool() const {
        return (code != 0);
    }
    #endif
JPCRE2_SELECT_LAST
#undef JPCRE2_LOCAL_WIDTH



#define JPCRE2_LOCAL_WIDTH 16
// Expand
JPCRE2_SELECT_FIRST

	#if __cplusplus >= 201103L
    explicit operator bool() const {
        return (code != 0);
    }
    #endif
JPCRE2_SELECT_LAST
#undef JPCRE2_LOCAL_WIDTH


#define JPCRE2_LOCAL_WIDTH 32
// Expand
JPCRE2_SELECT_FIRST

	#if __cplusplus >= 201103L
    explicit operator bool() const {
        return (code != 0);
    }
    #endif
JPCRE2_SELECT_LAST
#undef JPCRE2_LOCAL_WIDTH
#undef select

#ifdef JPCRE2_SELECT_BKP
#define select JPCRE2_SELECT_BKP
#undef JPCRE2_SELECT_BKP
#endif


#if PCRE2_CODE_UNIT_WIDTH == 8
    template<class Char_T>
    struct select: virtual public select8<Char_T> { virtual ~select(); };
#elif PCRE2_CODE_UNIT_WIDTH == 16
    template<class Char_T>
    struct select: virtual public select16<Char_T> { virtual ~select(); };
#elif PCRE2_CODE_UNIT_WIDTH == 32
    template<class Char_T>
    struct select: virtual public select32<Char_T> { virtual ~select(); };
#endif


} // jpcre2 namespace

#undef JPCRE2_GLUE
#undef JPCRE2_JOIN
#undef JPCRE2_SUFFIX
#undef JPCRE2_SELECT_FIRST
#undef JPCRE2_SELECT_LAST
#undef JPCRE2_CODE_UNIT_ASSERT


#endif
