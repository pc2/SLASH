#include <iostream>
#include <cstring> // for std::memcpy
#include <random>
#include <chrono>

#include <utils/logger.hpp>
#include <api/device.hpp>
#include <api/buffer.hpp>
#include <api/kernel.hpp>

int main() {
    try {
        uint32_t size = 1024 * 1024;
        vrt::utils::Logger::setLogLevel(vrt::utils::LogLevel::DEBUG);
        vrt::Device device("21:00.0", "00_example_hw.vrtbin", true, vrt::ProgramType::JTAG);
        device.setFrequency(200000000);
        vrt::Kernel accumulate(device, "accumulate_0");
        vrt::Kernel increment(device, "increment_0");
        vrt::Buffer<float> buffer(device, size, vrt::MemoryRangeType::HBM);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        float goldenModel = 0;
        std::cout << "Generating data...\n";
        for(uint32_t i = 0; i < size; i++) {
            buffer[i] = static_cast<float>(dis(gen));
            goldenModel+=buffer[i] + 1;
        }

        buffer.sync(vrt::SyncType::HOST_TO_DEVICE);
        increment.start(size, buffer.getPhysAddr());
        accumulate.start(size);
        auto start = std::chrono::high_resolution_clock::now();
        increment.wait();
        accumulate.wait();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Time taken for waits: " << duration << " us" << std::endl;

        uint32_t val = accumulate.read(0x18);
        float floatVal;
        std::memcpy(&floatVal, &val, sizeof(float));
        if(std::fabs(goldenModel - floatVal) > 0.0001) {
            std::cerr << "Test failed!" << std::endl;
            std::cout << "Expected: " << goldenModel << std::endl;
            std::cout << "Got: " << floatVal << std::endl;
            device.cleanup();
            return 1;
        } else {
            std::cout << "Expected: " << goldenModel << std::endl;
            std::cout << "Got: " << floatVal << std::endl;
            std::cout << "Test passed!" << std::endl;
        }
        
        device.cleanup();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } 
    return 0;
}