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

int main() 
{ 
    int size = 2; // matrix size

    srand(time(NULL));

    int MatA[size][size];
    int MatB[size][size];

    int i1, i2;

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
    printf("\nA: -----------------------\n");

    int i, e, d;

    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatA[e][i]);
        }
        printf("\n");
    }
        printf("\nB: -----------------------\n");

    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatB[e][i]);
        }
        printf("\n");
    }
    
    // allocate memory for another array (was getting seg faults if I didn't allocate first)
    int rows = size;
    int cols = size;

    int **MatC;
    MatC = malloc(sizeof(int*) * rows); // This creates an array of pointers to int
    for(int i = 0; i < rows; ++i) {
        MatC[i] = malloc(sizeof(int) * cols); // This creates a row of ints and asigns it to MatC[i]
    }

    
    for(i=0; i < size; i++){
        for(e=0; e< size; e++){
            for(d=0; d< size; d++){
                MatC[e][i] += MatA[d][i] * MatB[e][d];    
            }
        }
    }   

    printf("\n-----------------------\n");

    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatB[e][i]);
        }
        printf("\n");
    }


    int MatAT[size][size];
    int f, co;
    for (f = 0; f < size; f++) {
        for ( co = 0; co < i; co++) {
            MatAT[f][co] = MatA[co][f];
        }
    }

    printf("\nB: -----------------------\n");

    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatAT[e][i]);
        }
        printf("\n");
    }

    printf("\nC1: -----------------------\n");
    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatC[e][i]);
        }
        printf("\n");
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

    for(i=0; i < size; i++){
        for(e=0; e< size; e++){
            MatC[e][i] = 0;
            for(d=0; d< size; d++){
                MatC[e][i] += MatAT[i][d] * MatB[e][d];    
            }
        }
    } 



    printf("\nC2: -----------------------\n");
    for(i = 0; i < size; i++){
        for(e = 0; e< size; e++){
            printf("%d\t", MatC[e][i]);
        }
        printf("\n");
    }

    return 0; 
} 