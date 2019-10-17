#include <stdio.h>
#include <mpi.h>


//MPI_Bcast is a collective communicatoin, i.e it involves all processors in the communication world.
//It is also a point of synchronization meaning that every processor is blocked until everyone reaches the statement
//One data element/array is sent from a processor (e.g the root) to everyone else. Can exploit
//more efficient communication pattern so only takes O(logn) time rather than n to send to every processor.
//This is done via a tree-like pattern
int main(){

    int my_id;
    int secret_nbr;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    if(my_id == 0)
        secret_nbr = 7;

    MPI_Bcast(&secret_nbr, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Number is %d and I am %d \n", secret_nbr, my_id);

    MPI_Finalize();
    return 0;

}
