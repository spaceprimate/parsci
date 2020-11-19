#include<stdio.h>
#include <omp.h>
/*
    CSCI6454 UNO - Fall 2020
    HW1 - Question 3, part 1
    Daniel Murphy

    Serial implementation of PI approximation formula
    note: some code derived from lecture slides
*/

// set number of steps
static long num_steps = 100000;

// step (point from middle of each step)
double step;
int main (){
    printf("Approximating pi using 100000 steps:\n\n");
    int i;
    double x, pi, sum = 0.0, tdata;
    step = 1.0/(double) num_steps;
    tdata = omp_get_wtime();

    // implement pi approximation formula
    // sums all 100000 steps
    for (i=0;i< num_steps; i++){
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step * sum;
    tdata = omp_get_wtime() - tdata;
    printf("pi = %f \nCalculated in %f secs\n", pi, tdata);
}