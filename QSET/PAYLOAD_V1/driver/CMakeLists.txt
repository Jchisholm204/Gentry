cmake_minimum_required(VERSION 3.21)

project(QSET_USB_DRIVER)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Add additional include directories for standard libraries
include_directories(SYSTEM ${CMAKE_C_COMPILER_INCLUDE_DIRS})

# Set additional compiler flags (e.g., include paths for standard libraries)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${CMAKE_C_COMPILER_INCLUDE_DIRS}")

# Enable export of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(INCLUDE_DIRECTORIES
    ${PROJECT_DIR}/include
)

set(GENERIC_COMPILE_OPTIONS
    # -Wall
    # -Wextra
    # -Wpedantic
    # -pedantic
    # -fno-common
    )

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/lib/*.cpp
    ${PROJECT_DIR}/test/*.cpp
    ${PROJECT_DIR}/src/*.cpp
    # ${PROJECT_DIR}/src/*.cc
    )


add_executable(${EXECUTABLE}
    ${PROJECT_SOURCES}
)

target_include_directories(${EXECUTABLE} PRIVATE
    ${INCLUDE_DIRECTORIES})

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    ${GENERIC_COMPILE_OPTIONS}
    -Wdouble-promotion
    -Wformat
    -Wshadow
    # -Wconversion
    # -fstack-usage
    -g
    # -fmessage-length=0
    # -ffunction-sections
    # -fdata-sections
    # --specs=nano.specs
    #$<$<CONFIG:Debug>:-Os -g3>
    #$<$<CONFIG:Release>:-Os -g0>

    # Maximum Optimization
    # -O3
    -O0
)



target_link_libraries(${EXECUTABLE}
    # -lm
    # -lc
    # -lgcc
    # -lnosys
    # -lrdimon
    -lusb-1.0
    # -lusb
)
