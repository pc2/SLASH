#include "commands/reset_command.hpp"

ResetCommand::ResetCommand(const std::string& device) : device(device) {
    this->dev = nullptr;
    if (ami_dev_find(device.c_str(), &dev) != AMI_STATUS_OK) {
        std::cerr << "Error finding ami device: " << device << std::endl;
        throw std::runtime_error("Error finding device");
    }

    if (ami_dev_request_access(dev) != AMI_STATUS_OK) {
        throw std::runtime_error("Failed to request elevated access to device");
    }
}

void ResetCommand::execute() {
    PcieDriverHandler pcieDriverHandler(device + ":00.0");

    int ret = ami_prog_device_boot(&dev, 1);  // segmented PDI is on partition 1

    if (ret != AMI_STATUS_OK && geteuid() == 0) {
        throw std::runtime_error("Error booting device to partition 1");
    }

    ami_mem_bar_write(dev, 0, 0x1040000, 1);
    ami_dev_delete(&dev);
    pcieDriverHandler.execute(PcieDriverHandler::Command::REMOVE);
    pcieDriverHandler.execute(PcieDriverHandler::Command::TOGGLE_SBR);
    pcieDriverHandler.execute(PcieDriverHandler::Command::RESCAN);
    pcieDriverHandler.execute(PcieDriverHandler::Command::HOTPLUG);

    if (ami_dev_find(device.c_str(), &dev) != AMI_STATUS_OK) {
        std::cerr << "Error finding ami device: " << device << std::endl;
        throw std::runtime_error("Error finding device");
    }

    std::cout << "Device resetted successfully" << std::endl;
    ami_dev_delete(&dev);
}