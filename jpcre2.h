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

#include "pcre2.h"



#ifdef __cplusplus

#include <string>
#include <cstring>
#include <sstream>
#include <limits>
#include <vector>
#include <map>

namespace jpcre2_utils{
    template<typename T>
    std::string toString(T a);
}

namespace jpcre2{

    #define REGEX_STRING_MAX std::numeric_limits<int>::max() //This limits the maximum length of string that can be handled by default.
    #define DEFAULT_LOCALE "none"
    
    typedef std::map<std::string,std::string> MapNas; //Map for Named substring
    typedef std::map<int,std::string> MapNum;         //Map for Numbered substring
    typedef std::map<std::string,int> MapNtN;         //Map for Named substring to Numbered substring
    typedef std::vector<MapNas> VecNas;               //Vector of MapNas
    typedef std::vector<MapNtN> VecNtN;               //Vector of MapNtN
    typedef std::vector<MapNum> VecNum;               //Vector of MapNum
    
    
    class Pcre2Regex{
        
        private:
        
        std::string pat_str;
        std::string modifier;
        PCRE2_SPTR pattern;
        pcre2_code *code;
        int error_number;
        PCRE2_SIZE error_offset;
        uint32_t compile_opts,action_opts,jit_opts;
        int error_code;
        std::string mylocale;
        
        ///other opts
        bool opt_jit_compile;
        
        // Warning msg 
        std::string current_warning_msg;
        
        public:
        
        ///constructor must not call compile() function.
        Pcre2Regex(){pat_str="";modifier="";mylocale=DEFAULT_LOCALE;}
        Pcre2Regex(const std::string& re,const std::string& mod="",const std::string& loc=DEFAULT_LOCALE)
        {pat_str=re;modifier=mod;mylocale=loc;}
        
        ~Pcre2Regex(){freeRegexMemory();}
        
        void parseReplacementOpts(const std::string& mod);
        void parseCompileOpts(const std::string& mod);
        void parseOpts(const std::string& mod){parseReplacementOpts(mod);parseCompileOpts(mod);}
        
        std::string getModifier(){return modifier;}
        void setModifier(const std::string& mod){modifier=mod;}
        
        std::string getPattern(){return pat_str;}
        void setPattern(const std::string& pat){pat_str=pat;}
        
        void setLocale(const std::string& loc){mylocale=loc;}   ///Sets LC_CTYPE
        std::string getLocale(){return mylocale;}               ///Gets LC_CTYPE
        
        pcre2_code* getPcreCode(){return code;}                 ///returns address to compiled regex
        void freeRegexMemory(void){pcre2_code_free(code);}                 ///frees memory used for the compiled regex.
        
        ///Compiles the regex.
        ///If pattern or modifier or both not passed, they will be defaulted to previously set value.
        void compile(void){compile(pat_str,modifier,mylocale);}
        void compile(const std::string& re,const std::string& mod,const std::string& loc=DEFAULT_LOCALE);
        void compile(const std::string& re){compile(re,modifier,mylocale);}
        
        ///returns a replaced string after performing regex replace
        ///If modifier is not passed it will be defaulted to empty string
        std::string replace( std::string mains, std::string repl,const std::string& mod="",PCRE2_SIZE out_size=REGEX_STRING_MAX);
        std::string replace( std::string mains, std::string repl,size_t out_size){return replace(mains,repl,"",out_size);}
        
        ///returns true for successful match, stores the match results in the specified vectors
        bool match(const std::string& subject,VecNum& vec_num,VecNas& vec_nas,VecNtN& vec_nn,bool find_all=false);
        
        ///Other variants of match function
        ///3-vector variants
        bool match(const std::string& subject,VecNum& vec_num,VecNtN& vec_nn,VecNas& vec_nas,bool find_all=false);
        bool match(const std::string& subject,VecNas& vec_nas,VecNum& vec_num,VecNtN& vec_nn,bool find_all=false);
        bool match(const std::string& subject,VecNas& vec_nas,VecNtN& vec_nn,VecNum& vec_num,bool find_all=false);
        bool match(const std::string& subject,VecNtN& vec_nn,VecNas& vec_nas,VecNum& vec_num,bool find_all=false);
        bool match(const std::string& subject,VecNtN& vec_nn,VecNum& vec_num,VecNas& vec_nas,bool find_all=false);
        
        ///2-vector variants
        bool match(const std::string& subject,VecNum& vec_num,VecNas& vec_nas,bool find_all=false);
        bool match(const std::string& subject,VecNas& vec_nas,VecNum& vec_num,bool find_all=false);
        bool match(const std::string& subject,VecNum& vec_num,VecNtN& vec_nn,bool find_all=false);
        bool match(const std::string& subject,VecNtN& vec_nn,VecNum& vec_num,bool find_all=false);
        bool match(const std::string& subject,VecNas& vec_nas,VecNtN& vec_nn,bool find_all=false);
        bool match(const std::string& subject,VecNtN& vec_nn,VecNas& vec_nas,bool find_all=false);
        
        ///1-vector variants
        bool match(const std::string& subject,VecNum& vec_num,bool find_all=false);
        bool match(const std::string& subject,VecNas& vec_nas,bool find_all=false);
        bool match(const std::string& subject,VecNtN& vec_nn,bool find_all=false);
        
        ///Error handling
        std::string getErrorMessage(int err_num);
        std::string getErrorMessage();
        std::string getWarningMessage(){return current_warning_msg;}
        int getErrorNumber(){return error_number;}
        int getErrorCode(){return error_code;}
        PCRE2_SIZE getErrorOffset(){return error_offset;}
    };
    

} ///jpcre2 namespace





#endif
#endif
