EXTRA_DIST =

EXTRA_DIST += \
  jpcre2_match.cpp \
  jpcre2_replace.cpp \
  jpcre2.cpp \
  jpcre2.h \
  test_match.cpp \
  test_replace.cpp \
  test_match2.cpp \
  test_replace2.cpp
  
include_HEADERS = \
  jpcre2.h
  
noinst_PROGRAMS = jpcre2match jpcre2replace

JPCRE2_SOURCES = \
  jpcre2_match.cpp \
  jpcre2_replace.cpp \
  jpcre2.cpp \
  jpcre2.h

AM_LDFLAGS = -lpcre2-8

#Building jpcre2match
jpcre2match_SOURCES = \
  test_match2.cpp \
  $(JPCRE2_SOURCES)
jpcre2match_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building jpcre2replace
jpcre2replace_SOURCES = \
  test_replace2.cpp \
  $(JPCRE2_SOURCES)
jpcre2replace_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)
  
  
#Building a library
lib_LTLIBRARIES = libjpcre2-8.la
libjpcre2_8_la_SOURCES = \
  $(JPCRE2_SOURCES)
libjpcre2_8_la_CXXFLAGS = \
  $(AM_LIB_PATH) \
  $(AM_LDFLAGS) \
  $(AM_CXXFLAGS) \
  -fPIC
libjpcre2_8_la_LIBADD =

