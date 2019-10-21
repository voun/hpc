#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define SIZE 1000

// const int a = 3 means can't change value of a, i.e a=5
// const int *a means a points to constant. Can do e.g a = &b but can't change *a, i.e what it points to
// const double **matrix probably means matrix pointer to constant pointers.
void
row_sums(
  double *restrict sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix=0; ix < nrs; ++ix ) {
    //double sum = 0;
    for ( size_t jx=0; jx < ncs; ++jx )
        sums[ix] += matrix[ix][jx];
      //sum += matrix[ix][jx];
    //sums[ix] = sum;
  }
}


int main(){
    
    struct timespec t0,t1;
    double **as = malloc(sizeof(double*)*SIZE); 
    for(unsigned int i = 0; i < SIZE; i++)
        as[i] = malloc(sizeof(double)*SIZE);

    for(unsigned int i = 0; i < SIZE; i++)
        for(unsigned int j = 0; j < SIZE; j++)
            as[i][j] = 0.5;
    
    const double **matrix = (const double **)as;

    double *sums = malloc(sizeof(double)*SIZE);

    timespec_get(&t0, TIME_UTC);
    row_sums(sums, matrix, SIZE, SIZE);
    timespec_get(&t1, TIME_UTC);

    long sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    double nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff = sec_diff + nanosec_diff;

    printf("%f \n", diff);
    return 0; 

}
