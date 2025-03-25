#ifndef PROGRAM_COMMAND_HPP
#define PROGRAM_COMMAND_HPP

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
 * @brief Enumeration for image types.
 * 
 * This enum represents the different types of images that can be programmed.
 */
enum class ImageType {
    PDI,    ///< PDI (Platform Device Image) type
    VRTBIN, ///< VRTBIN (VRT Binary) type
};

/**
 * @brief Class for handling program commands.
 * 
 * The ProgramCommand class provides functionality to program a device
 * with a specified image file.
 */
class ProgramCommand {
public:
    /**
     * @brief Constructor for ProgramCommand.
     * 
     * @param device The BDF of the device to program.
     * @param image_path Path to the image file.
     * @param partition The partition number to program.
     */
    ProgramCommand(const std::string& device, const std::string& image_path, uint8_t partition);
    
    /**
     * @brief Executes the program command.
     * 
     * This method programs the specified device with the image file.
     */
    void execute();
    
    /**
     * @brief Boots the device.
     * 
     * This method initiates the boot process for the device after programming.
     */
    void bootDevice();

private:
    std::string device;    ///< The BDF of the device to program.
    std::string imagePath; ///< Path to the image file.
    uint8_t partition;     ///< The partition number to program.
    ami_device* dev;       ///< Pointer to the AMI device object.
};

#endif // PROGRAM_COMMAND_HPP