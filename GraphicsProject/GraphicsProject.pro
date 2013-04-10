#-------------------------------------------------
#
# Project created by QtCreator 2013-03-24T02:34:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicsProject
TEMPLATE = app


SOURCES += main.cpp\
        # Classes
        Camera.cpp \
        Cameras.cpp \
        Engine.cpp \
        # KinectInator.cpp \
        mat.cpp \
        meshMapping.cpp \
        Object.cpp \
        Particle.cpp \
        ParticleSystem.cpp \
        Scene.cpp \
        Screen.cpp \
        SpelchkCamera.cpp \
        Texture.cpp \
        TextureMan.cpp \
        Timer.cpp \
        TransCache.cpp \
        Transformation.cpp \
        vec.cpp \
        # Common
        glut_callbacks.cpp \
        InitShader.cpp \
        model.cpp \
        modelFunctions.cpp \
        ObjLoader.cpp \
        Util.cpp \
    mainwindow.cpp
        #WiiUtil.cpp\
        #Qt



HEADERS  += \
        # include
        Camera.hpp \
        Cameras.hpp \
        Engine.hpp \
        globals.h \
        glut_callbacks.h \
        InitShader.hpp \
        # KinectInator.hpp \
        mat.hpp \
        meshMapping.hpp \
        model.hpp \
        modelFunctions.hpp \
        ObjLoader.hpp \
        OpenGL.h \
        platform.h \
        Scene.hpp \
        Screen.hpp \
        SpelChkCamera.hpp \
        Texture.hpp \
        TextureMan.hpp \
        Timer.hpp \
        TransCache.hpp \
        Transformation.hpp \
        Util.hpp \
        vec.hpp \
    mainwindow.h
        #WiiUtil.h\
        #Qt


unix:!macx {
LIBS += -lGL -lGLU -lGLEW -lglu -lm

INCLUDEPATH +=. /usr/include/ImageMagick/
LIBS += -lMagick++
}

macx {
LIBS += -framework Carbon -framework OpenGL -framework GLUT

INCLUDEPATH +=. /opt/local/include/ImageMagick/
LIBS += -L/opt/local/lib -lMagick++
}

FORMS    += \
    mainwindow.ui

OTHER_FILES += \
    ../src/models/wineglass.obj \
    ../src/models/waterglass.obj \
    ../src/models/cognacglass.obj \
    ../src/models/cognacglass-LUMPY.obj \
    ../src/models/spyT.obj \
    ../src/models/spy.obj \
    ../src/models/soldier.obj \
    ../src/models/sniper.obj \
    ../src/models/scout.obj \
    ../src/models/rainbow_dashT.obj \
    ../src/models/rainbow_dashT.mtl \
    ../src/models/rainbow_dash_all-in-one.OBJ \
    ../src/models/pyro.obj \
    ../src/models/particle.obj \
    ../src/models/medicT.obj \
    ../src/models/medic.obj \
    ../src/models/heavyT.obj \
    ../src/models/heavyhead.obj \
    ../src/models/engineer.obj \
    ../src/models/demo.obj \
    ../src/models/bottle-b.obj \
    ../src/models/bottle-a.obj \
    ../src/models/bottle_wine_med.obj \
    ../src/models/bottle_wine_low.obj \
    ../src/models/bottle_liquor_med.obj \
    ../src/models/bottle_liquor_low.obj \
    ../src/models/candlestick.obj \
    ../src/models/candle.obj \
    ../src/models/candle_full.obj \
    ../src/models/bottle_wine_high.obj \
    ../src/models/bottle_liquor_high.obj \
    ../src/models/bottle_liquor_high_old.obj \
    ../src/Textures/GrassGreenTexture0002.jpg \
    ../src/Textures/GoodTextures_0013779.jpg \
    ../src/Textures/GoodTextures_0013423.jpg \
    ../src/Textures/GoodTextures_0013418.jpg \
    ../src/Textures/GoodTextures_0013291.jpg \
