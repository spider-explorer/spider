QT += core gui widgets network

#CONFIG += c++17

HEADERS += \
    $$PWD/jarchiver.h \
    $$PWD/jchecksum.h \
    $$PWD/jinstaller.h \
    $$PWD/jlistwidget.h \
    $$PWD/jnetwork.h \
    $$PWD/junctionmanager.h \
    $$PWD/pcloudapi.h \
    $$PWD/utf8LogHandler.h \
    $$PWD/varianttojson.h \
    $$PWD/youtubeapi.h

SOURCES += \
    $$PWD/jarchiver.cpp \
    $$PWD/jchecksum.cpp \
    $$PWD/jinstaller.cpp \
    $$PWD/jlistwidget.cpp \
    $$PWD/jnetwork.cpp \
    $$PWD/junctionmanager.cpp \
    $$PWD/pcloudapi.cpp \
    $$PWD/youtubeapi.cpp

INCLUDEPATH += $$PWD
msvc:INCLUDEPATH += $$(HOME)/conan/binaries/include

gcc {
    LIBS += -larchive -lz -lbz2 -llzma -liconv -lbcrypt -lexpat -lb2 -llz4
    LIBS += -lcrypto -lssl
    LIBS += -ladvapi32 -lole32 -loleaut32 -luser32 -luuid
}
msvc {
    LIBS += $$$(HOME)/conan/binaries/lib/archive.lib
    LIBS += -ladvapi32 # CreateJunction()
}
