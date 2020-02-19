#!/bin/bash

if [ "$TRAVIS" == "" ]; then
  echo "This script should only be run on Travis CI builds"
  exit 1
fi

set -e

mkdir build && cd build

cmake_opts="-DCMAKE_BUILD_TYPE=Release"

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  cmake_opts="$cmake_opts -DQt5_DIR=$(brew --prefix qt5)/lib/cmake/Qt5"
fi

cmake .. $cmake_opts

make

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  /usr/local/opt/qt/bin/macdeployqt herald-hub.app -dmg
fi

# This is just for debugging
ls -la
