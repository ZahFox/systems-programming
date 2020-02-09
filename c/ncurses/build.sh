#!/bin/bash

mkdir -p build
pushd build

if [ "${1,,}" = "release" ]; then
  echo "building the application in release mode"
  cmake -DCMAKE_BUILD_TYPE=Release ..
else
  echo "building the application in debug mode"
  cmake -DCMAKE_BUILD_TYPE=Debug ..
fi

make
popd
