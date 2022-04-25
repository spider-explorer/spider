QT += core gui widgets

CONFIG += c++17
CONFIG += console

#TEMPLATE = lib
#CONFIG += staticlib
#CONFIG += dll

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# disables all the APIs deprecated before Qt 6.0.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

gcc:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function

HEADERS += utf8LogHandler.h

SOURCES += dummy-project.cpp

INCLUDEPATH += $$PWD
INCLUDEPATH += $$(HOME)/include

DESTDIR = $$PWD

TARGET = $${TARGET}-$${QMAKE_HOST.arch}

#message($$QMAKE_QMAKE)
contains(QMAKE_QMAKE, .*\-static\/.*) {
    message( "[STATIC BUILD]" )
    DEFINES += QT_STATIC_BUILD
    TARGET = $${TARGET}-static
} else {
    message( "[SHARED BUILD]" )
}

#include($$(HOME)/qt/common/common.pri)
#include($$(HOME)/qt/common/boost/boost.pri)
