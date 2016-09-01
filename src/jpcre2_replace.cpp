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
    
    
        
    void jpcre2::RegexReplace::parseReplacementOpts(const String& mod){
        replace_opts |= PCRE2_SUBSTITUTE_OVERFLOW_LENGTH; ///This enables returning the required length of string
        ///in case substitute fails due to insufficient memory. It is required to try again with the correct amount of
        ///memory allocation.
        
        ///parse pcre options
        for(int i=0;i<(int)mod.length();i++){
            switch (mod[i]){
                case 'e': replace_opts  |= PCRE2_SUBSTITUTE_UNSET_EMPTY;break;
                case 'E': replace_opts  |= PCRE2_SUBSTITUTE_UNKNOWN_UNSET | PCRE2_SUBSTITUTE_UNSET_EMPTY;break;
                case 'g': replace_opts  |= PCRE2_SUBSTITUTE_GLOBAL;break;
                case 'x': replace_opts  |= PCRE2_SUBSTITUTE_EXTENDED;break;
                default : if((jpcre2_replace_opts & VALIDATE_MODIFIER)!=0)
                          {re->error_code=re->jpcre2_error_offset=(int)mod[i];throw((int)ERROR::INVALID_MODIFIER);}break;
            }
        }
    }
    
    
    jpcre2::String jpcre2::RegexReplace::replace( String mains, String repl,const String& mod,
                                    PCRE2_SIZE out_size,uint32_t opt_bits, uint32_t pcre2_opts){
        
        /// If code is null, there's no need to proceed any further
        if(re->null_code) return mains;
        ///populate some class vars
        ///Add PCRE2 options to replace_opts
        replace_opts |= pcre2_opts;
        ///Add opt_bits to jpcre2_replace_opts
        jpcre2_replace_opts |= opt_bits;
        
        ///Make additions to replace_opts
        parseReplacementOpts(mod);
        
        PCRE2_SPTR subject = (PCRE2_SPTR)mains.c_str();
        PCRE2_SIZE subject_length = strlen((char *)subject);
        PCRE2_SPTR replace = (PCRE2_SPTR)repl.c_str();
        PCRE2_SIZE replace_length = strlen((char *)replace);
        PCRE2_SIZE outlengthptr=(PCRE2_SIZE)out_size;
        int ret=0,try_count=0;
        PCRE2_UCHAR* output_buffer;
        output_buffer=(PCRE2_UCHAR*)malloc(outlengthptr * sizeof(PCRE2_UCHAR));
        
        loop:
        ret=pcre2_substitute(
            re->code,                          /*Points to the compiled pattern*/
            subject,                           /*Points to the subject string*/
            subject_length,                    /*Length of the subject string*/
            0,                                 /*Offset in the subject at which to start matching*/
            replace_opts,                      /*Option bits*/
            0,                                 /*Points to a match data block, or is NULL*/
            0,                                 /*Points to a match context, or is NULL*/
            replace,                           /*Points to the replacement string*/
            replace_length,                    /*Length of the replacement string*/
            output_buffer,                     /*Points to the output buffer*/
            &outlengthptr                      /*Points to the length of the output buffer*/
        );
        re->error_code=(int)ret;
        re->error_offset=ret;
        
        if (ret < 0){
            ///Handle errors
            if((replace_opts & PCRE2_SUBSTITUTE_OVERFLOW_LENGTH) !=0 && ret == (int)PCRE2_ERROR_NOMEMORY && try_count<1){
                /// Second retry in case output buffer was not big enough
                outlengthptr++;  /// It was changed to required length
                output_buffer=(PCRE2_UCHAR*)realloc(output_buffer,outlengthptr * sizeof(PCRE2_UCHAR));
                
                goto loop;
            }
            else {::free(output_buffer);throw(ret);}
        }
        String result=jpcre2::utils::toString((char*)output_buffer);
        ::free(output_buffer);
        return result;
    }
