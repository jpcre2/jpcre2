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
 * @brief Main header file for JPCRE2 library to be included by programs that uses its functionalities.
 * It includes the pcre2.h header, therefore you shouldn't include pcre2.h
 * separately in your program. Make sure to link both jpcre2 and pcre2 library when compiling.
 *
 * If you are using JPCRE2 with all of its source files, you won't need to link it with JPCRE2 library, but do remember that you
 * still need to link with  pcre2 library
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

/** Top level namespace of JPCRE2.
 *
 * All functions, classes, constants, enums that are provided by JPCRE2 belong to this namespace while
 * **PCRE2** functions, constants remain outside of its scope.
 *
 * If you want to use any PCRE2 functions or constants,
 *  remember that they are in the global scope and should be used as such.
 * */
namespace jpcre2 {

typedef std::size_t SIZE_T;                     ///< Used for match count and vector size
typedef uint32_t Uint;                          ///< Used for options (bitwise operation)
typedef std::string String;                     ///< Used as std::string
typedef std::map<String, String> MapNas;        ///< Map for Named substrings
typedef std::map<SIZE_T, String> MapNum;        ///< Map for Numbered substrings
typedef std::map<String, SIZE_T> MapNtN;        ///< Substring name to Substring number map
typedef MapNtN MapNtn;                          ///< Allow spelling mistake of MapNtN as MapNtn
typedef std::vector<MapNas> VecNas;             ///< Vector of matches with named substrings
typedef std::vector<MapNtN> VecNtN;             ///< Vector of substring name to Substring number map
typedef VecNtN VecNtn;                          ///< Allow spelling mistake of VecNtN as VecNtn
typedef std::vector<MapNum> VecNum;             ///< Vector of matches with numbered substrings

/// Namespace for error codes
namespace ERROR {

/** ERROR codes that are thrown in case error occurs.
 *  JPCRE2 error codes are positive integers while
 *  PCRE2 error codes are negative integers.
 * */
enum {
	INVALID_MODIFIER        = 2,        ///< Error to be thrown when invalid modifier detected
	JIT_COMPILE_FAILED      = 3         ///< Error to be thrown when JIT compile fails
};
}

extern const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE;    ///< Used by default to provide big enough initial buffer for replaced string
extern const String LOCALE_NONE;                    ///< Don't do anything about locale if it is set to #LOCALE_NONE
extern const String LOCALE_DEFAULT;                 ///< Default locale
extern const String JIT_ERROR_MESSAGE_PREFIX;       ///< Prefix to be added to JIT error message

/** These constants provide JPCRE2 options.
 * */
enum {
	NONE                    = 0x0000000u,   ///< Option 0 (zero)
	VALIDATE_MODIFIER       = 0x0000001u,   ///< Perform validation check on modifiers and throw #INVALID_MODIFIER if any wrong modifier is passed
	FIND_ALL                = 0x0000002u,   ///< Find all during match (global match)
	JIT_COMPILE             = 0x0000004u,   ///< Perform JIT compilation for optimization
	ERROR_ALL               = 0x0000008u    ///< Treat warnings as error and throw exception (warnings don't throw exception)
};

/// Namespace for some utility functions
namespace utils {
extern String toString(int a);                      ///< Converts an integer to String
extern String toString(char a);                     ///< Converts a char to String
extern String toString(const char* a);              ///< Converts const char* to String
extern String toString(PCRE2_UCHAR* a);             ///< Converts a PCRE2_UCHAR* to String
extern String getPcre2ErrorMessage(int err_num);    ///< Get PCRE2 error message for an error number
}

class Regex;

/** @class RegexMatch
 *  Performs regex matching.
 *  Provides chained methods to set various options.
 *
 *  All constructors of this class are private.
 */
class RegexMatch {

private:

	Regex* re;              ///< This is used to access private members in Regex.

	String m_subject;       ///< Subject string for match
	String m_modifier;      ///< Pattern for match
	Uint match_opts;        ///< PCRE2 options for pcre2_match() (PCRE2 internal function)
	Uint jpcre2_match_opts; ///< JPCRE2 options for match

	VecNum* vec_num;        ///< Pointer to vector that will store the numbered substring maps
	VecNas* vec_nas;        ///< Pointer to vector that will store the named substring maps
	VecNtN* vec_ntn;        ///< Pointer to vector that will store the name to number maps

	MapNum* num_map0;       ///< Pointer to map that will store numbered substrings temporarily
	MapNas* nas_map0;       ///< Pointer to map that will store named substrings temporarily
	MapNtN* ntn_map0;       ///< Pointer to map that will store name to number mapping temporarily

	/// Parse #m_modifier and set equivalent PCRE2 and JPCRE2 options.
	/// After a call to this function #match_opts and #jpcre2_match_opts will be properly set.
	void parseMatchOpts(void);

	void getNumberedSubstrings(int rc, pcre2_match_data *match_data);   ///< Populate #num_map0 with numbered substrings

	void getNamedSubstrings(int namecount, int name_entry_size,
			PCRE2_SPTR tabptr, pcre2_match_data *match_data);           ///< Populate #nas_map0 and/or #ntn_map0
                                                                        ///< with named substring and/or name to number mapping

    /// Initialize class variables
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

	/// Default constructor.
	/// Initialize class variables.
	RegexMatch() {
		init_vars();
	}

	/// This is a copy constructor which is only used to prevent public object creation.
	/// No need to implement it completely
	RegexMatch(const RegexMatch&) {
		init_vars();
	}

	/// Destructor.
	/// Deletes the temporary maps that were created to store substrings
	~RegexMatch() {
		delete num_map0;
		delete nas_map0;
		delete ntn_map0;
	}

	/// Define class Regex as friend and thus allow Regex to create object of this class
	friend class Regex;

public:

	/// Set a pointer to the numbered substring vector of type jpcre2::VecNum
	/// @param v #vec_num
	/// @return *this
	RegexMatch& setNumberedSubstringVector(VecNum* v) {
		vec_num = v;
		return *this;
	}

	/// Set a pointer to the named substring vector of type jpcre2::VecNas
	/// @param v #vec_nas
	/// @return *this
	RegexMatch& setNamedSubstringVector(VecNas* v) {
		vec_nas = v;
		return *this;
	}

	/// Set a pointer to the name to number map vector of type jpcre2::VecNtN
	/// @param v #vec_ntn
	/// @return *this
	RegexMatch& setNameToNumberMapVector(VecNtN* v) {
		vec_ntn = v;
		return *this;
	}

	/// Set the subject string #m_subject
	/// @param s Subject string
	/// @return *this
	RegexMatch& setSubject(const String& s) {
		m_subject = s;
		return *this;
	}

	/// Set the modifier #m_modifier (overwrites existing JPCRE2 and PCRE2 option).
	/// Re-initializes the option bits for PCRE2 and JPCRE2 options, then sets the modifier.
	/// @param s Modifier string
	/// @return *this
	RegexMatch& setModifier(const String& s) {
		match_opts = jpcre2_match_opts = 0;
		m_modifier = s;
		return *this;
	}

	/// Set JPCRE2 option #jpcre2_match_opts (overwrite existing option)
	/// @param x Option value
	/// @return *this
	RegexMatch& setJpcre2Option(Uint x) {
		jpcre2_match_opts = x;
		return *this;
	}

	/// Set PCRE2 option #match_opts (overwrite existing option)
	/// @param x Option value
	/// @return *this
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

	/// Add option to existing JPCRE2 options #jpcre2_match_opts
	/// @param x Option value
	/// @return *this
	RegexMatch& addJpcre2Option(Uint x) {
		jpcre2_match_opts |= x;
		return *this;
	}

	/// Add option to existing PCRE2 options #match_opts
	/// @param x Option value
	/// @return *this
	RegexMatch& addPcre2Option(Uint x) {
		match_opts |= x;
		return *this;
	}

	/// Remove option from existing JPCRE2 option #jpcre2_match_opts
	/// @param x Option value
	/// @return *this
	RegexMatch& removeJpcre2Option(Uint x) {
		jpcre2_match_opts &= ~x;
		return *this;
	}

	/// Remove option from existing PCRE2 option #match_opts
	/// @param x Option value
	/// @return *this
	RegexMatch& removePcre2Option(Uint x) {
		match_opts &= ~x;
		return *this;
	}

	/// Return the number of matches, store the match results in the specified vectors (#vec_num, #vec_nas, #vec_ntn)
	/**
	 * @return Number of matches found
	 * @see SIZE_T match(const String& s)
	 * @see SIZE_T match(const String& s, const String& mod)
	 * */
	SIZE_T match(void);
};

/** @class RegexReplace
 *  Performs regex replace on a string.
 *  Provides chained methods to set various options.
 *
 *  All constructors of this class are private.
 */
class RegexReplace {

private:

	Regex* re;                  ///< This is used to access private members in Regex.

	String r_subject;           ///< Subject string for replace
	String r_modifier;          ///< Modifier string for replace
	String r_replw;             ///< Replacement string i.e string to replace with
	Uint replace_opts;          ///< PCRE2 options for pcre2_substitute() (PCRE2 internal function)
	Uint jpcre2_replace_opts;   ///< JPCRE2 options
	PCRE2_SIZE buffer_size;     ///< Size of the resultant string after replacement

	/// Parse #r_modifier and set equivalent PCRE2 and JPCRE2 options.
	/// After a call to this function #replace_opts and #jpcre2_replace_opts will be properly set.
	void parseReplacementOpts(void);

	/// Initialize class variables
	void init_vars() {
		replace_opts = jpcre2_replace_opts = 0;
		buffer_size = SUBSTITUTE_RESULT_INIT_SIZE;
	}

	/// Default constructor.
	/// Initialize class variables
	RegexReplace() {
		init_vars();
	}

	/// This is a copy constructor which is only used to prevent public object creation.
	/// No need to implement it completely
	RegexReplace(const RegexReplace&) {
		init_vars();
	}

	/// Destructor.
	/// Nothing to be done here.
	~RegexReplace() {
	}

	/// Define Regex as a friend so that it can create object of this class.
	friend class Regex;

public:

	/** Set the subject string #r_subject
	 * @param s Subject string
	 * @return *this
	 * */
	RegexReplace& setSubject(const String& s) {
		r_subject = s;
		return *this;
	}

	/** Set the replacement string #r_replw
	 * @param s String to replace with
	 * @return *this
	 * */
	RegexReplace& setReplaceWith(const String& s) {
		r_replw = s;
		return *this;
	}

	/** Set the modifier string #r_modifier (overwrites existing JPCRE2 and PCRE2 option).
	 * @param s Modifier string
	 * @return *this
	 * */
	RegexReplace& setModifier(const String& s) {
		replace_opts = jpcre2_replace_opts = 0;
		r_modifier = s;
		return *this;
	}

	/** Set the initial buffer size (#buffer_size) to be allocated for replaced string (used by PCRE2)
	 * @param x Buffer size
	 * @return *this
	 * */
	RegexReplace& setBufferSize(PCRE2_SIZE x) {
		buffer_size = x;
		return *this;
	}

	/** Set JPCRE2 option #jpcre2_replace_opts (overwrite existing option)
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& setJpcre2Option(Uint x) {
		jpcre2_replace_opts = x;
		return *this;
	}

	/** Set PCRE2 option #replace_opts (overwrite existing option)
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& setPcre2Option(Uint x) {
		replace_opts = x;
		return *this;
	}

	/** Add specified JPCRE2 option to existing options #jpcre2_replace_opts
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& addJpcre2Option(Uint x) {
		jpcre2_replace_opts |= x;
		return *this;
	}

	/** Add specified PCRE2 option to existing options #replace_opts
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& addPcre2Option(Uint x) {
		replace_opts |= x;
		return *this;
	}

	/** Remove JPCRE2 option from existing options #jpcre2_replace_opts
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& removeJpcre2Option(Uint x) {
		jpcre2_replace_opts &= ~x;
		return *this;
	}

	/** Remove PCRE2 option from existing options #replace_opts
	 * @param x Option value
	 * @return *this
	 * */
	RegexReplace& removePcre2Option(Uint x) {
		replace_opts &= ~x;
		return *this;
	}

	/** Returns the resultant string after performing regex replace.
	 *@return Replaced string
	 * */
	String replace(void);
};

/** @class Regex
 * This class implements public overloaded and copy constructors to provide different means of creating objects and initializing class
 * variables.
 *
 * Each regex pattern needs an object of this class.
 *
 * A pattern must be compiled either by explicitly calling the compile function or using one of the parameterized constructors.
 *
 * */
class Regex {

private:

	RegexMatch *rm;		        ///< Pointer to RegexMatch object
	RegexReplace *rr;	        ///< Pointer to RegexReplace object

	String pat_str;		        ///< Pattern string
	String modifier;	        ///< Modifier string
	pcre2_code *code;	        ///< Pointer to compiled pattern
	int error_number;	        ///< Error number
	PCRE2_SIZE error_offset;	///< Error offset
	Uint compile_opts;          ///< Compile options for PCRE2 (used by PCRE2 internal function pcre2_compile())
	Uint jpcre2_compile_opts;	///< Compile options specific to JPCRE2
	String mylocale;	        ///< Locale as a string
	String current_warning_msg;	///< current warning message

	/// Initialize class variables
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

	/// Call Regex::init_vars() and initialize class variables.
	/// This function should not be attempted to call after creating object.
	/// To re-initialize class variables at a later stage after
	/// creating object, use the Regex::reset() function.
	/// This function is private and should remain as such.
	void init() {
		init_vars();
	}

	/// @overload
	/// @param re Regex pattern
	/// @param mod Modifier string
	void init(const String& re, const String& mod) {
		init_vars();
		pat_str = re;
		modifier = mod;
	}

	/// @overload
	/// @param re Regex pattern
	/// @param po PCRE2 options
	/// @param jo JPCRE2 options
	void init(const String& re, Uint po, Uint jo) {
		init_vars();
		pat_str = re;
		compile_opts = po;
		jpcre2_compile_opts = jo;
	}

	/// Free code if it's non-NULL
	void freeRegexMemory(void) {
		if (code)
			pcre2_code_free(code);
	}

	/// Parse #modifier and set equivalent PCRE2 and JPCRE2 options.
	/// After a call to this function #compile_opts and #jpcre2_compile_opts will be properly set.
	void parseCompileOpts(void);

	/// Define RegexMatch as friends. It needs to access the compiled pattern which is a private property of this class.
	friend class RegexMatch;
	/// Define RegexReplace as friends. It needs to access the compiled pattern which is a private property of this class.
	friend class RegexReplace;

	/// Do a shallow copy of class variables
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

	/// Do a deep copy of #rm, #rr and #code
	void deepCopy(const Regex& r);

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
	 *
	 *
	 *  Compiles pattern.
	 *  @param re Pattern string
	 *  @param mod Modifier string
	 * */
	Regex(const String& re, const String& mod) {
		compile(re, mod);
	}

	/** @overload
	 *
	 *
	 *  Compiles pattern.
	 *  @param re Pattern string
	 *  @param pcre2_opts PCRE2 option value
	 * */
	Regex(const String& re, Uint pcre2_opts) {
		compile(re, pcre2_opts, 0);
	}

	/** @overload
	 *
	 *
	 *  Compiles pattern.
	 *  @param re Pattern string
	 *  @param pcre2_opts	PCRE2 option value
	 *  @param opt_bits		JPCRE2 option value
	 * */
	Regex(const String& re, Uint pcre2_opts, Uint opt_bits) {
		compile(re, pcre2_opts, opt_bits);
	}

	/// @overload
	/// Copy constructor\. Compiles pattern and Performs a deep copy.
	/// @param r const Regex&
	Regex(const Regex& r) {
		///shallow copy must be performed **before** deep copy
		shallowCopy(r);
		deepCopy(r);
	}

	/// Overloaded assignment operator.
	/// Performs a deep copy.
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
		if (this == &r)
			return *this;
		///shallow copy must be performed before deep copy
		shallowCopy(r);
		deepCopy(r);
		return *this;
	}

	/// Destructor
	/// Deletes memory used by #rm an #rr
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

	/** Get error message by error number and error offset
	 *  @param err_num Error number
	 *  @param err_off Error offset
	 *  @return Error message as a string
	 * */
	String getErrorMessage(int err_num, PCRE2_SIZE err_off);

	/** @overload
	 *  Use class variable error_offest as error offset
	 *  @param err_num
	 *  @return Error message as a string
	 * */
	String getErrorMessage(int err_num) {
		return getErrorMessage(err_num, error_offset);
	}

	/** @overload
	 *  Use class variable error_number as error number and error_offest as error offset
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

	/// Get error number
	/// return #error_number
	int getErrorNumber() {
		return error_number;
	}

	/// Get error offset
	/// return #error_offset
	PCRE2_SIZE getErrorOffset() {
		return error_offset;
	}

	/// Set the Pattern string #pat_str
	/// @param re Pattern string
	/// @return *this
	Regex& setPattern(const String& re) {
		pat_str = re;
		return *this;
	}

	/// Set the modifier #modifier (overwrite existing JPCRE2 and PCRE2 option).
	/// Re-initializes the option bits for PCRE2 and JPCRE2 options, then sets the modifier.
	/// @param x Modifier string
	/// @return *this
	Regex& setModifier(const String& x) {
		compile_opts = jpcre2_compile_opts = 0;
		modifier = x;
		return *this;
	}

	/// Set the locale #mylocale.
	/// @param x Locale string
	/// @return *this
	Regex& setLocale(const String& x) {
		mylocale = x;
		return *this;
	}

	/// Set JPCRE2 option #jpcre2_compile_opts (overwrites existing option)
	/// @param x Option value
	/// @return *this
	Regex& setJpcre2Option(Uint x) {
		jpcre2_compile_opts = x;
		return *this;
	}

	/// Set PCRE2 option #compile_opts (overwrites existing option)
	/// @param x Option value
	/// @return *this
	Regex& setPcre2Option(Uint x) {
		compile_opts = x;
		return *this;
	}

	/// Add option to existing JPCRE2 options #jpcre2_compile_opts
	/// @param x Option value
	/// @return *this
	Regex& addJpcre2Option(Uint x) {
		jpcre2_compile_opts |= x;
		return *this;
	}

	/// Add option to existing PCRE2 options #compile_opts
	/// @param x Option value
	/// @return *this
	Regex& addPcre2Option(Uint x) {
		compile_opts |= x;
		return *this;
	}

	/// Remove option from existing JPCRE2 option #jpcre2_compile_opts
	/// @param x Option value
	/// @return *this
	Regex& removeJpcre2Option(Uint x) {
		jpcre2_compile_opts &= ~x;
		return *this;
	}

	/// Remove option from existing PCRE2 option #compile_opts
	/// @param x Option value
	/// @return *this
	Regex& removePcre2Option(Uint x) {
		compile_opts &= ~x;
		return *this;
	}

	/** Compile the regex pattern from class variable #pat_str.
	 * Use options from class variables.
	 *
	 * Prefer using one of its variants when compiling pattern for an already declared Regex object.
	 * An use of
	 * ```cppp
	 * re = Regex("pattern");
	 * ```
	 * (or such) is discouraged. see `Regex::operator=(const Regex& r)` for details.
	 * @see void compile(const String& re, Uint po, Uint jo)
	 * @see void compile(const String& re, Uint po)
	 * @see void compile(const String& re, const String& mod)
	 * @see void compile(const String& re)
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
		init(re, po, jo);
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
		init(re, po, 0);
		compile();
	}

	/** @overload
	 *
	 *
	 *  Set the specified parameters, then compile the pattern using options from class variables.
	 *  @param re Pattern string
	 *  @param mod Modifier string
	 * */
	void compile(const String& re, const String& mod) {
		init(re, mod);
		compile();
	}

	/** @overload
	 *
	 *
	 *  Set the specified parameters, then compile the pattern using options from class variables.
	 *  @param re Pattern string
	 * */
	void compile(const String& re) {
		init(re, 0, 0);
		compile();
	}

	/** Perform regex match.
	 *  This function takes the parameters, then sets the parameters to RegexMatch class and calls
	 *  RegexMatch::match() which returns the result
	 *  @param s Subject string
	 *  @param mod Modifier string
	 *  @return Match count
	 *  @see RegexMatch::match()
	 * */
	SIZE_T match(const String& s, const String& mod) {
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		rm->setSubject(s).setModifier(mod);
		return rm->match();
	}

	/** @overload
	 *  @param s Subject string
	 *  @return Match count
	 *  @see RegexMatch::match(const String& s)
	 * */
	SIZE_T match(const String& s) {
		delete rm;
		rm = new RegexMatch();
		rm->re = this;
		rm->setSubject(s);
		return rm->match();
	}

	/** Prepare to call RegexMatch::match().
	 * Other options can be set with the setter functions of RegexMatch class
	 * in-between the Regex::initMatch() and RegexMatch::match() call.
	 * @return RegexMatch object
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
	 *  This function takes the parameters, then sets the parameters to RegexReplace class and calls
	 *  RegexReplace::replace() which returns the result.
	 *  @param mains Subject string
	 *  @param repl String to replace with
	 *  @param mod Modifier string
	 *  @return Resultant string after regex replace
	 *  @see RegexReplace::replace()
	 * */
	String replace(const String& mains, const String& repl, const String& mod) {
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		rr->setSubject(mains).setReplaceWith(repl).setModifier(mod);
		return rr->replace();
	}

	/** @overload
	 *  @param mains Subject string
	 *  @param repl String to replace with
	 *  @return Resultant string after regex replace
	 *  @see RegexReplace::replace()
	 * */
	String replace(const String& mains, const String& repl) {
		delete rr;
		rr = new RegexReplace();
		rr->re = this;
		rr->setSubject(mains).setReplaceWith(repl);
		return rr->replace();
	}

	/** Prepare to call RegexReplace::replace().
	 * Other options can be set with the setter functions of RegexReplace class
	 * in-between the Regex::initReplace() and RegexReplace::replace() call.
	 * @return Resultant string after regex replace
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

}// jpcre2 namespace

#endif
