#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <ami.h>
#include <ami_mem_access.h>
#include <ami_program.h>
#include <ami_sensor.h>

#include "parser/xml_parser.hpp"
#include "api/vrtbin.hpp"
#include "api/kernel.hpp"
#include "qdma/pcie_driver_handler.hpp"
#include "qdma/qdma_intf.hpp"

#include <map>
#include <fcntl.h>
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

namespace vrt {
    class Device {
        ami_device* dev = nullptr;
        uint8_t bar = 0;
        uint64_t offset = 0;
        uint16_t pci_bdf = 0;
        std::string systemMap;
        std::string bdf;
        std::string pdiPath;
        Vrtbin vrtbin;
        std::map<std::string, Kernel> kernels;
    public:
        Device(const std::string& bdf, const std::string& vrtbinPath, bool program);
        vrt::Kernel getKernel(const std::string& name);
        std::string getBdf();
        void programDevice();
        void sendPcieDriverCmd(std::string cmd);
        void bootDevice();
        void getNewHandle();
        void createAmiDev();
        void destroyAmiDev();
        ~Device();
        void parseSystemMap();
        void cleanup();
    };
} // namespace vrt

#endif // DEVICE_HPP