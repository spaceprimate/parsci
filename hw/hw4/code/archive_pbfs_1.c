// Adjascency List representation in C
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#define MAX 7623

// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// GRAPH DATA STRUCTURES ====================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
struct node
{
    int vertex;
    struct node *next;
};
struct node *createNode(int);

struct Graph
{
    int numVertices;
    struct node **adjLists;
};

// Create a node
struct node *createNode(int v)
{
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Create a graph
struct Graph *createAGraph(int vertices)
{
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = malloc(vertices * sizeof(struct node *));

    int i;
    for (i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;

    return graph;
}

// Add edge
void addEdge(struct Graph *graph, int s, int d)
{
    // Add edge from s to d
    struct node *newNode = createNode(d);
    newNode->next = graph->adjLists[s];
    graph->adjLists[s] = newNode;

    // Add edge from d to s
    newNode = createNode(s);
    newNode->next = graph->adjLists[d];
    graph->adjLists[d] = newNode;
}

// Print the graph
void printGraph(struct Graph *graph)
{
    int v;
    for (v = 0; v < graph->numVertices; v++)
    {
        struct node *temp = graph->adjLists[v];
        printf("\n Vertex %d\n: ", v);
        while (temp)
        {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

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

// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// BREADTH FIRST SEARCH =====================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
int bfs(struct Graph *graph, int s)
{
    int i;
    int size = graph->numVertices;
    int d[MAX];

    for (i = 0; i < MAX; i++)
    {
        d[i] = -1;
    }
    struct Stack *FS = createStack(100); // frontier stack, we'll probably need to make this bigger
    struct Stack *NS = createStack(100); // next stack
    push(FS, s);
    d[s] = 0;
    int level = 0;

    printf("visited vertex: %d\n", s);

    // printf("start----------------------------------------------\n");
    while (!isEmpty(FS))
    {
        
        // printf("it wasn't empty");
        int u = pop(FS);
        level = d[u] + 1;
        // printf("parent: %d\n", u);
        struct node *temp = graph->adjLists[u];
        // struct node* temp2 = graph->adjLists[u]->next;
        // struct node* temp3 = graph->adjLists[u]->next->next;
        // struct node* temp4 = graph->adjLists[u]->next->next->next;

        while (temp != NULL)
        {
            if (d[temp->vertex] == -1)
            {
                push(NS, temp->vertex);
                d[temp->vertex] = level;
                printf("visited vertex: %d at level: %d\n", temp->vertex, level);
            }
            temp = temp->next;
        }

        // FS = NS
        // NS = {}
        while (!isEmpty(NS))
        {
            int t = pop(NS);
            push(FS, t);
        }

        
    }
}


// PARALLEL IMPLEMENTATION
// FS = frontier stack
// NS = next stack
int pbfs(struct Graph *graph, int s, int proc_size, int rank)
{
    int i;
    int size = graph->numVertices;
    int d[MAX];     // depth array
    int barrier = 0;
    int globalIsEmpty = 0;
    int localIsEmpty = 1;
    int res; // used to store barrier results

    for (i = 0; i < MAX; i++)
    {
        d[i] = -1;
    }
    struct Stack *FS = createStack(100); // frontier stack, we'll probably need to make this bigger
    struct Stack *NS = createStack(100); // next stack


    // parallel
    // this should only allow 1 process to push an initial value to FS
    if(s % proc_size == rank){
        push(FS, s);
        localIsEmpty = 0;
        printf("visited vertex: %d\n", s);    
    }
    // ep

    d[s] = 0;

    
    int level = 0;

    

    // printf("start----------------------------------------------\n");
    // while (!isEmpty(FS))
    while (!globalIsEmpty)
    {
        if(!localIsEmpty){

        
            // printf("it wasn't empty");
            int u = pop(FS);
            level = d[u] + 1;
            // printf("parent: %d\n", u);
            struct node *temp = graph->adjLists[u];
            // struct node* temp2 = graph->adjLists[u]->next;
            // struct node* temp3 = graph->adjLists[u]->next->next;
            // struct node* temp4 = graph->adjLists[u]->next->next->next;

            while (temp != NULL)
            {
                if (d[temp->vertex] == -1)
                {



                    
                    

                    











  










                    push(NS, temp->vertex);
                    d[temp->vertex] = level;
                    // printf("visited vertex: %d at level: %d\n", temp->vertex, level);

                    printf("FS stack top is: %d\n",FS->top);
                    printf("NS stack top is: %d\n",NS->top);
                }
                temp = temp->next;
            }

            // FS = NS
            // NS = {}
            // parallel new
            // all send to proc 0
            

        }


        



        //=============================== new =================================================
        // new!!
                    // local and global vertices pushed to ns, a fair amount of parallel here

                    
                    int dest[25*proc_size]; // this will receive nodes from all procs
    

                    
                    i = 0;
                    
                    // we need to know how many nodes will be passed in each proc's stack
                    int fs_size = FS->top;  // size of this stack
                    int fs_sizes[proc_size]; // to store stack size for




                    res = MPI_Barrier (MPI_COMM_WORLD); // procs wait

                   
                    MPI_Allgather(FS->array, 25, MPI_INT, dest, 25, MPI_INT, MPI_COMM_WORLD);
                    res = MPI_Barrier (MPI_COMM_WORLD); // procs wait
                    MPI_Allgather(&fs_size, 1, MPI_INT, fs_sizes, 1, MPI_INT, MPI_COMM_WORLD);

                    int p = 0;

                    int z = 0;

                    for(p=0; p< proc_size; p++){
                        printf("size: %d\n", fs_sizes[p]);
                        for(i = 0; i <= fs_sizes[p]; i++){
                            z = dest[p*25 + i];
                            if(d[z] == -1){
                                push(NS, z);
                                d[z] = level;
                            }
                            // printf("value: %d, from rank: %d\n", dest[p*25 + i], rank);
                            z++;
                        }
                    }
                    printf("%d", z);





        //=============================== end new =================================================


        while (!isEmpty(NS))
            {

                // todo: push local items only
                

                int t = pop(NS);
                push(FS, t);
            }









        // parallel
        int res;
        res = MPI_Barrier (MPI_COMM_WORLD); // procs wait
        // calculate each localIsEmpty, using isEmpty function
        localIsEmpty = isEmpty(FS);
        printf("304: localisempty: %d, rank: %d\n", localIsEmpty, rank);


        // reduce to proc 1, setting globalIsEmpty
        MPI_Reduce(&localIsEmpty, &globalIsEmpty, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if(rank == 0){
            if(globalIsEmpty == proc_size){
                globalIsEmpty = 1;
            }
            else{
                globalIsEmpty = 0;
            }
        }

        // broadcast globalIsEmpty
        res = MPI_Barrier (MPI_COMM_WORLD); // procs wait
        MPI_Bcast( &globalIsEmpty, 1, MPI_INT, 0, MPI_COMM_WORLD);



        
    }
}

int main( int argc, char *argv[] )
{
    // initial setup
    int rank, proc_size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &proc_size );



    int graph_size = 7623;

    int i = 0; // index
    int sub_graph_offset = graph_size % proc_size;
    int sub_graph_size = (graph_size - sub_graph_offset) / proc_size;
    if(rank == 0){
        sub_graph_size = sub_graph_size + sub_graph_offset;
    }

    struct Graph *graph = createAGraph(graph_size);

    

    FILE *filePointer;
    int bufferLength = 15;
    char buffer[bufferLength];

    filePointer = fopen("edges.csv", "r");

    while (fgets(buffer, bufferLength, filePointer))
    {

        char *n = strtok(buffer, ",");
        int i1 = atoi(n);
        char *n2 = strtok(NULL, ",");
        int i2 = atoi(n2);

        addEdge(graph, i1, i2);
        
        // if (i1 % proc_size == rank){
        //     int index = i1 / proc_size + rank;
        //     addEdge(graph, index, i2);
        // }
        
    }

    // fclose(filePointer);

    pbfs(graph, 0, proc_size, rank);

    printf("p is rank: %d\n", rank);

    MPI_Finalize();

    return 0;
}