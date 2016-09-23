JPCRE2                         {#mainpage}
======

C++ wrapper for PCRE2 library

[![Build status image](https://travis-ci.org/jpcre2/jpcre2.svg?branch=release)](https://travis-ci.org/jpcre2/jpcre2/) [![doc percentage image](https://neurobin.org/img/badge/doc-percent1.svg)](http://docs.neurobin.org/jpcre2/index.html) [![CPP depends image](https://neurobin.org/img/badge/CPP-depends.svg)](https://isocpp.org/) [![PCRE2 depends image](https://neurobin.org/img/badge/PCRE2-dep1.svg)](http://www.pcre.org/) 


> PCRE2 is the name used for a revised API for the PCRE library, which is a set of functions, written in C, that implement regular expression pattern matching using the same syntax and semantics as Perl, with just a few differences. Some features that appeared in Python and the original PCRE before they appeared in Perl are also available using the Python syntax.

This provides some C++ wrapper functions to perform regex operations such as regex match and regex replace.

You can read the complete documentation [here](http://docs.neurobin.org/jpcre2/index.html) or download it from [jpcre2-doc repository](https://github.com/jpcre2/jpcre2-doc).


# Dependency {#dependency}

1. PCRE2 library (`version >=10.21`).


If the required PCRE2 version is not available in the official channel, you can download <a href="https://github.com/jpcre2/pcre2">my fork of the library</a>.


# Install or Include {#install-or-include}

The `jpcre2.hpp` header should be included in the source file that uses JPCRE2 functionalities.

## Use as library {#use-as-library}

To build and install it as library in a Unix based system, run:

```sh
./configure
make
make install # or sudo make install
```
To build and install all three (8-bit, 16-bit and 32-bit) libraries you can do:

```sh
./configure --enable-jpcre2-16 --enable-jpcre2-32
make
make install # or sudo make install
```
[See here](#the-configure-script) for more options to the configure script.

Now `#include <jpcre2.hpp>` in your code and build/compile by linking with both JPCRE2 and PCRE2 library.

**You must define `PCRE2_CODE_UNIT_WIDTH` as 0 or 8 or 16 or 32 before including jpcre2.hpp**. 0 means you are going to use all of the libraries. See [Code unit width and character type](#code-unit-and-character-type) for details.

An example command to compile your code with GCC would be:

```sh
g++  mycpp.cpp -ljpcre2-8 -lpcre2-8 #sequence is important
```
An example command to build a program with multi-code unit width support:

```sh
g++ mycpp.cpp -ljpcre2-8 -ljpcre2-16 -ljpcre2-32 -lpcre2-8 -lpcre2-16 -lpcre2-32
```

i.e if you use all three code unit width in your application you will have to link with all three JPCRE2 and all three PCRE2 libraries.

If you are in a non-Unix system (e.g Windows), build a library from the JPCRE2 sources with your favorite IDE or use it as it is.

**Notes:**

1. When compiling with GCC you must put the JPCRE2 library before the PCRE2 library because JPCRE2 depends on PCRE2.
2. Building 8-bit jpcre2 requires 8-bit pcre2 and so on. Building all at once requires all of 8, 16 and 32 bit PCRE2 libraries.
3. To use the `PCRE2 POSIX` compatible library, add the `-lpcre2-posix` along with the others.



## Use with sources {#use-with-sources}

**If you use these direct sources, you won't be able to use multiple code unit width in a single program. For multi code unit width support, [build jpcre2 libraries](#use-as-library) and link against them.**


After including the header you can compile your source either by installing and linking with JPCRE2 libraries or providing the following sources to your compiler:

1. **jpcre2.hpp**
2. **jpcre2.cpp**

An example compile/build command with GCC would be:

```sh
g++ mycpp.cpp jpcre2.cpp jpcre2.hpp -lpcre2-8 -DPCRE2_CODE_UNIT_WIDTH=8
```

If your PCRE2 library is not in the standard library path, then add the path:

```sh
g++ mycpp.cpp ... -L/path/to/your/pcre2/library -lpcre2-8 -DPCRE2_CODE_UNIT_WIDTH=8
```

> Even if your cpp file defines the PCRE2_CODE_UNIT_WIDTH macro you will still need to define it like above if you are compiling with g++. If you use an IDE then you might not need that i.e your IDE will probably be intelligent enough to handle this situation. Better use JPCRE2 as a library.


# How to code with JPCRE2 {#how-to-code-with-jpcre2}

Performing a match or replacement against regex pattern involves two steps: 

1. Compiling the pattern
2. Performing the match or replacement operation

## Compile a regex pattern {#compile-a-regex-pattern}

Select a character type according to `PCRE2_CODE_UNIT_WDTH`. In this doc we are going to use 8 bit code unit width as reference and we will use `char` as the character type.

Let's use a typedef to shorten the code:

```cpp
typedef jpcre2::select<char> jp
// You have to select the basic data type (char, wchar_t, char16_t or char32_t)
// And this data type must be of the same bit size as PCRE2_CODE_UNIT_WIDTH
```

`jpcre2::select` is only available when you define a single code unit width (8, 16 or 32). When you support multi code unit width with `PCRE2_CODE_UNIT_WIDTH 0` you must call the selectors with their original names (`jpcre2::select8`, `jpcre2::select16` and `jpcre2::select32`)


### A Regex object {#a-regex-object}

(You can use temporary object too, see [short examples](#short-examples)).

This object will hold the pattern, modifiers (PCRE2 and JPCRE2 options) and compiled pattern.

```cpp
jp::Regex re;
```
Each object for each regex pattern.

### Compile the regex {#compile-the-regex}

```cpp
re.setPattern("(?:(?<word>[?.#@:]+)|(?<word>\\w+))\\s*(?<digit>\\d+)")  //set pattern
  .addModifier("iJ")                                                    //add modifier (J for PCRE2_DUPNAMES)
  .compile();                                                           //Finally compile it.
      
//Do not use setModifier() after adding any options, it will reset them.

//Another way is to use constructor to initialize and compile at the same time:
jp::Regex re2("pattern2","mSi");  //S is an optimization mod.
jp::Regex re3("pattern3", PCRE2_ANCHORED);
jp::Regex re4("pattern4", PCRE2_ANCHORED, jpcre2::JIT_COMPILE);

```

Now you can perform match or replace against the pattern. Use the `match()` member function to perform regex match and the `replace()` member function to perform regex replace.

### Check if regex compiled successfully {#check-regex}

You can check if the regex was compiled successfully or not, but **it's not required at all**. Any error will be handled automatically and you will always get the right answer, e.g a match against a non-compiled regex will give you 0 match and for replace you will be returned the exact subject string that you passed.

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

The `jp::Regex::match(const String& s)` family of member functions can take two arguments (subject & modifier) and returns the number of matches found against the compiled pattern.


To get the match result (captured groups) however, you need to call the `jp::RegexMatch::match()` function. Point be noted that, you can not call this function directly or create any object of the class `jp::RegexMatch`. To call this function, first invoke the `jp::Regex::initMatch()` (or `jp::Regex::getMatchObject()`) function. It will give you a reference to a `jp::RegexMatch` object. Now you can chain method calls of `jp::RegexMatch::setNumberedSubstringVector(VecNum* vec_num)` and such functions from `jp::RegexMatch` class to pass various parameters. After you are done passing all the parameter that you need, the `jp::RegexMatch::match()` function should be called to perform the actual match and return the match count. The match results will be stored in vectors (vectors of maps) whose pointers were passed as parameters.

### Check if a string matches a regex {#check-if-a-string-matches-a-regex}

```cpp
if(re.match("I am the subject")) std::cout<<"matched (case sensitive)";
else std::cout<<"Didn't match";

if(re.match("I am the subject","i")) std::cout<<"matched (case insensitive)";
else std::cout<<"Didn't match";
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
               .setNumberedSubstringVector(&vec_num)        //pass pointer to VecNum vector to store maps of numbered substrings
               .match();                                    //Finally perform the match.
//vec_num will be populated with maps of numbered substrings.
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
jp::VecNum vec_num;   ///Vector to store numbered substring Map.
jp::VecNas vec_nas;   ///Vector to store named substring Map.
jp::VecNtN vec_ntn;   ///Vector to store Named substring to Number Map.
std::string ac_mod="g";   // g is for global match. Equivalent to using setFindAll() or FIND_ALL in addJpcre2Option()
re.initMatch()
  .setSubject(subject)                         //set subject string
  .setModifier(ac_mod)                         //set modifier string
  .setNumberedSubstringVector(&vec_num)        //pass pointer to vector of numbered substring maps
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

You can iterate through the matches and their substrings like this:

```cpp
for(size_t i=0;i<vec_num.size();++i){
    //i=0 is the first match found, i=1 is the second and so forth
    for(jp::MapNum::iterator ent=vec_num[i].begin();ent!=vec_num[i].end();++ent){
	    //ent.first is the number/position of substring found
	    //ent.second is the substring itself
	    //when ent->first is 0, ent->second is the total match.
        std::cout<<"\n\t"<<ent->first<<": "<<ent->second<<"\n";
    }
}
```

If you are using `>=C++11`, you can make the loop a lot simpler:

<!-- if version [gte C++11] -->
```cpp
for(size_t i=0;i<vec_num.size();++i){
	for(auto const& ent : vec_num[i]){
	    std::cout<<"\n\t"<<ent.first<<": "<<ent.second<<"\n";
	}
}
```
<!-- end version if -->

*The process of iterating through the vectors and associated maps are the same for all three. The size of those vectors are the same and can be accessed in the same way.*

### Re-use a match object {#re-use-a-match-object}

Match object is a private property of `jp::Regex` class. You either have to use `jp::Regex::initMatch()` or `jp::Regex::getMatchObject()` function to get a reference to it. The only difference between these two is: `initMatch()` always creates a new match object deleting the previous one, while `getMatchObject()` gives you a reference to the existing match object if available, otherwise creates it.

This is an example where we will perform a match in two steps:

```cpp
re.initMatch()                                //invoke the initMatch() function
  .setNumberedSubstringVector(&vec_num)       //pointer to numbered substring vector
  .setNamedSubstringVector(&vec_nas)          //pointer to named substring vector
  .setNameToNumberMapVector(&vec_ntn)         //pointer to name-to-number map vector
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

The `jp::Regex::replace(const String& s, const String& r)` member function can take up-to three arguments (subject, replacement string, modifier) and returns the resultant replaced string.

If you want to pass more options or prefer method chaining, you will have to use the `jp::RegexReplace::replace()` function instead. Point be noted that, all constructors of the `jp::RegexReplace` class are private and thus you can't create any object of this class or call the mentioned function directly. In this case you need to call `jp::Regex::initReplace()` (or `jp::Regex::getReplaceObject()`) function which will give you a reference to the replace object that you can use to chain method calls to pass various options to be used by `jp::RegexReplace::replace()` before calling it.


### Simple replacement {#simple-replace}


```cpp
//Using a temporary regex object
std::cout<<jp::Regex("\\d+").replace("I am digits 1234","5678", "g");
//'g' modifier is for global replacement
//1234 gets replaced with 5678
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
`j`<sup>\*</sup> | `\u \U \x` and unset back-references will act as JavaScript standard. <ul><li><code>\U</code> matches an upper case "U" character (by default it causes a compile time error if this option is not set).</li><li><code>\u</code> matches a lower case "u" character unless it is followed by four hexadecimal digits, in which case the hexadecimal number defines the code point to match (by default it causes a compile time error if this option is not set).</li><li><code>\x</code> matches a lower case "x" character unless it is followed by two hexadecimal digits, in which case the hexadecimal number defines the code point to match (By default, as in Perl, a hexadecimal number is always expected after <code>\x</code>, but it may have zero, one, or two digits (so, for example, <code>\xz</code> matches a binary zero character followed by z) ).</li><li>Unset back-references in the pattern will match to empty strings.</li></ul>
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

These modifiers are not compiled in the regex itself, rather they are used per call of each match, replace or compile function.

Modifier | Action | Details
------ | ------ | ----- |
`A` | match | Match at start. Equivalent to `PCRE2_ANCHORED`. Can be used in match operation. Setting this option only at match time (i.e regex was not compiled with this option) will disable optimization during match time.
`e` | replace | Replaces unset group with empty string. Equivalent to `PCRE2_SUBSTITUTE_UNSET_EMPTY`.
`E` | replace | Extension of `e` modifier. Sets even unknown groups to empty string. Equivalent to PCRE2_SUBSTITUTE_UNSET_EMPTY \| PCRE2_SUBSTITUTE_UNKNOWN_UNSET
`g` | match<br>replace | Global. Will perform global matching or replacement if passed. Equivalent to `jp::FIND_ALL`.
`x` | replace | Extended replacement operation. Equivalent to `PCRE2_SUBSTITUTE_EXTENDED`. It enables some Bash like features:<br>`${<n>:-<string>}`<br>`${<n>:+<string1>:<string2>}`<br>`<n>` may be a group number or a name. The first form specifies a default value. If group `<n>` is set, its value is inserted; if not, `<string>` is expanded and the result is inserted. The second form specifies strings that are expanded and inserted when group `<n>` is set or unset, respectively. The first form is just a convenient shorthand for `${<n>:+${<n>}:<string>}`.


# Options {#options}

JPCRE2 allows both PCRE2 and native JPCRE2 options to be passed. PCRE2 options are recognized by the PCRE2 library itself.

## JPCRE2 options {#jpcre-options}

These options are meaningful only for the **JPCRE2** library, not the original **PCRE2** library. We use the `jp::Regex::addJpcre2Option()` family of functions to pass these options.

Option | Details
------ | ------
`jp::NONE` | This is the default option. Equivalent to 0 (zero).
`jp::FIND_ALL` | This option will do a global matching if passed during matching. The same can be achieved by passing the 'g' modifier with `jp::RegexMatch::setModifier()` function.
`jp::JIT_COMPILE` | This is same as passing the `S` modifier during pattern compilation.

## PCRE2 options {#pcre2-options}

While having its own way of doing things, JPCRE2 also supports the traditional PCRE2 options to be passed (and it's faster than passing modifier). We use the `jp::Regex::addPcre2Option()` family of functions to pass the PCRE2 options. These options are the same as the PCRE2 library and have the same meaning. For example instead of passing the 'g' modifier to the replacement operation we can also pass its PCRE2 equivalent `PCRE2_SUBSTITUTE_GLOBAL` to have the same effect.

## Code unit width & character type {#code-unit-and-character-type}

The bit size of character type must match with `PCRE2_CODE_UNIT_WIDTH`. That is, if you are compiling in a machine where `char` is 32 bit, you need to use the 32-libraries. The bit size of `char` can be 8 bit, 16 bit, 32 bit, 64 bit (not supported ) etc... depending on the system. Same goes for `wchar_t`. In Linux `wchar_t` is 32 bit where in Windows, it's 16 bit. A system which only have 8 bit support will define `wchar_t` as 8 bit. So a definition like this may be preferable:

```cpp
#include <climits> //CHAR_BIT
#define PCRE2_CODE_UNIT_WIDTH sizeof(char)*CHAR_BIT
//#define PCRE2_CODE_UNIT_WIDTH sizeof(wchar_t)*CHAR_BIT //when you will use wchar_t
#include <jpcre2.hpp>
```
Now if the code unit width is not supported you will get clear compiling error, and when there is no error, you can safely assume that you are using the right library for the character type you are going to use.

The types `char16_t` and `char32_t` are >= C++11 additions. To use them you need to compile the libraries with C++11 enabled. In such cases, use updated compiler versions as old compilers might not have the `codecvt` header that is required for these types.

# Error handling {#exception-handling}

When a known error is occurred during pattern compilation or match or replace, the error number and error offsets are set to corresponding variables of `jp::Regex` class. You can get the error number, error offset and error message with `jp::Regex::getErrorNumber()`, `jp::Regex::getErrorOffset()` and `jp::Regex::getErrorMessage()` functions respectively.

**Note** that, these errors always gets overwritten by previous error, so you only get the last error that occurred.

**Also note** that, these errors never get re-initialized (se to zero), they are always there even when everything else worked great (except some previous error). But these are harmless.

If you do experiment with various erroneous situations, make use of the `resetErrors()` function. You can call it from anywhere in your method chain and immediately set the errors to zero. This function is defined for all three classes and do the same thing.

If you want to debug, you should also use `#define JPCRE2_ENABLE_CODE_UNIT_VALIDATION` before including `jpcre2.hpp`


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
 * captured groups or substrings are stored in maps for each match,
 * and each match is stored in a vector.
 * Thus captured groups are in a vector of maps.
 *
 * PCRE2 provides two types of substrings:
 *  1. numbered (index) substring
 *  2. named substring
 *
 * For the above two, we have two vectors respectively:
 *  1. jp::VecNum (Corresponding map: jp::MapNum)
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
//The type of vec_num[0] is jp::MapNum
std::cout<<"\nTotal match of first match: "<<vec_num[0][0];
std::cout<<"\nCaptrued group 1 of first match: "<<vec_num[0][1];
std::cout<<"\nCaptrued group 2 of first match: "<<vec_num[0][2];

 //captured group 3 doesn't exist, it will give you empty string
std::cout<<"\nCaptrued group 3 of first match: "<<vec_num[0][3];

//Using the [] operator with jp::MapNum will create new element if it doesn't exist
// i.e vec_num[0][3] were created in the above example.
//This should be ok, if existence of a particular substring is not important

//If the existence of a substring is important, use the std::map::find() or std::map::at() (>=C++11) function to access map elements
/* //>=C++11
try{
    //This will throw exception, because substring 4 doesn't exist
    std::cout<<"\nCaptrued group 4 of first match: "<<vec_num[0].at(4);
} catch (std::logic_error& e){
    std::cerr<<"\nCaptrued group 4 doesn't exist";
}*/

//There were two matches found (vec_num.size() == 2) in the above example
std::cout<<"\nTotal match of second match: "<<vec_num[1][0];      //Total match (group 0) from second match
std::cout<<"\nCaptrued group 1 of second match: "<<vec_num[1][1]; //captured group 1 from second match
std::cout<<"\nCaptrued group 2 of second match: "<<vec_num[1][2]; //captured group 2 from second match


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

//If the existence of a substring is important, use the std::map::find() or std::map::at() (>=C++11) function to access map elements
/* //>=C++11
try{
    ///This will throw exception because the substring name 'name' doesn't exist
    std::cout<<"\nCaptured group (name) of first match: "<<vec_nas[0].at("name");
} catch(std::logic_error& e){
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
 * The initReplace() function can take a subject and replacement string as argument.
 * You can also pass the subject with setSubject() function in method chain,
 * replacement string with setReplaceWith() function in method chain, etc ...
 *
 * A call to replace() will return the resultant string
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

```

# The configure script {#the-configure-script}

The configure script generated by autotools checks for availability of several programs and let's you set several options to control your build environment. These are the options supported by configure scipt:

Option | Details
------ | -------
`--enable-jpcre2-8` | Build JPCRE2 8 bit library (default)
`--enable-jpcre2-16` | Build JPCRE2 16 bit library  
`--enable-jpcre2-32` | Build JPCRE2 32 bit library  
`--disable-jpcre2-8` | Do not build JPCRE2 8 bit library
`--disable-jpcre2-16` | Do not build JPCRE2 16 bit library  
`--disable-jpcre2-32` | Do not build JPCRE2 32 bit library  
`--disable-jpcre2-test-compile` | Do not build the test cpp files (enabled by default). This will be disabled automatically if 8 bit library is not selected to be built.
`--enable-jpcre2-test-compile-cpp11` | Build the test cpp files that require >=C++11 (disabled by default). No effect if 16 bit and 32 bit libraries weren't selected to be built.




