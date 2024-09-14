#!/bin/bash

# Define the directory containing the .bin files
directory="/home/chen/emu/am/am-kernels/tests/cpu-tests/build"

# Check if the directory exists
if [ ! -d "$directory" ]; then
    echo "Directory $directory does not exist."
    exit 1
fi

# Loop through all .bin files in the specified directory
for file in "$directory"/*.bin; do
    # Check if any .bin files exist
    if [ -e "$file" ]; then
        echo "Executing cemu with $file"
        
        # Run the cemu command and capture its output
        output=$(./cemu "-b" "$file")
        
        # Check if the output contains "CEMU Trap!"
        if [[ "$output" == *"CEMU Trap!"* ]]; then
            echo "Test with $file: PASS"
        else
            echo "Test with $file: FAIL"
        fi
    else
        echo "No .bin files found in $directory."
        break
    fi
done
