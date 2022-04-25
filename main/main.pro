QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TEMPLATE  = lib
#CONFIG += staticlib
CONFIG += dll

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$(HOME)/qt/common/common.pri)

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function

#LIBS += -larchive -lz -lbz2 -llzma -liconv -lbcrypt -lexpat -lb2 -llz4
#LIBS += -ladvapi32 -lole32 -loleaut32 -luser32 -luuid
LIBS += $$PWD/efsw/libefsw.a

SOURCES += \
    3rdparty/EasyOLE.cpp \
    binarysettings.cpp \
    chooseqtprojectdialog.cpp \
    cmdprocess.cpp \
    datetimemanager.cpp \
    entry.cpp \
    envvardialog.cpp \
    explorerform.cpp \
    favmanager.cpp \
    flowlayout.cpp \
    jsonsettings.cpp \
    mainwindow.cpp \
    msys2dialog.cpp \
    plainiconfsmodel.cpp \
    programdb.cpp \
    projectchecker.cpp \
    qsettings_binary.cpp \
    qsettings_json.cpp \
    recursivefilelister.cpp \
    recursivefilesystemwatcher.cpp \
    repodialog.cpp \
    repositorydirdialog.cpp \
    repositorydirview.cpp \
    scoopsearchdialog.cpp \
    scoopsoftwaredialog.cpp \
    scoopsoftwarelist.cpp \
    searchthread.cpp \
    spidercore.cpp \
    spiderform.cpp \
    spidermain.cpp \
    spiderprocess.cpp \
    test01dialog.cpp \
    variantserializer.cpp \
    windowsutils.cpp \
    wslcore.cpp \
    wslfilesystemmodel.cpp

HEADERS += \
    3rdparty/EasyOLE.h \
    3rdparty/WinReg.hpp \
    3rdparty/prettyprint.hpp \
    binarysettings.h \
    chooseqtprojectdialog.h \
    cmdprocess.h \
    common.h \
    datetimemanager.h \
    envvardialog.h \
    explorerform.h \
    favmanager.h \
    flowlayout.h \
    jsonsettings.h \
    mainwindow.h \
    msys2dialog.h \
    plainiconfsmodel.h \
    programdb.h \
    projectchecker.h \
    qsettings_binary.h \
    qsettings_json.h \
    recursivefilelister.h \
    recursivefilesystemwatcher.h \
    repodialog.h \
    repositorydirdialog.h \
    repositorydirview.h \
    scoopsearchdialog.h \
    scoopsoftwaredialog.h \
    scoopsoftwarelist.h \
    searchthread.h \
    spidercore.h \
    spiderform.h \
    spidermain.h \
    spiderprocess.h \
    test01dialog.h \
    variantserializer.h \
    windowsutils.h \
    wslcore.h \
    wslfilesystemmodel.h

FORMS += \
    chooseqtprojectdialog.ui \
    envvardialog.ui \
    explorerform.ui \
    mainwindow.ui \
    msys2dialog.ui \
    repodialog.ui \
    repositorydirdialog.ui \
    scoopsearchdialog.ui \
    scoopsoftwaredialog.ui \
    spiderform.ui \
    spidermain.ui \
    test01dialog.ui

INCLUDEPATH += $$(HOME)/include

DESTDIR = $$PWD/..

TARGET = $${TARGET}-$${QMAKE_HOST.arch}

#message($$QMAKE_QMAKE)
contains(QMAKE_QMAKE, .*\-static\/.*) {
    message( "[STATIC BUILD]" )
    DEFINES += QT_STATIC_BUILD
    TARGET = $${TARGET}-static
} else {
    message( "[SHARED BUILD]" )
}

# https://www.flaticon.com/free-icon/tarantula_2959301?term=spider&page=1&position=7&page=1&position=7&related_id=2959301
# https://www.hipdf.com/jp/png-to-ico
RC_ICONS = tarantula.ico

RESOURCES += \
    spider2.qrc \
    vscode.qrc

#include(SingleApplication/singleapplication.pri)
#DEFINES += QAPPLICATION_CLASS=QApplication

