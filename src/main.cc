#include <iostream>
#include <memory.h>
#include <stdlib.h>

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

int main()
{
    solis::SImage img(1080, 1920);

    solis::algo::load_font("tests/font.ttf");
    solis::algo::set_font_pixel_size(10, 10);
    solis::algo::prerender_font_glyphs();
    
    for(int i=0; i<img.get_height(); i+=10)
    {
        for(int j=0; j<img.get_width(); j+=10)
        {
            solis::algo::render_char(img, 'A', j, i, 255, 0, 0);
        }
    }

    img.export_to_file("test.bmp");

    return 0;
}