#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <complex.h>
#include <unistd.h>
#include <math.h>
#define PI 3.14159265358979323846


// NEED TO LINK BOTH -lpthread AND -lm
const double TOL = 0.001;
unsigned short int ** attractors; //every pixel contains which attractor to
unsigned int ** convergences;
unsigned short int DEGREE, N_THREADS;
unsigned int LINES;
unsigned short int *done; //is this line done???
double complex ** matrix; // note double pointer is easier to use if want 2d array (but single could also work)
double complex *roots;
//matrix with pixels
//and done array
//something with colors here


void *newton_fun(void *arg);
void *write_fun(void *arg);

int main(int argc, char *argv[]){

    char *x = argv[1];
    char *y = argv[2];
    
    char *t, *l;
    if(x[1] == 't'){
        t = x;
        l = y;
    } else {
        t = y;
        l = x;
    }

    char *t_string = malloc(sizeof(char)*(strlen(t)-2));
    char *l_string = malloc(sizeof(char)*(strlen(l)-2));
    strncpy(t_string, t + 2, strlen(t)-2);
    strncpy(l_string, l + 2, strlen(l)-2);

    N_THREADS = atoi(t_string);
    LINES = atoi(l_string);
    DEGREE = atoi(argv[3]);
    free(t_string); free(l_string); //cant free x or y since on stack

    matrix = malloc(sizeof(double complex*)*LINES);
    done = malloc(sizeof(unsigned short int)*LINES);
    roots = malloc(sizeof(double complex)*(DEGREE+1));

    for(unsigned int i = 0; i < LINES; i++)
        matrix[i] = malloc(sizeof(double complex)*LINES);

    double dt = 4.0/LINES; //   4/LINES is integer division
    for(unsigned int i = 0; i < LINES; i++){
        done[i] = 0;
        for(unsigned int j = 0; j < LINES; j++)
            matrix[i][j] = (-2+dt*j) + (2-dt*i)*I; 
    }   
    
    for(int k = 0; k < DEGREE; k++)
        roots[k] = cexp(2*PI*I*(double)k/DEGREE);
    roots[DEGREE] = 0; // means dont converge to any root!


    for(int k = 0; k < DEGREE; k++)
        printf("%f + i%f\n", creal(roots[k]), cimag(roots[k]));


    printf("%d %d %d \n", DEGREE, N_THREADS, LINES);

    attractors = malloc(sizeof(unsigned short int*)*LINES);
    convergences = malloc(sizeof(unsigned int*)*LINES);
    pthread_t newtoner[N_THREADS];
    pthread_t writer;

    for(unsigned short int i = 0; i < N_THREADS; i++){
        unsigned short int *a = malloc(sizeof(unsigned short int));
        *a = i;
        pthread_create(&newtoner[i], NULL, newton_fun, (void *)a);
    }
    //here wait until start writer thread
    pthread_create(&writer, NULL, write_fun, NULL);
    //sleep(2);
    pthread_join(writer, NULL);    
}

void *newton_fun(void *arg){
    
    unsigned short int offset = * (int *)arg;
    free(arg);

    for(unsigned int i = offset; i < LINES; i+=N_THREADS){

        attractors[i] = malloc(sizeof(unsigned short int)*LINES);
        convergences[i] = malloc(sizeof(unsigned int)*LINES);
        for(unsigned int j = 0; j < LINES;){
            
            double complex z = matrix[i][j]; 
            unsigned int iter = 0;
            while(1){

                double a = creal(z);
                double b = cimag(z);
                double normsq = a*a+b*b;

                if(normsq < TOL*TOL || a > 10000000000 || b > 10000000000){
                    attractors[i][j] = DEGREE;
                    convergences[i][j] = iter;
                    break;

                }
                        
                if(! (normsq >= (TOL+1)*(TOL+1) || normsq <= (1-TOL)*(1-TOL))){ //if this holds then can't be close to any root
                    //here check if close to any root
                    // only check the ones with same imaginary part but start with everyone
                    for(unsigned short int d = 0; d < DEGREE; d++){

                        double complex root = roots[i];
                        double root_a = creal(root);
                        double root_b = cimag(root);
                        if( (a-root_a)*(a-root_a)+(b-root_b)*(b-root_b) < TOL*TOL){
                            attractors[i][j] = d;
                            convergences[i][j] = iter;
                            goto end;

                        }
                    }
                }
                double complex zconj = (a-b*I)/normsq;
                switch(DEGREE){

                    case 1:
                        z = 0;                  
                        break;
                    case 2:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj);                       
                        break;
                    case 3:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj);   
                        break;
                    case 4:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj);
                        break;
                    case 5:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj);
                        break;
                    case 6:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj);
                        break;
                    case 7:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj);
                        break;
                    case 8:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj*zconj); 
                        break;
                    case 9:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj*zconj*zconj);
                        break;
                    case 10:
                        z = 1/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj*zconj*zconj*zconj);
                        break;
                                   
                    default:
                        fprintf(stderr, "unexpected degree\n"); //can choose which stream to print to. stdout (terminal)
                        exit(1);                                // or a file or stderr which is terminal and red text
                }
                iter++;
            }
            end:   
            j++;  //need statement after goto label    
        }
        done[i] = 1;

    }
    return NULL;

}

void *write_fun(void *arg){
    return NULL;

}

//./newton -t5 -l1000 7
//./newton -l1000 -t5 7
//printf("%f + i%f\n", creal(result), cimag(result))
