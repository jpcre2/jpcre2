AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src

EXTRA_DIST = autogen.sh \
	.gitignore \
	.gitattributes \
	.travis.yml
	m4/ax_cxx_compile_stdcxx.m4

dist_doc_DATA = \
  AUTHORS \
  COPYING \
  ChangeLog \
  NEWS \
  README.md
  

.PHONY: coverage
#.PHONY: covclean

if WITH_COVERAGE

coverage: src
	$(MAKE) coverage -C src

covclean: src 
	$(MAKE) covclean -C src

else

coverage:
	@printf "\n--- configure with --enable-coverage to run coverage ---\n\n"

covclean:
	@printf "\n--- configure with --enable-coverage to run coverage ---\n\n"

endif
