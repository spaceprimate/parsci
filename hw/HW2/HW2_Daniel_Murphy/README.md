# CSCI6454 - HW2

Notes regarding the format and assumptions for HW2


# Files

Files are labeled according to the homework question ID
> 1 - /HW_DANIEL_MURPHY_1/p1.c

> 2 - /HW_DANIEL_MURPHY_2/p2.c

> 2 - /HW_DANIEL_MURPHY_2/output.txt

## Running the programs

To run, both programs require the -fopenmp flag. Like so
~~~~
gcc p1.c -o p1 -fopenmp
./p1
~~~~

## Assumptions

No user input is required. All options and settings required by each question are programed directly into each file. To changes to thread counts and other variables used for computation comparisons were done by manually editing the file- for example.

~~~
#define  NUM_THREADS  4
~~~