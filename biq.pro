#-------------------------------------------------
#
# Project created by QtCreator 2013-11-15T19:44:12
#
#-------------------------------------------------

QT  += core gui sql concurrent

TARGET  = biq
CONFIG  += console warn_off

DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT QT_NO_DEBUG

TEMPLATE = app

INCLUDEPATH += /usr/include/ImageMagick/

SOURCES += main.cpp \
    batchjob.cpp \
    executor.cpp

HEADERS += \
    batchjob.h \
    executor.h
