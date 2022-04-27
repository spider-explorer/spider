QT += core gui widgets
equals(QT_MAJOR_VERSION, 6):QT += core5compat

CONFIG += c++17
CONFIG += console

#TEMPLATE = lib
#CONFIG += staticlib
#CONFIG += dll

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

gcc:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function

HEADERS += utf8LogHandler.h

SOURCES += %1.cpp

INCLUDEPATH += $$PWD
INCLUDEPATH += $$(HOME)/include

LIBS += -L$$[QT_INSTALL_PREFIX]/lib

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
#include($$(HOME)/qt/common/ZipLib/ZipLib.pri)
