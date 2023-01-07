#include <iostream>
#include <memory.h>
#include <stdlib.h>

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

int main()
{
    solis::SImage img("tests/ghost.jpg");
    solis::algo::load_font("tests/font.ttf");
    solis::algo::set_font_pixel_size(50, 50);

    img.export_to_file("test.jpg");

    return 0;
}