#!/bin/bash

PROJECT_NAME="stm32l052_blink"
BUILD_DIR="build"
JLINK_FILE="${BUILD_DIR}/${PROJECT_NAME}.jlink"

cd "$BUILD_DIR"
echo "Running J-Link with command file: $JLINK_FILE"
JLinkExe -CommandFile "${PROJECT_NAME}.jlink"