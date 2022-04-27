QT += core gui widgets network

#CONFIG += c++17

HEADERS += \
    $$PWD/MemoryModule.h \
    $$PWD/jchecksum.h \
    $$PWD/jlistwidget.h \
    $$PWD/jnetwork.h \
    $$PWD/junctionmanager.h \
    $$PWD/pcloudapi.h \
    $$PWD/utf8LogHandler.h \
    $$PWD/variantserializer.h \
    $$PWD/varianttojson.h \
    $$PWD/youtubeapi.h

#    $$PWD/zlib/crc32.h \
#    $$PWD/zlib/crypt.h \
#    $$PWD/zlib/deflate.h \
#    $$PWD/zlib/gzguts.h \
#    $$PWD/zlib/inffast.h \
#    $$PWD/zlib/inffixed.h \
#    $$PWD/zlib/inflate.h \
#    $$PWD/zlib/inftrees.h \
#    $$PWD/zlib/iowin32.h \
#    $$PWD/zlib/trees.h \
#    $$PWD/zlib/unzip.h \
#    $$PWD/zlib/zconf.h \
#    $$PWD/zlib/zip.h \
#    $$PWD/zlib/zlib.h \
#    $$PWD/zlib/zutil.h

gcc:HEADERS += $$PWD/jarchiver.h $$PWD/jinstaller.h

SOURCES += \
    $$PWD/MemoryModule.c \
    $$PWD/jchecksum.cpp \
    $$PWD/jlistwidget.cpp \
    $$PWD/jnetwork.cpp \
    $$PWD/junctionmanager.cpp \
    $$PWD/pcloudapi.cpp \
    $$PWD/variantserializer.cpp \
    $$PWD/youtubeapi.cpp

#    $$PWD/zlib/adler32.c \
#    $$PWD/zlib/compress.c \
#    $$PWD/zlib/crc32.c \
#    $$PWD/zlib/deflate.c \
#    $$PWD/zlib/gzclose.c \
#    $$PWD/zlib/gzlib.c \
#    $$PWD/zlib/gzread.c \
#    $$PWD/zlib/gzwrite.c \
#    $$PWD/zlib/infback.c \
#    $$PWD/zlib/inffast.c \
#    $$PWD/zlib/inflate.c \
#    $$PWD/zlib/inftrees.c \
#    $$PWD/zlib/iowin32.c \
#    $$PWD/zlib/trees.c \
#    $$PWD/zlib/uncompr.c \
#    $$PWD/zlib/unzip.c \
#    $$PWD/zlib/zip.c \
#    $$PWD/zlib/zutil.c

gcc:SOURCES += $$PWD/jarchiver.cpp $$PWD/jinstaller.cpp

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/zlib
msvc:INCLUDEPATH += $$(HOME)/conan/binaries/include
msvc:LIBS += -L$$(HOME)/conan/binaries/lib

msvc:QMAKE_LFLAGS += /FORCE

msvc:LIBS += -LC:/qtb/qt-6.2.2-static-msvc2019-x86_64/lib

msvc:INCLUDEPATH += C:/qtb/bit7z/include
msvc:LIBS += -LC:/qtb/bit7z/lib bit7z64.lib

msvc:DEFINES += LIBARCHIVE_STATIC

gcc {
    LIBS += -larchive -lz -lbz2 -llzma -liconv -lbcrypt -lexpat -lb2 -llz4
    LIBS += -lcrypto -lssl
    LIBS += -ladvapi32 -lole32 -loleaut32 -luser32 -luuid
}
msvc {
    #LIBS += archive_static.lib
    #LIBS += archive.lib
    LIBS += -ladvapi32 # CreateJunction()
}
