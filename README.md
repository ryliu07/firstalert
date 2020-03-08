# 4F03_Assignment_3

Assignment 3 is designed to investigate the impact of communication and cache on a problem with MPI using Matrix-Matrix multiplication.

## Specifications

- The assignment is to be completed in C using MPI.
- The program accepts the following parameters: ./assig3 n m d
- Use drand48 as the random number generator, only root shall generate data.
- Root processor must send the required blocks of data to each node.
- The program will use mpirun to specify the number of cores to use.
- Use MPI Wtime() for timing, ensure that only the computation is timed, not I/O or data generation.
- The program should output the time elapsed. Starting from when the root processor sent the information.
- Please ensure calculation is correct during development by comparing to serial version.

## Variables

- n = Number of rows of the matrix A
- m = Number of columns of the matrix A
- d = Type of major scheme during computation: 0 Row-Major, 1 Column-Major
- r = The number of nodes used to execute the program

## Makefile

- Make the program 
```bash
make 
```

- Clean the program 
```bash
make clean
```

- Run the program
```bash
make run node=r row=n col=m type=d
```


