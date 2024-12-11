#ifndef PCIE_DRIVER_HANDLER_HPP
#define PCIE_DRIVER_HANDLER_HPP

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

namespace vrt {
    class PcieDriverHandler {

    public:
        enum class Command {
            REMOVE,
            TOGGLE_SBR,
            RESCAN,
            HOTPLUG
        };

        static PcieDriverHandler& getInstance();
        void execute(Command cmd);

    private:

        PcieDriverHandler(const PcieDriverHandler&) = delete;
        PcieDriverHandler& operator=(const PcieDriverHandler&) = delete;
        std::string commandToString(Command cmd);
        PcieDriverHandler() {}
    };
}
#endif // PCIE_DRIVER_HANDLER_HPP