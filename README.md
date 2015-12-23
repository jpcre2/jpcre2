<div id="description"></div>
PCRE2 is the name used for a revised API for the PCRE library, which is a set of functions, written in C, that implement regular expression pattern matching using the same syntax and semantics as Perl, with just a few differences. Some features that appeared in Python and the original PCRE before they appeared in Perl are also available using the Python syntax.

This provides some C++ wrapper functions to provide some useful utilities like regex match and regex replace.

<div id="requires"></div>
#Requirements:

1. C++ compiler with C++11 support (optional but recommended).
2. pcre2 library (`version >=10.21`).

If the required `pcre2` version is not available in the official channel, download <a href="https://github.com/jpcre2">my fork of the library from here</a>, Or use <a href="https://github.com/jpcre2/pcre2">this repository</a> which will always be kept compatible with `jpcre2`.

<div id="how-to"></div>
#How To:

##How to compile:

###Without installing it as a library:

1. Include `jpcre2.h` and `jpcre2.cpp` in your project and `#include` the header in your code.

2. Compile with `pcre2` library linked.

**Example:**

A *mycpp.cpp* file can be compiled with the following command with GCC.

```sh
g++ mycpp.cpp jpcre2.cpp jpcre2.h -lpcre2-8
```

`-lpcre2-8` should be changed to the actual library i.e for 16 bit code unit: `-lpcre2-16` and for 32 bit code unit: `-lpcre2-32`.

If your `pcre2` library is not in the standard library path, then add the path:

```sh
g++ -std=c++11 mycpp.cpp -L/path/to/your/pcre2/library -lpcre2-8
```

**Note that** it requires the `pcre2` library installed in your system. If it is not already installed and linked in your compiler, you will need to link it with appropriate path and options.

###By installing jpcre2 as a library:

1. Install it with `./configure`, `make`, `sudo make install`.
2. `#include <jpcre2.h>` in your code.
3. Build/compile by linking with jpcre2 library.

An example command for GCC would be:

```sh
g++  mycpp.cpp -ljpcre2 -lpcre2-8
```

##How to code:

<ol>
<li>
First create a <code>jpcre2::Regex</code> object. This object will hold the pattern, modifiers, compiled pattern, error and warning codes.
<pre class="highlight"><code class="highlight-source-c++ cpp">jpcre2::Regex re;   //Create object</code></pre>
Each object for each regex pattern.
</li>
<li>
Compile the pattern and catch any error exception:
<pre class="highlight"><code class="highlight-source-c++ cpp">
try{
    re.compile("pattern","mgi");          //This compiles the pattern and modifier provided.
    
    //Another way is to use constructor to initialize and compile at the same time:
    jpcre2::Regex re2("pattern2","mSi");  //S is an optimization mod. Try to use it always.
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}
</code></pre>
</li>
<li>
Now you can perform match or replace against the pattern. Use the <code>match()</code> member function to preform regex match and the <code>replace()</code> member function to perform regex replace.
</li>
  <ol>
<li>
<b>Match:</b> The <code>match()</code> member function takes the subject string and some specialized vectors (vectors of maps of substrings) as its arguments and a last argument to tell whether to match all or only the first. It puts the results in the maps of the vectors and returns true on successful match and false otherwise.
</li>
    <ul>
<li>
Perform match and catch any error exception:
<pre class="highlight"><code class="highlight-source-c++ cpp">
try{
    re.match("I am a subject string",vec_num);
    //vec_num will be populated with numbered substrings.
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}
</code></pre>
Access the substrings like this:
<pre class="highlight"><code class="highlight-source-c++ cpp">
for(int i=0;i&lt;(int)vec_num.size();i++){
    //This loop will iterate only once if find_all is false.
    //i=0 is the first match found, i=1 is the second and so forth
    for(auto const&amp; ent : vec_num[i]){
        //ent.first is the number/position of substring found
        //ent.second is the substring itself
        //when ent.first is 0, ent.second is the total match.
        //for(auto const&amp; ent : vec_num[i]):C++11 feature
        //If you want &lt;C++11 method, see pcre2match.cpp.
    }
}
</code></pre>
</li>
<li>
Other variations of this function can be used to get named substrings and the position of named substrings. Simply pass the appropriate vectors in the match function:
<pre class="highlight"><code class="highlight-source-c++ cpp">
try{
    re.match("I am a subject string",vec_num,vec_nas,vec_nn);
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}
</code></pre>
And access the substrings by looping through the vectors and associated maps. The size of all three vectors are the same and they can be passed in any sequence (i.e the order of the vectors as arguments is not important).
</li>
    </ul>
<li>
<b>Replace:</b> The <code>replace()</code> member function takes the subject string as first argument and replacement string as the second argument and two optional arguments (modifier and the size of the resultant string) and returns the resultant string after performing the replacement operation. If no modifier is passed an empty modifier is assumed.
</li>
    <ul>
<li>
Perform replace and catch any error exception:
<pre class="highlight prettyprint"><code class="highlight-source-c++ cpp">
try{
    std::cout&lt;&lt;re.replace("replace this string according to the pattern","with this string","mgi")&lt;&lt;std::endl;
    //mgi is the modifier passed (multiline, global, case insensitive).
    //Access substrings/captured groups with ${1234},$1234 (for numbered substrings)
    // or ${name} (for named substrings) in the replacement part
}
catch(int e){
    /*Handle error*/
    std::cout&lt;&lt;re.getErrorMessage(e)&lt;&lt;std::endl;
}

</code></pre>
</li>
<li>
If you pass the size of the resultant string with the replace function, then make sure it will be enough to store the whole resultant replaced string, otherwise the internal replace function (<code>pcre2_substitute()</code>) will be called <i>twice</i> to adjust the size to hold the whole resultant string in order to avoid <code>PCRE2_ERROR_NOMEMORY</code> error. Two consecutive call of the same function may affect overall performance of your code.
</li>
    </ul>
  </ol>
</ol>

#Insight:

##Namespaces:

1. **jpcre2_utils :** Some utility functions used by `jpcre2`.
2. **jpcre2 :** This is the namespace you will be using in your code to access `jpcre2` classes and functions.

##Classes:

1. **Regex :** This is the main class which holds the key utilities of `jpcre2`. Every regex needs an object of this class.

##Functions:

```cpp
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
```


#Testing:

1. **test.cpp**: Contains an example code for match and replace function.
2. **test_match.cpp**: Contains an example code for match function.
3. **test_replace.cpp**: Contains an example code for replace function.
4. **jpcre2match**: Another matching example.
5. **jpcre2replace**: Another replacement example.

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

