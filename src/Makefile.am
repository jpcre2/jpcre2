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
  test0.cpp \
  test.cpp

include_HEADERS = \
  jpcre2.hpp
  
noinst_PROGRAMS = 
TESTS =


JPCRE2_SOURCES = 

if WITH_TEST_8

noinst_PROGRAMS += test_match2 test_replace2 test_match test_replace test_shorts
TESTS += test_match test_replace test_shorts

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
TESTS += test16

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
TESTS += test32

#Building test32
test32_SOURCES = \
  test32.cpp \
  $(JPCRE2_SOURCES)
test32_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

endif


if WITH_TEST_ALL

noinst_PROGRAMS += test0
TESTS += test0

#Building test0
test0_SOURCES = \
  test0.cpp \
  $(JPCRE2_SOURCES)
test0_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS)

endif

COVERAGE_FLAGS =
DISTCLEANFILES =


if WITH_COVERAGE

COVERAGE_TEST_NAME = $(PACKAGE)
COVERAGE_NAME = $(PACKAGE)-$(VERSION)
COVERAGE_OUTPUT_FILE = $(COVERAGE_NAME)-coverage.info
COVERAGE_OUTPUT_DIR = $(COVERAGE_NAME)-coverage
#COVERAGE_LCOV_EXTRA_FLAGS =
#COVERAGE_GENHTML_EXTRA_FLAGS =

#coverage_quiet = $(coverage_quiet_$(V))
#coverage_quiet_ = $(coverage_quiet_$(AM_DEFAULT_VERBOSITY))
#coverage_quiet_0 = --quiet


coverage: test jpcre2.hpp test.cpp
	-$(LCOV) $(coverage_quiet) --zerocounters --directory $(top_builddir)
	./test
	$(LCOV) --no-external --test-name "$(COVERAGE_TEST_NAME)" \
		--directory . \
		--directory "$(srcdir)" \
		--capture \
		--output-file "$(COVERAGE_OUTPUT_FILE)"
	mkdir -p "$(COVERAGE_OUTPUT_DIR)"
	$(GENHTML) -o "$(COVERAGE_OUTPUT_DIR)" "$(COVERAGE_OUTPUT_FILE)"

distclean-local:
	rm -f *.info *.gcno *.gcda
	rm -rf "$(COVERAGE_OUTPUT_DIR)"
covclean: distclean-local


# Without coverage support, still arrange for 'make distclean' to get rid of
# any coverage files that may have been left from a different configuration.

endif


if WITH_TEST_SUIT

noinst_PROGRAMS += test
TESTS += test

#Building test
test_SOURCES = \
  test.cpp \
  $(JPCRE2_SOURCES)
test_CXXFLAGS = \
  $(AM_CXXFLAGS) \
  $(AM_LDFLAGS) \
  $(COVERAGE_FLAGS)

endif

if WITH_VALGRIND

check: $(TESTS)
	$(VALGRIND) --show-reachable=no --leak-check=full --track-origins=yes ./test
	@printf "\n\n\t*** Congrats! No memory leak found. ***\n\n\n"
	$(VALGRIND) --tool=helgrind ./test
	@printf "\n\n\t*** Congrats! Helgrind success. ***\n\n\n"

endif


