#include "s_image.h"

#include <iostream>
#include <fstream>
#include <memory.h>

#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define BYTES_PER_PIXEL  3

namespace solis
{

////////////////////////////////////////////////////////////
/// SImage
////////////////////////////////////////////////////////////

SImage::SImage(unsigned int height, unsigned int width, unsigned char* image): height(height), width(width)
{
    colors_size = height * width * BYTES_PER_PIXEL;

    init_bitmap_colors();
    if(image)
    {
        memcpy(colors, image, colors_size);
    }
    else
    {
        memset(colors, 0, colors_size);
    }
}
SImage::~SImage()
{
    delete[] colors;
}

void SImage::init_bitmap_colors()
{
    colors = new unsigned char[colors_size];
}
void SImage::set_pattern(void* pattern)
{
    memcpy(colors, pattern, colors_size);
}
void SImage::create_rgb_pattern()
{
    unsigned char pattern[height][width][BYTES_PER_PIXEL];
    unsigned int i, j;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            pattern[i][j][0] = (unsigned char)( ((float)j / (float)width)*255 );           // R
            pattern[i][j][1] = (unsigned char)( (1.0f - ((float)j / (float)width))*255 ); // G
            pattern[i][j][2] = (unsigned char)( ((float)i / (float)height) *255 );         // B
        }
    }

    memcpy(colors, pattern, colors_size);
}

unsigned char* const SImage::get_color(unsigned int x, unsigned int y) const
{
    unsigned int index=y*width+x;
    return colors+index+3;
}
void SImage::set_color(unsigned char* color, unsigned int x, unsigned int y)
{
    unsigned int index=(y*width+x);
    colors[index+2]=color[0];
    colors[index+1]=color[1];
    colors[index  ]=color[2];
}

unsigned char* SImage::create_file_header(const unsigned int file_size) const
{
    //- Creating the file header -//
    static unsigned char file_header[FILE_HEADER_SIZE] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    // Define the file type
    file_header[0] = 'B';
    file_header[1] = 'M';

    // Define the file size (split up into 4 chars)
    file_header[2] = file_size;
    file_header[3] = file_size >> 8;
    file_header[4] = file_size >> 16;
    file_header[5] = file_size >> 24;

    // Pixel data offset
    file_header[10] = FILE_HEADER_SIZE + INFO_HEADER_SIZE; // Never exceeds 255, no need for bit shifting

    return file_header;
}
unsigned char* SImage::create_info_header() const
{
    //- Creating the information header -//
    static unsigned char info_header[INFO_HEADER_SIZE] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    // Define the header size
    info_header[0] = INFO_HEADER_SIZE; // Never exceeds 255, no need for bit shifting
    // Define the image width
    info_header[4] = width;
    info_header[5] = width >> 8;
    info_header[6] = width >> 16;
    info_header[7] = width >> 24;

    // Define the image height
    info_header[8] = height;
    info_header[9] = height >> 8;
    info_header[10] = height >> 16;
    info_header[11] = height >> 24;

    // Define the planes
    info_header[12] = 1;

    // Define the bits per pixel (RGB, 24 = 8 + 8 + 8)
    info_header[14] = 24;

    return info_header;
}

unsigned char* const SImage::get_as_bitmap() const
{
    return colors;
}

/*
void SImage::export_to_file(const char* path) const
{
    FILE* file=std::fopen(path, "wb");
    unsigned char padding[3] = {0, 0, 0};

    // Create the padding to add to the bmp file
    const unsigned int padding_amt = ((4 - (width * 3) % 4) % 4);
    const unsigned int file_size = FILE_HEADER_SIZE + INFO_HEADER_SIZE + colors_size + (padding_amt * height);

    unsigned char* file_header = create_file_header(file_size);
    unsigned char* info_header = create_info_header();

    std::fwrite(file_header, 1, FILE_HEADER_SIZE, file);
    std::fwrite(info_header, 1, INFO_HEADER_SIZE, file);
    
    int i;
    unsigned wb=(width*BYTES_PER_PIXEL);
    for (i = height; i > 0; --i) {
        std::fwrite(colors + (i*wb), BYTES_PER_PIXEL, width, file);
        std::fwrite(padding, 1, padding_amt, file);
    }
    
    std::fclose(file);

    std::cout << "Successfully wrote to `" << path << "`" << std::endl;
}
*/

}