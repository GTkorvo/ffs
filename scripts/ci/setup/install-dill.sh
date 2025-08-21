#!/bin/bash
echo "Starting install DILL script"
mkdir dill
cd dill
git clone https://github.com/GTKorvo/dill.git source
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
echo "Finish install DILL script"
