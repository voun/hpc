#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(){

    int *array1;
    int *array2;
    int my_id, nbr_procs, eles_per_proc, intervals;
    double globalsum;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_procs);

    if (my_id == 0){
        intervals = 10000;
        array1 = malloc(sizeof(int)*intervals);
        for(int i = 0; i < intervals; i++)
            array1[i] = i;
    }

    MPI_Bcast(&intervals, 1, MPI_INT, 0, MPI_COMM_WORLD);
    eles_per_proc = intervals/nbr_procs;


    array2 = malloc(sizeof(int)*eles_per_proc);
    MPI_Scatter(array1, eles_per_proc, MPI_INT, array2,
                eles_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    double sum = 0;
    for(int i = 0; i < eles_per_proc; i++)
        sum +=  1/(double)(intervals)*4.0/(1+(array2[i]*array2[i] / (double)(intervals*intervals)));

    MPI_Reduce(&sum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(my_id == 0)
        printf("Pi is: %f \n", globalsum);

    MPI_Finalize();
    return 0;
              
}
