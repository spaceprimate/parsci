// Adjascency List representation in C

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
// file is 7623??
// #define MAX 7
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

int main()
{
    struct Graph *graph = createAGraph(7623);

    FILE *filePointer;
    int bufferLength = 15;
    char buffer[bufferLength];

    filePointer = fopen("edges.csv", "r");

    // to test, set max to 7
    // addEdge(graph, 0, 1);
    // addEdge(graph, 0, 2);
    // addEdge(graph, 0, 3);
    // addEdge(graph, 1, 4);
    // addEdge(graph, 1, 5);
    // addEdge(graph, 2, 5);
    // addEdge(graph, 4, 6);


    while (fgets(buffer, bufferLength, filePointer))
    {

        char *n = strtok(buffer, ",");
        int i1 = atoi(n);
        char *n2 = strtok(NULL, ",");
        int i2 = atoi(n2);
        // printf("gorsh");
        addEdge(graph, i1, i2);
    }


    // heap is corrupted, can't close this file :(
    // fclose(filePointer);

    printf("balogna sammich");

    bfs(graph, 0);

    return 0;
}