#-------------------------------------------------
#
# Project created by QtCreator 2016-12-21T18:09:49
#
#-------------------------------------------------

QT       += core gui serialport network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plate_Rec
CONFIG += force_debug_info c++11
TEMPLATE = app
#####################    OPEN_CV DIR   #####################
INCLUDEPATH += F:\my_work\install341\include

#####################    Vlc DIR   #####################
INCLUDEPATH +=F:\WORK\VLC\include

#####################    Vlc LIB  #####################
LIBS += F:\WORK\VLC\vlc_lib\libvlc.lib
LIBS += F:\WORK\VLC\vlc_lib\libvlccore.lib

####################    DLIB DIR   #####################
INCLUDEPATH += F:\WORK\dlib-19.10

#####################    OPEN_CV LIB  #####################
 CONFIG(debug, debug|release) {
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_core341d.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_highgui341d.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_imgcodecs341d.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_imgproc341d.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_objdetect341d.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_video341d.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_videoio341d.lib
    }
    else {

        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_core341.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_highgui341.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_imgcodecs341.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_imgproc341.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_objdetect341.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_video341.lib
        LIBS += F:\my_work\install341\x64\vc14\lib\opencv_videoio341.lib
    }
#####################    YOLO DIR   #####################
INCLUDEPATH += F:\my_work\company\Plate1\include

#####################    YOLO LIB  #####################
LIBS += F:\my_work\company\MT\MT\lib\yolo_cpp_dll.lib
LIBS += F:\my_work\company\Plate1\dlib19.10.0_release_64bit_msvc1900.lib
SOURCES += main.cpp\
        mainwindow.cpp\
    capturethread.cpp \
    form.cpp \
    ip_camera.cpp \
    md5.cpp \
    reg.cpp \
    #plate_thread.cpp
    src/VideoSortDetector.cpp \
    src/Utils.cpp \
    src/Detector.cpp \
    src/YoloDetector.cpp \
    ../../../WORK/dlib-19.10/dlib/all/source.cpp \
    httprequestworker.cpp

HEADERS  += mainwindow.h \
    capturethread.h \
    form.h \
    ip_camera.h \
    md5.h \
    reg.h \
    include/config.h \
    include/Detector.h \
    include/Utils.h \
    include/VideoSortDetector.h \
    include/YoloDetector.h \
    httprequestworker.h
    #plate_thread.h

FORMS    += mainwindow.ui \
    form.ui \
    ip_camera.ui \
    reg.ui


DISTFILES += \
    bulid/release/Cars.sqlite

RESOURCES += \
    release/imageformats/rcc.qrc

