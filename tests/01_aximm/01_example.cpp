#include <iostream>
#include <cstring> // for std::memcpy
#include <cstdint>

#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>


#include "../../include/api/device.hpp"
#include "../../include/api/buffer.hpp"
#include "../../include/api/kernel.hpp"

int main() {
    try {
        uint32_t size = 1000;
        vrt::Device device("21:00.0", "01_example.vrtbin", true);
        vrt::Kernel dma(device, "dma_0");
        vrt::Kernel offset(device, "offset_0");
        

        vrt::Buffer<uint32_t> in_buff(size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<uint32_t> out_buff(size, vrt::MemoryRangeType::HBM);
        for(uint32_t i = 0; i < size; i++) {
            in_buff[i] = 1;
        }
        in_buff.sync(vrt::SyncType::HOST_TO_DEVICE);

        offset.write(0x10, size);
        offset.write(0x18, in_buff.getPhysAddrLow());
        offset.write(0x1c, in_buff.getPhysAddrHigh());
        offset.write(0x24, 3);
        offset.write(0x2c, 2);
        dma.write(0x10, size);
        dma.write(0x18, out_buff.getPhysAddrLow());
        dma.write(0x1c, out_buff.getPhysAddrHigh());
        offset.start(false);
        dma.start(false);
        offset.wait();
        dma.wait();
        out_buff.sync(vrt::SyncType::DEVICE_TO_HOST);
        for(uint32_t i = 0; i < size; i++) {
            std::cout << out_buff[i] << std::endl;
        }
        device.cleanup();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } 
    return 0;
}

