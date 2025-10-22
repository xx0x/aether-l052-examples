# CMakeLists.txt for STM32L052K8U3 Blink Example
cmake_minimum_required(VERSION 3.16)

# Main definitions
set(PROJECT_NAME stm32l052_blink)
set(LD_FILE stm32l052k8u3.ld)
set(DEVICE_NAME STM32L052K8U3)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PATH "")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}arm-none-eabi-g++)
set(CMAKE_LINKER ${TOOLCHAIN_PATH}arm-none-eabi-g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}arm-none-eabi-objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PATH}arm-none-eabi-size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

# Disables tests
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TARGET_FLAGS "-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections -Wno-volatile")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -g0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -g0")
endif()

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/${LD_FILE}\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs --specs=nosys.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS}")

project(${PROJECT_NAME})

enable_language(C CXX ASM)

# Define STM32L052xx for preprocessor
add_definitions(-DSTM32L052xx)
add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-DMSI_VALUE=2097000U)

# Common sources
set(COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/../../lib/cmsis-device-l0/Source/Templates/system_stm32l0xx.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_dma.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_flash.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_flash_ex.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_gpio.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_pwr.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_pwr_ex.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_rcc.c
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Src/stm32l0xx_hal_rcc_ex.c
    ${CMAKE_SOURCE_DIR}/../../lib/cmsis-device-l0/Source/Templates/gcc/startup_stm32l052xx.s
)

set(COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../../lib/CMSIS_6/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/../../lib/cmsis-device-l0/Include
    ${CMAKE_SOURCE_DIR}/../../lib/stm32l0xx-hal-driver/Inc
)

add_executable(${PROJECT_NAME}
    ${COMMON_SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    USE_HAL_DRIVER 
    STM32L052xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${COMMON_INCLUDES}
)

# Post build - Create hex and binary files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    COMMENT "Building ${PROJECT_NAME}.bin and ${PROJECT_NAME}.hex"
)

# Print build information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Device: ${DEVICE_NAME}")
message(STATUS "Linker script: ${LD_FILE}")