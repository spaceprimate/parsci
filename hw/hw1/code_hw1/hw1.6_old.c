#include<stdio.h>
#include <omp.h>


int main (){
    // create big array
    int i, ct = 0;
    int arr[20][100];
    for(i = 0; i <20; i++){
        int e;
        for(e=0; e<100; e++){
            arr[i][e] = ct;
            ct += 1;
            // if(ct > 1990){
            //     printf("ct is: %d", ct);
            // }
        }
    }

    

    // calc serial sum and average
    int col_ct = sizeof(arr[0]) / 4;
    int row_ct = sizeof(arr) / col_ct / 4;
    int total_cells = col_ct * row_ct;

    printf("there are %d columns and %d rows, for a total of %d cells!\n", col_ct, row_ct, total_cells);

    int i2;
    double sum = 0, avg;

    for (i2 = 0; i2 < row_ct; i2++){
        int e2;
        for(e2 = 0; e2 < col_ct; e2++){
            sum += arr[i2][e2];
        }
    }

    printf("the sum total is: %e\n\n", sum);

    printf("calculating nested for sum\n");

    // int i3;
    
    // double sum2;
    // for(i3=0; i3<row_ct; i3++){
        
    //     omp_set_num_threads(20);
    //     #pragma omp parallel
    //     {
    //         double inner_sum = 0;
    //         #pragma omp for
    //             // int e3;
                
    //             for(int e3=0; e3<col_ct; e3++){
    //                 inner_sum += arr[i3][e3];
    //             }
    //         sum2 += inner_sum;
    //         printf("the sum total is: %e\n\n", sum2);
    //     }
    // }
    double sum2 = 0;
    omp_set_num_threads(20);
    #pragma omp parallel
    {
        
        #pragma omp for
            for(int i3=0; i3<row_ct; i3++){
                int inner_sum = 0;
                for(int e3=0; e3<col_ct; e3++){
                    inner_sum += arr[i3][e3];
                }
                #pragma omp critical
                    sum2 += inner_sum;
            }
        
    }
    printf("the sum total is: %e\n\n", sum2);


    
        double sumz = 0.0;
    #pragma omp parallel
    {
        #pragma omp for reduction(+:sumz)
            for(int i = 0; i<row_ct; i++){
                for(int e = 0; e < col_ct; e++){
                    sumz = sumz + arr[i][e];
                }
            }

    }        

    printf("the sum zzzzzzzzzzztotal is: %e\n\n", sumz);
    
    






}