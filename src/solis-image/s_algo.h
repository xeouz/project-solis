#pragma once

#include "s_image.h"

#define DEFAULT_FONT_PIXEL_SIZE 64

namespace solis
{

// This namespace contains several algorithms to modify an image
// `create_xxx_pattern` means to remove original information and replace it with that pattern
// `apply_xxx_pattern` means to apply a certain pattern on the existing image
namespace algo
{
    // Color pattern algorithms
    void create_rgb_pattern(SImage& image);
    void apply_color(SImage& image, unsigned char rstep=1, unsigned char gstep=1, unsigned char bstep=1);
    void apply_color(SImage& image, SColor const& color);
    void darken(SImage& image, unsigned char darken_amount);
    void apply_fade_pattern(SImage& image, unsigned char r=0, unsigned char g=0, unsigned char b=0);
    void apply_fade_pattern(SImage& image, SColor const& color);

    // Font algorithms
    void load_font(const char* font_path);
    void set_font_pixel_size(unsigned int font_size_h, unsigned int font_size_w);
    void set_font_size(unsigned int font_size, unsigned int height, unsigned int width);
    void set_font_size(unsigned int size, SImage const& image);
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, unsigned char r=0, unsigned char g=0, unsigned char b=0);
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, SColor const& color);
}
}