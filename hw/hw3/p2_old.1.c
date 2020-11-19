#include "mpi.h"
#include<stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
struct timeval start, stop;



/*
    CSCI6454 UNO - Fall 2020
    HW3 - Question 2
    Daniel Murphy
*/

#define SIZE 4            // size of matrices to be multiplied

int main( int argc, char *argv[] )
{ 
    int rank, proc_size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &proc_size );


    double secs = 0;
    gettimeofday(&start, NULL);


    // adapted from stack overflow user Jonathan Dursi
    int malloc2dfloat(int ***array, int n, int m) {

        /* allocate the n*m contiguous items */
        int *p = (int *)malloc(n*m*sizeof(int));
        if (!p) return -1;

        /* allocate the row pointers into the memory */
        (*array) = (int **)malloc(n*sizeof(int*));
        if (!(*array)) {
        free(p);
        return -1;
        }

        /* set up the pointers into the contiguous memory */
        for (int i=0; i<n; i++) 
        (*array)[i] = &(p[i*m]);

        return 0;
    }







    // varaiables for displaying output
    // todo: we need new vars here
    // double naiveTime, optimizedTime, parallelTime;
    

    int size = SIZE;   // matrix size
                        // this will create a size x size matrix

    // seed rand                        
    srand(time(NULL));

    // allocate memory for another array (was getting seg faults if I didn't allocate first)
    int rows = size;
    int cols = size;
    int **MatA, **MatB;
    MatA = malloc(sizeof(int*) * rows); // create array of int pointers
    MatB = malloc(sizeof(int*) * rows); 

    

    // declare some idices
    int i, e, d, i1, i2;

    for(int i = 0; i < rows; ++i) {
            MatA[i] = malloc(sizeof(int) * cols); // create rows
            MatB[i] = malloc(sizeof(int) * cols); 
            
    }

    for (i1 = 0; i1 < size; i1++){
            for (i2 = 0; i2 < size; i2++){
                MatA[i1][i2] = 0;      // MatA matrix
            }
        }
        for (i1 = 0; i1 < size; i1++){
            for (i2 = 0; i2 < size; i2++){
                MatB[i1][i2] = 0;      // MatB matrix
            }
        }

    
    int msg; // tmp delete

    // ugh, your idea will work, but you need to use: https://stackoverflow.com/questions/5104847/mpi-bcast-a-dynamic-2d-array

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    // -- P1 only
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    if(rank == 0){
        msg = 42;
    

        int **MatC;
        MatC = malloc(sizeof(int*) * rows); 

        for(int i = 0; i < rows; ++i) {
            MatC[i] = malloc(sizeof(int) * cols); 
        }


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

    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    // -- Broadcast to all procs: MatA, MatB
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
    // MPI_Bcast(&msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&(MatA[0][0]), size, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&MatA, size, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(MatB, size*size, MPI_INT, 0, MPI_COMM_WORLD);



int malloc2dfloat(int ***array, int n, int m) {

    /* allocate the n*m contiguous items */
    int *p = (int *)malloc(n*m*sizeof(int));
    if (!p) return -1;

    /* allocate the row pointers into the memory */
    (*array) = (int **)malloc(n*sizeof(int*));
    if (!(*array)) {
       free(p);
       return -1;
    }

    /* set up the pointers into the contiguous memory */
    for (int i=0; i<n; i++) 
       (*array)[i] = &(p[i*m]);

    return 0;
}

int **array;
malloc2dfloat(&array, 10, 10);

int j = 0;

if (rank == 0) {
    for(i=0;i<10;i++)
         for(j=0;j<10;j++)
              array[i][j]=i+j;
}
// printf("arr p%d: %d\n",rank, array[1][2]);
MPI_Bcast(&(array[0][0]), 10*10, MPI_FLOAT, 0, MPI_COMM_WORLD);
printf("arr p%d: %d\n",rank, array[2][4]);

MPI_Finalize();

    // print input matrices (disabled for very large matrix size)
    if(size < 101){
        printf("\nINPUT:\nMatA, p%d: ---------------------------       MatB ---------------------------- \n", rank);
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


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for(i=0; i < size; i++){
        for(e=0; e< size; e++){
            for(d=0; d< size; d++){
                MatC[e][i] += MatA[d][i] * MatB[e][d];    
            }
        }
    }   

    // naiveTime = omp_get_wtime() - tdata;
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






    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("time taken %f\n",secs);



    //-----------------------------------------------------------------------------------------------
    //
    // Optimized implementation
    //
    // todo: solve for 4 procs
    //      then make it work for 3
    //-----------------------------------------------------------------------------------------------
    // here we transpose the MatA matrix to optimize memory locality
    int f = 0;
    int interval = size/proc_size;
    int start_index = rank * interval;
    int end_index = start_index + interval;
    for (i = start_index; i < size; i++) {
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
    

    // reset MatC
    for (i = 0; i < size; i++) {
        for ( e = 0; e < size; e++) {
            MatC[i][e] = 0;
        }
    }






















    // reset count
    secs = 0;
    gettimeofday(&start, NULL);

    // sleep(3);

    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("time taken %f\n",secs);




    !!!!!!!!!!!!!!!!!!!!!!!!!! */
}