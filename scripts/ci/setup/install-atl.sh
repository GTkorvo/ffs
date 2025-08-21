#!/bin/bash
echo "Starting install ATL script"
mkdir atl
cd atl
git clone https://github.com/GTKorvo/atl.git source
mkdir build
cd build
cmake \
  -DCMAKE_BUILD_TYPE=$1 \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=${PWD}/../install \
  -DCMAKE_INSTALL_LIBDIR=${PWD}/../lib/Release \
  -DCMAKE_INSTALL_BINDIR=${PWD}/../bin/Release \
  ../source
cmake --build . -j4 --config $1
cmake --install . --config $1
echo "Finish install ATL script, dest is ${PWD}/../install"
