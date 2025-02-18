#include <iostream>

#include <api/buffer.hpp>
#include <api/device.hpp>
#include <api/streaming_buffer.hpp>
#include <utils/logger.hpp>

int main() {
    vrt::utils::Logger::setLogLevel(vrt::utils::LogLevel::DEBUG);
    uint32_t size = 1024;
    vrt::Device device("21:00.0", "06_streaming_hw.vrtbin", false, vrt::ProgramType::JTAG);
    device.setFrequency(400000000);
    vrt::Kernel streaming(device, "streaming_0");
    vrt::StreamingBuffer<uint32_t> input0(device, streaming, "axis_in", size);
    vrt::Buffer<uint32_t> output(device, size, vrt::MemoryRangeType::HBM);

    for (uint32_t i = 0; i < size; i++) {
        input0[i] = 1;
    }
    streaming.start(output.getPhysAddr(), size * sizeof(uint32_t) / 64);
    input0.sync();
    streaming.wait();
    output.sync(vrt::SyncType::DEVICE_TO_HOST);
    for (uint32_t i = 0; i < size; i++) {
        if (output[i] != 1) {
            std::cout << "Error: output[" << i << "] = " << output[i] << std::endl;
            return -1;
        }
    }
    std::cout << "Test passed\n";
    device.cleanup();
    return 0;
}