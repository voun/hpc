#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define CL_TARGET_OPENCL_VERSION 120 //choose version 1.2 only one supported...
#include <CL/cl.h>

#define MAX_SOURCE_SIZE 100000


int main(){


    int as = 536870912;
    int *A = malloc(sizeof(int)*as);
    int *B = malloc(sizeof(int)*as/32);

    for(int i = 0; i < as/32; i++)
        B[i] = 0;
    for(int i = 0; i < as; i++)
        A[i] = i%5;
        
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
    platform = platforms[0]; //the first one is NVIDIA which has device GPU but we want to use the other platform which has device CPU

    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    
    FILE *file = fopen("reductionsum.cl", "r");
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

        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        char *log = (char *) malloc(log_size);
        
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        printf("%s\n", log);
        printf("failed to compile kernel.cl\n");
        exit(1);
     }

     kernel = clCreateKernel(program, "reductionkernel", &ret);
     if(ret != CL_SUCCESS){
        printf("failed to create kernel\n");
     }

     cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY,
                        as*sizeof(int), NULL, NULL);
     cl_mem bufferB = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                        as/32*sizeof(int), NULL, NULL);
     clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0,
                        as*sizeof(int), (void *)A, 0, NULL, NULL);
     clEnqueueWriteBuffer(command_queue, bufferB, CL_TRUE, 0,
                        as/32*sizeof(int), (void *)B, 0, NULL, NULL);

     clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferA);
     clSetKernelArg(kernel, 1, sizeof(int)*32, NULL);
     clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufferB);
 
     size_t globalws[1] = {as};
     size_t localws[1] = {32}; 

     ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                            globalws, localws, 0, NULL, NULL);
     if( ret != CL_SUCCESS){
        printf("failed to queue kernel\n");
        exit(1);
     }
   
    clEnqueueReadBuffer(command_queue, bufferB, CL_TRUE, 0,
                        as/32*sizeof(int), (void *)B, 0, NULL, NULL);

    long sum = 0;
    for(int i = 0; i < as/32; i++)
        sum += B[i];
    printf("%d \n", sum);
    
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseCommandQueue(command_queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);

        
    printf("\t-------------------------\n");
    cl_int err;
    cl_char string[10240] = {0}; 
    // Get device name
    err = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(string), &string, NULL);
    printf("\t\tName: %s\n", string);

    // Get device OpenCL version
    err = clGetDeviceInfo(device_id, CL_DEVICE_OPENCL_C_VERSION, sizeof(string), &string, NULL);
    printf("\t\tVersion: %s\n", string);

    // Get Max. Compute units
    cl_uint num;
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num, NULL);
    printf("\t\tMax. Compute Units: %d\n", num);

    // Get local memory size
    cl_ulong mem_size;
    err = clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
    printf("\t\tLocal Memory Size: %llu KB\n", mem_size/1024);

    // Get global memory size
    err = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &mem_size, NULL);
    printf("\t\tGlobal Memory Size: %llu MB\n", mem_size/(1024*1024));

    // Get maximum buffer alloc. size
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &mem_size, NULL);
    printf("\t\tMax Alloc Size: %llu MB\n", mem_size/(1024*1024));

    // Get work-group size information
    size_t size;
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &size, NULL);
    printf("\t\tMax Work-group Total Size: %ld\n", size);

    // Find the maximum dimensions of the work-groups
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &num, NULL);
    // Get the max. dimensions of the work-groups
    size_t dims[num];
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(dims), &dims, NULL);
    printf("\t\tMax Work-group Dims: ( ");
    for (size_t k = 0; k < num; k++)
    {
        printf("%ld ", dims[k]);
    }
    printf(")\n");

    printf("\t-------------------------\n");

    return 0;
}
