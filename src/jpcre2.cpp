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

#include "jpcre2.h"

    template<class T>
    jpcre2::String jpcre2::utils::toString(T a){
		thread_local std::ostringstream ss;
		ss.str("");
		ss.clear();
        ss << a;
        return ss.str();
    }

    jpcre2::String jpcre2::Regex::getErrorMessage(){
        return getErrorMessage(error_code);
    }
    
    jpcre2::String jpcre2::Regex::getErrorMessage(int err_num){
        if(err_num==ERROR::INVALID_MODIFIER){
            return "Invalid Modifier: "+utils::toString((char)jpcre2_error_offset);
        }
        else if(err_num == ERROR::JIT_COMPILE_FAILED) {
            return "JIT compilation failed! Is it supported?";
        }
        else{
            PCRE2_UCHAR buffer[4024];
            pcre2_get_error_message(err_num, buffer, sizeof(buffer));
            return utils::toString((PCRE2_UCHAR*)buffer)+"; error offset: "+utils::toString((int)error_offset);
            
        }
    }
    
    
    void jpcre2::Regex::parseCompileOpts(const String& mod, uint32_t opt_bits, uint32_t pcre2_opts){
        ///This function sets opts from scratch
        compile_opts = pcre2_opts;
        jpcre2_compile_opts = opt_bits;
        ///parse pcre options
        for(size_t i=0;i<mod.length();++i){
            switch (mod[i]){
                case 'e': compile_opts |= PCRE2_MATCH_UNSET_BACKREF;break;
                case 'i': compile_opts |= PCRE2_CASELESS;break;
                case 'j': compile_opts |= PCRE2_ALT_BSUX                  ///\u \U \x will act as javascript standard
                                       | PCRE2_MATCH_UNSET_BACKREF;break; ///unset back-references will act as javascript std.
                case 'm': compile_opts |= PCRE2_MULTILINE;break;
                case 'n': compile_opts |= PCRE2_UTF | PCRE2_UCP;break;
                case 's': compile_opts |= PCRE2_DOTALL;break;
                case 'u': compile_opts |= PCRE2_UTF;break;
                case 'x': compile_opts |= PCRE2_EXTENDED;break;
                case 'A': compile_opts |= PCRE2_ANCHORED;break;
                case 'D': compile_opts |= PCRE2_DOLLAR_ENDONLY;break;
                case 'J': compile_opts |= PCRE2_DUPNAMES;break;
                case 'S': jpcre2_compile_opts |= JIT_COMPILE;break;    ///Optimization opt
                case 'U': compile_opts |= PCRE2_UNGREEDY;break;
                default : if((opt_bits & VALIDATE_MODIFIER)!=0)
                          {error_code=jpcre2_error_offset=(int)mod[i];throw((int)ERROR::INVALID_MODIFIER);}break;
            }
        }
    }
    
    
    void jpcre2::Regex :: compileRegex(const String& re,const String& mod, const String& loc,
                                    uint32_t opt_bits, uint32_t pcre2_opts){
        c_pattern=(PCRE2_SPTR)re.c_str();
        
        ///populate some class vars
        pat_str=re;
        modifier=mod;
        mylocale=loc;
        
        ///populate compile_opts from scratch
        parseCompileOpts(mod,opt_bits, pcre2_opts);
    
    /*************************************************************************
    * Now we are going to compile the regular expression pattern, and handle *
    * any errors that are detected.                                          *
    *************************************************************************/
    
        pcre2_compile_context *ccontext = pcre2_compile_context_create(NULL);
        
        if(loc!="none"){
            String loc_old;
            loc_old=utils::toString(std::setlocale(LC_CTYPE,loc.c_str()));
            const unsigned char *tables = pcre2_maketables(NULL);
            pcre2_set_character_tables(ccontext, tables);
            std::setlocale(LC_CTYPE,loc_old.c_str());
        }
    
        null_code = false;
        
        code = pcre2_compile(
            c_pattern,                    /* the pattern */
            PCRE2_ZERO_TERMINATED,      /* indicates pattern is zero-terminated */
            compile_opts,               /* default options */
            &error_number,              /* for error number */
            &error_offset,              /* for error offset */
            ccontext);                  /* use compile context */
        
        error_code=error_number;
        /* Compilation failed: print the error message and exit. */
    
        if (code == NULL){
            ///must not free regex memory, the only function has that right is the destroyer.
            ///freeRegexMemory();
            /// Perform a dummy compile, code must not be NULL
            null_code = true;
            code = pcre2_compile((PCRE2_SPTR)"", PCRE2_ZERO_TERMINATED,0,&error_number,&error_offset,ccontext);
            throw(error_number);
        }
        else if((jpcre2_compile_opts & JIT_COMPILE) != 0){
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
