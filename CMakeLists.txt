cmake_minimum_required(VERSION 3.28)
include(cmake/toolchain.cmake)

project(gd32e23x_template)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)
set(VERSION "V${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
string(TIMESTAMP CURRENT_DATE "%Y-%m-%d")

# Options 1
set(OPT1 "")
#set(OPT1 "_NONE")

# Options 2
set(OPT2 "")
#set(OPT2 "_NONE")

enable_language(C)
enable_language(CXX)
enable_language(ASM)

# Use custom startup.S
set(TARGET_STARTUP_ASM ${CMAKE_SOURCE_DIR}/startup/startup_gd32e23x.S)
# Use custom linker script
set(TARGET_LD_SCRIPT ${CMAKE_SOURCE_DIR}/ld/gd32e23x_gcc.ld)
# Add GD SDK
add_subdirectory(sdk)

set(TARGET_C_SRC
        ${CMAKE_SOURCE_DIR}/src/main.c
        ${CMAKE_SOURCE_DIR}/src/gd32e23x_it.c
        ${CMAKE_SOURCE_DIR}/src/systick.c
        ${CMAKE_SOURCE_DIR}/src/led.c
        ${CMAKE_SOURCE_DIR}/src/usart.c
        ${CMAKE_SOURCE_DIR}/src/soft_i2c.c
        ${CMAKE_SOURCE_DIR}/src/i2c.c
        ${CMAKE_SOURCE_DIR}/src/fwdgt.c
)

add_executable(gd32e23x_template ${TARGET_C_SRC})

target_link_libraries(gd32e23x_template GD32E23X_SDK)
target_include_directories(gd32e23x_template PUBLIC inc)

# Generate .bin and .hex
generate_binary_file(gd32e23x_template)
generate_hex_file(gd32e23x_template)
