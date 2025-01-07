#include "qdma/pcie_driver_handler.hpp"

namespace vrt{

    PcieDriverHandler::PcieDriverHandler(const std::string& bdf) {
        this->bdf = bdf;
        utils::Logger::log(utils::LogLevel::DEBUG,__PRETTY_FUNCTION__, "Creating PCIe Hotplug Driver with BDF: {}", bdf);
        driverPath = pcieHotplugRootPath + "_0000:" + bdf;
    }

    void PcieDriverHandler::execute(Command cmd) {
        utils::Logger::log(utils::LogLevel::DEBUG,__PRETTY_FUNCTION__, "Executing command: {} for PCIe device {}", commandToString(cmd), bdf);
        std::string cmdStr = commandToString(cmd);
        int fd = open(driverPath.c_str(), O_WRONLY);
        if (fd < 0) {
            throw std::runtime_error("Could not open device");
        }

        if (write(fd, cmdStr.c_str(), cmdStr.size()) < 0) {
            close(fd);
            throw std::runtime_error("Could not write to device " + driverPath);
        }
        close(fd);
    }

    std::string PcieDriverHandler::commandToString(Command cmd) {
        switch (cmd) {
            case Command::REMOVE: return "remove";
            case Command::TOGGLE_SBR: return "toggle_sbr";
            case Command::RESCAN: return "rescan";
            case Command::HOTPLUG: return "hotplug";
            default: throw std::invalid_argument("Invalid command");
        }
    }
}