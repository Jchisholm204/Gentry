cmake_minimum_required(VERSION 3.20)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
include(./gcc-arm-none-eabi.cmake)
set(CMAKE_SYSTEM_NAME Generic)

project(stm32f429)

set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F429xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    # FPU enable code must be written before it can be used
    # -mfpu=fpv4-sp-d16
    # -mfloat-abi=hard
    )

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/DeviceSupport/startup_stm32f4xx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Setup/STM32F429ZI_FLASH.ld)

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/SEGGER
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreSupport
    # ${CMAKE_CURRENT_SOURCE_DIR}/DeviceSupport
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc
)

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    # ${CMAKE_SOURCE_DIR}/Application/*.c
    # ${CMAKE_SOURCE_DIR}/DeviceSupport/*.c
    ${CMAKE_SOURCE_DIR}/Setup/*.c
    ${CMAKE_SOURCE_DIR}/SEGGER/*.c
    )

add_executable(${EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/OS_StartLEDBlink.c
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT})

target_include_directories(${EXECUTABLE} PRIVATE
    ${INCLUDE_DIRECTORIES})


add_library(embOS STATIC IMPORTED GLOBAL)
set_target_properties(embOS PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../Lib/libosT7VLDP.a)
target_include_directories(embOS INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc)
target_link_libraries(${EXECUTABLE} PRIVATE embOS)

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
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
    -Wall
    --specs=nano.specs
    -lgcc
    -Wl,--gc-sections
    -Wl,-L./ld
    # Printf floats can be removed in production code
    -Wl,-u_printf_float
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
    )

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> firmware.bin)
