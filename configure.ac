AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
include([ax_cxx_compile_stdcxx.m4])
include([acx_pthread.m4])

AC_INIT([jpcre2], [10.28.09], [https://github.com/jpcre2/jpcre2/issues])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/jpcre2.hpp])
#AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE
#AM_INIT_AUTOMAKE([foreign parallel-tests color-tests])

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

# Handle --enable-test (disabled by default)
AC_ARG_ENABLE(test,
              AS_HELP_STRING([--enable-test],
                             [enable test]),
              , enable_test=unset)
AC_SUBST(enable_test)

# Handle --enable-test (disabled by default)
AC_ARG_ENABLE(thread-check,
              AS_HELP_STRING([--enable-thread-check],
                             [enable thread check]),
              , enable_thread_check=unset)
AC_SUBST(enable_thread_check)

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

# Set the default value for test
if test "x$enable_test" = "xunset"
then
  enable_test=no
fi

# Set default value for enable_thread_check
if test "x$enable_thread_check" = "xunset"
then
  enable_thread_check=no
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
  
  AC_ARG_VAR([LCOV],[the ltp lcov program])
  AC_PATH_PROG([LCOV],[lcov],[false])
  if test "x$LCOV" = "xfalse"; then
    AC_MSG_ERROR([lcov not found])
  fi

  AC_ARG_VAR([GENHTML],[the ltp genhtml program])
  AC_PATH_PROG([GENHTML],[genhtml],[false])
  if test "x$GENHTML" = "xfalse"; then
    AC_MSG_ERROR([genhtml not found])
  fi
  enable_test=yes
  CXXFLAGS="-O0 -ggdb3 -fprofile-arcs -ftest-coverage"
fi


if test "$enable_thread_check" = "yes"; then
  AC_DEFINE([SUPPORT_THREAD_CHECK], [], [
    Define to any value to enable thread check])
  LIBS="$LIBS -lpthread"
  AC_ARG_VAR([VALGRIND],[the valgrind program])
  AC_PATH_PROG([VALGRIND],[valgrind],[false])
  if test "x$VALGRIND" = "xfalse"; then
    AC_MSG_ERROR([valgrind not found])
  fi
  #enable_valgrind=yes
#  AC_CHECK_HEADERS([pthread.h],[AC_MSG_ERROR([Unable to find pthread.h header])])
#  AC_CHECK_LIB([pthread], [pthread_exit((void*) 0)], [], [AC_MSG_ERROR([unable to find pthread library])])
fi

if test "$enable_valgrind" = "yes"; then

  AC_ARG_VAR([VALGRIND],[the valgrind program])
  AC_PATH_PROG([VALGRIND],[valgrind],[false])
  if test "x$VALGRIND" = "xfalse"; then
    AC_MSG_ERROR([valgrind not found])
  fi
  enable_test=yes
fi


if test "$enable_silent_rules" = "yes"; then
  AM_SILENT_RULES([yes])
  AC_DEFINE([SUPPORT_SILENT_RULES], [], [
    Define to any value to enable prettified make output])
fi

if test "$enable_test" = "yes"; then
  AC_DEFINE([SUPPORT_TEST], [], [
    Define to any value to enable the test suit])
  AC_CHECK_LIB([pcre2-8], [pcre2_code_free_8], [], [AC_MSG_ERROR([unable to find pcre2-8 library])])
  AC_CHECK_LIB([pcre2-16], [pcre2_code_free_16], [], [AC_MSG_ERROR([unable to find pcre2-16 library])])
  AC_CHECK_LIB([pcre2-32], [pcre2_code_free_32], [], [AC_MSG_ERROR([unable to find pcre2-32 library])])
fi



if test "$enable_cpp11" = "yes"; then
  AC_DEFINE([SUPPORT_CPP11], [], [
    Define to any value to enable C++ 11])
  #check for c++11 support and add flags
  AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
  AC_CHECK_HEADERS([codecvt],[AC_MSG_ERROR([Unable to find codecvt header])])
fi

# Conditional compilation
AM_CONDITIONAL(WITH_TEST_SUIT, test "x$enable_valgrind" = "xyes" || test "x$enable_coverage" = "xyes" || test "x$enable_test" = "xyes")
AM_CONDITIONAL(WITH_COVERAGE, test "x$enable_coverage" = "xyes")
AM_CONDITIONAL(WITH_THREAD_TEST, test "x$enable_thread_check" = "xyes")
AM_CONDITIONAL(WITH_VALGRIND, test "x$enable_valgrind" = "xyes")
AM_CONDITIONAL(WITH_CPP11, test "x$enable_cpp11" = "xyes")


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
    Build tests ....................... : ${enable_test}
    Build with C++ 11 ................. : ${enable_cpp11}
    Valgrind memory leak test ......... : ${enable_valgrind}
    Multi-thread test ................. : ${enable_thread_check}
    Coverage report ................... : ${enable_coverage}
    Enable Silent Rules ............... : ${enable_silent_rules}

EOF
