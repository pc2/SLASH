#include <iostream>
#include <cstring> // for std::memcpy
#include <cstdint>

#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>


#include <api/device.hpp>
#include <api/buffer.hpp>
#include <api/kernel.hpp>

int main() {
    try {
        uint32_t size = 4096;
        uint32_t m = 3;
        uint32_t n = 2;
        vrt::Device device("21:00.0", "01_example.vrtbin", true, vrt::ProgramType::FLASH);
        vrt::Kernel dma(device, "dma_0");
        vrt::Kernel offset(device, "offset_0");
        //device.setFrequency(233333333);
        vrt::Buffer<uint32_t> in_buff(device, size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<uint32_t> out_buff(device, size, vrt::MemoryRangeType::HBM);
        for(uint32_t i = 0; i < size; i++) {
            in_buff[i] = 1;
        }
        in_buff.sync(vrt::SyncType::HOST_TO_DEVICE);
        offset.call(size, in_buff.getPhysAddr(), m, n);
        dma.call(size, out_buff.getPhysAddr());
        out_buff.sync(vrt::SyncType::DEVICE_TO_HOST);
        for(uint32_t i = 0; i < size; i++) {
            if(out_buff[i] != in_buff[i] * m + n) {
                std::cerr << "Test failed" << std::endl;
                std::cerr << "Error: " << i << " " << out_buff[i] << " " << in_buff[i] << std::endl;
                device.cleanup();
                return 1;
            }
        }
        std::cout << "Test passed" << std::endl;
        device.cleanup();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } 
    return 0;
}

