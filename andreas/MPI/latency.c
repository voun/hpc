#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#define SIZE 10
#define tag 10
int main(){

    double *array = malloc(sizeof(double)*SIZE);
    int my_id, nbr_procs;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_procs);

    if(my_id == 0){
        for(int i  = 0; i < SIZE; i++)
            array[i] = i%27;

        for(int id = 1; id < nbr_procs; id++)
            MPI_Send(array, SIZE, MPI_DOUBLE, id, tag, MPI_COMM_WORLD);
    }
    else       
        MPI_Recv(array, SIZE, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, NULL);

    MPI_Finalize();
    return 0;

    
}
