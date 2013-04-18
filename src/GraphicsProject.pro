#-------------------------------------------------
#
# Project created by QtCreator 2013-03-24T02:34:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = GraphicsProject
TEMPLATE = app

release: DESTDIR = Binaries/
debug:   DESTDIR = Binaries/

OBJECTS_DIR =  Qt/obj/
MOC_DIR =  Qt/moc/
RCC_DIR =  Qt/rcc/
UI_DIR =  Qt/ui/

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
        Classes/MONOLITH.cpp \
        # Common
        Common/glut_callbacks.cpp \
        Common/InitShader.cpp \
        Common/model.cpp \
        Common/modelFunctions.cpp \
        Common/ObjLoader.cpp \
        #Common/WiiUtil.cpp\
        Common/Util.cpp \
        #Qt
        Qt/mainwindow.cpp

HEADERS  += include/Camera.hpp \
            include/Cameras.hpp \
            include/Engine.hpp \
            include/globals.h \
            include/glut_callbacks.h \
            include/InitShader.hpp \
            #include/KinectInator.hpp\
            include/mat.hpp \
            include/meshMapping.hpp \
            include/modelFunctions.hpp \
            include/model.hpp \
            include/Object.hpp \
            include/ObjLoader.hpp \
            include/OpenGL.h \ 
            include/Particle.hpp \
            include/ParticleSystem.hpp \
            include/platform.h \ 
            include/Scene.hpp \ 
            include/Screen.hpp \ 
            include/SpelchkCamera.hpp \
            include/Texture.hpp \ 
            include/TextureMan.hpp \ 
            include/Timer.hpp \ 
            include/TransCache.hpp \ 
            include/Transformation.hpp \ 
            include/Util.hpp \ 
            include/vec.hpp \ 
            include/WiiUtil.h \
            # QT Stuff >(^_^)>
            include/mainwindow.h \
            include/MONOLITH.hpp

# Common Settings
INCLUDEPATH += . ./include/
LIBS += -lMagick++


# Linux Settings
unix:!macx {
LIBS += -lGL -lGLU -lGLEW -lglut -lX11 -lm -lMagick++
INCLUDEPATH +=. /usr/include/ImageMagick/ include/
}

# Mac OSX Settings
macx {
LIBS += -framework Carbon -framework OpenGL -framework GLUT
INCLUDEPATH += /opt/local/include/ImageMagick/
LIBS += -L/opt/local/lib
}

FORMS    += \
    Qt/mainwindow.ui
