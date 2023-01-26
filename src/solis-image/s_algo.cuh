#pragma once

#define DEFAULT_CUDA_NUM_THREADS 16
class dim3;

namespace solis
{

void wrapper_init_cuda(unsigned char* img, unsigned int height, unsigned int width);
void wrapper_realloc_pixels_gpu(unsigned int new_size, unsigned int new_width);
void wrapper_average_reduce_adjust_size(unsigned int old_size, unsigned int font_size_y,  unsigned int font_size_x);
void wrapper_blend_color_gpu(unsigned char* img, double alpha, unsigned char r, unsigned char g, unsigned char b);
void wrapper_average_reduce_gpu(unsigned char* img, const char* charset, unsigned int charset_len, unsigned int font_size_y, unsigned int font_size_x, bool use_uniform_space);
unsigned char* wrapper_finish_cuda();

unsigned int calculate_average_reduce_realloc_size(unsigned int old_size, unsigned int font_size_y,  unsigned int font_size_x);

}