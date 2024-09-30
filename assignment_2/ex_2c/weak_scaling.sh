#!/bin/bash

# Base parameters for the Mandelbrot problem
nx_base=1000
ny_base=1000
xL=-2
yL=-1.5
xR=1
yR=1.5
Imax=1000

# Output file to save execution times
output_file="weak_scaling_results.txt"

# Clear the output file
echo "Weak Scaling Results" > $output_file
echo "Base problem size: ${nx_base}x${ny_base}, Parameters: xL=$xL, yL=$yL, xR=$xR, yR=$yR, Imax=$Imax" >> $output_file
echo -e "Threads\t\tProblem Size\t\tTime (s)" >> $output_file

# Function to calculate the approximate square root
sqrt() {
  echo | awk "{print int(sqrt($1))}"
}

# Weak Scaling Test
echo "Running Weak Scaling Tests..."
for threads in 1 2 4 8 16 32 64 128; do
    export OMP_NUM_THREADS=$threads

    # Calculate problem size based on the number of threads (square root of total points)
    total_points=$((nx_base * ny_base * threads))
    nx=$(sqrt $total_points)
    ny=$nx

    # Measure execution time and append to the output file
    time=$(srun ./mandelbrot $nx $ny $xL $yL $xR $yR $Imax | grep "Computational time" | awk '{print $3}')
    
    # Append formatted result to the output file
    echo -e "$threads\t\t${nx}x${ny}\t\t$time" >> $output_file
done

echo "Weak scaling results saved to $output_file"

