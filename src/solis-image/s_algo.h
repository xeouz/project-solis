#pragma once

#include "s_image.h"
#include "uchar.h"
#include <string>
#include <vector>

#define DEFAULT_FONT_PIXEL_SIZE 64
#define DEFAULT_FONT_SPACING 10
#define DEFAULT_FONT_USE_Y_OFFSET false
#define DEFAULT_FONT_WHITESACE_WIDTH 20
#define DEFAULT_FONT_PRERENDER_CHARSET ",.?/;:abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()[]{}-+*=\\"

namespace solis
{

// This namespace contains several algorithms to modify an image
// `create_xxx_pattern` means to remove original information and replace it with that pattern
// `apply_xxx_pattern` means to apply a certain pattern on the existing image
namespace algo
{
    // Image modifying algorithms
    void create_rgb_pattern(SImage& image);
    void apply_color(SImage& image, unsigned char rstep=1, unsigned char gstep=1, unsigned char bstep=1);
    void apply_color(SImage& image, SColor const& color);
    void blend_color(SImage& image, double alpha, unsigned char r, unsigned char g, unsigned char b);
    void blend_color(SImage& image, double alpha, SColor const& color);
    void darken(SImage& image, unsigned int darken_amount);
    void blend_image(SImage& base_image, SImage& secondary_image, double alpha);
    void paste_image(SImage& base_image, SImage& secondary_image, unsigned int x, unsigned int y);

    // Font algorithms
    void load_font(const char* font_path);
    void prerender_font_glyphs(const char* charset, unsigned int charset_len);
    void prerender_font_glyphs(std::string const& charset);
    void prerender_font_glyphs(char start, char end);
    void prerender_font_glyphs();
    void set_font_pixel_size(unsigned int font_size_h, unsigned int font_size_w);
    void set_font_size(double font_size, unsigned int height, unsigned int width);
    void set_font_size(double size, SImage const& image);
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, unsigned char r=0, unsigned char g=0, unsigned char b=0, bool use_y_offset=DEFAULT_FONT_USE_Y_OFFSET);
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, SColor const& color, bool use_y_offset=DEFAULT_FONT_USE_Y_OFFSET);
    void render_str(SImage& image, const char* str, unsigned int len, unsigned int whitespace_width=DEFAULT_FONT_WHITESACE_WIDTH, unsigned int spacing_px=DEFAULT_FONT_SPACING);
    void render_str(SImage& image, std::string const& str, unsigned int whitespace_width=DEFAULT_FONT_WHITESACE_WIDTH, unsigned int spacing_px=DEFAULT_FONT_SPACING);

    // Goofy algorithms
    void create_ascii_filter(SImage& image, const char* charset, unsigned int charset_len);
    void create_ascii_filter(SImage& image, std::string const& charset);

    // Matrix Rain
    void init_matrix_rain(SImage& image, char ascii_range_start='A', char ascii_range_end='Z');
    void generate_matrix_rain_frame(SImage& frame);
    void end_matrix_rain();

    // Internal GPU based algorithms
    void init_cuda(SImage& image);
    void finish_cuda(SImage& image);
    void blend_color_gpu(SImage& image, double alpha, unsigned char r, unsigned char g, unsigned char b);
    void blend_color_gpu(SImage& image, double alpha, SColor const& color);
    void create_ascii_filter_gpu(SImage& image, const char* charset, unsigned int charset_len);
    void create_ascii_filter_gpu(SImage& image, std::string const& charset);
}
}