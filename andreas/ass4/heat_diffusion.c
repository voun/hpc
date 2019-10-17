#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define CL_TARGET_OPENCL_VERSION 120 //choose version 1.2 only one supported...
#include <CL/cl.h>
#include <math.h>
#include <time.h>

#define MAX_SOURCE_SIZE 100000

unsigned int ITERATIONS, WIDTH, HEIGHT;
double dconst;
double *temps_1, *temps_2, *temps;

int roundUp(int numToRound, int multiple)
{
    return ((numToRound + multiple - 1) / multiple) * multiple;
}

int main(int argc, char *argv[]){

/*-----------------------------------------------
 * READ ARGUMENTS AND INITIALIZE MATRICES */
    char *arg1 = argv[1];
    char *arg2 = argv[2];
    
    char *n, *d;
    if(arg1[1] == 'n'){
        n = arg1;
        d = arg2;
    }else{
        n = arg2;
        d = arg1;
    }

    ITERATIONS = atoi(n+2);
    dconst = atof(d+2);

    FILE *file = fopen("test_data/diffusion_10000_10000", "r");
    //FILE *file = fopen("test.txt", "r");
    fscanf(file, "%d %d\n", &WIDTH, &HEIGHT);
    temps_1 = calloc((WIDTH+2)*(HEIGHT+2), sizeof(double));
    temps_2 = calloc((WIDTH+2)*(HEIGHT+2), sizeof(double));
    temps = calloc((WIDTH+2)*(HEIGHT+2), sizeof(double));


    int x,y;
    double temp;
    while( fscanf(file, "%d %d %lf\n", &x, &y, &temp) != EOF){
        temps_1[(WIDTH+2)*(y+1)+(x+1)] = temp; // THINK ABOUT X AND Y AND COORDINATES !!
    }

    fclose(file);

/*----------------------------------------------------------
 * INITIALIZE ENVIRONMENT*/

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
    cl_kernel kernel_aux = NULL;
    

    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = malloc(sizeof(cl_platform_id)*platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);
    platform = platforms[0]; //the first one is NVIDIA which has device GPU but we want to use the other platform which has device CPU

    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    if(ret != CL_SUCCESS){
        printf("failed to create context\n");
        exit(1);
     }

    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    if(ret != CL_SUCCESS){
        printf("failed to create command queue\n");
        exit(1);
     }
   
    file = fopen("kernels.cl", "r");
    if( !file){
        printf("failed to open kernels.cl\n");
        exit(1);
    }
    char *source_str = malloc(sizeof(char)*MAX_SOURCE_SIZE);
    size_t source_size = fread(source_str, 1, sizeof(char)*MAX_SOURCE_SIZE, file);
    fclose(file);

    program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
                                       (const size_t *)&source_size, &ret);

    free(source_str);
    if(ret != CL_SUCCESS){
        printf("failed to load source code for kernels.cl\n");
        exit(1);
     }

     ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

     if(ret != CL_SUCCESS){

        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        char *log = (char *) malloc(log_size);
        
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        printf("%s\n", log);
        printf("failed to compile kernels.cl\n");
        exit(1);
     }

     kernel = clCreateKernel(program, "eqkernel", &ret);

     if(ret != CL_SUCCESS){
        printf("failed to create eqkernel\n");
     }


 /*---------------------------------------------------------
  * CREATE BUFFERS AND RUN KERNELS */

    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_WRITE,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), NULL, NULL);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_WRITE,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), NULL, NULL);
    clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), (void *)temps_1, 0, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, bufferB, CL_TRUE, 0,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), (void *)temps_2, 0, NULL, NULL);


    size_t globalws_eqk[2] = {roundUp(HEIGHT+2, 32), roundUp(WIDTH+2, 32)};
    size_t localws_eqk[2] = {32, 32}; 

    int PADDING_WIDTH = roundUp(WIDTH+2, 32) - (WIDTH+2);
    int PADDING_HEIGHT = roundUp(HEIGHT+2, 32) - (HEIGHT+2);

    clSetKernelArg(kernel, 2, sizeof(double), (void *)&dconst);
    clSetKernelArg(kernel, 3, sizeof(int), (void *)&PADDING_HEIGHT);
    clSetKernelArg(kernel, 4, sizeof(int), (void *)&PADDING_WIDTH);
    clSetKernelArg(kernel, 5, sizeof(int), (void *)&WIDTH);
    clSetKernelArg(kernel, 6, sizeof(int), (void *)&HEIGHT);


    /*------------------------------------------
     * iterate the equation*/

    for(int i = 0; i < ITERATIONS; i++){


        if(i%2 == 0){
            clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferA);
            clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferB);

            ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
                                globalws_eqk, localws_eqk, 0, NULL, NULL);
        }
        else{

            clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferB);
            clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferA);
 
            ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
                            globalws_eqk, localws_eqk, 0, NULL, NULL);
        }

        if( ret != CL_SUCCESS){
            printf("failed to queue kernel for iteration: %d\n", i);
            exit(1);
        }  

    }


    if(ITERATIONS%2 == 0){
        ret = clEnqueueReadBuffer(command_queue, bufferA, CL_TRUE, 0,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), (void *)temps_1, 0, NULL, NULL);
        temps = temps_1;
    }
    else{
        ret = clEnqueueReadBuffer(command_queue, bufferB, CL_TRUE, 0,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), (void *)temps_2, 0, NULL, NULL);
        temps = temps_2;
    }
    if( ret != CL_SUCCESS){
            printf("failed to fetch data for eqkernel");
            exit(1);
    }

    /*------------------------------
     * compute the mean temperature*/
    
    kernel = clCreateKernel(program, "avgkernel", &ret);
    if(ret != CL_SUCCESS){
        printf("failed to create avgkernel\n");
     }

    int gs = roundUp((HEIGHT+2)*(WIDTH+2),32);
    int nbr_of_groups = gs/32;
    double *res = malloc(sizeof(double)*nbr_of_groups);
    double num = 0;
    size_t globalws_avgk[1] = {gs};
    size_t localws_avgk[1] = {32};
    int PADDING = roundUp((HEIGHT+2)*(WIDTH+2),32) - (HEIGHT+2)*(WIDTH+2);  
    
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                        nbr_of_groups*sizeof(double), NULL, NULL);
    
    clEnqueueWriteBuffer(command_queue, bufferC, CL_TRUE, 0,
                        nbr_of_groups*sizeof(double), (void *)res, 0, NULL, NULL);

    clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), (void *)temps, 0, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferC);
    clSetKernelArg(kernel, 2, sizeof(double)*32, NULL);
    clSetKernelArg(kernel, 3, sizeof(int), (void *)&PADDING);

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                            globalws_avgk, localws_avgk, 0, NULL, NULL);

    if( ret != CL_SUCCESS){
        printf("failed to queue avgkernel\n");
        exit(1);
    }

    ret = clEnqueueReadBuffer(command_queue, bufferC, CL_TRUE, 0,
                        nbr_of_groups*sizeof(double), (void *)res, 0, NULL, NULL);

    if( ret != CL_SUCCESS){
        printf("failed to fetch data for avgkernel\n");
        exit(1);
    }

    //SUM UP RESULTS
    double avg = 0; 
    for(unsigned int i = 0; i < nbr_of_groups; i++)
        avg+= res[i];
    avg = avg/nbr_of_groups * gs/(WIDTH*HEIGHT);

    /*------------------------------
     * compute variance of temperatures*/

    kernel = clCreateKernel(program, "varkernel", &ret);
    if(ret != CL_SUCCESS){
        printf("failed to create varkernel\n");
     }
    
    clEnqueueWriteBuffer(command_queue, bufferC, CL_TRUE, 0,
                        nbr_of_groups*sizeof(double), (void *)res, 0, NULL, NULL);

    clEnqueueWriteBuffer(command_queue, bufferA, CL_TRUE, 0,
                        (WIDTH+2)*(HEIGHT+2)*sizeof(double), (void *)temps, 0, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufferC);
    clSetKernelArg(kernel, 2, sizeof(double)*32, NULL);
    clSetKernelArg(kernel, 3, sizeof(int), (void *)&PADDING);
    clSetKernelArg(kernel, 4, sizeof(double), (void *)&avg);

 
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                            globalws_avgk, localws_avgk, 0, NULL, NULL);

    if( ret != CL_SUCCESS){
        printf("failed to queue varkernel\n");
        exit(1);
    }

    ret = clEnqueueReadBuffer(command_queue, bufferC, CL_TRUE, 0,
                        nbr_of_groups*sizeof(double), (void *)res, 0, NULL, NULL);

    if( ret != CL_SUCCESS){
        printf("failed to fetch data for varkernel\n");
        exit(1);
    }

    //SUM UP RESULTS
    double var = 0; 
    for(unsigned int i = 0; i < nbr_of_groups; i++)
        var+= res[i];
    var = var - (2*WIDTH+2*HEIGHT+4)*avg/32;
    var = var/nbr_of_groups * gs/(WIDTH*HEIGHT);

    free(res);
    free(temps_1);
    free(temps_2);

/*---------------------------------------------
 * PRINT-RESULTS */
    printf("Average temperature is: %f\n", avg);
    printf("Variance of temperatures is: %f\n", var);

     
/*---------------------------------------------
 * CLEAN-UP */
    
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseCommandQueue(command_queue);
    clReleaseKernel(kernel);
    clReleaseKernel(kernel_aux);
    clReleaseProgram(program);
    clReleaseContext(context);

    return 0;
  
}
