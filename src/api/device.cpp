#include "api/device.hpp"

namespace vrt {

    Device::Device(const std::string& bdf, const std::string& systemMap) {
        this->systemMap = systemMap;
        if(ami_dev_find(bdf.c_str(), &dev) != AMI_STATUS_OK) {
            throw std::runtime_error("Failed to find device");
        }
        ami_dev_get_pci_bdf(dev, &pci_bdf);
        if(ami_dev_request_access(dev) != AMI_STATUS_OK) {
            throw std::runtime_error("Failed to request elevated access to device");
        }

        parseSystemMap();
    }

    Device::~Device() {
        
    }

    void Device::parseSystemMap() {
        XMLParser parser(systemMap);
        parser.parseXML();
        kernels = parser.getKernels();
        for(auto& kernel : kernels) {
            kernel.second.setDevice(dev);
        }
    }
    Kernel Device::getKernel(const std::string& name) {
        return kernels[name];
    }

    void Device::cleanup() {
        ami_dev_delete(&dev);
    }
} // namespace vrt