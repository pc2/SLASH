#ifndef PCIE_DRIVER_HANDLER_HPP
#define PCIE_DRIVER_HANDLER_HPP

#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

#include "utils/logger.hpp"
namespace vrt {
/**
 * @brief Class for handling PCIe driver commands.
 */
class PcieDriverHandler {
   public:
    /**
     * @brief Enum for PCIe driver commands.
     */
    enum class Command {
        REMOVE,      ///< Remove command
        TOGGLE_SBR,  ///< Toggle Secondary Bus Reset command
        RESCAN,      ///< Rescan command
        HOTPLUG      ///< Hotplug command
    };

    /**
     * @brief Constructor for PcieDriverHandler.
     * @param bdf The BDF of the PCIe device.
     */
    PcieDriverHandler(const std::string& bdf);

    /**
     * @brief Sends a command to the PCIe driver.
     * @param cmd The command to send.
     */
    void sendCommand(Command cmd);

    /**
     * @brief Executes a PCIe driver command.
     * @param cmd The command to execute.
     */
    void execute(Command cmd);

   private:
    /**
     * @brief Helper method to convert enum to string.
     * @param cmd The command to convert.
     * @return The string representation of the command.
     */
    std::string commandToString(Command cmd);
    std::string bdf;                                        ///< The BDF of the PCIe device.
    std::string driverPath;                                 ///< The path to the PCIe driver.
    std::string pcieHotplugRootPath = "/dev/pcie_hotplug";  ///< The root path for PCIe hotplug.
};

}  // namespace vrt

#endif  // PCIE_DRIVER_HANDLER_HPP