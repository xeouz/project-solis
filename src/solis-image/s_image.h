#pragma once

#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define BYTES_PER_PIXEL  3

namespace solis
{

class SColor
{
public:
    unsigned char r, g, b;
public:
    SColor(unsigned char r=0, unsigned char g=0, unsigned char b=0);
};

class SImage
{
    unsigned int height, width;
    unsigned char* colors;
    unsigned int colors_size;

    unsigned char* create_file_header(const unsigned int file_header_size) const;
    unsigned char* create_info_header() const;
    void init_bitmap_pixels();
public:
    SImage(unsigned int height, unsigned int width, unsigned char* image=nullptr);
    SImage(unsigned int height, unsigned int width, const char* path="test.png");
    SImage(const char* path);
    ~SImage();
    void load_image(const char* path, bool reset_dimensions=false);

    unsigned int get_height() const;
    unsigned int get_width() const;
    unsigned int get_bitmap_size() const;

    unsigned char* const get_color(unsigned int x, unsigned int y) const;
    void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned int x, unsigned int y);

    void set_pixels(void* pixels);
    unsigned char* const get_pixels() const;
    
    unsigned char* const generate_bitmap_format() const;
    void export_to_file(const char* path) const;
};

}