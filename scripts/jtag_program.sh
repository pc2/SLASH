#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <program_file.pdi>"
    exit 1
fi

# Get the program file from the arguments
program_file=$1

vivado -nolog -nojournal -mode batch -source /usr/local/vrt/program.tcl -tclargs "$program_file"
