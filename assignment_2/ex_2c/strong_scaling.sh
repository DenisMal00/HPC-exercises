#!/bin/bash

# Mandelbrot strong_scaling.sh
# Parameters for running the Mandelbrot program
nx=4000
ny=4000
xL=-2
yL=-1.5
xR=1
yR=1.5
Imax=1000

# Output file to save execution times
output_file="strong_scaling_results.txt"

# Clear the output file
echo "Strong Scaling Results" > $output_file
echo "Parameters: nx=$nx, ny=$ny, xL=$xL, yL=$yL, xR=$xR, yR=$yR, Imax=$Imax" >> $output_file
echo -e "Threads\t\tTime (s)" >> $output_file

# Strong Scaling Test
echo "Running Strong Scaling Tests..."
for threads in 1 2 4 8 16 32 64 128; do
    export OMP_NUM_THREADS=$threads
    # Run the Mandelbrot program and measure execution time
    time=$(srun ./mandelbrot $nx $ny $xL $yL $xR $yR $Imax | grep "Computational time" | awk '{print $3}')
    # Save the number of threads and execution time to the output file
    echo -e "$threads\t\t$time" >> $output_file
done
echo "Strong scaling results saved to $output_file"

