#include <iostream>
#include <memory.h>
#include <stdlib.h>

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

int main()
{
    solis::SImage img(1080, 1980);
    solis::algo::load_font("tests/font.ttf");
    solis::algo::set_font_pixel_size(20, 20); 
    solis::algo::create_rgb_pattern(img);

    solis::algo::init_cuda(img);
    solis::algo::create_ascii_filter_gpu(img, "AB");

    img.export_to_file("test.jpg");

    return 0;
}