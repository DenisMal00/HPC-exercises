# HPC Assignment 2: Mandelbrot Parallel Implementation

This directory contains a parallel implementation of the **Mandelbrot set** using OpenMP for multi-threaded computation. The Mandelbrot set is a famous fractal, and this implementation allows for the generation of the fractal using parallel computing to improve performance.

## Overview

The Mandelbrot implementation computes the fractal over a grid of complex numbers. The computation is parallelized using OpenMP to take advantage of multi-core processors. The result is an image where each pixel represents whether a corresponding complex number belongs to the Mandelbrot set.

## Steps to Compile and Run

### Prerequisites

Ensure you have the following:
- **GCC Compiler** with OpenMP support.
- Access to the **Orfeo HPC** cluster.

### Allocating Resources on Orfeo
Before running the program, allocate the necessary resources on the EPYC node, which provides 128 cores. Use the following command to allocate resources:

```bash
salloc -p EPYC --nodes 1 --tasks-per-node=1 --cpus-per-task=128 --time=00:30:00
```

### Compilation
To compile the Mandelbrot program, use the following command:
```bash
gcc -fopenmp -o mandelbrot mandelbrot.c -lm
```
This will generate the executable mandelbrot.

### Running the Mandelbrot Program
Once the resources are allocated and the program is compiled, you can run the Mandelbrot program with the following command:

```bash
./mandelbrot 1000 1000 -2.0 -1.5 1.0 1.5 255
```
This command generates a 1000x1000 pixel image of the Mandelbrot set with the specified bounds:
-1000x1000: Resolution of the output image.
-2.0 -1.5 1.0 1.5: Coordinates of the region of the complex plane to be computed.
-255: Maximum number of iterations for each point.

## Weak and Strong Scaling
You can perform both weak scaling and strong scaling tests to evaluate the parallel performance of the Mandelbrot implementation.

### Weak Scaling
In weak scaling, the problem size increases with the number of cores used.
To run the weak scaling test, use the script weak_scaling.sh:

```bash
./weak_scaling.sh
```

### Strong Scaling
In strong scaling, the problem size remains fixed while the number of cores increases.
To run the strong scaling test, use the script strong_scaling.sh:

```bash
./strong_scaling.sh
```
