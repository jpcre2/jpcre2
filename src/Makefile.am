EXTRA_DIST =

EXTRA_DIST += \
  jpcre2.cpp \
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



JPCRE2_SOURCES = \
  jpcre2.cpp \
  jpcre2.hpp

if WITH_JPCRE2_TEST_COMPILE

noinst_PROGRAMS += test_match2 test_replace2 test_match test_replace test_shorts

#Building test_match2
test_match2_SOURCES = \
  test_match2.cpp \
  $(JPCRE2_SOURCES)
test_match2_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=8 \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_match
test_match_SOURCES = \
  test_match.cpp \
  $(JPCRE2_SOURCES)
test_match_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=8 \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_replace
test_replace2_SOURCES = \
  test_replace2.cpp \
  $(JPCRE2_SOURCES)
test_replace2_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=8 \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_replace2
test_replace_SOURCES = \
  test_replace.cpp \
  $(JPCRE2_SOURCES)
test_replace_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=8 \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

#Building test_shorts
test_shorts_SOURCES = \
  test_shorts.cpp \
  $(JPCRE2_SOURCES)
test_shorts_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=8 \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)
endif

if WITH_JPCRE2_TEST_COMPILE_CPP11

noinst_PROGRAMS += test16 test32

#Building test16
test16_SOURCES = \
  test16.cpp \
  $(JPCRE2_SOURCES)
test16_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=16 \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)
  

#Building test32
test32_SOURCES = \
  test32.cpp \
  $(JPCRE2_SOURCES)
test32_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=32 \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

endif



#Building libraries
lib_LTLIBRARIES =

if WITH_JPCRE2_8
lib_LTLIBRARIES += libjpcre2-8.la
libjpcre2_8_la_SOURCES = \
  $(JPCRE2_SOURCES)
libjpcre2_8_la_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=8 \
  $(AM_LIB_PATH) \
  $(AM_LDFLAGS) \
  $(AM_CXXFLAGS)
libjpcre2_8_la_LIBADD =
endif


if WITH_JPCRE2_16
lib_LTLIBRARIES += libjpcre2-16.la
libjpcre2_16_la_SOURCES = \
  $(JPCRE2_SOURCES)
libjpcre2_16_la_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=16 \
  $(AM_LIB_PATH) \
  $(AM_LDFLAGS) \
  $(AM_CXXFLAGS)
libjpcre2_16_la_LIBADD =
endif



if WITH_JPCRE2_32
lib_LTLIBRARIES += libjpcre2-32.la
libjpcre2_32_la_SOURCES = \
  $(JPCRE2_SOURCES)
libjpcre2_32_la_CXXFLAGS = \
  -DPCRE2_CODE_UNIT_WIDTH=32 \
  $(AM_LIB_PATH) \
  $(AM_LDFLAGS) \
  $(AM_CXXFLAGS)
libjpcre2_32_la_LIBADD =
endif
