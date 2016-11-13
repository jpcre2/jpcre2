AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
include([ax_cxx_compile_stdcxx.m4])

AC_INIT([jpcre2], [10.28.06], [https://github.com/jpcre2/jpcre2/issues])
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

# Handle --disable-test-8 (enabled by default)
AC_ARG_ENABLE(test-8,
              AS_HELP_STRING([--disable-test-8],
                             [disable 8 bit test]),
              , enable_test_8=unset)
AC_SUBST(enable_test_8)

# Handle --enable-test-16 (disabled by default)
AC_ARG_ENABLE(test-16,
              AS_HELP_STRING([--enable-test-16],
                             [enable 16 bit test]),
              , enable_test_16=unset)
AC_SUBST(enable_test_16)

# Handle --enable-test-32 (disabled by default)
AC_ARG_ENABLE(test-32,
              AS_HELP_STRING([--enable-test-32],
                             [enable 32 bit test]),
              , enable_test_32=unset)
AC_SUBST(enable_test_32)

# Handle --enable-test-multi (disabled by default)
AC_ARG_ENABLE(test-multi,
              AS_HELP_STRING([--enable-test-multi],
                             [enable multi bit test]),
              , enable_test_multi=unset)
AC_SUBST(enable_test_multi)

# Handle --enable-cpp11 (disabled by default)
AC_ARG_ENABLE(cpp11,
              AS_HELP_STRING([--enable-cpp11],
                             [enable compiling with cpp11]),
              , enable_cpp11=unset)
AC_SUBST(enable_cpp11)


# Set the default value for cpp11
if test "x$enable_cpp11" = "xunset"
then
  enable_cpp11=no
fi

# Set the default value silent-rules
if test "x$enable_silent_rules" = "xunset"
then
  enable_silent_rules=yes
fi

# Set the default value for test-8
if test "x$enable_test_8" = "xunset"
then
  enable_test_8=yes
fi

# Set the default value for test-16
if test "x$enable_test_16" = "xunset"
then
  enable_test_16=no
fi

# Set the default value for test-32
if test "x$enable_test_32" = "xunset"
then
  enable_test_32=no
fi

# Set the default value for test-multi
if test "x$enable_test_multi" = "xunset"
then
  enable_test_multi=no
fi


# Here is where JPCRE2-specific defines are handled
if test "$enable_silent_rules" = "yes"; then
  AM_SILENT_RULES([yes])
  AC_DEFINE([SUPPORT_SILENT_RULES], [], [
    Define to any value to enable compiling the test examples])
fi

if test "$enable_test_multi" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_MULTI], [], [
    Define to any value to enable the 32 bit PCRE2 library.])
  enable_test_8=yes
  enable_test_16=yes
  enable_test_32=yes
fi


if test "$enable_test_8" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_8], [], [
    Define to any value to enable the 8 bit JPCRE2 library.])
    
  AC_CHECK_LIB([pcre2-8], [pcre2_code_free_8], [], [AC_MSG_WARN([unable to find pcre2-8 library])])
fi

if test "$enable_test_16" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_16], [], [
    Define to any value to enable the 16 bit JPCRE2 library.])
  AC_CHECK_LIB([pcre2-16], [pcre2_code_free_16], [], [AC_MSG_WARN([unable to find pcre2-16 library])])
  enable_cpp11=yes
fi

if test "$enable_test_32" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_32], [], [
    Define to any value to enable the 32 bit PCRE2 library.])
  AC_CHECK_LIB([pcre2-32], [pcre2_code_free_32], [], [AC_MSG_WARN([unable to find pcre2-32 library])])
  enable_cpp11=yes
fi

if test "$enable_cpp11" = "yes"; then
  AC_DEFINE([SUPPORT_CPP11], [], [
    Define to any value to enable C++ 11] compilation)
  #check for c++11 support and add flags
  AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
  AC_CHECK_HEADERS([codecvt],[AC_MSG_ERROR([Unable to find codecvt header])])
fi




# Conditional compilation
AM_CONDITIONAL(WITH_CPP11, test "x$enable_cpp11" = "xyes")
AM_CONDITIONAL(WITH_TEST_8, test "x$enable_test_8" = "xyes")
AM_CONDITIONAL(WITH_TEST_16, test "x$enable_test_16" = "xyes")
AM_CONDITIONAL(WITH_TEST_32, test "x$enable_test_32" = "xyes")
AM_CONDITIONAL(WITH_TEST_MULTI, test "x$enable_test_multi" = "xyes")


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
    Build 8-bit examples .............. : ${enable_test_8}
    Build 16-bit examples ..............: ${enable_test_16}
    Build 32-bit examples ............. : ${enable_test_32}
    Build Multi-bit examples .......... : ${enable_test_multi}
    Build with C++ 11 ................. : ${enable_cpp11}
    Enable Silent Rules ............... : ${enable_silent_rules}

EOF
