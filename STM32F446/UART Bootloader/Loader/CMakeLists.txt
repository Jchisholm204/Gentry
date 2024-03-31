cmake_minimum_required(VERSION 3.21)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
#include(../gcc-arm-none-eabi.cmake)
# set(CMAKE_SYSTEM_NAME Generic)

project(LOADER)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Project)

set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F446xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    # FPU enable code must be written before it can be used
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    )

set(START_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f446retx.s)
set(MCU_LINK_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F446RETX.ld)

set(LOADER_EXECUTABLE "LOADER")
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Add additional include directories for standard libraries
include_directories(SYSTEM ${CMAKE_C_COMPILER_INCLUDE_DIRS})

# Set additional compiler flags (e.g., include paths for standard libraries)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${CMAKE_C_COMPILER_INCLUDE_DIRS}")

# Enable export of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_core/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_f4/Include
    ${PROJECT_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
)

set(GENERIC_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -pedantic
    -fno-common
    )

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/src/*.c)

add_executable(${LOADER_EXECUTABLE}
    ${PROJECT_SOURCES}
    ${START_SCRIPT})

target_include_directories(${LOADER_EXECUTABLE} PRIVATE
    ${INCLUDE_DIRECTORIES})

target_compile_options(${LOADER_EXECUTABLE} PRIVATE
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

target_link_options(${LOADER_EXECUTABLE} PRIVATE
    -T${MCU_LINK_SCRIPT}
    ${CPU_PARAMETERS}
    #-nostartfiles
    ${GENERIC_COMPILE_OPTIONS}
    --specs=nano.specs
    -lgcc
    -Wl,--gc-sections
    -Wl,-L./ld
    # Printf floats can be removed in production code
    -Wl,-u_printf_float
    -Wl,-Map=${PROJECT_NAME}.map
    # -Wl,--print-memory-usage
    )

add_custom_command(TARGET ${LOADER_EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${LOADER_EXECUTABLE}> ${LOADER_EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${LOADER_EXECUTABLE}> ${LOADER_EXECUTABLE}.bin)

# Install the generated binary file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LOADER_EXECUTABLE}.bin DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)