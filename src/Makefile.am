EXTRA_DIST =

EXTRA_DIST += \
  jpcre2.hpp \
  test_match.cpp \
  test_replace.cpp \
  test_match2.cpp \
  test_replace2.cpp \
  test_shorts.cpp \
  test16.cpp \
  test32.cpp \
  test0.cpp

include_HEADERS = \
  jpcre2.hpp
  
noinst_PROGRAMS = 



JPCRE2_SOURCES = 

if WITH_TEST_8

noinst_PROGRAMS += test_match2 test_replace2 test_match test_replace test_shorts

#Building test_match2
test_match2_SOURCES = \
  test_match2.cpp \
  $(JPCRE2_SOURCES)
test_match2_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_match
test_match_SOURCES = \
  test_match.cpp \
  $(JPCRE2_SOURCES)
test_match_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_replace
test_replace2_SOURCES = \
  test_replace2.cpp \
  $(JPCRE2_SOURCES)
test_replace2_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_replace2
test_replace_SOURCES = \
  test_replace.cpp \
  $(JPCRE2_SOURCES)
test_replace_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_shorts
test_shorts_SOURCES = \
  test_shorts.cpp \
  $(JPCRE2_SOURCES)
test_shorts_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)
endif

if WITH_TEST_16

noinst_PROGRAMS += test16

#Building test16
test16_SOURCES = \
  test16.cpp \
  $(JPCRE2_SOURCES)
test16_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

endif

if WITH_TEST_32

noinst_PROGRAMS += test32

#Building test32
test32_SOURCES = \
  test32.cpp \
  $(JPCRE2_SOURCES)
test32_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

endif


if WITH_TEST_MULTI
noinst_PROGRAMS += test0

#Building test0
test0_SOURCES = \
  test0.cpp \
  $(JPCRE2_SOURCES)
test0_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

endif

