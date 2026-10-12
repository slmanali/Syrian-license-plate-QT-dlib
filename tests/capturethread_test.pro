QT = core testlib
CONFIG += console testcase c++11 link_pkgconfig
CONFIG -= app_bundle
TEMPLATE = app
TARGET = capturethread_test
INCLUDEPATH += ..
SOURCES += capturethread_test.cpp ../capturethread.cpp
HEADERS += ../capturethread.h
PKGCONFIG += libvlc
packagesExist(opencv4) {
    PKGCONFIG += opencv4
} else {
    # Ubuntu's component development packages do not install opencv4.pc.
    INCLUDEPATH += /usr/include/opencv4
    LIBS += -lopencv_videoio -lopencv_imgproc -lopencv_core
}
