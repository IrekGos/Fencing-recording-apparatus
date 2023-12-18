cmake_minimum_required(VERSION 3.14)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(
    Fencing-recording-apparatus
    LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Initialize the SDK
pico_sdk_init()

include(${PROJECT_SOURCE_DIR}/lib/TM1637-pico/PicoTM1637.cmake)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(apps)

add_compile_options(-Wall
        -Wextra
        -Wno-format
        -Wno-unused-function
        )