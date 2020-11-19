#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// STACK STRUCTURES =========================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// from here: https://www.geeksforgeeks.org/stack-data-structure-introduction-program/

// example usage:
// struct Stack* stack = createStack(100);

//     push(stack, 10);
//     push(stack, 20);
//     push(stack, 30);

//     printf("%d popped from stack\n", pop(stack));

// A structure to represent a stack
struct Stack
{
    int top;
    unsigned capacity;
    int *array;
};

// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack *createStack(unsigned capacity)
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
    return stack;
}

// Stack is full when top is equal to the last index
int isFull(struct Stack *stack)
{
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmpty(struct Stack *stack)
{
    return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(struct Stack *stack, int item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
    // printf("%d pushed to stack\n", item);
}

// Function to remove an item from stack.  It decreases top by 1
int pop(struct Stack *stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}

// Function to return the top from stack without removing it
int peek(struct Stack *stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}



int main( int argc, char *argv[] )
{
    // preliminaries / setting up environment
    int rank, proc_size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &proc_size );

    struct Stack *FS = createStack(100);

    int res;

    push(FS, rank);
    push(FS, rank * 2);
    push(FS, rank * 2);
    // printf("fs array test: %d\n", FS->array[0]);

    int i = 0;
    // pop the stack into an array, then send the array
    int dest[25*proc_size];
    // int fa[25];
    // for(i= 0; i< 25; i++){
    //     fa[i]=-1;
    // }
    
    i = 0;
    int fs_size = FS->top;
    int fs_sizes[proc_size];
    // while(!isEmpty(FS)){
    //     fa[i] = pop(FS);
    //     i++;
    // }

    



    MPI_Allgather(FS->array, 25, MPI_INT, dest, 25, MPI_INT, MPI_COMM_WORLD);
    res = MPI_Barrier (MPI_COMM_WORLD); // procs wait
    MPI_Allgather(&fs_size, 1, MPI_INT, fs_sizes, 1, MPI_INT, MPI_COMM_WORLD);

    int p = 0;

    
    for(p=0; p< proc_size; p++){

        printf("size: %d\n", fs_sizes[p]);
        for(i = 0; i <= fs_sizes[p]; i++){
            printf("value: %d, from rank: %d\n", dest[p*25 + i], rank);
        }
    }

    // for(i = 0; i < proc_size; i++){
    //     // printf("procsizes: %d\n", fs_sizes[i]);
    // }


    // i = 0;
    // while(dest[i] != -1){
    // // for(i = 0; i < 100; i++){
    //     push(FS, dest[i]);
    //     printf("rank: %d, val: %d\n", rank, dest[i]);
    //     i++;
    // }

    // printf("test: %d\n", *dest[1]);
    // printf("test: %d\n", dest[2]);
    // printf("test: %d\n", dest[3]);
    // printf("test: %d\n", dest[4]);
    



    

    MPI_Finalize();

    return 0;
}