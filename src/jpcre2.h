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
    
    typedef size_t Uint;
    typedef size_t SIZE_T;
    typedef unsigned short Ush;
    typedef std::string String;
    
    typedef std::map<String,String> MapNas;       //Map for Named substring
    typedef std::map<Uint,String> MapNum;         //Map for Numbered substring
    typedef std::map<String,Uint> MapNtN;         //Map for Named substring to Numbered substring
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
    
    extern const size_t REGEX_STRING_MAX; //Used by default to provide big enough buffer for replaced string
    
    extern const String DEFAULT_LOCALE;   ///We won't do anything about locale if it is set to "none" 
    
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
            
            String m_subject,m_modifier;
            uint32_t match_opts,jpcre2_match_opts;
            
            ///vectors to contain the matches and maps of associated substrings
            VecNum* p_vec_num;
            VecNas* p_vec_nas;
            VecNtN* p_vec_ntn;
            
            ///Maps to contain the captured groups
            MapNum* num_map0;
            MapNas* nas_map0;
            MapNtN* ntn_map0;
            
            
            void parseMatchOpts(const String& mod, uint32_t opt_bits, uint32_t pcre2_opts);
            void getNumberedSubstrings(int rc, pcre2_match_data *match_data,MapNum* num_map0);
            void getNamedSubstrings(int namecount,int name_entry_size,PCRE2_SPTR tabptr, pcre2_match_data *match_data,
                                                                                     MapNas* nas_map0, MapNtN* nn_map0);
                                                                                     
            ///returns the number of matches, stores the match results in the specified vectors
            Uint match(const std::string& s,VecNum* vec_num,VecNas* vec_nas,VecNtN* vec_ntn,
                                            const std::string& mod,uint32_t opt_bits,uint32_t pcre2_opts);
                                            
            void init_vars()
                                {
                                    p_vec_num=NULL;
                                    p_vec_nas=NULL;
                                    p_vec_ntn=NULL;
                                    num_map0=NULL;
                                    nas_map0=NULL;
                                    ntn_map0=NULL;
                                    match_opts=jpcre2_match_opts=0;
                                }
            void init(const String& s="")
                                {
                                    init_vars();
                                    m_subject=s;
                                    m_modifier="";
                                }
            void init(const String& s, const String& mod)
                                {
                                    init_vars();
                                    m_subject=s;
                                    m_modifier=mod;
                                }
                            
            RegexMatch(){init();}
            RegexMatch(RegexMatch&){init();}
            RegexMatch(const String& s){init(s);}
            RegexMatch(const String& s, const String& mod){init(s,mod);}
            
            ~RegexMatch(){
                ///Delete map pointers
                if(num_map0) delete num_map0;
                if(nas_map0) delete nas_map0;
                if(ntn_map0) delete ntn_map0;
            }
            
            
            ///define buddies for RegexMatch
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexMatch& setNumberedSubstringVector(VecNum* vec_num)       {p_vec_num=vec_num;              return *this;}
            RegexMatch& setNamedSubstringVector(VecNas* vec_nas)          {p_vec_nas=vec_nas;              return *this;}
            RegexMatch& setNameToNumberMapVector(VecNtN* vec_ntn)         {p_vec_ntn=vec_ntn;              return *this;}
            RegexMatch& setSubject(const String& s)                       {m_subject=s;                    return *this;}
            RegexMatch& setModifiers(const String& s)                     {m_modifier=s;                   return *this;}
            RegexMatch& addJpcre2Options(uint32_t x)                      {jpcre2_match_opts |= x;         return *this;}
            RegexMatch& addPcre2Options(uint32_t x)                       {match_opts |= x;                return *this;}
            RegexMatch& removeJpcre2Options(uint32_t x)                   {jpcre2_match_opts &= ~x;        return *this;}
            RegexMatch& removePcre2Options(uint32_t x)                    {match_opts &= ~x;               return *this;}
            RegexMatch& setFindAll(bool x=true){
                                                if(x) jpcre2_match_opts |= FIND_ALL;
                                                else  jpcre2_match_opts &= ~FIND_ALL;
                                                return *this;
                                               }
            
            Uint exec(){return execute();}
            Uint execute(){return match(m_subject,p_vec_num,p_vec_nas,p_vec_ntn,m_modifier,jpcre2_match_opts,match_opts);}
    };
    
    
    class RegexReplace{
        
        private: 
        
            Regex* re;    ///We will use this to access private members in Regex
            
            String r_subject,r_modifier,r_replw;
            uint32_t replace_opts,jpcre2_replace_opts;
            PCRE2_SIZE buffer_size;
            
            void parseReplacementOpts(const String& mod, uint32_t opt_bits, uint32_t pcre2_opts);
            
            ///returns a replaced string after performing regex replace
            String replace( String mains, String repl,const String& mod,
                                PCRE2_SIZE out_size,uint32_t opt_bits, uint32_t pcre2_opts);
                                            
            void init_vars(){ 
                              replace_opts=jpcre2_replace_opts=0;
                              buffer_size=REGEX_STRING_MAX;
                              r_modifier="";
                            }
            void init(const String& s=""){ r_subject=s;r_modifier="";r_replw="";init_vars(); }
            void init(const String& s,const String& repl){ r_subject=s;r_replw=repl;init_vars(); }
                            
            RegexReplace(RegexReplace&){init();}
            RegexReplace(const String& s,const String& repl){init(s,repl);}
            RegexReplace(const String& s){init(s);}
            RegexReplace(){init();}
            ~RegexReplace(){}
            
            ///define buddies for RegexReplace
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexReplace& setSubject(const String& s)                        {r_subject=s;                   return *this;}
            RegexReplace& setReplaceWith(const String& s)                    {r_replw=s;                     return *this;}
            RegexReplace& setModifiers(const String& s)                      {r_modifier=s;                  return *this;}
            RegexReplace& setBufferSize(PCRE2_SIZE x)                        {buffer_size=x;                 return *this;}
            RegexReplace& addJpcre2Options(uint32_t x)                       {jpcre2_replace_opts |= x;      return *this;}
            RegexReplace& addPcre2Options(uint32_t x)                        {replace_opts |= x;             return *this;}
            RegexReplace& removeJpcre2Options(uint32_t x)                    {jpcre2_replace_opts &= ~x;     return *this;}
            RegexReplace& removePcre2Options(uint32_t x)                     {replace_opts &= ~x;            return *this;}
            
            String exec(){return execute();}
            String execute(){
                return replace(r_subject,r_replw,r_modifier,buffer_size,jpcre2_replace_opts,replace_opts);          
            }
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
            int error_code,jpcre2_error_offset;
            String mylocale;
            String current_action;
            
            ///other opts
            bool null_code; //whether code was null after compilation
            
            // Warning msg 
            String current_warning_msg;
            
            //~ ///Regex re=Regex(); and such are not allowed.
            //~ Regex(const Regex&);
            //~ void operator=(const Regex&);
            
            ///We can't let user call this function explicitly
            void freeRegexMemory(void){if(code) pcre2_code_free(code);}                 ///frees memory used for the compiled regex.
            
            
            void parseCompileOpts(const String& mod, uint32_t opt_bits, uint32_t pcre2_opts);
                            
            pcre2_code* getPcreCode(){return code;}                 ///returns pointer to compiled regex
            
            void init_vars(){ mylocale = DEFAULT_LOCALE;
                              error_number = error_offset = error_code = jpcre2_error_offset = 0;
                              compile_opts = jpcre2_compile_opts = 0;
                              null_code = false;
                              pat_str=modifier="";
                              code=NULL;
                              }
                              
            void init(const String& re="") 
                                           { init_vars();
                                             pat_str=re;
                                             //compileRegex(pat_str,modifier,DEFAULT_LOCALE,jpcre2_compile_opts,compile_opts);
                                           }
            void init(const String& re, const String& mod)
                                           { init_vars();
                                             pat_str=re;modifier=mod;
                                             //compileRegex(pat_str,modifier,DEFAULT_LOCALE,jpcre2_compile_opts,compile_opts);
                                           }
            void init(const String& re, uint32_t pcre2_opts, uint32_t opt_bits=0) 
                                                             { init_vars();
                                                               pat_str=re;
                                                               compile_opts |= pcre2_opts;
                                                               jpcre2_compile_opts |= opt_bits;
                                                               //compileRegex(pat_str,modifier,DEFAULT_LOCALE,jpcre2_compile_opts,compile_opts);
                                                             }
            ///init() must perform a dummy compile, otherwise it will yield to a 
            /// segmentation fault when regex is not initialized and goes out of scope, due to a call of
            ///freeRegexMemory() in the destructor.
            
            
            ///Compiles the regex.
            void compileRegex(const String& re,const String& mod,const String& loc,uint32_t opt_bits, uint32_t pcre2_opts);
                            
                            
            ///Define buddies for Regex
            friend class RegexMatch;
            friend class RegexReplace;
            
            void shallowCopy(const Regex& r){
                rm = r.rm;
                rr = r.rr;
                pat_str = r.pat_str;
                mylocale = r.mylocale;
                modifier = r.modifier;
                null_code = r. null_code;
                error_code = r.error_code;
                error_number = r.error_number;
                error_offset = r.error_offset;
                compile_opts = r.compile_opts;
                current_action = r.current_action;
                jpcre2_compile_opts = r.jpcre2_compile_opts;
                jpcre2_error_offset = r.jpcre2_error_offset;
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
                                error_code = jpcre2_error_offset = ERROR::JIT_COMPILE_FAILED;
                                throw((int)ERROR::JIT_COMPILE_FAILED);
                            }
                            else current_warning_msg="JIT compilation failed! Is it supported?";
                        }  
                    }
                }
                else code = NULL;
            }
            
        public:
            Regex(){init();}
            Regex(const String& re, const String& mod="")  {init(re,mod);execute();}
            Regex(const String& re, uint32_t pcre2_opts, uint32_t opt_bits=0)  {init(re, pcre2_opts, opt_bits);execute();}
            
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
            
            ~Regex(){freeRegexMemory();}
            
                
            String getModifier()        {return modifier;    }
            String getPattern()         {return pat_str;     }
            String getLocale()          {return mylocale;    }      ///Gets LC_CTYPE
            uint32_t getCompileOpts()   {return compile_opts;}      ///returns the compile opts used for compilation
            
            
            ///Error handling
            String getErrorMessage(int err_num);
            String getErrorMessage();
            String getWarningMessage()      {return current_warning_msg;}
            int getErrorNumber()            {return error_number;}
            PCRE2_SIZE getErrorOffset()     {return error_offset;}
            int getErrorCode()              {return error_code;}
            
            
            Regex& compile(const String& re,const String& mod)     {pat_str=re; modifier=mod;  return *this;}
            Regex& compile(const String& re="")                    {pat_str=re;                return *this;}
            Regex& setPattern(const String& re)                    {pat_str=re;                return *this;}
            Regex& setModifiers(const String& x)                   {modifier=x;                return *this;}
            Regex& setLocale(const String& x)                      {mylocale=x;                return *this;}
            Regex& addJpcre2Options(uint32_t x)                    {jpcre2_compile_opts |= x;  return *this;}
            Regex& addPcre2Options(uint32_t x)                     {compile_opts |= x;         return *this;}
            Regex& removeJpcre2Options(uint32_t x)                 {jpcre2_compile_opts &= ~x; return *this;}
            Regex& removePcre2Options(uint32_t x)                  {compile_opts &= ~x;        return *this;}
            
            void exec(){execute();}
            void execute(void){
                compileRegex(pat_str,modifier,mylocale,jpcre2_compile_opts,compile_opts);
            }
            
            
            ///This is the match() function that will be called by users
            RegexMatch& match(const String& s=""){rm=RegexMatch(s);rm.re = this;return rm;}
            RegexMatch& match(const String& s, const String& mod){rm=RegexMatch(s, mod);rm.re = this;return rm;}
            
            ///This is the replace function that will be called by users
            RegexReplace& replace(const String& mains="") {rr=RegexReplace(mains);rr.re=this;return rr;}
            RegexReplace& replace(const String& mains,const String& repl) {rr=RegexReplace(mains,repl);rr.re=this;return rr;}
            
    };
    
    namespace utils{
        String toString(int a);
        String toString(char a);
        String toString(const char* a);
        String toString(PCRE2_UCHAR* a);
            
    }

} ///jpcre2 namespace




#endif
#endif
