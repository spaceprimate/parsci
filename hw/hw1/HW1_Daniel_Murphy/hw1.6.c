#include<stdio.h>
#include <omp.h>
/*
    CSCI6454 UNO - Fall 2020
    HW1 - Question 6
    Daniel Murphy

    Computing the sum and average of all elements in a 2D array
    Uses a nested a OpenMP parallel-for statement containing a nested for-loop
    Utilizes reduction and implements versions for both static and dynamic scheduling
*/

#define STATIC_CHUNK 20     // threads assigned to static implementation
#define DYNAMIC_CHUNK 20    // threads suggested to use for the dynamic implementation

int main (){
    // create big array to sum
    int i, ct = 0;
    int arr[200][10000];
    for(i = 0; i <20; i++){
        int e;
        for(e=0; e<100; e++){
            arr[i][e] = ct;
            ct += 1;
        }
    }

    // save array dimensions
    int col_ct = sizeof(arr[0]) / 4;
    int row_ct = sizeof(arr) / col_ct / 4;
    int total_cells = col_ct * row_ct;
    printf("Generating array with %d columns and %d rows\nTotal cells: %d\n\n", col_ct, row_ct, total_cells);

    double sum = 0.0, dynamic_time = omp_get_wtime();

    // DYNAMIC SCHEDULING implementation
    #pragma omp parallel
    {
        // parallel-for loop using reduction
        #pragma omp for reduction(+:sum) schedule(dynamic,DYNAMIC_CHUNK)
            for(int i = 0; i<row_ct; i++){
                for(int e = 0; e < col_ct; e++){
                    sum = sum + arr[i][e];
                }
            }

    }        
    dynamic_time = omp_get_wtime() - dynamic_time;
    printf("schedule_type\tsum\t\tavg\t\ttime\n");
    printf("dynamic\t\t%e\t%e\t%e\n", sum, sum/total_cells, dynamic_time);

    //reset sum and times
    sum = 0.0;
    double static_time = omp_get_wtime();

    // STATIC SCHEDULING implementation
    #pragma omp parallel
    {
        // parallel-for loop using reduction
        #pragma omp for reduction(+:sum) schedule(static,STATIC_CHUNK)
            for(int i = 0; i<row_ct; i++){
                for(int e = 0; e < col_ct; e++){
                    sum = sum + arr[i][e];
                }
            }
    }      
    static_time = omp_get_wtime() - static_time;  
    printf("static\t\t%e\t%e\t%e\n", sum, sum/total_cells, static_time);

}