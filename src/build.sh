#!/bin/bash

CLEAN=${1}

if [ "$CLEAN" == "--clean" ]; then
    echo "Cleaning build directory..."
    rm -rf build
fi

mkdir -p build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/local/Qt-6.9.0
make

