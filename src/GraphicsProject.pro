#-------------------------------------------------
#
# Project created by QtCreator 2013-03-24T02:34:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = GraphicsProject
TEMPLATE = app

release: DESTDIR = release/
debug:   DESTDIR = debug/

OBJECTS_DIR =  $$DESTDIR/obj/
MOC_DIR =  $$DESTDIR/moc/
RCC_DIR =  $$DESTDIR/rcc/
UI_DIR =  $$DESTDIR/ui/

SOURCES += Qt/main.cpp\
        # Classes
        Classes/Camera.cpp \
        Classes/Cameras.cpp \
        Classes/Engine.cpp \
        # KinectInator.cpp \
        Classes/mat.cpp \
        Classes/meshMapping.cpp \
        Classes/Object.cpp \
        Classes/Particle.cpp \
        Classes/ParticleSystem.cpp \
        Classes/Scene.cpp \
        Classes/Screen.cpp \
        Classes/SpelchkCamera.cpp \
        Classes/Texture.cpp \
        Classes/TextureMan.cpp \
        Classes/Timer.cpp \
        Classes/TransCache.cpp \
        Classes/Transformation.cpp \
        Classes/vec.cpp \
        # Common
        Common/glut_callbacks.cpp \
        Common/InitShader.cpp \
        Common/model.cpp \
        Common/modelFunctions.cpp \
        Common/ObjLoader.cpp \
 	#Common/WiiUtil.cpp\
        Common/Util.cpp \
   	#Qt
	Qt/mainwindow.cpp\
        #Binaries
	Binaries/MONOLITH.cpp\
	#Binaries/partTest.cpp



HEADERS  += \
        # include
        include/Camera.hpp \
        include/Cameras.hpp \
        include/Engine.hpp \
        include/globals.h \
        include/glut_callbacks.h \
        include/InitShader.hpp \
        # include/KinectInator.hpp \
        include/mat.hpp \
        include/meshMapping.hpp \
        include/model.hpp \
        include/modelFunctions.hpp \
        include/ObjLoader.hpp \
        include/OpenGL.h \
        include/platform.h \
        include/Scene.hpp \
        include/Screen.hpp \
        include/SpelChkCamera.hpp \
        include/Texture.hpp \
        include/TextureMan.hpp \
        include/Timer.hpp \
        include/TransCache.hpp \
        include/Transformation.hpp \
        include/Util.hpp \
        include/vec.hpp \
  	include/mainwindow.h\
	include/MONOLITH.hpp\
	#include/partTest.hpp\
        #WiiUtil.h\
        #Qt


unix:!macx {
LIBS += -lGL -lGLU -lGLEW -L$(CURDIR)/expensiveglut/lib -lexpensiveglut -lm -lboost_thread

INCLUDEPATH +=. ImageVoodoo/include ImageVoodoo/include/ImageMagick-6 include/ expensiveglut/include

LIBS += -L$(CURDIR)/ImageVoodoo/lib -lMagick++ 
}

macx {
LIBS += -lGL -lGLU -lGLEW -L$(CURDIR)/expensiveglut/lib -lexpensiveglut -lm -lboost_thread

INCLUDEPATH +=. ImageVoodoo/include ImageVoodoo/include/ImageMagick-6  include/ expensiveglut/include

LIBS += -L$(CURDIR)/ImageVoodoo/lib -lMagick++ 
}

FORMS    += \
    Qt/mainwindow.ui
