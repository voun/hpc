#include <stdio.h>
#include <mpi.h>

int main(int argc, char * argv[]){ //every process exists here already, it runs main
    int ierr, num_procs, my_id; //everyone has copies of this memory (but stupid if every process has copy of same large matrix...)
                                //not how it is supposed to work
    //printf("bajs\n");
    ierr = MPI_Init(&argc, &argv); //initializes message passing environment between processes
                                  //mpirun -n 5 helloworld arg1 arg2 arg3
                                  // then MPI_Init(..) makes argc and argv act as normal, i.e
                                  // like had only called ./helloworld arg1 arg2 arg3. Otherwise can't know their positions  
    printf("%d \n", argc);

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    if( my_id == 0)
        printf("serial work\n");

    printf("Hello %d and %d \n", num_procs, my_id);

    ierr = MPI_Finalize(); //destroys message passing environment between processes
    return 0;             //should only have return after this statement!i


}
