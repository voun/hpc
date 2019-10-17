#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

double computeavg(double *arr, int eles);

int main(){

    double *array2;
    double *array1; //only known to root process
    double *res; //only known to root process
    double avg;
    int my_id;
    int nbr_procs;
    int eles_per_proc;


    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_procs);
    eles_per_proc = 100000/nbr_procs;

    if(my_id == 0){

        array1 = malloc(sizeof(double)*100000);
        for(int i = 0; i < 100000; i++)
            array1[i] = i%27;
        
        res = malloc(sizeof(double)*nbr_procs);
    }  

    array2 = malloc(sizeof(double)*eles_per_proc); //note receive array has to have size of receive count!!   
    
    //Scatter is collective communication. Point of synch
    MPI_Scatter(array1, eles_per_proc, MPI_DOUBLE, array2, eles_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    avg = computeavg(array2, eles_per_proc);
    MPI_Gather(&avg, 1, MPI_DOUBLE, res, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //note these methods can also use more clever communication patterns

    if(my_id == 0){
        avg = computeavg(res, nbr_procs);
        printf("average is %f \n", avg);
    }

    MPI_Finalize();
    return 0;
}

double computeavg(double* restrict arr, int nbr_eles){

    double avg = 0;
    for(int i = 0; i < nbr_eles; i++)
        avg += arr[i];
    avg = avg/nbr_eles;

    return avg;
}
