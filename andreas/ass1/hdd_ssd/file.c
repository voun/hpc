#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(){

    struct timespec t0,t1;

    timespec_get(&t0, TIME_UTC);
    FILE *ptr = fopen("file.bin", "w+b"); //read and write binary
    for(unsigned int i = 0; i < 1048576; i++) //void är en datatyp i C som kan användas för allt
        fwrite(&i, sizeof(int), 1, ptr); //pointer till array skriver ifrån, storlek varje element, hur många åt gången, fil
    timespec_get(&t1, TIME_UTC);

    long sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    double nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff1 = sec_diff + nanosec_diff;

    fseek(ptr, 0, SEEK_SET); //takes the iterator beginning of file

    timespec_get(&t0, TIME_UTC);
    while(!feof(ptr)){ 
        int num;
        fread(&num, sizeof(int), 1, ptr);
    }  
    timespec_get(&t1, TIME_UTC);

    sec_diff = (long)t1.tv_sec-(long)t0.tv_sec;
    nanosec_diff = (t1.tv_nsec - t0.tv_nsec)/pow(10,9);
    double diff2 = sec_diff + nanosec_diff;
    
    fclose(ptr);
    printf("%f %f \n", diff1, diff2);
    return 0;

}
