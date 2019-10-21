#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(){
    struct timespec t0,t1;

    long long sum = 0;
    timespec_get(&t0, TIME_UTC);
    for(unsigned int i = 0; i <= 1000000000; i++)
        sum += i;
    timespec_get(&t1, TIME_UTC);

    long sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    double nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff = sec_diff + nanosec_diff;

    printf("Time: %f and sum: %lld \n", diff, sum);
    return 0;


}
