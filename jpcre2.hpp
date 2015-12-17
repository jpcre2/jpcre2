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

#include <string>
#include <cstring>
#include <sstream>
#include <limits>
#include <vector>
#include <map>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


namespace jpcre2_utils{
    template<typename T>
    std::string toString(T a){
        std::stringstream ss;
        ss <<a;
        return ss.str();
    }
}

namespace jpcre2{

    #define REGEX_STRING_MAX std::numeric_limits<int>::max()
    #define DEFAULT_LOCALE "none"
    
    //~ typedef std::vector<std::map<std::string,std::string>> MatchVector;
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
        
        ~Pcre2Regex(){free();}
        
        void parseReplacementOpts(const std::string& mod);
        void parseCompileOpts(const std::string& mod);
        void parseOpts(const std::string& mod){parseReplacementOpts(mod);parseCompileOpts(mod);}
        
        std::string getModifier(){return modifier;}
        void setModifier(const std::string& mod){modifier=mod;}
        
        std::string getPattern(){return pat_str;}
        void setPattern(const std::string& pat){pat_str=pat;}
        
        void setLocale(const std::string& loc){mylocale=loc;}
        std::string getLocale(){return mylocale;}
        
        void compile(void){compile(pat_str,modifier,mylocale);}
        void compile(const std::string& re,const std::string& mod="", const std::string& loc=DEFAULT_LOCALE);
        
        std::string replace( std::string mains, std::string repl,const std::string& mod="",PCRE2_SIZE out_size=REGEX_STRING_MAX);
        std::string replace( std::string mains, std::string repl,size_t out_size){return replace(mains,repl,"",out_size);}
        
        pcre2_code* getPcreCode(){return code;}
        void free(void){pcre2_code_free(code);}
        
        bool match(const std::string& subject,VecNum& vec_num,VecNas& vec_nas,VecNtN& vec_nn,bool find_all=false);
        
        std::string getErrorMessage(int err_num);
        std::string getErrorMessage();
        std::string getWarningMessage(){return current_warning_msg;}
        int getErrorNumber(){return error_number;}
        int getErrorCode(){return error_code;}
        PCRE2_SIZE getErrorOffset(){return error_offset;}
        
        
    };
    
    std::string Pcre2Regex::getErrorMessage(){
        return getErrorMessage(error_code);
    }
    
    std::string Pcre2Regex::getErrorMessage(int err_num){
        PCRE2_UCHAR buffer[4024];
        pcre2_get_error_message(err_num, buffer, sizeof(buffer));
        return jpcre2_utils::toString((PCRE2_UCHAR*)buffer)+" at offset: "+jpcre2_utils::toString((int)error_offset);
    }
    
    
    void Pcre2Regex::parseReplacementOpts(const std::string& mod){
        action_opts=PCRE2_SUBSTITUTE_OVERFLOW_LENGTH;
            ///parse pcre options
        for(int i=0;i<(int)mod.length();i++){
            switch (mod[i]){
                case 'e': action_opts  |= PCRE2_SUBSTITUTE_UNSET_EMPTY;break;
                case 'E': action_opts  |= PCRE2_SUBSTITUTE_UNKNOWN_UNSET | PCRE2_SUBSTITUTE_UNSET_EMPTY;break;
                case 'g': action_opts  |= PCRE2_SUBSTITUTE_GLOBAL;break;
                case 'x': action_opts  |= PCRE2_SUBSTITUTE_EXTENDED;break;
                default : break;
            }
        }
    }
    
    
    void Pcre2Regex::parseCompileOpts(const std::string& mod){
        compile_opts=0;
        opt_jit_compile=false;
        jit_opts=PCRE2_JIT_COMPLETE;
        
        ///default options
        compile_opts |=  PCRE2_ALT_BSUX;              //\u \U \x will act as javascript standard
        
        ///parse pcre options
        for(int i=0;i<(int)mod.length();i++){
            switch (mod[i]){
                case 'e': compile_opts |= PCRE2_MATCH_UNSET_BACKREF;break;
                case 'g': action_opts  |= PCRE2_SUBSTITUTE_GLOBAL;break;
                case 'i': compile_opts |= PCRE2_CASELESS;break;
                case 'm': compile_opts |= PCRE2_MULTILINE;break;
                case 's': compile_opts |= PCRE2_DOTALL;break;
                case 'u': compile_opts |= PCRE2_UTF;compile_opts |= PCRE2_UCP;break;
                case 'x': compile_opts |= PCRE2_EXTENDED;break;
                case 'A': compile_opts |= PCRE2_ANCHORED;action_opts |= PCRE2_ANCHORED;break;
                case 'D': compile_opts |= PCRE2_DOLLAR_ENDONLY;break;
                case 'J': compile_opts |= PCRE2_DUPNAMES;break;
                case 'S': opt_jit_compile=true;jit_opts |= PCRE2_JIT_COMPLETE;break;//optimization opt this should be default
                case 'U': compile_opts |= PCRE2_UNGREEDY;break;
                default : break;
            }
        }
    }
    
    
    void Pcre2Regex :: compile(const std::string& re,const std::string& mod, const std::string& loc){
        pattern=(PCRE2_SPTR)re.c_str();
        mylocale=loc;
        
        ///populate class compile_opts actions opts etc...
        parseCompileOpts(mod);
        
        ///populate some class vars
        pat_str=re;
        modifier=mod;
    
    /*************************************************************************
    * Now we are going to compile the regular expression pattern, and handle *
    * any errors that are detected.                                          *
    *************************************************************************/
    
        pcre2_compile_context *ccontext = pcre2_compile_context_create(NULL);
        
        if(loc!="none"){
            std::string loc_old;
            loc_old=jpcre2_utils::toString(std::setlocale(LC_CTYPE,loc.c_str()));
            const unsigned char *tables = pcre2_maketables(NULL);
            pcre2_set_character_tables(ccontext, tables);
            std::setlocale(LC_CTYPE,loc_old.c_str());
        }
    
    
        code = pcre2_compile(
            pattern,                    /* the pattern */
            PCRE2_ZERO_TERMINATED,      /* indicates pattern is zero-terminated */
            compile_opts,               /* default options */
            &error_number,              /* for error number */
            &error_offset,              /* for error offset */
            ccontext);                  /* use compile context */
        
        error_code=error_number;
        /* Compilation failed: print the error message and exit. */
    
        if (code == NULL){
            free();
            throw(getErrorMessage());
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
    
    
    std::string Pcre2Regex :: replace( std::string mains, std::string repl,const std::string& mod,PCRE2_SIZE out_size){
        
        parseReplacementOpts(mod);
        PCRE2_SPTR subject = (PCRE2_SPTR)mains.c_str();
        PCRE2_SIZE subject_length = strlen((char *)subject);
        PCRE2_SPTR replace = (PCRE2_SPTR)repl.c_str();
        PCRE2_SIZE replace_length = strlen((char *)replace);
        PCRE2_SIZE outlengthptr=(PCRE2_SIZE)out_size;
        
        PCRE2_UCHAR* output_buffer;
        output_buffer=(PCRE2_UCHAR*)malloc(outlengthptr * sizeof(PCRE2_UCHAR));
        int ret=pcre2_substitute(
            code,                              /*Points to the compiled pattern*/
            subject,                           /*Points to the subject string*/
            subject_length,                    /*Length of the subject string*/
            0,                                 /*Offset in the subject at which to start matching*/
            action_opts,                       /*Option bits*/
            0,                                 /*Points to a match data block, or is NULL*/
            0,                                 /*Points to a match context, or is NULL*/
            replace,                           /*Points to the replacement string*/
            replace_length,                    /*Length of the replacement string*/
            output_buffer,                     /*Points to the output buffer*/
            &outlengthptr                      /*Points to the length of the output buffer*/
        );
        error_code=ret;
        error_offset=ret;
        
        if (ret < 0){
            /// Second retry in case output buffer was not big enough
            outlengthptr++;  /// It was changed to required length
            output_buffer=(PCRE2_UCHAR*)realloc(output_buffer,outlengthptr * sizeof(PCRE2_UCHAR));
            if((action_opts & PCRE2_SUBSTITUTE_OVERFLOW_LENGTH) !=0 && ret == (int)PCRE2_ERROR_NOMEMORY){
                ret=pcre2_substitute(
                code,                              /*Points to the compiled pattern*/
                subject,                           /*Points to the subject string*/
                subject_length,                    /*Length of the subject string*/
                0,                                 /*Offset in the subject at which to start matching*/
                action_opts,                       /*Option bits*/
                0,                                 /*Points to a match data block, or is NULL*/
                0,                                 /*Points to a match context, or is NULL*/
                replace,                           /*Points to the replacement string*/
                replace_length,                    /*Length of the replacement string*/
                output_buffer,                     /*Points to the output buffer*/
                &outlengthptr                      /*Points to the length of the output buffer*/
                );
                error_offset=(int)ret;
                error_code=ret;
                
                if(ret<0){throw(getErrorMessage(ret));}
                
            }
            else throw(getErrorMessage(ret));
        }
        std::string result=jpcre2_utils::toString((char*)output_buffer);
        std::free(output_buffer);
        return result;
    }
    
    
    bool Pcre2Regex::match(const std::string& s,VecNum& vec_num,VecNas& vec_nas,VecNtN& vec_nn,bool find_all){
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
    
        match_data = pcre2_match_data_create_from_pattern(code, NULL);
    
        rc = pcre2_match(
            code,                 /* the compiled pattern */
            subject,              /* the subject string */
            subject_length,       /* the length of the subject */
            0,                    /* start at offset 0 in the subject */
            0,                    /* default options */
            match_data,           /* block for storing the result */
            NULL);                /* use default match context */
    
            /* Matching failed: handle error cases */
    
        error_code=rc;
        error_offset=rc;
        
        if (rc < 0){
            pcre2_match_data_free(match_data);      /* Release memory used for the match */
            pcre2_code_free(code);                  /* data and the compiled pattern. */
            switch(rc){
                case PCRE2_ERROR_NOMATCH: return false; break;
                /*
                Handle other special cases if you like
                */
                default: throw(getErrorMessage(rc)); break;
            }
            return false;
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
            return false;
      
        }
    
    
        for (int i = 0; i < rc; i++){
            //~ PCRE2_SPTR substring_start = subject + ovector[2*i];
            //~ size_t substring_length = ovector[2*i+1] - ovector[2*i];
            //~ std::string tmps1=jpcre2_utils::toString((char *)substring_start);
            //~ std::string key=std::to_string(i);
            std::string value;
            PCRE2_UCHAR **bufferptr;
            PCRE2_SIZE bufflen=REGEX_STRING_MAX;
            bufferptr=(PCRE2_UCHAR**)malloc(bufflen * sizeof(PCRE2_UCHAR));
            pcre2_substring_get_bynumber(match_data, (uint32_t)i, bufferptr, &bufflen);
            value=jpcre2_utils::toString((char*)*bufferptr);
            std::free(bufferptr);
            num_map0[i]=value;
        }
        
        
        
    
        /**************************************************************************
        * That concludes the basic part of this demonstration program. We have    *
        * compiled a pattern, and performed a single match. The code that follows *
        * shows first how to access named substrings, and then how to code for    *
        * repeated matches on the same subject.                                   *
        **************************************************************************/
    
        /* See if there are any named substrings, and if so, show them by name. First
        we have to extract the count of named parentheses from the pattern. */
    
        (void)pcre2_pattern_info(
        code,                       /* the compiled pattern */
        PCRE2_INFO_NAMECOUNT,       /* get the number of named substrings */
        &namecount);                /* where to put the answer */
    
        if (namecount <= 0);        /*No named substrings*/
        
        else{
            PCRE2_SPTR tabptr;
            
            /* Before we can access the substrings, we must extract the table for
            translating names to numbers, and the size of each entry in the table. */
    
            (void)pcre2_pattern_info(
            code,                           /* the compiled pattern */
            PCRE2_INFO_NAMETABLE,           /* address of the table */
            &name_table);                   /* where to put the answer */
    
            (void)pcre2_pattern_info(
            code,                           /* the compiled pattern */
            PCRE2_INFO_NAMEENTRYSIZE,       /* size of each entry in the table */
            &name_entry_size);              /* where to put the answer */
    
            /* Now we can scan the table and, for each entry, print the number, the name,
            and the substring itself. In the 8-bit library the number is held in two
            bytes, most significant first. */
    
            tabptr = name_table;
            for (int i = 0; i < namecount; i++){
                int n = (tabptr[0] << 8) | tabptr[1];
                std::string tmps1=jpcre2_utils::toString((char*)(tabptr+2));
                std::string tmps2=jpcre2_utils::toString((char*)(subject + ovector[2*n]));
                std::string key=tmps1.substr(0,name_entry_size - 3);
                std::string value,value1;
                value1=tmps2.substr(0,(ovector[2*n+1] - ovector[2*n]));
                PCRE2_UCHAR **bufferptr;
                PCRE2_SIZE bufflen=REGEX_STRING_MAX;
                bufferptr=(PCRE2_UCHAR **)malloc(bufflen * sizeof(PCRE2_UCHAR));
                pcre2_substring_get_byname(match_data, (PCRE2_SPTR)key.c_str(), bufferptr, &bufflen);
                value=jpcre2_utils::toString((char *)*bufferptr);
                std::free(bufferptr);                  ///must free memory
                if(value!=value1){tabptr += name_entry_size;continue;}
                nas_map0[key]=value;
                nn_map0[key]=n;
                tabptr += name_entry_size;
            }
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
    
        if (!find_all){
            pcre2_match_data_free(match_data);      /* Release the memory that was used */
            //pcre2_code_free(re);                  /// Don't do this. This function has no right to modify regex.
            return false;                           /* Exit the program. */
        }
    
        /* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
        sequence. First, find the options with which the regex was compiled and extract
        the UTF state. */
    
        (void)pcre2_pattern_info(code, PCRE2_INFO_ALLOPTIONS, &option_bits);
        utf8 = (option_bits & PCRE2_UTF) != 0;
        
        /* Now find the newline convention and see whether CRLF is a valid newline
        sequence. */
        
        (void)pcre2_pattern_info(code, PCRE2_INFO_NEWLINE, &newline);
        crlf_is_newline = newline == PCRE2_NEWLINE_ANY ||
                          newline == PCRE2_NEWLINE_CRLF ||
                          newline == PCRE2_NEWLINE_ANYCRLF;
        
        /* Loop for second and subsequent matches */
    
        for (;;){
            num_map0.clear();                         ///must clear map before filling it with new values
            nas_map0.clear();
            nn_map0.clear();
            uint32_t options = 0;                       /* Normally no options */
            PCRE2_SIZE start_offset = ovector[1];       /* Start at end of previous match */
            
            /* If the previous match was for an empty string, we are finished if we are
            at the end of the subject. Otherwise, arrange to run another match at the
            same point to see if a non-empty match can be found. */
            
            if (ovector[0] == ovector[1]){
                if (ovector[0] == subject_length) break;
                options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
                }
            
              /* Run the next matching operation */
            
            rc = pcre2_match(
                code,                 /* the compiled pattern */
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
                return false;
            }
            
            
            
            if (rc == 0){
                /* The match succeeded, but the output vector wasn't big enough. This
                should not happen. */
                return false;
            }
            
            /* As before, get substrings stored in the output vector by number, and then
            also any named substrings. */
            
            for (int i = 0; i < rc; i++){
                //~ PCRE2_SPTR substring_start = subject + ovector[2*i];
                //~ size_t substring_length = ovector[2*i+1] - ovector[2*i];
                //~ std::string tmps=toString((char *)(substring_start));
                //~ std::string key=std::to_string(i);
                std::string value;
                PCRE2_UCHAR **bufferptr;
                PCRE2_SIZE bufflen=REGEX_STRING_MAX;
                bufferptr=(PCRE2_UCHAR**)malloc(bufflen * sizeof(PCRE2_UCHAR));
                pcre2_substring_get_bynumber(match_data, (uint32_t)i, bufferptr, &bufflen);
                value=jpcre2_utils::toString((char*)*bufferptr);
                std::free(bufferptr);                  ///must free memory
                num_map0[i]=value;
            }
            
            if (namecount <= 0);  /*No named substrings*/
            else{
                PCRE2_SPTR tabptr = name_table;
                for (int i = 0; i < namecount; i++){
                    int n = (tabptr[0] << 8) | tabptr[1];
                    std::string tmps1=jpcre2_utils::toString((char*)(tabptr+2));
                    std::string tmps2=jpcre2_utils::toString((char*)(subject + ovector[2*n]));
                    std::string key=tmps1.substr(0,name_entry_size - 3);
                    std::string value,value1;
                    value1=tmps2.substr(0,(ovector[2*n+1] - ovector[2*n]));
                    PCRE2_UCHAR **bufferptr;
                    PCRE2_SIZE bufflen=REGEX_STRING_MAX;
                    bufferptr=(PCRE2_UCHAR **)malloc(bufflen * sizeof(PCRE2_UCHAR));
                    pcre2_substring_get_byname(match_data, (PCRE2_SPTR)key.c_str(), bufferptr, &bufflen);
                    value=jpcre2_utils::toString((char *)*bufferptr);
                    std::free(bufferptr);                  ///must free memory
                    if(value!=value1){tabptr += name_entry_size;continue;}
                    nn_map0[key]=n;
                    nas_map0[key]=value;
                    tabptr += name_entry_size;
                }
            }
            
            
            ///populate vector
            vec_num.push_back(num_map0);
            vec_nas.push_back(nas_map0);
            vec_nn.push_back(nn_map0);
            
        }      /* End of loop to find second and subsequent matches */
    
        pcre2_match_data_free(match_data);
        /// Must not free pcre2_code* code. This function has no right to modify regex.
        return true;
    }

} ///jpcre2 namespace






#endif
