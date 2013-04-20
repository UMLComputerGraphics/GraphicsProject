SHELL = /usr/bin/env bash

UNAME := $(shell uname)

#BUILD EVERYTHING FROM A FOLDER OTHER THAN COMMON OR CLASSES AS AN EXECUTABLE!
SOURCESRAW=$(filter-out Incomplete/%.cpp Common/%.cpp Classes/%.cpp,$(wildcard */*.cpp))
COMMONRAW=$(wildcard Common/*.cpp)
CLASSESRAW=$(wildcard Classes/*.cpp)

#Binaries/particleBottle.cpp
#things not needed or that break build
DIAF=Classes/KinectInator.cpp \
     fMod/playsound_example.cpp fMod/3d_example.cpp Binaries/morphkeysound.cpp \
     Qt/main.cpp Qt/mainwindow.cpp Qt/qtLite.cpp

ifndef WII
DIAF += Common/WiiUtil.cpp
endif

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

ifeq ($(UNAME), Linux)
    CXXINCS += -I/usr/include/ImageMagick/
    CXXDEFS += -DFREEGLUT_STATIC -DGLEW_STATIC
    LDLIBS += -lGL -lGLU -lGLEW -lglut -lX11 -lm
    #FMODNAME = libfmodex.so
    #FMODLIB = $(FMODLIBDIR)/$(FMODLIBNAME)
    #If linux, we don't need any extra build steps
    #FMODLINK = true
endif

ifeq ($(UNAME), Darwin)
    CXXINCS += -I/opt/local/include -I/opt/local/include/ImageMagick
   # LDLIBS += -framework Carbon -framework OpenGL -framework GLUT -L/opt/local/lib
   # if you can't find imgMagick, then try the line above. be sure to comment the line below
    LDLIBS += -framework Carbon -framework OpenGL -framework GLUT -L/opt/local/lib

    FMODLIBNAME = libfmodex.dylib
   # If apple, we do need a funky post-compile build step
    FMODLIB = $(FMODLIBDIR)/$(FMODLIBNAME)
    FMODLINK = install_name_tool -change ./$(FMODLIBNAME) ../$(FMODLIB)
endif

ifdef DEBUG
	CXXOPTS += -DDEBUG -O0 -g
else
	CXXOPTS += -O3
endif

#USE_FMOD = 1
#ifdef USE_FMOD
#    LDLIBS += $(FMODLIB)
#else
#in this context, true means "do nothing successfully"
#    FMODLINK = true
#endif

CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)
LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

#-----------------------------------------------------------------------------

.PHONY: Makefile

default all:  $(TARGETS)

$(TARGETS): $(OBJS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
#	$(FMODLINK) $@

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


