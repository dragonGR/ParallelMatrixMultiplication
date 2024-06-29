# Multithreaded Matrix Multiplication

This project implements a multithreaded matrix multiplication program in C. The program demonstrates proficiency in dynamic memory management, concurrent programming using POSIX Threads (pthread), and computational algorithms.

## Features

- Dynamic allocation of matrices
- Multithreaded computation using pthreads
- Command-line arguments for specifying matrix dimensions and the number of threads
- Randomly filled matrices for demonstration purposes

## Requirements

- GCC

## Usage

### Compilation

To compile the program, run:
```sh
gcc -o matrix_mult matrix_mult.c -pthread
