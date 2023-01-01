add_library(
    solis-image
    ${SRC_DIR}/src/solis-image/s_image.h
    ${SRC_DIR}/src/solis-image/s_image.cc
)

target_link_libraries(SOLIS PRIVATE solis-image)