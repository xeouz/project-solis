#include <iostream>
#include <memory.h>

#include "solis-image/s_image.h"

#include "libs/stb_image/stb_image.h"
#include "libs/stb_image/stb_image_write.h"

int main()
{
    int width, height, channels;
    unsigned char* image=stbi_load("tests/anakinpanakin.png", &width, &height, &channels, 3);

    if(image == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }

    std::cout << "Loaded image of height: " << height << " and width: " << width << " with " << channels << " channels" << std::endl;

    solis::SImage img(height, width);
    img.create_rgb_pattern();
    
    stbi_write_bmp("test.bmp", width, height, channels, img.get_as_bitmap());
}