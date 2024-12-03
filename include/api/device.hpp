#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <ami.h>
#include <ami_mem_access.h>

#include "parser/xml_parser.hpp"

#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "api/kernel.hpp" // Include the Kernel class
namespace vrt {
    class Device {
        ami_device* dev = nullptr;
        uint8_t bar = 0;
        uint64_t offset = 0;
        uint16_t pci_bdf = 0;
        std::string systemMap;
        std::map<std::string, Kernel> kernels;
    public:
        Device(const std::string& bdf, const std::string& systemMap);
        vrt::Kernel getKernel(const std::string& name);
        ~Device();
        void parseSystemMap();
        void cleanup();
    };
} // namespace vrt

#endif // DEVICE_HPP