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

    template<typename T>
    jpcre2::String jpcre2_utils::toString(T a){
        std::stringstream ss;
        ss <<a;
        return ss.str();
    }

    jpcre2::String jpcre2::Regex::getErrorMessage(){
        return getErrorMessage(error_code);
    }
    
    jpcre2::String jpcre2::Regex::getErrorMessage(int err_num){
        if(err_num==ERROR::INVALID_MODIFIER){
            return "Invalid Modifier: "+jpcre2_utils::toString((char)jpcre2_error_offset);
        }
        else{
            PCRE2_UCHAR buffer[4024];
            pcre2_get_error_message(err_num, buffer, sizeof(buffer));
            return jpcre2_utils::toString((PCRE2_UCHAR*)buffer)+"; error offset: "+jpcre2_utils::toString((int)error_offset);
            
        }
    }
    
    
    void jpcre2::Regex::parseCompileOpts(const String& mod,uint32_t opt_bits){

        compile_opts=0;
        jit_opts=0;
        opt_jit_compile=false;
        
        ///default options          
        
        ///parse pcre options
        for(int i=0;i<(int)mod.length();i++){
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
                case 'S': opt_jit_compile=true;jit_opts |= PCRE2_JIT_COMPLETE;break;    ///Optimization opt
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
        
        ///populate compile_opts...
        parseCompileOpts(mod,opt_bits);
        ///Now add other PCRE2 options to the compile_opts
        compile_opts |= pcre2_opts;
    
    /*************************************************************************
    * Now we are going to compile the regular expression pattern, and handle *
    * any errors that are detected.                                          *
    *************************************************************************/
    
        pcre2_compile_context *ccontext = pcre2_compile_context_create(NULL);
        
        if(loc!="none"){
            String loc_old;
            loc_old=jpcre2_utils::toString(std::setlocale(LC_CTYPE,loc.c_str()));
            const unsigned char *tables = pcre2_maketables(NULL);
            pcre2_set_character_tables(ccontext, tables);
            std::setlocale(LC_CTYPE,loc_old.c_str());
        }
    
    
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
            throw(error_number);
        }
        else if(opt_jit_compile){
            ///perform jit compilation:
            int jit_ret=pcre2_jit_compile(code, jit_opts);
            if(jit_ret!=0){
                //{throw(JIT_COMPILE_ERROR);};      // Must not throw any exception here
                current_warning_msg="JIT compilation failed! Is it supported?";
            }  
        }
    }
