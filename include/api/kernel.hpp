#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <string>
#include <ami.h>
#include <ami_mem_access.h>
#include <stdexcept>
#include <vector>

#include "register/register.hpp"
//#include "api/device.hpp"

namespace vrt {
    class Device;
    class Kernel {
        static constexpr uint64_t BASE_BAR_ADDR = 0x20100000000;
        uint8_t bar = 0;
        ami_device* dev = nullptr;
        std::string name;
        uint64_t baseAddr;
        uint64_t range;
        std::vector<Register> registers;

    public:
        Kernel(ami_device* device, const std::string& name, uint64_t baseAddr, uint64_t range, std::vector<Register>& registers);
        Kernel() = default;
        
        Kernel(vrt::Device device, const std::string& kernelName);

        void setDevice(ami_device* device);
        void write(uint32_t offset, uint32_t value);
        uint32_t read(uint32_t offset);
        void wait();
        void start(bool autorestart);
        ~Kernel();
    };

} // namespace vrt

#endif // KERNEL_HPP