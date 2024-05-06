# CMakeLists.txt
# Jacob Chisholm (https://jchisholm.github.io)
# 2024-03-31
#
# STM32 CMake project template for a UART Bootloader
# 

cmake_minimum_required(VERSION 3.21)

# Include the toolchain file
include(./gcc-arm-none-eabi.cmake)

# Project name
project(template)

# Set the project source directory
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Project)
# Include the FreeRTOS Kernel
set(RTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/embOS)

# Compiler options for MCU family and model
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F446xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    # FPU enable code must be written before it can be used
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    )

# Select the startup code, bootloader link file, and linker script
set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/core/startup_stm32f446retx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/core/STM32F446RETX.ld)

# Basic project settings
set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Add additional include directories for standard libraries
include_directories(SYSTEM ${CMAKE_C_COMPILER_INCLUDE_DIRS})

# Set additional compiler flags (e.g., include paths for standard libraries)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${CMAKE_C_COMPILER_INCLUDE_DIRS}")

# Enable export of compile_commands.json
# Used for clangd static analysis and makefile generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project specific include directories + FreeRTOS includes + cmsis includes
set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_core/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_f4/Include
    ${PROJECT_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
    # ${RTOS_DIR}/Inc
)

add_library(embos STATIC IMPORTED GLOBAL)

# Locate all FreeRTOS sources
file(GLOB RTOS_SOURCES
    ${RTOS_DIR}/src/*.c)

set_target_properties(embos PROPERTIES
    IMPORTED_LOCATION ${RTOS_DIR}/Lib/libosT7VLDP.a
    )
target_include_directories(embos INTERFACE
    ${RTOS_DIR}/Inc
    )


# Compiler warnings and options
set(GENERIC_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -pedantic
    -fno-common
    )

# Locate all project sources
file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/src/*.c)

# Create the executable
add_executable(${EXECUTABLE}
    # Include the FreeRTOS sources (heap + port selected seperatly)
    # ${RTOS_DIR}/portable/GCC/ARM_CM4F/port.c
    # ${RTOS_DIR}/portable/MemMang/heap_4.c
    # ${RTOS_SOURCES}
    # Include the project sources
    ${PROJECT_SOURCES}
    # Include the ASM startup code and bootloader ELF (linked in loader.asm)
    ${STARTUP_SCRIPT}
)

# Add the include directories to the Executable
target_include_directories(${EXECUTABLE} PRIVATE
    ${INCLUDE_DIRECTORIES})

target_link_libraries(${EXECUTABLE} embos)

# MCU specific compiler and linker options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    ${GENERIC_COMPILE_OPTIONS}
    -Wdouble-promotion
    -Wformat
    -Wshadow
    -Wconversion
    -fstack-usage
    -g
    -fmessage-length=0
    -ffunction-sections
    -fdata-sections
    --specs=nano.specs
    #$<$<CONFIG:Debug>:-Os -g3>
    #$<$<CONFIG:Release>:-Os -g0>
    )

# MCU specific linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}

    # -nostartfiles

    ${GENERIC_COMPILE_OPTIONS}
    --specs=nano.specs
    -lgcc
    -Wl,--gc-sections
    -Wl,-L./ld

    # Printf floats can be removed in production code
    -Wl,-u_printf_float
    # Generate a map file (useful for debugging memory placement)
    -Wl,-Map=${PROJECT_NAME}.map
    # Print memory usage (ALWAYS double check this)
    -Wl,--print-memory-usage
)

# Generate the binary and hex files after compilation
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> firmware.bin)


