# -*- makefile -*-

# This tells the Makefile where to find the source files
export BINDIRS=src/main src/test/suite src/test/tween
export SRCDIRS=src/core
export CHECKDIRS=src/core $(BINDIRS)

# Test bin is run out of the dist directory
TEST_BIN+=./suite

# VERSION will be set as a preprocessor define
export VC_VERSION=$(shell git rev-parse HEAD)

export LIBS=-lm `pkg-config --libs allegro_font-5 allegro_primitives-5 allegro-5`

# Output directory; CURDIR is a make constant
export BUILDDIR=$(CURDIR)/build
export DATADIR=$(CURDIR)/data
export OUTDIR=$(CURDIR)/dist
export BUILDROOT=$(CURDIR)

# Set the VERSION preprocessor definition and add all SRCDIRS and their subdirs to the include search path
INCLUDE_PATHS=$(foreach d, $(shell $(FIND) $(SRCDIRS) -type d), -I$(CURDIR)/$(d))
EXTRA_COMMON=-DVERSION=\"$(VC_VERSION)\" $(INCLUDE_PATHS) -fno-strict-aliasing
EXTRA_CC+=$(EXTRA_COMMON)
EXTRA_CPP+=$(EXTRA_COMMON)

export BUILD_CPP=$(CXX) $(EXTRA_CPP) -c
export BUILD_C=$(CC) $(EXTRA_CC) -c
export LINK_BIN=$(CC)

export RELEASE_CPP=$(BUILD_CPP) -O3 -DNO_SAFETY_CHECKS=1
export RELEASE_C=$(BUILD_C) -O3 -DNO_SAFETY_CHECKS=1
export RELEASE_BIN=$(LINK_BIN) -O3 -DNO_SAFETY_CHECKS=1

export DEBUG_CPP=$(BUILD_CPP) -DDEBUG=1 -g
export DEBUG_C=$(BUILD_C) -DDEBUG=1 -g
export DEBUG_BIN=$(LINK_BIN) -DDEBUG=1 -g

export COPY=cp -u
export REMOVE=rm -f
export MAKE=make --no-print-directory
export ECHO=echo
export MKDIR=mkdir -p
export FIND=find
export CD=cd
export WC=wc -l -L -c

export CPPCHECK=cppcheck --quiet --enable=warning --enable=style --enable=performance --enable=portability --enable=unusedFunction $(INCLUDE_PATHS)

all:
	@$(ECHO) "You probably want one of debug, release, clean, clean_dist, deploy, test or linestats"

release: clean check
	@$(ECHO) ----------------------------------------
	@$(ECHO) Building Release
	@$(ECHO) ----------------------------------------
	@$(ECHO)
	@$(MAKE) -f $(BUILDROOT)/Makefile release_build;

release_build: release_config clean compile link_bin deploy

debug: debug_config compile link_bin deploy

debug_config: build_information
	@$(eval export BUILD_CPP=$(DEBUG_CPP))
	@$(eval export BUILD_C=$(DEBUG_C))
	@$(eval export LINK_BIN=$(DEBUG_BIN))

compile:
	@$(MKDIR) "$(OUTDIR)" "$(BUILDDIR)"
	@$(MAKE) -f $(BUILDROOT)/Makefile.Build.mk compile;
	@$(ECHO)

link_bin:
	@$(MAKE) -f $(BUILDROOT)/Makefile.Build.mk link_bin;
	@$(ECHO)

release_config: build_information
	@$(eval export LINK_BIN=$(RELEASE_BIN))
	@$(eval export BUILD_CPP=$(RELEASE_CPP))
	@$(eval export BUILD_C=$(RELEASE_C))

build_information:
	@$(ECHO) Using $(SHELL) as shell
	@$(ECHO)

check:
	@$(MAKE) -f $(BUILDROOT)/Makefile.Build.mk check;

clean: clean_dist
	@$(MAKE) -f $(BUILDROOT)/Makefile.Build.mk clean;
	@$(FIND) ./ -iname .cache -exec $(RM) -rf {} \;

deploy:
	@$(MAKE) -f $(BUILDROOT)/Makefile.Build.mk deploy;
	@$(ECHO)

test: debug
	@$(CD) $(OUTDIR);$(TEST_BIN);$(CD) -

linestats:
	@$(MAKE) -f $(BUILDROOT)/Makefile.Build.mk linestats;

clean_dist:
	@$(ECHO) "[RM]  $(OUTDIR)";
	@$(REMOVE) -rf $(OUTDIR);
