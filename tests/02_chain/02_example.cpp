#include <iostream>
#include <cstdint>
#include <utils/logger.hpp>
#include <api/device.hpp>
#include <api/buffer.hpp>
#include <api/kernel.hpp>
int main() {
    try {
        uint32_t size = 2048;
        vrt::utils::Logger::setLogLevel(vrt::utils::LogLevel::DEBUG);
        vrt::Device device("21:00.0", "02_example.vrtbin", true, vrt::ProgramType::JTAG);
        vrt::Kernel dma_in(device, "dma_in_0");
        vrt::Kernel dma_out(device, "dma_out_0");
        vrt::Buffer<uint64_t> buffer_in(device, size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<uint64_t> buffer_out(device, size, vrt::MemoryRangeType::HBM);
        for(uint32_t i = 0; i < size; i++) {
            buffer_in[i] = static_cast<uint64_t>(i);
        }
        buffer_in.sync(vrt::SyncType::HOST_TO_DEVICE);
        dma_in.start(buffer_in.getPhysAddr(), size);
        dma_out.start(buffer_out.getPhysAddr(), size);
        dma_in.wait();
        dma_out.wait();
        buffer_out.sync(vrt::SyncType::DEVICE_TO_HOST);
        for(uint32_t i = 0; i < size; i++) {
            if(buffer_in[i] != buffer_out[i]) {
                vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__, "Test failed");
                device.cleanup();
                return 0;
            }
        }
        vrt::utils::Logger::log(vrt::utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Test passed");
        device.cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}