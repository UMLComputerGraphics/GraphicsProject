###############################################
#                                             #
#  Makefile for the Graphics II 2013 project  #
#                                             #
###############################################

#colors
CL_RED="\033[31m"
CL_GRN="\033[32m"
CL_YLW="\033[33m"
CL_BLU="\033[34m"
CL_MAG="\033[35m"
CL_CYN="\033[36m"
CL_RST="\033[0m"

SHELL = /usr/bin/env bash
UNAME := $(shell uname)

#BUILD EVERYTHING FROM A FOLDER OTHER THAN COMMON OR CLASSES AS AN EXECUTABLE!
SOURCESRAW=$(filter-out Incomplete/%.cpp Common/%.cpp Classes/%.cpp,$(wildcard */*.cpp))
COMMONRAW=$(wildcard Common/*.cpp)
CLASSESRAW=$(wildcard Classes/*.cpp)

#things not needed or that break build
DIAF=Classes/KinectInator.cpp \
     Qt/main.cpp Qt/mainwindow.cpp Qt/qtLite.cpp Qt/dialog.cpp \
     fMod/playsound_example.cpp fMod/3d_example.cpp

ifndef WII
    DIAF += Common/WiiUtil.cpp
endif

#todo Remove this when some hero fixes the GL_TEXTURE_BUFFER undefined error for Apple users
ifeq ($(UNAME), Darwin)
    DIAF += Binaries/raytrace1.cpp Binaries/raytraceIntegrated.cpp Binaries/rti.cpp
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
CXXDEFS = -DWITHOUT_QT -Wno-long-long
LDLIBS = -lMagick++ -lboost_thread-mt

# Configuration Options
ifdef POSTMULT
	CXXDEFS += -DPOSTMULT
endif
ifdef WII
    CXXOPTS = -DWITHOUT_QT
    CXXDEFS += -DWII -I/usr/local/include/wiic
    LDLIBS += -lwiic -lwiicpp
endif
ifdef EXPRTK
    CXXDEFS += -DEXPRTK
endif

# Platform-dependent configuration.

# LINUX
ifeq ($(UNAME), Linux)
    prepare = preparelinux
    CXXINCS += -I/usr/include/ImageMagick/
    CXXDEFS += -DFREEGLUT_STATIC -DGLEW_STATIC
    LDLIBS += -lGL -lGLU -lGLEW -lglut -lX11 -lm -lfmodex
    FMODLINK = @true
    QT_FINALIZE = @true
endif

# APPLE OSX
ifeq ($(UNAME), Darwin)
    prepare = preparemac
    CXXINCS += -I/opt/local/include -I/opt/local/include/ImageMagick 
    CXXDEFS += -Wno-long-long
   # If apple, we need a funky post-compile build step
    LDLIBS += -framework Carbon -framework OpenGL -framework GLUT -L/opt/local/lib fMod/lib/libfmodex.dylib
   #FMODLINK = install_name_tool -change ./libfmodex.dylib ../fMod/lib/libfmodex.dylib
   FMODLINK = install_name_tool -change ./libfmodex.dylib `pwd`/fMod/lib/libfmodex.dylib
   LDOPTS = -headerpad_max_install_names
   QT_FINALIZE = @cp -f Binaries/FinalProject.app/Contents/MacOS/FinalProject Binaries/FinalProject
endif

ifdef PRINTLEVEL
	CXXOPTS += -DPRINTLEVEL=$(PRINTLEVEL)
endif

ifdef DEBUG
	CXXOPTS += -DDEBUG -O0 -g
else
	CXXOPTS += -O3
endif

ifdef DEBUG_MOTION
	CXXOPTS += -DDEBUG_MOTION
endif

CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)
LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

#-----------------------------------------------------------------------------

.PHONY: Makefile

#explicitely force ordering of preliminary steps, rather than letting make try to build targets and prepare stuff simultaneously
default all:
	$(MAKE) $(prepare)
	$(MAKE) clean-qt
	$(MAKE) $(TARGETS)

.PHONY: preparelinux
preparelinux:
	Util/fMod/install.sh

.PHONY: preparemac
preparemac:
	Util/fixboost.sh
	Util/fMod/macfix.sh

$(TARGETS): $(OBJS)

#.o.cpp:
#	@echo -e $(CL_GRN) OBJ: $@$(CL_RST)
#	@$(CC) $(CFLAGS) -c $<

%: %.cpp
	@echo -e $(CL_GRN) BIN: $@$(CL_RST)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	$(FMODLINK) $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	@echo $@
	$(CXX) -E $(CXXFLAGS) $< | uniq | grep -v $@ > $@

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

.PHONY: fresh
fresh:
	$(MAKE) clobber
	$(MAKE) all

.PHONY: qt
qt: qMakefile
	$(MAKE) -f qMakefile
	$(QT_FINALIZE)

.PHONY: clean-qt
clean-qt: qMakefile
	$(MAKE) -f qMakefile distclean
	rm -rf debug/ release/
	rm -f qMakefile

qMakefile:
	qmake -o qMakefile

