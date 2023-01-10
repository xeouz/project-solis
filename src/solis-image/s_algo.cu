#include "s_algo.cuh"
#include "s_image.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
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

namespace solis
{
    typedef struct
    {
        unsigned char pix[3];
    } Pixel;

    static unsigned char* in_pixels;
    static unsigned char* in_gpu_pixels;
    static unsigned int pixels_size;
    static dim3 grid;
    static dim3 threads_per_block;

    void wrapper_init_cuda(unsigned char* img, unsigned int height, unsigned int width)
    {
        pixels_size = height*width*BYTES_PER_PIXEL;
        
        // Copy image
        in_pixels = (unsigned char*)malloc(pixels_size);
        memcpy(in_pixels, img, pixels_size);

        // Init CUDA
        cudaFree(0);

        cudaMalloc((void**) &in_gpu_pixels, pixels_size);
        cudaMemcpy(in_gpu_pixels, in_pixels, pixels_size, cudaMemcpyHostToDevice);
        
        constexpr const int threads = 16;
        unsigned int num_blocks_w = width/threads + (bool)(width%threads);
        unsigned int num_blocks_h = height/threads + (bool)(height%threads);

        threads_per_block = dim3(threads, threads);
        grid = dim3(num_blocks_w, num_blocks_h);
    }
    void wrapper_blend_color_gpu(unsigned char* pixels, unsigned int height, unsigned int width, double alpha, unsigned char r, unsigned char g, unsigned char b)
    {
        double one_minus_alpha = (1 - alpha);

        internal_blend_color_gpu<<<grid, threads_per_block>>>(in_gpu_pixels, width, one_minus_alpha, alpha, r, g, b);
        gpuErrchk(cudaPeekAtLastError());
        gpuErrchk(cudaDeviceSynchronize());
    }
    unsigned char* wrapper_finish_cuda(unsigned int height, unsigned int width)
    {
        cudaMemcpy(in_pixels, in_gpu_pixels, pixels_size, cudaMemcpyDeviceToHost);
        cudaFree(in_gpu_pixels);

        return in_pixels;
    }

}