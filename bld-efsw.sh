#! bash -uvx
set -e
time pacman -S --needed --noconfirm --disable-download-timeout \
    git make mingw-w64-x86_64-gcc
rm -rf efsw2.tmp
git clone https://github.com/SpartanJ/efsw efsw2.tmp
cd efsw2.tmp
mkdir build && cd build
cmake .. -G "MSYS Makefiles" -DBUILD_SHARED_LIBS=OFF
cmake --build . -- -j
cd ..
cp -rp include/efsw ../
cp -rp build/libefsw.a ../efsw
