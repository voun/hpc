#include <mpi.h>
#include <stdio.h>
#define tag 10

int main(int argc, char* argv[]){

    int token;
    int num_procs;
    int my_id; 
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(my_id != 0){ //I am not main so wait for token
        MPI_Recv(&token, 1, MPI_INT, my_id-1, tag, MPI_COMM_WORLD, &status);
        printf("Receive token: %d from process %d \n", token, my_id-1);
    }else
        token = -1;

    MPI_Send(&token, 1, MPI_INT, (my_id+1)% num_procs, tag, MPI_COMM_WORLD);
    //without below received have deadlocked!! All communication in MPI is synch
    if(my_id == 0){
        MPI_Recv(&token, 1, MPI_INT, num_procs-1, tag, MPI_COMM_WORLD, &status);
        printf("Receive token: %d from process %d \n", token, num_procs-1);
    }


    MPI_Finalize();
    return 0;



}
