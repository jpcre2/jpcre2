/**********************************************************************
 * ******************* C++ wrapper of PCRE2 Library *******************
 * ********************************************************************/

/* 
This is a public C++ wrapper for several utilities of the PCRE library, second API, to be
#included by applications that call PCRE2 functions.

           Copyright (c) 2015-2016 Md. Jahidul Hamid

-----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * The names of its contributors may be used to endorse or promote 
      products derived from this software without specific prior written
      permission.
      
Dsclaimer:

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
    
*/



#ifndef JPCRE2_HPP
#define JPCRE2_HPP

///You should define PCRE2_CODE_UNIT_WIDTH before including this header (jpcre2).
#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8 /* default code unit width */
#endif

#include <pcre2.h>



#ifdef __cplusplus

#include <limits>
#include <string>
#include <cstring>
#include <vector>
#include <map>


namespace jpcre2{
    
    typedef size_t SIZE_T;
    typedef std::string String;
    
    typedef std::map<String,String> MapNas;       //Map for Named substring
    typedef std::map<SIZE_T,String> MapNum;       //Map for Numbered substring
    typedef std::map<String,SIZE_T> MapNtN;       //Map for Named substring to Numbered substring
    typedef std::vector<MapNas> VecNas;           //Vector of MapNas
    typedef std::vector<MapNtN> VecNtN;           //Vector of MapNtN
    typedef std::vector<MapNum> VecNum;           //Vector of MapNum
    
    ///Errors // JPCRE2 error codes are positive numbers while PCRE2 error codes are negative numbers
    namespace ERROR {
        enum { 
                INVALID_MODIFIER                 = 2,
                JIT_COMPILE_FAILED               = 3
            };
    }
    
    extern const SIZE_T SUBSTITUTE_RESULT_INIT_SIZE; //Used by default to provide big enough buffer for replaced string
    
    extern const String LOCALE_NONE;    ///We won't do anything about locale if it is set to LOCALE_NONE
    extern const String LOCALE_DEFAULT; ///Default locale
    
    ///Option bits. These are the options for JPCRE2.
    enum {  NONE                                = 0x0000000u,
            VALIDATE_MODIFIER                   = 0x0000001u,
            FIND_ALL                            = 0x0000002u, //find all during match
            JIT_COMPILE                         = 0x0000004u, //prform JIT compilation during pattern compilation
            ERROR_ALL                           = 0x0000008u  //treat warnings as error and throw exception
         };
    
    
    ///declare classes
    class Regex;
    class RegexMatch;
    class RegexReplace;
    
    
    ///define classes
    
    class RegexMatch{
        
        private: 
        
            Regex* re;    ///We will use this to access private members in Regex
            
            String m_subject;
            String m_modifier;
            uint32_t match_opts;
            uint32_t jpcre2_match_opts;
            
            ///vectors to contain the matches and maps of associated substrings
            VecNum* vec_num;
            VecNas* vec_nas;
            VecNtN* vec_ntn;
            
            ///Maps to contain the captured groups, make sure to delete these in destructor
            MapNum* num_map0;
            MapNas* nas_map0;
            MapNtN* ntn_map0;
            
            
            void parseMatchOpts(void);
            void getNumberedSubstrings(int rc, pcre2_match_data *match_data);
            void getNamedSubstrings(int namecount,int name_entry_size,PCRE2_SPTR tabptr, pcre2_match_data *match_data);
                                                                                     
            ///returns the number of matches, stores the match results in the specified vectors
            SIZE_T match(void);
                                            
            void init_vars()
                                {
                                    vec_num=NULL;
                                    vec_nas=NULL;
                                    vec_ntn=NULL;
                                    num_map0=NULL;
                                    nas_map0=NULL;
                                    ntn_map0=NULL;
                                    match_opts=jpcre2_match_opts=0;
                                }
            void init(const String& s="")
                                {
                                    init_vars();
                                    m_subject=s;
                                }
            void init(const String& s, const String& mod)
                                {
                                    init_vars();
                                    m_subject=s;
                                    m_modifier=mod;
                                }
                            
            RegexMatch()                                    { init();      }
            RegexMatch(const RegexMatch&)                   { init();      }    /// No need to implement copy constructor
            RegexMatch(const String& s)                     { init(s);     }
            RegexMatch(const String& s, const String& mod)  { init(s,mod); }
            
            ~RegexMatch()
              {
                ///Delete map pointers
                if(num_map0) delete num_map0;
                if(nas_map0) delete nas_map0;
                if(ntn_map0) delete ntn_map0;
              }
            
            
            ///define buddies for RegexMatch
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexMatch& setNumberedSubstringVector(VecNum* v)             { vec_num=v;                      return *this; }
            RegexMatch& setNamedSubstringVector(VecNas* v)                { vec_nas=v;                      return *this; }
            RegexMatch& setNameToNumberMapVector(VecNtN* v)               { vec_ntn=v;                      return *this; }
            RegexMatch& setSubject(const String& s)                       { m_subject=s;                    return *this; }
            RegexMatch& setModifiers(const String& s)                     { m_modifier=s;                   return *this; }
            RegexMatch& setJpcre2Options(uint32_t x)                      { jpcre2_match_opts = x;          return *this; }
            RegexMatch& setPcre2Options(uint32_t x)                       { match_opts = x;                 return *this; }
            RegexMatch& setFindAll(bool x=true)                           { if(x) jpcre2_match_opts |= FIND_ALL;
                                                                            else  jpcre2_match_opts &= ~FIND_ALL;
                                                                            return *this;                                 }
            
            RegexMatch& addJpcre2Options(uint32_t x)                      { jpcre2_match_opts |= x;         return *this; }
            RegexMatch& addPcre2Options(uint32_t x)                       { match_opts |= x;                return *this; }
            
            RegexMatch& removeJpcre2Options(uint32_t x)                   { jpcre2_match_opts &= ~x;        return *this; }
            RegexMatch& removePcre2Options(uint32_t x)                    { match_opts &= ~x;               return *this; }
            
            SIZE_T exec()       { return execute(); }
            SIZE_T execute()    { return match();   }
    };
    
    
    class RegexReplace{
        
        private: 
        
            Regex* re;    ///We will use this to access private members in Regex
            
            String r_subject;
            String r_modifier;
            String r_replw;
            uint32_t replace_opts;
            uint32_t jpcre2_replace_opts;
            PCRE2_SIZE buffer_size;
            
            void parseReplacementOpts(void);
            
            ///returns a replaced string after performing regex replace
            String replace(void);
                                            
            void init_vars(){ 
                              replace_opts = jpcre2_replace_opts=0;
                              buffer_size=SUBSTITUTE_RESULT_INIT_SIZE;
                            }
            void init(const String& s="")                                       { init_vars(); r_subject=s;                               }
            void init(const String& s,const String& repl)                       { init_vars(); r_subject=s; r_replw=repl;                 }
            void init(const String& s,const String& repl, const String& mod)    { init_vars(); r_subject=s; r_replw=repl; r_modifier=mod; }
                            
            RegexReplace(const String& s,const String& repl, const String& mod) { init(s, repl, mod); }
            RegexReplace(const String& s,const String& repl)                    { init(s, repl);      }
            RegexReplace(const RegexReplace&)                                   { init();             }    /// No need to implement copy constructor
            RegexReplace(const String& s)                                       { init(s);            }
            RegexReplace()                                                      { init();             }
            
            ~RegexReplace(){}
            
            ///define buddies for RegexReplace
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexReplace& setSubject(const String& s)                        { r_subject=s;                   return *this; }
            RegexReplace& setReplaceWith(const String& s)                    { r_replw=s;                     return *this; }
            RegexReplace& setModifiers(const String& s)                      { r_modifier=s;                  return *this; }
            RegexReplace& setBufferSize(PCRE2_SIZE x)                        { buffer_size=x;                 return *this; }
            RegexReplace& setJpcre2Options(uint32_t x)                       { jpcre2_replace_opts = x;       return *this; }
            RegexReplace& setPcre2Options(uint32_t x)                        { replace_opts = x;              return *this; }
            
            RegexReplace& addJpcre2Options(uint32_t x)                       { jpcre2_replace_opts |= x;      return *this; }
            RegexReplace& addPcre2Options(uint32_t x)                        { replace_opts |= x;             return *this; }
            
            RegexReplace& removeJpcre2Options(uint32_t x)                    { jpcre2_replace_opts &= ~x;     return *this; }
            RegexReplace& removePcre2Options(uint32_t x)                     { replace_opts &= ~x;            return *this; }
            
            String exec()       { return execute(); }
            String execute()    { return replace(); }
    };
    
    
    class Regex{
        
        private:
        
            RegexMatch rm;
            RegexReplace rr;
            
            String pat_str;
            String modifier;
            pcre2_code *code;
            int error_number;
            PCRE2_SIZE error_offset;
            uint32_t compile_opts,jpcre2_compile_opts;
            String mylocale;
            String current_warning_msg;
            
            void freeRegexMemory(void)  { if(code) pcre2_code_free(code); } ///frees memory used for the compiled regex.
            
            void parseCompileOpts(void);
            
            void init_vars(){ mylocale = LOCALE_DEFAULT;
                              error_number = error_offset = 0;
                              compile_opts = jpcre2_compile_opts = 0;
                              code = NULL;
                            }
                              
            void init(const String& re="") 
                                            { init_vars();
                                              pat_str=re;
                                            }
            void init(const String& re, const String& mod)
                                            { init_vars();
                                              pat_str=re;modifier=mod;
                                            }
            void init(const String& re, uint32_t pcre2_opts, uint32_t opt_bits=0) 
                                            { init_vars();
                                              pat_str=re;
                                              compile_opts |= pcre2_opts;
                                              jpcre2_compile_opts |= opt_bits;
                                            }
            
            ///Compiles the regex.
            void compileRegex(void);
                            
            ///Define buddies for Regex
            friend class RegexMatch;
            friend class RegexReplace;
            
            void shallowCopy(const Regex& r){
                rm = r.rm;
                rr = r.rr;
                pat_str = r.pat_str;
                mylocale = r.mylocale;
                modifier = r.modifier;
                error_number = r.error_number;
                error_offset = r.error_offset;
                compile_opts = r.compile_opts;
                jpcre2_compile_opts = r.jpcre2_compile_opts;
                current_warning_msg = r.current_warning_msg;
            }
            void deepCopy(const Regex& r){
                ///Now copy r.code if it is non-null
                if(r.code){
                    freeRegexMemory();  ///first release memory if it is non-NULL
                    ///copy only if code is non-null
                    code = pcre2_code_copy(r.code);
                    ///pcre2_code_copy doesn't copy JIT memory
                    ///JIT compilation is needed
                    if((jpcre2_compile_opts & JIT_COMPILE) != 0){
                    ///perform jit compilation:
                        int jit_ret=pcre2_jit_compile(code, PCRE2_JIT_COMPLETE);
                        if(jit_ret!=0){
                            if((jpcre2_compile_opts & ERROR_ALL) != 0) {
                                error_number = error_offset = ERROR::JIT_COMPILE_FAILED;
                                throw((int)ERROR::JIT_COMPILE_FAILED);
                            }
                            else current_warning_msg="JIT compile failed: "+utils::getPcre2ErrorMessage(jit_ret);
                        }  
                    }
                }
                else code = NULL;
            }
            
        public:
            Regex(const String& re, uint32_t pcre2_opts, uint32_t opt_bits=0)  { init(re, pcre2_opts, opt_bits);execute();   }
            Regex(const String& re, const String& mod="")                      { init(re,mod);execute();                     }
            Regex()                                                            { init();                                     }
            
            ///Deep copy constructor
            Regex(const Regex& r){
                ///shallow copy must be performed before deep copy
                shallowCopy(r);
                deepCopy(r);
            }
            Regex& operator=(const Regex& r){
                if(this == &r) return *this;
                ///shallow copy must be performed before deep copy
                shallowCopy(r);
                deepCopy(r);
                return *this;
            }
            
            ~Regex()    { freeRegexMemory(); }
            
                
            String      getModifier()         { return modifier;            }
            String      getPattern()          { return pat_str;             }
            String      getLocale()           { return mylocale;            }      ///Gets LC_CTYPE
            uint32_t    getPcre2Options()     { return compile_opts;        }      ///returns the compile opts used for compilation
            uint32_t    getJpcre2Options()    { return jpcre2_compile_opts; }
            
            
            ///Error handling
            String      getErrorMessage(int err_num, PCRE2_SIZE err_off);
            
            String      getErrorMessage(int err_num)    { return getErrorMessage(err_num, error_offset);           }
            String      getErrorMessage()               { return getErrorMessage(error_number, error_offset);      }
            String      getWarningMessage()             { return current_warning_msg;                              }
            int         getErrorNumber()                { return error_number;                                     }
            PCRE2_SIZE  getErrorOffset()                { return error_offset;                                     }
            
            
            Regex&      compile(const String& re, uint32_t po, uint32_t jo=0){ init(re, po, jo);     return *this; }
            Regex&      compile(const String& re,const String& mod)     { init(re, mod);             return *this; }
            Regex&      compile(const String& re="")                    { init(re);                  return *this; }
            
            Regex&      setPattern(const String& re)                    { pat_str=re;                return *this; }
            Regex&      setModifiers(const String& x)                   { modifier=x;                return *this; }
            Regex&      setLocale(const String& x)                      { mylocale=x;                return *this; }
            Regex&      setJpcre2Options(uint32_t x)                    { jpcre2_compile_opts = x;   return *this; }
            Regex&      setPcre2Options(uint32_t x)                     { compile_opts = x;          return *this; }
            
            Regex&      addJpcre2Options(uint32_t x)                    { jpcre2_compile_opts |= x;  return *this; }
            Regex&      addPcre2Options(uint32_t x)                     { compile_opts |= x;         return *this; }
            
            Regex&      removeJpcre2Options(uint32_t x)                 { jpcre2_compile_opts &= ~x; return *this; }
            Regex&      removePcre2Options(uint32_t x)                  { compile_opts &= ~x;        return *this; }
            
            void        execute()   {compileRegex();}
            void        exec()      {execute();     }
            
            
            ///This is the match() function that will be called by users
            RegexMatch& match(const String& s, const String& mod)   { rm=RegexMatch(s, mod);rm.re = this;return rm; }
            RegexMatch& match(const String& s="")                   { rm=RegexMatch(s);rm.re = this;return rm;      }
            
            ///This is the replace function that will be called by users
            RegexReplace& replace(const String& mains,const String& repl, const String& mod) { rr=RegexReplace(mains,repl, mod);rr.re=this;return rr; }
            RegexReplace& replace(const String& mains,const String& repl)                    { rr=RegexReplace(mains,repl);rr.re=this;return rr;      }
            RegexReplace& replace(const String& mains="")                                    { rr=RegexReplace(mains);rr.re=this;return rr;           }
            
    };
    
    namespace utils{
        String toString(int a);
        String toString(char a);
        String toString(const char* a);
        String toString(PCRE2_UCHAR* a);
        String getPcre2ErrorMessage(int err_num);
            
    }

} ///jpcre2 namespace




#endif
#endif
