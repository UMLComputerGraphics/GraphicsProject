#-------------------------------------------------
#
# Project created by QtCreator 2013-05-01T20:09:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpencvTest
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

unix:!macx {
INCLUDEPATH += . $(shell pkg-config opencv --cflags | sed 's/-I//g')
LIBS += $(shell pkg-config opencv --libs)
}

unix:macx {
LIBS += /opt/local/lib/libopencv_calib3d.dylib \
        /opt/local/lib/libopencv_contrib.dylib \
        /opt/local/lib/libopencv_core.dylib \
        /opt/local/lib/libopencv_features2d.dylib \
        /opt/local/lib/libopencv_flann.dylib \
        /opt/local/lib/libopencv_gpu.dylib \
        /opt/local/lib/libopencv_highgui.dylib \
        /opt/local/lib/libopencv_imgproc.dylib \
        /opt/local/lib/libopencv_legacy.dylib \
        /opt/local/lib/libopencv_ml.dylib \
        /opt/local/lib/libopencv_nonfree.dylib \
        /opt/local/lib/libopencv_objdetect.dylib \
        /opt/local/lib/libopencv_photo.dylib \
        /opt/local/lib/libopencv_stitching.dylib \
        /opt/local/lib/libopencv_superres.dylib \
        /opt/local/lib/libopencv_ts.dylib \
        /opt/local/lib/libopencv_video.dylib \
        /opt/local/lib/libopencv_videostab.dylib

INCLUDEPATH += . /opt/local/include/opencv /opt/local/include
}
