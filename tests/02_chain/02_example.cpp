#include <iostream>
#include <cstdint>
#include <vrt/api/device.hpp>
#include <vrt/api/buffer.hpp>
#include <vrt/api/kernel.hpp>

int main() {
    try {
        uint32_t size = 2048;
        vrt::Device device("21:00.0", "02_example.vrtbin", true, vrt::ProgramType::JTAG);
        vrt::Kernel dma_in(device, "dma_in_0");
        vrt::Kernel dma_out(device, "dma_out_0");
        vrt::Buffer<uint64_t> buffer_in(size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<uint64_t> buffer_out(size, vrt::MemoryRangeType::HBM);
        for(uint32_t i = 0; i < size; i++) {
            buffer_in[i] = static_cast<uint64_t>(i);
        }
        buffer_in.sync(vrt::SyncType::HOST_TO_DEVICE);
        dma_in.write(0x10, buffer_in.getPhysAddrLow());
        dma_in.write(0x14, buffer_in.getPhysAddrHigh());
        dma_in.write(0x1c, size);
        dma_out.write(0x18, buffer_out.getPhysAddrLow());
        dma_out.write(0x1c, buffer_out.getPhysAddrHigh());
        dma_out.write(0x10, size);
        dma_in.start();
        dma_out.start();
        dma_in.wait();
        dma_out.wait();
        buffer_out.sync(vrt::SyncType::DEVICE_TO_HOST);
        for(uint32_t i = 0; i < size; i++) {
            if(buffer_in[i] != buffer_out[i]) {
                std::cout << "Test failed\n";
                device.cleanup();
                return 0;
            }
        }
        std::cout << "Test passed\n";
        device.cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}