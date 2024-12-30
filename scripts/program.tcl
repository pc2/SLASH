# Check if the correct number of arguments is provided
if { $argc != 1 } {
    puts "Usage: program.tcl <program_file>"
    exit 1
}

# Get the program file from the arguments
set program_file [lindex $argv 0]

open_hw_manager
connect_hw_server -allow_non_jtag
open_hw_target
current_hw_device [get_hw_devices xcv80_1]
refresh_hw_device -update_hw_probes false [lindex [get_hw_devices xcv80_1] 0]

set_property PROBES.FILE {} [get_hw_devices xcv80_1]
set_property FULL_PROBES.FILE {} [get_hw_devices xcv80_1]
set_property PROGRAM.FILE $program_file [get_hw_devices xcv80_1]
program_hw_devices [get_hw_devices xcv80_1]
refresh_hw_device [lindex [get_hw_devices xcv80_1] 0]
close_hw_target
close_hw_manager
exit