#!/bin/bash

# weak_scaling.sh
# Arguments: $1 = base_size (the number of elements per process)

if [ $# -lt 1 ]; then
    echo "Usage: $0 <base_size>"
    exit 1
fi

base_size=$1
output_file="weak_scaling_results.txt"

# Clear output file
echo "Weak Scaling Results" > $output_file
echo "Base size per process: $base_size" >> $output_file
echo -e "Processes\tElements\tTime (s)" >> $output_file

echo "Running Weak Scaling Tests..."

# Run for 1 process
t1=$(mpirun -np 1 ./quicksort_mpi $base_size | grep "Sorting completed" | awk '{print $4}')
echo -e "1\t\t$base_size\t\t$t1" >> $output_file

# Loop for multiple processes
for np in 2 4 8 12 16 20 24; do
    total_size=$(($base_size * $np))
    time=$(mpirun -np $np ./quicksort_mpi $total_size | grep "Sorting completed" | awk '{print $4}')
    echo -e "$np\t\t$total_size\t\t$time" >> $output_file
done

echo "Weak scaling results saved to $output_file"
