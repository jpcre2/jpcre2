AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
include([ax_cxx_compile_stdcxx.m4])

AC_INIT([jpcre2], [10.27.02], [https://github.com/jpcre2/jpcre2/issues])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/jpcre2.hpp])
#AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

AC_TYPE_INT64_T

# Checks for programs.

AC_PROG_INSTALL

#AM_DISABLE_SHARED

LT_INIT([shared])

AC_PROG_CXX
AC_PROG_CC


#CXXFLAGS=


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

AC_CHECK_HEADERS([pcre2.h],[AC_MSG_ERROR([Unable to find pcre2.h header])])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([setlocale])

# handle --disable-silent-rules (enabled by default)
AC_ARG_ENABLE(silent-rules,
              AS_HELP_STRING([--disable-silent-rules],
                             [Disable silent rules and run in verbose mode]),
              , enable_silent_rules=unset)
AC_SUBST(enable_silent_rules)

AC_ARG_ENABLE(jpcre2-test-compile,
              AS_HELP_STRING([--disable-jpcre2-test-compile],
                             [Do not compile the examples]),
              , enable_jpcre2_test_compile=unset)
AC_SUBST(enable_jpcre2_test_compile)

AC_ARG_ENABLE(jpcre2-test-compile-cpp11,
              AS_HELP_STRING([--enable-jpcre2-test-compile-cpp11],
                             [Compile the examples that require >=c++11]),
              , enable_jpcre2_test_compile_cpp11=unset)
AC_SUBST(enable_jpcre2_test_compile_cpp11)


# Handle --disable-jpcre2-8 (enabled by default)
AC_ARG_ENABLE(jpcre2-8,
              AS_HELP_STRING([--disable-jpcre2-8],
                             [disable 8 bit character support]),
              , enable_jpcre2_8=unset)
AC_SUBST(enable_jpcre2_8)

# Handle --enable-jpcre2-16 (disabled by default)
AC_ARG_ENABLE(jpcre2-16,
              AS_HELP_STRING([--enable-jpcre2-16],
                             [enable 16 bit character support]),
              , enable_jpcre2_16=unset)
AC_SUBST(enable_jpcre2_16)

# Handle --enable-jpcre2-32 (disabled by default)
AC_ARG_ENABLE(jpcre2-32,
              AS_HELP_STRING([--enable-jpcre2-32],
                             [enable 32 bit character support]),
              , enable_jpcre2_32=unset)
AC_SUBST(enable_jpcre2_32)

# Handle --enable-jpcre2-cpp11 (disabled by default)
AC_ARG_ENABLE(jpcre2-cpp11,
              AS_HELP_STRING([--enable-jpcre2-cpp11],
                             [enable compiling for cpp11]),
              , enable_jpcre2_cpp11=unset)
AC_SUBST(enable_jpcre2_cpp11)



if test "x$enable_jpcre2_test_compile" = "xunset"
then
  enable_jpcre2_test_compile=yes
fi



if test "x$enable_jpcre2_test_compile_cpp11" = "xunset"
then
  enable_jpcre2_test_compile_cpp11=no
else
  enable_jpcre2_cpp11=yes
fi

# Set the default value silent-rules
if test "x$enable_silent_rules" = "xunset"
then
  enable_silent_rules=yes
fi

# Set the default value for jpcre2-8
if test "x$enable_jpcre2_8" = "xunset"
then
  enable_jpcre2_8=yes
else
  enable_jpcre2_test_compile=no
fi

# Set the default value for jpcre2-16
if test "x$enable_jpcre2_16" = "xunset"
then
  enable_jpcre2_16=no
  enable_jpcre2_test_compile_cpp11=no
fi

# Set the default value for jpcre2-32
if test "x$enable_jpcre2_32" = "xunset"
then
  enable_jpcre2_32=no
  enable_jpcre2_test_compile_cpp11=no
fi

# Set the default value for jpcre2-cpp11
if test "x$enable_jpcre2_cpp11" = "xunset"
then
  enable_jpcre2_cpp11=no
fi

# Conditional compilation
AM_CONDITIONAL(WITH_JPCRE2_TEST_COMPILE, test "x$enable_jpcre2_test_compile" = "xyes")
AM_CONDITIONAL(WITH_JPCRE2_TEST_COMPILE_CPP11, test "x$enable_jpcre2_test_compile_cpp11" = "xyes")
AM_CONDITIONAL(WITH_JPCRE2_8, test "x$enable_jpcre2_8" = "xyes")
AM_CONDITIONAL(WITH_JPCRE2_16, test "x$enable_jpcre2_16" = "xyes")
AM_CONDITIONAL(WITH_JPCRE2_32, test "x$enable_jpcre2_32" = "xyes")
AM_CONDITIONAL(WITH_JPCRE2_CPP11, test "x$enable_jpcre2_cpp11" = "xyes")



# Here is where JPCRE2-specific defines are handled
if test "$enable_silent_rules" = "yes"; then
  AM_SILENT_RULES([yes])
  AC_DEFINE([SUPPORT_SILENT_RULES], [], [
    Define to any value to enable compiling the test examples])
fi

if test "$enable_jpcre2_test_compile" = "yes"; then
  AC_DEFINE([SUPPORT_JPCRE2_TEST_COMPILE], [], [
    Define to any value to enable compiling the test examples])
fi


if test "$enable_jpcre2_test_compile_cpp11" = "yes"; then
  AC_DEFINE([SUPPORT_JPCRE2_TEST_COMPILE_CPP11], [], [
    Define to any value to enable compiling c++11 examples])
    
  #check for c++11 support and add flags
  AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
  AC_CHECK_HEADERS([codecvt],[AC_MSG_ERROR([Unable to find codecvt header])])

fi


if test "$enable_jpcre2_8" = "yes"; then
  AC_DEFINE([SUPPORT_JPCRE2_8], [], [
    Define to any value to enable the 8 bit JPCRE2 library.])
    
  AC_CHECK_LIB([pcre2-8], [pcre2_code_free_8], [], [AC_MSG_WARN([unable to find pcre2-8 library])])
fi

if test "$enable_jpcre2_16" = "yes"; then
  AC_DEFINE([SUPPORT_JPCRE2_16], [], [
    Define to any value to enable the 16 bit JPCRE2 library.])
  AC_CHECK_LIB([pcre2-16], [pcre2_code_free_16], [], [AC_MSG_WARN([unable to find pcre2-16 library])])
fi

if test "$enable_jpcre2_32" = "yes"; then
  AC_DEFINE([SUPPORT_JPCRE2_32], [], [
    Define to any value to enable the 32 bit PCRE2 library.])
  AC_CHECK_LIB([pcre2-32], [pcre2_code_free_32], [], [AC_MSG_WARN([unable to find pcre2-32 library])])
fi

if test "$enable_jpcre2_cpp11" = "yes"; then
  AC_DEFINE([SUPPORT_JPCRE2_cpp11], [], [
    Define to any value to enable C++ 11] compilation)
  #check for c++11 support and add flags
  AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
  AC_CHECK_HEADERS([codecvt],[AC_MSG_ERROR([Unable to find codecvt header])])
fi



AC_OUTPUT(Makefile src/Makefile)

cat <<EOF

configuration summary:

    Install prefix .................... : ${prefix}
    C preprocessor .................... : ${CC}
    C++ compiler ...................... : ${CXX}
    Linker ............................ : ${LD}
    C++ preprocessor flags ............ : ${CXXFLAGS}
    C++ compiler flags ................ : ${CXXFLAGS}
    Linker flags ...................... : ${LDFLAGS}
    Extra libraries ................... : ${LIBS}
    Build with C++ 11 ................. : ${enable_jpcre2_cpp11}
    Build shared library .............. : ${enable_shared}
    Build static library .............. : ${enable_static}
    Build 8-bit jpcre2 library ........ : ${enable_jpcre2_8}
    Build 16-bit jpcre2 library ....... : ${enable_jpcre2_16}
    Build 32-bit jpcre2 library ....... : ${enable_jpcre2_32}
    Build examples .................... : ${enable_jpcre2_test_compile}
    Build >= C++11 examples ........... : ${enable_jpcre2_test_compile_cpp11}
    Enable Silent Rules ............... : ${enable_silent_rules}

EOF
