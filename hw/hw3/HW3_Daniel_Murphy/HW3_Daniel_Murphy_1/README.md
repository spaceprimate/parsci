# CSCI6454 - HW3

Notes regarding the format and assumptions for HW3, problem 1

# Files

Files are labeled according to the homework question ID
> 1 - /HW3_DANIEL_MURPHY_1/p1.c

## Running the program

to compile
~~~~
mpicc -g p1.c -o p1
~~~~

to run
~~~~
mpirun -np 4 p1
~~~~

## Assumptions

No user input is required. All options and settings required by each question are programed directly into each file. The changes to thread counts and other variables used for computation comparisons were done by specifying processor count using the -np flag

~~~
mpirun -np 2 p1
mpirun -np 4 p1
~~~