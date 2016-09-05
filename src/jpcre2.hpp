/* *****************************************************************************
 * ******************* C++ wrapper of PCRE2 Library ****************************
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
 * @brief Main header file for JPCRE library to be included by programs that call JPCRE2 functions.
 * It includes the pcre2.h header, therefore you shouldn't include pcre2.h
 * separately in your program. Make sure to link pcre2 library when compiling.
 * If you are using JPCRE2 as a library, then link this library too.
 * @author Md. Jahidul Hamid.
 */

#ifndef JPCRE2_HPP
#define JPCRE2_HPP

#ifndef PCRE2_CODE_UNIT_WIDTH
/// Code unit width 8 is used by default
#define PCRE2_CODE_UNIT_WIDTH 8
#endif

#include <pcre2.h> 
#include <stdint.h> 	// uint32_t
#include <cstddef>  	// std::size_t
#include <string>  		// std::string
#include <vector>   	// std::vector
#include <map>      	// std::map

/// Main namespace of JPCRE2
namespace jpcre2 {

typedef std::size_t SIZE_T;           			///< Used for match count and vector size
typedef uint32_t Uint;                			///< Used for options (bitwise operation)
typedef std::string String;                   	///< Used as std::string
typedef std::map<String, String> MapNas;       	///< Map for Named substrings
typedef std::map<SIZE_T, String> MapNum;       	///< Map for Numbered substrings
typedef std::map<String, SIZE_T> MapNtN; 		///< Substring name to Substring number map
typedef MapNtN MapNtn; 							///< Allow spelling mistake of MapNtN as MapNtn
typedef std::vector<MapNas> VecNas; 			///< Vector of matches with named substrings
typedef std::vector<MapNtN> VecNtN; 			///< Vector of substring name to Substring number map
typedef VecNtN VecNtn; 							///< Allow spelling mistake of VecNtN as VecNtn
typedef std::vector<MapNum> VecNum; 			///< Vector of matches with numbered substrings

/// Namespace for error codes
namespace ERROR {
/** ERROR codes that are thrown in case error occurs.
 *  JPCRE2 error codes are positive integers while
 *  PCRE2 error codes are negative integers.
 */
enum {
	INVALID_MODIFIER = 2,   	///< Invalid modifier error
	JIT_COMPILE_FAILED = 3    	///< JIT compile failed error
};
}

extern const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE; ///< Used by default to provide big enough buffer for replaced string
extern const String LOCALE_NONE; ///< Don't do anything about locale if it is set to #LOCALE_NONE
extern const String LOCALE_DEFAULT;              ///< Default locale
extern const String JIT_ERROR_MESSAGE_PREFIX; ///< Prefix to be added to JIT error message

/// Option bits.
enum {
	NONE = 0x0000000u, ///< Option 0 (zero)
	VALIDATE_MODIFIER = 0x0000001u, ///< throw INVALID_MODIFIER if any modifier is wrong
	FIND_ALL = 0x0000002u, ///< Find all during match (global match)
	JIT_COMPILE = 0x0000004u, ///< Prform JIT compilation during pattern compilation
	ERROR_ALL = 0x0000008u  ///< Treat warnings as error and throw exception
};

/// Namespace for some utility functions
namespace utils {
extern String toString(int a);  //!< Converts an integer to String
extern String toString(char a); ///< Converts a char to String
extern String toString(const char* a); ///< Converts const char* to String
extern String toString(PCRE2_UCHAR* a); ///< Converts a PCRE2_UCHAR* to String
extern String getPcre2ErrorMessage(int err_num); ///< Get PCRE2 error message for an error number
}

class Regex;

/** @class RegexMatch
 *  Performs regex matching.
 *  Provides overloaded and chained methods to set various options.
 *
 *  All constructors of this class are private.
 */
class RegexMatch {

private:

	Regex* re;    //We will use this to access private members in Regex

	String m_subject;
	String m_modifier;
	Uint match_opts;
	Uint jpcre2_match_opts;

	VecNum* vec_num; ///< vector to contain the numbered substring maps
	VecNas* vec_nas; ///< vector to contain the named substring maps
	VecNtN* vec_ntn; ///< vector to contain the name to number maps

	//Maps to contain the captured groups, make sure to delete these in destructor
	MapNum* num_map0;
	MapNas* nas_map0;
	MapNtN* ntn_map0;

	/// @cond
	void parseMatchOpts(void);

	void getNumberedSubstrings(int rc, pcre2_match_data *match_data);

	void getNamedSubstrings(int namecount, int name_entry_size,
			PCRE2_SPTR tabptr, pcre2_match_data *match_data);

	void init_vars() {
		vec_num = 0;
		vec_nas = 0;
		vec_ntn = 0;
		num_map0 = 0;
		nas_map0 = 0;
		ntn_map0 = 0;
		match_opts = 0;
		jpcre2_match_opts = 0;
	}

	RegexMatch() {
		init_vars();
	}

	RegexMatch(const RegexMatch&) {
		init_vars();
	}    // No need to implement copy constructor

	~RegexMatch() {
		//Delete map pointers
		delete num_map0;
		delete nas_map0;
		delete ntn_map0;
	}

	/// @endcond

	/// Define class Regex as friend and thus allow Regex to create object of this class
	friend class Regex;

public:

	/// Set a pointer to the numbered substrings vector
	/// @param v #vec_num
	/// @return *this
	RegexMatch& setNumberedSubstringVector(VecNum* v) {
		vec_num = v;
		return *this;
	}

	/// Set a pointer to the named substrings vector
	/// @param v #vec_nas
	/// @return *this
	RegexMatch& setNamedSubstringVector(VecNas* v) {
		vec_nas = v;
		return *this;
	}

	/// Set a pointer to the name to number map vector
	/// @param v #vec_ntn
	/// @return *this
	RegexMatch& setNameToNumberMapVector(VecNtN* v) {
		vec_ntn = v;
		return *this;
	}

	/// Set the subject string
	/// @param s Subject string
	/// @return *this
	RegexMatch& setSubject(const String& s) {
		m_subject = s;
		return *this;
	}

	/// Set the modifier (overwrites existing JPCRE2 and PCRE2 option).
	/// Re-initializes the option bits for PCRE2 and JPCRE2 options, then sets the modifier.
	/// @param s Modifier string
	/// @return *this
	RegexMatch& setModifier(const String& s) {
		match_opts = jpcre2_match_opts = 0;
		m_modifier = s;
		return *this;
	}

	/// Set JPCRE2 option (overwrites existing option)
	/// @param x Option value
	/// @return *this
	RegexMatch& setJpcre2Option(Uint x) {
		jpcre2_match_opts = x;
		return *this;
	}

	/// Set PCRE2 option (overwrites existing option)
	/// @param x Option value
	/// #return *this
	RegexMatch& setPcre2Option(Uint x) {
		match_opts = x;
		return *this;
	}

	/// Set whether to perform global match
	/// @param x True or False
	/// @return *this
	RegexMatch& setFindAll(bool x = true) {
		if (x)
			jpcre2_match_opts |= FIND_ALL;
		else
			jpcre2_match_opts &= ~FIND_ALL;
		return *this;
	}

	/// Add option to existing JPCRE2 options
	/// @param x Option value
	/// @return *this
	RegexMatch& addJpcre2Option(Uint x) {
		jpcre2_match_opts |= x;
		return *this;
	}

	/// Add option to existing PCRE2 options
	/// @param x Option value
	/// @return *this
	RegexMatch& addPcre2Option(Uint x) {
		match_opts |= x;
		return *this;
	}

	/// Remove option from existing JPCRE2 option
	/// @param x Option value
	/// @return *this
	RegexMatch& removeJpcre2Option(Uint x) {
		jpcre2_match_opts &= ~x;
		return *this;
	}

	/// Remove option from existing PCRE2 option
	/// @param x Option value
	/// @return *this
	RegexMatch& removePcre2Option(Uint x) {
		match_opts &= ~x;
		return *this;
	}

	/// Returns the number of matches, stores the match results in the specified vectors (#vec_num, #vec_nas, #vec_ntn)
	/**
	 * @return Number of matches found
	 * @see SIZE_T match(const String& s)
	 * @see SIZE_T match(const String& s, const String& mod)
	 * */
	SIZE_T match(void);

	/** @overload
	 *  @param s Subject string
	 *  @see SIZE_T match(const String& s, const String& mod)
	 * */
	SIZE_T match(const String& s) {
		m_subject = s;
		return match();
	}

	/** @overload
	 * @param s Subject string
	 * @param mod Modifier
	 * @see SIZE_T match(const String& s)
	 * */
	SIZE_T match(const String& s, const String& mod) {
		m_subject = s;
		m_modifier = mod;
		return match();
	}
};

/** @class RegexReplace
 *  Performs regex replace on a string.
 *  Provides overloaded and chained methods to set various options.
 *
 *  All constructors of this class are private.
 */
class RegexReplace {

private:

	/// @cond
	Regex* re;    //We will use this to access private members in Regex

	String r_subject;
	String r_modifier;
	String r_replw;
	Uint replace_opts;
	Uint jpcre2_replace_opts;
	PCRE2_SIZE buffer_size;

	void parseReplacementOpts(void);

	void init_vars() {
		replace_opts = jpcre2_replace_opts = 0;
		buffer_size = SUBSTITUTE_RESULT_INIT_SIZE;
	}

	RegexReplace() {
		init_vars();
	}

	RegexReplace(const RegexReplace& r) {
		init_vars();
	}

	~RegexReplace() {
	}
	/// @endcond
	/// Define Regex as a friend so that it can create object of this class
	friend class Regex;

public:

	/** Set the subject string
	 * @param s Subject string
	 * @return *this
	 * */
	RegexReplace& setSubject(const String& s) {
		r_subject = s;
		return *this;
	}

	/** Set the replacement string
	 * @param s String to replace with
	 * @return *this
	 * */
	RegexReplace& setReplaceWith(const String& s) {
		r_replw = s;
		return *this;
	}

	/** Set the modifier string (overwrites existing JPCRE2 and PCRE2 option).
	 * @param s Modifier string
	 * @return *this
	 * */
	RegexReplace& setModifier(const String& s) {
		replace_opts = jpcre2_replace_opts = 0;
		r_modifier = s;
		return *this;
	}

	/** Set the initial buffer size to be allocated for replaced string (used by PCRE2)
	 * @param x Buffer size
	 * @return *this
	 * */
	RegexReplace& setBufferSize(PCRE2_SIZE x) {
		buffer_size = x;
		return *this;
	}

	/** Set JPCRE2 option (overwrites existing option)
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& setJpcre2Option(Uint x) {
		jpcre2_replace_opts = x;
		return *this;
	}

	/** Set PCRE2 option (overwrites existing option)
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& setPcre2Option(Uint x) {
		replace_opts = x;
		return *this;
	}

	/** Add specified JPCRE2 option to existing options
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& addJpcre2Option(Uint x) {
		jpcre2_replace_opts |= x;
		return *this;
	}

	/** Add specified PCRE2 option to existing options
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& addPcre2Option(Uint x) {
		replace_opts |= x;
		return *this;
	}

	/** Remove JPCRE2 option from existing options
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& removeJpcre2Option(Uint x) {
		jpcre2_replace_opts &= ~x;
		return *this;
	}

	/** Remove PCRE2 option from existing options
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& removePcre2Option(Uint x) {
		replace_opts &= ~x;
		return *this;
	}

	/** Returns a replaced string after performing regex replace.
	 *@return Resultant string after regex replace
	 * */
	String replace(void);

	/** @overload
	 *  @param s Subject string
	 *  @param r Replacement string (string to replace with)
	 *  @param m Modifier string
	 * */
	String replace(const String& s, const String& r, const String& m) {
		r_subject = s;
		r_replw = r;
		r_modifier = m;
		return replace();
	}

	/** @overload
	 *  @param s Subject string
	 *  @param r Replacement string (string to replace with)
	 * */
	String replace(const String& s, const String& r) {
		r_subject = s;
		r_replw = r;
		return replace();
	}

	/** @overload
	 *  @param s Subject string
	 * */
	String replace(const String& s) {
		r_subject = s;
		return replace();
	}
};

/** @class Regex
 * This is the main class that is to be used to perform all JPCRE2 related actions.
 *
 * This class implements public default constructor, deep copy constructor and overloaded assignment operator.
 *
 * An object of this class must be created (can be temporary) to access its public functions and perform match or
 * replace against a compiled pattern.
 *
 * A pattern must be compiled either by explicitly calling the compile function or using one of the parameterized constructors.
 *
 * */
class Regex {

private:

	RegexMatch *rm;		///< Pointer to RegexMatch object
	RegexReplace *rr;	///< Pointer to RegexReplace object

	String pat_str;		///< Pattern string
	String modifier;	///< Modifier string
	pcre2_code *code;	///< Compiled pattern (code)
	int error_number;	///< Error number
	PCRE2_SIZE error_offset;	///< Error offset
	Uint compile_opts;	///< Compile options
	Uint jpcre2_compile_opts;	///< Compile options specific to JPCRE2
	String mylocale;	///< Locale string
	String current_warning_msg;	///< current warning message

	/// @cond
	void init_vars() {
		mylocale = LOCALE_DEFAULT;
		jpcre2_compile_opts = 0;
		error_number = 0;
		error_offset = 0;
		compile_opts = 0;
		code = 0;
		rr = 0;
		rm = 0;
	}

	void init() {
		init_vars();
	}

	void init(const String& re, const String& mod) {
		init_vars();
		pat_str = re;
		modifier = mod;
	}

	void init(const String& re, Uint pcre2_opts, Uint opt_bits) {
		init_vars();
		pat_str = re;
		compile_opts = pcre2_opts;
		jpcre2_compile_opts = opt_bits;
	}

	void freeRegexMemory(void) {
		if (code)
			pcre2_code_free(code);
	}

	void parseCompileOpts(void);

	/// Define RegexMatch and  for RegexReplace as friends. They need to access the compiled pattern which is a private to this class.
	friend class RegexMatch;
	friend class RegexReplace;

	void shallowCopy(const Regex& r) {
		pat_str = r.pat_str;
		mylocale = r.mylocale;
		modifier = r.modifier;
		error_number = r.error_number;
		error_offset = r.error_offset;
		compile_opts = r.compile_opts;
		jpcre2_compile_opts = r.jpcre2_compile_opts;
		current_warning_msg = r.current_warning_msg;
	}

	void deepCopy(const Regex& r);
	/// @endcond

public:

	/// Default Constructor.
	/// Initializes all class variables to defaults.
	/// Does not perform any compilation.
	Regex() {
		init();
	}

	/** Compiles pattern with initialization.
	 *  @param re Pattern string
	 * */
	Regex(const String& re) {
		compile(re, 0, 0);
	}

	/** @overload
	 *  Compiles pattern.
	 *  @param re Pattern string
	 *  @param mod Modifier string
	 * */
	Regex(const String& re, const String& mod) {
		compile(re, mod);
	}

	/** @overload
	 *  Compiles pattern.
	 *  @param re Pattern string
	 *  @param pcre2_opts PCRE2 option value
	 * */
	Regex(const String& re, Uint pcre2_opts) {
		compile(re, pcre2_opts, 0);
	}

	/** @overload
	 *  Compiles pattern.
	 *  @param re Pattern string
	 *  @param pcre2_opts	PCRE2 option value
	 *  @param opt_bits		JPCRE2 option value
	 * */
	Regex(const String& re, Uint pcre2_opts, Uint opt_bits) {
		compile(re, pcre2_opts, opt_bits);
	}

	/** @overload
	 *  Copy constructor\. Compiles pattern and Performs a deep copy.
	 */
	Regex(const Regex& r) { ///shallow copy must be performed **before** deep copy
		shallowCopy(r);
		deepCopy(r);
	}

	/// Overloaded assignment operator.
	/// Performs a deep copy
	/// @return *this
	Regex& operator=(const Regex& r) {
		if (this == &r)
			return *this;
		///shallow copy must be performed before deep copy
		shallowCopy(r);
		deepCopy(r);
		return *this;
	}

	/// Destructor (deletes memory used)
	~Regex() {
		freeRegexMemory();
		delete rm;
		delete rr;
	}

	/** Reset all class variables to its default (initial) state
	 * @return *this
	 * */
	Regex& reset() {
		freeRegexMemory();
		init();
		return *this;
	}

	/** Get modifier string
	 *  @return #modifier
	 * */
	String getModifier() {
		return modifier;
	}

	/** Get pattern string
	 *  @return #pat_str
	 * */
	String getPattern() {
		return pat_str;
	}

	/** Get locale as a string
	 *  @return #mylocale
	 * */
	String getLocale() {
		return mylocale;
	}

	/** Get PCRE2 option
	 *  @return #compile_opts
	 * */
	Uint getPcre2Option() {
		return compile_opts;
	}

	/** Get JPCRE2 option
	 *  @return #jpcre2_compile_opts
	 * */
	Uint getJpcre2Option() {
		return jpcre2_compile_opts;
	}

	/** Return error message by error number and error offset
	 *  @param err_num Error number
	 *  @param err_off Error offset
	 *  @return Error message as a string
	 * */
	String getErrorMessage(int err_num, PCRE2_SIZE err_off);

	/** @overload
	 *  Uses class variable error_offest as error offset
	 *  @param err_num
	 *  @return Error message as a string
	 * */
	String getErrorMessage(int err_num) {
		return getErrorMessage(err_num, error_offset);
	}

	/** @overload
	 *  Uses class variable error_number as error number and error_offest as error offset
	 *  @return Error message as a string (empty if there is no error)
	 * */
	String getErrorMessage() {
		return getErrorMessage(error_number, error_offset);
	}

	/// Get current warning message
	/// @return #current_warning_msg
	String getWarningMessage() {
		return current_warning_msg;
	}

	/// return #error_number
	int getErrorNumber() {
		return error_number;
	}

	/// return #error_offset
	PCRE2_SIZE getErrorOffset() {
		return error_offset;
	}

	/// Set the Pattern string
	/// @param re Pattern string
	/// @return *this
	Regex& setPattern(const String& re) {
		pat_str = re;
		return *this;
	}

	/// Set the modifier (overwrites existing JPCRE2 and PCRE2 option).
	/// Re-initializes the option bits for PCRE2 and JPCRE2 options, then sets the modifier.
	/// @param x Modifier string
	/// @return *this
	Regex& setModifier(const String& x) {
		compile_opts = jpcre2_compile_opts = 0;
		modifier = x;
		return *this;
	}

	/// Set the locale.
	/// @param x Locale string
	/// @return *this
	Regex& setLocale(const String& x) {
		mylocale = x;
		return *this;
	}

	/// Set JPCRE2 option (overwrites existing option)
	/// @param x Option value
	/// @return *this
	Regex& setJpcre2Option(Uint x) {
		jpcre2_compile_opts = x;
		return *this;
	}

	/// Set PCRE2 option (overwrites existing option)
	/// @param x Option value
	/// #return *this
	Regex& setPcre2Option(Uint x) {
		compile_opts = x;
		return *this;
	}

	/// Add option to existing JPCRE2 options
	/// @param x Option value
	/// @return *this
	Regex& addJpcre2Option(Uint x) {
		jpcre2_compile_opts |= x;
		return *this;
	}

	/// Add option to existing PCRE2 options
	/// @param x Option value
	/// @return *this
	Regex& addPcre2Option(Uint x) {
		compile_opts |= x;
		return *this;
	}

	/// Remove option from existing JPCRE2 option
	/// @param x Option value
	/// @return *this
	Regex& removeJpcre2Option(Uint x) {
		jpcre2_compile_opts &= ~x;
		return *this;
	}

	/// Remove option from existing PCRE2 option
	/// @param x Option value
	/// @return *this
	Regex& removePcre2Option(Uint x) {
		compile_opts &= ~x;
		return *this;
	}

	/** Compiles the regex using values from class variables.
	 * */
	void compile(void);

	/** @overload
	 *  Initializes with class variables, sets the specified parameters, then compiles the pattern.
	 *  @param re Pattern string
	 *  @param po PCRE2 option
	 *  @param jo JPCRE2 option
	 * */
	void compile(const String& re, Uint po, Uint jo) {
		init(re, po, jo);
		compile();
	}

	/** @overload
	 *  Initializes with class variables, sets the specified parameters, then compiles the pattern.
	 *  @param re Pattern string
	 *  @param po PCRE2 option
	 * */
	void compile(const String& re, Uint po) {
		init(re, po, 0);
		compile();
	}

	/** @overload
	 *  Initializes with class variables, sets the specified parameters, then compiles the pattern.
	 *  @param re Pattern string
	 *  @param mod Modifier string
	 * */
	void compile(const String& re, const String& mod) {
		init(re, mod);
		compile();
	}

	/** @overload
	 *  Initializes with class variables, sets the specified parameters, then compiles the pattern.
	 *  @param re Pattern string
	 * */
	void compile(const String& re) {
		init(re, 0, 0);
		compile();
	}

	/** Perform regex match.
	 *  This function takes the parameters, then passes the parameters to the appropriate function
	 *  RegexMatch::match(const String& s, const String& mod) which returns the result
	 *  @see RegexMatch::match(const String& s, const String& mod)
	 * */
	SIZE_T match(const String& s, const String& mod) {
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		return rm->match(s, mod);
	}

	/** Perform regex match.
	 *  This function takes the parameters, then passes the parameters to the appropriate function
	 *  RegexMatch::match(const String& s) which returns the result
	 *  @see RegexMatch::match(const String& s)
	 * */
	SIZE_T match(const String& s) {
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		return rm->match(s);
	}

	/** Prepare to call RegexMatch::match().
	 * Other options can be set with the setter functions of RegexMatch class
	 * in-between the Regex::initMatch() and RegexMatch::match() call.
	 * @see RegexMatch::match()
	 * @see RegexMatch::setSubject(const String& s)
	 * @see RegexMatch::setModifier(const String& mod)
	 * @see RegexMatch::setNumberedSubstringVector(VecNum* vec_num)
	 * @see RegexMatch::setNamedSubstringVector(VecNas* vec_nas)
	 * @see RegexMatch::setNameToNumberMapVector(VecNtN* vec_ntn)
	 * */
	RegexMatch& initMatch() {
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		return *rm;
	}

	/** Perform regex replace.
	 *  This function takes the parameters, then passes the parameters to the appropriate function
	 *  RegexReplace::replace(const String& s, const String& r, const String& m) which returns the result
	 *  @see RegexReplace::replace(const String& s, const String& r, const String& m)
	 * */
	String replace(const String& mains, const String& repl, const String& mod) {
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		return rr->replace(mains, repl, mod);
	}

	/** Perform regex replace.
	 *  This function takes the parameters, then passes the parameters to the appropriate function
	 *  RegexReplace::replace(const String& s, const String& r) which returns the result
	 *  @see RegexReplace::replace(const String& s, const String& r)
	 * */
	String replace(const String& mains, const String& repl) {
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		return rr->replace(mains, repl);
	}

	/** Perform regex replace.
	 *  This function takes the parameters, then passes the parameters to the appropriate function
	 *  RegexReplace::replace(const String& s) which returns the result
	 *  @see RegexReplace::replace(const String& s)
	 * */
	String replace(const String& mains) {
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		return rr->replace(mains);
	}

	/** Prepare to call RegexReplace::replace().
	 * Other options can be set with the setter functions of RegexReplace class
	 * in-between the Regex::initReplace() and RegexReplace::replace() call.
	 * @see RegexReplace::replace()
	 * @see RegexReplace::setSubject(const String& s)
	 * @see RegexReplace::setModifier(const String& mod)
	 * @see RegexReplace::setReplaceWith(const String& s)
	 * @see RegexReplace::setBufferSize(PCRE2_SIZE x)
	 * */
	RegexReplace& initReplace() {
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		return *rr;
	}

};

} ///jpcre2 namespace

#endif
