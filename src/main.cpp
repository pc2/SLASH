#include <iostream>
#include "api/device.hpp"
#include "api/buffer.hpp"
#include "api/kernel.hpp"
int main() {
    try {

        vrt::Device device("21:00.0", "/scratch/users/aulmamei/git/hls_parser_cpp/build/system_map.xml");
        vrt::Kernel kernel(device, "offset");
        vrt::Buffer<uint64_t> in_buffer(100, vrt::MemoryRangeType::HBM);
        vrt::Buffer<uint64_t> out_buffer(100, vrt::MemoryRangeType::HBM);
        for (size_t i = 0; i < 100; ++i) {
            in_buffer[i] = static_cast<uint64_t>(i);
        }
        in_buffer.sync(vrt::SyncType::HOST_TO_DEVICE);
        kernel.write(0x10, (in_buffer.getPhysAddr()) & 0xFFFFFFFF);
        kernel.write(0x14, (in_buffer.getPhysAddr() >> 32) & 0xFFFFFFFF);
        kernel.write(0x1c, (out_buffer.getPhysAddr()) & 0xFFFFFFFF);
        kernel.write(0x20, (out_buffer.getPhysAddr() >> 32) & 0xFFFFFFFF);
        kernel.write(0x28, 8*100);
        kernel.write(0x00, 0x81);
        out_buffer.sync(vrt::SyncType::DEVICE_TO_HOST);
        device.cleanup();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}