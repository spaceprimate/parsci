# CSCI6454 - HW1

Notes regarding the format and assumptions for HW1


# Files

Files are labeled according to the homework question ID
> 1 - hw1.1.c
> 2 - hw1.2.c
> 3 - hw1.3.c
> 4 - hw1.4.c
> 5 - hw1.5.c
> 6 - hw1.6.c	

## Running the programs

To test, hw1.1.c was compiled using the -lpthread flag. The rest of the programs used the -fopenmp flag. Like so
~~~~
gcc hw1.1.c -o hw1 -lpthread
./hw1
...
gcc hw1.2.c -o hw2 -fopenmp
./hw2
~~~~
Running each program will generate output pertinent to the given question. for example
> pi = 3.141593 in 03000385 secs

## Assumptions

No user input is required. All options and settings required by each question are programed directly into each file. To changes to thread counts and other variables used for computation comparisons were done by manually editing the file- for example.

~~~
#define  NUM_THREADS  6
~~~