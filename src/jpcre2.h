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

namespace jpcre2_utils{
    template<typename T>
    std::string toString(T a);
}

namespace jpcre2{

    #define REGEX_STRING_MAX std::numeric_limits<int>::max() //This limits the maximum length of string that can be handled by default.
    #define DEFAULT_LOCALE "none"
    
    ///Errors// jpcre2 error codes are positive numbers
    #define JPCRE2_ERROR_INVALID_MODIFIER             (2)
    
    ///Option bits
    #define JPCRE2_VALIDATE_MODIFIER                  0x0000001u
    
    typedef std::map<std::string,std::string> MapNas; //Map for Named substring
    typedef std::map<int,std::string> MapNum;         //Map for Numbered substring
    typedef std::map<std::string,int> MapNtN;         //Map for Named substring to Numbered substring
    typedef std::vector<MapNas> VecNas;               //Vector of MapNas
    typedef std::vector<MapNtN> VecNtN;               //Vector of MapNtN
    typedef std::vector<MapNum> VecNum;               //Vector of MapNum
    
    
    class Regex{
        
        private:
        
        std::string pat_str;
        std::string modifier;
        PCRE2_SPTR pattern;
        pcre2_code *code;
        int error_number;
        PCRE2_SIZE error_offset;
        uint32_t compile_opts,action_opts,jit_opts,jpcre2_compile_opts,jpcre2_action_opts;
        int error_code,jpcre2_error_offset;
        std::string mylocale;
        
        ///other opts
        bool opt_jit_compile;
        
        // Warning msg 
        std::string current_warning_msg;
        
        ///Only allowed object declaration is Regex re; i.e re=Regex() and such are not allowed.
        ///This is to prevent segmentation fault regarding the call of function pcre2_code_free() 
        ///inside destructor which is essential to avoid memory leaks.
        Regex(Regex&);
        void operator=(Regex&);
        
        ///We can't let user call this function explicitly
        void freeRegexMemory(void){pcre2_code_free(code);}                 ///frees memory used for the compiled regex.
        void parseReplacementOpts(const std::string& mod,uint32_t opt_bits);
        void parseCompileOpts(const std::string& mod,uint32_t opt_bits);
        void init(void){pat_str="";modifier="";mylocale=DEFAULT_LOCALE;error_number=0;error_offset=0;error_code=0;
                        jpcre2_error_offset=0;jpcre2_compile_opts=0;jpcre2_action_opts=0;compile("","");}
        ///init must perform a dummy compile.
        pcre2_code* getPcreCode(){return code;}                 ///returns pointer to compiled regex
        
        public:
        

        Regex(){init();}
        Regex(const std::string& re,const std::string& mod="",const std::string& loc=DEFAULT_LOCALE)
        {init();compile(re,mod,loc);}
        Regex(const std::string& re,const std::string& mod,const std::string& loc,uint32_t opt_bits)
        {init();compile(re,mod,loc,opt_bits);}
        Regex(const std::string& re,const std::string& mod,uint32_t opt_bits)
        {init();compile(re,mod,opt_bits);}
        Regex(const std::string& re,uint32_t opt_bits)
        {init();compile(re,opt_bits);}
        
        ~Regex(){freeRegexMemory();}
        
            
        std::string getModifier(){return modifier;}
        std::string getPattern(){return pat_str;}
        std::string getLocale(){return mylocale;}               ///Gets LC_CTYPE
        uint32_t getCompileOpts(){return jpcre2_compile_opts;}
        uint32_t getActionOpts(){return jpcre2_action_opts;}
        
        ///Compiles the regex.
        ///If any argument is not passed, it will be left empty
        void compile(const std::string& re,const std::string& mod,const std::string& loc,uint32_t opt_bits);
        void compile(const std::string& re,const std::string& mod,const std::string& loc){compile(re,mod,loc,0);}
        void compile(const std::string& re,const std::string& mod,uint32_t opt_bits=0){compile(re,mod,DEFAULT_LOCALE,opt_bits);}
        void compile(const std::string& re,uint32_t opt_bits=0){compile(re,"",DEFAULT_LOCALE,opt_bits);}
        
        ///returns a replaced string after performing regex replace
        ///If modifier is not passed it will be defaulted to empty string
        std::string replace( std::string mains, std::string repl,const std::string& mod="",PCRE2_SIZE out_size=REGEX_STRING_MAX,uint32_t opt_bits=0);
        
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
        
        ///0-vector variants //useful for true-false check
        bool match(const std::string& subject);  ///find_all=true is not meaningful
        
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
