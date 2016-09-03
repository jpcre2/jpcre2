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

#include "jpcre2.h"
#include <cstdio> ///snprintf
#include <cassert>

const jpcre2::SIZE_T jpcre2::SUBSTITUTE_RESULT_INIT_SIZE = std::numeric_limits<int>::max();

const jpcre2::String jpcre2::LOCALE_NONE = "JPCRE2_NONE";
const jpcre2::String jpcre2::LOCALE_DEFAULT = LOCALE_NONE;
const jpcre2::String jpcre2::JIT_ERROR_MESSAGE_PREFIX = "JIT compilation failed! ";

jpcre2::String jpcre2::utils::toString(const char* a){
    if(a) return String(a);
    else return "";
}
jpcre2::String jpcre2::utils::toString(char a){
    if(a!='\0')return String(1, a);
    else return "";
}
jpcre2::String jpcre2::utils::toString(int x) {
    int length = snprintf( 0, 0, "%d", x );
    assert( length >= 0 );
    char* buf = new char[length + 1];
    snprintf( buf, length + 1, "%d", x );
    String str( buf );
    delete[] buf;
    return str;
}
jpcre2::String jpcre2::utils::toString(PCRE2_UCHAR* a){
    if(a) return String((char*)a);
    else return "";
}

jpcre2::String jpcre2::utils::getPcre2ErrorMessage(int err_num){
    PCRE2_UCHAR buffer[8048];
    pcre2_get_error_message(err_num, buffer, sizeof(buffer));
    return toString((PCRE2_UCHAR*)buffer);
}

jpcre2::String jpcre2::Regex::getErrorMessage(int err_num, PCRE2_SIZE err_off){
    if(err_num == ERROR::INVALID_MODIFIER){
        return "Invalid Modifier: "+utils::toString((char)err_off);
    }
    else if(err_num == ERROR::JIT_COMPILE_FAILED) {
        return JIT_ERROR_MESSAGE_PREFIX+utils::getPcre2ErrorMessage((int)err_off);
    }
    else{
        return utils::getPcre2ErrorMessage((int)err_num)+". Error offset: "+utils::toString((int)err_off);
    }
}


void jpcre2::Regex::parseCompileOpts(){
    ///parse modifiers
    for(SIZE_T i=0;i<modifier.length();++i){
        switch (modifier[i]){
            case 'e': compile_opts |= PCRE2_MATCH_UNSET_BACKREF;break;
            case 'i': compile_opts |= PCRE2_CASELESS;break;
            case 'j': compile_opts |= PCRE2_ALT_BSUX                  ///\u \U \x will act as javascript standard
                                   |  PCRE2_MATCH_UNSET_BACKREF;break; ///unset back-references will act as javascript std.
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
            default : if((jpcre2_compile_opts & VALIDATE_MODIFIER)!=0)
                      {error_number=error_offset=(int)modifier[i];throw((int)ERROR::INVALID_MODIFIER);}break; ///break is needed
        }
    }
}


void jpcre2::Regex :: compileRegex(){
    ///Get c_str of pattern
    PCRE2_SPTR c_pattern=(PCRE2_SPTR)pat_str.c_str();
    
    ///populate compile_opts
    parseCompileOpts();

    /*************************************************************************
    * Now we are going to compile the regular expression pattern, and handle *
    * any errors that are detected.                                          *
    *************************************************************************/

    pcre2_compile_context *ccontext = pcre2_compile_context_create(0);
    
    if(mylocale != LOCALE_NONE){
        String loc_old;
        loc_old=utils::toString(std::setlocale(LC_CTYPE, mylocale.c_str()));
        const unsigned char *tables = pcre2_maketables(0);
        pcre2_set_character_tables(ccontext, tables);
        std::setlocale(LC_CTYPE,loc_old.c_str());
    }

    code = pcre2_compile(
        c_pattern,                  /* the pattern */
        PCRE2_ZERO_TERMINATED,      /* indicates pattern is zero-terminated */
        compile_opts,               /* default options */
        &error_number,              /* for error number */
        &error_offset,              /* for error offset */
        ccontext);                  /* use compile context */
        
        
    if (code == 0){
        /* Compilation failed */
        ///must not free regex memory, the only function has that right is the destroyer.
        throw(error_number);
    }
    else if((jpcre2_compile_opts & JIT_COMPILE) != 0){
        ///perform jit compilation:
        int jit_ret=pcre2_jit_compile(code, PCRE2_JIT_COMPLETE);
        if(jit_ret!=0){
            if((jpcre2_compile_opts & ERROR_ALL) != 0) {
                error_number = ERROR::JIT_COMPILE_FAILED;
                error_offset = jit_ret;
                throw((int)ERROR::JIT_COMPILE_FAILED);
            }
            else current_warning_msg="JIT compile failed: "+utils::getPcre2ErrorMessage(jit_ret);
        }  
    }
}
