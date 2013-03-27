SHELL = /usr/bin/env bash

#BUILD EVERYTHING FROM A FOLDER OTHER THAN COMMON OR CLASSES AS AN EXECUTABLE!
SOURCESRAW=$(filter-out Incomplete/%.cpp Common/%.cpp Classes/%.cpp,$(wildcard */*.cpp))
SPELLCHKCOMMONRAW=$(wildcard SpelchkCommon/*.cpp)
COMMONRAW=$(wildcard Common/*.cpp)
CLASSESRAW=$(wildcard Classes/*.cpp)


#things not needed or that break build
DIAF=Common/LoadTexture.cpp \
     Binaries/raytracer1.cpp \
     Classes/KinectInator.cpp


ifndef WII
DIAF += \
	Common/WiiUtil.cpp
endif
SOURCES=$(filter-out $(DIAF),$(SOURCESRAW))
COMMON=$(filter-out $(DIAF),$(COMMONRAW))
CLASSES=$(filter-out $(DIAF),$(CLASSESRAW))
SPELLCHKCOMMON=$(filter-out $(DIAF),$(SPELLCHKCOMMONRAW))

TARGETS = $(basename $(strip $(SOURCES)))

OBJS = $(subst .cpp,.o,$(COMMON)) \
       $(subst .cpp,.o,$(CLASSES)) \
       $(subst .cpp,.o,$(SPELLCHKCOMMON))

CXXOPTS = -fmessage-length=0 -pedantic 
CXXINCS = "-I$(CURDIR)/include"

DIRT = $(wildcard */*.o */*.so */*.d *.i *~ */*~ *.log)

LDLIBS = -lSOIL

# Configuration Options
ifdef POSTMULT
	CXXDEFS += -DPOSTMULT
endif

ifdef WII
    CXXDEFS += -DWII -I/usr/local/include/wiic
    LDLIBS += -lwiic -lwiicpp
endif

# Platform-dependent configuration.
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    CXXINCS += -I/usr/include/SOIL
    CXXDEFS += -DFREEGLUT_STATIC -DGLEW_STATIC
    LDLIBS += -lGL -lGLU -lGLEW -lglut -lX11 -lm
endif

ifeq ($(UNAME), Darwin)
    LDLIBS += -framework Carbon -framework OpenGL -framework GLUT
endif

ifdef DEBUG
	CXXOPTS += -DDEBUG -O0 -g
else
	CXXOPTS += -O3
endif

CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)
LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

#-----------------------------------------------------------------------------

.PHONY: Makefile

default all: $(TARGETS)

$(TARGETS): $(OBJS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	$(CXX) -E $(CXXFLAGS) $< | uniq > $@

#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
.PHONY: docs
docs:
	pushd ../Doxygen && doxygen 2>warnings.log && popd;

_clean:	
	@$(RM) $(DIRT)

_rmtargets:
	@$(RM) $(TARGETS)

clean: _clean
	@echo "Removed everything BUT compiled executables"

rmtargets: _rmtargets
	@echo "Removed executables"

clobber: _clean _rmtargets
	@echo "Cleaned EVERYTHING MUAHAHAHA"
