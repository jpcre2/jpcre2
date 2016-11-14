AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
include([ax_cxx_compile_stdcxx.m4])

AC_INIT([jpcre2], [10.28.06], [https://github.com/jpcre2/jpcre2/issues])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/jpcre2.hpp])
#AC_CONFIG_HEADERS([config.h])

#AM_INIT_AUTOMAKE
AM_INIT_AUTOMAKE([foreign parallel-tests color-tests])

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
AC_CHECK_FUNCS([setlocale, free])

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

# Handle --enable-test-all (disabled by default)
AC_ARG_ENABLE(test-all,
              AS_HELP_STRING([--enable-test-all],
                             [enable all test]),
              , enable_test_all=unset)
AC_SUBST(enable_test_all)

# Handle --enable-cpp11 (disabled by default)
AC_ARG_ENABLE(cpp11,
              AS_HELP_STRING([--enable-cpp11],
                             [enable compiling with cpp11]),
              , enable_cpp11=unset)
AC_SUBST(enable_cpp11)

AC_ARG_ENABLE(coverage,
              AS_HELP_STRING([--enable-coverage],
                              [use Gcov and lcov to test the test suite]),
              , enable_coverage=unset)
AC_SUBST(enable_coverage)

AC_ARG_ENABLE(valgrind,
              AS_HELP_STRING([--enable-valgrind],
                              [use valgrind to test the test suite]),
              , enable_valgrind=unset)
AC_SUBST(enable_valgrind)


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

# Set the default value for test-all
if test "x$enable_test_all" = "xunset"
then
  enable_test_all=no
fi

if test "x$enable_coverage" = "xunset"
then
  enable_coverage=no
fi

if test "x$enable_valgrind" = "xunset"
then
  enable_valgrind=no
fi

# Here is where JPCRE2-specific defines are handled

if test "$enable_coverage" = "yes"; then
  AC_DEFINE([SUPPORT_COVERAGE], [], [
    Define to any value to enable coverage test])
  AC_CHECK_PROG([gcov_test], [gcov], [yes])
  if test "$gcov_test" != yes; then
    AC_MSG_ERROR([missing program: gcov])
  fi
  AC_CHECK_PROG([lcov_test], [lcov], [yes])
  if test "$lcov_test" != yes; then
    AC_MSG_ERROR([missing program: lcov])
  fi
  enable_test_all=yes
  CXXFLAGS= #`echo "$CXXFLAGS" | sed -e 's/-O[0-9]*//g'`
fi

if test "$enable_valgrind" = "yes"; then
  AC_DEFINE([SUPPORT_VALGRIND], [], [
    Define to any value to enable valgrind test])
  AC_CHECK_PROG([valgrind_test], [valgrind], [yes])
  if test "$valgrind_test" != yes; then
    AC_MSG_ERROR([missing program: valgrind])
  fi
  enable_test_all=yes
fi


if test "$enable_silent_rules" = "yes"; then
  AM_SILENT_RULES([yes])
  AC_DEFINE([SUPPORT_SILENT_RULES], [], [
    Define to any value to enable compiling the test examples])
fi

if test "$enable_test_all" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_ALL], [], [
    Define to any value to enable the 32 bit PCRE2 library.])
  enable_test_8=yes
  enable_test_16=yes
  enable_test_32=yes
fi


if test "$enable_test_8" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_8], [], [
    Define to any value to enable the 8 bit JPCRE2 library.])
    
  AC_CHECK_LIB([pcre2-8], [pcre2_code_free_8], [], [AC_MSG_ERROR([unable to find pcre2-8 library])])
fi

if test "$enable_test_16" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_16], [], [
    Define to any value to enable the 16 bit JPCRE2 library.])
  AC_CHECK_LIB([pcre2-16], [pcre2_code_free_16], [], [AC_MSG_ERROR([unable to find pcre2-16 library])])
  enable_cpp11=yes
fi

if test "$enable_test_32" = "yes"; then
  AC_DEFINE([SUPPORT_TEST_32], [], [
    Define to any value to enable the 32 bit PCRE2 library.])
  AC_CHECK_LIB([pcre2-32], [pcre2_code_free_32], [], [AC_MSG_ERROR([unable to find pcre2-32 library])])
  enable_cpp11=yes
fi

if test "$enable_cpp11" = "yes"; then
  AC_DEFINE([SUPPORT_CPP11], [], [
    Define to any value to enable C++ 11])
  #check for c++11 support and add flags
  AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
  AC_CHECK_HEADERS([codecvt],[AC_MSG_ERROR([Unable to find codecvt header])])
fi


# Conditional compilation
AM_CONDITIONAL(WITH_COVERAGE, test "x$enable_coverage" = "xyes")
AM_CONDITIONAL(WITH_VALGRIND, test "x$enable_valgrind" = "xyes")
AM_CONDITIONAL(WITH_CPP11, test "x$enable_cpp11" = "xyes")
AM_CONDITIONAL(WITH_TEST_8, test "x$enable_test_8" = "xyes")
AM_CONDITIONAL(WITH_TEST_16, test "x$enable_test_16" = "xyes")
AM_CONDITIONAL(WITH_TEST_32, test "x$enable_test_32" = "xyes")
AM_CONDITIONAL(WITH_TEST_ALL, test "x$enable_test_all" = "xyes")
AM_CONDITIONAL(WITH_TEST_SUIT, test "x$enable_valgrind" = "xyes" || test "x$enable_coverage" = "xyes")


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
    Build Multi-bit examples .......... : ${enable_test_all}
    Build with C++ 11 ................. : ${enable_cpp11}
    Enable Silent Rules ............... : ${enable_silent_rules}
    Coverage report ................... : ${enable_coverage}
    Valgrind test ..................... : ${enable_valgrind}

EOF
