#include "commands/list_command.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

ListCommand::ListCommand(uint16_t vendorId, uint16_t deviceId)
    : vendorId(vendorId), deviceId(deviceId) {}

void ListCommand::execute() const {
    listDevices();
}

void ListCommand::listDevices() const {
    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "Listing V80 devices " << "\n";
    std::cout << "--------------------------------------------------------------------\n";

    for (const auto& entry : std::filesystem::directory_iterator("/sys/bus/pci/devices")) {
        std::string path = entry.path();
        std::ifstream vendorFile(path + "/vendor");
        std::ifstream deviceFile(path + "/device");

        if (vendorFile.is_open() && deviceFile.is_open()) {
            std::string vendorIdStr, deviceIdStr;
            std::getline(vendorFile, vendorIdStr);
            std::getline(deviceFile, deviceIdStr);

            uint16_t vendorId = std::stoi(vendorIdStr, nullptr, 16);
            uint16_t deviceId = std::stoi(deviceIdStr, nullptr, 16);

            if (vendorId == this->vendorId && deviceId == this->deviceId) {
                std::cout << "V80 device found with BDF: " << entry.path().filename().string() << "\n";
                std::cout << "--------------------------------------------------------------------\n";
            }
        }
    }
}