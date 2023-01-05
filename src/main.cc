#include <iostream>
#include <memory.h>
#include <stdlib.h>

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

int main()
{
    solis::SImage img(1080, 1920);

    solis::algo::load_font("tests/font.ttf");
    solis::algo::set_font_pixel_size(40, 40);
    
    for(int i=0; i<img.get_height(); i+=40)
    {
        for(int j=0; j<img.get_width(); j+=40)
        {
            solis::algo::render_char(img, 'A', j, i, 255, 255, 255);
        }
    }

    img.export_to_file("test.bmp");

    return 0;
}