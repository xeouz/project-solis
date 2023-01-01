cmake_minimum_required(VERSION 3.16.3)

message(STATUS "${CMAKE_BUILD_TYPE}")

project(SOLIS VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

get_filename_component(SRC_DIR ./ ABSOLUTE)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(CXX_COMPILE_FLAGS "-I${SRC_DIR}/src -std=c17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

add_executable(SOLIS ${SRC_DIR}/src/main.cc)

link_libraries()

set(SOLIS_SRC_PATH "${SRC_DIR}/src")

include(${SOLIS_SRC_PATH}/solis-image/s_image.cmake)
include(${SOLIS_SRC_PATH}/libs/stb_image/stb_image.cmake)

add_custom_command(
    TARGET SOLIS POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/tests $<TARGET_FILE_DIR:SOLIS>/tests
)