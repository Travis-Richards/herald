#!/bin/bash

if [ "$TRAVIS" == "" ]; then
  echo "This script should only be run on Travis CI builds"
  exit 1
fi

set -e

mkdir build && cd build

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  Qt5_DIR_OPT="-DQt5Core_DIR=/usr/local/opt/qt -DQt5Widgets_DIR=/usr/local/opt/qt"
fi

# Debug statement
ls -la /usr/local/opt/qt

cmake .. "-DCMAKE_BUILD_TYPE=Release" $Qt5_DIR_OPT

make

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  /usr/local/opt/qt/bin/macdeployqt herald-hub.app -dmg
fi

# This is just for debugging
ls -la
