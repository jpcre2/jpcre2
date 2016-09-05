EXTRA_DIST =

EXTRA_DIST += \
  jpcre2.cpp \
  jpcre2.hpp \
  test_match.cpp \
  test_replace.cpp \
  test_match2.cpp \
  test_replace2.cpp \
  test_shorts.cpp

include_HEADERS = \
  jpcre2.hpp
  
noinst_PROGRAMS = test_match2 test_replace2 test_match test_replace test_shorts

JPCRE2_SOURCES = \
  jpcre2.cpp \
  jpcre2.hpp

#Building match
test_match2_SOURCES = \
  test_match2.cpp \
  $(JPCRE2_SOURCES)
test_match2_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building match0
test_match_SOURCES = \
  test_match.cpp \
  $(JPCRE2_SOURCES)
test_match_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building replace
test_replace2_SOURCES = \
  test_replace2.cpp \
  $(JPCRE2_SOURCES)
test_replace2_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building replace0
test_replace_SOURCES = \
  test_replace.cpp \
  $(JPCRE2_SOURCES)
test_replace_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building shorts
test_shorts_SOURCES = \
  test_shorts.cpp \
  $(JPCRE2_SOURCES)
test_shorts_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)
  
#Building a library
lib_LTLIBRARIES = libjpcre2-8.la
libjpcre2_8_la_SOURCES = \
  $(JPCRE2_SOURCES)
libjpcre2_8_la_CXXFLAGS = \
  $(AM_LIB_PATH) \
  $(AM_LDFLAGS) \
  $(AM_CXXFLAGS)
libjpcre2_8_la_LIBADD =

