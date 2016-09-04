/*******************************************************************************
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
 * Dsclaimer:
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
 * @brief Main header file for JPCRE library to be #included by programs that call JPCRE2 functions.
 * It includes the pcre2.h header, therefore you shouldn't include pcre2.h
 * seperately in your program. Make sure to link pcre2 library when compiling.
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
#include <stdint.h> /// uint32_t
#include <cstddef>  ///< std::size_t
#include <string>  ///< std::string
#include <vector>   ///< std::vector 
#include <map>      ///< std::map

/** Main namespace that is to used to access its functionalities.
 * 
 */
namespace jpcre2{
    
    typedef std::size_t SIZE_T;                   ///< Used for match count and vector size.
    typedef uint32_t Uint;                        ///< Used for options (bitwise operation).
    typedef std::string String;                   ///< String.
    
    typedef std::map<String,String> MapNas;       ///< Map for Named substring.
    typedef std::map<SIZE_T,String> MapNum;       ///< Map for Numbered substrings.
    typedef std::map<String,SIZE_T> MapNtN;       ///< Substring name to Substring number map.
    typedef std::vector<MapNas> VecNas;           ///< Vector of matches with named substrings.
    typedef std::vector<MapNtN> VecNtN;           ///< Vector of substring name to Substring number map.
    typedef std::vector<MapNum> VecNum;           ///< Vector of matches with numbered substrings.
    
    /** namespace for errors.
     * 
     */
    namespace ERROR {
    /** ERROR codes.
     * JPCRE2 error codes are positive integers
     * PCRE2 error codes are negative integers
     * In case of errors, these error codes are thrown as exception
     */
        enum { 
                INVALID_MODIFIER                 = 2,   ///< Invalid modifer
                JIT_COMPILE_FAILED               = 3    ///< JIT compile failed
            };
    }
    
    extern const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE;    ///< Used by default to provide big enough buffer for replaced string
    
    extern const String LOCALE_NONE;                    ///< Don't do anything about locale if it is set to LOCALE_NONE
    extern const String LOCALE_DEFAULT;                 ///< Default locale
    extern const String JIT_ERROR_MESSAGE_PREFIX;       ///< Prefix to be added to JIT error message 
    
    /** Option bits.
     * 
     * */
    enum {  NONE                                = 0x0000000u, ///< Option 0 (zero)
            VALIDATE_MODIFIER                   = 0x0000001u, ///< throw INVALID_MODIFIER if any modifier is wrong
            FIND_ALL                            = 0x0000002u, ///< Find all during match (global match)
            JIT_COMPILE                         = 0x0000004u, ///< Prform JIT compilation during pattern compilation
            ERROR_ALL                           = 0x0000008u  ///< Treat warnings as error and throw exception
         }; ///< enums
    
    namespace utils{
        extern String toString(int a);  //!< Converts an integer to String
        extern String toString(char a);
        extern String toString(const char* a);
        extern String toString(PCRE2_UCHAR* a);
        extern String getPcre2ErrorMessage(int err_num);
    }
    
    class Regex;
    
    /** @class RegexMatch
     * class to perform regex matching
     */
    class RegexMatch{
        
        private: 
        
            Regex* re;    //We will use this to access private members in Regex
            
            String m_subject;
            String m_modifier;
            Uint match_opts;
            Uint jpcre2_match_opts;
            
            //vectors to contain the matches and maps of associated substrings
            VecNum* vec_num;
            VecNas* vec_nas;
            VecNtN* vec_ntn;
            
            //Maps to contain the captured groups, make sure to delete these in destructor
            MapNum* num_map0;
            MapNas* nas_map0;   
            MapNtN* ntn_map0;   
            
            
            void parseMatchOpts(void);
            void getNumberedSubstrings(int rc, pcre2_match_data *match_data);
            void getNamedSubstrings(int namecount,int name_entry_size,PCRE2_SPTR tabptr, pcre2_match_data *match_data);
            
                                            
            void init_vars()                                            { vec_num=0;
                                                                          vec_nas=0;
                                                                          vec_ntn=0;
                                                                          num_map0=0;
                                                                          nas_map0=0;
                                                                          ntn_map0=0;
                                                                          match_opts=0;
                                                                          jpcre2_match_opts=0; 
                                                                        }
                            
            RegexMatch()                                                { init_vars(); }
            RegexMatch(const RegexMatch&)                               { init_vars(); }    /// No need to implement copy constructor
            
            ~RegexMatch()                                               { ///Delete map pointers
                                                                          delete num_map0;
                                                                          delete nas_map0;
                                                                          delete ntn_map0;
                                                                        }
            
            
            ///define buddies for RegexMatch
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexMatch& setNumberedSubstringVector(VecNum* v)           { vec_num=v;                       return *this;   }
            RegexMatch& setNamedSubstringVector(VecNas* v)              { vec_nas=v;                       return *this;   }
            RegexMatch& setNameToNumberMapVector(VecNtN* v)             { vec_ntn=v;                       return *this;   }
            RegexMatch& setSubject(const String& s)                     { m_subject=s;                     return *this;   }
            RegexMatch& setModifier(const String& s)                    { match_opts = jpcre2_match_opts = 0;
                                                                          m_modifier = s;                   
                                                                          return *this; 
                                                                        }
            RegexMatch& setJpcre2Option(Uint x)                         { jpcre2_match_opts = x;           return *this;   }
            RegexMatch& setPcre2Option(Uint x)                          { match_opts = x;                  return *this;   }
            RegexMatch& setFindAll(bool x=true)                         { if(x) jpcre2_match_opts |= FIND_ALL;
                                                                          else  jpcre2_match_opts &= ~FIND_ALL;
                                                                          return *this;                                 
                                                                        }
            
            RegexMatch& addJpcre2Option(Uint x)                         { jpcre2_match_opts |= x;          return *this;   }
            RegexMatch& addPcre2Option(Uint x)                          { match_opts |= x;                 return *this;   }
            
            RegexMatch& removeJpcre2Option(Uint x)                      { jpcre2_match_opts &= ~x;         return *this;   }
            RegexMatch& removePcre2Option(Uint x)                       { match_opts &= ~x;                return *this;   }
                                                                                     
            ///returns the number of matches, stores the match results in the specified vectors
            SIZE_T match(const String& s, const String& mod)            { m_subject = s; m_modifier = mod; return match(); }
            SIZE_T match(const String& s)                               { m_subject = s;                   return match(); }
            SIZE_T match(void);
    };
    
    
    class RegexReplace{
        
        private: 
        
            Regex* re;    //We will use this to access private members in Regex
            
            String r_subject;
            String r_modifier;
            String r_replw;
            Uint replace_opts;
            Uint jpcre2_replace_opts;
            PCRE2_SIZE buffer_size;
            
            void parseReplacementOpts(void);
            
                                            
            void init_vars()                                            { replace_opts = jpcre2_replace_opts = 0;
                                                                          buffer_size=SUBSTITUTE_RESULT_INIT_SIZE; 
                                                                        }
                            
            RegexReplace()                                              { init_vars(); }
            RegexReplace(const RegexReplace& r)                         { init_vars(); }
            
            ~RegexReplace(){}
            
            ///define buddies for RegexReplace
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexReplace& setSubject(const String& s)                   { r_subject=s;                   return *this; }
            RegexReplace& setReplaceWith(const String& s)               { r_replw=s;                     return *this; }
            RegexReplace& setModifier(const String& s)                  { replace_opts = jpcre2_replace_opts = 0;
                                                                          r_modifier=s;                  return *this;
                                                                        }
            RegexReplace& setBufferSize(PCRE2_SIZE x)                   { buffer_size=x;                 return *this; }
            RegexReplace& setJpcre2Option(Uint x)                       { jpcre2_replace_opts = x;       return *this; }
            RegexReplace& setPcre2Option(Uint x)                        { replace_opts = x;              return *this; }
            
            RegexReplace& addJpcre2Option(Uint x)                       { jpcre2_replace_opts |= x;      return *this; }
            RegexReplace& addPcre2Option(Uint x)                        { replace_opts |= x;             return *this; }
            
            RegexReplace& removeJpcre2Option(Uint x)                    { jpcre2_replace_opts &= ~x;     return *this; }
            RegexReplace& removePcre2Option(Uint x)                     { replace_opts &= ~x;            return *this; }
            
            ///returns a replaced string after performing regex replace
            String replace(const String& s,const String& r,const String& m) 
                                                                        { r_subject=s; r_replw=r; r_modifier=m; return replace(); }
            String replace(const String& s,const String& r)             { r_subject=s; r_replw=r;               return replace(); }
            String replace(const String& s)                             { r_subject=s;                          return replace(); }
            String replace(void);
    };
    
    
    class Regex{
        
        private:
        
            RegexMatch *rm;
            RegexReplace *rr;
            
            String pat_str;
            String modifier;
            pcre2_code *code;
            int error_number;
            PCRE2_SIZE error_offset;
            Uint compile_opts,jpcre2_compile_opts;
            String mylocale;
            String current_warning_msg;
            
            void init_vars()                                            { mylocale = LOCALE_DEFAULT;
                                                                          jpcre2_compile_opts = 0;
                                                                          error_number = 0;
                                                                          error_offset = 0;
                                                                          compile_opts = 0;
                                                                          code = 0;
                                                                          rr = 0;
                                                                          rm = 0;
                                                                        }
                                                                        
            void init()                                                 { init_vars(); }
        
            void init(const String& re, const String& mod)              { init_vars();
                                                                          pat_str=re;
                                                                          modifier=mod;
                                                                        }
                                                                        
            void init(const String& re, Uint pcre2_opts, Uint opt_bits) { init_vars();
                                                                          pat_str=re;
                                                                          compile_opts = pcre2_opts;
                                                                          jpcre2_compile_opts = opt_bits;
                                                                        }
            
            void freeRegexMemory(void)                                  { if(code) pcre2_code_free(code); }
            
            void parseCompileOpts(void);
                            
            ///Define buddies for Regex
            friend class RegexMatch;
            friend class RegexReplace;
            
            void shallowCopy(const Regex& r)                            { pat_str = r.pat_str;
                                                                          mylocale = r.mylocale;
                                                                          modifier = r.modifier;
                                                                          error_number = r.error_number;
                                                                          error_offset = r.error_offset;
                                                                          compile_opts = r.compile_opts;
                                                                          jpcre2_compile_opts = r.jpcre2_compile_opts;
                                                                          current_warning_msg = r.current_warning_msg;
                                                                        }
            void deepCopy(const Regex& r);
            
        public:
        
            
            ///Constructors
            
            Regex(const String& re, Uint pcre2_opts, Uint opt_bits)     { compile(re, pcre2_opts, opt_bits); }
            Regex(const String& re, Uint pcre2_opts)                    { compile(re, pcre2_opts, 0);        }
            Regex(const String& re, const String& mod)                  { compile(re, mod);                  }
            Regex(const String& re)                                     { compile(re, 0, 0);                 }
            Regex()                                                     { init();                            }
            
            ///Deep copy constructor
            Regex(const Regex& r)                                       { ///shallow copy must be performed **before** deep copy
                                                                          shallowCopy(r);
                                                                          deepCopy(r);
                                                                        }
            Regex& operator=(const Regex& r)                            { if(this == &r) return *this;
                                                                          ///shallow copy must be performed before deep copy
                                                                          shallowCopy(r);
                                                                          deepCopy(r);
                                                                          return *this;
                                                                        }
            
            ~Regex()                                                    { freeRegexMemory();
                                                                          delete rm; 
                                                                          delete rr; 
                                                                        }
            Regex& reset()                                              { freeRegexMemory(); init(); return *this; }
            
            
            String      getModifier()                                   { return modifier;            }
            String      getPattern()                                    { return pat_str;             }
            String      getLocale()                                     { return mylocale;            } 
            Uint        getPcre2Option()                                { return compile_opts;        } 
            Uint        getJpcre2Option()                               { return jpcre2_compile_opts; }
            
            
            ///Error handling
            String      getErrorMessage(int err_num, PCRE2_SIZE err_off);
            
            String      getErrorMessage(int err_num)                    { return getErrorMessage(err_num, error_offset);           }
            String      getErrorMessage()                               { return getErrorMessage(error_number, error_offset);      }
            String      getWarningMessage()                             { return current_warning_msg;                              }
            int         getErrorNumber()                                { return error_number;                                     }
            PCRE2_SIZE  getErrorOffset()                                { return error_offset;                                     }
            
            
            
            Regex&      setPattern(const String& re)                    { pat_str=re;                return *this; }
            Regex&      setModifier(const String& x)                    { compile_opts = jpcre2_compile_opts = 0;
                                                                          modifier = x;
                                                                          return *this;
                                                                        }
            Regex&      setLocale(const String& x)                      { mylocale=x;                return *this; }
            Regex&      setJpcre2Option(Uint x)                         { jpcre2_compile_opts = x;   return *this; }
            Regex&      setPcre2Option(Uint x)                          { compile_opts = x;          return *this; }
            
            Regex&      addJpcre2Option(Uint x)                         { jpcre2_compile_opts |= x;  return *this; }
            Regex&      addPcre2Option(Uint x)                          { compile_opts |= x;         return *this; }
            
            Regex&      removeJpcre2Option(Uint x)                      { jpcre2_compile_opts &= ~x; return *this; }
            Regex&      removePcre2Option(Uint x)                       { compile_opts &= ~x;        return *this; }
            
            ///Compiles the regex.
            void        compile(void);
            void        compile(const String& re, Uint po, Uint jo)     { init(re, po, jo);          compile();    }
            void        compile(const String& re, Uint po)              { init(re, po, 0);           compile();    }
            void        compile(const String& re, const String& mod)    { init(re, mod);             compile();    }
            void        compile(const String& re)                       { init(re, 0, 0);            compile();    }
            
            
            ///This is the match() function that will be called by users
            SIZE_T match(const String& s, const String& mod)            { delete rm;
                                                                          rm = new RegexMatch(); 
                                                                          rm->re = this; 
                                                                          return rm->match(s, mod); 
                                                                        }
            SIZE_T match(const String& s)                               { delete rm;
                                                                          rm = new RegexMatch(); 
                                                                          rm->re = this; 
                                                                          return rm->match(s); 
                                                                        }
                                                                        
            RegexMatch& initMatch()                                     { delete rm;
                                                                          rm = new RegexMatch();
                                                                          rm->re = this; 
                                                                          return *rm; 
                                                                        }
                
            ///This is the replace function that will be called by users
            String replace(const String& mains, const String& repl, const String& mod) 
                                                                        { delete rr;
                                                                          rr = new RegexReplace();
                                                                          rr->re = this;            
                                                                          return rr->replace(mains,repl, mod); 
                                                                        }
            String replace(const String& mains,const String& repl)
                                                                        { delete rr;
                                                                          rr = new RegexReplace();
                                                                          rr->re = this;            
                                                                          return rr->replace(mains,repl); 
                                                                        }
            String replace(const String& mains)                         { delete rr;
                                                                          rr = new RegexReplace();
                                                                          rr->re = this;            
                                                                          return rr->replace(mains); 
                                                                        }
            RegexReplace& initReplace()                                 { delete rr;
                                                                          rr = new RegexReplace();
                                                                          rr->re = this;
                                                                          return *rr; 
                                                                        }
            
    };

    
} ///jpcre2 namespace

#endif
