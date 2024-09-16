#!/bin/bash

# Define the directory containing the .bin files
directory="/home/chen/emu/am/am-kernels/tests/cpu-tests/build"

# Check if the directory exists
if [ ! -d "$directory" ]; then
    echo "Directory $directory does not exist."
    exit 1
fi

# Initialize an array to hold failed test files
failed_tests=()

# Loop through all .bin files in the specified directory
for file in "$directory"/*.bin; do
    # Check if any .bin files exist
    if [ -e "$file" ]; then
        echo "Executing cemu with $file"
        
        # Run the cemu command and capture its output
        output=$(./cemu "-b" "--img=$file")
        
        # Check if the output contains "CEMU Trap!"
        if [[ "$output" == *"Good!"* ]]; then
            echo "Test with $file: PASS"
        else
            echo -e "\e[31mTest with $file: FAIL\e[0m"
            failed_tests+=("$file")  # Store the failed test file
        fi
    else
        echo "No .bin files found in $directory."
        break
    fi
done

# Display failures in red at the end
if [ ${#failed_tests[@]} -ne 0 ]; then
    echo -e "\nFailed Tests:"
    for failed in "${failed_tests[@]}"; do
        echo -e "\e[31m$failed\e[0m"  # Display in red
    done
else
    echo -e "\e[32mAll tests passed.\e[0m"  # Display in green
fi
