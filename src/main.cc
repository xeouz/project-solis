#include <iostream>
#include <memory.h>
#include <stdlib.h>

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

int main()
{
    solis::SImage img("tests/anakinpanakin.png");

    solis::algo::load_font("tests/font.ttf");
    solis::algo::set_font_size(1, img);
    solis::algo::create_ascii_filter(img, "ABCD");

    img.export_to_file("test.bmp");

    return 0;
}