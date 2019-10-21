#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define SIZE 30000


void mul_cpx_mainfile( double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im){

    *a_re = (*b_re)*(*c_re)-(*b_im)*(*c_im);
    *a_im = (*b_re)*(*c_im)+(*b_im)*(*c_re);
}


int main(){

    struct timespec t0,t1;

    double vals[SIZE];
    for(unsigned int i = 0; i < SIZE; i++)
        vals[i] = i;

    double *a_res = malloc(sizeof(double*)*SIZE);
    double *a_ims = malloc(sizeof(double*)*SIZE);
   
    timespec_get(&t0, TIME_UTC);
    for(unsigned int i = 0; i < SIZE; i++)
        mul_cpx_mainfile(&a_res[i], &a_ims[i], &vals[i], &vals[i], &vals[i], &vals[i]);
    timespec_get(&t1, TIME_UTC);

    long sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    double nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff = sec_diff + nanosec_diff;

    free(a_res);
    free(a_ims);

    printf("%f \n", diff);
    return 0;
      
}

