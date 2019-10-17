
#include<stdlib.h>
#include<stdio.h>

int myfun(int* restrict A, int* restrict B);

int main(){

    int* restrict A = malloc(sizeof(int)*100);
    int* restrict B = malloc(sizeof(int)*100);
    int *C = malloc(sizeof(int)*100);

    for(int i = 0; i < 100; i++){
        A[i] = 2*i;
        B[i] = i;
    }

    myfun(A,B);
    

}

int myfun(int* restrict A, int* restrict B){
    
    for(int i = 0; i < 100;i++)
        A[i]+=B[i];

}




