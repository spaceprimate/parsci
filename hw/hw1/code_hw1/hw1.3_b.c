#include<stdio.h>
#include <omp.h>
/*
    CSCI6454 UNO - Fall 2020
    HW1 - Question 3, part 2
    Daniel Murphy

    OpenMP implementation of PI approximation formula
    note: some code derived from lecture slides
*/
static long num_steps = 100000;
double step;
#define NUM_THREADS 5
int main (){
    printf("Approximating pi using 100000 steps with OpenMP threads:\n\n");
    int i;
    double x, pi, sum[NUM_THREADS], tdata;
    step = 1.0/(double) num_steps;
    tdata = omp_get_wtime();

    // set the OpenMP thread count
    omp_set_num_threads(NUM_THREADS);

    // set start of parallel region
    #pragma omp parallel
    {
        int i,id;
        double x;

        // each iteration is stored in sum array with offset relative to it's thread number
        id = omp_get_thread_num();
        for (i=id, sum[id] = 0.0; i< num_steps; i= i+2){
            x = (i+0.5)*step;
            sum[id] += + 4.0/(1.0+x*x);
        }
    }
    for(i=0, pi=0.0; i<2; i++)pi += sum[i] * step;
    tdata = omp_get_wtime() - tdata;
    printf("pi = %f \nCalculated in %f secs\n", pi, tdata);
}