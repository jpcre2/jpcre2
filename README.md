#C++ wrapper for several useful utility of PCRE2 Library

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

#Testing:

###test.cpp:

Contains an example code for match and replace function.

###test_match.cpp:

Contains an example code for match function.

###test_replace.cpp:
Contains an example code for replace function.

#Screenshots of some outputs:

![jpcre2 screenshot1](img/s1_1.png)
![jpcre2 screenshot2](img/s1_2.png)
![jpcre2 screenshot3](img/s1_3.png)
![jpcre2 screenshot4](img/s1_4.png)

