#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define SIZE 10

int main(){

    struct timespec t0,t1;

    double vals[SIZE];
    for(unsigned int i = 0; i < SIZE; i++)
        vals[i] = i;

    double *a_res = malloc(sizeof(double*)*SIZE);
    double *a_ims = malloc(sizeof(double*)*SIZE);

    timespec_get(&t0, TIME_UTC);
    for(unsigned int i = 0; i < SIZE; i++){
        a_res[i] = vals[i]*vals[i] - vals[i]*vals[i];
        a_ims[i] = vals[i]*vals[i] + vals[i]*vals[i];
        
    }
    timespec_get(&t1, TIME_UTC);

    long sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    double nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff = sec_diff + nanosec_diff;

    free(a_res);
    free(a_ims);

    printf("%f \n", diff);
    return 0;

    
}

