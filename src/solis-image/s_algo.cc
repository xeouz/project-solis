#include "s_algo.h"

#include <iostream>
#include <memory.h>

namespace solis
{
namespace algo
{
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
}
}