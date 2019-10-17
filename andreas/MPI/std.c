#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>


int main(){

    double *array1; //only known to root
    double *array2;
    int my_id, nbr_procs, eles_per_proc;
    double globalsum;
    double globalsqdiff;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_procs);
    
    eles_per_proc = 100000/nbr_procs;

    if(my_id == 0){

        array1 = malloc(sizeof(double)*100000);
        for(int i = 0; i < 100000; i++)
            array1[i] = i%27;          
    }

    array2 = malloc(sizeof(double)*eles_per_proc);
    MPI_Scatter(array1, eles_per_proc, MPI_DOUBLE, array2, eles_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double sum = 0;
    for(int i = 0; i < eles_per_proc; i++)
        sum += array2[i];
    
    //reduces results to globalsum and then broadcasts it
    MPI_Allreduce(&sum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    double mean = globalsum/100000;
    sum = 0;
    //note every processor already has its part of array1 (in array2)
    for(int i = 0; i < eles_per_proc; i++)
        sum += (array2[i]-mean)*(array2[i]-mean);
    
    MPI_Reduce(&sum, &globalsqdiff, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(my_id == 0){
        double var = globalsqdiff/100000;
        printf("variance is %f \n", var);
    }

    MPI_Finalize();
    return 0;
}
