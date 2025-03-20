#ifndef PARTIAL_PROGRAM_COMMAND_HPP
#define PARTIAL_PROGRAM_COMMAND_HPP

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

#define DELAY_PARTIAL_BOOT (4 * 1000 * 1000)


class PartialProgramCommand {
public:
    PartialProgramCommand(const std::string& device, const std::string& image_path);
    void execute();

private:
    std::string device;
    std::string imagePath;
    ami_device* dev;
};

#endif // PARTIAL_PROGRAM_COMMAND_HPP