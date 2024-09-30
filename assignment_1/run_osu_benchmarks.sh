#!/bin/bash

# Variable for the tar file name
TAR_FILE="osu-micro-benchmarks-7.4.tar.gz"
TAR_DIR="osu-micro-benchmarks-7.4"

# Check if the tar file exists
if [ ! -f "$TAR_FILE" ]; then
  echo "File $TAR_FILE not found!"
  exit 1
fi

# Extract the tar file
echo "Extracting the file $TAR_FILE..."
tar -xzf $TAR_FILE

# Change to the extracted directory
cd $TAR_DIR

# Configure the package with MPI compilers
echo "Configuring the package..."
./configure CC=mpicc CXX=mpicxx

# Compile the benchmarks
echo "Compiling the benchmarks..."
make

# Change to the directory with the executables
cd c/mpi/collective/blocking

# Check if the executables exist
if [ ! -f "osu_bcast" ] || [ ! -f "osu_scatter" ]; then
  echo "Executables osu_bcast or osu_scatter not found!"
  exit 1
fi

# Grant execution permissions to the executables
chmod +x osu_bcast osu_scatter

# Run the broadcast benchmarks and print results
echo "Running broadcast benchmarks..."

# Base broadcast
echo "=== Base Broadcast ==="
mpirun ./osu_bcast -x 100 -i 1000

# Broadcast with algorithm 2 (chain)
echo "=== Broadcast with Algorithm 2 (Chain) ==="
mpirun --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_bcast_algorithm 2 ./osu_bcast -x 100 -i 1000

# Broadcast with algorithm 9 (scatter-allgather ring)
echo "=== Broadcast with Algorithm 9 (Scatter-Allgather Ring) ==="
mpirun --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_bcast_algorithm 9 ./osu_bcast -x 100 -i 1000

echo "Broadcast benchmarks completed."

# Run the scatter benchmarks and print results
echo "Running scatter benchmarks..."

# Base scatter
echo "=== Base Scatter ==="
mpirun ./osu_scatter -x 100 -i 1000

# Scatter with algorithm 1 (basic linear)
echo "=== Scatter with Algorithm 1 (Basic Linear) ==="
mpirun --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_scatter_algorithm 1 ./osu_scatter -x 100 -i 1000

# Scatter with algorithm 3 (non-blocking linear)
echo "=== Scatter with Algorithm 3 (Non-Blocking Linear) ==="
mpirun --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_scatter_algorithm 3 ./osu_scatter -x 100 -i 1000

echo "Scatter benchmarks completed."
