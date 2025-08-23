set(SFX_DIR "${PROJECT_SOURCE_DIR}/core")
set(SOURCE_DIR "${SFX_DIR}/src/")
file(GLOB_RECURSE SOURCE_FILES 
    "${SOURCE_DIR}*.c"
    "${SOURCE_DIR}*.cpp"
)

add_library(sfx STATIC ${SOURCE_FILES})

target_compile_features(sfx PUBLIC cxx_std_20)

target_include_directories(sfx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # $<INSTALL_INTERFACE:include>
)

target_link_directories(sfx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(sfx
    PUBLIC
        glfw3
        freetype
        assimp
        z
        minizip
)

