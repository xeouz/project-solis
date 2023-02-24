#include "s_algo.h"
#include "s_algo.cuh"

#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <functional>
#include <memory.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_LCD_FILTER_H

#define MAX(a, b) a>b?a:b
#define MIN(a, b) a>b?b:a

namespace solis
{
namespace algo
{
    static FT_Library font_library;
    static FT_Face font_face;
    static std::unordered_map<char, std::pair<FT_Bitmap, FT_Glyph_Metrics>> font_bitmaps;
    static std::pair<unsigned int, unsigned long> font_max_y;
    static std::pair<unsigned int, unsigned long> font_max_x;

    ///-- Basic Rendering --///
    void create_rgb_pattern(SImage& image)
    {
        unsigned int height=image.get_height(), width=image.get_width();
        unsigned char* pattern = image.get_pixels();
        unsigned int i, j, idx;

        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                idx = (i*width+j)*BYTES_PER_PIXEL;
                pattern[idx  ] = (unsigned char)( ((float)j / (float)width)*255 );           // R
                pattern[idx+1] = (unsigned char)( (1.0f - ((float)j / (float)width))*255 ); // G
                pattern[idx+2] = (unsigned char)( ((float)i / (float)height) *255 );         // B
            }
        }
    }
    void apply_color(SImage& image, unsigned char r, unsigned char g, unsigned char b)
    {
        unsigned int x=0, y=0;
        unsigned int h=image.get_height(), w=image.get_width();

        unsigned char pattern[h][w][BYTES_PER_PIXEL];
        memcpy(pattern, image.get_pixels(), image.get_pixels_size());

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
    void blend_color(SImage& image, double alpha, unsigned char r, unsigned char g, unsigned char b)
    {
        double one_minus_alpha = (1 - alpha);
        unsigned int size = image.get_height() * image.get_width();
        unsigned char* pixels = image.get_pixels();
        unsigned char* pixel;

        for(unsigned int i=0; i<size; ++i)
        {
            pixel = pixels+(i*BYTES_PER_PIXEL);
            pixel[0] = (alpha * r) + (one_minus_alpha * pixel[0]);
            pixel[1] = (alpha * r) + (one_minus_alpha * pixel[1]);
            pixel[2] = (alpha * r) + (one_minus_alpha * pixel[2]);
        }
    }
    void blend_color(SImage& image, double alpha, SColor const& color)
    {
        blend_color(image, alpha, color.r, color.g, color.b);
    }
    void darken(SImage& image, unsigned int darken_amount)
    {
        unsigned int lightness=100-darken_amount;
        unsigned int size = image.get_height() * image.get_width();
        unsigned char* pixels = image.get_pixels();
        unsigned char* pixel;

        for(unsigned int i=0; i<size; ++i)
        {
            pixel = pixels+(i*BYTES_PER_PIXEL);
            pixel[0] = (unsigned char)(pixel[0]*lightness/100);
            pixel[1] = (unsigned char)(pixel[1]*lightness/100);
            pixel[2] = (unsigned char)(pixel[2]*lightness/100);
        }
    }
    void apply_fade_pattern(SImage& image, unsigned char r, unsigned char g, unsigned char b)
    {

    }
    void apply_fade_pattern(SImage& image, SColor const& color)
    {
        apply_fade_pattern(image, color.r, color.g, color.b);
    }
    //--- Basic Rendering ---//

    ///-- Font and Character Rendering --///
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
    void render_font_glyph(char ch) // Private method
    {
        FT_UInt glyph_index = FT_Get_Char_Index(font_face, ch);
        FT_Error error = FT_Load_Glyph(font_face, glyph_index, FT_LOAD_DEFAULT);
        if(error)
        {
            std::cout << "SOLIS: Could not LOAD glpyh for character: `" << ch << "`" << std::endl;
            return;
        }

        error = FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_NORMAL);
        if(error)
        {
            std::cout << "SOLIS: Could not RENDER glpyh for character: `" << ch << "`" << std::endl;
            return;
        }

        FT_Pos height = font_face->glyph->metrics.height;
        FT_Pos width = font_face->glyph->metrics.width;
        if(font_max_y.second < height)
        {
            font_max_y.second = height;
            font_max_y.first = height/64;
        }
        if(font_max_x.second < width)
        {
            font_max_x.second = width;
            font_max_x.first = width/64;
        }
    }
    void prerender_font_glyphs(const char* charset, unsigned int charset_len)
    {
        for(unsigned int i=0; i<charset_len; ++i)
        {
            char ch = charset[i];
            if(font_bitmaps.count(ch)) continue;
            render_font_glyph(ch);
            
            FT_Bitmap_Copy(font_library, &font_face->glyph->bitmap, &font_bitmaps[ch].first);
            font_bitmaps[ch].second = font_face->glyph->metrics;
        }
    }
    void prerender_font_glyphs(std::string const& charset)
    {
        prerender_font_glyphs(charset.c_str(), charset.size());
    }
    void prerender_font_glyphs(char start, char end)
    {
        char* st=(char*)malloc(end-start+2);
        int j=0;
        for(int i=start; i<=end; ++i)
            st[j++] = i;
        st[j] = '\0';
        prerender_font_glyphs(st, end-start+1);
        free(st);
    }
    void prerender_font_glyphs()
    {
        prerender_font_glyphs(DEFAULT_FONT_PRERENDER_CHARSET);
    }
    void set_font_pixel_size(unsigned int font_size_h, unsigned int font_size_w)
    {
        FT_Error error = FT_Set_Pixel_Sizes(font_face, font_size_w, font_size_h);
        if(error)
        {
            std::cout << "SOLIS: Could not set font pixel size to " << font_size_h << "x" << font_size_w << ", (height x width)" << std::endl;
            return;
        }
    }
    void set_font_size(double font_size, unsigned int height, unsigned int width)
    {
        FT_Error error = FT_Set_Char_Size(font_face, 0, font_size*64, height, width);
        if(error)
        {
            std::cout << "SOLIS: Could not set font size to " << font_size << std::endl;
            return;
        }
    }
    void set_font_size(double size, SImage const& image)
    {
        set_font_size(size, image.get_height(), image.get_width());
    }
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, bool use_y_offset)
    {
        FT_Bitmap& bitmap = font_face->glyph->bitmap;
        FT_Glyph_Metrics& metrics = font_face->glyph->metrics;
        if(!font_bitmaps.count(ch)) // Glyph is not yet rendered
        {
            std::cout << "Not rendered: " << ch << std::endl;
            render_font_glyph(ch);
            FT_Bitmap_Copy(font_library, &font_face->glyph->bitmap, &font_bitmaps[ch].first);
            font_bitmaps[ch].second = font_face->glyph->metrics;
            bitmap = font_face->glyph->bitmap;
            metrics = font_face->glyph->metrics;
        }
        else
        {
            bitmap = font_bitmaps[ch].first;
            metrics = font_bitmaps[ch].second;
        }

        unsigned int font_height = bitmap.rows, font_width = bitmap.width;
        unsigned int i, j, p, q;
        FT_Int x_max = x+font_width, y_max = y+font_height;

        if(use_y_offset)
        {
            const unsigned int y_offset = (font_max_y.first) - bitmap.rows;
            i = y_offset + y;
            y_max += y_offset;
        }
        else
        {
            i = y;
        }

        if(x_max > image.get_width()) x_max = image.get_width();
        if(y_max > image.get_height()) y_max = image.get_height();

        for(q=0; i<y_max; ++i, ++q)
        {
            for(j=x, p=0; j<x_max; ++j, ++p)
            {
                unsigned char pix=bitmap.buffer[q*font_width+p];

                if(pix)
                {
                    image.set_pixel(r, g, b, j, i);
                }
            }
        }
    }
    void render_char(SImage& image, char ch, unsigned int x, unsigned int y, SColor const& color, bool use_y_offset)
    {
        render_char(image, ch, x, y, color.r, color.g, color.b, use_y_offset);
    }
    void render_str(SImage& image, const char* str, unsigned int len, unsigned int whitespace_width, unsigned int spacing_px)
    {
        unsigned int x=0, y_offset=0, i=0;
        for(i=0; i<len && x<image.get_width(); ++i)
        {
            if(str[i] == ' ')
            {
                x += whitespace_width;
                continue;
            }
            render_char(image, str[i], x, 0, 255, 0, 0, true);
            x += font_face->glyph->metrics.width/64 + spacing_px;
        }
    }
    void render_str(SImage& image, std::string const& str, unsigned int whitespace_width, unsigned int spacing_px)
    {
        render_str(image, str.c_str(), str.size());
    }
    //--- Font and Character Rendering ---//

    void create_ascii_filter(SImage& image, const char* charset, unsigned int charset_len)
    {
        FT_Library_SetLcdFilter(font_library, FT_LCD_FILTER_DEFAULT);
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distrib(0, charset_len-1);
        auto random_fn = std::bind(distrib, generator);

        prerender_font_glyphs(charset, charset_len); // Prerender font glyphs since we need to use font_max_y

        unsigned int block_size_xy = font_max_y.first;
        unsigned int height=image.get_height(), width=image.get_width();

        unsigned char pattern[height][width][BYTES_PER_PIXEL];
        memcpy(pattern, image.get_pixels(), image.get_pixels_size());
        memset(image.get_pixels(), 0, image.get_pixels_size());

        unsigned int i, j;
        unsigned int x, y, sum_r, sum_g, sum_b, block_size_xy_sq=block_size_xy*block_size_xy;
        unsigned int avg_r, avg_g, avg_b, jlim, ilim;
        for(i = 0; i<height; i+=block_size_xy)
        {
            for(j = 0; j<width; j+=block_size_xy)
            {
                // Get the average color of the current block of size block_size_xy*block_size_xy
                sum_r = sum_g = sum_b = 0;
                ilim = i+block_size_xy;
                jlim = j+block_size_xy;

                if((ilim > height) || (jlim > width))
                    continue;
                
                for(y=i; y<ilim; ++y)
                {
                    for(x=j; x<jlim; ++x)
                    {
                        unsigned char* indx = pattern[y][x];
                        sum_r += indx[0]; sum_g += indx[1]; sum_b += indx[2];
                    }
                }

                avg_r = sum_r/block_size_xy_sq;
                avg_g = sum_g/block_size_xy_sq;
                avg_b = sum_b/block_size_xy_sq;

                render_char(image, charset[random_fn()], j, i, avg_r, avg_g, avg_b, true);
            }
        }
    }
    void create_ascii_filter(SImage& image, std::string const& charset)
    {
        create_ascii_filter(image, charset.c_str(), charset.size());
    }

    ///-- Matrix Rain --///
    unsigned int* matrix_generation_drops; // X-Coordinate = Index && Y-Coordinate = Value 
    unsigned int num_cols; 
    unsigned int col_height;
    char ascii_range_a, ascii_range_b;

    template<typename Integral>
    Integral randint(Integral min, Integral max)
    {
        using param_type =
            typename std::uniform_int_distribution<Integral>::param_type;

        // only create these once (per thread)
        thread_local static std::mt19937 eng {std::random_device{}()};
        thread_local static std::uniform_int_distribution<Integral> dist;

        // presumably a param_type is cheaper than a uniform_int_distribution
        return dist(eng, param_type{min, max});
    }
    void init_matrix_rain(SImage& img, char ascii_range_start, char ascii_range_end)
    {
        prerender_font_glyphs(ascii_range_start, ascii_range_end);
        num_cols = (img.get_width() / font_max_x.first) + (img.get_width() % font_max_x.first);
        col_height = img.get_height() / font_max_y.first;
        matrix_generation_drops = (unsigned int*)malloc(num_cols*sizeof(unsigned int));

        ascii_range_a = ascii_range_start;
        ascii_range_b = ascii_range_end;
        
        for(unsigned int i=0; i<num_cols; ++i)
            matrix_generation_drops[i] = 0;

    }
    void generate_matrix_rain_frame(SImage& frame)
    {
        unsigned int i;
        
        for(i=0; i<num_cols; ++i)
        {
            render_char(frame, randint<int>(ascii_range_a, ascii_range_b), i*font_max_x.first, matrix_generation_drops[i]*font_max_y.first, 10, 255, 10);

            if(matrix_generation_drops[i] > col_height && randint<int>(0, 1000) > 975)
                matrix_generation_drops[i] = 0;
            else
                matrix_generation_drops[i]++;
        }

        darken(frame, 5);
    }
    void end_matrix_rain()
    {
        free(matrix_generation_drops);
        num_cols = col_height = 0;
    }
    //--- Matrix Rain ---//

    ///-- CUDA Functions --///
    void init_cuda(SImage& image)
    {
        wrapper_init_cuda(image.get_pixels(), image.get_height(), image.get_width());
    }
    void finish_cuda(SImage& image)
    {
        image.set_pixels(wrapper_finish_cuda());
    }
    void blend_color_gpu(SImage& image, double alpha, unsigned char r, unsigned char g, unsigned char b)
    {
        wrapper_blend_color_gpu(alpha, r, g, b);
    }
    void blend_color_gpu(SImage& image, double alpha, SColor const& color)
    {
        blend_color_gpu(image, alpha, color.r, color.g, color.b);
    }
    void create_ascii_filter_gpu(SImage& image, const char* charset, unsigned int charset_len)
    {
        prerender_font_glyphs(charset, charset_len);

        unsigned int height=image.get_height(), width=image.get_width();
        wrapper_average_reduce_adjust_size(image.get_pixels_size(), font_max_y.first, font_max_x.first);
        wrapper_average_reduce_gpu(charset, charset_len, font_max_y.first, font_max_x.first);

        FT_Library_SetLcdFilter(font_library, FT_LCD_FILTER_DEFAULT);
        unsigned char* pixels=wrapper_finish_cuda();
        unsigned int i, j, avg_r, avg_g, avg_b, rand_num, idx;
        unsigned int ysize=font_max_y.first, xsize=font_max_x.first;

        for(i=0; i<height; i+=ysize)
        {
            for(j=0; j<width; j+=xsize)
            {
                idx = (i*width+j)*BYTES_PER_PIXEL;
                if((i+ysize)>height || (j+xsize)>width)
                {
                    pixels[idx] = 0; pixels[idx+1] = 0; pixels[idx+2] = 0;
                    continue;
                }

                avg_r = pixels[idx]; avg_g = pixels[idx+1]; avg_b = pixels[idx+2], rand_num = pixels[idx+3];
                render_char(image, charset[rand_num], j, i, avg_r, avg_g, avg_b, true);
                pixels[idx] = 0; pixels[idx+1] = 0; pixels[idx+2] = 0;
            }
        }

        wrapper_init_cuda(pixels, height, width);
    }
    void create_ascii_filter_gpu(SImage& image, std::string const& charset)
    {
        create_ascii_filter_gpu(image, charset.c_str(), charset.size());
    }
    //--- CUDA Functions ---//
}
}