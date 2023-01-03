#include <iostream>
#include <memory.h>

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

#include "libs/stb_image/stb_image.h"
#include "libs/stb_image/stb_image_write.h"

int main()
{
    solis::SImage img("tests/anakinpanakin.png");

    solis::algo::apply_color(img, 100, 0, 0);
    solis::algo::darken(img, 50);

    img.export_to_file("test.bmp");

    return 0;
}