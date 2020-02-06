#!/bin/bash

if [ "$TRAVIS" == "" ]; then
  echo "This script should only be run on Travis CI builds"
  exit 1
fi

set -e
set -u

mkdir build && cd build

cmake_opts=

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  cmake_opts="$cmake_opts -DQt5_DIR=$(brew --prefix qt5)/lib/cmake/Qt5"
fi

cmake_opts="$cmake_opts -DCMAKE_BUILD_TYPE=Release"
cmake_opts="$cmake_opts -DCMAKE_INSTALL_PREFIX=$PWD/dmsc"

cmake .. $cmake_opts

make
