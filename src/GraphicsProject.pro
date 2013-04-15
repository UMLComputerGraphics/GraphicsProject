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

SOURCES += \
           # Classes
           Classes/Camera.cpp \
           Classes/Cameras.cpp \
           Classes/Engine.cpp \
           #Classes/KinectInator.cpp \
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
           Common/Util.cpp \
           #Common/WiiUtil.cpp \
           # QT
           Qt/mainwindow.cpp \
           Qt/main.cpp

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
            # QT
            include/mainwindow.h

# Common Settings
INCLUDEPATH += . ./include/
LIBS += -lMagick++

# Linux Settings
unix:!macx {
LIBS += -lGL -lGLU -lGLEW -lglut -lm
INCLUDEPATH += /usr/include/ImageMagick/
}

# Mac OSX Settings
macx {
LIBS += -framework Carbon -framework OpenGL -framework GLUT
INCLUDEPATH += /opt/local/include/ImageMagick/
LIBS += -L/opt/local/lib
}

FORMS    += \
    Qt/mainwindow.ui

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
    ../src/Textures/GoodTextures_0013291.jpg
