#!/bin/bash

# strong_scaling.sh
# Arguments: $1 = total_size (number of elements for all processes)

if [ $# -lt 1 ]; then
    echo "Usage: $0 <total_size>"
    exit 1
fi

total_size=$1
output_file="strong_scaling_results.txt"

# Clear output file
echo "Strong Scaling Results" > $output_file
echo "Total size: $total_size" >> $output_file
echo -e "Processes\tTime (s)" >> $output_file

echo "Running Strong Scaling Tests..."

t1=$(mpirun -np 1 ./quicksort_mpi $total_size | grep "Sorting completed" | awk '{print $4}')
echo -e "1\t\t$t1" >> $output_file

for np in 2 4 8 12 16 20 24; do
    time=$(mpirun -np $np ./quicksort_mpi $total_size | grep "Sorting completed" | awk '{print $4}')
    echo -e "$np\t\t$time" >> $output_file
done

echo "Strong scaling results saved to $output_file"
