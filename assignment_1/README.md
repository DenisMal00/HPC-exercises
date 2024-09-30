# HPC Assignment 1: OSU Micro-Benchmarks

This directory contains the **OSU Micro-Benchmarks** (OSU) tests for evaluating the performance of communication in parallel systems. The benchmarks help in assessing latency, bandwidth, and MPI performance using a single script.

## Overview

The **OSU Micro-Benchmarks** provide a comprehensive set of tests for evaluating:
- **MPI Collective Operations**: Performance of collective MPI routines like `MPI_Bcast`, `MPI_Scatter`, etc.

These tests are useful for analyzing the performance of high-performance computing (HPC) systems.

## Running the Benchmarks

You can run all the OSU Micro-Benchmarks using the provided script `run_osu_benchmarks.sh`.

### Prerequisites

Make sure you have:
- **MPI Library** (OpenMPI, MPICH, or any compatible MPI implementation): ```bash module load openMPI/4.1.6/gnu/14.2.1```
- Access to the **Orfeo HPC** cluster.

### Allocating Resources on Orfeo

Before running the benchmarks, you need to allocate the required resources on the Orfeo cluster. Use the following command to allocate resources:

```bash
salloc -p THIN --nodes 2 --tasks-per-node=24 --time=00:30:00
```
### Steps to Run the Benchmarks
**Ensure the script is executable**:
If necessary, you can make the script executable by running:
```bash
chmod +x run_osu_benchmarks.sh
```
Then run the script
```bash
./run_osu_benchmarks.sh
```
The script will print the result for the broadcast and scatter tests 
