add_library(
    stb-image
    ${SRC_DIR}/src/libs/stb_image/stb_image.h
    ${SRC_DIR}/src/libs/stb_image/stb_image_resize.h
    ${SRC_DIR}/src/libs/stb_image/stb_image_write.h
    ${SRC_DIR}/src/libs/stb_image/stb_image.cc
)

target_link_libraries(SOLIS PRIVATE stb-image)