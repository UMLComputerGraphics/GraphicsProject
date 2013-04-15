SHELL = /usr/bin/env bash

#BUILD EVERYTHING FROM A FOLDER OTHER THAN COMMON OR CLASSES AS AN EXECUTABLE!
SOURCESRAW=$(filter-out Incomplete/%.cpp Common/%.cpp Classes/%.cpp,$(wildcard */*.cpp))
COMMONRAW=$(wildcard Common/*.cpp)
CLASSESRAW=$(wildcard Classes/*.cpp)

#things not needed or that break build

DIAF=Classes/KinectInator.cpp Binaries/morphkeysound.cpp fMod/3d_example.cpp fMod/playsound_example.cpp Binaries/qtFinalProject.cpp Binaries/qtPartTest.cpp Qt/mainwindow.cpp Qt/main.cpp

ifndef WII
DIAF += \
	Common/WiiUtil.cpp
endif

SOURCES=$(filter-out $(DIAF),$(SOURCESRAW))
COMMON=$(filter-out $(DIAF),$(COMMONRAW))
CLASSES=$(filter-out $(DIAF),$(CLASSESRAW))

TARGETS = $(basename $(strip $(SOURCES)))

OBJS = $(subst .cpp,.o,$(COMMON)) \
       $(subst .cpp,.o,$(CLASSES)) \

CXXOPTS = -fmessage-length=0 -ansi -Wall -pedantic -DWITHOUT_QT
CXXINCS = -I$(CURDIR)/include -I$(CURDIR)/expensiveglut/include  -I$(CURDIR)/expensiveglut/GL

DIRT = $(wildcard */*.o */*.so */*.d *.i *~ */*~ *.log)

# Configuration Options
ifdef POSTMULT
	CXXDEFS += -DPOSTMULT
endif

ifdef WII
    CXXDEFS += -DWII -I/usr/local/include/wiic
    LDLIBS += -lwiic -lwiicpp
endif

#FMOD stuff:
#FMODLIBDIR = fMod/lib

# Platform-dependent configuration.
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    CXXOPTS += -std=c++0x
    CXXINCS += -I$(CURDIR)/ImageVoodoo/include  -I$(CURDIR)/ImageVoodoo/include/ImageMagick-6
    CXXDEFS += -DFREEGLUT_STATIC -DGLEW_STATIC
    LDLIBS += -lGL -lGLU -lGLEW -L$(CURDIR)/expensiveglut/lib -lexpensiveglut -lX11 -lm -lboost_thread-mt -L$(CURDIR)/ImageVoodoo/lib -lMagick++-6.Q16
    EXPENSIVEGLUT = $(CURDIR)/expensiveglut/lib/libexpensiveglut.so $(CURDIR)/ImageVoodoo/lib/libMagic++-6.Q16.so
    #FMODNAME = libfmodex.so
    #FMODLIB = $(FMODLIBDIR)/$(FMODLIBNAME)
	#If linux, we don't need any extra build steps
    #FMODLINK = true
endif

ifeq ($(UNAME), Darwin)
#    CXXOPTS -= -mmacosx-version-min=10.6
#    CXXOPTS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++ -m64
    CXXINCS += -I/opt/local/include -I$(CURDIR)/ImageVoodoo/include -I$(CURDIR)/ImageVoodoo/include/ImageMagick-6 -I/usr/X11R6/include 
   # LDLIBS += -framework Carbon -framework OpenGL -framework GLUT -L/opt/local/lib
   # if you can't find imgMagick, then try the line above. be sure to comment the line below
#    LDOPTS -= -mmacosx-version-min=10.6
#    LDOPTS += -stdlib=libc++ -mmacosx-version-min=10.7
    LDLIBS += -L/opt/local/lib -L/usr/X11R6/lib -lGL -lGLU -lGLEW -L$(CURDIR)/expensiveglut/lib -lexpensiveglut -lX11 -lm -L$(CURDIR)/ImageVoodoo/lib -lMagick++-6.Q16 -lboost_thread-mt 
    EXPENSIVEGLUT = $(CURDIR)/expensiveglut/lib/libexpensiveglut.dylib $(CURDIR)/ImageVoodoo/lib/libMagick++-6.Q16.dylib
    FMODLIBNAME = libfmodex.dylib
   # If apple, we do need a funky post-compile build step
    FMODLIB = $(FMODLIBDIR)/$(FMODLIBNAME)
    FMODLINK = install_name_tool -change ./$(FMODLIBNAME) ../$(FMODLIB)
endif

ifdef DEBUG
	CXXOPTS += -DDEBUG -O0 -ggdb
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

default all: ImageVoodoo expensiveglut $(TARGETS)
	@echo ""
	@echo "******************************************************"
	@echo "For ld to find the customized freeglut library, run: "
	@echo '     "export LD_LIBRARY_PATH=`pwd`/expensiveglut/lib:`pwd`/ImageVoodoo/lib:$$LD_LIBRARY_PATH"'
	@echo "******************************************************"

$(TARGETS): $(OBJS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ $(EXPENSIVEGLUT) $(LDFLAGS) -o $@
#PKG_CONFIG_PATH=$(CURDIR)/ImageVoodoo/lib/pkgconfig:$(PKG_CONFIG_PATH) 
#	$(FMODLINK) $@

#-----------------------------------------------------------------------------

%.i: %.cpp
	$(CXX) -E $(CXXFLAGS)  $(EXPENSIVEGLUT) $< | uniq > $@
#PKG_CONFIG_PATH=$(CURDIR)/ImageVoodoo/lib/pkgconfig:$(PKG_CONFIG_PATH) LD_LIBRARY_PATH=$(CURDIR)/ExpensiveGlut/lib 

#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
.PHONY: docs
docs:
	pushd ../Doxygen && doxygen 2>warnings.log && popd;

_clean:	
	@$(RM) $(DIRT)	
	@$(RM) -R debug
	@$(RM) qMakefile

_rmtargets:
	@$(RM) $(TARGETS)
	cd freeglut; make clean
	cd ImageMagick; make clean
	rm -Rf expensiveglut
	rm -Rf ImageVoodoo

clean: _clean
	@echo "Removed everything except compiled executables."

rmtargets: _rmtargets
	@echo "Removed executables."

clobber: _clean _rmtargets
	@echo "Removed objects and executables."


.PHONY: qt
qt: ImageVoodoo expensiveglut
	qmake -o qMakefile
	make -f qMakefile
	@$(RM) Binaries/MONOLITH
	ln debug/GraphicsProject Binaries/MONOLITH
	@echo ""
	@echo "******************************************************"
	@echo "For ld to find the customized freeglut library, run: "
	@echo '     "export LD_LIBRARY_PATH=`pwd`/expensiveglut/lib:`pwd`/ImageVoodoo/lib:$$LD_LIBRARY_PATH"'
	@echo "******************************************************"

.PHONY: configfreeglut
configfreeglut:
ifeq ($(UNAME), Darwin)
	cd freeglut; CPPFLAGS="-I/usr/X11R6/include -L/usr/X11R6/lib" LDFLAGS="-L/usr/X11R6/lib" ./configure --prefix=$(CURDIR)/expensiveglut
else
	cd freeglut; ./configure --prefix=$(CURDIR)/expensiveglut
endif

.PHONY: compilefreeglut
compilefreeglut:
	cd freeglut; make; make install

expensiveglut: configfreeglut compilefreeglut

.PHONY: configmagick
configmagick:
ifeq ($(UNAME), Darwin)
	cd ImageMagick; ./configure --prefix=$(CURDIR)/ImageVoodoo --with-quantum-depth=16 \
      --disable-dependency-tracking --with-x=yes \
      --x-includes=/usr/X11R6/include --x-libraries=/usr/X11R6/lib/ \
      --without-perl
else
	cd ImageMagick; ./configure --prefix=$(CURDIR)/ImageVoodoo --disable-dependency-tracking --without-perl
endif

.PHONY: compilemagick
compilemagick:
	cd ImageMagick; make; make install

ImageVoodoo: configmagick compilemagick
