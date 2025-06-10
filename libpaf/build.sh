#/usr/bin/bash

if [ ! -d build ]; then
  mkdir build
fi

cd build

if [ ! -d build_emd2yml ]; then
  mkdir build_emd2yml
  cd build_emd2yml
  cmake ../../emd2yml
  make
  cd ..
fi

cmake ..
make development
rm -r ./stubs
cd ..
