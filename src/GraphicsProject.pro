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
	include/MONOLITH.hpp


unix:!macx {
INCLUDEPATH +=. include /usr/include/ImageMagick/
LIBS += -lGL -lGLU -lGLEW -lglut -lm -L/usr/lib/ImageMagick -lMagick++
}

macx {
LIBS += -framework Carbon -framework OpenGL -framework GLUT
INCLUDEPATH +=. include /opt/local/include/ImageMagick/
LIBS += -L/opt/local/lib -lMagick++
}

FORMS    += \
    Qt/mainwindow.ui
