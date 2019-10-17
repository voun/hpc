#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <string.h>
#include <stdint.h>
#define BLOCK_SIZE 10000

static inline short toInt(char *number);

int main(int argc, char *argv []){

    char *x = argv[1];
    const unsigned short int N_THREADS = atoi(x+2);
    
    FILE *ptr_read = fopen("test_data/cell_e5", "r");
    //FILE *ptr_read = fopen("cells", "r");

    unsigned int **restrict distances_threads = malloc(N_THREADS*sizeof(unsigned int*));
    for(unsigned short int i = 0; i < N_THREADS; i++)
        distances_threads[i] = calloc(3465, sizeof(unsigned int));

    unsigned int block1_pos = 0, block2_pos = 0;
    
    short *restrict x_coords1 = malloc(sizeof(short)*BLOCK_SIZE);                     
    short *restrict y_coords1 = malloc(sizeof(short)*BLOCK_SIZE);
    short *restrict z_coords1 = malloc(sizeof(short)*BLOCK_SIZE);

    short *restrict x_coords2 = malloc(sizeof(short)*BLOCK_SIZE);                   
    short *restrict y_coords2 = malloc(sizeof(short)*BLOCK_SIZE);
    short *restrict z_coords2 = malloc(sizeof(short)*BLOCK_SIZE);

    
    char *restrict block1 = malloc(BLOCK_SIZE*sizeof(char)*24);
    char *restrict block2 = malloc(BLOCK_SIZE*sizeof(char)*24);
    
    size_t d1, d2;
    while( (d1 = fread(block1, sizeof(char), BLOCK_SIZE*sizeof(char)*24, ptr_read)) != 0 ){
        
        unsigned short int lines_to_read1 = d1/24;
        #pragma omp parallel num_threads(N_THREADS)
        {         
            unsigned int *restrict distances_loc = distances_threads[omp_get_thread_num()];
            #pragma omp for schedule(static)
            for(unsigned short int i = 0; i < lines_to_read1; i++){  
                  
                x_coords1[i] = toInt(block1 + 24*i);
                y_coords1[i] = toInt(block1 + 24*i+8);
                z_coords1[i] = toInt(block1 + 24*i+16);
                
            } 

            #pragma omp for schedule(dynamic)
            for(unsigned short int i = 0; i < lines_to_read1-1; i++){  
                                                 
                short p1x = x_coords1[i];
                short p1y = y_coords1[i];
                short p1z = z_coords1[i];
                short p2x, p2y, p2z; 
                
                                                          
                for(unsigned short int j = i+1; j < lines_to_read1; j++){ 
                   
                    p2x = x_coords1[j];
                    p2y = y_coords1[j];
                    p2z = z_coords1[j];

                    unsigned short int dist = (int)sqrt(((p1x-p2x)*(p1x-p2x) + (p1y-p2y)*(p1y-p2y) + (p1z-p2z)*(p1z-p2z))/100.0);
                    
                    distances_loc[dist]++; 
                    
                 }
            }           
        }
        
        block2_pos = block1_pos+1;
        while( (d2 = fread(block2, sizeof(char), BLOCK_SIZE*sizeof(char)*24, ptr_read)) != 0){

            unsigned short int lines_to_read2 = d2/24;         
            #pragma omp parallel num_threads(N_THREADS)
            {

                unsigned int *restrict distances_loc = distances_threads[omp_get_thread_num()];
                #pragma omp for schedule(static)
                for(unsigned short int i = 0; i < lines_to_read2; i++){
                    
                    x_coords2[i] = toInt(block2 + 24*i);
                    y_coords2[i] = toInt(block2 + 24*i+8);
                    z_coords2[i] = toInt(block2 + 24*i+16);
                    
                }
                
                #pragma omp for schedule(static)            
                for(unsigned short int i = 0; i < lines_to_read1; i++){
                       
                      
                    short p1x = x_coords1[i];
                    short p1y = y_coords1[i];
                    short p1z = z_coords1[i];
                    short p2x, p2y, p2z;
                                                              
                    for(unsigned short int j = 0; j < lines_to_read2; j++){
                        
                        p2x = x_coords2[j];
                        p2y = y_coords2[j];
                        p2z = z_coords2[j];
                        
                        unsigned short int dist = (int)sqrt(((p1x-p2x)*(p1x-p2x) + (p1y-p2y)*(p1y-p2y) + (p1z-p2z)*(p1z-p2z))/100.0);
                        distances_loc[dist]++; 
                    }  
                }
            }

            block2_pos++;
            fseek(ptr_read, BLOCK_SIZE*24*block2_pos, SEEK_SET);
        }
        block1_pos++;
        fseek(ptr_read, BLOCK_SIZE*24*block1_pos, SEEK_SET);
     }

    fclose(ptr_read);
    
    
    for(unsigned short int i = 0; i < 3465; i++){
        unsigned long long int count = 0;
        for(unsigned short int j = 0; j < N_THREADS; j++)
            count += distances_threads[j][i];
      
        if(count == 0)
            continue;      
                  
        printf("%05.2f %lld\n", i/100.0, count);
    }
    
}

static inline short toInt(char *number){

    short total = 0; 

    total += (number[1] - 48) * 10000;
    total += (number[2] - 48)*1000;
    total += (number[4] - 48)*100;
    total += (number[5] - 48) * 10;
    total += number[6] - 48;

    return -1*(number[0]-44)*total;
}


