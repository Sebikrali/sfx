set(TARGET_NAME shadow_maps)

set(DEV_DIR "${PROJECT_SOURCE_DIR}/demos/${TARGET_NAME}")
set(SOURCE_DIR "${DEV_DIR}/")
file(GLOB_RECURSE SOURCE_FILES 
    "${SOURCE_DIR}*.c"
    "${SOURCE_DIR}*.cpp"
)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PRIVATE sfx)

target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CONFIG:Debug>:-Og -Wall -Werror -Wpedantic>
    $<$<CONFIG:Release>:-O3 -Werror>
)

