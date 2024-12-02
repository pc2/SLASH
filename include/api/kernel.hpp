#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <string>
#include <ami.h>
#include <ami_mem_access.h>
#include <stdexcept>

namespace vrt {
    class Kernel {
        ami_device* dev = nullptr;
        uint8_t bar = 0;
        uint64_t offset = 0;
        uint16_t pci_bdf = 0;

    public:
        Kernel(const std::string& bdf);
        void write(uint32_t offset, uint32_t value);
        uint32_t read(uint32_t offset);
        ~Kernel();
    };

} // namespace vrt

#endif // KERNEL_HPP