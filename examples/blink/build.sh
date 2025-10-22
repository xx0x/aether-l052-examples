#!/bin/bash

# Build script for STM32L052 Blink Example
# This script configures and builds the project using CMake

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}STM32L052K8U3 Blink Example Build Script${NC}"
echo "=========================================="

# Check if arm-none-eabi-gcc is available
if ! command -v arm-none-eabi-gcc &> /dev/null; then
    echo -e "${RED}Error: arm-none-eabi-gcc not found in PATH${NC}"
    echo "Please install the ARM GCC toolchain:"
    echo "  macOS: brew install arm-none-eabi-gcc"
    echo "  Ubuntu: sudo apt-get install gcc-arm-none-eabi"
    exit 1
fi

# Display toolchain info
echo -e "${YELLOW}Toolchain Information:${NC}"
arm-none-eabi-gcc --version | head -1
echo ""

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning existing build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo -e "${YELLOW}Configuring project with CMake...${NC}"
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build the project
echo -e "${YELLOW}Building project...${NC}"
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Check if build was successful
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ Build completed successfully!${NC}"
    echo ""
    echo -e "${YELLOW}Generated files:${NC}"
    ls -la stm32l052_blink.*
    echo ""
    echo -e "${YELLOW}Flash the firmware with:${NC}"
    echo "  st-flash write stm32l052_blink.bin 0x8000000"
    echo "  or"
    echo "  openocd -f interface/stlink.cfg -f target/stm32l0.cfg -c \"program stm32l052_blink.elf verify reset exit\""
else
    echo -e "${RED}✗ Build failed!${NC}"
    exit 1
fi