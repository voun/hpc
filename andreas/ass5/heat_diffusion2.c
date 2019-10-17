#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define tag 1001

//assume HEIGHT is divisble by nbr_proc
int my_id, nbr_procs, eles_per_proc, ITER, WIDTH, HEIGHT;
double dconst, mean, var;
double *glob_temp, *loc_temp_old, *loc_temp_new;

double compute_var(double mean);
double compute_avg();
void comm_with_neighs();
void simulate_heat();
void calculate_heat();
void initialize(int argc, char* argv[]);

int main(int argc, char *argv[]){

    int args[3];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_procs);

    if(my_id == 0){
        initialize(argc, argv);
        args[0] = ITER;
        args[1] = WIDTH;
        args[2] = HEIGHT;
    }

    MPI_Bcast(args, 3, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&dconst, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    ITER = args[0];
    WIDTH = args[1];
    HEIGHT = args[2];
    
    eles_per_proc = (HEIGHT/nbr_procs)*(WIDTH+2);
    loc_temp_old = calloc((HEIGHT/nbr_procs+2)*(WIDTH+2), sizeof(double));
    loc_temp_new = calloc((HEIGHT/nbr_procs+2)*(WIDTH+2), sizeof(double));

    MPI_Scatter(&glob_temp[WIDTH+2], eles_per_proc, MPI_DOUBLE, &loc_temp_old[WIDTH+2],
                eles_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    simulate_heat();

    //This isn't really needed but nice to have all temperatures stored in array glob_temp
    //This is also used as a point of synchronization
    MPI_Gather(&loc_temp_old[WIDTH+2], eles_per_proc, MPI_DOUBLE, &glob_temp[WIDTH+2],
              eles_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double avg = compute_avg();
    MPI_Allreduce(&avg, &mean, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    mean /= WIDTH*HEIGHT;

    avg = compute_var(mean);
    MPI_Reduce(&avg, &var, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_id == 0)
        var /= WIDTH*HEIGHT;
     
    if (my_id == 0)
        printf("Average temperature: %f\nVariance: %f \n", mean, var);

    MPI_Finalize();
    return 0;

}

double compute_var(double mean){

    double avg = 0;
    for(int i = 1; i < HEIGHT/nbr_procs+1; i++)
        for(int j = 1; j < WIDTH+1; j++){

            double temp = loc_temp_old[(WIDTH+2)*i+j];
            double absdiff = temp - mean >= 0 ? temp - mean : mean-temp;
            avg += absdiff;
         }

    return avg;
}

double compute_avg(){

    double avg = 0;
    for(int i = 1; i < HEIGHT/nbr_procs+1; i++)
        for(int j = 1; j < WIDTH+1; j++)
            avg += loc_temp_old[(WIDTH+2)*i+j];

    return avg;
}

void comm_with_neighs(){

    //Note how this message passing pattern avoids deadlock

    //SEND DOWN RECEIVE DOWN
    if(my_id == 0)
        MPI_Sendrecv(&loc_temp_old[(HEIGHT/nbr_procs)*(WIDTH+2)], WIDTH+2, MPI_DOUBLE, my_id+1, tag,
                    &loc_temp_old[(HEIGHT/nbr_procs+1)*(WIDTH+2)], WIDTH+2, MPI_DOUBLE, my_id+1, tag, MPI_COMM_WORLD, NULL);
    //SEND UP RECEIVE UP
    else if(my_id == nbr_procs-1)
        MPI_Sendrecv(&loc_temp_old[WIDTH+2], WIDTH+2, MPI_DOUBLE, my_id-1, tag,
                    loc_temp_old, WIDTH+2, MPI_DOUBLE, my_id-1, tag, MPI_COMM_WORLD, NULL);
    else{
        //SEND UP RECEIVE DOWN
        MPI_Sendrecv(&loc_temp_old[WIDTH+2], WIDTH+2, MPI_DOUBLE, my_id-1, tag,
                    &loc_temp_old[(HEIGHT/nbr_procs+1)*(WIDTH+2)], WIDTH+2, MPI_DOUBLE, my_id+1, tag, MPI_COMM_WORLD, NULL);
        //SEND DOWN RECEIVE UP
        MPI_Sendrecv(&loc_temp_old[(HEIGHT/nbr_procs)*(WIDTH+2)], WIDTH+2, MPI_DOUBLE, my_id+1, tag,
                    loc_temp_old, WIDTH+2, MPI_DOUBLE, my_id-1, tag, MPI_COMM_WORLD, NULL);
    }    
}

void simulate_heat(){

    for(int i = 0; i < ITER; i++){
        
        if(nbr_procs != 1)
            comm_with_neighs();

        calculate_heat();

        double *helper;
        helper = loc_temp_new;
        loc_temp_new = loc_temp_old;
        loc_temp_old = helper;              
    }
}


void calculate_heat(){

    //note that we have good caching here for both read and write
    //dont have to worry about false sharing 
    for(int i = 1; i < HEIGHT/nbr_procs+1; i++){
        for(int j = 1; j < WIDTH+1; j++){

            double me = loc_temp_old[(WIDTH+2)*i+j];                
            double up = loc_temp_old[(WIDTH+2)*(i-1)+j];
            double down = loc_temp_old[(WIDTH+2)*(i+1)+j];
            double left = loc_temp_old[(WIDTH+2)*i+(j-1)];
            double right = loc_temp_old[(WIDTH+2)*i+(j+1)];
       
            loc_temp_new[(WIDTH+2)*i+j] = me*(1-dconst)+ dconst*(up+down+left+right)/4.0;    
        } 
     }
}

void initialize(int argc, char* argv[]){

    char *arg1 = argv[1];
    char *arg2 = argv[2];
    
    char *n, *d;
    if(arg1[1] == 'n'){
        n = arg1;
        d = arg2;
    }else{
        n = arg2;
        d = arg1;
    }
  
    ITER = atoi(n+2);
    dconst = atof(d+2);

    FILE *file = fopen("test_data/diffusion_10000_10000", "r");
    //FILE *file = fopen("test.txt", "r");
    fscanf(file, "%d %d\n", &WIDTH, &HEIGHT);

    glob_temp = calloc((WIDTH+2)*(HEIGHT+2), sizeof(double));

    int x,y;
    double temp;
    while( fscanf(file, "%d %d %lf\n", &x, &y, &temp) != EOF)
        glob_temp[(WIDTH+2)*(y+1)+(x+1)] = temp;
   
    fclose(file);

}
