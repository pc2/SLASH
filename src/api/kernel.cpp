#include "api/kernel.hpp"
#include "api/device.hpp"

namespace vrt {

    Kernel::Kernel(ami_device* device, const std::string& name, uint64_t baseAddr, uint64_t range, const std::vector<Register>& registers) {
        this->dev = device;
        this->name = name;
        this->baseAddr = baseAddr;
        this->range = range;
        this->registers = registers;
    }

    Kernel::Kernel(Device device, const std::string& kernelName)
        : Kernel(device.getKernel(kernelName)) {
            deviceBdf = device.getBdf();
            this->platform = device.getPlatform();
            this->server = device.getZmqServer();
        }

    void Kernel::write(uint32_t offset, uint32_t value) {
        if(platform == Platform::HARDWARE) {
            utils::Logger::log(utils::LogLevel::DEBUG, __PRETTY_FUNCTION__, "Writing to device {} kernel: {} at offset: {x} value: {x}", deviceBdf, name, offset, value);
            uint32_t* buf = (uint32_t*) calloc(1, sizeof(uint32_t));
            *buf = value;
            if(buf) {
                // TODO: add bar from device....
                int ret = ami_mem_bar_write(dev, bar, baseAddr - BASE_BAR_ADDR + offset, buf[0]);
                if(ret != AMI_STATUS_OK) {
                    throw std::runtime_error("Failed to write to device");
                }
            }
            free(buf);
        }
    }

    uint32_t Kernel::read(uint32_t offset) {
        if(platform == Platform::HARDWARE) {
            //usleep(1000000); // sometimes sync takes some time
            if(offset != 0)
                utils::Logger::log(utils::LogLevel::DEBUG, __PRETTY_FUNCTION__, "Reading from device {} kernel: {} at offset: {x}", deviceBdf, name, offset);
            uint32_t* buf = (uint32_t*) calloc(1, sizeof(uint32_t));
            if(buf) {
                int ret = ami_mem_bar_read(dev, bar, baseAddr - BASE_BAR_ADDR + offset, &buf[0]);
                if(ret != AMI_STATUS_OK) {
                    throw std::runtime_error("Failed to read from device");
                }
            }
            uint32_t value = buf[0];
            free(buf);
            return value;
        } else if(platform == Platform::EMULATION) {
            currentRegisterIndex = 4;
            std::size_t argIdx = 0;
            while(currentRegisterIndex < registers.size()) {
                std::regex re(".*_\\d+$");
                if(std::regex_match(registers.at(currentRegisterIndex).getRegisterName(), re)) {
                    currentRegisterIndex+=2;
                } else {
                    if(registers.at(currentRegisterIndex).getOffset() == offset) {
                        return server->fetchScalar(name, "arg" + std::to_string(argIdx));
                    }
                    currentRegisterIndex++;
                }
                argIdx++;
            }
            // server->fetchScalar(name, "arg" + std::to_string(currentRegisterIndex - ));
        }
        return 0;
    }

    void Kernel::setDevice(ami_device* device) {
        this->dev = device;
    }

    void Kernel::wait() {
        if(platform == Platform::EMULATION) {
            return;
        }
        while(read(0x00) != 4) {
            //usleep(1);
            // wait for the kernel to finish
        }

    }

    // void Kernel::wait() {
    //     // uint32_t val = read(0x00); // read status register
    //     // std::cout << "Val: " << std::hex << val << std::endl;
    //     // while(((val >> 1) & 0x01) == 0) {
    //     //     // wait for the kernel to finish
    //     //     val = read(0x00);
    //     //     std::cout << "Val: " << std::hex << val << std::endl;
    //     // }

    // }

    void Kernel::startKernel(bool autorestart) {
        if(autorestart) {
            write(0x00, 0x81);
        } else {
            write(0x00, 0x01);
        }
    }

    Kernel::~Kernel() {
        // if(dev != nullptr) {
        //     ami_dev_delete(&dev);
        // }
        //ami_dev_delete(&dev);
    }

    void Kernel::setPlatform(Platform platform) {
        this->platform = platform;
    }

    std::string Kernel::getName() const {
        return name;
    }

} // namespace vrt
