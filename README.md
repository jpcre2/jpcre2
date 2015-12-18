#C++ wrapper for several utilities of PCRE2 Library

PCRE2 is the name used for a revised API for the PCRE library, which is a set of functions, written in C, that implement regular expression pattern matching using the same syntax and semantics as Perl, with just a few differences. Some features that appeared in Python and the original PCRE before they appeared in Perl are also available using the Python syntax.

This provides some C++ wrapper functions to provide some useful utilities like regex match and regex replace.

#Requirements:

1. C++ compiler with C++11 support.
2. pcre2 library.

#How To:

Just `#include` the `jpcre2.hpp` file in your program and you are done. 

Note that it requires the `pcre2` library installed in your system. If it is not already installed and linked in your compiler, you will need to link it with appropriate path and options.

In general case the compile command with `gcc` would be:

```
g++ -std=c++11 mycpp.cpp -lpcre2-8
```

If your pcre2 library is not in the standard library path, then add the path:

```
g++ -std=c++11 mycpp.cpp -L/path/to/your/pcre2/library -lpcre2-8
```

#Insight:

##Namespaces:

1. jpcre2_utils
2. jpcre2

##Classes:

1. Pcre2Regex

##Functions:

```lang-c++
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

pcre2_code* getPcreCode(){return code;}
void free(void){pcre2_code_free(code);}

std::string replace( std::string mains, std::string repl,const std::string& mod="",PCRE2_SIZE out_size=REGEX_STRING_MAX);
std::string replace( std::string mains, std::string repl,size_t out_size){return replace(mains,repl,"",out_size);}

bool match(const std::string& subject,VecNum& vec_num,VecNas& vec_nas,VecNtN& vec_nn,bool find_all=false);

std::string getErrorMessage(int err_num);
std::string getErrorMessage();
std::string getWarningMessage(){return current_warning_msg;}
int getErrorNumber(){return error_number;}
int getErrorCode(){return error_code;}
PCRE2_SIZE getErrorOffset(){return error_offset;}
```


#Testing:

1. **test.cpp**: Contains an example code for match and replace function.
2. **test_match.cpp**: Contains an example code for match function.
3. **test_replace.cpp**: Contains an example code for replace function.

#Screenshots of some test outputs:

test_match:
----------

![jpcre2 screenshot1](img/s1_1.png)
![jpcre2 screenshot2](img/s1_2.png)
![jpcre2 screenshot3](img/s1_3.png)
![jpcre2 screenshot4](img/s1_4.png)

test_replace:
-------------

![jpcre2 screenshot5](img/s2_1.png)

