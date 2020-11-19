#include<omp.h>
#include<stdio.h>

/*
    CSCI6454 UNO - Fall 2020
    HW1 - Question 2
    Daniel Murphy

    A very simple Hello World program using 10 OMP threads
    note: some code derived from lecture slides
*/
int main(){
    printf("Creating 10 POSIX threads, \neach calling the \"SayHello\" function\n\n");
    omp_set_num_threads(10);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("hello world from thread #%d\n", id);
    }
}