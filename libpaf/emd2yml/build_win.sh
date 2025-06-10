
if [ ! -d build ]; then
  mkdir build
fi

cd build

cmake -DWIN_BUILD=1 ../ -DCMAKE_TOOLCHAIN_FILE=toolchain-x86_64-w64-mingw32.cmake
make
cd ..
