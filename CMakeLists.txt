cmake_minimum_required(VERSION 3.30)
project(SFX DESCRIPTION "Main Vulkan implementation" LANGUAGES CXX)

set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(GLOB_RECURSE source_files 
    "${source_dir}*.c"
    "${source_dir}*.cpp")

add_executable(${PROJECT_NAME} ${source_files})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    include
    ${GLFW_INCLUDE_DIRS}
)

find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# Define the link libraries
set(LIBS glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

install(
    TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug DESTINATION "debug"
)
install(
    TARGETS ${PROJECT_NAME} CONFIGURATIONS Release DESTINATION "release"
)

if(${CMAKE_BUILD_TYPE} MATCHES debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -Wall -Werror -Wpedantic -std=c++20")
elseif(${CMAKE_BUILD_TYPE} MATCHES release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Werror -std=c++20")
endif()
