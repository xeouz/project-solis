#include "s_algo.cuh"
#include "s_image.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <curand.h>
#include <curand_kernel.h>

#include <iostream>
#include <memory.h>

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

#ifdef __INTELLISENSE__
    void __syncthreads();
    void __cudaPushCallConfiguration(dim3 gridDim, dim3 blockDim, size_t shared_mem=0, void* stream = 0);
#endif

__global__ void internal_blend_color_gpu(unsigned char* in_pixels, int width, double one_minus_alpha, double alpha, unsigned char r, unsigned char g, unsigned char b)
{
    int i = (blockIdx.y * blockDim.y + threadIdx.y);
    int j = (blockIdx.x * blockDim.x + threadIdx.x);

    int idx = (i*width+j)*BYTES_PER_PIXEL;
    in_pixels[idx  ] = ((alpha * r) + (one_minus_alpha * in_pixels[idx  ]));
    in_pixels[idx+1] = ((alpha * g) + (one_minus_alpha * in_pixels[idx+1]));
    in_pixels[idx+2] = ((alpha * b) + (one_minus_alpha * in_pixels[idx+2]));
}

__global__ void internal_curand_init(curandState* current_state, int num_blocks_w, unsigned long seed)
{
    int i = (blockIdx.y * blockDim.y + threadIdx.y);
    int j = (blockIdx.x * blockDim.x + threadIdx.x);
    int idx = i*num_blocks_w+j;
    curand_init(seed, idx, 0, &current_state[idx]);
}

__global__ void internal_average_reduce_gpu(unsigned char* in_pixels, int charset_len, int font_size_y, int font_size_x, int width, int num_blocks_w, curandState* state)
{
    int i = (blockIdx.y * blockDim.y + threadIdx.y);
    int j = (blockIdx.x * blockDim.x + threadIdx.x);
    int rand_idx = i*num_blocks_w+j;
    int rand_num = ((int)(curand_uniform(&state[rand_idx]) * 1000)) % charset_len;
    i *= font_size_y;
    j *= font_size_x;

    int block_size_sq = font_size_y*font_size_x;
    int x, y, y_max=i+font_size_y, x_max=j+font_size_x, idx;
    int sum_r=0, sum_g=0, sum_b=0, avg_r, avg_g, avg_b;
    for(y=i; y<y_max; ++y)
    {
        for(x=j; x<x_max; ++x)
        {
            idx = (y*width+x)*BYTES_PER_PIXEL;
            sum_r += in_pixels[idx  ];
            sum_g += in_pixels[idx+1];
            sum_b += in_pixels[idx+2];

            in_pixels[idx  ] = 0;
            in_pixels[idx+1] = 0;
            in_pixels[idx+2] = 0;
        }
    }

    avg_r = sum_r/block_size_sq;
    avg_g = sum_g/block_size_sq;
    avg_b = sum_b/block_size_sq;

    idx = (i*width+j)*BYTES_PER_PIXEL;
    in_pixels[idx  ] = avg_r;
    in_pixels[idx+1] = avg_g;
    in_pixels[idx+2] = avg_b;
    in_pixels[idx+3] = rand_num;
}

namespace solis
{
    typedef struct
    {
        unsigned char pix[3];
    } Pixel;

    static unsigned char* in_pixels;
    static unsigned char* in_gpu_pixels;
    static unsigned int pixels_size;
    static unsigned int height;
    static unsigned int width;

    static unsigned int resized_height;
    static unsigned int resized_width;
    static unsigned int resized_size;

    static curandState* rand_state;

    unsigned int calculate_average_reduce_realloc_size(unsigned int old_size, unsigned int font_size_y, unsigned int font_size_x)
    {
        constexpr const int threads = DEFAULT_CUDA_NUM_THREADS;
        unsigned int advance_x = (threads*font_size_x), advance_y = (threads*font_size_y);
        unsigned int num_blocks_w = width/advance_x + (bool)(width%advance_x);
        unsigned int num_blocks_h = height/advance_y + (bool)(height%advance_y);

        resized_width = num_blocks_w * advance_x;
        resized_height = num_blocks_h * advance_y;

        resized_size = (resized_height*resized_width + (font_size_y*font_size_x)) * BYTES_PER_PIXEL;
        return resized_size;
    }

    void wrapper_init_cuda(unsigned char* img, unsigned int h, unsigned int w)
    {   
        resized_size = 0;
        resized_height = height = h; resized_width = width = w;
        resized_size = pixels_size = height*width*BYTES_PER_PIXEL;
        
        // Set image
        in_pixels = img;

        // Init CUDA
        cudaFree(0);
        cudaMalloc((void**) &in_gpu_pixels, pixels_size);
        cudaMemcpy(in_gpu_pixels, in_pixels, pixels_size, cudaMemcpyHostToDevice);
    }
    void wrapper_realloc_pixels_gpu(unsigned int new_size, unsigned int new_width)
    {
        unsigned char* buffer=(unsigned char*)malloc(new_size);
        memset(buffer, 0, new_size);
        unsigned int i, j;
        for(i=0; i<height; ++i)
        {
            for(j=0; j<width; ++j)
            {
                unsigned int idx1=(i*width+j)*BYTES_PER_PIXEL, idx2=(i*new_width+j)*BYTES_PER_PIXEL;
                buffer[idx2] = in_pixels[idx1];
                buffer[idx2+1] = in_pixels[idx1+1];
                buffer[idx2+2] = in_pixels[idx1+2];
            }
        }

        cudaFree(in_gpu_pixels);
        cudaMalloc((void**) &in_gpu_pixels, new_size);
        cudaMemcpy(in_gpu_pixels, buffer, new_size, cudaMemcpyHostToDevice);
        resized_size = new_size;
    }
    void wrapper_average_reduce_adjust_size(unsigned int old_size, unsigned int font_size_y,  unsigned int font_size_x)
    {
        unsigned int size=calculate_average_reduce_realloc_size(old_size, font_size_y, font_size_x);
        wrapper_realloc_pixels_gpu(size, resized_width);
    }
    void wrapper_blend_color_gpu(double alpha, unsigned char r, unsigned char g, unsigned char b)
    {
        constexpr const int threads = DEFAULT_CUDA_NUM_THREADS;
        unsigned int num_blocks_w = resized_width/threads + (bool)(resized_width%threads);
        unsigned int num_blocks_h = resized_height/threads + (bool)(resized_height%threads);
        dim3 threads_per_block = dim3(threads, threads);
        dim3 grid = dim3(num_blocks_w, num_blocks_h);
        double one_minus_alpha = (1 - alpha);

        internal_blend_color_gpu<<<grid, threads_per_block>>>(in_gpu_pixels, resized_width, one_minus_alpha, alpha, r, g, b);
    }
    void wrapper_average_reduce_gpu(const char* charset, unsigned int charset_len, unsigned int font_size_y, unsigned int font_size_x)
    {
        constexpr const int threads = DEFAULT_CUDA_NUM_THREADS;
        unsigned int advance_x = (threads*font_size_x), advance_y = (threads*font_size_y);
        unsigned int num_blocks_w = resized_width/advance_x + (bool)(resized_width%advance_x);
        unsigned int num_blocks_h = resized_height/advance_y + (bool)(resized_height%advance_y);
        dim3 threads_per_block = dim3(threads, threads);
        dim3 grid = dim3(num_blocks_w, num_blocks_h);

        unsigned int charset_size = sizeof(char) * charset_len;

        cudaMalloc(&rand_state, num_blocks_h * num_blocks_w * threads*threads * sizeof(curandState));
        internal_curand_init<<<grid, threads_per_block>>>(rand_state, num_blocks_w, 1);

        char* charset_gpu;
        cudaMalloc(&charset_gpu, charset_size);
        cudaMemcpy(charset_gpu, charset, charset_size, cudaMemcpyHostToDevice);
        internal_average_reduce_gpu<<<grid, threads_per_block>>>(in_gpu_pixels, charset_len, font_size_y, font_size_x, resized_width, num_blocks_w, rand_state);
        cudaFree(&charset_gpu);
    }
    unsigned char* wrapper_finish_cuda()
    {
        if(false)
            cudaMemcpy(in_pixels, in_gpu_pixels, pixels_size, cudaMemcpyDeviceToHost);
        else
        {
            unsigned char* buffer=(unsigned char*)malloc(resized_size), *gpu_pix, *res_pix;
            unsigned int i, j;
            cudaMemcpy(buffer, in_gpu_pixels, resized_size, cudaMemcpyDeviceToHost);

            for(i=0; i<height; ++i)
            {
                for(j=0; j<width; ++j)
                {
                    gpu_pix = buffer+((i*resized_width+j)*BYTES_PER_PIXEL); res_pix = in_pixels+((i*width+j)*BYTES_PER_PIXEL);
                    res_pix[0] = gpu_pix[0];
                    res_pix[1] = gpu_pix[1];
                    res_pix[2] = gpu_pix[2];                    
                }
            }
            free(buffer);
        }
        cudaFree(in_gpu_pixels);

        return in_pixels;
    }

}