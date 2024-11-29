#!/bin/bash

# Define an array of directories containing the .bin files
directories=("$AMKERNEL_HOME/tests/cpu-tests/build" 
            "$EMU_HOME/riscv-tests-am/build" 
            "$EMU_HOME/riscv-arch-test-am/build")

# Initialize an array to hold failed test files
failed_tests=()

# Loop through each directory in the array
for directory in "${directories[@]}"; do
    # Check if the directory exists
    if [ ! -d "$directory" ]; then
        echo "Directory $directory does not exist."
        continue  # Skip to the next directory if this one doesn't exist
    fi

    # Loop through all .bin files in the current directory
    for file in "$directory"/*.bin; do
        # Check if any .bin files exist
        if [ -e "$file" ]; then
            echo "Executing cemu with $file"
            
            # Run the cemu command and capture its output
            output=$(./cemu "-b" "--img=$file")
            
            # Check if the output contains "Good!"
            if [[ "$output" == *"Good!"* ]]; then
                echo -e "Test with $file: \033[0;32mPASS\033[0m"
            else
                echo -e "\e[31mTest with $file: FAIL\e[0m"
                failed_tests+=("$file")  # Store the failed test file
            fi
        else
            echo "No .bin files found in $directory."
        fi
    done
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
