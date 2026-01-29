#!/bin/bash

# This script builds the project in Debug mode with Clang/Clang++ compilers
# Usage: ./build_debug.sh [clean] - if "clean" is passed, will remove the build cache first

# Check if we should clean the build directory
if [ "$1" = "clean" ]; then
    echo "Cleaning build cache ..."
    rm build/CMakeCache.txt
elif [ "$1" = "rebuild" ]; then
    echo "Forcing rebuild (without deleting build directory)..."
    cmake --build build -j10 --clean-first
    exit 0
fi

# Run CMake configuration with Debug flags
echo "Configuring project with CMake (Debug mode)..."
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug