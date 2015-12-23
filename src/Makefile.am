EXTRA_DIST =



EXTRA_DIST += \
  jpcre2.cpp \
  jpcre2.h \
  jpcre2match.cpp \
  jpcre2replace.cpp \
  test_match.cpp \
  test_replace.cpp \
  test.cpp
  
include_HEADERS = \
  jpcre2.h
  
bin_PROGRAMS = jpcre2match jpcre2replace

JPCRE2_SOURCES = \
  jpcre2.cpp \
  jpcre2.h

AM_LDFLAGS = -lpcre2-8

#Building jpcre2match
jpcre2match_SOURCES = \
  jpcre2match.cpp \
  $(JPCRE2_SOURCES)
jpcre2match_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building jpcre2replace
jpcre2replace_SOURCES = \
  jpcre2replace.cpp \
  $(JPCRE2_SOURCES)
jpcre2replace_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)
  
  
#Building a library
lib_LTLIBRARIES = libjpcre2.la
libjpcre2_la_SOURCES = \
  $(JPCRE2_SOURCES)
libjpcre2_la_CXXFLAGS = \
  $(AM_LIB_PATH) \
  $(AM_LDFLAGS) \
  $(AM_CXXFLAGS) \
  -fPIC
libjpcre2_la_LIBADD =

