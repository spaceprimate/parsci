#include<stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>
/*
    CSCI6454 UNO - Fall 2020
    HW2 - Question 2
    Daniel Murphy

    // ugh, use this: https://stackoverflow.com/questions/1907557/optimized-matrix-multiplication-in-c
*/




  

int main() 
{ 

    srand(time(NULL));

    int MatA[1000][1000];
    int MatB[1000][1000];

    int r = 1000, c = 1000;
    int *arr = (int *)malloc(r*c*sizeof(int));
    int i1, i2;
    for (i1 = 0; i1 < 1000; i1++){
        for (i2 = 0; i2 < 1000; i2++){
            MatA[i1][i2] = rand() % 50;
            MatB[i1][i2] = rand() % 50;
        }
    }
    

    int sizeA = sizeof MatA / sizeof MatA[0];

    printf("the size of the matrix is: %d", sizeA);

    int sizeB = sizeof MatB / sizeof MatB[0];

    printf("the size of the matrix is: %d", sizeB);

    int AN = sizeof MatA[0] / 4; //assumes an int
    int AM = sizeof MatA / sizeof MatA[0];
    int BN = sizeof MatB[0] / 4; //assumes an int
    int BM = sizeof MatB / sizeof MatB[0];

    // printf("N is: %d\n\n\n", NA);

    // multiply( sizeA, sizeB, MatA, NA, NB, MatB);

    int MatC[1000][1000];
    memset(MatC, 0, sizeof MatC);

    printf("\n------------------------------------\n");

    int i, e, d;
    for(i=0; i < AN; i++){
        for(e=0; e< BM; e++){
            for(d=0; d< BN; d++){
                MatC[e][i] += MatA[d][i] * MatB[e][d];
                // printf("%d * %d\t", MatA[d][i], MatB[e][d]);
            }
            // printf("\t\t");
        }
        // printf("\n");
    }



    for(i = 0; i < AN; i++){
        for(e = 0; e< BM; e++){
            // printf("%d\t", MatC[e][i]);
        }
        // printf("\n");
    }





    // int ax = sizeof MatA / sizeof MatA[0];


    // multiply(m1, m2, mat1, n1, n2, mat2); 

    // rando();
    return 0; 
} 