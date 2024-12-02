#include "api/kernel.hpp"

namespace vrt {

    Kernel::Kernel(const std::string& bdf) {
        if(ami_dev_find(bdf.c_str(), &dev) != AMI_STATUS_OK) {
            throw std::runtime_error("Failed to find device");
        }
        ami_dev_get_pci_bdf(dev, &pci_bdf);
    }

    void Kernel::write(uint32_t offset, uint32_t value) {
        uint32_t* buf = (uint32_t*) calloc(1, sizeof(uint32_t));
        *buf = value;
        if(buf) {
            int ret = ami_mem_bar_write(dev, bar, offset, buf[0]);
            if(ret != AMI_STATUS_OK) {
                throw std::runtime_error("Failed to write to device");
            }
        }
        free(buf);
    }

    uint32_t Kernel::read(uint32_t offset) {
        uint32_t* buf = (uint32_t*) calloc(1, sizeof(uint32_t));
        if(buf) {
            int ret = ami_mem_bar_read(dev, bar, offset, &buf[0]);
            if(ret != AMI_STATUS_OK) {
                throw std::runtime_error("Failed to read from device");
            }
        }
        uint32_t value = buf[0];
        free(buf);
        return value;
    }

    Kernel::~Kernel() {
        ami_dev_delete(&dev);
    }

} // namespace vrt