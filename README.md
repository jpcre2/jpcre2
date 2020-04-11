<a name="mainpage"></a>

JPCRE2                         
======

C++ wrapper for PCRE2 library

[![Build status image](https://travis-ci.org/jpcre2/jpcre2.svg?branch=release)](https://travis-ci.org/jpcre2/jpcre2/) [![Coverage Status](https://coveralls.io/repos/github/jpcre2/jpcre2/badge.svg?branch=release)](https://coveralls.io/github/jpcre2/jpcre2?branch=release) [![CPP depends image](https://neurobin.org/img/badge/CPP-depends1.svg)](https://isocpp.org/) [![PCRE2 depends image](https://neurobin.org/img/badge/PCRE2-dep1.svg)](http://www.pcre.org/) 


> PCRE2 is the name used for a revised API for the PCRE library, which is a set of functions, written in C, that implement regular expression pattern matching using the same syntax and semantics as Perl, with just a few differences. Some features that appeared in Python and the original PCRE before they appeared in Perl are also available using the Python syntax.

This provides some C++ wrapper classes/functions to perform regex operations such as regex match and regex replace.

You can read the complete documentation [here](http://docs.neurobin.org/jpcre2/) or download it from [jpcre2-doc repository](https://github.com/jpcre2/jpcre2-doc).


<a name="dependency"></a>

# Dependency 

1. PCRE2 library (`version >=10.21`).


If the required PCRE2 version is not available in the official channel, you can download <a href="https://github.com/jpcre2/pcre2">my fork of the library</a>.


<a name="getting-started"></a>

# Getting started 

This is a **header only** library. All you need to do is include the header `jpcre2.hpp` in your program.

```cpp
#include "jpcre2.hpp"
```

**Notes:** 

* `jpcre2.hpp` \#includes `pcre2.h`, thus you don't need to include `pcre2.h` manually in your program.
* If `pcre2.h` is in a non-standard path then you may include it before `jpcre2.hpp` with correct path (you will need to define `PCRE2_CODE_UNIT_WIDTH` before including `pcre2.h` in this case)
* There's no need to define `PCRE2_CODE_UNIT_WIDTH` before including `jpcre2.hpp`.
* On windows, if you are working with a static PCRE2 library, you must define `PCRE2_STATIC` before including `jpcre2.hpp` (or before `pcre2.h` if you included it manually).

**Install:**

You can copy this header to a standard include directory (folder) so that it becomes available from a standard include path.

**[Download](https://github.com/jpcre2/jpcre2/archive/release.zip) or clone the release branch unless you want the master (continuous dev) branch specifically:**

```bash
git clone --branch release https://github.com/jpcre2/jpcre2
```

On Unix you can do:

```sh
./configure
make
make install #(may require root privilege)
```

It will check if all dependencies are satisfied and install the header in a standard include path.

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

<a name="coding-guide"></a>

# Basic usage 

Performing a match or replacement against regex pattern involves two steps: 

1. Compiling the pattern
2. Performing the match or replacement operation


<a name="compile-a-regex-pattern"></a>

## Compile a regex pattern 

Select a character type according to the library you want to use. In this doc we are going to use 8 bit library as reference and we will use `char` as the character type. If `char` in your system is 16-bit you will have to link against 16-bit library instead, same goes for 32-bit. Other bit sizes are not supported by PCRE2.

Let's use a typedef to shorten the code:

```cpp
typedef jpcre2::select<char> jp;
// You have to select the basic data type (char, wchar_t, char16_t or char32_t)
```

<a name="a-regex-object"></a>

### A Regex object 

(You can use temporary objects too, see [short examples](#short-examples)).

This object will hold the pattern, options and compiled pattern.

```cpp
jp::Regex re;
```
Each object for each regex pattern.

<a name="compile-the-regex"></a>

### Compile the regex 

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

Now you can perform match or replace against the pattern. Use the `RegexMatch::match()` function to perform regex match and the `RegexReplace::replace()` member function to perform regex replace.

<a name="check-regex"></a>

### Check if regex compiled successfully 

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

<a name="match"></a>

## Match 

Match is generally performed using the `jp::RegexMatch::match()` function.

For convenience, a shortcut function in `Regex` is available: `jp::Regex::match()`. It can take upto three arguments. It uses a temporary match object to perform the match.

To get match results, you will need to pass vector pointers that will be filled with match data.

<a name="check-if-a-string-matches-a-regex"></a>

### Check if a string matches a regex 

```cpp
jp::Regex re("\\w+ect");

if(re.match("I am the subject")) //always uses a new temporary match object
    std::cout<<"matched (case sensitive)";
else
    std::cout<<"Didn't match";

//For case insensitive match, re-compile with modifier 'i'
re.addModifier("i").compile();

if(re.match("I am the subjEct")) //always uses a new temporary match object
    std::cout<<"matched (case insensitive)";
else
    std::cout<<"Didn't match";
```

<a name="simple-match-count"></a>

### Get match count 

```cpp
size_t count = jp::Regex("[aijst]","i").match("I am the subject","g"); //always uses a new temporary match object
```
The `g` modifier performs global match.

<a name="do-match"></a>

### Get match result 

To get the match results, you need to pass appropriate vector pointers. This is an example of how you can get the numbered substrings/captured groups from a match:


```cpp
jp::VecNum vec_num;
jp::RegexMatch rm;
size_t count=rm.setRegexObject(&re)                          //set associated Regex object
               .setSubject(&subject)                         //set subject string
               .addModifier(ac_mod)                          //add modifier
               .setNumberedSubstringVector(&vec_num)         //pass pointer to VecNum vector
               .match();                                     //Finally perform the match.
//vec_num will be populated with vectors of numbered substrings.
//count is the total number of matches found
```
<a name="access-a-capture-group"></a>

### Access a captured group 

You can access a substring/captured group by specifying their index (position):

```cpp
std::cout<<vec_num[0][0]; // group 0 in first match
std::cout<<vec_num[0][1]; // group 1 in first match
std::cout<<vec_num[1][0]; // group 0 in second match
```
<a name="get-named-capture-group"></a>

### Get named capture group 

To get named substring and/or name to number mapping, pass pointer to the appropriate vectors with `jp::RegexMatch::setNamedSubstringVector()` and/or `jp::RegexMatch::setNameToNumberMapVector()` before doing the match.

```cpp
jp::VecNum vec_num;   ///Vector to store numbered substring vector.
jp::VecNas vec_nas;   ///Vector to store named substring Map.
jp::VecNtN vec_ntn;   ///Vector to store Named substring to Number Map.
std::string ac_mod="g";   // g is for global match. Equivalent to using setFindAll() or FIND_ALL in addJpcre2Option()
jp::RegexMatch rm;
rm.setRegexObject(&re)
  .setSubject(&subject)                        //set subject string
  .addModifier(ac_mod)                         //add modifier
  .setNumberedSubstringVector(&vec_num)        //pass pointer to vector of numbered substring vectors
  .setNamedSubstringVector(&vec_nas)           //pass pointer to vector of named substring maps
  .setNameToNumberMapVector(&vec_ntn)          //pass pointer to vector of name to number maps
  .match();                                    //Finally perform the match()

```

<a name="access-substring-by-name"></a>

### Access a capture group by name 

```cpp
std::cout<<vec_nas[0]["name"]; // captured group by name in first match
std::cout<<vec_nas[1]["name"]; // captured group by name in second match
```

<a name="get-number-to-name"></a>

### Get the position of a capture group name 

If you need this information, you should have passed a `jp::VecNtN` pointer to `jp::RegexMatch::setNameToNumberMapVector()` function before doing the match ([see above](#get-named-capture-group)).

```cpp
std::cout<<vec_ntn[0]["name"]; // position of captured group 'name' in first match
```

<a name="iterate"></a>

### Iterate through match result 

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

<a name="independent-match-object"></a>

### RegexMatch object 

Every match object needs to be associated with a Regex object. A match object without regex object associated with it, will always give 0 match.

```cpp
jp::RegexMatch rm;
rm.setRegexObject(&re);

//Another way is to use constructor
jp::RegexMatch rm1(&re);

size_t count = rm.setSubject("subject")
                 .setModifier("g")
                 .match();
```
The `RegexMatch` class stores a pointer to its' associated Regex object. If the content of the associated Regex object is changed, it will be reflected on the next operation/result.

<a name="replace"></a>

## Replace or Substitute 

Regex replace is generally performed using the `jp::RegexReplace::replace()` function.

However, a convenience shortcut function is available in Regex class: `jp::Regex::replace(subject, replacewith, modifier)`. It uses a temporary replace object to perform the replacement.


<a name="simple-replace"></a>

### Simple replacement 


```cpp
//Using a temporary regex object
std::cout<<jp::Regex("\\d+").replace("I am digits 1234 0000","5678", "g");
//'g' modifier is for global replacement
//1234 and 0000 gets replaced with 5678
```

<a name="using-method-chain"></a>

### Using method chain 

```cpp
jp::RegexReplace rr;
std::cout<<
rr.setRegexObject(&re)  //set associated Regex object
  .setSubject(&s)       //Set various parameters
  .setReplaceWith(&s2)  //...
  .addModifier("gE")    //...
  .addJpcre2Option(0)   //...
  .addPcre2Option(0)    //...
  .replace();           //Finally do the replacement.
//gE is the modifier passed (global and unknown-unset-empty).
//Access substrings/captured groups with ${1234},$1234 (for numbered substrings)
// or ${name} (for named substrings) in the replacement part i.e in setReplaceWith()
```

<a name="independent-replace-object"></a>

### RegexReplace object 

Every replace object needs to be associated with a Regex object. A replace object not associated with any Regex object will perform no replacement and return the same subject string that was given.

```cpp
jp::RegexReplace rr;
rr.setRegexObject(&re);

//Another way is to use constructor
jp::RegexReplace rr1(&re);

rr.setSubject("subjEct")
  .setReplaceWith("me")
  .setModifier("g")
  .replace();
```
The `RegexReplace` class stores a pointer to its' associated Regex object. If the content of the associated Regex object is changed, it will be reflected on the next operation/result.

<a name="matchevaluator"></a>

### MatchEvaluator 

The `jp::RegexReplace` class has two replace functions: `jp::RegexReplace::replace()` and `jp::RegexReplace::nreplace()`. Both of them can take a `jp::MatchEvaluator` instance as argument and perform the replace operation according to the callback function set in the MatchEvaluator class.

And those two are just wrappers of `jp::MatchEvaluator::replace()` and `jp::MatchEvaluator::nreplace()`. Using these functions directly, one can re-use existing match data for new replacement operation without doing the match again. Though, this facility comes with some quirks, <a href="#re-use-match-data">see Re-use match data section.</a>. By default all replace functions do a new match every time and re-create the match data.

The first function mentioned (`replace()`) above, is for PCRE2 compatible replacement which uses `pcre2_substitute` to process the replacement string returned by the callback function, where the second one (`nreplace()`) uses a native approach without using `pcre2_substitute` which treats the string returned by the callback function as literal.

The class `MatchEvaluator` implements several constructor overloads to take different callback functions. Also, there are setter functions which allow changing the callback functions if desired.

The callback function takes exactly three positional arguments. If you don't need one or more arguments, you may pass `void*` in their respective positions in the argument list.

<a name="simple-example"></a>

#### Simple example 

The callback function:
```cpp
jp::String callback1(const jp::NumSub& m1, void*, void*){
    return "("+m1[0]+")";
}
```
then,
```cpp
jp::Regex re("(?<total>\\w+)", "n");
jp::RegexReplace rr;
String s3 = "I am a string 879879 fdsjkll ১ ২ ৩ ৪ অ আ ক খ গ ঘ";

rr.setRegexObject(&re)
  .setSubject(&s3)
  .setModifier("g");
  
std::cout<<"Result:\n"<<
    rr.nreplace(jp::MatchEvaluator(callback1)); //replace() function can take the same argument
```
Detailed examples are in the testme.cpp file.

<a name="matchevaluator-with-lambda"></a>

#### MatchEvaluator with lambda 

```cpp
std::cout<<"Result:\n"<<
    rr.nreplace(jp::MatchEvaluator
               (
                 [](const jp::NumSub& m1, const jp::MapNas& m2, void*){
                    return "("+m1[0]+"/"+m2.at("total")+")";
                 }
               ));
//replace() function can take the same argument
```

<a name="matchevaluator-only"></a>

#### MatchEvaluator only 

Replacement can be done with only MatchEvaluator:

```cpp
std::cout<<"Result:\n"<<
jp::MatchEvaluator(callback1).setSubject(&s3)
                             .setRegexObject(&re)
                             .setModifier("g")
                             .nreplace();
//replace() function can take the same argument
```


<a name="matchevaluator-object"></a>

#### MatchEvaluator object 

A `MatchEvaluator` object can be created using one of its many constructors. Callback functions can be provided with the constructors or can be changed later with `jp::MatchEvaluator::setCallback()` function. If no callback function is set/given, then the default callback function is `jp::callback::erase()` which deletes matched part/s from the subject string.

```cpp
jp::MatchEvaluator me; //default callback jp::callback::erase
me.setRegexObject(&re).setSubject(&sub).nreplace(); //this will remove matched parts from sub.

jp::MatchEvaluator me1(callback1); //arbitrary callback function.

jp::MatchEvaluator me2(&re); //default callback jp::callback::erase
me2.setSubject(sub).nreplace(); //this will remove matched parts from sub.
```

<a name="re-use-match-data"></a>

#### Re-use match data 

It is possible to use existing match data to perform replacement without performing a new match operation.

**Safest way but not the best:**

```cpp
jp::MatchEvaluator me(jp::callback::fill); //this callback implements all vectors and does not modify subject string.
//Now you need to populate the vectors with match data:
me.setSubject(&sub).setRegexObject(&re).match();

//Now that we have all the match data we need, we can use it to perform replacement according to
//different callback functions:
me.setCallback(callback2).nreplace(false); //'false' tells nreplace() to not perform new match.
me.setCallback(callback3).nreplace(false);
//etc..
```

**Best but not the safest:**

Instead of creating data for all vectors, you can do it as necessary, but it requires you to be vigilant about what you are doing:

```cpp
jp::MatchEvaluator me; //no vector with jp::callback::erase callback
me.setSubject(sub).setRegexObject(&re); //no data yet.
```
Let's say, we have a callback `cb3` that implements NumSub and MapNas and we do this:

```cpp
me.setCallback(cb3).nreplace(); 
//this creates match data for NumSub and MapNas and performs the replacement.
```
Now, if we want to perform the replacement with a different callback function `cb2` which implements only MapNas or NumSub or both, we can re-use the data created above:

```cpp
me.setCallback(cb2).nreplace(false);
```

If we want to use a callback function `cb4` which implements `jp::MapNtN`, we can not re-use the existing data because there is no data for `jp::MapNtn` yet. (it will give assertion error if we try). Thus we will need to do the match again:

```cpp
me.setCallback(cb4).nreplace(); //creating data again and performing replacement.
```
After the above operation, all the vectors are filled with data (missing `jp::MapNtn` was created), consequently, we can use any callback function we want at this stage because we have all the data that we will need.

Thus a callback `cb7` that implements all match data vectors can be used without doing the match again:
```cpp
me.setCallback(cb7).nreplace(false); //OK, as we have all the data we need.
```

**Quirks:**

1. Changes in replace related option takes effect without a re-match.
2. Changes in match related option (e.g start offset) needs a re-match to take effect.
3. To re-use existing match data, callback function must be compatible with the data, otherwise it's an assertion failure.
4. If the associated Regex object or subject string changes, a new match must be performed, trying to use the existing match data in such cases is undefined behavior.

**Make sure you at least understand the #3 and #4 points above before going for practical implementation of re-using match data. see jpcre2::select::MatchEvaluator for details**

<a name="modifiers"></a>

# Modifiers 

**JPCRE2** uses a default set of modifier to provide an easy path to setting different options for different operations. There are three basic operations, namely compile, match and replace and thus the set is divided into three subset of modifiers. For convenience, we call them modifier tables.

If the default modifier table is not suitable for your application, you may use a custom modifier table instead of the default one. The `jpcre2::ModifierTable` class provides this interface. (note the namespace, it's directly under `jpcre2`).

> All modifier strings are parsed and converted to equivalent PCRE2 and JPCRE2 options on the fly. If you don't want it to spend any time parsing modifier then pass the equivalent option directly with one of the many variants of `addJpcre2Option()` and `addPcre2Option()` functions.

**Types of modifiers:** 

1. Compile modifier
2. Match modifier
3. Replace modifier

All of the modifiers above can be divided further into two categories:

1. Unique modifier
2. Combined or mixed modifier (e.g 'n', 'E')


<a name="compile-modifier"></a>

## Default Compile modifiers 

These modifiers define the behavior of a regex pattern (they are integrated in the compiled regex). They have more or less the same meaning as the [PHP regex modifiers](https://php.net/manual/en/reference.pcre.pattern.modifiers.php) except for `e, j and n` (marked with <sup>\*</sup>). 

Modifier | Details
-------- | -------
`e`<sup>\*</sup> | Unset back-references in the pattern will match to empty strings. Equivalent to `PCRE2_MATCH_UNSET_BACKREF`.
`i` | Case-insensitive. Equivalent to `PCRE2_CASELESS` option.
`j`<sup>\*</sup> | `\u \U \x` and unset back-references will act as JavaScript standard. Equivalent to PCRE2_ALT_BSUX \| PCRE2_MATCH_UNSET_BACKREF. <ul><li><code>\U</code> matches an upper case "U" character (by default it causes a compile error if this option is not set).</li><li><code>\u</code> matches a lower case "u" character unless it is followed by four hexadecimal digits, in which case the hexadecimal number defines the code point to match (by default it causes a compile error if this option is not set).</li><li><code>\x</code> matches a lower case "x" character unless it is followed by two hexadecimal digits, in which case the hexadecimal number defines the code point to match (By default, as in Perl, a hexadecimal number is always expected after <code>\x</code>, but it may have zero, one, or two digits (so, for example, <code>\xz</code> matches a binary zero character followed by z) ).</li><li>Unset back-references in the pattern will match to empty strings.</li></ul>
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


## Default Replace or Match modifiers 

These modifiers are not compiled in the regex itself, rather they are used per call of each match or replace function.

Modifier | Action | Details
-------- | ------ | -------
`A` | match | Match at start. Equivalent to `PCRE2_ANCHORED`. Can be used in match operation. Setting this option only at match time (i.e regex was not compiled with this option) will disable optimization during match time.
`e` | replace | Replaces unset group with empty string. Equivalent to `PCRE2_SUBSTITUTE_UNSET_EMPTY`.
`E` | replace | Extension of `e` modifier. Sets even unknown groups to empty string. Equivalent to `PCRE2_SUBSTITUTE_UNSET_EMPTY | PCRE2_SUBSTITUTE_UNKNOWN_UNSET`
`g` | match<br>replace | Global. Will perform global matching or replacement if passed. Equivalent to `jpcre2::FIND_ALL` for match and `PCRE2_SUBSTITUTE_GLOBAL` for replace.
`x` | replace | Extended replacement operation. Equivalent to `PCRE2_SUBSTITUTE_EXTENDED`. It enables some Bash like features:<br>`${<n>:-<string>}`<br>`${<n>:+<string1>:<string2>}`<br>`<n>` may be a group number or a name. The first form specifies a default value. If group `<n>` is set, its value is inserted; if not, `<string>` is expanded and the result is inserted. The second form specifies strings that are expanded and inserted when group `<n>` is set or unset, respectively. The first form is just a convenient shorthand for `${<n>:+${<n>}:<string>}`.


<a name="modifier-table"></a>

## Modifier table 

Modifier table is an instance of the `jpcre2::ModifierTable` class. You can bind this table with any of the compile, match and replace related class objects. Different objects can have different tables.

Examples:

```cpp
        /* ***************************
         * Compile modifier table 
         * ***************************/
             
//character table is either std::string or const char* (not jp::String)
std::string nametab = "IJMS"; //arbitrary modifier characters.
//now the option values sequentially
jpcre2::Uint valtab[] = { PCRE2_CASELESS, PCRE2_DUPNAMES, PCRE2_MULTILINE, jpcre2::JIT_COMPILE };
//if the above two doesn't have the same number of elements, the behavior is undefined.

//init ModifierTable
jpcre2::ModifierTable mdt; //creates empty table.

//change the Compile modifier table only:
mdt.setCompileModifierTable(nametab, valtab);

//now bind the table with the object
jp::Regex re;
re.setModifierTable(&mdt);

//let's perform a compile
re.compile("JPCRE2","I"); //now I is PCRE2_CASELESS and small 'i' is an invalid modifier.
```

For details, see the `testmd.cpp` file.


<a name="options"></a>

# Options 

JPCRE2 allows both PCRE2 and native JPCRE2 options to be passed. PCRE2 options are recognized by the PCRE2 library itself.

<a name="jpcre-options"></a>

## JPCRE2 options 

These options are meaningful only for the **JPCRE2** library, not the original **PCRE2** library. We use the `jp::Regex::addJpcre2Option()` family of functions to pass these options.

Option | Details
------ | ------
`jpcre2::NONE` | This is the default option. Equivalent to 0 (zero).
`jpcre2::FIND_ALL` | This option will do a global match if passed during matching. The same can be achieved by passing the 'g' modifier with `jp::RegexMatch::addModifier()` function.
`jpcre2::JIT_COMPILE` | This is same as passing the `S` modifier during pattern compilation.

<a name="pcre2-options"></a>

## PCRE2 options 

We use the `jp::Regex::addPcre2Option()` family of functions to pass the PCRE2 options. These options are the same as the PCRE2 library and have the same meaning. For example instead of passing the 'g' modifier to the replacement operation we can also pass its PCRE2 equivalent `PCRE2_SUBSTITUTE_GLOBAL` to have the same effect. Passing these options directly will be faster than passing modifiers.

<a name="jpcre2-vs-pcre2-spec"></a>

# JPCRE2 vs PCRE2 spec 

This is where deviations from the PCRE2 specification will be laid out.

Details | PCRE2 | JPCRE2
------- | ----- | ------
Different name for same group | not supported (`10.21`) | supported (`>=10.30.01`)

<a name="code-unit-and-character-type"></a>

# Code unit width & character type 

The bit size of character type must match with the PCRE2 library you are linking against. There are three PCRE2 libraries according to code unit width, namely 8, 16 and 32 bit libraries. So, if you use a character type (e.g `char` which is generally 8 bit) of 8-bit code unit width then you will have to link your program against the 8-bit PCRE2 library. If it's 16-bit character, you will need 16-bit library. If you use a combination of various code unit width supported or use all of them, you will have to link your program against their corresponding PCRE2 libraries. Missing library will yield to compile time error.

**Implementation defined behavior:**

Size of integral types (`char`, `wchar_t`, `char16_t`, `char32_t`) is implementation defined. `char` may be 8, 16, 32 or 64 (not supported) bit. Same goes for `wchar_t` and others. In Linux `wchar_t` is 32 bit and in windows it's 16 bit.

<a name="portable-coding"></a>

# Portable coding 

<a name="code-unit-width-quirk"></a>

## Code unit width quirk 

JPCRE2 codes are portable in regards of code unit width. Your program gets compiled according to the code unit width defined by your system. Consider the following example, where you do:

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
4. In a system where `char` is not 8 or 16 or 32 bit, it will yield compile error.

If you don't want to be so aware of the code unit width of the character type/s you are using, link your program against all PCRE2 libraries. The code unit width will be handled automatically and if anything unsupported is encountered, you will get compile time error.

A common example in this regard can be the use of `wchar_t`:

```cpp
jpcre2::select<wchar_t>::Regex re;
```

1. In windows, the above code will use 16-bit library and UTF-16 in UTF mode.
2. In Linux, the above code will use 32-bit library and UTF-32 in UTF mode.


<a name="use-of-string-class"></a>

## Use of string class 

For portable code, instead of using the standard names `std::string` or such, use `jp::String` (you may further typedef it as `String` or whatever). It will be defined to an appropriate string class according to the basic character type you selected and thus provide all the functionalities and conveniences you get with `std::string` and such string classes. Being said that, there's no harm if you use the standard names (`std::string` etc...). Using `jp::String` will just ensure that you are using the correct string class for the correct character type you selected. If you need to use the basic character type, use `jp::Char`.

<a name="use-of-vectors-for-match-result"></a>

## Use of vectors for match result 

Instead of using full names like `std::vector<std::string>` and such for storing match result, use the typedefs:

1. `jp::NumSub`: Equivalent to `std::vector<jp::String>`
2. `jp::MapNas`: Equivalent to `std::map<jp::String, jp::String>` (You can set arbitrary map (e.g `std::unordered_map`) instead of `std::map` when using `>=C++11`)
3. `jp::MapNtN`: Equivalent to `std::map<jp::String, size_t>` (You can set arbitrary map (e.g `std::unordered_map`) instead of `std::map` when using `>=C++11`)
4. `jp::VecNum`: Equivalent to `std::vector<jp::NumSub>`
5. `jp::VecNas`: Equivalent to `std::vector<jp::MapNas>`
6. `jp::VecNtN`: Equivalent to `std::vector<jp::MapNtN>`
7. `jpcre2::VecOff`: Equivalent to `std::vector<size_t>` (note the namespace, it's directly under `jpcre2`)

<a name="other-typedefs"></a>

## Other typedefs 

Other typedefs are mostly for internal use

* You should not use the `jpcre2::Ush` as unsigned short. In JPCRE2 context, it is the smallest unsigned integer type to cover at least the numbers from 1 to 126.
* `jpcre2::Uint` is a fixed width unsigned integer type and will be at least 32 bit wide.
* `jpcre2::SIZE_T` is the same as `PCRE2_SIZE` which is defined as `size_t`.
* `jpcre2::VecOpt` is defined as `std::vector<jpcre2::Uint>`.


<a name="error-handling"></a>

# Error handling 

<a name="runtime-error"></a>

## Runtime error 

When a known error is occurred during pattern compilation or match or replace, the error number and error offsets are set to corresponding variables of the respective classes. You can get the error number, error offset and error message with `getErrorNumber()`, `getErrorOffset()` and `getErrorMessage()` functions respectively. These functions are available for all three classes.

**Note** that, these errors always gets overwritten by previous error, so you only get the last error that occurred.

**Also note** that, these errors never get re-initialized (set to zero), they are always there even when everything else worked great (except some previous error).

If you do experiment with various erroneous situations, make use of the `resetErrors()` function. You can call it from anywhere in your method chain and immediately set the errors to zero. This function is also defined for all three classes to reset their corresponding errors.

<a name="assertion-failure"></a>

## Assertion failure 

JPCRE2 asserts some errors with descriptive error messages. These errors are mistakes in your code and not to be shipped to the client without fixing.

In no situation these errors should be bypassed by `#define NDEBUG` before including `jpcre2.hpp`. You should investigate the error message and fix the cause.

> When there is no such errors in your finalized code, you may use `#define NDEBUG` to strip out these assertions.

<a name="null-safety"></a>

# Null safety 

JPCRE2 treats null as valid input and its usage have well-defined behavior throughout JPCRE2 interface. Most of the time a null is treated as 'set something to its initial or empty state'. And also, initial state doesn't necessarily have to be an empty state, and empty state doesn't necessarily have to be an initial state. It depends on what you are working with, refer to the [doc](http://docs.neurobin.org/jpcre2) when you are in a bind.

As an example, if null is passed with `setSubject()`, then the subject is set to its initial state which is empty (not null).

Another example is, when a null is passed to the `setRegexObject()` function, it literally sets the Regex object to null, which is actually the initial state for that calling object.

<a name="null-safety-with-std-string"></a>

## Null safety with std::string 

Giving a null to `std::string` (and such) constructor is undefined behavior. But you don't need to worry about it with JPCRE2, if it's too much to type Two double quotes (`""`) to pass an empty string to a JPCRE2 function, you can just use `0`, it's perfectly fine. But it's a bad practice, so just use this statement as a safety measure.

**Note:** JPCRE2 is supposed to be completely null safe, i.e no undefined behavior for null input. So, if you find any loophole or bug that makes this statement invalid, please report it.

<a name="multi-threading"></a>

# Multi threading 

<a name="functions-thread-safety"></a>

## Functions 

<b>(C) MT safe:</b> _All functions in JPCRE2 library are MT safe provided that the instances calling those functions are themselves thread safe._

**When we say '(C) MT safe' or simply 'thread safe' throughout this doc, we mean the above definition of Conditional Multi-Thread safety.**

<a name="objects-thread-safety"></a>

## Objects 

1. There is no data race between two separate objects (`Regex`, `RegexMatch`, `RegexReplace` etc..) because the classes do not contain any static variables.
2. Temporary class objects are always thread safe.
3. Temporary class object that uses another third party object reference or pointer is thread safe provided that the access to the third party object is thread safe.
4. Simultaneous access of the same object is MT unsafe. You can use mutex lock or other mechanisms to ensure thread safety.


**Examples:**

The following function is thread safe:

```cpp
typedef jpcre2::select<char> jp;

void* thread_safe_fun1(void* arg){//uses no global or static variable, thus thread safe.
    jp::Regex re("\\w", "g");
    jp::RegexMatch rm(&re); //It's a local variable
    rm.setSubject("fdsf").setModifier("g").match();
    return 0;
}
```

The following function is thread safe for joined thread only:

```cpp
jp::Regex rec("\\w", "g"); //thread unsafe.

void *thread_pseudo_safe_fun1(void *arg){
    //uses global variable 'rec', but uses
    //mutex lock, thus thread safe when the thread is joined with the main thread.
    //But, when thread is detached from the main thread, it won't be thread safe any more,
    //because, the main thread can destroy the rec object while possibly being used by the detached child thread.
    pthread_mutex_lock( &mtx );
    jp::RegexMatch rm(&rec);
    rm.setSubject("fdsf").setModifier("g").match();
    pthread_mutex_unlock( &mtx);
    return 0;
}
```

Example multi-threaded programs are provided in *src/test_pthread.cpp* and *src/teststdthread.cpp*. The thread safety of these programs are tested with Valgrind (`drd` tool). See <a href="#test-suit">Test suit</a> for more details on the test.


<a name="compatibility-with-compilers"></a>

# Compatibility with compilers 

* To use JPCRE2 in its full capability (including `>=C++11` features), use latest compilers with full `C++11` support.
* If you do not use `>=C++11`, you will be OK with older compilers.

<a name="examples"></a>

# Examples 

Examples and test programs are available in `src/test*.cpp` files.

File | Containing examples
---- | -------------------
`test0.cpp` | Handling `std::string` and `std::wstring`.
`test16.cpp` | Performing regex match and regex replace with `std::wstring` and `std::u16string`.
`test32.cpp` | Performing regex match and regex replace with `std::wstring` and `std::u32string`.
`test_match.cpp` | Performing regex match against a pattern and getting the match count and match results. Shows how to iterate over the match results to get the captured groups/substrings.
`test_match2.cpp` | Contains an example to take subject string, pattern and modifier from user input and perform regex match using JPCRE2.
`testmd.cpp` | Examples of working with modifier table.
`testme.cpp` | Examples of using MatchEvaluator to perform replace.
`test_replace.cpp` | Example of doing regex replace.
`test_replace2.cpp` |  Contains an example to take subject string, replacement string, modifier and pattern from user input and perform regex replace with JPCRE2
`test_pthread.cpp` | Multi threaded examples with POSIX pthread.
`teststdthread.cpp` |  Multi threaded examples with `std::thread`.
`test_shorts.cpp` | Contains some short examples.

<a name="short-examples"></a>

# Short examples 

```cpp
size_t count;
//Check if string matches the pattern
/*
 * The following uses a temporary Regex object.
 */
if(jp::Regex("(\\d)|(\\w)").match("I am the subject"))
    std::cout<<"\nmatched";
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
jp::RegexMatch rm;
jp::Regex re("(\\w+)\\s*(\\d+)","m");
count =
jp::RegexMatch(&re).setSubject("I am 23, I am digits 10")
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
    //~ std::cerr<<"\n"<<e.what();
//~ }


//There were two matches found (vec_num.size() == 2) in the above example
std::cout<<"\nTotal match of second match: "<<vec_num[1][0];      //Total match (group 0) from second match
std::cout<<"\nCaptured group 1 of second match: "<<vec_num[1][1]; //captured group 1 from second match
std::cout<<"\nCaptured group 2 of second match: "<<vec_num[1][2]; //captured group 2 from second match


// ***** Get named substring ***** //

jp::VecNas vec_nas;
jp::VecNtN vec_ntn; // We will get name to number map vector too
re.compile("(?<word>\\w+)\\s*(?<digit>\\d+)","m");
count =
jp::RegexMatch(&re).setSubject("I am 23, I am digits 10")
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
re.compile("^([^\t]+)\t([^\t]+)$");
jp::RegexReplace(&re).setSubject("I am the subject\tTo be swapped according to tab")
                     .setReplaceWith("$2 $1")
                     .replace();
```

<a name="test-suit"></a>

# Test suit 

Some test programs are written to check for major flaws like segfault, memory leak and crucial input/output validation. Before trying to run the tests, make sure you have all 3 PCRE2 libraries installed on your system.

For the simplest (minimal) test, run:

```sh
#You can add --enable-cpp11 to test cpp11 features.
./configure --enable-test
make check
```
To check with `valgrind`, run:

```sh
#requires valgrind to be installed on the system
#You can add --enable-cpp11 to test cpp11 features.
./configure --enable-valgrind
make check
```
To check the multi threaded examples with `drd`, run:

```sh
#requires valgrind to be installed on the system
#You can add --enable-cpp11 to test cpp11 features.
./configure --enable-thread-check
make check
```

To prepare a coverage report, run:

```sh
#requires lcov and genhtml to be installed on the system
#enable cpp11 to cover cpp11 codes.
#clean any previous make
make distclean #ignore errors
./configure --enable-coverage --enable-cpp11
make coverage
```

<a name="the-configure-script"></a>

# The configure script 

The configure script generated by autotools checks for availability of several programs and let's you set several options to control your testing environment. These are the options supported by configure scipt:

Option | Details
------ | -------
`--[enable/disable]-test` | Enable/Disable test suit.
`--[enable/disable]-cpp11` | Enable/Disable building tests with C++11 features.
`--[enable/disable]-valgrind` | Enable/Disable valgrind test (memory leak test).
`--[enable/disable]-thread-check` | Enable/Disable thread check on multi threaded examples.
`--[enable/disable]-coverage` | Enable/Disable coverage report.
`--[enable/disable]-silent-rules` | Enable/Disable silent rules (enabled by default). You will get prettified `make` output if enabled.

<a name="contributing-pull-request"></a>

# Contributing/Pull request 

~~**Please do all pull requests against the master branch. The default branch is 'release' which is not where continuous development of JPCRE2 is done.**~~

If you find any error in the documentation or confusing/misleading use of terms, or anything that cathces your eye and feels not right, please open an issue in the [issue page](https://github.com/jpcre2/jpcre2/issues). Or if you want to fix it and do pull request then use the master branch.

This page is generated from doxy/doxydoc.md file, thus changing the README.md file will have no impact.

<a name="licence"></a>

# LICENCE 
This project comes with a BSD LICENCE, see the LICENCE file for more details.

It is not necessary to let me know which project you are using this library on, but an optional choice. I would very much appreciate it, if you let me know about the name (and short description if applicable) of the project. So if you have the time, please send me an [email](https://neurobin.org/about/contact/?s=Using+jpcre2+in+a+project&m=I+am+using+jpcre2+in+the+following+project%3A%0A%0AProject+Name%3A+%0AShort+description%3A%0A%0AYou+can+share+the+project+name+publicly%3A+%5Byes%2Fno%5D%0AYou+can+share+the+project+description+publicly%3A+%5Byes%2Fno%5D%0AYou+can+share+the+project+author+name+publicly%3A+%5Byes%2Fno%5D%0AEmail+will+be+private+and+not+shared%3A+yes%0A).


