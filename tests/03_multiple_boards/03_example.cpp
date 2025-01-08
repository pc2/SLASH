#include <iostream>
#include <cstring> // for std::memcpy
#include <random>

#include <utils/logger.hpp>
#include <api/device.hpp>
#include <api/buffer.hpp>
#include <api/kernel.hpp>

int main() {
    try {
        uint32_t size = 2048;
        vrt::utils::Logger::setLogLevel(vrt::utils::LogLevel::INFO);
        vrt::Device fpga0("e2:00.0", "03_example.vrtbin", false, vrt::ProgramType::FLASH);
        vrt::Device fpga1("21:00.0", "03_example.vrtbin", false, vrt::ProgramType::FLASH);
        fpga0.setFrequency(200000000);
        fpga1.setFrequency(200000000);
        vrt::Kernel accumulate0(fpga0, "accumulate_0");
        vrt::Kernel increment0(fpga0, "increment_0");
        vrt::Kernel accumulate1(fpga1, "accumulate_0");
        vrt::Kernel increment1(fpga1, "increment_0");
        vrt::Buffer<float> buffer0(fpga0, size, vrt::MemoryRangeType::HBM);
        vrt::Buffer<float> buffer1(fpga1, size, vrt::MemoryRangeType::HBM);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        float goldenModel0 = 0, goldenModel1 = 0;
        for(uint32_t i = 0; i < size; i++) {
            buffer0[i] = static_cast<float>(dis(gen));
            buffer1[i] = static_cast<float>(dis(gen));
            goldenModel0+=buffer0[i] + 1;
            goldenModel1+=buffer1[i] + 1;
        }

        buffer0.sync(vrt::SyncType::HOST_TO_DEVICE);
        buffer1.sync(vrt::SyncType::HOST_TO_DEVICE);
        increment0.call(size, buffer0.getPhysAddr());
        accumulate0.call(size);
        increment1.call(size, buffer1.getPhysAddr());
        accumulate1.call(size);
        uint32_t val0 = accumulate0.read(0x18);
        uint32_t val1 = accumulate1.read(0x18);
        float floatVal0, floatVal1;
        std::memcpy(&floatVal0, &val0, sizeof(float));
        std::memcpy(&floatVal1, &val1, sizeof(float));
        if(std::fabs(goldenModel0 - floatVal0) > 0.0001) {
            vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__,"Test failed for FPGA 0!");
            vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__,"Expected: {}", goldenModel0);
            vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__,"Got: {}", floatVal0);
            fpga0.cleanup();
            return 1;
        } else {
            vrt::utils::Logger::log(vrt::utils::LogLevel::INFO, __PRETTY_FUNCTION__,"Expected: {}", goldenModel0);
            vrt::utils::Logger::log(vrt::utils::LogLevel::INFO, __PRETTY_FUNCTION__,"Got: {}", floatVal0);
            vrt::utils::Logger::log(vrt::utils::LogLevel::INFO, __PRETTY_FUNCTION__,"Test passed for FPGA 0!");
        }

        if(std::fabs(goldenModel1 - floatVal1) > 0.0001) {
            vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__,"Test failed for FPGA 1!");
            vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__,"Expected: {}", goldenModel1);
            vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__,"Got: {}", floatVal1);
            fpga1.cleanup();
            return 1;
        } else {
            vrt::utils::Logger::log(vrt::utils::LogLevel::INFO, __PRETTY_FUNCTION__,"Expected: {}", goldenModel1);
            vrt::utils::Logger::log(vrt::utils::LogLevel::INFO, __PRETTY_FUNCTION__,"Got: {}", floatVal1);
            vrt::utils::Logger::log(vrt::utils::LogLevel::INFO, __PRETTY_FUNCTION__,"Test passed for FPGA 1!");
        }
        
        fpga0.cleanup();
        fpga1.cleanup();

    } catch (const std::exception& e) {
        vrt::utils::Logger::log(vrt::utils::LogLevel::ERROR, __PRETTY_FUNCTION__,"Exception: {}", e.what());
        return 1;
    } 
    return 0;
}