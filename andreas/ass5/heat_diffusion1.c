#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define send_tag 2001
#define receive_tag 2002


//RUN 100x100 -n100 -d0.01 DIFFERENT RESULTS VAR VARIANCE. TOO LARGE NUMBERS??

//assume HEIGHT is divisble by nbr_proc
unsigned int my_id, nbr_procs, eles_per_proc, ITER, WIDTH, HEIGHT;
double dconst, mean, var;
double *glob_temp_old, *glob_temp_new, *loc_temp_old, *loc_temp_new;

double compute_var(double *temp, double mean);
double compute_avg(double *temp);
void send_and_receive();
void calculate_heat(double *temp);
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
    
    eles_per_proc = (HEIGHT/nbr_procs+2)*(WIDTH+2);
    loc_temp_old = malloc(sizeof(double)*(eles_per_proc));
    loc_temp_new = malloc(sizeof(double)*(eles_per_proc));


    //cant scatter array with overlapping regions so
    //have to do it with send and receive. Since dont have 
    //so many MPI-processes this won't affect performance that much
    
    for(int i = 0; i < ITER; i++){
              
        if(my_id == 0)
            send_and_receive();
        else{
            
            MPI_Recv(loc_temp_old, eles_per_proc, MPI_DOUBLE, 0, send_tag, MPI_COMM_WORLD, NULL);
            calculate_heat(loc_temp_old);
            MPI_Send(&loc_temp_new[WIDTH+2], (HEIGHT/nbr_procs)*(WIDTH+2), MPI_DOUBLE, 0, receive_tag, MPI_COMM_WORLD);
        }
    }
    MPI_Scatter(&glob_temp_old[WIDTH+2], (HEIGHT/nbr_procs)*(WIDTH+2), MPI_DOUBLE, loc_temp_old,
                (HEIGHT/nbr_procs)*(WIDTH+2), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double avg = compute_avg(loc_temp_old);
    MPI_Allreduce(&avg, &mean, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    mean /= nbr_procs;

    avg = compute_var(loc_temp_old, mean);
    MPI_Reduce(&avg, &var, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(my_id == 0)
        var /= nbr_procs;
     
    if (my_id == 0){

        printf("Average temperature: %f\nVariance: %f \n", mean, var);

        /*
        for(int i = 0; i < HEIGHT+2; i++){
            for(int j = 0; j < WIDTH+2; j++)
                printf("%f ", glob_temp_old[(WIDTH+2)*i+j]); 
            printf("\n")/
        }*/       
    }

    MPI_Finalize();
    return 0;

}


double compute_var(double *temps, double mean){

    double avg = 0;
    for(int i = 0; i < HEIGHT/nbr_procs;i++)
        for(int j = 1; j < WIDTH+1; j++){

            double temp = temps[(WIDTH+2)*i+j];
            double absdiff = temp - mean >= 0 ? temp - mean : mean-temp;
            avg += absdiff;
         }

    return avg/(WIDTH*HEIGHT/nbr_procs);

}
double compute_avg(double *temps){

    double avg = 0;
    for(int i = 0; i < HEIGHT/nbr_procs;i++)
        for(int j = 1; j < WIDTH+1; j++)
            avg += temps[(WIDTH+2)*i+j];

    return avg/(WIDTH*HEIGHT/nbr_procs);

}

void send_and_receive(){

    for(int id = 1; id < nbr_procs; id++){

        int start = id*(HEIGHT/nbr_procs)*(WIDTH+2); 
        int end = start + eles_per_proc;    //unnecessary but makes code clearer
        int eles_to_send = end - start;

        MPI_Send(&glob_temp_old[start], eles_to_send, MPI_DOUBLE, id, send_tag, MPI_COMM_WORLD);
    }

    calculate_heat(glob_temp_old);
    for(int j = WIDTH+2; j < (HEIGHT/nbr_procs+1)*(WIDTH+2); j++) //memcpy instead?
        glob_temp_new[j] = loc_temp_new[j];

    for(int id = 1; id < nbr_procs; id++)              
        MPI_Recv(&glob_temp_new[(id*(HEIGHT/nbr_procs)+1)*(WIDTH+2)], (HEIGHT/nbr_procs)*(WIDTH+2) ,
                                MPI_DOUBLE, id, receive_tag, MPI_COMM_WORLD, NULL);
  
    double *helper;
    helper = glob_temp_new;
    glob_temp_new = glob_temp_old;
    glob_temp_old = helper;

}

void calculate_heat(double *temps){

    //note that we have good caching here for both read and write
    //dont have to worry about false sharing 
    for(int i = 1; i < HEIGHT/nbr_procs+1; i++){
        for(int j = 1; j < WIDTH+1; j++){

            double me = temps[(WIDTH+2)*i+j];                
            double up = temps[(WIDTH+2)*(i-1)+j];
            double down = temps[(WIDTH+2)*(i+1)+j];
            double left = temps[(WIDTH+2)*i+(j-1)];
            double right = temps[(WIDTH+2)*i+(j+1)];
       
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

    char *n_string = malloc(sizeof(char)*strlen(n));
    char *d_string = malloc(sizeof(char)*strlen(d));
    strncpy(n_string, n+2, strlen(n)-2);
    strncpy(d_string, d+2, strlen(d)-2);

    ITER = atoi(n_string);
    dconst = atof(d_string);

    FILE *file = fopen("test_data/diffusion_100_100", "r");
    //FILE *file = fopen("test.txt", "r");
    fscanf(file, "%d %d\n", &WIDTH, &HEIGHT);

    glob_temp_old = calloc((WIDTH+2)*(HEIGHT+2), sizeof(double));
    glob_temp_new = calloc((WIDTH+2)*(HEIGHT+2), sizeof(double));

    int x,y;
    double temp;
    while( fscanf(file, "%d %d %lf\n", &x, &y, &temp) != EOF)
        glob_temp_old[(WIDTH+2)*(y+1)+(x+1)] = temp;
   
    fclose(file);

}
