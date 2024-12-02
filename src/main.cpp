#include <iostream>
#include "api/buffer.hpp"
#include "qdma/qdma_intf.hpp"
int main() {
    try {

        vrt::Buffer<int> intBuffer(100, vrt::MemoryRangeType::HBM);

        //vrt::Buffer<double> doubleBuffer(50, vrt::MemoryRangeType::HBM);
        std::cout << "0x" << std::hex <<intBuffer.getPhysAddr() << std::endl;
        //std::cout << "0x" << std::hex <<doubleBuffer.getPhysAddr() << std::endl;
        for (size_t i = 0; i < 100; ++i) {
            intBuffer[i] = static_cast<int>(i);
        }

        // for (size_t i = 0; i < 50; ++i) {
        //     doubleBuffer[i] = static_cast<double>(i) * 1.1;
        // }

        // Print some values to verify
        std::cout << "intBuffer[10]: " << intBuffer[10] << std::endl;
        intBuffer.sync(vrt::SyncType::HOST_TO_DEVICE);
        for(size_t i = 0; i < 100; i++) {
            intBuffer[i] = 0;
        }
        intBuffer.sync(vrt::SyncType::DEVICE_TO_HOST);
        for(size_t i = 0; i < 100; i++) {
            std::cout << "intBuffer[" << i << "]: " << intBuffer[i] << std::endl;
        }
        usleep(1000000);
        //std::cout << "doubleBuffer[10]: " << doubleBuffer[10] << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}