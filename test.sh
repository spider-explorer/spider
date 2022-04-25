#! bash -uvx
set -e
cwd=`pwd`
LOCALE=C astyle --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx
