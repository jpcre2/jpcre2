AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
include([ax_cxx_compile_stdcxx.m4])

AC_INIT([jpcre2], [10.21.03], [https://github.com/jpcre2/jpcre2/issues])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/jpcre2.h])
#AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

AC_TYPE_INT64_T

# Checks for programs.

AC_PROG_INSTALL

LT_INIT([disable-shared])

AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
AM_DISABLE_SHARED


AC_PROG_CXX
AC_PROG_CC
CXXFLAGS=" "


#check for c++11 support and add flags
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([setlocale])

AC_OUTPUT(Makefile src/Makefile)


cat <<EOF

configuration summary:

    Install prefix .................... : ${prefix}
    C preprocessor .................... : ${CC}
    C++ compiler ...................... : ${CXX}
    C++ preprocessor flags ............ : ${CXXFLAGS}
    C++ compiler flags ................ : ${CXXFLAGS}

EOF
