#include<stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>
/*
    CSCI6454 UNO - Fall 2020
    HW2 - Question 1
    Daniel Murphy

    
*/
#define STATIC_CHUNK 250     // threads assigned to static implementation
#define NUM_THREADS 4
#define SIZE 5




int main() 
{ 
    int size = SIZE;   // matrix size
                        // this will create a size x size matrix

    // seed rand                        
    srand(time(NULL));

    // allocate memory for another array (was getting seg faults if I didn't allocate first)
    int rows = size;
    int cols = size;
    int **MatA, **MatB, **MatC;
    MatA = malloc(sizeof(int*) * rows); // create array of int pointers
    MatB = malloc(sizeof(int*) * rows); // create array of int pointers
    MatC = malloc(sizeof(int*) * rows); // create array of int pointers

    for(int i = 0; i < rows; ++i) {
        MatA[i] = malloc(sizeof(int) * cols); // create rows
        MatB[i] = malloc(sizeof(int) * cols); // create rows
        MatC[i] = malloc(sizeof(int) * cols); // create rows
    }

    // declare some idices
    int i, e, d, i1, i2;

    // init the MatC matrix
    for (i = 0; i < size; i++) {
        for ( e = 0; e < size; e++) {
            MatC[i][e] = 0;
        }
    }

    // create 2 1000 x 1000 arrays populated with random values
    for (i1 = 0; i1 < size; i1++){
        for (i2 = 0; i2 < size; i2++){
            MatA[i1][i2] = rand() % 9;
        }
    }
    for (i1 = 0; i1 < size; i1++){
        for (i2 = 0; i2 < size; i2++){
            MatB[i1][i2] = rand() % 9;
        }
    }

    // this keeps track of our time
    double tdata;

    tdata = omp_get_wtime();

    //-----------------------------------------------------------------------------------------------
    // Naive implementation
    for(i=0; i < size; i++){
        for(e=0; e< size; e++){
            for(d=0; d< size; d++){
                MatC[e][i] += MatA[d][i] * MatB[e][d];    
            }
        }
    }   

    tdata = omp_get_wtime() - tdata;
    printf(" \nnaive implementation completed in\t %f secs\n", tdata);


    printf("\nC2: -----------------------\n");
    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatC[e][i]);
        }
        printf("\n");
    }



    // int MatAT[size][size];
    // int f, co;
    // for (f = 0; f < size; f++) {
    //     for ( co = 0; co < i; co++) {
    //         MatAT[f][co] = MatA[co][f];
    //     }
    // }


    // tranpose MatA (optimizes spatial memory access)
    int f = 0;
    for (i = 0; i < size; i++) {
        for ( e = 0; e < f; e++) {
            int tmp = MatA[i][e];
            MatA[i][e] = MatA[e][i];
            MatA[e][i] = tmp;
        }
        f++;
    }





    // reset MatC
    
    for (i = 0; i < size; i++) {
        for ( e = 0; e < size; e++) {
            MatC[i][e] = 0;
        }
    }



    // for (i = 0; i < size; i++){
    //     for (e = 0; e < size; e++){
    //         int sum = 0;
    //         for (d = 0; d < size; d++){
    //             MatC[i][e] += MatA[i][d]*MatAT[e][d];
    //             // MatC[i][e] = sum;
    //         }
    //     }    
    // }

    /*
    =================================================================================================
    =================================================================================================
    =================================================================================================
    2             ===================================================================================
    =================================================================================================
    =================================================================================================
    */
   
    tdata = omp_get_wtime();

    //-----------------------------------------------------------------------------------------------
    // memory 
    for(i=0; i < size; i++){
        for(e=0; e< size; e++){
            // MatC[e][i] = 0;
            int sum = 0;
            for(d=0; d< size; d++){
                sum += MatA[i][d] * MatB[e][d];    
            }
            MatC[e][i] = sum;
        }
    } 
    tdata = omp_get_wtime() - tdata;
    printf("\nnew in %f secs\n", tdata);



    printf("\nC2: -----------------------\n");
    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatC[e][i]);
        }
        printf("\n");
    }

    // reset MatC
    
    for (i = 0; i < size; i++) {
        for ( e = 0; e < size; e++) {
            MatC[i][e] = 0;
        }
    }



    tdata = omp_get_wtime();
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    
    {
        int i, e, d;
        #pragma omp for schedule(static,STATIC_CHUNK)
        for(i=0; i < size; i++){
            // #pragma omp parallel
            //     {
            //     #pragma omp for schedule(static,STATIC_CHUNK)
            for(e=0; e< size; e++){
                // MatC[e][i] = 0;
                int sum = 0;
                
                    for(d=0; d< size; d++){
                        sum += MatA[i][d] * MatB[e][d];    
                    }
                    
                // MatC[e][i] = sum;
                MatC[i][e] = sum;
            }
                // }
        }
    }
     
    tdata = omp_get_wtime() - tdata;
    printf("\nnew in %f secs\n", tdata);

    // printf("\nC2: -----------------------\n");
    // for(i = 0; i < size; i++){
    //     for(e = 0; e< size; e++){
    //         printf("%d\t", MatC[e][i]);
    //     }
    //     printf("\n");
    // }

    // tranpose MatC back
    f = 0;
    for (i = 0; i < size; i++) {
        for ( e = 0; e < f; e++) {
            int tmp = MatC[i][e];
            MatC[i][e] = MatC[e][i];
            MatC[e][i] = tmp;
        }
        f++;
    }

    printf("\nC3: -----------------------\n");
    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatC[e][i]);
        }
        printf("\n");
    }


    // write to file
    FILE *fptr;
    fptr = fopen("program.txt", "w");

    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            fprintf(fptr, "%d\t", MatC[e][i]);
        }
        fprintf(fptr, "%s", "\n");
    }

    

    fclose(fptr);

    

    return 0; 
} 