#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define CL_TARGET_OPENCL_VERSION 120 //choose version 1.2 only one supported...
#include <CL/cl.h>

#define MAX_SOURCE_SIZE 100000

int A[4096*4096]; //4096x4096
int B[4096*4096]; //4096x4096
int C[4096*4096]; //4096x4096
int rsA = 4096;
int csA = 4096;
int rsB = 4096;
int csB = 4096;
int rsC = 4096;
int csC = 4096;

int main(){

    for(int i = 0; i < 4096*4096; i++){
        A[i] = i%5;
        B[i] = i%27;
        C[i] = 0;
     }
    cl_uint platformCount;
    cl_platform_id *platforms;
    cl_platform_id platform;
    cl_device_id device_id;
    cl_uint ret_num_devices;
    cl_int ret;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;

    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = malloc(sizeof(cl_platform_id)*platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);
    platform = platforms[1]; //the first one is NVIDIA which has device GPU but we want to use the other platform which has device CPU

    clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device_id, &ret_num_devices);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    FILE *file = fopen("kernel.cl", "r");
    if( !file){
        printf("failed to open kernel.cl\n");
        exit(1);
    }
    char *source_str = malloc(sizeof(char)*MAX_SOURCE_SIZE);
    size_t source_size = fread(source_str, 1, sizeof(char)*MAX_SOURCE_SIZE, file);
    fclose(file);

    program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
                                       (const size_t *)&source_size, &ret);
    if(ret != CL_SUCCESS){
        printf("failed to load source code for kernel.cl\n");
        exit(1);
     }

     ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
     if(ret != CL_SUCCESS){


        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);
        
         // Get the log
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
        printf("failed to compile kernel.cl\n");
        exit(1);
     }

     kernel = clCreateKernel(program, "matmul", &ret);
     if(ret != CL_SUCCESS){
        printf("failed to create kernel\n");
     }

     cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY,
                        rsA*csA*sizeof(int), NULL, NULL);
     cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY,
                        rsB*csB*sizeof(int), NULL, NULL);
     cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                        rsC*csC*sizeof(int), NULL, NULL);
     clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0,
                        rsA*csA*sizeof(int), (void *)A, 0, NULL, NULL);
     clEnqueueWriteBuffer(command_queue, bufferB, CL_TRUE, 0,
                        rsB*csB*sizeof(int), (void *)B, 0, NULL, NULL);
     clEnqueueWriteBuffer(command_queue, bufferC, CL_TRUE, 0,
                        rsC*csC*sizeof(int), (void *)C, 0, NULL, NULL);

     clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferA);
     clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferB);
     clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufferC);
     clSetKernelArg(kernel, 3, sizeof(int), (void *)&rsA);
     clSetKernelArg(kernel, 4, sizeof(int), (void *)&csA);
     clSetKernelArg(kernel, 5, sizeof(int), (void *)&rsB);
     clSetKernelArg(kernel, 6, sizeof(int), (void *)&csB);

     size_t globalws[2] = {4096, 4096};
     size_t localws[2] = {32, 32}; //size of work group. So 3 work groups

     ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
                            globalws, localws, 0, NULL, NULL);
     if( ret != CL_SUCCESS){
        printf("failed to queue kernel\n");
        exit(1);
     }
   
    clEnqueueReadBuffer(command_queue, bufferC, CL_TRUE, 0,
                        rsC*csC*sizeof(int), (void *)C, 0, NULL, NULL);

    
    for(int i = 0; i < 100; i++)
        printf("%d ", C[2*i+4096*i]);
    
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseCommandQueue(command_queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);

    return 0;
}
