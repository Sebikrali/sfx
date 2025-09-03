cmake_minimum_required(VERSION 3.30)
project(SFX_GL DESCRIPTION "Base OpenGL implementation" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${BIN_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${BIN_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/Release)

add_subdirectory(core)
add_subdirectory(demos/dev)
add_subdirectory(demos/rgb_cube)

add_compile_options(
    $<$<CONFIG:Debug>:-Og -Wall -Werror -Wpedantic>
    $<$<CONFIG:Release>:-O3 -Werror>
)
