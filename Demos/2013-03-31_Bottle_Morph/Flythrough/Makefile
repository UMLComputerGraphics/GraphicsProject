#! /usr/bin/make -f

# John Huston
# Adapted from My Generic Makefile,
# Written September 20, 2011
# Modified December 6, 2012
#
# Usages:
# make [all]: Build all code with optimization and no debugging symbols.
# make DEBUG=true: Build all code with optimization disabled, gdb debugging symbols, and DEBUG flag enabled.
# make docs: Build doxygen documentation.
#
# Flags:
# WII=TRUE -- Enable Wii Controls.
# POSTMULT -- Enable Post multiplication code.

# We can define the relative dir. By default, assume the root is up one.
ifndef RELDIR
	RELDIR=..
endif

# Define our directories where the goodies live.
COMMON_DIR = $(RELDIR)/Common
CLASSES_DIR = $(RELDIR)/Classes

# Some basic, common settings.
EXT=cpp
CC=g++
CFLAGS=-Wall -pedantic -I $(RELDIR)/include
LDLIBS=-lSOIL

EXECS=fly terrain morph


#EXEC sources are implied to be $EXEC.cpp !!

COMMON_SOURCES=model.cpp InitShader.cpp

CLASS_SOURCES=Camera.cpp vec.cpp mat.cpp Cameras.cpp Screen.cpp Timer.cpp Object.cpp Scene.cpp Transformation.cpp LightSource.cpp Lights.cpp TransCache.cpp

SOURCES=$(addprefix $(COMMON_DIR)/, $(COMMON_SOURCES)) \
	$(addprefix $(CLASSES_DIR)/, $(CLASS_SOURCES))

# Platform-dependent configuration.
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CFLAGS += -I /usr/include/SOIL
LDLIBS += -lGL -lGLU -lGLEW -lglut -lXmu -lX11 -lm
endif

ifeq ($(UNAME), Darwin)
LDLIBS += -framework Carbon -framework OpenGL -framework GLUT
endif

# Configuration Options
ifdef POSTMULT
	CFLAGS += -DPOSTMULT
endif

ifdef WII
	CFLAGS += -DWII -I/usr/local/include/wiic
	LDLIBS += -lwiic -lwiicpp
	SOURCES += $(COMMON_DIR)/WiiUtil.cpp
endif

#DEBUG=1

ifdef DEBUG
	CFLAGS += -DDEBUG -O0 -ggdb
else
	CFLAGS += -O3
endif

OBJS=$(subst .$(EXT),.o,$(SOURCES))
DEPS=$(subst .$(EXT),.d,$(SOURCES)) $(addsuffix .d,$(EXECS))
BIN_OBJS=$(addsuffix .o,$(EXECS))

# This .PHONY is required to prevent Makefile's built-in rules from processing it.
.PHONY: all
all: $(EXECS)

# Include all of the dependencies.
-include $(DEPS)

# Are the dependency files up to date? Are they missing? (re)Generate them.
%.d: %.cpp
	@echo "Building Dependency for $<"
	@$(CC) -M -MQ "$*.o" $(CFLAGS) $< > $@.$$$$;			\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;	\
	rm -f $@.$$$$

# Any *.o can be built from its matching sourcefile.
%.o:
	$(CC) $(CFLAGS) -c -o $@ $*.$(EXT)

$(EXECS): %: %.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *~ $(OBJS) $(BIN_OBJS) $(EXECS)


.PHONY: scrub
scrub: realclean

.PHONY: wipe
wipe: realclean

# Cleans emacs ~ files and deletes .d dependency files
realclean: clean
	rm -f *~
	find $(RELDIR) -name '*.d' -exec rm '{}' \;

# Make documentation ...
.PHONY: docs
docs:
	cd $(RELDIR)/../gh-pages/Doxygen; doxygen

print-%:
	@echo $* = $($*)
