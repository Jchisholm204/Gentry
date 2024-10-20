cmake_minimum_required(VERSION 3.21)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
include(./gcc-arm-none-eabi.cmake)
# set(CMAKE_SYSTEM_NAME Generic)

project(Q24VCU)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(RTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS)
set(TUSB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyusb)

set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F446xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    # FPU enable code must be written before it can be used
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    )

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/vendor/startup_stm32f446retx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/vendor/STM32F446RETX_FLASH.ld)

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
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_core/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_f4/Include
    ${PROJECT_DIR}/include
    ${RTOS_DIR}/portable/GCC/ARM_CM4F
    ${PROJECT_DIR}/include/os/config
    ${RTOS_DIR}/include
    ${TUSB_DIR}/src
)

# FreeRTOS Sources
file(GLOB RTOS_SOURCES
    ${RTOS_DIR}/*.c)

# TinyUSB sources for CDC and custom classes
set(TINYUSB_SOURCES
    ${TUSB_DIR}/src/tusb.c
    ${TUSB_DIR}/src/device/usbd.c
    ${TUSB_DIR}/src/class/cdc/cdc_device.c  # CDC class for virtual COM port
    # ${TUSB_DIR}/src/class/vendor/vendor_device.c  # Vendor-specific class for custom data
    ${TUSB_DIR}/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c  # STM32 FS device driver
)
# file(GLOB_RECURSE TINYUSB_SOURCES FOLLOW_SYMLINKS
#     ${TUSB_DIR}/*.c
# )

# Add these files to your target
# add_library(TinyUSB STATIC ${TINYUSB_SOURCES})

set(GENERIC_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -pedantic
    -fno-common
    )

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/src/*.c
)

add_executable(${EXECUTABLE}
    ${RTOS_DIR}/portable/GCC/ARM_CM4F/port.c
    ${RTOS_DIR}/portable/MemMang/heap_4.c
    ${TUSB_SOURCES}
    ${RTOS_SOURCES}
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT})

target_include_directories(${EXECUTABLE} PRIVATE
    ${INCLUDE_DIRECTORIES})

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


target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    #-nostartfiles
    ${GENERIC_COMPILE_OPTIONS}
    --specs=nano.specs
    -lgcc
    -Wl,--gc-sections
    -Wl,-L./ld
    # Printf floats can be removed in production code
    -Wl,-u_printf_float
    -Wl,-Map=firmware.map
    -Wl,--print-memory-usage
)


add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXECUTABLE}> firmware.elf
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> firmware.bin)
