#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <chrono>

using namespace std::chrono;

#include "solis-image/s_image.h"
#include "solis-image/s_algo.h"

int matrix_rain_test()
{
    solis::SImage img(1080, 1920);
    solis::algo::load_font("tests/font.ttf");
    solis::algo::set_font_pixel_size(20, 20);

    solis::algo::init_matrix_rain(img, '0', 'Z');
    system_clock::time_point a=system_clock::now();
    for(int i=0; i<144; ++i)
    {
        solis::algo::generate_matrix_rain_frame(img);
        std::string n="out/frame";
        n+=std::to_string(i);
        n+=".jpg";
        // img.save_as(n.c_str());
    }
    system_clock::time_point b=system_clock::now();
    std::cout << "Took: " << (duration_cast<milliseconds>(b-a).count()/1000.0) << std::endl;
    solis::algo::end_matrix_rain();

    return 0;
}

int main()
{
    solis::SImage ghost("tests/ghost.jpg");
    solis::SImage chicken("tests/chicken.png");

    solis::algo::paste_image(chicken, ghost, 0, 0);
    chicken.save_as("test.jpg");
}