#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

/*
    CSCI6454 UNO - Fall 2020
    HW3 - Question 1
    Daniel Murphy
*/
int main( int argc, char *argv[] )
{
    // preliminaries / setting up environment
    int rank, size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    





    /*  ################################################################################################
    *   Q1  - Hello! world
    *   each process identifies itself
    */
    int adj_rank = rank + 1;
    printf( "Q1 - hello from process %d\n", adj_rank );
    MPI_Barrier(MPI_COMM_WORLD);







    /*  ################################################################################################
    *   Q2 - Basic send/receive
    *   process i will send a message to process i + 1
    *   but we must handle the special cases of the first and last
    *   processes
    */
        
    int src=7, dest=5;
    if(rank == 0){
        src = size - 1;
    }
    else{
        src = rank - 1;
    }
    if(rank == size - 1){
        dest = 0;
    }
    else{
        dest=rank + 1;
    }
        
    // our message
    int buf = 123456;
    MPI_Send( &buf, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);    
    MPI_Recv( &buf, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &status );
    printf("Q2 - Recieved a message from process %d\n", src );

    MPI_Barrier(MPI_COMM_WORLD);







    /*  ################################################################################################
    *   Q3 - Broadcasting
    *   process 0 broadcasts the number 42 to all other processes
    */

    int msg;
    if(rank == 0){
        msg = 42;
        
    }
    MPI_Bcast( &msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank > 0){
        // print the number of the process and the interger message it received
        printf("Q3 - process %d received number: %d\n", rank, msg);
    }

    MPI_Barrier(MPI_COMM_WORLD);





    /*  ################################################################################################
    *   Q4 - Scatter and Gather
    *   show an example of scatter and gather operations. 
    *   Print what each process had before and after scatter and gather collectives.
    */
    // SCATTER --------------------------------------------------------------------------------------
    int gsize,*sendbuf;
    int bufsize = 8; // this is the sub-divider, should be lower?
    int sendSize = 2;   // we'll send an array of 2 numbers
    int recSize = 2;    // we'll also receive an array of 2 numbers
    int root = 0;       // Process 0 serves as the root
    int sbuf[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };   // our send buffer
    int rbuf[2] = {0,0};                        // our recieve buffer
    

    MPI_Barrier(MPI_COMM_WORLD);

    // print the values held in the receive buffer before
    printf("Q4 Scatter, before - \tProcess %d:\t[ %d, %d ]\n", rank, rbuf[0], rbuf[1]);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Comm_size(MPI_COMM_WORLD, &gsize);
    sendbuf = (int *)malloc(gsize*bufsize*sizeof(int));
    
    MPI_Scatter(sbuf, sendSize, MPI_INT, &rbuf, recSize, MPI_INT, root, MPI_COMM_WORLD);

    // print the values held in the receive buffer after scatter has been called
    printf("Q4 Scatter, after - \tProcess %d:\t[ %d, %d ]\n", rank, rbuf[0], rbuf[1]);
    
    // GATHER --------------------------------------------------------------------------------------
    int *gather_rec = NULL;     // points to the array values are gathered to
    if(rank == 0){
        gather_rec = malloc(sizeof(int) * size);
    }
    int val = (rank + 1) * 2;
    MPI_Gather(&val, 1, MPI_INT, gather_rec, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        int i = 0;
        for(i = 0; i< size; i++){
            // print each value of the gather array
            printf("Q4 Gather - \tvalue gathered: %d\n", *(gather_rec + i));
        }
    }





    /*  ################################################################################################    
    *   Q5 - Reduce
    *   sum and average of an array of numbers using reduce
    */

    // each process saves a unique number ( calculated using val from above)
    float sum_val = (float)val;
    float sum = 0;
    float avg = 0;
    
    MPI_Reduce(&sum_val, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0){
            avg = sum / size;

            // print out sum
            printf("Q5 - sum = %f\n", sum);

            // print out average
            printf("Q5 - avg = %f\n", avg);
    }

    MPI_Finalize();

    return 0;
}
