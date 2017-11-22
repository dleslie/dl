# -*- makefile -*-

# Clear implicit build rules
.SUFFIXES:

####################
# Utility Variables
####################

SRCDIR=$(CURDIR)

BUILD_C+=-I$(SRCDIR)
BUILD_CPP+=-I$(SRCDIR)
LINK_BIN+=-L$(BUILDDIR)

####################
# Object Files
####################

CPPFILES=$(foreach d, $(SRCDIRS), $(shell $(FIND) ./$(d) -wholename \*.cpp))
CPPOFILES=$(CPPFILES:.cpp=.o)

CPPHEADERFILES=$(foreach d, $(SRCDIRS), $(shell $(FIND) ./$(d) -wholename \*.hpp))

BINCPPFILES=$(foreach d, $(BINDIRS), $(shell $(FIND) ./$(d) -wholename \*.cpp))
BINCPPOFILES=$(BINCPPFILES:.cpp=.o)

BINCPPHEADERFILES=$(foreach d, $(BINDIRS), $(shell $(FIND) ./$(d) -wholename \*.hpp))

CFILES=$(foreach d, $(SRCDIRS), $(shell $(FIND) ./$(d) -wholename \*.c))
COFILES=$(CFILES:.c=.o)

CHEADERFILES=$(foreach d, $(SRCDIRS), $(shell $(FIND) ./$(d) -wholename \*.h))

BINCFILES=$(foreach d, $(BINDIRS), $(shell $(FIND) ./$(d) -wholename \*.c))
BINCOFILES=$(BINCFILES:.c=.o)

BINCHEADERFILES=$(foreach d, $(BINDIRS), $(shell $(FIND) ./$(d) -wholename \*.h))

OFILES=$(CPPOFILES) $(COFILES) $(filter ./$(BINDIR)%,$(BINCPPOFILES) $(BINCOFILES))
BUILD_OFILES=$(addprefix $(BUILDDIR)/, $(OFILES))

SRCFILES=$(CPPFILES) $(CFILES) $(BINCPPFILES) $(BINCFILES)
HEADERFILES=$(CPPHEADERFILES) $(CHEADERFILES) $(BINCPPHEADERFILES) $(BINCHEADERFILES)

BINFILES=$(addprefix $(BUILDDIR)/bin/,$(notdir $(BINDIRS)))
DATAFILES=$(shell $(FIND) $(DATADIR) -type f 2> /dev/null)

DEPLOY_BINS=$(addprefix $(OUTDIR)/, $(notdir $(BINDIRS)))
DEPLOY_DATA=$(addprefix $(OUTDIR)/,$(patsubst $(DATADIR)/%,%,$(DATAFILES)))

DEPENDENCIES=$(shell $(FIND) $(BUILDDIR) -type f -wholename \*.d)

CGCOV_FILES=$(COFILES:.o=.c.gcov) $(filter ./$(BINDIR)%,$(BINCOFILES:.o=.c.gcov))
CPPGCOV_FILES=$(CPPOFILES:.o=.cpp.gcov) $(filter ./$(BINDIR)%,$(BINCPPOFILES:.o=.cpp.gcov))
GCOV_FILES=$(CGCOV_FILES) $(CPPGCOV_FILES)

####################
# Rules
####################

all: 
	@$(ECHO) "You want one of compile, link_bin, deploy or check"

compile: $(BUILD_OFILES)

link_bin: $(BINFILES)

-include $(DEPENDENCIES)

check: $(SRCFILES)
	@$(CPPCHECK) $(SRCFILES)

deploy: $(DEPLOY_BINS) $(DEPLOY_DATA)

$(DEPLOY_BINS): $(BINFILES)
	@$(ECHO) "[DEP] $@"
	@$(MKDIR) `dirname $@`
	@$(eval export BUILD_FILE=$(patsubst $(OUTDIR)/%,$(BUILDDIR)/bin/%,$@))
	@$(COPY) -ru $(BUILD_FILE) $@;

$(DEPLOY_DATA): $(DATAFILES)
	@$(ECHO) "[DEP] $@"
	@$(MKDIR) `dirname $@`
	@$(eval export DATA_FILE=$(patsubst $(OUTDIR)/%,$(DATADIR)/%,$@))
	@$(COPY) -ru $(DATA_FILE) $@;

$(BUILDDIR)/%.o: %.cpp 
	@$(ECHO) "[CPP] $<"
	@$(MKDIR) `dirname $@`
	@$(BUILD_CPP) -MD -MF $(patsubst %.o,%.d,$@) -o $@ $(SRCDIR)/$<

$(BUILDDIR)/%.o: %.c
	@$(ECHO) "[CC]  $<";
	@$(MKDIR) `dirname $@`
	@$(BUILD_C) -MD -MF $(patsubst %.o,%.d,$@) -o $@ $(SRCDIR)/$<

$(BINFILES): $(BUILD_OFILES)
	@$(ECHO) "[LNK] $(notdir $@): $(LIBS) $(notdir $(BUILD_OFILES))";
	@$(eval export BINDIR=$(filter %$(notdir $@),$(BINDIRS)))
	@$(eval export THISBINOFILES=$(filter $(BUILDDIR)/./$(BINDIR)%,$(BINOFILES)))
	@$(MKDIR) `dirname $@`
	@$(LINK_BIN) $(BUILD_OFILES) $(LIBS) -o $@;

linestats:
	@$(WC) $(SRCFILES) $(HEADERFILES)
	@$(ECHO) "Listing: Lines, Bytes, Longest Line in File, Filename"

clean:
	@$(ECHO) [RM]  $(BUILD_OFILES) $(BINFILES) $(DATAFILES)
	@$(RM) $(BUILD_OFILES) $(BINFILES) $(DATAFILES)
	@$(ECHO) [RM]  $(GCOV_FILES)
	@$(RM) $(GCOV_FILES)

$(CGCOV_FILES): $(BUILDDIR)/$($@:.c.gcov=.o)

$(CPPGCOV_FILES): $(BUILDDIR)/$($@:.cpp.gcov=.o)
