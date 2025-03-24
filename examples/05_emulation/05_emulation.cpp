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
        uint32_t size = 512;
        uint32_t m = 3;
        uint32_t n = 2;
        vrt::Device device("21:00.0", "05_emulation_sim.vrtbin", true, vrt::ProgramType::JTAG);
        vrt::Kernel dma(device, "dma_0");
        vrt::Kernel offset(device, "offset_0");
        device.setFrequency(233333333);
        vrt::Buffer<uint32_t> in_buff(device, size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<uint32_t> out_buff(device, size, vrt::MemoryRangeType::HBM);
	std::cout << std::showbase << std::hex << in_buff.getPhysAddr() << std::endl;
	std::cout << std::showbase << std::hex << out_buff.getPhysAddr() << std::endl;
        for(uint32_t i = 0; i < size; i++) {
            in_buff[i] = 1;
        }
        in_buff.sync(vrt::SyncType::HOST_TO_DEVICE);
        offset.start(size, in_buff.getPhysAddr(), m, n);
        dma.start(size, out_buff.getPhysAddr());
        offset.wait();
        dma.wait();
        out_buff.sync(vrt::SyncType::DEVICE_TO_HOST);
// 	in_buff.sync(vrt::SyncType::DEVICE_TO_HOST);
        for(uint32_t i = 0; i < size; i++) {
            if(out_buff[i] != in_buff[i] * m + n) {
                std::cerr << "Test failed" << std::endl;
                std::cerr << "Error: " << std::showbase << std::hex << i << " " << out_buff[i] << " " << in_buff[i] * m + n << std::endl;
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

