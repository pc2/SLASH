#ifndef STREAMING_BUFFER_HPP
#define STREAMING_BUFFER_HPP

#include "api/device.hpp"
#include "qdma/qdma_intf.hpp"
#include "utils/platform.hpp"
#include "utils/zmq_server.hpp"
#include "qdma/qdma_connection.hpp"
#include <regex>

namespace vrt {


    template<typename T>
    class StreamingBuffer {
    public:
        StreamingBuffer(Device device, Kernel kernel, const std::string& portName, size_t size);
        ~StreamingBuffer();
        T* get() const;
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        std::string getName() const;
        void sync();

    private:
        T* localBuffer;
        size_t size;
        StreamDirection syncType;
        Device device;
        Kernel kernel;
        std::size_t index;
        std::string name;
        std::string portName;
    };
} // namespace vrt

#include "api/streaming_buffer.tpp"

#endif // STREAMING_BUFFER_HPP