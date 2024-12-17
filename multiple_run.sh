#!/bin/bash

# Compile both serial and parallel programs
echo "Running make..."
if make; then
  echo "Compilation successful."
else
  echo "Error: Compilation failed."
  exit 1;
fi

# Find the executables
serial_program="./programa_serie"
parallel_program="./programa_paralelo"

# Verify they exist and they are executable
echo "Checking executables..."
if [ ! -x "$serial_program" ]; then
  echo "Error: $serial_program does not exist or is not executable."
  exit 1
fi
if [ ! -x "$parallel_program" ]; then
  echo "Error: $parallel_program does not exist or is not executable."
  exit 1
fi

# Creating the table
date_now=$(date '+%Y-%m-%d_%H-%M-%S')
table="results${2}_$(printenv OMP_SCHEDULE)_${date_now}.csv"

# Create the headers of the table
echo "OMP_NUM_THREADS,T_irakurtzea,T_sailkatzea,T_eritasunak,T_idaztea,T_osoa" >> $table
 
possible_thread_num=(1 2 4 8 16 24 32 48 64)

# Print number of CPUs
CPUs=$(lscpu -e=CPU | tail -n 1)
((CPUs+=1))
echo "The device has ${CPUs} CPUs."

# Log the scheduling used
echo "Scheduling set to $(printenv OMP_SCHEDULE)"

# Custom number of runs
default_num_runs=1
if [ -z "$1" ]; then
  echo "Number of runs not stated, setting to default: ${default_num_runs}."
  echo ""
  num_of_runs=$default_num_runs
else
  # Check number of runs is a valid integer
  if [[ "$1" =~ ^[0-9]+$ ]]; then
    echo "Number of runs set to $1."
    echo ""
    num_of_runs=$1
  else
    echo "Usage: bash $0 [number_of_runs] [number_of_data]"
    exit 1
  fi
fi

for i in ${possible_thread_num[@]}; do
  for count in $(seq 1 $num_of_runs); do

    # Get the current execution ordinal
    count_nth=$count
    case "$count" in
      *1) count_nth+="st" ;;
      *2) count_nth+="nd" ;;
      *3) count_nth+="rd" ;;
      *)  count_nth+="th" ;;
    esac      

    if [[ $i -eq 1 ]]; then
      # Execute serial program
      echo "Executing serial version for the ${count_nth} time."
      output=$(./programa_serie ./dbgen.dat ./dberi.dat $2)
    else
      # Set the number of threads
      export OMP_NUM_THREADS=$i
      # Execute parallel program
      echo "Executing parallel version with $i threads for the ${count_nth} time."
      output=$(./programa_paralelo ./dbgen.dat ./dberi.dat $2)
    fi

    # Declare an array to store times
    times=()
    # Process each line in the output
    while read -r line; do
      # Match lines that contain the time (e.g., T_irakurtzea:  0.041s)
      if [[ "$line" =~ ^T_.*[0-9]+\.[0-9]+ ]]; then
        # Extract the numeric value (time) from the line
        time=$(echo "$line" | awk '{print $2}')
        # Add the extracted time to the array
        times+=("$time")
      fi
    done <<< "$output"

    echo "$i,${times[0]},${times[1]},${times[2]},${times[3]},${times[4]}" >> "$table"
  done
  echo ""
done

echo "Creating the table..."
echo "$table successfully created."
echo "Finished the process."
