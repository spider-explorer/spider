#! bash -uvx
bname=`basename $1 .pro`
bname=`basename $bname .`
rm -rf build-$bname.tmp
mkdir build-$bname.tmp
cd build-$bname.tmp
qmake.exe ../$bname.pro
if command -v mingw32-make.exe &> /dev/null
then
  mingw32-make -f Makefile.Release
else
  nmake -f Makefile.Release
fi
if [ $? -eq 0 ]; then
  rm -rf .qmake* debug Makefile* *_plugin_import.cpp ui_*.h rep_*.h
fi
