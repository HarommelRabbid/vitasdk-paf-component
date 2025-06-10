
TEST_MODE=0
SDK_PATH=${VITASDK}

if [ ${TEST_MODE} -ne 0 ]; then
  SDK_PATH="./vitasdk"
fi

echo "vitasdk path: \"${SDK_PATH}\""

if [ ! -d ${SDK_PATH} ]; then
  echo "vitasdk not found."
  if [ ${TEST_MODE} -eq 0 ]; then
    exit 1
  fi

  echo "TestMode enabled."
  echo "will vitasdk installing"

  VITASDK_LINUX="https://github.com/vitasdk/autobuilds/releases/download/master-linux-v2.529/vitasdk-x86_64-linux-gnu-2025-03-11_15-12-52.tar.bz2"
  VITASDK_LINUX="https://github.com/vitasdk/autobuilds/releases/download/master-win-v2.530/vitasdk-x86_64-w64-mingw32-2025-04-21_10-40-48.tar.bz2"

  curl -L ${VITASDK_LINUX} | tar xj -C "./"
fi

if [ ! -d build ]; then
  mkdir build
  cd build
  if [ ! -d ScePaf-RE ]; then
    git clone https://github.com/GrapheneCt/ScePaf-RE
    cd ScePaf-RE
    git checkout b76d6ae00516b107d37e7e8a1a16211e3b729a07
    git branch
    patch -p2 < "../../vitasdk_2.patch"
    cd ..
  fi

  if [ ! -d build_emd2yml ]; then
    mkdir build_emd2yml
    cd build_emd2yml
    cmake ../../libpaf/emd2yml
    make
    cd ..
  fi

  if [ ! -d build_libpaf ]; then
    mkdir build_libpaf
    cd build_libpaf
    ../build_emd2yml/emd2yml > ScePaf.yml
    vita-libs-gen-2 -yml=./ScePaf.yml -output=./ -cmake=true
    cmake . && make -j$(nproc)
    cd ..
  fi

  cd ..
fi

if [ ! -d final ]; then
  mkdir final
  mkdir final/include
  mkdir final/include/psp2
  mkdir final/lib
fi

cp -r ./build/ScePaf-RE/include ./final
cp -r ./build/build_libpaf/*.a ./final/lib

cp "./include/map" "./final/include/paf/std/map"
cp "./include/math.h" "./final/include/paf/math/math.h"
cp "./include/paf.h" "./final/include/psp2/paf.h"

INCLUDE_BASE="${SDK_PATH}/arm-vita-eabi/include"
STUB_BASE="${SDK_PATH}/arm-vita-eabi/lib"

if [ -e "${STUB_BASE}/libScePaf_stub.a" ]; then
  rm ${STUB_BASE}/libScePaf_stub.a
fi

if [ -e "${STUB_BASE}/libScePaf_stub_weak.a" ]; then
  rm ${STUB_BASE}/libScePaf_stub_weak.a
fi

if [ -e "${INCLUDE_BASE}/psp2/paf.h" ]; then
  rm ${INCLUDE_BASE}/psp2/paf.h
fi

if [ -d "${INCLUDE_BASE}/psp2/paf" ]; then
  rm -r ${INCLUDE_BASE}/psp2/paf
fi

cp -r "./final/include" "${SDK_PATH}/arm-vita-eabi"
cp -r "./final/lib" "${SDK_PATH}/arm-vita-eabi"

echo "paf-component install successful."

exit 0
