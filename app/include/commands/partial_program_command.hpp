#ifndef PARTIAL_PROGRAM_COMMAND_HPP
#define PARTIAL_PROGRAM_COMMAND_HPP

#include "arg_parser.hpp"
#include "pcie_hotplug.hpp"
#include "utils/vrtbin.hpp"

#include <ami.h>
#include <ami_mem_access.h>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>
#include <iostream>
#include <string>
#include <ami_program.h>
#include <unistd.h>

/**
 * @brief Delay in microseconds for partial boot process.
 * 
 * This constant defines the delay time in microseconds that the system
 * will wait during the partial boot process.
 */
#define DELAY_PARTIAL_BOOT (4 * 1000 * 1000)

/**
 * @brief Class for handling partial program commands.
 * 
 * The PartialProgramCommand class provides functionality to program a device
 * with a segmented PDI image file.
 */
class PartialProgramCommand {
public:
    /**
     * @brief Constructor for PartialProgramCommand.
     * 
     * @param device The BDF of the device to program.
     * @param image_path Path to the segmented PDI image file.
     */
    PartialProgramCommand(const std::string& device, const std::string& image_path);
    
    /**
     * @brief Executes the partial program command.
     * 
     * This method programs the specified device with the segmented PDI image.
     */
    void execute();

private:
    std::string device;    ///< The BDF of the device to program.
    std::string imagePath; ///< Path to the segmented PDI image file.
    ami_device* dev;       ///< Pointer to the AMI device object.
};

#endif // PARTIAL_PROGRAM_COMMAND_HPP