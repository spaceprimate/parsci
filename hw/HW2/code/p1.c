#include<stdio.h>
#include <omp.h>
/*
    CSCI6454 UNO - Fall 2020
    HW2 - Question 1
    Daniel Murphy

    
*/


// double threaded implementation

int fib(int n){
    int x,y;
    if(n<2) return n;

    #pragma omp task shared(x)
        x = fib(n-1);
    #pragma omp task shared(y)
        y = fib(n-2);

    #pragma omp taskwait
        return(x+y);
}



// single threaded implementation
int fib_single(int n){
    if(n<2)return n;

    return fib(n-1) + fib(n-2);
}


int runTest  (int num_threads, int fib_input){

    int ans;

    double tdata = omp_get_wtime();

    if(num_threads == 1){
        ans = fib_single(fib_input);
    }
    else{
        omp_set_num_threads(num_threads);
        #pragma omp parallel
        {
            #pragma omp single
                ans = fib(fib_input);
        }
    }


    tdata = omp_get_wtime() - tdata;

    
    printf("%d\t%d\t\t%d\t\t%f\n", fib_input, ans, num_threads, tdata);


}



int main (){
    printf("input\tresult\t\tno. threads\tproc time\n");
    runTest(1, 35);
    runTest(2, 35);
    runTest(3, 35);
    runTest(4, 35);

    
}