cmake_minimum_required(VERSION 3.30)
project(SFX_GL DESCRIPTION "Base OpenGL implementation" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")
file(GLOB_RECURSE SOURCE_FILES 
    "${SOURCE_DIR}*.c"
    "${SOURCE_DIR}*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    include
)

# Define the link libraries
target_link_directories(${PROJECT_NAME} PRIVATE "lib")
set(LINK_LIBRARIES 
    debug glfw3d optimized glfw3
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES})

install(
    TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug DESTINATION "debug"
)
install(
    TARGETS ${PROJECT_NAME} CONFIGURATIONS Release DESTINATION "release"
)

if(${CMAKE_BUILD_TYPE} MATCHES debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -Wall -Werror -Wpedantic")
elseif(${CMAKE_BUILD_TYPE} MATCHES release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Werror")
endif()
