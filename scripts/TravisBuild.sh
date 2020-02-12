#!/bin/bash

if [ "$TRAVIS" == "" ]; then
  echo "This script should only be run on Travis CI builds"
  exit 1
fi

set -e
set -u

mkdir build && cd build

qmake=qmake
if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  qmake=/usr/local/opt/qt/bin/qmake
fi

$qmake -config release ..

make

echo "Travis OS Name: $TRAVIS_OS_NAME"

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
  /usr/local/opt/qt/bin/macdeployqt herald.app -dmg
fi
