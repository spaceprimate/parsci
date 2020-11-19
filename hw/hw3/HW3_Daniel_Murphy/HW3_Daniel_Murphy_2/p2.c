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

    A MPI-based program that performs matrix-matrix multiplication on multiple processors

    Values for matrices MatA and MatB are randomgly generated. 
    MatA and MatB are distributed to multiple processors using MPI_Bcast
    Each processor calculates values for a subsection of result matrix
        These values are returned in a contiguous 1D array, MatFlat
        Finally, the root process assembles MatFlat into MatC and prints the results.
*/

#define SIZE 2016           // size of matrices to be multiplied
                            // for testing purposes, a value divisible by 1,2,3, and 4 is a good choice

int main( int argc, char *argv[] )
{ 
    // initial setup
    int rank, proc_size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &proc_size );

    double secs;// used to calculate computation time
     

    // prints 2 input matrices to terminal
    // only if their size is reasonably small
    int printAB(int size, int** MatA, int** MatB){
        int i, e;
        if(size < 10){
            printf("\nINPUT:\nMatA: ---------------------------------------------------       MatB ---------------------------------------------------- \n");
            for(i = 0; i < size; i++){
                for(e = 0; e< size; e++){
                    printf("%d\t", MatA[e][i]);
                }
                // printf("");
                for(e = 0; e< size; e++){
                    printf("%d\t", MatB[e][i]);
                }
                printf("\n");
            }
        }
        return 0;
    }

    // prints a matrix to the terminal
    int print_2d(int *arr, int w, int h){
        printf("\nOUTPUT:----------------------------------------------------\n");
        int i, e;
        for(i=0; i < w; i++){
            for(e=0; e< h; e++){
                printf("%d\t",arr[i*w + e]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // writes a matrix to file
    int write_2d(int *arr, int w, int h){
        int i, e;
        FILE *fptr;
        fptr = fopen("MatC.txt", "w");
        for(i = 0; i < w; i++){
            for(e = 0; e< h; e++){
                fprintf(fptr, "%d\t\t", arr[i*w + e]);
            }
            fprintf(fptr, "%s", "\n");
        }

        fclose(fptr);
        return 0;
    }

    // creates a contiguos-in-memory, 2 dimensional array that plays nicely 
    // with MPI operations
    // - informed by stack overflow comment by user Jonathan Dursi
    int contig_2d(int ***arr, int n, int m) {
        int *x = (int *)malloc(n*m*sizeof(int)); // allocate memory for items
        if (!x) return -1;
        (*arr) = (int **)malloc(n*sizeof(int*)); // allocate memory for row pointers
        for (int i=0; i<n; i++) {
            (*arr)[i] = &(x[i*m]);    
        }
        return 0;
    }

    
    // set matrix and sub-matrix size (interval)
    int size = SIZE;   // matrix size
                        // this will create a size x size matrix
    int interval = size / proc_size;

    // indices for each processes sub-matrix
    int start_index = interval * rank;
    int end_index = start_index + interval ;

    // seed rand                        
    srand(time(NULL));

    // allocate memory for another array (was getting seg faults if I didn't allocate first)
    int rows = size;
    int cols = size;
    

    // declare matrices
    // MatA and MatB are distributed to multiple processors using MPI_Bcast
    // MatFlat is an intermediary 1D array
    int **MatA, **MatB, *MatFlat;
    int *MatC = NULL;

    contig_2d(&MatA, size, size); // format 2d matrices as MPI friendly contiguous ints
    contig_2d(&MatB, size, size);
    

    // MatFlat is essentially a stripe of 'interval' rows (in the case of MatA, transposed columns) that 
    // are laid out in memory as a 1 dimensional array. This is done to simplify the the MPI_Gather step, once
    // each process has finished multiplying its respective rows
    MatFlat = malloc(size*interval*sizeof(int));

    

    // declare some idices
    int i, e, d, f, i1, i2;

    // populate with zeroes
    for(i=0; i<size*interval; i++){
        MatFlat[i] = 0;
    }
    

    // populate MatA and MatB with random numbers
    if(rank == 0){
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

        // print the matrices to terminal before we transpose MatA
        printAB(size, MatA, MatB);

        // here we transpose the MatA matrix to optimize memory locality
        f=0;
        for (i = 0; i < size; i++) {
            for ( e = 0; e < f; e++) {
                int tmp = MatA[i][e];
                MatA[i][e] = MatA[e][i];
                MatA[e][i] = tmp;
            }
            f++;
        }
    }

    
    // start timer
    if(rank == 0){
        secs = 0;
        gettimeofday(&start, NULL);
    }
    


    // make populated input matrices available to all processes
    MPI_Bcast(&(MatA[0][0]), size*size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&(MatB[0][0]), size*size, MPI_INT, 0, MPI_COMM_WORLD);

    // special index that maps 2d matrix locations onto 1d array
    int flatIndex = 0;

    // Run matrix multiplication calculation: 
    for(i=start_index; i < end_index; i++){
        f = i - rank * interval;
        for(e=0; e< size; e++){
            int sum = 0;
            for(d=0; d< size; d++){
                sum += MatA[i][d] * MatB[e][d];    
            }
            MatFlat[flatIndex*size + e] = sum;
        }
        flatIndex++;
    }


    // allocate memory for MatC
    if(rank == 0){
        MatC = malloc(sizeof(int) * size * size);
    }

    // Gather sub matrices from all processes to the root process
    MPI_Gather(MatFlat, (size * interval), MPI_INT, MatC, (size * interval), MPI_INT, 0, MPI_COMM_WORLD);


    // Print results
    if(rank == 0 && size < 10){
        print_2d(MatC, size, size);
    }

    if(rank == 0){
        gettimeofday(&stop, NULL);
        secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
        printf("time taken %f\n\n\n",secs);
    }


    if(rank == 0){
        write_2d(MatC, size, size);
    }
    
    MPI_Finalize();
    // fin.
}