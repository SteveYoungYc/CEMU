#!/bin/bash

# Define an array of directories containing the .bin files
directories=("$AMKERNEL_HOME/tests/cpu-tests/build" 
            "$EMU_HOME/riscv-tests-am/build" 
            "$EMU_HOME/riscv-arch-test-am/build")

# Initialize an array to hold failed test files
failed_tests=()

# Initialize a counter for the tests
test_counter=0

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
            # Increment the test counter
            test_counter=$((test_counter + 1))

            echo "Executing test #$test_counter with $file"
            
            # Run the cemu command and capture its output
            output=$(./cemu "-b" "--img=$file")
            
            # Check if the output contains "Good!"
            if [[ "$output" == *"Good!"* ]]; then
                echo -e "Test #$test_counter: \033[0;32mPASS\033[0m"
            else
                echo -e "\e[31mTest #$test_counter: FAIL\e[0m"
                failed_tests+=("$test_counter: $file")  # Store the failed test with its index
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
        echo -e "\e[31m$failed\e[0m"  # Display in red with test number
    done
else
    echo -e "\e[32mAll tests passed.\e[0m"  # Display in green
fi
