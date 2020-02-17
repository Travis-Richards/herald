#!/bin/bash

if [ "$TRAVIS" == "" ]; then
  echo "This script should only be run on Travis CI builds"
  exit 1
fi

set -e
set -u

mkdir build && cd build

cmake .. -DCMAKE_BUILD_TYPE=Release

make

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  /usr/local/opt/qt/bin/macdeployqt herald-hub.app -dmg
fi

# This is just for debugging
ls -la
