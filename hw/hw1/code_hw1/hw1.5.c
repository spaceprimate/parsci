#include<stdio.h>
#include <omp.h>
/*
    CSCI6454 UNO - Fall 2020
    HW1 - Question 5
    Daniel Murphy

    approximation of PI using both implicit and explicity OpenMP barriers
    note: some code derived from lecture slides
*/
static long num_steps = 100000;
double step;
#define NUM_THREADS 6
int main (){
    printf("Approximating pi using 100000 steps \n with OpenMP threads utilizing a critical region:\n\n");

    int nthreads;
    double pi = 0.0, tdata;
    tdata = omp_get_wtime();
    step = 1.0/(double) num_steps;
    omp_set_num_threads(NUM_THREADS);

    // begin parallel region
    #pragma omp parallel
    {
        int i, id, nthrds;
        double x, sum;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if(id == 0){
            nthreads = nthrds;
        }
        for(i=id, sum=0.0; i<num_steps; i=i+nthrds){
            x = (i+0.5)*step;
            sum += 4.0/(1.0+x*x);
        }

        // explicit barrier, exclusive access only
        // threads must wait their turn to access (one at a time)
        #pragma omp critical
            pi += sum * step;

    // implicit barrier, all threads must complete before progressing further
    }

    tdata = omp_get_wtime() - tdata;
    printf(" pi = %f in %f secs\n", pi, tdata);

}