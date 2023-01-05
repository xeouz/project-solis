cmake_minimum_required(VERSION 3.16.3)

message(STATUS "${CMAKE_BUILD_TYPE}")

project(SOLIS VERSION 3.5.1)

find_package(Freetype REQUIRED)
find_package(ZLIB REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

get_filename_component(SRC_DIR ./ ABSOLUTE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CXX_COMPILE_FLAGS "-I${SRC_DIR}/src  -I/usr/local/include/freetype2 -std=c17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

add_executable(SOLIS ${SRC_DIR}/src/main.cc)
set(SOLIS_SRC_PATH "${SRC_DIR}/src")
include(${SOLIS_SRC_PATH}/solis-image/s_image.cmake)
include(${SOLIS_SRC_PATH}/libs/stb_image/stb_image.cmake)
target_link_libraries(SOLIS PRIVATE ${FREETYPE_LIBRARIES})
target_link_libraries(SOLIS PRIVATE ZLIB::ZLIB)
target_include_directories(SOLIS PRIVATE ${FREETYPE_INCLUDE_DIRS})

add_custom_command(
    TARGET SOLIS POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/tests $<TARGET_FILE_DIR:SOLIS>/tests
)