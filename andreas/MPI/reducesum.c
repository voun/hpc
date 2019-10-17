#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#define max_size 40000
#define row_size 100000

#define send_tag 2002
#define receive_tag 2001

int main(int argc, char* argv[]){

    int *array2 = malloc(sizeof(int)*max_size);
    int sum = 0;
    MPI_Status status;
    int nbr_of_rows_received;
    int my_id;
    int nbr_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_procs);

    if(my_id == 0){
        
        int rows_per_proc = row_size/nbr_procs;
        int *array1 = malloc(sizeof(int)*row_size);
        for(int i = 0; i < row_size; i++)
            array1[i] = i%27;

        for(int id = 1; id < nbr_procs; id++){

            int start = id*rows_per_proc+1;
            int end = (id+1)*rows_per_proc;

            if(row_size-end < rows_per_proc)
                end = row_size-1;
            
            int nbr_of_rows_to_send = end-start+1;

            MPI_Send(&nbr_of_rows_to_send, 1, MPI_INT, id, send_tag, MPI_COMM_WORLD);
            MPI_Send(&array1[start], nbr_of_rows_to_send, MPI_INT, id, send_tag, MPI_COMM_WORLD); 
        }

        
        for(int i = 0; i < rows_per_proc; i++)
            sum +=  array1[i];

        for(int i = 1; i < nbr_procs; i++){

            int partialsum = 0;
            MPI_Recv(&partialsum, 1, MPI_INT, MPI_ANY_SOURCE, receive_tag, MPI_COMM_WORLD, &status);  
            int sender = status.MPI_SOURCE;

            printf("Received %d from process %d \n", partialsum, sender);
            sum += partialsum;
         }

         printf("Total sum is %d \n", sum);

                
    }else{
        
        MPI_Recv(&nbr_of_rows_received, 1, MPI_INT, 0, send_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(array2, nbr_of_rows_received, MPI_INT, 0, send_tag, MPI_COMM_WORLD, &status);
        // the receive count is actually the maximum number of elements I can receive.
        // In contrast, you supply the exact number of elements to be received in MPI_Scatter

        for(int i = 0; i < nbr_of_rows_received; i++)
            sum += array2[i];
        
        MPI_Send(&sum, 1, MPI_INT, 0, receive_tag, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;

}
