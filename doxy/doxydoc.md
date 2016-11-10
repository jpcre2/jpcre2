JPCRE2                         {#mainpage}
======

C++ wrapper for PCRE2 library

[![Build status image](https://travis-ci.org/jpcre2/jpcre2.svg?branch=release)](https://travis-ci.org/jpcre2/jpcre2/) [![doc percentage image](https://neurobin.org/img/badge/doc-percent2.svg)](http://docs.neurobin.org/jpcre2/index.html) [![CPP depends image](https://neurobin.org/img/badge/CPP-depends1.svg)](https://isocpp.org/) [![PCRE2 depends image](https://neurobin.org/img/badge/PCRE2-dep1.svg)](http://www.pcre.org/) 


> PCRE2 is the name used for a revised API for the PCRE library, which is a set of functions, written in C, that implement regular expression pattern matching using the same syntax and semantics as Perl, with just a few differences. Some features that appeared in Python and the original PCRE before they appeared in Perl are also available using the Python syntax.

This provides some C++ wrapper classes/functions to perform regex operations such as regex match and regex replace.

You can read the complete documentation [here](http://docs.neurobin.org/jpcre2/index.html) or download it from [jpcre2-doc repository](https://github.com/jpcre2/jpcre2-doc).


# Dependency {#dependency}

1. PCRE2 library (`version >=10.21`).


If the required PCRE2 version is not available in the official channel, you can download <a href="https://github.com/jpcre2/pcre2">my fork of the library</a>.


# Getting started {#getting-started}

This is a **header only** library. All you need to do is include the header `jpcre2.hpp` in your program.

```cpp
#include "jpcre2.hpp"
```

**Notes:** 

* `jpcre2.hpp` \#includes `pcre2.h`, thus you should not include `pcre2.h` manually in your program.
* There's no need to define `PCRE2_CODE_UNIT_WIDTH` before including `jpcre2.hpp`.
* On windows, if you are working with a static PCRE2 library, you must define `PCRE2_STATIC` before including `jpcre2.hpp`.

**Install:**

You can copy this header to a standard include directory (folder) so that it becomes available from a standard include path.

On Unix you can do:

```sh
./configure
make
make install #(may require root privilege)
```
This will compile some examples (for 8-bit) and install the header. For other tests or compile options see <a href="#the-configure-script">configure script</a>

**Compile/Build:**

Compile/Build your code with corresponding PCRE2 libraries linked. For 8-bit code unit width, you need to link with 8-bit library, for 16-bit, 16-bit library and so on. If you want to use multiple code unit width, link against all 8-bit, 16-bit and 32-bit libraries. See <a href="#code-unit-and-character-type">code unit width and character type</a> for details.


**Example compilation with g++:**

```cpp
g++ main.cpp -lpcre2-8
g++ multi.cpp -lpcre2-8 -lpcre2-16 -lpcre2-32
```
If PCRE2 is not installed in the standard path, add the path with `-L` option:

```cpp
g++ main.cpp -L/my/library/path -lpcre2-8
```

# Coding guide {#coding-guide}

Performing a match or replacement against regex pattern involves two steps: 

1. Compiling the pattern
2. Performing the match or replacement operation

## Compile a regex pattern {#compile-a-regex-pattern}

Select a character type according to the library you want to use. In this doc we are going to use 8 bit library as reference and we will use `char` as the character type. If `char` in your system is 16-bit you will have to link against 16-bit library instead, same goes for 32-bit. Other bit sizes are not supported by PCRE2.

Let's use a typedef to shorten the code:

```cpp
typedef jpcre2::select<char> jp;
// You have to select the basic data type (char, wchar_t, char16_t or char32_t)
```

### A Regex object {#a-regex-object}

(You can use temporary object too, see [short examples](#short-examples)).

This object will hold the pattern, options and compiled pattern.

```cpp
jp::Regex re;
```
Each object for each regex pattern.

### Compile the regex {#compile-the-regex}

```cpp
re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")  //set pattern
  .addModifier("iJ")                                                    //add modifier (J for PCRE2_DUPNAMES)
  .compile();                                                           //Finally compile it.
      
//Do not use setModifier() after adding any modifier/s, it will reset them.

//Another way is to use constructor to initialize and compile at the same time:
jp::Regex re2("pattern2","mSi");  //S is an optimization mod.
jp::Regex re3("pattern3", PCRE2_ANCHORED);
jp::Regex re4("pattern4", PCRE2_ANCHORED, jpcre2::JIT_COMPILE);

```

Now you can perform match or replace against the pattern. Use the `match()` member function to perform regex match and the `replace()` member function to perform regex replace.

### Check if regex compiled successfully {#check-regex}

You can check if the regex was compiled successfully or not, but it's not necessary. A match against a non-compiled regex will give you 0 match and for replace you will be returned the exact same subject string that you passed.

```cpp
if(!re) std::cout<<"Failed";
else std::cout<<"successfull";
```
The `if(re)` conditional is only available for `>= C++11`:

```cpp
if(re) std::cout<<"Success";
else std::cout<<"Failure";
```
For `< C++11`, you can use the double bang trick as an alternative to `if(re)`:

```cpp
if(!!re) std::cout<<"Success";
else std::cout<<"Failure";
```

## Match {#match}

The `jp::Regex::match(const String& s)` family of member functions can take two arguments (subject & modifier) and returns the number of matches found against the compiled pattern. What it actually does is create a `RegexMatch` object and forward all options to it, then finally call `jp::RegexMatch::match()` to perform the match. To get match results, you will need to pass vector pointers that will be filled with match data.

### Check if a string matches a regex {#check-if-a-string-matches-a-regex}

```cpp
jp::Regex re("\\w+ect");

if(re.match("I am the subject"))
    std::cout<<"matched (case sensitive)";
else
    std::cout<<"Didn't match";

//For case insensitive match, re-compile with modifier 'i'
re.addModifier("i").compile();

if(re.match("I am the subjEct"))
    std::cout<<"matched (case insensitive)";
else
    std::cout<<"Didn't match";
```

### Get match count {#simple-match-count}

```cpp
size_t count = jp::Regex("(\\d)|(\\w)","i").match("I am the subject","g");
```

### Get match result {#do-match}

To get the match results, you need to pass appropriate vector pointers. This is an example of how you can get the numbered substrings/captured groups from a match:


```cpp
jp::VecNum vec_num;
size_t count=re.initMatch()									//Initialize match object
			   .setSubject(subject)                         //set subject string
               .setModifier(ac_mod)                         //set modifier string
               .setNumberedSubstringVector(&vec_num)        //pass pointer to VecNum vector
               .match();                                    //Finally perform the match.
//vec_num will be populated with vectors of numbered substrings.
//count is the total number of matches found
```
### Access a captured group {#access-a-capture-group}

You can access a substring/captured group by specifying their index (position):

```cpp
std::cout<<vec_num[0][0]; // group 0 in first match
std::cout<<vec_num[0][1]; // group 1 in first match
std::cout<<vec_num[1][0]; // group 0 in second match
```
### Get named capture group {#get-named-capture-group}

To get named substring and/or name to number mapping, pass pointer to the appropriate vectors with `jp::RegexMatch::setNamedSubstringVector()` and/or `jp::RegexMatch::setNameToNumberMapVector()` before doing the match.

```cpp
jp::VecNum vec_num;   ///Vector to store numbered substring vector.
jp::VecNas vec_nas;   ///Vector to store named substring Map.
jp::VecNtN vec_ntn;   ///Vector to store Named substring to Number Map.
std::string ac_mod="g";   // g is for global match. Equivalent to using setFindAll() or FIND_ALL in addJpcre2Option()
re.initMatch()
  .setSubject(subject)                         //set subject string
  .setModifier(ac_mod)                         //set modifier string
  .setNumberedSubstringVector(&vec_num)        //pass pointer to vector of numbered substring vectors
  .setNamedSubstringVector(&vec_nas)           //pass pointer to vector of named substring maps
  .setNameToNumberMapVector(&vec_ntn)          //pass pointer to vector of name to number maps
  .match();                                    //Finally perform the match()

```

### Access a capture group by name {#access-substring-by-name}

```cpp
std::cout<<vec_nas[0]["name"]; // captured group by name in first match
std::cout<<vec_nas[1]["name"]; // captured group by name in second match
```

### Get the position of a capture group name {#get-number-to-name}

If you need this information, you should have passed a `jp::VecNtN` pointer to `jp::RegexMatch::setNameToNumberMapVector()` function before doing the match ([see above](#get-named-capture-group)).

```cpp
std::cout<<vec_ntn[0]["name"]; // position of captured group 'name' in first match
```

### Iterate through match result {#iterate}

You can iterate through the matches for numbered substrings (`jp::VecNum`) like this:

```cpp
for(size_t i=0;i<vec_num.size();++i){
    //i=0 is the first match found, i=1 is the second and so forth
    for(size_t j=0;j<vec_num[i].size();++j){
    	//j=0 is the capture group 0 i.e the total match
    	//j=1 is the capture group 1 and so forth.
    	std::cout<<"\n\t"<<j<<": "<<vec_num[i][j]<<"\n";
    }
}
```

You can iterate through named substrings (`jp::VecNas`) like this:

```cpp
for(size_t i=0;i<vec_nas.size();++i){
    //i=0 is the first match found, i=1 is the second and so forth
    for(jp::MapNas::iterator ent=vec_nas[i].begin();ent!=vec_nas[i].end();++ent){
	    //ent->first is the number/position of substring found
	    //ent->second is the substring itself
	    //when ent->first is 0, ent->second is the total match.
        std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
    }
}
```

If you are using `>=C++11`, you can make the loop a lot simpler:

<!-- if version [gte C++11] -->
```cpp
for(size_t i=0;i<vec_nas.size();++i){
	for(auto const& ent : vec_nas[i]){
	    std::cout<<"\n\t"<<ent.first<<": "<<ent.second<<"\n";
	}
}
```
<!-- end version if -->

`jp::VecNtN` can be iterated through the same way as `jp::VecNas`.

### Re-use a match object {#re-use-a-match-object}

Match object is a private property of `Regex` class. You either have to use `jp::Regex::initMatch()` or `jp::Regex::getMatchObject()` function to get a reference to it. The only difference between these two is: `initMatch()` always creates a new match object deleting the previous one, while `getMatchObject()` gives you a reference to the existing match object if available, otherwise creates it.

This is an example where we will perform a match in two steps:

```cpp
re.getMatchObject()                     //get a match object (new if it's the first call)
  .setNumberedSubstringVector(&vec_num) //pointer to numbered substring vector
  .setNamedSubstringVector(&vec_nas)    //pointer to named substring vector
  .setNameToNumberMapVector(&vec_ntn)   //pointer to name-to-number map vector
```
In the first step, we just set the vectors that we want our results in. This is pretty convenient when we are going to reuse the same vectors for multiple matches against the same regex.

```cpp
size_t count = re.getMatchObject()
                 .setSubject("I am the subject")
                 .setModifier("i")
                 .match()
```
We can perform this kind of matches as many times as we want. The vectors always get re-initialized and thus contain new data.

##Replace or Substitute {#replace}

The `jp::Regex::replace(const String& s, const String& r)` member function can take up-to three arguments (subject, replacement string, modifier) and returns the resultant replaced string. What it actually does is create a `RegexReplace` object and forward all options to it and finally call `jp::RegexReplace::replace()` function to perform the replacement.


### Simple replacement {#simple-replace}


```cpp
//Using a temporary regex object
std::cout<<jp::Regex("\\d+").replace("I am digits 1234 0000","5678", "g");
//'g' modifier is for global replacement
//1234 and 0000 gets replaced with 5678
```

### Using method chain {#using-method-chain}

```cpp
std::cout<<
re.initReplace()       //Prepare to call jp::RegexReplace::replace()
  .setSubject(s)       //Set various parameters
  .setReplaceWith(s2)  //...
  .setModifier("gE")   //...
  .addJpcre2Option(0)  //...
  .addPcre2Option(0)   //...
  .replace();          //Finally do the replacement.
//gE is the modifier passed (global and unknown-unset-empty).
//Access substrings/captured groups with ${1234},$1234 (for numbered substrings)
// or ${name} (for named substrings) in the replacement part i.e in setReplaceWith()

```
If you pass the size of the resultant string with `jp::RegexReplace::setBufferSize()` function, make sure it will be enough to store the whole resultant replaced string; otherwise the internal replace function (`pcre2_substitute()`) will be called *twice* to adjust the size of the buffer to hold the whole resultant string in order to avoid `PCRE2_ERROR_NOMEMORY` error.


# Modifiers {#modifiers}

**JPCRE2** uses modifiers to control various options, type, behavior of the regex and its' interactions with different functions that uses it. 

> All modifier strings are parsed and converted to equivalent PCRE2 and JPCRE2 options on the fly. If you don't want it to spend any time parsing modifier then pass the equivalent option directly with one of the many variants of `addJpcre2Option()` and `addPcre2Option()` functions.

Types of modifiers: 

1. Compile modifier
  1. Unique modifier
  2. Combined or mixed modifier (e.g 'n')
2. Action modifier
  1. Unique modifier
  2. Combined or mixed modifier (e.g 'E')


## Compile modifiers {#compile-modifier}

These modifiers define the behavior of a regex pattern (they are integrated in the compiled regex). They have more or less the same meaning as the [PHP regex modifiers](https://php.net/manual/en/reference.pcre.pattern.modifiers.php) except for `e, j and n` (marked with <sup>\*</sup>). 

Modifier | Details
-------- | -------
`e`<sup>\*</sup> | Unset back-references in the pattern will match to empty strings. Equivalent to `PCRE2_MATCH_UNSET_BACKREF`.
`i` | Case-insensitive. Equivalent to `PCRE2_CASELESS` option.
`j`<sup>\*</sup> | `\u \U \x` and unset back-references will act as JavaScript standard. <ul><li><code>\U</code> matches an upper case "U" character (by default it causes a compile error if this option is not set).</li><li><code>\u</code> matches a lower case "u" character unless it is followed by four hexadecimal digits, in which case the hexadecimal number defines the code point to match (by default it causes a compile error if this option is not set).</li><li><code>\x</code> matches a lower case "x" character unless it is followed by two hexadecimal digits, in which case the hexadecimal number defines the code point to match (By default, as in Perl, a hexadecimal number is always expected after <code>\x</code>, but it may have zero, one, or two digits (so, for example, <code>\xz</code> matches a binary zero character followed by z) ).</li><li>Unset back-references in the pattern will match to empty strings.</li></ul>
`m` | Multi-line regex. Equivalent to `PCRE2_MULTILINE` option.
`n`<sup>\*</sup> | Enable Unicode support for `\w \d` etc... in pattern. Equivalent to PCRE2_UTF \| PCRE2_UCP.
`s` | If this modifier is set, a dot meta-character in the pattern matches all characters, including newlines. Equivalent to `PCRE2_DOTALL` option.
`u` | Enable UTF support.Treat pattern and subjects as UTF strings. It is equivalent to `PCRE2_UTF` option.
`x` | Whitespace data characters in the pattern are totally ignored except when escaped or inside a character class, enables commentary in pattern. Equivalent to `PCRE2_EXTENDED` option.
`A` | Match only at the first position. It is equivalent to `PCRE2_ANCHORED` option.
`D` | A dollar meta-character in the pattern matches only at the end of the subject string. Without this modifier, a dollar also matches immediately before the final character if it is a newline (but not before any other newlines). This modifier is ignored if `m` modifier is set. Equivalent to `PCRE2_DOLLAR_ENDONLY` option.
`J` | Allow duplicate names for sub-patterns. Equivalent to `PCRE2_DUPNAMES` option.
`S` | When a pattern is going to be used several times, it is worth spending more time analyzing it in order to speed up the time taken for matching/replacing. It may also be beneficial for a very long subject string or pattern. Equivalent to an extra compilation with JIT\_COMPILER with the option `PCRE2_JIT_COMPLETE`.
`U` | This modifier inverts the "greediness" of the quantifiers so that they are not greedy by default, but become greedy if followed by `?`. Equivalent to `PCRE2_UNGREEDY` option.

## Action modifiers {#action-modifiers}

These modifiers are not compiled in the regex itself, rather they are used per call of each match or replace function.

Modifier | Action | Details
------ | ------ | ----- |
`A` | match | Match at start. Equivalent to `PCRE2_ANCHORED`. Can be used in match operation. Setting this option only at match time (i.e regex was not compiled with this option) will disable optimization during match time.
`e` | replace | Replaces unset group with empty string. Equivalent to `PCRE2_SUBSTITUTE_UNSET_EMPTY`.
`E` | replace | Extension of `e` modifier. Sets even unknown groups to empty string. Equivalent to PCRE2_SUBSTITUTE_UNSET_EMPTY \| PCRE2_SUBSTITUTE_UNKNOWN_UNSET
`g` | match<br>replace | Global. Will perform global matching or replacement if passed. Equivalent to `jpcre2::FIND_ALL`.
`x` | replace | Extended replacement operation. Equivalent to `PCRE2_SUBSTITUTE_EXTENDED`. It enables some Bash like features:<br>`${<n>:-<string>}`<br>`${<n>:+<string1>:<string2>}`<br>`<n>` may be a group number or a name. The first form specifies a default value. If group `<n>` is set, its value is inserted; if not, `<string>` is expanded and the result is inserted. The second form specifies strings that are expanded and inserted when group `<n>` is set or unset, respectively. The first form is just a convenient shorthand for `${<n>:+${<n>}:<string>}`.


# Options {#options}

JPCRE2 allows both PCRE2 and native JPCRE2 options to be passed. PCRE2 options are recognized by the PCRE2 library itself.

## JPCRE2 options {#jpcre-options}

These options are meaningful only for the **JPCRE2** library, not the original **PCRE2** library. We use the `jp::Regex::addJpcre2Option()` family of functions to pass these options.

Option | Details
------ | ------
`jpcre2::NONE` | This is the default option. Equivalent to 0 (zero).
`jpcre2::FIND_ALL` | This option will do a global matching if passed during matching. The same can be achieved by passing the 'g' modifier with `jp::RegexMatch::setModifier()` function.
`jpcre2::JIT_COMPILE` | This is same as passing the `S` modifier during pattern compilation.

## PCRE2 options {#pcre2-options}

While having its own way of doing things, JPCRE2 also supports the traditional PCRE2 options to be passed (and it's faster than passing modifier). We use the `jp::Regex::addPcre2Option()` family of functions to pass the PCRE2 options. These options are the same as the PCRE2 library and have the same meaning. For example instead of passing the 'g' modifier to the replacement operation we can also pass its PCRE2 equivalent `PCRE2_SUBSTITUTE_GLOBAL` to have the same effect.

# Code unit width & character type {#code-unit-and-character-type}

The bit size of character type must match with the PCRE2 library you are linking against. There are three PCRE2 libraries according to code unit width, namely 8, 16 and 32 bit libraries. So, if you use a character type (e.g `char` which is generally 8 bit) of 8-bit code unit width then you will have to link your program against the 8-bit PCRE2 library. If it's 16-bit character, you will need 16-bit library. If you use a combination of various code unit width supported or use all of them, you will have to link your program against their corresponding PCRE2 libraries. Missing library will yield to compile time error.

**Implementation defined behavior:**

Size of integral types (`char`, `wchar_t`, `char16_t`, `char32_t`) is implementation defined. `char` may be 8, 16, 32 or 64 (not supported) bit. Same goes for `wchar_t` and others. In Linux `wchar_t` is 32 bit and in windows it's 16 bit.

# Portable coding {#portable-coding}

## Code unit width quirk {#code-unit-width-quirk}

JPCRE2 codes are portable when you use the selector (`jpcre2::select`) without an explicit bit size. In this case, code will get compiled according to the code unit width defined by your system. Consider the following example, where you do :

```cpp
#include <jpcre2.hpp>

typedef jpcre2::select<char> jp;

int main(){
    jp::Regex re;

    ///other things
    // ...
    
    return 0;
}
```
This is what will happen when you compile:

1. In a system where `char` is 8 bit, it will use 8-bit library and UTF-8 in UTF-mode.
2. In a system where `char` is 16 bit, it will use 16-bit library and UTF-16 in UTF-mode.
3. In a system where `char` is 32 bit, it will use 32-bit library and UTF-32 in UTF-mode.
4. In a system where `char` is not 8, 16 or 32 bit, it will yield compile error.

So, if you don't want to be so aware of the code unit width of the character type/s you are using, link your program against all PCRE2 libraries. The code unit width will be handled automatically (unless you use explicit code unit width like `jpcre2::select<char, 8>`) and if anything unsupported is encountered, you will get compile time error.

A common example in this regard can be the use of `wchar_t`:

```cpp
jpcre2::select<wchar_t>::Regex re;
```

1. In windows, the above code will use 16-bit library and UTF-16 in UTF mode.
2. In Linux, the above code will use 32-bit library and UTF-32 in UTF mode.

> If you want to fix the code unit width, use an explicit bit size such as `jpcre2::select<Char_T, BS>`, but in this case, your code will not be portable, and you will get compile error (if not suppressed) when code unit width mismatch occurs.

## Use of string class {#use-of-string-class}

For portable code, instead of using the standard names `std::string` or such, use `jp::String` (you may further typedef it as `String` or whatever). It will be defined to an appropriate string class according to the basic character type you selected and thus provide all the functionalities and conveniences you get with `std::string` and such. Being said that, there's no harm if you use the standard names (`std::string` and such). Using `jp::String` will just ensure that you are using the correct string class for the correct character type. If you need to use the basic character type, use `jp::Char`.

## Use of vectors {#use-of-vectors}

Instead of using full names like `std::vector<std::string>` and such for storing match result, use the typedefs:

1. `jp::NumSub`: Equivalent to `std::vector<std::string>`
2. `jp::MapNas`: Equivalent to `std::map<std::string, std::string>`
3. `jp::MapNtN`: Equivalent to `std::map<std::string, size_t>`
4. `jp::VecNum`: Equivalent to `std::vector<jp::NumSub>`
5. `jp::VecNas`: Equivalent to `std::vector<jp::MapNas>`
6. `jp::VecNtN`: Equivalent to `std::vector<jp::MapNtN>`

## Other typedefs {#other-typedefs}

Other typedefs are mostly for internal use

* You can use `jpcre2::Convert16` to convert between UTF-8 and UTF-16. (`>=C++11`)
* You can use `jpcre2::Convert32` to convert between UTF-8 and UTF-32. (`>=C++11`)
* You should not use the `jpcre2::Ush` as unsigned short. In JPCRE2 context, it is the smallest unsigned integer type to cover at least the numbers from 1 to 126.
* `jpcre2::Uint` is a fixed width unsigned integer type and will be at least 32 bit wide.
* `jpcre2::SIZE_T` is the same as `PCRE2_SIZE` which is defined as `size_t`.


# Error handling {#exception-handling}

When a known error is occurred during pattern compilation or match or replace, the error number and error offsets are set to corresponding variables of `jp::Regex` class. You can get the error number, error offset and error message with `jp::Regex::getErrorNumber()`, `jp::Regex::getErrorOffset()` and `jp::Regex::getErrorMessage()` functions respectively.

**Note** that, these errors always gets overwritten by previous error, so you only get the last error that occurred.

**Also note** that, these errors never get re-initialized (set to zero), they are always there even when everything else worked great (except some previous error).

If you do experiment with various erroneous situations, make use of the `resetErrors()` function. You can call it from anywhere in your method chain and immediately set the errors to zero. This function is defined for all three classes and do the same thing.

# Short examples {#short-examples}

```cpp

size_t count;
//Check if string matches the pattern
/*
 * The following uses a temporary Regex object.
 */
if(jp::Regex("(\\d)|(\\w)").match("I am the subject"))
    std::cout<<"\nmatched";
else
    std::cout<<"\nno match";
/*
 * Using the modifier S (i.e jpcre2::JIT_COMPILE) with temporary object may or may not give you
 * any performance boost (depends on the complexity of the pattern). The more complex
 * the pattern gets, the more sense the S modifier makes.
 */

//If you want to match all and get the match count, use the action modifier 'g':
std::cout<<"\n"<<
    jp::Regex("(\\d)|(\\w)","m").match("I am the subject","g");

/*
 * Modifiers passed to the Regex constructor or with compile() function are compile modifiers
 * Modifiers passed with the match() or replace() functions are action modifiers
 */

// Substrings/Captured groups:

/*
 * *** Getting captured groups/substring ***
 *
 * captured groups or substrings are stored in maps/vectors for each match,
 * and each match is stored in a vector.
 * Thus captured groups are in a vector of maps/vectors.
 *
 * PCRE2 provides two types of substrings:
 *  1. numbered (indexed) substring
 *  2. named substring
 *
 * For the above two, we have two vectors respectively:
 *  1. jp::VecNum (Corresponding vector: jp::NumSub)
 *  2. jp::VecNas (Corresponding map: jp::MapNas)
 *
 * Another additional vector is available to get the substring position/number
 * for a particular captured group by name. It's a vector of name to number maps
 *  * jp::VecNtN (Corresponding map: jp:MapNtN)
 */

// ***** Get numbered substring ***** ///
jp::VecNum vec_num;
count =
jp::Regex("(\\w+)\\s*(\\d+)","m")
    .initMatch()
    .setSubject("I am 23, I am digits 10")
    .setModifier("g")
    .setNumberedSubstringVector(&vec_num)
    .match();
/*
* count (the return value) is guaranteed to give you the correct number of matches,
* while vec_num.size() may give you wrong result if any match result
* was failed to be inserted in the vector. This should not happen
* i.e count and vec_num.size() should always be equal.
*/
std::cout<<"\nNumber of matches: "<<count/* or vec_num.size()*/;

//Now vec_num is populated with numbered substrings for each match
//The size of vec_num is the total match count
//vec_num[0] is the first match
//The type of vec_num[0] is jp::NumSub
std::cout<<"\nTotal match of first match: "<<vec_num[0][0];
std::cout<<"\nCaptured group 1 of first match: "<<vec_num[0][1];
std::cout<<"\nCaptured group 2 of first match: "<<vec_num[0][2];

//captured group 3 doesn't exist, (with operator [] it's a segfault)
//std::cout<<"\nCaptured group 3 of first match: "<<vec_num[0][3];

//Using at() will throw std::out_of_range exception
//~ try {
    //~ std::cout<<"\nCaptured group 3 of first match: "<<vec_num[0].at(3);
//~ } catch (const std::out_of_range& e) {
    //~ std::cout<<"\n"<<e.what();
//~ }


//There were two matches found (vec_num.size() == 2) in the above example
std::cout<<"\nTotal match of second match: "<<vec_num[1][0];      //Total match (group 0) from second match
std::cout<<"\nCaptured group 1 of second match: "<<vec_num[1][1]; //captured group 1 from second match
std::cout<<"\nCaptured group 2 of second match: "<<vec_num[1][2]; //captured group 2 from second match


// ***** Get named substring ***** //

jp::VecNas vec_nas;
jp::VecNtN vec_ntn; // We will get name to number map vector too
count =
jp::Regex("(?<word>\\w+)\\s*(?<digit>\\d+)","m")
    .initMatch()
    .setSubject("I am 23, I am digits 10")
    .setModifier("g")
    //.setNumberedSubstringVector(vec_num) // We don't need it in this example
    .setNamedSubstringVector(&vec_nas)
    .setNameToNumberMapVector(&vec_ntn) // Additional (name to number maps)
    .match();
std::cout<<"\nNumber of matches: "<<vec_nas.size()/* or count */;
//Now vec_nas is populated with named substrings for each match
//The size of vec_nas is the total match count
//vec_nas[0] is the first match
//The type of vec_nas[0] is jp::MapNas
std::cout<<"\nCaptured group (word) of first match: "<<vec_nas[0]["word"];
std::cout<<"\nCaptured group (digit) of first match: "<<vec_nas[0]["digit"];

//Trying to access a non-existence named substirng with [] operator will give you empty string
//If the existence of a substring is important, use the std::map::find() or std::map::at() 
//(>=C++11) function to access map elements.
/* //>=C++11
try{
    ///This will throw exception because the substring name 'name' doesn't exist
    std::cout<<"\nCaptured group (name) of first match: "<<vec_nas[0].at("name");
} catch(const std::logic_error& e){
    std::cerr<<"\nCaptured group (name) doesn't exist";
}*/

//There were two matches found (vec_nas.size() == 2) in the above example
std::cout<<"\nCaptured group (word) of second match: "<<vec_nas[1]["word"];
std::cout<<"\nCaptured group (digit) of second match: "<<vec_nas[1]["digit"];

//Get the position (number) of a captured group name (that was found in match)
std::cout<<"\nPosition of captured group (word) in first match: "<<vec_ntn[0]["word"];
std::cout<<"\nPosition of captured group (digit) in first match: "<<vec_ntn[0]["digit"];

/*
 * Replacement Examples
 * Replace pattern in a string with a replacement string
 *
 * The Regex::replace() function can take a subject and replacement string as argument.
 * 
 * You can also pass the subject with setSubject() function in method chain,
 * replacement string with setReplaceWith() function in method chain, etc ...
 * A call to RegexReplace::replace() in the method chain will return the resultant string
 */

std::cout<<"\n"<<
//replace first occurrence of a digit with @
jp::Regex("\\d").replace("I am the subject string 44", "@");

std::cout<<"\n"<<
//replace all occurrences of a digit with @
jp::Regex("\\d").replace("I am the subject string 44", "@", "g");

//swap two parts of a string
std::cout<<"\n"<<
jp::Regex("^([^\t]+)\t([^\t]+)$")
    .replace("I am the subject\tTo be swapped according to tab", "$2 $1");
    
//Doing the above with method chain:
jp::Regex("^([^\t]+)\t([^\t]+)$")
    .initReplace()
    .setSubject("I am the subject\tTo be swapped according to tab")
    .setReplaceWith("$2 $1")
    .replace();

```

# API change notice {#api-change-notice}
If you are using the previous version of JPCRE2 (10.27), you can easily migrate to this latest API by replacing all `jpcre2::select8` or `jpcre2::select16` or `jpcre2::select32` with `jpcre2::select` in your code.


# The configure script {#the-configure-script}

The configure script generated by autotools checks for availability of several programs and let's you set several options to control your testing environment. These are the options supported by configure scipt:

Option | Details
------ | -------
`--[enable/disable]-test-8` | Enable/Disable building 8 bit examples (enabled by default)
`--[enable/disable]-test-16` | Enable/Disable building 16 bit examples
`--[enable/disable]-test-32` | Enable/Disable building 32 bit examples
`--[enable/disable]-cpp11` | Enable/Disable building examples with C++11 features
`--[enable/disable]-test-multi` | Enable/Disable building examples for multi code unit width support. (includes all previous ones.)
`--[enable/disable]-silent-rules` | Enable/Disable silent rules (enabled by default). You will get prettified `make` output if enabled.


# LICENCE {#licence}
This project comes with a BSD LICENCE, see the LICENCE file for more details.

It is not necessary to let me know which project you are using this library on, but an optional choice. I would very much appreciate it, if you let me know about the name (and short description if applicable) of the project. So if you have the time, please send me an [email](https://neurobin.org/about/contact/?s=Using+jpcre2+in+a+project&m=I+am+using+jpcre2+in+the+following+project%3A%0A%0AProject+Name%3A+%0AShort+description%3A%0A%0AYou+can+share+the+project+name+publicly%3A+%5Byes%2Fno%5D%0AYou+can+share+the+project+description+publicly%3A+%5Byes%2Fno%5D%0AYou+can+share+the+project+author+name+publicly%3A+%5Byes%2Fno%5D%0AEmail+will+be+private+and+not+shared%3A+yes%0A).

