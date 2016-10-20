QT += core
QT -= gui
QT += multimedia

CONFIG += c++11

TARGET = DetectorSueno
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    helpers.cpp \
    eyecenter.cpp \
    eyecorner.cpp \
    system.cpp \
    message.cpp \
    gps.cpp

INCLUDEPATH += /usr/include/opencv
LIBS += -L/usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_bgsegm -lopencv_video -lopencv_videoio -lopencv_objdetect

HEADERS += \
    helpers.h \
    constants.h \
    eyecenter.h \
    eyecorner.h \
    system.h \
    message.h \
    gps.h
