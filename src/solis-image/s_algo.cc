#include "s_algo.h"

#include <iostream>
#include <unordered_map>
#include <memory.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

namespace solis
{
namespace algo
{
    static FT_Library font_library;
    static FT_Face font_face;

    void create_rgb_pattern(SImage& image)
    {
        unsigned int height=image.get_height(), width=image.get_width();
        unsigned char pattern[height][width][BYTES_PER_PIXEL];
        unsigned int i, j;

        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                pattern[i][j][0] = (unsigned char)( ((float)j / (float)width)*255 );           // R
                pattern[i][j][1] = (unsigned char)( (1.0f - ((float)j / (float)width))*255 ); // G
                pattern[i][j][2] = (unsigned char)( ((float)i / (float)height) *255 );         // B
            }
        }

        image.set_pixels(pattern);
    }
    void apply_color(SImage& image, unsigned char r, unsigned char g, unsigned char b)
    {
        unsigned int x=0, y=0;
        unsigned int h=image.get_height(), w=image.get_width();

        unsigned char pattern[h][w][BYTES_PER_PIXEL];
        memcpy(pattern, image.get_pixels(), image.get_bitmap_size());

        for(y=0; y<h; ++y)
        {
            for(x=0; x<w; x++)
            {
                unsigned char (&col)[3]=pattern[y][x];
                if(r>0)
                {
                    if(col[0]+r > 255) col[0]=255;
                    else    col[0]+=r;
                }
                if(g>0)
                {
                    if(col[1]+g > 255) col[1]=255;
                    else    col[1]+=g;
                }
                if(b>0)
                {
                    if(col[2]+b > 255) col[2]=255;
                    else    col[2]+=b;
                }
            }
        }

        image.set_pixels(pattern);
    }
    void apply_color(SImage& image, SColor const& color)
    {
        apply_color(image, color.r, color.g, color.b);
    }
    void darken(SImage& image, unsigned char darken_amount)
    {
        unsigned int lightness=256-darken_amount;
        unsigned int x=0, y=0;
        unsigned int h=image.get_height(), w=image.get_width();

        unsigned char pattern[h][w][BYTES_PER_PIXEL];
        memcpy(pattern, image.get_pixels(), image.get_bitmap_size());

        for(y=0; y<h; ++y)
        {
            for(x=0; x<w; x++)
            {
                unsigned char (&col)[3]=pattern[y][x];
                col[0]=(unsigned char)(col[0]*lightness/256);
                col[1]=(unsigned char)(col[1]*lightness/256);
                col[2]=(unsigned char)(col[2]*lightness/256);
            }
        }

        image.set_pixels(pattern);
    }
    void apply_fade_pattern(SImage& image, unsigned char r, unsigned char g, unsigned char b)
    {

    }
    void apply_fade_pattern(SImage& image, SColor const& color)
    {
        apply_fade_pattern(image, color.r, color.g, color.b);
    }

    void load_font(const char* font_path)
    {
        FT_Error error = FT_Init_FreeType( &font_library );
        if(error)
        {
            std::cout << "SOLIS: Could not initialize font library" << std::endl;
            return;
        }

        error = FT_New_Face(font_library, font_path, 0, &font_face);
        if(error == FT_Err_Unknown_File_Format)
        {
            std::cout << "SOLIS: Unknown font file format, error loading font from path: `" << font_path << "`" << std::endl;
            return;
        }
        else if(error)
        {
            std::cout << "SOLIS: Could not initilize font face, error loading font from path: `" << font_path << "`" << std::endl;
            return;
        }

        set_font_pixel_size(DEFAULT_FONT_PIXEL_SIZE, DEFAULT_FONT_PIXEL_SIZE);
    }
    void set_font_pixel_size(unsigned int font_size_h, unsigned int font_size_w)
    {
        FT_Error error= FT_Set_Pixel_Sizes(font_face, font_size_w, font_size_h);
        if(error)
        {
            std::cout << "SOLIS: Could not set font pixel size to " << font_size_h << "x" << font_size_w << ", (height x width)" << std::endl;
            return;
        }
    }
    void set_font_size(unsigned int font_size, unsigned int height, unsigned int width)
    {
        FT_Error error = FT_Set_Char_Size(font_face, 0, font_size*64, height, width);
        if(error)
        {
            std::cout << "SOLIS: Could not set font size to " << font_size << std::endl;
            return;
        }
    }
    void set_font_size(unsigned int size, SImage const& image)
    {
        set_font_size(size, image.get_height(), image.get_width());
    }
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
    {
        unsigned int height=image.get_height(), width=image.get_width();
        FT_UInt glyph_index = FT_Get_Char_Index(font_face, ch);

        FT_Error error = FT_Load_Glyph(font_face, glyph_index, FT_LOAD_RENDER);
        if(error)
        {
            std::cout << "SOLIS: Could not render glpyh for character: `" << ch << "`" << std::endl;
            return;
        }

        FT_Bitmap const& bitmap = font_face->glyph->bitmap;
        unsigned int font_height=bitmap.rows, font_width=bitmap.width;
        unsigned int i, j, p, q;
        FT_Int x_max = x+font_width, y_max = y+font_height;

        for(i=y, q=0; i<y_max; ++i, ++q)
        {
            for(j=x, p=0; j<x_max; ++j, ++p)
            {
                unsigned char pix=bitmap.buffer[q*font_width+p];

                if(pix)
                    image.set_pixel(r,g,b,j,i);
            }
        }
    }
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, SColor const& color)
    {
        render_char(image, ch, x, y, color.r, color.g, color.b);
    }
}
}