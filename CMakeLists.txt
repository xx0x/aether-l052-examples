# CMakeLists.txt for STM32L052K8 Examples
cmake_minimum_required(VERSION 3.16)

# Shared device configuration
set(DEVICE_NAME STM32L052K8)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
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

# Function to setup linker flags for a given target and linker script
function(setup_linker_flags target_name ld_file)
    set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
    set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/${ld_file}\"")
    set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs --specs=nosys.specs")
    set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${target_name}.map -Wl,--gc-sections")
    set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")
    
    set_target_properties(${target_name} PROPERTIES LINK_FLAGS ${CMAKE_C_LINK_FLAGS})
endfunction()

# Function to generate J-Link script file
function(generate_jlink_script target_name)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND echo "device ${DEVICE_NAME}" > ${target_name}.jlink
        COMMAND echo "si SWD" >> ${target_name}.jlink
        COMMAND echo "speed 4000" >> ${target_name}.jlink
        COMMAND echo "connect" >> ${target_name}.jlink
        COMMAND echo "halt" >> ${target_name}.jlink
        COMMAND echo "loadfile ${target_name}.hex" >> ${target_name}.jlink
        COMMAND echo "r" >> ${target_name}.jlink
        COMMAND echo "g" >> ${target_name}.jlink
        COMMAND echo "exit" >> ${target_name}.jlink
        COMMENT "Generating J-Link script for ${target_name}"
    )
endfunction()

project(stm32l052_examples)

enable_language(C CXX ASM)

# Define STM32L052xx for preprocessor
add_definitions(-DSTM32L052xx)
add_definitions(-DUSE_HAL_DRIVER)
add_definitions(-DMSI_VALUE=2097000U)

# Paths to core libraries
set(HAL_DRIVER_PATH ${CMAKE_SOURCE_DIR}/lib/stm32l0xx-hal-driver)
set(CMSIS_6_PATH ${CMAKE_SOURCE_DIR}/lib/CMSIS_6)
set(CMSIS_DEVICE_L0_PATH ${CMAKE_SOURCE_DIR}/lib/cmsis-device-l0)

# Common sources shared by all examples
set(COMMON_SOURCES_BASE
    ${CMSIS_DEVICE_L0_PATH}/Source/Templates/system_stm32l0xx.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_cortex.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_dma.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_flash.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_flash_ex.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_gpio.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_pwr.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_pwr_ex.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_rcc.c
    ${HAL_DRIVER_PATH}/Src/stm32l0xx_hal_rcc_ex.c
    ${CMSIS_DEVICE_L0_PATH}/Source/Templates/gcc/startup_stm32l052xx.s
)

# Common includes shared by all examples
set(COMMON_INCLUDES_BASE
    ${CMSIS_6_PATH}/CMSIS/Core/Include
    ${CMSIS_DEVICE_L0_PATH}/Include
    ${HAL_DRIVER_PATH}/Inc
)

# Function to create example targets
function(create_example_targets example_name)
    set(EXAMPLE_DIR ${CMAKE_SOURCE_DIR}/examples/${example_name})
    
    # Check if example directory exists
    if(NOT EXISTS ${EXAMPLE_DIR})
        message(WARNING "Example directory ${EXAMPLE_DIR} does not exist")
        return()
    endif()
    
    # Check if example has its own example.cmake
    if(NOT EXISTS ${EXAMPLE_DIR}/example.cmake)
        message(WARNING "Example ${example_name} missing example.cmake")
        return()
    endif()
    
    # Include the example's example.cmake to get its configuration
    include(${EXAMPLE_DIR}/example.cmake)
    
    # Convert relative paths in EXAMPLE_SOURCES to absolute paths
    set(ABSOLUTE_EXAMPLE_SOURCES)
    foreach(SOURCE ${EXAMPLE_SOURCES})
        if(IS_ABSOLUTE ${SOURCE})
            list(APPEND ABSOLUTE_EXAMPLE_SOURCES ${SOURCE})
        else()
            list(APPEND ABSOLUTE_EXAMPLE_SOURCES ${EXAMPLE_DIR}/${SOURCE})
        endif()
    endforeach()
    
    # Combine common sources with example-specific sources
    set(ALL_SOURCES 
        ${COMMON_SOURCES_BASE} 
        ${ABSOLUTE_EXAMPLE_SOURCES}
        ${EXAMPLE_HAL_SOURCES}
    )
    
    # Setup includes (example directory + common includes + example-specific includes)
    set(ALL_INCLUDES ${COMMON_INCLUDES_BASE})
    foreach(INCLUDE ${EXAMPLE_INCLUDES})
        if(IS_ABSOLUTE ${INCLUDE})
            list(APPEND ALL_INCLUDES ${INCLUDE})
        else()
            list(APPEND ALL_INCLUDES ${EXAMPLE_DIR}/${INCLUDE})
        endif()
    endforeach()
    
    # Create target for normal version (without bootloader)
    set(TARGET_NAME_NORMAL "${example_name}")
    add_executable(${TARGET_NAME_NORMAL} ${ALL_SOURCES})
    
    target_compile_definitions(${TARGET_NAME_NORMAL} PRIVATE
        USE_HAL_DRIVER 
        STM32L052xx
        ${EXAMPLE_DEFINITIONS}
        $<$<CONFIG:Debug>:DEBUG>
    )
    
    target_include_directories(${TARGET_NAME_NORMAL} PRIVATE ${ALL_INCLUDES})
    
    setup_linker_flags(${TARGET_NAME_NORMAL} "ld/stm32l052k8.ld")
    
    # Create target for bootloader version
    set(TARGET_NAME_BOOTLOADER "${example_name}_bootloader")
    add_executable(${TARGET_NAME_BOOTLOADER} ${ALL_SOURCES})
    
    target_compile_definitions(${TARGET_NAME_BOOTLOADER} PRIVATE
        USE_HAL_DRIVER 
        STM32L052xx
        BOOTLOADER_BUILD
        ${EXAMPLE_DEFINITIONS}
        $<$<CONFIG:Debug>:DEBUG>
    )
    
    target_include_directories(${TARGET_NAME_BOOTLOADER} PRIVATE ${ALL_INCLUDES})
    
    setup_linker_flags(${TARGET_NAME_BOOTLOADER} "ld/stm32l052k8_bootloader.ld")
    
    # Post build commands for normal version
    add_custom_command(TARGET ${TARGET_NAME_NORMAL} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_NAME_NORMAL}.elf ${TARGET_NAME_NORMAL}.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${TARGET_NAME_NORMAL}.elf ${TARGET_NAME_NORMAL}.hex
        COMMAND ${CMAKE_SIZE} ${TARGET_NAME_NORMAL}.elf
        COMMENT "Building ${TARGET_NAME_NORMAL}.bin and ${TARGET_NAME_NORMAL}.hex"
    )
    
    # Generate J-Link script for normal version
    generate_jlink_script(${TARGET_NAME_NORMAL})
    
    # Post build commands for bootloader version
    add_custom_command(TARGET ${TARGET_NAME_BOOTLOADER} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_NAME_BOOTLOADER}.elf ${TARGET_NAME_BOOTLOADER}.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${TARGET_NAME_BOOTLOADER}.elf ${TARGET_NAME_BOOTLOADER}.hex
        COMMAND ${CMAKE_SIZE} ${TARGET_NAME_BOOTLOADER}.elf
        COMMENT "Building ${TARGET_NAME_BOOTLOADER}.bin and ${TARGET_NAME_BOOTLOADER}.hex"
    )
    
    # Generate J-Link script for bootloader version
    generate_jlink_script(${TARGET_NAME_BOOTLOADER})
    
    # Create flash targets
    add_custom_target(flash-${example_name}
        COMMAND JLinkExe -CommanderScript ${TARGET_NAME_NORMAL}.jlink
        DEPENDS ${TARGET_NAME_NORMAL}
        COMMENT "Flashing ${example_name} (normal version)"
    )
    
    add_custom_target(flash-${example_name}_bootloader
        COMMAND JLinkExe -CommanderScript ${TARGET_NAME_BOOTLOADER}.jlink
        DEPENDS ${TARGET_NAME_BOOTLOADER}
        COMMENT "Flashing ${example_name} (bootloader version)"
    )
    
    # Add targets to global lists
    list(APPEND ALL_TARGETS ${TARGET_NAME_NORMAL} ${TARGET_NAME_BOOTLOADER})
    set(ALL_TARGETS ${ALL_TARGETS} PARENT_SCOPE)
    
    message(STATUS "Created targets for example: ${example_name}")
    message(STATUS "  - ${TARGET_NAME_NORMAL}")
    message(STATUS "  - ${TARGET_NAME_BOOTLOADER}")
endfunction()

# Find all example directories and configure all of them
file(GLOB EXAMPLE_DIRS "${CMAKE_SOURCE_DIR}/examples/*")
set(ALL_TARGETS)

message(STATUS "Configuring all examples...")
foreach(EXAMPLE_DIR ${EXAMPLE_DIRS})
    if(IS_DIRECTORY ${EXAMPLE_DIR})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_DIR} NAME)
        create_example_targets(${EXAMPLE_NAME})
    endif()
endforeach()

# Create a target to build all examples
if(ALL_TARGETS)
    add_custom_target(all_examples ALL DEPENDS ${ALL_TARGETS})
    message(STATUS "Created 'all_examples' target")
endif()

# Print build information
message(STATUS "Device: ${DEVICE_NAME}")
message(STATUS "Available linker scripts:")
message(STATUS "  - ld/stm32l052k8.ld (normal)")
message(STATUS "  - ld/stm32l052k8_bootloader.ld (bootloader)")
message(STATUS "")
message(STATUS "Usage:")
message(STATUS "  Configure once: cmake ..")
message(STATUS "  Build all examples: make")
message(STATUS "  Build specific example: make <example_name>")
message(STATUS "  Build specific bootloader: make <example_name>_bootloader")
message(STATUS "  Flash example: make flash-<example_name>")
message(STATUS "  Available examples:")
foreach(EXAMPLE_DIR ${EXAMPLE_DIRS}) 
    if(IS_DIRECTORY ${EXAMPLE_DIR})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_DIR} NAME)
        message(STATUS "    - ${EXAMPLE_NAME} (targets: ${EXAMPLE_NAME}, ${EXAMPLE_NAME}_bootloader)")
    endif()
endforeach()