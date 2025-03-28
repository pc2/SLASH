#ifndef RELOAD_COMMAND_HPP
#define RELOAD_COMMAND_HPP

#include <iostream>
#include <string>

#include "pcie_hotplug.hpp"

/**
 * @brief Class for handling device reload commands.
 *
 * The ReloadCommand class provides functionality to reload a specified FPGA device
 * by managing the PCIe connection through a hot-plug cycle without performing a complete
 * hardware reset. This is useful for refreshing the device state or recovering from
 * certain error conditions.
 */
class ReloadCommand {
   public:
    /**
     * @brief Constructor for ReloadCommand.
     *
     * @param device The BDF (Bus:Device.Function) identifier of the device to reload.
     */
    ReloadCommand(const std::string& device);

    /**
     * @brief Executes the reload command.
     *
     * This method performs the device reload sequence, which typically involves
     * removing the device from the PCIe bus and then rescanning to rediscover it,
     * effectively reestablishing the connection without a full hardware reset.
     */
    void execute();

   private:
    std::string device;  ///< The BDF identifier of the device to reload.
};

#endif  // RELOAD_COMMAND_HPP