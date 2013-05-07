#-------------------------------------------------
#
# Project created by QtCreator 2013-05-01T20:09:45
#
#-------------------------------------------------

QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpencvTest
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

unix:!macx {
LIBS += /usr/local/lib/libopencv_calib3d.so \
	/usr/local/lib/libopencv_contrib.so \
	/usr/local/lib/libopencv_core.so \	
	/usr/local/lib/libopencv_features2d.so \
	/usr/local/lib/libopencv_flann.so \
	/usr/local/lib/libopencv_gpu.so \
	/usr/local/lib/libopencv_highgui.so \
	/usr/local/lib/libopencv_imgproc.so \
	/usr/local/lib/libopencv_legacy.so \
	/usr/local/lib/libopencv_ml.so \
	/usr/local/lib/libopencv_nonfree.so \
	/usr/local/lib/libopencv_objdetect.so \
	/usr/local/lib/libopencv_photo.so \
	/usr/local/lib/libopencv_stitching.so \
	/usr/local/lib/libopencv_superres.so \
	/usr/local/lib/libopencv_ts.so \
	/usr/local/lib/libopencv_video.so \
	/usr/local/lib/libopencv_videostab.so  

INCLUDEPATH += . /usr/local/include /usr/local/include/opencv
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
