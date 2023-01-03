#pragma once

#include "s_image.h"

namespace solis
{

// This namespace contains several algorithms to modify an image
// `create_xxx_pattern` means to remove original information and replace it with that pattern
// `apply_xxx_pattern` means to apply a certain pattern on the existing image
namespace algo
{
    // Color pattern algorithms
    void create_rgb_pattern(SImage& image);
    void apply_color(SImage& image, unsigned char rstep, unsigned char gstep, unsigned char bstep);
    void apply_color(SImage& image, SColor const& color);
    void darken(SImage& image, unsigned char darken_amount);
    void apply_fade_pattern(SImage& image, unsigned char r, unsigned char g, unsigned char b);
    void apply_fade_pattern(SImage& image, SColor const& color);

    // Font algorithms
    void something(SImage& image);
}
}