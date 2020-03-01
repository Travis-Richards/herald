#!/bin/bash

if [ "$TRAVIS" == "" ]; then
  echo "This script should only be run on Travis CI builds"
  exit 1
fi

set -e

# A working space for all generated files.
mkdir build && cd build

# Where the local install will go.
local_instdir="$PWD/herald"

# Make sure we get full optimizations
# for deployment builds
cmake_opts="-DCMAKE_BUILD_TYPE=Release"

# Ensure CMake installs the project into a location
# within the build directory.
cmake_opts="$cmake_opts -DCMAKE_INSTALL_PREFIX=herald"

# Special care has to be taken on macOS
# due to the way brew installs packages.
if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  cmake_opts="$cmake_opts -DQt5_DIR=$(brew --prefix qt5)/lib/cmake/Qt5"
fi

cmake .. $cmake_opts
make
make install

# Make sure we run deployment scripts for macOS builds.
if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  /usr/local/opt/qt/bin/macdeployqt herald -dmg
fi

# This is just for debugging
ls -la
ls -la herald
