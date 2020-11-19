#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

/*
    CSCI6454 UNO - Fall 2020
    HW1 - Question 1
    Daniel Murphy

    A very simple Hello World program using 10 POSIX threads
    note: some code derived from lecture slides
*/

// prints Hello message, identifying the current system thread being used
void* SayHello(void *foo) {
    // get current thread id
    pid_t x = syscall(__NR_gettid);
    printf("Hello World from thread %d \n", x);
    return NULL;
}
int main() {
    printf("Creating 10 POSIX threads, \neach calling the \"SayHello\" function\n\n");

    // set 10 POSIX threads
    pthread_t threads[10];
    int tn;

    // call function for each thread
    for(tn=0; tn<10; tn++) {
        pthread_create(&threads[tn], NULL, SayHello, NULL);
    }

    // join each thread in turn
    for(tn=0; tn<10 ; tn++) {
        pthread_join(threads[tn], NULL);
    }
    return 0;
}