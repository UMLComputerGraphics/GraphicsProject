# Nicholas St.Pierre
# Makefile for FMod Examples
# Adapted from the makefile provided by the fmod team
# 3/10/13

# This is currently for Macs only, but linux support is on the horizon

LIB_PATH                = lib
LIB_NAME_RELEASE        = libfmodex.dylib
LIB_NAME_LOGGING        = libfmodexL.dylib

# IMPORTANT: pick your architecture

#ARCH                    = -arch i386
ARCH                    = -arch x86_64
#ARCH                    = -arch ppc


all: playsound_release 3d_release
	

all_debug: playsound_debug 3d_debug
	

playsound_release: playsound_example.cpp
	g++ -O3 ${ARCH} -o playsound_example playsound_example.cpp ${LIB_PATH}/${LIB_NAME_RELEASE}
	install_name_tool -change ./${LIB_NAME_RELEASE} ${LIB_PATH}/${LIB_NAME_RELEASE} playsound_example

playsound_debug: playsound_example.cpp
	g++ -O0 ${ARCH} -g -o playsound_example playsound_example.cpp ${LIB_PATH}/${LIB_NAME_LOGGING}
	install_name_tool -change ./${LIB_NAME_LOGGING} ${LIB_PATH}/${LIB_NAME_LOGGING} playsound_example

3d_release: 3d_example.cpp
	g++ -O3 ${ARCH} -o 3d_example 3d_example.cpp ${LIB_PATH}/${LIB_NAME_RELEASE}
	install_name_tool -change ./${LIB_NAME_RELEASE} ${LIB_PATH}/${LIB_NAME_RELEASE} 3d_example

3d_debug: 3d_example.cpp
	g++ -O0 ${ARCH} -g -o 3d_example 3d_example.cpp ${LIB_PATH}/${LIB_NAME_LOGGING}
	install_name_tool -change ./${LIB_NAME_LOGGING} ${LIB_PATH}/${LIB_NAME_LOGGING} 3d_example

clean:
	rm -f playsound_example 3d_example *~