include([ax_cxx_compile_stdcxx.m4])
AC_PREREQ([2.69])
AC_INIT([jpcre2], [10.21.01], [https://github.com/jpcre2/jpcre2/issues])
AC_CONFIG_SRCDIR([src/jpcre2.h])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

#check for c++11 support and add flags
AX_CXX_COMPILE_STDCXX([11], [noext], [optional])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([setlocale])

AC_OUTPUT(Makefile)
