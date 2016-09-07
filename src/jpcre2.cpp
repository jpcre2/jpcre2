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

/**
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * */

#include "jpcre2.hpp"

#include <cstdio>   // snprintf
#include <limits>   // std::numeric_limits
#include <cassert>  // assert
#include <cstring>  // strlen


const jpcre2::String jpcre2::INFO::NAME("JPCRE2");
const jpcre2::String jpcre2::INFO::FULL_VERSION("10.25.02");
const jpcre2::String jpcre2::INFO::VERSION_GENRE("10");
const jpcre2::String jpcre2::INFO::VERSION_MAJOR("25");
const jpcre2::String jpcre2::INFO::VERSION_MINOR("02");
const jpcre2::String jpcre2::INFO::VERSION_PRE_RELEASE("");


/// Use max of int as the initial size of replaced string
const jpcre2::SIZE_T jpcre2::SUBSTITUTE_RESULT_INIT_SIZE = std::numeric_limits<int>::max();
const jpcre2::String jpcre2::LOCALE_NONE = "JPCRE2_NONE";                           ///< Nothing to be done on locale
const jpcre2::String jpcre2::LOCALE_DEFAULT = LOCALE_NONE;                          ///< Default local to be used
const jpcre2::String jpcre2::JIT_ERROR_MESSAGE_PREFIX = "JIT compilation failed! "; ///< Prefix to be added to JIT error message


// Define modifiers for compile
// Every modifier needs to be unique in this block
const jpcre2::String jpcre2::MOD::C_N("eijmnsuxADJU");
const jpcre2::Uint jpcre2::MOD::C_V[12] = { PCRE2_MATCH_UNSET_BACKREF,                  ///< Modifier e
                                            PCRE2_CASELESS,                             ///< Modifier i
                                            PCRE2_ALT_BSUX | PCRE2_MATCH_UNSET_BACKREF, ///< Modifier j
                                            PCRE2_MULTILINE,                            ///< Modifier m
                                            PCRE2_UTF | PCRE2_UCP,                      ///< Modifier n (includes u)
                                            PCRE2_DOTALL,                               ///< Modifier s
                                            PCRE2_UTF,                                  ///< Modifier u
                                            PCRE2_EXTENDED,                             ///< Modifier x
                                            PCRE2_ANCHORED,                             ///< Modifier A
                                            PCRE2_DOLLAR_ENDONLY,                       ///< Modifier D
                                            PCRE2_DUPNAMES,                             ///< Modifier J
                                            PCRE2_UNGREEDY                              ///< Modifier U
                                          };


const jpcre2::String jpcre2::MOD::CJ_N("S~&");
const jpcre2::Uint jpcre2::MOD::CJ_V[3] = { JIT_COMPILE,                                ///< Modifier S
                                            ERROR_ALL,                                  ///< Modifier ~
                                            VALIDATE_MODIFIER                           ///< Modifier &
                                          };



// Define modifiers for replace
// Every modifier needs to be unique in this block
const jpcre2::String jpcre2::MOD::R_N("eEgx");
const jpcre2::Uint jpcre2::MOD::R_V[4] = { PCRE2_SUBSTITUTE_UNSET_EMPTY,                ///< Modifier  e
                                           PCRE2_SUBSTITUTE_UNKNOWN_UNSET | PCRE2_SUBSTITUTE_UNSET_EMPTY,   ///< Modifier E (includes e)
                                           PCRE2_SUBSTITUTE_GLOBAL,                     ///< Modifier g
                                           PCRE2_SUBSTITUTE_EXTENDED                    ///< Modifier x
                                         };


const jpcre2::String jpcre2::MOD::RJ_N("~&");
const jpcre2::Uint jpcre2::MOD::RJ_V[2] = { ERROR_ALL,                                  ///< Modifier ~
                                            VALIDATE_MODIFIER                           ///< Modifier &
                                          };


// Define modifiers for match
// Every modifier needs to be unique in this block
const jpcre2::String jpcre2::MOD::M_N("A");
const jpcre2::Uint jpcre2::MOD::M_V[1] = { PCRE2_ANCHORED                               ///< Modifier  A
                                         };

const jpcre2::String jpcre2::MOD::MJ_N("g~&");
const jpcre2::Uint jpcre2::MOD::MJ_V[3] = { FIND_ALL,                                   ///< Modifier  g
                                            ERROR_ALL,                                  ///< Modifier  ~
                                            VALIDATE_MODIFIER                           ///< Modifier  &
                                          };

///////// utils namespace

/// @param a const char* to be converted to jpcre2::String
/// @return jpcre2::String
jpcre2::String jpcre2::utils::toString(const char* a) {
	if (a)
		return String(a);
	else
		return "";
}

/// @param a char to be converted to jpcre2::String
/// @return jpcre2::String
jpcre2::String jpcre2::utils::toString(char a) {
	if (a != '\0')
		return String(1, a);
	else
		return "";
}

/// @param x Integer to be converted to jpcre2::String
/// @return jpcre2::String
jpcre2::String jpcre2::utils::toString(int x) {
	int length = snprintf(0, 0, "%d", x);
	assert(length >= 0);
	char* buf = new char[length + 1];
	snprintf(buf, length + 1, "%d", x);
	String str(buf);
	delete[] buf;
	return str;
}

/// @param a PCRE2_UCHAR* to be converted to jpcre2::String
/// @return jpcre2::String
jpcre2::String jpcre2::utils::toString(PCRE2_UCHAR* a) {
	if (a)
		return String((char*) a);
	else
		return "";
}

/// @param err_num PCRE2 error number
/// @return Error message as jpcre2::String
std::string jpcre2::utils::getPcre2ErrorMessage(int err_num) {
	PCRE2_UCHAR buffer[8048];
	pcre2_get_error_message(err_num, buffer, sizeof(buffer));
	return toString((PCRE2_UCHAR*) buffer);
}


/**
 * Return error message according to error number and error offset
 * @param err_num Error number
 * @param err_off Error offset
 * @return Error message as a string
 * */
std::string jpcre2::utils::getErrorMessage(int err_num, int err_off) {
	if (err_num == (int)ERROR::INVALID_MODIFIER) {
		return "Invalid Modifier: " + toString((char) err_off);
	} else if (err_num == (int)ERROR::JIT_COMPILE_FAILED) {
		return JIT_ERROR_MESSAGE_PREFIX
				+ getPcre2ErrorMessage((int) err_off);
	} else {
		return getPcre2ErrorMessage((int) err_num) + ". Error offset: "
				+ toString((int) err_off);
	}
}


/// Used throughout JPCRE2 to throw exceptions
///@throw jpcre2::Except Throws exception. 
///       Should be caught by reference. See jpcre2::Except for details.
///@param err_num Error number
///@param err_off Error offset
void jpcre2::utils::throwException(int err_num, int err_off){
    throw Except(getErrorMessage(err_num, err_off), err_num, err_off);
}

/////////

///////// Regex class


/**
 *  Calculate modifier string from #compile_opts and #jpcre2_compile_opts and return it
 *
 *  Note that, this only gives you the modifiers used for pattern compilation.
 *  There is no such function to get the action modifiers and neither there's any need for it as
 *  action modifiers are required to be passed anew with every action. On the other hand,
 *  you may set some modifiers to the Regex object and forget about it later or you may want
 *  to get the existing modifier and compile the regex again by modifying the existing ones.
 *
 *  Do remember that modifiers (or PCRE2 and JPCRE2 options) do not change or get initialized
 *  as long as you don't do that explicitly. Calling Regex::setModifier() will re-set them.
 *
 *  **Mixed or combined modifier**.
 *
 *  Some modifier may include other modifiers i.e they have the same meaning of some modifiers
 *  combined together. For example, the 'n' modifier includes the 'u' modifier and together they
 *  are equivalent to `PCRE2_UTF | PCRE2_UCP`. When you set a modifier like this, both options
 *  get set, and when you remove (`Regex::changeModifier())` the 'n', both will get removed
 *  @return Calculated modifier string
 * */
jpcre2::String jpcre2::Regex::getModifier(){
    //Calculate PCRE2 mod
    String temp("");
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


/// Copy compiled pattern to a new location, free the old memory and set the new pointer to #code
/// @throw jpcre2::Except Throws exception with PCRE2 error number and error offset.
/// @param r Regex&
void jpcre2::Regex::deepCopy(const Regex& r) {
	//Copy r.code if it is non-null
	if (r.code) {
        //First release memory of code if it is non-NULL
		freeRegexMemory();
		//Copy only if code is non-null
		code = pcre2_code_copy(r.code);
		//pcre2_code_copy doesn't copy JIT memory, JIT compilation is needed
		if ((jpcre2_compile_opts & JIT_COMPILE) != 0) {
			//Perform JIT compilation:
			int jit_ret = pcre2_jit_compile(code, PCRE2_JIT_COMPLETE);
			if (jit_ret != 0) {
				if ((jpcre2_compile_opts & ERROR_ALL) != 0) {
					utils::throwException((int)ERROR::JIT_COMPILE_FAILED,
                     (int)ERROR::JIT_COMPILE_FAILED);
				} else
					current_warning_msg = JIT_ERROR_MESSAGE_PREFIX
							+ utils::getPcre2ErrorMessage(jit_ret);
			}
		}
	} else
		code = 0;
	///copy rm
	///No need to copy it, just set it to NULL and delete r.rm
	delete rm;
	rm = 0;
	delete r.rm;
	///copy rr
	///No need to copy it, just set it to NULL and delete r.rr
	delete rr;
	rr = 0;
	delete r.rr;
}


/// After a call to this function #compile_opts and #jpcre2_compile_opts will be properly set.
/// This function does not initialize or re-initialize options.
/// If you want to set options from scratch, initialize them to 0 before calling this function.
///
/// **Note:** If speed of operation is very crucial, use Regex::changeJpcre2Option() and
/// Regex::changePcre2Option() with equivalent options. It will be faster that way.
/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER if jpcre2::VALIDATE_MODIFIER or
///        jpcre2::ERROR_ALL was set and a wrong modifier was encountered.
/// @param mod Modifier string
/// @param x Whether to add or remove option
/// @return Regex&
/// @see RegexMatch::changeModifier()
/// @see RegexReplace::changeModifier()
jpcre2::Regex& jpcre2::Regex::changeModifier(const String& mod, bool x) {
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
        //Modifier didn't match, invalid modifier error
        if((jpcre2_compile_opts & VALIDATE_MODIFIER) != 0 || (jpcre2_compile_opts & ERROR_ALL) != 0) {
            utils::throwException((int)ERROR::INVALID_MODIFIER, (int) mod[i]);
        }
        //If exception wasn't thrown, add to warning message
        current_warning_msg = utils::getErrorMessage((int)ERROR::INVALID_MODIFIER, mod[i]);
        
        endfor:;
	}
    return *this;
}


/**
 *
 * Use options from class variables.
 *
 * Prefer using one of its variants when compiling pattern for an already declared Regex object.
 * A use of
 * ```cppp
 * re = Regex("pattern");
 * ```
 * (or such) is discouraged. see `Regex::operator=(const Regex& r)` for details.
 * @throw jpcre2::Except Throws exception with PCRE2 error number and error offset.
 * @see void compile(const String& re, Uint po, Uint jo)
 * @see void compile(const String& re, Uint po)
 * @see void compile(const String& re, const String& mod)
 * @see void compile(const String& re)
 * */
void jpcre2::Regex::compile() {
	///Get c_str of pattern
	PCRE2_SPTR c_pattern = (PCRE2_SPTR) pat_str.c_str();
	int error_number;	        ///< Error number
	PCRE2_SIZE error_offset;	///< Error offset

	/*************************************************************************
	 * Now we are going to compile the regular expression pattern, and handle *
	 * any errors that are detected.                                          *
	 *************************************************************************/

	pcre2_compile_context *ccontext = pcre2_compile_context_create(0);

	if (mylocale != LOCALE_NONE) {
		String loc_old;
		loc_old = utils::toString(std::setlocale(LC_CTYPE, mylocale.c_str()));
		const unsigned char *tables = pcre2_maketables(0);
		pcre2_set_character_tables(ccontext, tables);
		std::setlocale(LC_CTYPE, loc_old.c_str());
	}

	code = pcre2_compile(c_pattern,     /* the pattern */
                PCRE2_ZERO_TERMINATED,  /* indicates pattern is zero-terminated */
                compile_opts,           /* default options */
                &error_number,          /* for error number */
                &error_offset,          /* for error offset */
                ccontext);              /* use compile context */

	if (code == 0) {
		/* Compilation failed */
		//must not free regex memory, the only function has that right is the destroyer.
		utils::throwException(error_number, error_number);
	} else if ((jpcre2_compile_opts & JIT_COMPILE) != 0) {
		///perform jit compilation:
		int jit_ret = pcre2_jit_compile(code, PCRE2_JIT_COMPLETE);
		if (jit_ret != 0) {
			if ((jpcre2_compile_opts & ERROR_ALL) != 0) {
				utils::throwException((int)ERROR::JIT_COMPILE_FAILED, jit_ret);
			} else
				current_warning_msg = "JIT compile failed: "
						+ utils::getPcre2ErrorMessage(jit_ret);
		}
	}
}
/////////

///////// RegexReplace class

/// After a call to this function #replace_opts and #jpcre2_replace_opts will be properly set.
/// This function does not initialize or re-initialize options.
/// If you want to set options from scratch, initialize them to their defaults before calling this function.
///
/// **Note:** If speed of operation is very crucial, use RegexReplace::changeJpcre2Option() and
/// RegexReplace::changePcre2Option() with equivalent options. It will be faster that way.
/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER if jpcre2::VALIDATE_MODIFIER or
///        jpcre2::ERROR_ALL was set and a wrong modifier was encountered.
/// @param mod Modifier string
/// @param x Whether to add or remove option
/// @return RegexReplace&
/// @see RegexMatch::changeModifier()
/// @see Regex::changeModifier()
jpcre2::RegexReplace& jpcre2::RegexReplace::changeModifier(const String& mod, bool x) {
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
        //Modifier didn't match, invalid modifier error
        if((jpcre2_replace_opts & VALIDATE_MODIFIER) != 0 || (jpcre2_replace_opts & ERROR_ALL) != 0) {
            utils::throwException((int)ERROR::INVALID_MODIFIER, (int) mod[i]);
        }
        //If exception wasn't thrown, add to warning message
        re->current_warning_msg = utils::getErrorMessage((int)ERROR::INVALID_MODIFIER, mod[i]);
        
        endfor:;
	}
    return *this;
}


/** Retrieves subject string, replacement string, modifier and other options from class variables.
/// @throw jpcre2::Except Throws exception with PCRE2 error number and error offset.
 * @return Replaced string
 * */
jpcre2::String jpcre2::RegexReplace::replace() {

	/// If code is null, there's no need to proceed any further
	if (re->code == 0)
		return r_subject;

	PCRE2_SPTR subject = (PCRE2_SPTR) r_subject.c_str();
	PCRE2_SIZE subject_length = strlen((char *) subject);
	PCRE2_SPTR replace = (PCRE2_SPTR) r_replw.c_str();
	PCRE2_SIZE replace_length = strlen((char *) replace);
	PCRE2_SIZE outlengthptr = (PCRE2_SIZE) buffer_size;
	int ret = 0;
	bool retry = true;
	PCRE2_UCHAR* output_buffer;
	output_buffer = (PCRE2_UCHAR*) malloc(outlengthptr * sizeof(PCRE2_UCHAR));

	///loop to retry this substitute process
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
			///Handle errors
			if ((replace_opts & PCRE2_SUBSTITUTE_OVERFLOW_LENGTH) != 0
					&& ret == (int) PCRE2_ERROR_NOMEMORY && retry) {
				retry = false;
				/// Second retry in case output buffer was not big enough
				outlengthptr++;  /// It was changed to required length
				output_buffer = (PCRE2_UCHAR*) realloc(output_buffer,
						outlengthptr * sizeof(PCRE2_UCHAR));
				/// Go and try to perform the substitute again
				continue;
			} else {
				::free(output_buffer);
				utils::throwException(ret, ret);
			}
		}
		///If everything's ok exit the loop
		break;
	}
	String result = utils::toString((char*) output_buffer);
	::free(output_buffer);
	return result;
}
/////////

///////// RegexMatch class


/// After a call to this function #match_opts and #jpcre2_match_opts will be properly set.
/// This function does not initialize or re-initialize options.
/// If you want to set options from scratch, initialize them to their default values before calling this function.
///
/// **Note:** If speed of operation is very crucial, use RegexMatch::changeJpcre2Option() and
/// RegexMatch::changePcre2Option() with equivalent options. It will be faster that way.
/// @throw jpcre2::Except Throws exception with ERROR::INVALID_MODIFIER if jpcre2::VALIDATE_MODIFIER or
///        jpcre2::ERROR_ALL was set and a wrong modifier was encountered.
/// @param mod Modifier string
/// @param x Whether to add or remove options
/// @return RegexMatch&
/// @see RegexReplace::changeModifier()
/// @see Regex::changeModifier()
jpcre2::RegexMatch& jpcre2::RegexMatch::changeModifier(const String& mod, bool x) {
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
        //Modifier didn't match, invalid modifier error
        if((jpcre2_match_opts & VALIDATE_MODIFIER) != 0 || (jpcre2_match_opts & ERROR_ALL) != 0) {
            utils::throwException((int)ERROR::INVALID_MODIFIER, (int) mod[i]);
        }
        //If exception wasn't thrown, add to warning message
        re->current_warning_msg = utils::getErrorMessage((int)ERROR::INVALID_MODIFIER, mod[i]);
        endfor:;
	}
    return *this;
}

/// @throw jpcre2::Except Throws exception with PCRE2 error number and error offset.
/// @param rc Return value from pcre2_match()
/// @param match_data pcre2 match data
void jpcre2::RegexMatch::getNumberedSubstrings(int rc,
		pcre2_match_data *match_data) {
	for (int i = 0; i < rc; i++) {
		String value;
		///If we use pcre2_substring_get_bynumber(),
		///we will have to deal with returned error codes and memory
		PCRE2_UCHAR **bufferptr;
		PCRE2_SIZE bufflen;
		pcre2_substring_length_bynumber(match_data, (Uint) i, &bufflen);
		bufferptr = (PCRE2_UCHAR**) malloc(bufflen * sizeof(PCRE2_UCHAR));
		int ret = pcre2_substring_get_bynumber(match_data, (Uint) i, bufferptr,
				&bufflen);
		if (ret < 0) {
			switch (ret) {
			case PCRE2_ERROR_NOMEMORY:
				utils::throwException(ret, ret);
			default:
				break;   ///Other errors should be ignored
			}
		}
		value = utils::toString((char*) *bufferptr);
		///pcre2_substring_free(*bufferptr);
		::free(bufferptr);                  ///must free memory
		if (num_map0)
			(*num_map0)[i] = value; //This null check is paranoid, this function shouldn't be called if this map is null
	}
}

/// @throw jpcre2::Except Throws exception with PCRE2 error number and error offset.
/// @param namecount Number of capture group name
/// @param name_entry_size Size of each entry in the name table.
/// @param tabptr Pointer to name table
/// @param match_data PCRE2 match data
void jpcre2::RegexMatch::getNamedSubstrings(int namecount, int name_entry_size,
		PCRE2_SPTR tabptr, pcre2_match_data *match_data) {

	for (int i = 0; i < namecount; i++) {
		String key, value, value1;

		int n = (tabptr[0] << 8) | tabptr[1];
		key = utils::toString((char*) (tabptr + 2));
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
				utils::throwException(ret, ret);
			default:
				break;   ///Other errors should be ignored
			}
		}
		value = utils::toString((char *) *bufferptr);

		///Let's get the value again, this time with number
		///We will match this value with the previous
		///If it matches, then we got the right one,
		///Otherwise the number is not valid for the corresponding name
		///we will skip this iteration, if that happens.
		///Don't use pcre2_substring_number_from_name() to get the number for the name (It's messy).
		::free(bufferptr);
		pcre2_substring_length_bynumber(match_data, (Uint) n, &bufflen);
		bufferptr = (PCRE2_UCHAR **) malloc(bufflen * sizeof(PCRE2_UCHAR));
		ret = pcre2_substring_get_bynumber(match_data, (Uint) n, bufferptr,
				&bufflen);
		if (ret < 0) {
			switch (ret) {
			case PCRE2_ERROR_NOMEMORY:
				utils::throwException(ret, ret);
			default:
				break;   ///Other errors should be ignored
			}
		}
		value1 = utils::toString((char *) *bufferptr);

		///pcre2_substring_free(*bufferptr);
		///must free memory, pcre2_substring_free() yields to segmentation fault in several cases ( try '(?<name>\d)?' )
		/// (may be a bug?)
		///Instead use free() to free the memory
		::free(bufferptr);                  ///must free memory
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
}

/** Clear the specified vectors (#vec_num, #vec_nas, #vec_ntn) and re-fill them with match results,
 * then return the match count.
 *
 * The size of each vectors should be equal to the match count.
 * The return value is guaranteed to give you the right match count.

 * @throw jpcre2::Except Throws exception with PCRE2 error number and error offset.
 * @return Number of matches found
 * @see SIZE_T Regex::match(const String& s)
 * @see SIZE_T Regex::match(const String& s, const String& mod)
 * */
jpcre2::SIZE_T jpcre2::RegexMatch::match() {

	/// If code is null, there's no need to proceed any further
	if (re->code == 0)
		return 0;

	PCRE2_SPTR subject = (PCRE2_SPTR) m_subject.c_str();
	PCRE2_SPTR name_table;
	int crlf_is_newline;
	int namecount;
	int name_entry_size;
	int rc;
	int utf8;
	SIZE_T count = 0;
	Uint option_bits;
	Uint newline;
	PCRE2_SIZE *ovector;
	SIZE_T subject_length;
	pcre2_match_data *match_data;
	subject_length = strlen((char *) subject);

	///Clear all vectors and initialize maps
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
		default:
			utils::throwException(rc, rc);
		}
		return count;
	}

	++count; //Increment the counter
	/* Match succeded. Get a pointer to the output vector, where string offsets are
	 stored. */
	ovector = pcre2_get_ovector_pointer(match_data);

	/*************************************************************************
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

	///Let's get the numbered substrings
	if (num_map0)
		getNumberedSubstrings(rc, match_data);

	/**************************************************************************
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

		///Let's get the named substrings
		if (nas_map0 || ntn_map0)
			getNamedSubstrings(namecount, name_entry_size, tabptr, match_data);

	}

	///populate vector
	pushMapsIntoVectors();

	/*************************************************************************
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
	utf8 = (option_bits & PCRE2_UTF) != 0;

	/* Now find the newline convention and see whether CRLF is a valid newline
	 sequence. */

	(void) pcre2_pattern_info(re->code, PCRE2_INFO_NEWLINE, &newline);
	crlf_is_newline = newline == PCRE2_NEWLINE_ANY
			|| newline == PCRE2_NEWLINE_CRLF
			|| newline == PCRE2_NEWLINE_ANYCRLF;

	/* Loop for second and subsequent matches */

	for (;;) {
		///must clear map before filling it with new values
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

		/* Run the next matching operation */

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
			else if (utf8) { /* advance a whole UTF-8 */
				while (ovector[1] < subject_length) {
					if ((subject[ovector[1]] & 0xc0) != 0x80)
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

		///Let's get the numbered substrings
		if (num_map0)
			getNumberedSubstrings(rc, match_data);

		if (namecount <= 0)
			; /*No named substrings*/
		else {
			PCRE2_SPTR tabptr = name_table;

			///Let's get the named substrings
			if (nas_map0 || ntn_map0)
				getNamedSubstrings(namecount, name_entry_size, tabptr,
						match_data);
		}

		///populate vector
		pushMapsIntoVectors();

	} /* End of loop to find second and subsequent matches */

	pcre2_match_data_free(match_data);
	/// Must not free pcre2_code* code. This function has no right to modify regex.
	return count;
}
/////////

