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
double dt;
char **convergences_strings; 
char **attractors_strings;
unsigned short int DEGREE, N_THREADS;
unsigned int LINES;
double complex * restrict roots;
char* restrict COLORS[] = {"9 0 0 ", "0 9 0 ", "9 5 3 ", "9 9 0 ", "0 9 9 ", "0 5 5 ", "0 5 0 ", "5 0 5 ", "0 0 5 ", "8 0 8 ", "0 0 8 "};
//pre-format strings for speed
char* restrict ITERATIONS[] = {"00 00 00 ", "01 01 01 ", "02 02 02 ", "03 03 03 ", "04 04 04 ", "05 05 05 ", "06 06 06 ", "07 07 07 ", "08 08 08 ", "09 09 09 ",
                      "10 10 10 ", "11 11 11 ", "12 12 12 ", "13 13 13 ", "14 14 14 ", "15 15 15 ", "16 16 16 ", "17 17 17 ", "18 18 18 ", "19 19 19 ",
                      "20 20 20 ", "21 21 21 ", "22 22 22 ", "23 23 23 ", "24 24 24 ", "25 25 25 ", "26 26 26 ", "27 27 27 ", "28 28 28 ", "29 29 29 ",
                      "30 30 30 ", "31 31 31 ", "32 32 32 ", "33 33 33 ", "34 34 34 ", "35 35 35 ", "36 36 36 ", "37 37 37 ", "38 38 38 ", "39 39 39 ",
                      "40 40 40 ", "41 41 41 ", "42 42 42 ", "43 43 43 ", "44 44 44 ", "45 45 45 ", "46 46 46 ", "47 47 47 ", "48 48 48 ", "49 49 49 ",
                      "50 50 50 ", "51 51 51 ", "52 52 52 ", "53 53 53 ", "54 54 54 ", "55 55 55 ", "56 56 56 ", "57 57 57 ", "58 58 58 ", "59 59 59 ",
                      "60 60 60 ", "61 61 61 ", "62 62 62 ", "63 63 63 ", "64 64 64 ", "65 65 65 ", "66 66 66 ", "67 67 67 ", "68 68 68 ", "69 69 69 ",
                      "70 70 70 ", "71 71 71 ", "72 72 72 ", "73 73 73 ", "74 74 74 ", "75 75 75 ", "76 76 76 ", "77 77 77 ", "78 78 78 ", "79 79 79 ",
                      "80 80 80 ", "81 81 81 ", "82 82 82 ", "83 83 83 ", "84 84 84 ", "85 85 85 ", "86 86 86 ", "87 87 87 ", "88 88 88 ", "89 89 89 ",
                      "90 90 90 ", "91 91 91 ", "92 92 92 ", "93 93 93 ", "94 94 94 ", "95 95 95 ", "96 96 96 ", "97 97 97 ", "98 98 98 ", "99 99 99 "};

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

    char *t_string = malloc(sizeof(char)*(strlen(t)-1));
    char *l_string = malloc(sizeof(char)*(strlen(l)-1));
    strncpy(t_string, t + 2, strlen(t)-2); 
    strncpy(l_string, l + 2, strlen(l)-2);

    N_THREADS = atoi(t_string);
    LINES = atoi(l_string);
    DEGREE = atoi(argv[3]);


    free(t_string); free(l_string); 
    dt = 4.0/LINES;
    roots = malloc(sizeof(double complex)*(DEGREE+1));
    
    for(unsigned short int d = 0; d < DEGREE; d++)
        roots[d+1] = cexp(2*PI*I*(double)d/DEGREE);
    roots[0] = 0;

    attractors_strings = malloc(sizeof(char *)*LINES);
    convergences_strings = malloc(sizeof(char *)*LINES);
    
    for(unsigned int i = 0; i < LINES; i++){
        attractors_strings[i] = malloc(sizeof(char)*6*LINES+1);
        attractors_strings[i][LINES*6] = 'N'; //means this line not yet finished
        convergences_strings[i] = malloc(sizeof(char)*9*LINES);
    }

    pthread_t newtoner[N_THREADS];
    pthread_t writer;
    
    for(unsigned short int i = 0; i < N_THREADS; i++){
        unsigned short int *a = malloc(sizeof(unsigned short int));
        *a = i;
        pthread_create(&newtoner[i], NULL, newton_fun, (void *)a);
    }
    pthread_create(&writer, NULL, write_fun, NULL);

    
    pthread_join(writer, NULL);
    return 0;
}

void *newton_fun(void* restrict arg){ 
    
    unsigned short int offset = *(int *)arg;
    free(arg);
    
    for(unsigned int i = offset; i < LINES; i+=N_THREADS){
        
        for(unsigned int j = 0, k1 = 0, k2 = 0; j < LINES; k1+=9, k2+=6){
            
            double complex z = (-2+dt*j) + (2-dt*i)*I; 
            
            unsigned int iter = 0;
            double complex zconj, root;
            double a, b, normsq, root_a, root_b;
                   
            while(1){

                a = creal(z);
                b = cimag(z);
                normsq = a*a+b*b;

                if(!(normsq >= (TOL+1)*(TOL+1)) && !( normsq <= (1-TOL)*(1-TOL))){ //if normsq too large or too small then cant be close to root
                    
                    for(unsigned short int d = 1; d <= DEGREE; d++){
                        
                        root = roots[d];
                        root_a = creal(root);
                        root_b = cimag(root);
                        if( (a-root_a)*(a-root_a)+(b-root_b)*(b-root_b) < TOL*TOL){
                                                        
                            if(iter > 99) 
                                iter = 99;
                            
                            memcpy(convergences_strings[i] + k1, ITERATIONS[iter], 9);
                            memcpy(attractors_strings[i] + k2, COLORS[d], 6); 
                                                   
                            goto end;
                        }
                    }                   
                }
              
                else if(normsq < TOL*TOL || a > 10000000000 || b > 10000000000){
                               
                    if(iter > 99) 
                        iter = 99;
                    
                    memcpy(convergences_strings[i] + k1, ITERATIONS[iter], 9);
                    memcpy(attractors_strings[i] + k2, COLORS[0], 6);

                    break;
                }
                        
                zconj = (a-b*I)/normsq;
                iter++;
                switch(DEGREE){

                    case 1:
                        z = 1.0;
                        break;
                    case 2:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj);                       
                        break;
                    case 3:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj);   
                        break;
                    case 4:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj);
                        break;
                    case 5:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj);
                        break;
                    case 6:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj);
                        break;
                    case 7:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj);
                        break;
                    case 8:      
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj*zconj);                                           
                        break;
                    case 9:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj*zconj*zconj);                    
                        break;
                    case 10:
                        z = 1.0/DEGREE*(z*(DEGREE-1)+zconj*zconj*zconj*zconj*zconj*zconj*zconj*zconj*zconj);  
                        break;
                                   
                    default:
                        fprintf(stderr, "unexpected degree\n"); 
                        exit(1);                              
                }
            }
            end:   
            j++;  //need statement after goto label    
        }
        attractors_strings[i][LINES*6] = 'Y'; //now this line is done
    }
    return NULL;
}

void *write_fun(void *arg){

        
    char name1[50], name2[50];
    
    struct timespec sleepvalue;
    sleepvalue.tv_sec = 0;
    sleepvalue.tv_nsec = 10000; 

    sprintf(name1, "%s%d%s", "newton_attractors_x", DEGREE, ".ppm");
    sprintf(name2, "%s%d%s", "newton_convergence_x", DEGREE, ".ppm");

    FILE *attr_ptr = fopen(name1, "w");
    FILE *conv_ptr = fopen(name2, "w");

    fprintf(attr_ptr, "P3\n%d %d\n10\n", LINES, LINES);
    fprintf(conv_ptr, "P3\n%d %d\n100\n", LINES, LINES);
       
    for(unsigned int i = 0; i < LINES;){
        
        
        if(attractors_strings[i][LINES*6] == 'N'){ //wait until finished
            nanosleep(&sleepvalue, NULL);
            continue;
        }

        for(; i < LINES && attractors_strings[i][LINES*6] == 'Y' ; i++){
       
            fwrite(attractors_strings[i], sizeof(char), 6*LINES, attr_ptr);    
            fwrite("\n", sizeof(char), 1, attr_ptr);  
               
            fwrite(convergences_strings[i], sizeof(char), 9*LINES, conv_ptr);
            fwrite("\n", sizeof(char), 1, conv_ptr);
                    
        }       
    }
    fclose(attr_ptr);
    fclose(conv_ptr);
    return NULL;
}

