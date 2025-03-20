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

enum class ImageType {
    PDI,
    VRTBIN,
};

class ProgramCommand {
public:
    ProgramCommand(const std::string& device, const std::string& image_path, uint8_t partition);
    void execute();
    void bootDevice();

private:
    std::string device;
    std::string imagePath;
    uint8_t partition;
    ami_device* dev;

};

#endif // PROGRAM_COMMAND_HPP