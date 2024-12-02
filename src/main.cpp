#include <iostream>
#include "api/buffer.hpp"
#include "api/kernel.hpp"
int main() {
    try {
        vrt::Kernel kernel("21:00.0");
        kernel.write(0x00, 0xff);
        uint32_t val = kernel.read(0x00);
        std::cout << "0x" << std::hex <<val << std::endl;
        vrt::Buffer<int> intBuffer(100, vrt::MemoryRangeType::HBM);
        std::cout << "0x" << std::hex <<intBuffer.getPhysAddr() << std::endl;
        for (size_t i = 0; i < 100; ++i) {
            intBuffer[i] = static_cast<int>(i);
        }
        intBuffer.sync(vrt::SyncType::HOST_TO_DEVICE);
        for(size_t i = 0; i < 100; i++) {
            intBuffer[i] = 0;
        }
        intBuffer.sync(vrt::SyncType::DEVICE_TO_HOST);
        for(size_t i = 0; i < 100; i++) {
            std::cout << "intBuffer[" << i << "]: " << intBuffer[i] << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}