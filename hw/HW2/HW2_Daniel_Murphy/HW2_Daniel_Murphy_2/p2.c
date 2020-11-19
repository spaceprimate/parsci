#include<stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>
/*
    CSCI6454 UNO - Fall 2020
    HW2 - Question 2
    Daniel Murphy

    Implements matrix multiplication rythms using a naive method, optimized method, and parallel method.

    Matrices are automatically generated using random values. The size of the matrices used can be changed
    via the SIZE constant below

    Input and output matrices will be logged to the terminal when SIZE is below 101, otherwise only
    the performance (in seconds) of the individual algorithm will be logged.
*/
#define STATIC_CHUNK 250     // number of tasks assigned to each thread
#define NUM_THREADS 1        // thread count
#define SIZE 1000            // size of matrices to be multiplied

int main() 
{ 
    // varaiables for displaying output
    double naiveTime, optimizedTime, parallelTime;
    int threadCount = NUM_THREADS;

    int size = SIZE;   // matrix size
                        // this will create a size x size matrix

    // seed rand                        
    srand(time(NULL));

    // allocate memory for another array (was getting seg faults if I didn't allocate first)
    int rows = size;
    int cols = size;
    int **MatA, **MatB, **MatC;
    MatA = malloc(sizeof(int*) * rows); // create array of int pointers
    MatB = malloc(sizeof(int*) * rows); 
    MatC = malloc(sizeof(int*) * rows); 

    for(int i = 0; i < rows; ++i) {
        MatA[i] = malloc(sizeof(int) * cols); // create rows
        MatB[i] = malloc(sizeof(int) * cols); 
        MatC[i] = malloc(sizeof(int) * cols); 
    }

    // declare some idices
    int i, e, d, i1, i2;

    // init the MatC matrix
    for (i = 0; i < size; i++) {
        for ( e = 0; e < size; e++) {
            MatC[i][e] = 0;     // Matc matrix initialized to zero
        }
    }

    // create 2 1000 x 1000 arrays populated with random values
    for (i1 = 0; i1 < size; i1++){
        for (i2 = 0; i2 < size; i2++){
            MatA[i1][i2] = rand() % 9;      // MatA matrix
        }
    }
    for (i1 = 0; i1 < size; i1++){
        for (i2 = 0; i2 < size; i2++){
            MatB[i1][i2] = rand() % 9;      // MatB matrix
        }
    }


    // print input matrices (disabled for very large matrix size)
    if(size < 101){
        printf("\nINPUT:\nMatA: ---------------------------       MatB ---------------------------- \n");
        for(i = 0; i < size; i++){
            for(e = 0; e< size; e++){
                printf("%d\t", MatA[e][i]);
            }
            // printf("\t");
            for(e = 0; e< size; e++){
                printf("%d\t", MatB[e][i]);
            }
            printf("\n");
        }
    }

    // this keeps track of our time
    double tdata;
    tdata = omp_get_wtime();

//-----------------------------------------------------------------------------------------------
//
// Naive implementation
//
//-----------------------------------------------------------------------------------------------
    for(i=0; i < size; i++){
        for(e=0; e< size; e++){
            for(d=0; d< size; d++){
                MatC[e][i] += MatA[d][i] * MatB[e][d];    
            }
        }
    }   

    naiveTime = omp_get_wtime() - tdata;
    // printf(" \nnaive implementation completed in\t %f secs\n", tdata);


    // output result (disabled for very large matrix size)
    if(size < 101){
        printf("\nRESULT:\nMatC: ----------------------------\n");
        for(i = 0; i < size; i++){
            for(e = 0; e< size; e++){
                printf("%d\t", MatC[e][i]);
            }
            printf("\n");
        }
    }
    


    // reset MatC
    for (i = 0; i < size; i++) {
        for ( e = 0; e < size; e++) {
            MatC[i][e] = 0;
        }
    }
   
    tdata = omp_get_wtime(); // reset time

//-----------------------------------------------------------------------------------------------
//
// Optimized implementation
//
//-----------------------------------------------------------------------------------------------
    // here we transpose the MatA matrix to optimize memory locality
    int f = 0;
    for (i = 0; i < size; i++) {
        for ( e = 0; e < f; e++) {
            int tmp = MatA[i][e];
            MatA[i][e] = MatA[e][i];
            MatA[e][i] = tmp;
        }
        f++;
    }

    for(i=0; i < size; i++){
        for(e=0; e< size; e++){
            int sum = 0;
            for(d=0; d< size; d++){
                sum += MatA[i][d] * MatB[e][d];    
            }
            MatC[e][i] = sum;
        }
    } 
    optimizedTime = omp_get_wtime() - tdata;

    // reset MatC
    for (i = 0; i < size; i++) {
        for ( e = 0; e < size; e++) {
            MatC[i][e] = 0;
        }
    }

    tdata = omp_get_wtime(); // reset time

//-----------------------------------------------------------------------------------------------
//
// Parallel implementation
//
//-----------------------------------------------------------------------------------------------
    // this will run the optimized algorithm above in parallel
    // an additional modification is that the output matrix is output transposed, 
    //    allowing each thread to write to contiguous memory locations
    //    transposing the output matrix MatC again, gives us the final result
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int i, e, d;

        // the machine I'm running the tests on has 4 cores available. So I'm using a static chunk of 250 threads
        #pragma omp for schedule(static,STATIC_CHUNK)
        for(i=0; i < size; i++){
            
            for(e=0; e< size; e++){
                int sum = 0;
                for(d=0; d< size; d++){
                    sum += MatA[i][d] * MatB[e][d];    
                }
                MatC[i][e] = sum; // output to transposed location
            }
        }
    }

    // transpose MatC back
    f = 0;
    for (i = 0; i < size; i++) {
        for ( e = 0; e < f; e++) {
            int tmp = MatC[i][e];
            MatC[i][e] = MatC[e][i];
            MatC[e][i] = tmp;
        }
        f++;
    }
     
    parallelTime = omp_get_wtime() - tdata;


    // print results
    printf("\nCALCULATION TIMES: \n");
    printf("naive: \t\t%f seconds\n", naiveTime);
    printf("optimized: \t%f seconds\n", optimizedTime);
    printf("%d threads: \t%f seconds\n", threadCount, parallelTime);


    // write to file
    FILE *fptr;
    fptr = fopen("output.txt", "w");
    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            fprintf(fptr, "%d\t", MatC[e][i]);
        }
        fprintf(fptr, "%s", "\n");
    }

    fclose(fptr);
    return 0; 
} 