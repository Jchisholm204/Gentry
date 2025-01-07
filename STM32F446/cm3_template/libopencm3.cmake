set(CM3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencm3)

# add_custom_target(libopencm3 make WORKING_DIRECTORY ${libopencm3_SOURCE_DIR})
# create a target to build libopencm3 -- only for the target we need
add_custom_target(libopencm3 
    make
    TARGETS=stm32/f4 
    WORKING_DIRECTORY ${CM3_DIR})

# Create a specific CPU target with the appropriate options etc
add_library(stm32f446 STATIC IMPORTED)
set_property(TARGET stm32f446 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CM3_DIR}/include)
set_property(TARGET stm32f446 PROPERTY IMPORTED_LOCATION ${CM3_DIR}/lib/libopencm3_stm32f4.a)
add_dependencies(stm32f446 libopencm3)
target_link_directories(stm32f446 INTERFACE ${CM3_DIR}/lib)

target_compile_definitions(stm32f446 INTERFACE -DSTM32F4)

set(COMPILE_OPTIONS 
  --static
  -nostartfiles
  -fno-common
  -mcpu=cortex-m4
  -mthumb
  -mfloat-abi=hard
  -mfpu=fpv4-sp-d16
)

target_compile_options(stm32f446 INTERFACE ${COMPILE_OPTIONS})
target_link_options(stm32f446 INTERFACE ${COMPILE_OPTIONS})
