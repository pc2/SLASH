#ifndef RESET_COMMAND_HPP
#define RESET_COMMAND_HPP

#include <ami.h>
#include <ami_mem_access.h>
#include <ami_program.h>

#include <exception>
#include <iostream>
#include <string>

#include "pcie_hotplug.hpp"

/**
 * @brief Class for handling device reset commands.
 *
 * The ResetCommand class provides functionality to reset a specified FPGA device
 * by managing the device's PCIe connection and hardware reset sequence.
 */
class ResetCommand {
   public:
    /**
     * @brief Constructor for ResetCommand.
     *
     * @param device The BDF (Bus:Device.Function) identifier of the device to reset.
     */
    ResetCommand(const std::string& device);

    /**
     * @brief Executes the reset command.
     *
     * This method performs the device reset sequence, which includes
     * properly shutting down the PCIe connection, performing the hardware
     * reset, and re-establishing the connection if necessary.
     */
    void execute();

   private:
    std::string device;  ///< The BDF identifier of the device to reset.
    ami_device* dev;     ///< Pointer to the AMI device object.
};

#endif  // RESET_COMMAND_HPP