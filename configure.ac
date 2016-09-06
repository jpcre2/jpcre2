AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
include([ax_cxx_compile_stdcxx.m4])

AC_INIT([jpcre2], [10.25.02], [https://github.com/jpcre2/jpcre2/issues])
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

#check for c++11 support and add flags
#AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
## C++11 is no longer a requirement


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

AC_CHECK_HEADERS([pcre2.h],
	[AC_MSG_ERROR([Unable to find pcre2.h header])])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([setlocale])


AC_CHECK_LIB([pcre2-8], [pcre2_code_free_8], [], [
  AC_MSG_ERROR([unable to find pcre2-8 library])
])

AC_OUTPUT(Makefile src/Makefile doc/Makefile)

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
    Build shared library .............. : ${enable_shared}
    Build static library .............. : ${enable_static}

EOF
