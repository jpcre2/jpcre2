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

    void jpcre2::RegexMatch::parseMatchOpts(const String& mod){
        ///This function works by retaining previous value
        match_opts = 0;jpcre2_match_opts = 0;
        ///parse pcre and jpcre2 options
        for(size_t i=0;i<mod.length();++i){
            switch (mod[i]){
                case 'A': match_opts        |= PCRE2_ANCHORED;break;
                case 'g': jpcre2_match_opts |= FIND_ALL;break;
                default : if((jpcre2_match_opts & VALIDATE_MODIFIER)!=0)
                          {re->error_code=re->jpcre2_error_offset=(int)mod[i];throw((int)ERROR::INVALID_MODIFIER);}break;
            }
        }
    }




    void jpcre2::RegexMatch::getNumberedSubstrings(int rc, pcre2_match_data *match_data,jpcre2::MapNum& num_map0){
        for (int i = 0; i < rc; i++){
            String value;
            //~ PCRE2_SPTR substring_start = subject + ovector[2*i];
            //~ size_t substring_length = ovector[2*i+1] - ovector[2*i];
            //~ String tmps1=jpcre2::utils::toString((char *)substring_start);
            //~ value=tmps1.substr(0,substring_length);
            ///If we use pcre2_substring_get_bynumber(),
            ///we will have to deal with returned error codes and memory
            PCRE2_UCHAR **bufferptr;
            PCRE2_SIZE bufflen/*=REGEX_STRING_MAX*/;
            pcre2_substring_length_bynumber(match_data,(uint32_t)i,&bufflen);
            bufferptr=(PCRE2_UCHAR**)malloc(bufflen * sizeof(PCRE2_UCHAR));
            int ret=pcre2_substring_get_bynumber(match_data, (uint32_t)i, bufferptr, &bufflen);
            if(ret<0){
                switch(ret){
                    case PCRE2_ERROR_NOMEMORY: throw(ret);break;
                    default:break;   ///Other errors should be ignored
                }
            }
            value=jpcre2::utils::toString((char*)*bufferptr);
            ///pcre2_substring_free(*bufferptr); 
            ///must free memory, pcre2_substring_free() yields to segmentation fault in several cases ( try '(?<name>\d)?' )
            /// (may be a bug?)
            ///Instead use free() to free the memory
            ::free(bufferptr);                  ///must free memory
            num_map0[i]=value;
        }
    }
    
    void jpcre2::RegexMatch::getNamedSubstrings(int namecount,int name_entry_size,PCRE2_SPTR tabptr, pcre2_match_data *match_data,
                                                            jpcre2::MapNas& nas_map0, jpcre2::MapNtN& nn_map0){
        
        for (int i = 0; i < namecount; i++){
            String key,value,value1;
            
            //~ #if PCRE2_CODE_UNIT_WIDTH == 8
            int n = (tabptr[0] << 8) | tabptr[1];
            key=jpcre2::utils::toString((char*)(tabptr+2));
            //~ #elif PCRE2_CODE_UNIT_WIDTH == 16
            //~ int n = tabptr[0];
            //~ key=jpcre2::utils::toString((char*)(tabptr+1));
            //~ #elif PCRE2_CODE_UNIT_WIDTH == 32
            //~ int n = tabptr[0];
            //~ key=jpcre2::utils::toString((char*)(tabptr+1));
            //~ #else
            //~ #error PCRE2_CODE_UNIT_WIDTH must be 8 or 16 or 32
            //~ #endif
            
            //~ String tmps2=jpcre2::utils::toString((char*)(subject + ovector[2*n]));
            //~ //String key=tmps1.substr(0,name_entry_size - 3);
            //~ value1=tmps2.substr(0,(ovector[2*n+1] - ovector[2*n]));
            PCRE2_UCHAR **bufferptr;
            PCRE2_SIZE bufflen/*=REGEX_STRING_MAX*/;
            pcre2_substring_length_byname(match_data,(PCRE2_SPTR)key.c_str(),&bufflen);
            bufferptr=(PCRE2_UCHAR **)malloc((bufflen+1) * sizeof(PCRE2_UCHAR));
            int ret=pcre2_substring_get_byname(match_data, (PCRE2_SPTR)key.c_str(), bufferptr, &bufflen);
            if(ret<0){
                switch(ret){
                    case PCRE2_ERROR_NOMEMORY: throw(ret);break;
                    default:break;   ///Other errors should be ignored
                }
            }
            value=jpcre2::utils::toString((char *)*bufferptr);
            
            ///Let's get the value again, this time with number
            ///We will match this value with the previous
            ///If it matches, then we got the right one,
            ///Otherwise the number is not valid for the corresponding name
            ///we will skip this iteration, if that happens.
            ///Don't use pcre2_substring_number_from_name() to get the number for the name (It's messy).
            ::free(bufferptr);
            pcre2_substring_length_bynumber(match_data,(uint32_t)n,&bufflen);
            bufferptr=(PCRE2_UCHAR **)malloc(bufflen * sizeof(PCRE2_UCHAR));
            ret=pcre2_substring_get_bynumber(match_data, (uint32_t)n, bufferptr, &bufflen);
            if(ret<0){
                switch(ret){
                    case PCRE2_ERROR_NOMEMORY: throw(ret);break;
                    default:break;   ///Other errors should be ignored
                }
            }
            value1=jpcre2::utils::toString((char *)*bufferptr);
            
            ///pcre2_substring_free(*bufferptr); 
            ///must free memory, pcre2_substring_free() yields to segmentation fault in several cases ( try '(?<name>\d)?' )
            /// (may be a bug?)
            ///Instead use free() to free the memory
            ::free(bufferptr);                  ///must free memory
            if(value!=value1){tabptr += name_entry_size;continue;}
            nas_map0[key]=value;
            nn_map0[key]=n;
            tabptr += name_entry_size;
        }
    }
    
    jpcre2::Uint jpcre2::RegexMatch::match(const std::string& s,VecNum& vec_num,VecNas& vec_nas,VecNtN& vec_nn,
                                            const std::string& mod,uint32_t opt_bits,uint32_t pcre2_opts){
        
        //Clear all verctors
        vec_num.clear();
        vec_nas.clear();
        vec_nn.clear();
        
        // If code is null, there's no need to proceed any further
        if(re->null_code) return vec_num.size();
        
        ///Add opt_bits to jpcre2_match_opts before running parseMatchOpts()
        ///This may require additional filter in future
        jpcre2_match_opts |= opt_bits;
        ///Add pcre2_opts to match_opts
        match_opts |= pcre2_opts;
        
        ///Make additions to available options
        parseMatchOpts(mod);
        
        
        PCRE2_SPTR subject=(PCRE2_SPTR)s.c_str();
        MapNum num_map0;
        MapNas nas_map0;
        MapNtN nn_map0;
        PCRE2_SPTR name_table;
        int crlf_is_newline;
        int namecount;
        int name_entry_size;
        int rc;
        int utf8;
        uint32_t option_bits;
        uint32_t newline;
        PCRE2_SIZE *ovector;
        size_t subject_length;
        pcre2_match_data *match_data;
        subject_length = strlen((char *)subject);
    
    
        /* Using this function ensures that the block is exactly the right size for
        the number of capturing parentheses in the pattern. */
    
        match_data = pcre2_match_data_create_from_pattern(re->code, NULL);
    
        rc = pcre2_match(
            re->code,                 /* the compiled pattern */
            subject,              /* the subject string */
            subject_length,       /* the length of the subject */
            0,                    /* start at offset 0 in the subject */
            match_opts,                    /* default options */
            match_data,           /* block for storing the result */
            NULL);                /* use default match context */
    
            /* Matching failed: handle error cases */
    
        re->error_code=rc;
        re->error_offset=rc;
        
        if (rc < 0){
            pcre2_match_data_free(match_data);      /* Release memory used for the match */
            //pcre2_code_free(code);                //must not free code. This function has no right to modify regex
            switch(rc){
                case PCRE2_ERROR_NOMATCH: return vec_num.size(); break;
                /*
                Handle other special cases if you like
                */
                default: throw(rc); break;
            }
            return vec_num.size();
        }
    
        /* Match succeded. Get a pointer to the output vector, where string offsets are
        stored. */
    
        ovector = pcre2_get_ovector_pointer(match_data);
    
    
        /*************************************************************************
        * We have found the first match within the subject string. If the output *
        * vector wasn't big enough, say so. Then output any substrings that were *
        * captured.                                                              *
        *************************************************************************/
    
        /* The output vector wasn't big enough. This should not happen, because we used
        pcre2_match_data_create_from_pattern() above. */
    
        if (rc == 0){
            //ovector was not big enough for all the captured substrings;
            return vec_num.size();
      
        }
    
        ///Let's get the numbered substrings
        getNumberedSubstrings(rc,match_data,num_map0);
        
        
        
    
        /**************************************************************************
        * That concludes the basic part of this demonstration program. We have    *
        * compiled a pattern, and performed a single match. The code that follows *
        * shows first how to access named substrings, and then how to code for    *
        * repeated matches on the same subject.                                   *
        **************************************************************************/
    
        /* See if there are any named substrings, and if so, show them by name. First
        we have to extract the count of named parentheses from the pattern. */
    
        (void)pcre2_pattern_info(
        re->code,                       /* the compiled pattern */
        PCRE2_INFO_NAMECOUNT,       /* get the number of named substrings */
        &namecount);                /* where to put the answer */
    
        if (namecount <= 0);        /*No named substrings*/
        
        else{
            PCRE2_SPTR tabptr;
            
            /* Before we can access the substrings, we must extract the table for
            translating names to numbers, and the size of each entry in the table. */
    
            (void)pcre2_pattern_info(
            re->code,                           /* the compiled pattern */
            PCRE2_INFO_NAMETABLE,           /* address of the table */
            &name_table);                   /* where to put the answer */
    
            (void)pcre2_pattern_info(
            re->code,                           /* the compiled pattern */
            PCRE2_INFO_NAMEENTRYSIZE,       /* size of each entry in the table */
            &name_entry_size);              /* where to put the answer */
    
            /* Now we can scan the table and, for each entry, print the number, the name,
            and the substring itself. In the 8-bit library the number is held in two
            bytes, most significant first. */
    
            tabptr = name_table;
            
            ///Let's get the named substrings
            getNamedSubstrings(namecount,name_entry_size,tabptr,match_data,nas_map0,nn_map0);
            
        }
        
        
        ///populate vector
        vec_num.push_back(num_map0);
        vec_nas.push_back(nas_map0);
        vec_nn.push_back(nn_map0);
    
        /*************************************************************************
        * If the "-g" option was given on the command line, we want to continue  *
        * to search for additional matches in the subject string, in a similar   *
        * way to the /g option in Perl. This turns out to be trickier than you   *
        * might think because of the possibility of matching an empty string.    *
        * What happens is as follows:                                            *
        *                                                                        *
        * If the previous match was NOT for an empty string, we can just start   *
        * the next match at the end of the previous one.                         *
        *                                                                        *
        * If the previous match WAS for an empty string, we can't do that, as it *
        * would lead to an infinite loop. Instead, a call of pcre2_match() is    *
        * made with the PCRE2_NOTEMPTY_ATSTART and PCRE2_ANCHORED flags set. The *
        * first of these tells PCRE2 that an empty string at the start of the    *
        * subject is not a valid match; other possibilities must be tried. The   *
        * second flag restricts PCRE2 to one match attempt at the initial string *
        * position. If this match succeeds, an alternative to the empty string   *
        * match has been found, and we can print it and proceed round the loop,  *
        * advancing by the length of whatever was found. If this match does not  *
        * succeed, we still stay in the loop, advancing by just one character.   *
        * In UTF-8 mode, which can be set by (*UTF) in the pattern, this may be  *
        * more than one byte.                                                    *
        *                                                                        *
        * However, there is a complication concerned with newlines. When the     *
        * newline convention is such that CRLF is a valid newline, we must       *
        * advance by two characters rather than one. The newline convention can  *
        * be set in the regex by (*CR), etc.; if not, we must find the default.  *
        *************************************************************************/
    
        if ((jpcre2_match_opts & FIND_ALL) == 0){
            pcre2_match_data_free(match_data);      /* Release the memory that was used */
            //pcre2_code_free(re);                  /// Don't do this. This function has no right to modify regex.
            return vec_num.size();                           /* Exit the program. */
        }
    
        /* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
        sequence. First, find the options with which the regex was compiled and extract
        the UTF state. */
    
        (void)pcre2_pattern_info(re->code, PCRE2_INFO_ALLOPTIONS, &option_bits);
        utf8 = (option_bits & PCRE2_UTF) != 0;
        
        /* Now find the newline convention and see whether CRLF is a valid newline
        sequence. */
        
        (void)pcre2_pattern_info(re->code, PCRE2_INFO_NEWLINE, &newline);
        crlf_is_newline = newline == PCRE2_NEWLINE_ANY ||
                          newline == PCRE2_NEWLINE_CRLF ||
                          newline == PCRE2_NEWLINE_ANYCRLF;
        
        /* Loop for second and subsequent matches */
    
        for (;;){
            num_map0.clear();                         ///must clear map before filling it with new values
            nas_map0.clear();
            nn_map0.clear();
            uint32_t options = match_opts;                       /* Normally no options */
            PCRE2_SIZE start_offset = ovector[1];       /* Start at end of previous match */
            
            /* If the previous match was for an empty string, we are finished if we are
            at the end of the subject. Otherwise, arrange to run another match at the
            same point to see if a non-empty match can be found. */
            
            if (ovector[0] == ovector[1]){
                if (ovector[0] == subject_length) break;
                options |= PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
                }
            
              /* Run the next matching operation */
            
            rc = pcre2_match(
                re->code,                 /* the compiled pattern */
                subject,              /* the subject string */
                subject_length,       /* the length of the subject */
                start_offset,         /* starting offset in the subject */
                options,              /* options */
                match_data,           /* block for storing the result */
                NULL);                /* use default match context */
            
            /* This time, a result of NOMATCH isn't an error. If the value in "options"
            is zero, it just means we have found all possible matches, so the loop ends.
            Otherwise, it means we have failed to find a non-empty-string match at a
            point where there was a previous empty-string match. In this case, we do what
            Perl does: advance the matching position by one character, and continue. We
            do this by setting the "end of previous match" offset, because that is picked
            up at the top of the loop as the point at which to start again.
            
            There are two complications: (a) When CRLF is a valid newline sequence, and
            the current position is just before it, advance by an extra byte. (b)
            Otherwise we must ensure that we skip an entire UTF character if we are in
            UTF mode. */
            
            if (rc == PCRE2_ERROR_NOMATCH){
                if (options == 0) break;                                    /* All matches found */
                ovector[1] = start_offset + 1;                              /* Advance one code unit */
                if (crlf_is_newline &&                                      /* If CRLF is newline & */
                    start_offset < subject_length - 1 &&                    /* we are at CRLF, */
                    subject[start_offset] == '\r' &&
                    subject[start_offset + 1] == '\n')
                    ovector[1] += 1;                                        /* Advance by one more. */
                else if (utf8){                                             /* advance a whole UTF-8 */
                    while (ovector[1] < subject_length){
                        if ((subject[ovector[1]] & 0xc0) != 0x80) break;
                        ovector[1] += 1;
                    }
                }
                continue;    /* Go round the loop again */
            }
            
              /* Other matching errors are not recoverable. */
            
            if (rc < 0){
                pcre2_match_data_free(match_data);
                //pcre2_code_free(code);           //must not do this. This function has no right to modify regex.
                return vec_num.size();
            }
            
            
            
            if (rc == 0){
                /* The match succeeded, but the output vector wasn't big enough. This
                should not happen. */
                return vec_num.size();
            }
            
            /* As before, get substrings stored in the output vector by number, and then
            also any named substrings. */
            
            ///Let's get the numbered substrings
            getNumberedSubstrings(rc,match_data,num_map0);
            
            if (namecount <= 0);  /*No named substrings*/
            else{
                PCRE2_SPTR tabptr = name_table;
                
                ///Let's get the named substrings
                getNamedSubstrings(namecount,name_entry_size,tabptr,match_data,nas_map0,nn_map0);
            }
            
            
            ///populate vector
            vec_num.push_back(num_map0);
            vec_nas.push_back(nas_map0);
            vec_nn.push_back(nn_map0);
            
        }      /* End of loop to find second and subsequent matches */
    
        pcre2_match_data_free(match_data);
        /// Must not free pcre2_code* code. This function has no right to modify regex.
        return vec_num.size();
    }
