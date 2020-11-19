# CSCI6454 - HW3

Notes regarding the format and assumptions for HW3, problem 2

# Files

Files are labeled according to the homework question ID
> 1 - /HW3_DANIEL_MURPHY_1/p2.c

> 2 - /HW3_DANIEL_MURPHY_1/MatC.txt

## Running the program

to compile
~~~~
mpicc -g p2.c -o p2
~~~~

to run
~~~~
mpirun -np 4 p2
~~~~

## Assumptions

No user input is required. All options and settings required by each question are programed directly into each file. The changes to thread counts and other variables used for computation comparisons were done by specifying processor count using the -np flag

~~~
mpirun -np 2 p2
mpirun -np 4 p2
~~~

A default matrix size is set to 2016, but this can be changed by editing the constant: SIZE
~~~
#define SIZE 2016
~~~

To simplify the program, it is asuumed that the matrices MatA and MatB are square. Further, SIZE should be a number divisible by the number of processors used. In this case, we alloted for 1,2,3,and 4 processors for testing purposes.