#pragma once

class dim3;

namespace solis
{

void wrapper_init_cuda(unsigned char* img, unsigned int height, unsigned int width);
void wrapper_blend_color_gpu(unsigned char* img, unsigned int height, unsigned int width, double alpha, unsigned char r, unsigned char g, unsigned char b);
unsigned char* wrapper_finish_cuda(unsigned int height, unsigned int width);

}