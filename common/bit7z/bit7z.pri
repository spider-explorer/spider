#VERSION  = 3.1.3

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/lib/7zSDK/CPP/
DEFINES += _UNICODE _7Z_VOL

SOURCES += $$PWD/lib/7zSDK/C/Alloc.c \
           $$PWD/lib/7zSDK/CPP/Windows/FileIO.cpp \
           $$PWD/lib/7zSDK/CPP/Windows/FileDir.cpp \
           $$PWD/lib/7zSDK/CPP/Windows/FileName.cpp \
           $$PWD/lib/7zSDK/CPP/Windows/FileFind.cpp \
           $$PWD/lib/7zSDK/CPP/7zip/Common/FileStreams.cpp \
           $$PWD/lib/7zSDK/CPP/7zip/Common/StreamObjects.cpp \
           $$PWD/lib/7zSDK/CPP/Common/IntToString.cpp \
           $$PWD/lib/7zSDK/CPP/Common/MyString.cpp \
           $$PWD/lib/7zSDK/CPP/Common/MyVector.cpp \
           $$PWD/src/bit7zlibrary.cpp \
           $$PWD/src/bitarchivecreator.cpp \
           $$PWD/src/bitarchivehandler.cpp \
           $$PWD/src/bitarchiveinfo.cpp \
           $$PWD/src/bitarchiveitem.cpp \
           $$PWD/src/bitarchiveopener.cpp \
           $$PWD/src/bitcompressor.cpp \
           $$PWD/src/bitexception.cpp \
           $$PWD/src/bitextractor.cpp \
           $$PWD/src/bitformat.cpp \
           $$PWD/src/bitguids.cpp \
           $$PWD/src/bitinputarchive.cpp \
           $$PWD/src/bitmemcompressor.cpp \
           $$PWD/src/bitmemextractor.cpp \
           $$PWD/src/bitpropvariant.cpp \
           $$PWD/src/bitstreamcompressor.cpp \
           $$PWD/src/bitstreamextractor.cpp \
           $$PWD/src/bufferextractcallback.cpp \
           $$PWD/src/bufferupdatecallback.cpp \
           $$PWD/src/callback.cpp \
           $$PWD/src/cbufoutstream.cpp \
           $$PWD/src/cmultivoloutstream.cpp \
           $$PWD/src/cstdinstream.cpp \
           $$PWD/src/cstdoutstream.cpp \
           $$PWD/src/extractcallback.cpp \
           $$PWD/src/fileextractcallback.cpp \
           $$PWD/src/fileupdatecallback.cpp \
           $$PWD/src/fsindexer.cpp \
           $$PWD/src/fsitem.cpp \
           $$PWD/src/fsutil.cpp \
           $$PWD/src/opencallback.cpp \
           $$PWD/src/streamextractcallback.cpp \
           $$PWD/src/streamupdatecallback.cpp \
           $$PWD/src/updatecallback.cpp

HEADERS += $$PWD/include/bit7z.hpp \
           $$PWD/include/bit7zlibrary.hpp \
           $$PWD/include/bitarchivecreator.hpp \
           $$PWD/include/bitarchivehandler.hpp \
           $$PWD/include/bitarchiveinfo.hpp \
           $$PWD/include/bitarchiveitem.hpp \
           $$PWD/include/bitarchiveopener.hpp \
           $$PWD/include/bitcompressionlevel.hpp \
           $$PWD/include/bitcompressionmethod.hpp \
           $$PWD/include/bitcompressor.hpp \
           $$PWD/include/bitexception.hpp \
           $$PWD/include/bitextractor.hpp \
           $$PWD/include/bitformat.hpp \
           $$PWD/include/bitguids.hpp \
           $$PWD/include/bitinputarchive.hpp \
           $$PWD/include/bitmemcompressor.hpp \
           $$PWD/include/bitmemextractor.hpp \
           $$PWD/include/bitpropvariant.hpp \
           $$PWD/include/bitstreamcompressor.hpp \
           $$PWD/include/bitstreamextractor.hpp \
           $$PWD/include/bittypes.hpp \
           $$PWD/include/bufferextractcallback.hpp \
           $$PWD/include/bufferupdatecallback.hpp \
           $$PWD/include/callback.hpp \
           $$PWD/include/cbufoutstream.hpp \
           $$PWD/include/cmultivoloutstream.hpp \
           $$PWD/include/cstdinstream.hpp \
           $$PWD/include/cstdoutstream.hpp \
           $$PWD/include/extractcallback.hpp \
           $$PWD/include/fileextractcallback.hpp \
           $$PWD/include/fileupdatecallback.hpp \
           $$PWD/include/fsindexer.hpp \
           $$PWD/include/fsitem.hpp \
           $$PWD/include/fsutil.hpp \
           $$PWD/include/opencallback.hpp \
           $$PWD/include/streamextractcallback.hpp \
           $$PWD/include/streamupdatecallback.hpp \
           $$PWD/include/updatecallback.hpp
