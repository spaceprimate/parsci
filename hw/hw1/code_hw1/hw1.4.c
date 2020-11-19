#include<stdio.h>
#include <omp.h>
/*
    CSCI6454 UNO - Fall 2020
    HW1 - Question 4
    Daniel Murphy

    OpenMP implementation of appriximation of pi using padding
    note: some code derived from lecture slides
*/
static long num_steps = 100000;
double step;
#define PAD 8  // assume 64 byte L1 cache line size
#define NUM_THREADS 6
void main (){
    printf("Approximating pi using 100000 steps with OpenMP threads stored in padded array:\n\n");
    
    // sum is 2 dimensional array with a pad to fit 2 ints (64 bits)
    int i, nthreads; double pi, sum[NUM_THREADS][PAD], tdata;
    tdata = omp_get_wtime();
    step = 1.0/(double) num_steps;
    omp_set_num_threads(NUM_THREADS);


    //start parallel region
    #pragma omp parallel
    {
        int i, id,nthrds;

        double x;

        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;

        // loop over 100000 steps, each thread id representing
        // padded cell in the array containing the iterated sum
        for (i=id, sum[id][0]=0.0; i< num_steps; i=i+nthrds) {
            x = (i+0.5)*step;
            sum[id][0] += 4.0/(1.0+x*x);
        }
    }
    for(i=0, pi=0.0;i<nthreads;i++)pi += sum[i][0] * step;
    tdata = omp_get_wtime() - tdata;
    printf(" pi = %f\n\n%f secs\t\t%d padding\n", pi, tdata, PAD);
}