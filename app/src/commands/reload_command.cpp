#include "commands/reload_command.hpp"

ReloadCommand::ReloadCommand(const std::string& device) : device(device) {}

void ReloadCommand::execute() {
    PcieDriverHandler driver(device + ":00.0");
    std::cout << "Reloading device " << device << ":00.0" << std::endl;
    driver.execute(PcieDriverHandler::Command::REMOVE);
    driver.execute(PcieDriverHandler::Command::RESCAN);
    driver.execute(PcieDriverHandler::Command::HOTPLUG);
    std::cout << "Device " << device << ":00.0 reloaded" << std::endl;
}