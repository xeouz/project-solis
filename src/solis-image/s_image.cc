#include "s_image.h"
#include "libs/stb_image/stb_image.h"
#include "libs/stb_image/stb_image_write.h"

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <memory.h>

namespace solis
{

////////////////////////////////////////////////////////////
/// SColor
////////////////////////////////////////////////////////////

static std::unordered_map<const char*, int> color_hex_values({
{"red",        0xFF0000},
{"white",      0xFFFFFF},
{"cyan",       0x00FFFF},
{"silver",     0xC0C0C0},
{"blue",       0x0000FF},
{"gray",       0x808080},
{"dark-blue",  0x00008B},
{"black",      0x000000},
{"light-blue", 0xADD8E6},
{"orange",     0xFFA500},
{"purple",     0x800080},
{"brown",      0xA52A2A},
{"yellow",     0xFFFF00},
{"maroon",     0x800000},
{"lime",       0x00FF00},
{"green",      0x008000},
{"magenta",    0xFF00FF},
{"olive",      0x808000},
{"pink",       0xFFC0CB},
{"aquamarine", 0x7FFFD4},
});

std::tuple<unsigned char, unsigned char, unsigned char> hex_to_rgb(int hex)
{
    unsigned char r = ((hex >> 16) & 0xFF);
    unsigned char g = ((hex >> 8) & 0xFF);
    unsigned char b = ((hex) & 0xFF);

    return std::tuple<unsigned char, unsigned char, unsigned char>(r, g, b);
}
std::tuple<unsigned char, unsigned char, unsigned char> hex_to_rgb(const char* col_name)
{
    return hex_to_rgb(color_hex_values[col_name]);
}

SColor::SColor(unsigned char r, unsigned char g, unsigned char b): r(r), g(g), b(b)
{
}
SColor::SColor(const char* col_name)
{
    std::tuple<unsigned char, unsigned char, unsigned char> rgb = hex_to_rgb(col_name);
    r = std::get<0>(rgb); g = std::get<1>(rgb); b = std::get<2>(rgb);
}
SColor::SColor(int col_hex)
{
    std::tuple<unsigned char, unsigned char, unsigned char> rgb = hex_to_rgb(col_hex);
    r = std::get<0>(rgb); g = std::get<1>(rgb); b = std::get<2>(rgb);
}

////////////////////////////////////////////////////////////
/// SImage
////////////////////////////////////////////////////////////

SImage::SImage(unsigned int height, unsigned int width, unsigned char* image): height(height), width(width), colors_size(height * width * BYTES_PER_PIXEL)
{
    init_bitmap_pixels();
    if(image)
    {
        memcpy(colors, image, colors_size);
    }
    else
    {
        memset(colors, 0, colors_size);
    }
}
SImage::SImage(unsigned int height, unsigned int width, const char* path): height(height), width(width), colors_size(height * width * BYTES_PER_PIXEL)
{
    load_image(path);
}
SImage::SImage(const char* path): height(0), width(0), colors_size(0)
{
    load_image(path, true);
}
SImage::~SImage()
{
    delete[] colors;
}
void SImage::load_image(const char* path, bool reset_dimensions)
{
    int h, w, channels;
    colors=stbi_load(path, &w, &h, &channels, 3);

    if(colors == NULL)
    {
        std::cout << "SOLIS: File `" << path << "` could not be loaded, try using the absolute path" << std::endl;

        // Create an empty image
        init_bitmap_pixels();
        memset(colors, 0, colors_size); 
    }

    if(reset_dimensions)
    {
        height = h;
        width = w;
        colors_size = h * w * 3;
    }
}

unsigned int SImage::get_height() const
{
    return height;
}
unsigned int SImage::get_width() const
{
    return width;
}
unsigned int SImage::get_bitmap_size() const
{
    return colors_size;
}

void SImage::init_bitmap_pixels()
{
    colors = new unsigned char[colors_size];
}
void SImage::set_pixels(void* pixels)
{
    memcpy(colors, pixels, colors_size);
}
unsigned char* const SImage::get_pixels() const
{
    return colors;
}

unsigned char* const SImage::get_pixel(unsigned int x, unsigned int y) const
{
    unsigned int index=(y*width+x)*3;
    return colors+index;
}
void SImage::set_pixel(unsigned char r, unsigned char g, unsigned char b, unsigned int x, unsigned int y)
{
    unsigned int index=(y*width+x)*3;
    colors[index  ]=r;
    colors[index+1]=g;
    colors[index+2]=b;
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

unsigned char* const SImage::generate_bitmap_format() const
{
    return colors;
}
void SImage::export_to_file(const char* path) const
{
    stbi_write_jpg(path, width, height, 3, get_pixels(), 100);
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