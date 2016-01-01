/***********************************************************************
 * C++ wrapper for several utilities of PCRE2 Library
 * ********************************************************************/

/* 
This is a public C++ wrapper for several utilities of the PCRE library, second API, to be
#included by applications that call PCRE2 functions.

           Copyright (c) 2015 Md. Jahidul Hamid

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

#include <string>
#include <cstring>
#include <sstream>
#include <limits>
#include <vector>
#include <map>


namespace jpcre2{
    
    ///Errors // JPCRE2 error codes are positive numbers while PCRE2 error codes are negative numbers
    namespace ERROR {
    enum { INVALID_MODIFIER                 = 2 };
    }
    
    #define REGEX_STRING_MAX std::numeric_limits<int>::max() //This limits the maximum length of string that can be handled by default.
                                                             //This limit may or may not be used.
    #define DEFAULT_LOCALE "none"   ///We won't do anything about locale if it is set to "none" 
    
    ///Option bits. These are the options for JPCRE2.
    enum {  NONE                                = 0x0000000u,
            VALIDATE_MODIFIER                   = 0x0000001u,
            FIND_ALL                            = 0x0000002u };
    
    
    
    
    typedef size_t Uint;
    typedef size_t SIZE_T;
    typedef unsigned short Ush;
    typedef std::string String;
    
    typedef std::map<String,String> MapNas;       //Map for Named substring
    typedef std::map<Uint,String> MapNum;         //Map for Numbered substring
    typedef std::map<String,Uint> MapNtN;         //Map for Named substring to Numbered substring
    typedef std::vector<MapNas> VecNas;               //Vector of MapNas
    typedef std::vector<MapNtN> VecNtN;               //Vector of MapNtN
    typedef std::vector<MapNum> VecNum;               //Vector of MapNum
    
    
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
            
            void parseMatchOpts(const String& mod);
            void getNumberedSubstrings(int rc, pcre2_match_data *match_data,MapNum& num_map0);
            void getNamedSubstrings(int namecount,int name_entry_size,PCRE2_SPTR tabptr, pcre2_match_data *match_data,
                                                                                     MapNas& nas_map0, MapNtN& nn_map0);
                                                                                     
            ///returns the number of matches, stores the match results in the specified vectors
            Uint match(const std::string& s,VecNum& vec_num,VecNas& vec_nas,VecNtN& vec_nn,
                                            const std::string& mod,uint32_t opt_bits,uint32_t pcre2_opts);
                                            
            void init(String s=""){p_vec_num=nullptr;p_vec_nas=nullptr;p_vec_ntn=nullptr;
                                    m_subject=s;m_modifier="";match_opts=0;jpcre2_match_opts=NONE;}
                            
            RegexMatch(RegexMatch&){init();}
            //~ RegexMatch& operator=(RegexMatch&){init(); return this;}
            //~ RegexMatch& operator=(RegexMatch&,const String& s){init(s); return this;}
            RegexMatch(const String& s){init(s);}
            RegexMatch(){init();}
            ~RegexMatch(){}
            
            
            ///define buddies for RegexMatch
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexMatch& numberedSubstringVector(VecNum& vec_num)       {p_vec_num=&vec_num;            return *this;}
            RegexMatch& namedSubstringVector(VecNas& vec_nas)          {p_vec_nas=&vec_nas;            return *this;}
            RegexMatch& nameToNumberMapVector(VecNtN& vec_ntn)         {p_vec_ntn=&vec_ntn;            return *this;}
            RegexMatch& subject(const String& s)                               {m_subject=s;                   return *this;}
            RegexMatch& modifiers(const String& s)                             {m_modifier=s;                  return *this;}
            RegexMatch& jpcre2Options(uint32_t x=NONE)                  {jpcre2_match_opts=x;           return *this;}
            RegexMatch& pcre2Options(uint32_t x=NONE)                   {match_opts=x;                  return *this;}
            RegexMatch& findAll()                                       {jpcre2_match_opts |= FIND_ALL; return *this;}
            
            Uint execute(){
                VecNum vec_num0;
                VecNas vec_nas0;
                VecNtN vec_ntn0;
                
                VecNum& vec_num = p_vec_num ? *p_vec_num : vec_num0;
                VecNas& vec_nas = p_vec_nas ? *p_vec_nas : vec_nas0;
                VecNtN& vec_ntn = p_vec_ntn ? *p_vec_ntn : vec_ntn0;
                
                return match(m_subject,vec_num,vec_nas,vec_ntn,m_modifier,jpcre2_match_opts,match_opts);               
            }
    };
    
    
    class RegexReplace{
        
        private: 
        
            Regex* re;    ///We will use this to access private members in Regex
            
            String r_subject,r_modifier,r_replw;
            uint32_t replace_opts,jpcre2_replace_opts;
            PCRE2_SIZE buffer_size;
            
            
            void parseReplacementOpts(const String& mod);
            
            ///returns a replaced string after performing regex replace
            String replace( String mains, String repl,const String& mod,
                                PCRE2_SIZE out_size,uint32_t opt_bits, uint32_t pcre2_opts);
                                            
            void init(const String& s=""){r_subject=s;r_modifier="";r_replw="";replace_opts=0;
                                            jpcre2_replace_opts=NONE;buffer_size=REGEX_STRING_MAX;}
            void init(const String& s,const String& repl){r_subject=s;r_modifier="";r_replw=repl;replace_opts=0;
                                            jpcre2_replace_opts=NONE;buffer_size=REGEX_STRING_MAX;}
                            
            RegexReplace(RegexReplace&){init();}
            //~ RegexMatch& operator=(RegexMatch&){init(); return this;}
            //~ RegexMatch& operator=(RegexMatch&,const String& s){init(s); return this;}
            RegexReplace(const String& s,const String& repl){init(s,repl);}
            RegexReplace(const String& s=""){init(s);}
            ~RegexReplace(){}
            
            
            ///define buddies for RegexReplace
            friend class Regex;
            
            
        public:
           
            ///Chained functions for taking parameters
            RegexReplace& subject(const String& s)                        {r_subject=s;                   return *this;}
            RegexReplace& replaceWith(const String& s)                    {r_replw=s;                     return *this;}
            RegexReplace& modifiers(const String& s)                      {r_modifier=s;                  return *this;}
            RegexReplace& jpcre2Options(uint32_t x=NONE)                  {jpcre2_replace_opts=x;         return *this;}
            RegexReplace& pcre2Options(uint32_t x=NONE)                   {replace_opts=x;                return *this;}
            RegexReplace& bufferSize(PCRE2_SIZE x)                        {buffer_size=x;                 return *this;}
            
            
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
            PCRE2_SPTR c_pattern;
            pcre2_code *code;
            int error_number;
            PCRE2_SIZE error_offset;
            uint32_t compile_opts,jit_opts,jpcre2_compile_opts;
            int error_code,jpcre2_error_offset;
            String mylocale;
            String current_action;
            
            ///other opts
            bool opt_jit_compile;
            
            
            // Warning msg 
            String current_warning_msg;
            
            ///Regex re=Regex(); and such are not allowed.
            Regex(const Regex&);
            void operator=(const Regex&);
            
            ///We can't let user call this function explicitly
            void freeRegexMemory(void){pcre2_code_free(code);}                 ///frees memory used for the compiled regex.
            
            
            void parseCompileOpts(const String& mod,uint32_t opt_bits);
                            
            pcre2_code* getPcreCode(){return code;}                 ///returns pointer to compiled regex
            
            
            void init(const String& re=""){ pat_str=re;modifier="";mylocale=DEFAULT_LOCALE;error_number=0;
                                            error_offset=0;error_code=0;jpcre2_error_offset=0;compile_opts=0;
                                            compileRegex("","",DEFAULT_LOCALE,0,0);}
            void init(const String& re, const String& mod){ pat_str=re;modifier=mod;mylocale=DEFAULT_LOCALE;error_number=0;
                                                            error_offset=0;error_code=0;jpcre2_error_offset=0;compile_opts=0;
                                                            compileRegex("","",DEFAULT_LOCALE,0,0);}  
                ///init() must perform a dummy compile, otherwise it will yield to a 
                /// segmentation fault when regex is not initialized and goes out of scope, due to a call of
                ///freeRegexMemory() in the destructor.
            
            
            ///Compiles the regex.
            void compileRegex(const String& re,const String& mod,const String& loc,uint32_t opt_bits, uint32_t pcre2_opts);
                            
                            
            ///Define buddies for Regex
            friend class RegexMatch;
            friend class RegexReplace;
            
        public:
    
            Regex(){init();}
            Regex(const String& re)                     {init(re);    }
            Regex(const String& re, const String& mod)  {init(re,mod);}
            
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
            
            
            Regex& compile(const String& re,const String& mod)  {pat_str=re; modifier=mod;  return *this;}
            Regex& compile(const String& re="")                 {pat_str=re;                return *this;}
            Regex& pattern(const String& re)                    {pat_str=re;                return *this;}
            Regex& modifiers(const String& x)                   {modifier=x;                return *this;}
            Regex& locale(const String& x)                      {mylocale=x;                return *this;}
            Regex& jpcre2Options(uint32_t x)                    {jpcre2_compile_opts=x;     return *this;}
            Regex& pcre2Options(uint32_t x)                     {compile_opts=x;            return *this;}
            
            void execute(void){
                compileRegex(pat_str,modifier,mylocale,jpcre2_compile_opts,compile_opts);
            }
            
            
            ///This is the match() function that will be called by users
            RegexMatch& match(String s=""){rm=RegexMatch(s);rm.re = this;return rm;}
            
            ///This is the replace function that will be called by users
            RegexReplace& replace() {rr=RegexReplace();rr.re=this;return rr;}
            RegexReplace& replace(const String& mains) {rr=RegexReplace(mains);rr.re=this;return rr;}
            RegexReplace& replace(const String& mains,const String& repl) {rr=RegexReplace(mains,repl);rr.re=this;return rr;}
            
    };
    

} ///jpcre2 namespace


namespace jpcre2_utils{
    
    
    #define Glue(a,b) a##b 
    
    template<typename T>
    jpcre2::String toString(T a);
}



#endif
#endif
