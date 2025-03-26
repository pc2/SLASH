#include <iostream>
#include <cstring>

#include <api/device.hpp>
#include <api/buffer.hpp>
#include <api/kernel.hpp>

int main() {
    try {
        vrt::utils::Logger::setLogLevel(vrt::utils::LogLevel::DEBUG);
        uint32_t size = 1024 * 1024;

        vrt::Device device("21:00.0", "04_vadd_hw.vrtbin");
        vrt::Kernel vadd_0(device, "vadd_0");
        device.setFrequency(200000000);
        vrt::Buffer<int> a(device, size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<int> b(device, size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<int> c(device, size, vrt::MemoryRangeType::HBM);

        for (int i = 0; i < size; i++) {
            a[i] = i;
            b[i] = i;
        }
        a.sync(vrt::SyncType::HOST_TO_DEVICE);
        b.sync(vrt::SyncType::HOST_TO_DEVICE);
        vadd_0.call(a.getPhysAddr(), b.getPhysAddr(), c.getPhysAddr(), size);
        // vadd_0.wait();
        c.sync(vrt::SyncType::DEVICE_TO_HOST);
        // b.sync(vrt::SyncType::DEVICE_TO_HOST);
        for (int i = 0; i < size; i++) {
            if (c[i] != a[i] + b[i]) {
                std::cerr << "Error: " << c[i] << " != " << a[i] << " + " << b[i] << std::endl;
                device.cleanup();
                return 1;
            }
        }
        std::cout << "Test passed" << std::endl;
        device.cleanup();
     } catch (std::exception const& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}