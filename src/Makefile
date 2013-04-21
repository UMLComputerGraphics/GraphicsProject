###############################################
#                                             #
#  Makefile for the Graphics II 2013 project  #
#                                             #
###############################################

SHELL = /usr/bin/env bash
UNAME := $(shell uname)

UNAME := $(shell uname)

#BUILD EVERYTHING FROM A FOLDER OTHER THAN COMMON OR CLASSES AS AN EXECUTABLE!
SOURCESRAW=$(filter-out Incomplete/%.cpp Common/%.cpp Classes/%.cpp,$(wildcard */*.cpp))
COMMONRAW=$(wildcard Common/*.cpp)
CLASSESRAW=$(wildcard Classes/*.cpp)

#things not needed or that break build
DIAF=Classes/KinectInator.cpp \
     Qt/main.cpp Qt/mainwindow.cpp Qt/qtLite.cpp \
     fMod/playsound_example.cpp fMod/3d_example.cpp
#     fMod/playsound_example.cpp fMod/3d_example.cpp Binaries/morphkeysound.cpp \
#     Qt/main.cpp Qt/mainwindow.cpp Qt/qtLite.cpp Binaries/FinalProject.cpp

ifndef WII
    DIAF += Common/WiiUtil.cpp
endif

#todo Remove this when some hero fixes the GL_TEXTURE_BUFFER undefined error for Apple users
ifeq ($(UNAME), Darwin)
    DIAF += Binaries/raytrace1.cpp Binaries/raytraceIntegrated.cpp
endif


SOURCES=$(filter-out $(DIAF),$(SOURCESRAW))
COMMON=$(filter-out $(DIAF),$(COMMONRAW))
CLASSES=$(filter-out $(DIAF),$(CLASSESRAW))

TARGETS = $(basename $(strip $(SOURCES)))

OBJS = $(subst .cpp,.o,$(COMMON)) \
       $(subst .cpp,.o,$(CLASSES)) \

DIRT = $(wildcard */*.o */*.so */*.d *.i *~ */*~ *.log)
CXXOPTS = -fmessage-length=0 -ansi -Wall -pedantic -DWITHOUT_QT

CXXINCS = "-I$(CURDIR)/include"
CXXINCS+= -IfMod/include
CXXDEFS = -DWITHOUT_QT
LDLIBS = -lMagick++ -lboost_thread-mt

# Configuration Options
ifdef POSTMULT
	CXXDEFS += -DPOSTMULT
endif

ifdef WII
    CXXDEFS += -DWII -I/usr/local/include/wiic
    LDLIBS += -lwiic -lwiicpp
endif

# Platform-dependent configuration.

# LINUX
ifeq ($(UNAME), Linux)
    prepare = preparelinux
    CXXINCS += -I/usr/include/ImageMagick/
    CXXDEFS += -DFREEGLUT_STATIC -DGLEW_STATIC
    LDLIBS += -lGL -lGLU -lGLEW -lglut -lX11 -lm -lfmodex
    # in this context, true means 'do nothing' (type 'true' in your shell to see what I mean)
    FMODLINK = true
endif

# APPLE OSX
ifeq ($(UNAME), Darwin)
    prepare = preparemac
    CXXINCS += -I/opt/local/include -I/opt/local/include/ImageMagick 
   # If apple, we need a funky post-compile build step
    LDLIBS += -framework Carbon -framework OpenGL -framework GLUT -L/opt/local/lib fMod/lib/libfmodex.dylib
   #FMODLINK = install_name_tool -change ./libfmodex.dylib ../fMod/lib/libfmodex.dylib
   FMODLINK = install_name_tool -change ./libfmodex.dylib `pwd`/fMod/lib/libfmodex.dylib
endif


ifdef DEBUG
	CXXOPTS += -DDEBUG -O0 -g
else
	CXXOPTS += -O3
endif


CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)
LDFLAGS = $(LDOPTS) -headerpad_max_install_names $(LDDIRS) $(LDLIBS)

#-----------------------------------------------------------------------------

.PHONY: Makefile


default all: $(prepare) clean-qt $(TARGETS)


.PHONY: preparelinux
preparelinux:
	Util/fMod/install.sh

.PHONY: preparemac
preparemac:
# todo Move mac setup here?
	true

$(TARGETS): $(OBJS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	$(FMODLINK) $@

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
	@echo "Removed everything except compiled executables."

rmtargets: _rmtargets
	@echo "Removed executables."

clobber: _clean _rmtargets clean-qt
	@echo "Removed objects and executables."

.PHONY: qt
qt: qMakefile
	make -f qMakefile

.PHONY: clean-qt
clean-qt: qMakefile
	make -f qMakefile distclean
	rm -rf debug/ release/
	rm -f qMakefile

qMakefile:
	qmake -o qMakefile

