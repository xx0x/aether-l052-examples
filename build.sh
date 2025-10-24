#!/bin/bash

BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    cd "$BUILD_DIR"
    make -j4
else
    echo "Build directory does not exist. Please run configure.sh first."
    exit 1
fi