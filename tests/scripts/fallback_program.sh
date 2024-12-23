#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <program_file.pdi>"
    exit 1
fi

# Get the program file from the arguments
program_file=$1

vivado -mode batch -source /scratch/users/aulmamei/git/vrt-api/tests/scripts/program.tcl -tclargs "$program_file"
