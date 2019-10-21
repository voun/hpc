#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define n 1000000
#define m 1000

int main(){
    
   
    struct timespec t0,t1;
    int a=1;

    int y[n] = {0}; //all 0. Can't write int n = 1000000 
    int x[n] = {1}; //fills with 1
    int *p1 = malloc(sizeof(int)*n);
    int *p2 = malloc(sizeof(int)*n);

    
    int ix = 0;
    for (int jx=0; jx < m; ++jx) //bad locality
      for (int kx=0; kx < m; ++kx)
        p1[jx + m*kx] = ix++; //fills all entries

    
    timespec_get(&t0, TIME_UTC);
    for (int i = 0; i < n; i++) { 
      int jx = p1[i];
      y[jx] += a * x[jx];
    }
    timespec_get(&t1, TIME_UTC);

    long sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    double nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff1 = sec_diff + nanosec_diff;

    for (int ix=0; ix < n; ++ix) //good locality
        p2[ix] = ix; //fills all entries
    
    timespec_get(&t0, TIME_UTC);
    for (int i = 0; i < n; i++) { 
      int jx = p2[i];
      y[jx] += a * x[jx];
    }
    timespec_get(&t1, TIME_UTC);

    sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff2 = sec_diff + nanosec_diff;

    printf("%f %f\n", diff1, diff2);
    return 0;
    

}



