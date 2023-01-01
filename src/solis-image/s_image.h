#pragma once

namespace solis
{

class SImage
{
    unsigned int height, width;
    unsigned char* colors;
    unsigned int colors_size;

    unsigned char* create_file_header(const unsigned int file_header_size) const;
    unsigned char* create_info_header() const;
    void init_bitmap_colors();
public:
    SImage(unsigned int height, unsigned int width, unsigned char* image=nullptr);
    ~SImage();

    unsigned char* const get_color(unsigned int x, unsigned int y) const;
    void set_color(unsigned char* rgb, unsigned int x, unsigned int y);

    void set_pattern(void* pattern);
    void create_rgb_pattern();
    
    // void export_to_file(const char* path) const;

    unsigned char* const get_as_bitmap() const;
};

}