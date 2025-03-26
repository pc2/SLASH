#ifndef RELOAD_COMMAND_HPP
#define RELOAD_COMMAND_HPP

#include <iostream>
#include <string>

#include "pcie_hotplug.hpp"

class ReloadCommand {
public:
    ReloadCommand(const std::string& device);
    void execute();
private:
    std::string device;
};

#endif // RELOAD_COMMAND_HPP