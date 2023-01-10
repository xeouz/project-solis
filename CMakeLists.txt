cmake_minimum_required(VERSION 3.25.1-g7aed5f7)

message(STATUS "${CMAKE_BUILD_TYPE}")

project(SOLIS VERSION 1)
enable_language(CUDA)

find_package(Freetype REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CUDA  REQUIRED)
include_directories("${CUDA_INCLUDE_DIRS}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

get_filename_component(SRC_DIR ./ ABSOLUTE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CXX_COMPILE_FLAGS "-I${SRC_DIR}/src  -I/usr/local/include/freetype2 -Wno-unused-command-line-argument -diag-suppress815-D")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

add_executable(SOLIS ${SRC_DIR}/src/main.cc)
set_target_properties(SOLIS PROPERTIES CUDA_ARCHITECTURES "75")
set_target_properties(SOLIS PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set(SOLIS_SRC_PATH "${SRC_DIR}/src")
include(${SOLIS_SRC_PATH}/solis-image/s_image.cmake)
include(${SOLIS_SRC_PATH}/libs/stb_image/stb_image.cmake)
target_link_libraries(SOLIS PRIVATE ${FREETYPE_LIBRARIES})
target_link_libraries(SOLIS PRIVATE ZLIB::ZLIB)
target_link_libraries(SOLIS PRIVATE pthread z)
target_include_directories(SOLIS PRIVATE ${FREETYPE_INCLUDE_DIRS})

add_custom_command(
    TARGET SOLIS POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/tests $<TARGET_FILE_DIR:SOLIS>/tests
)