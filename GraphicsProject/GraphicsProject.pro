#-------------------------------------------------
#
# Project created by QtCreator 2013-03-24T02:34:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicsProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Camera.cpp \
    mat.cpp \
    Cameras.cpp \
    vec.cpp \
    Scene.cpp \
    Engine.cpp \
    Object.cpp \
    Screen.cpp \
    Timer.cpp \
    TransCache.cpp \
    Transformation.cpp \
    ParticleSystem.cpp \
    Particle.cpp \
    Texture.cpp

HEADERS  += mainwindow.h \
    globals.h \
    glut_callbacks.h \
    OpenGL.h \
    platform.h \
    Camera.hpp \
    Cameras.hpp \
    Engine.hpp \
    InitShader.hpp \
    mat.hpp \
    model.hpp \
    Object.hpp \
    Scene.hpp \
    Screen.hpp \
    TransCache.hpp \
    Transformation.hpp \
    vec.hpp \
    ParticleSystem.hpp \
    Particle.hpp \
    Texture.hpp \
    Timer.hpp

INCLUDEPATH +=. /opt/local/include/ImageMagick/
LIBS += -L/opt/local/lib -lMagick++

FORMS    += mainwindow.ui
