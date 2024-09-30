# HPC Assignment 2: MPI Quicksort Implementation

This directory contains the parallel implementation of the **Quicksort** algorithm using MPI (Message Passing Interface). The goal of this implementation is to leverage multiple processes for efficient sorting of large datasets in a distributed environment.

## Overview

Quicksort is a highly efficient sorting algorithm based on the divide-and-conquer paradigm. In this MPI implementation, the array is partitioned and distributed across multiple processes using MPI, allowing parallel execution of sorting tasks.

## Steps to Compile and Run

### Prerequisites

Ensure that you have the following:
- **MPI Library** (OpenMPI, MPICH, or another compatible MPI implementation).
- Access to the **Orfeo HPC** cluster.

### Allocating Resources on Orfeo

Before running the program, you need to allocate the required resources on Orfeo. Use the following command to allocate one node with 24 tasks for 30 minutes:

```bash
salloc -p THIN --nodes 1 --tasks-per-node=24 --time=00:30:00
```

### Compilation

To compile the MPI Quicksort code, use the following `mpicc` command:

```bash
mpicc -o quicksort_mpi quicksort_mpi.c
```

### Running the Quicksort Program
Once the resources are allocated and the program is compiled, you can run the MPI Quicksort with:

```bash
mpirun -np <number_of_processes> ./quicksort_mpi <length_of_array>
```

## Weak and Strong Scaling Tests

To evaluate the performance of your MPI Quicksort implementation, you can perform both **weak scaling** and **strong scaling** tests. These tests help analyze how well your algorithm scales with the number of processes and input size.
You can run the weak and strong scaling tests using the provided scripts:

- **Weak Scaling**: To run the weak scaling tests, use the script `weak_scaling.sh`:
  ```bash
  ./weak_scaling.sh
  ```
- **Strong Scaling**: To run the storng scaling tests, use the script `strong_scaling.sh`:
  ```bash
  ./strong_scaling.sh
  ```
