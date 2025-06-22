#!/bin/bash

# This script builds the project in Debug mode with Clang/Clang++ compilers
# Usage: ./build_debug.sh [clean] - if "clean" is passed, will remove the build directory first

# Check if we should clean the build directory
if [ "$1" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf cmake-build-debug
elif [ "$1" = "rebuild" ]; then
    echo "Forcing rebuild (without deleting build directory)..."
    cd cmake-build-debug || exit
    cmake --build . --config Debug --clean-first
    exit 0
fi

# Run CMake configuration with Debug flags
echo "Configuring project with CMake (Debug mode)..."
cmake -B cmake-build-debug\
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_CXX_STANDARD=20 \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \