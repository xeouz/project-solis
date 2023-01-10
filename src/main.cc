#include <iostream>
#include <memory.h>
#include <stdlib.h>

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

int main()
{
    solis::SImage img(4000, 4000);
    solis::algo::load_font("tests/font.ttf");
    solis::algo::set_font_pixel_size(50, 50); 
    solis::algo::create_rgb_pattern(img);

    for(int i=1; i<1001; ++i)
        solis::algo::blend_color(img, 0.5, 255, 0, 0);

    img.export_to_file("test.jpg");

    return 0;
}