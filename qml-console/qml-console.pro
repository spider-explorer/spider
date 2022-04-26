QT += quick gui widgets

CONFIG += c++17
CONFIG += console

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function

SOURCES += \
        main.cpp

RESOURCES +=

HEADERS += \
    AJSEngine.h \
    EscapeString.h \
    VariantToText.h \
    app-data.h

DISTFILES += \
    code2ary.mjs \
    main.mjs \
    miniMAL.mjs \
    main.qml

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

include($$(HOME)/qt/common/common.pri)
