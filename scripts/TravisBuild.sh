#!/bin/bash

if [ "$TRAVIS" == "" ]; then
  echo "This script should only be run on Travis CI builds"
  exit 1
fi

set -e

mkdir build && cd build

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  Qt5_DIR_OPT="-DQt5_DIR=/usr/local/opt/qt/5.13.2"
  ls -la /usr/local/opt/qt/5.13.2
fi

cmake .. "-DCMAKE_BUILD_TYPE=Release" $Qt5_DIR_OPT

make

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  /usr/local/opt/qt/bin/macdeployqt herald-hub.app -dmg
fi

# This is just for debugging
ls -la
