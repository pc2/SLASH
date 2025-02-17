#include "api/streaming_buffer.hpp"

namespace vrt {
    template <typename T>
    StreamingBuffer<T>::StreamingBuffer(Device device, Kernel kernel, const std::string& portName, size_t size)
        : device(device), size(size), kernel(kernel), portName(portName) {
            
        std::vector<QdmaConnection> qdmaConnections = device.getQdmaConnections();
        bool gotQdma = false;
        for(const auto& con : qdmaConnections) {
           if(con.getKernel() == kernel.getName() && portName == con.getInterface()) {
                index = con.getQid();
                syncType = con.getDirection();
                gotQdma = true;
           }
        }
        if(!gotQdma) {
            throw std::runtime_error("No QDMA connection found for kernel " + kernel.getName() + " and port " + portName);
        }
        name = (syncType == StreamDirection::HOST_TO_DEVICE) ? ("streamingBuffer_" + std::to_string(index)) : ("outputStreamingBuffer_" + std::to_string(index));
        localBuffer = new T[size];
        Platform platform = device.getPlatform();
        // setup qdma queue
    }

    template <typename T>
    StreamingBuffer<T>::~StreamingBuffer() {
        delete[] localBuffer;
        // cleanup qdma queue
    }
    
    template <typename T>
    T& StreamingBuffer<T>::operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return localBuffer[index];
    }

    template <typename T>
    const T& StreamingBuffer<T>::operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return localBuffer[index];
    }

    template <typename T>
    void StreamingBuffer<T>::sync() {
        Platform platform = device.getPlatform();
        if(platform == Platform::EMULATION) {
            ZmqServer* server = device.getZmqServer();
            if(syncType == StreamDirection::HOST_TO_DEVICE) {
                std::vector<uint8_t> sendData;
                std::size_t dataSize = size * sizeof(T);
                sendData.resize(dataSize);
                std::memcpy(sendData.data(), localBuffer, dataSize);
                server->sendStream(name, sendData);
            } else {
                std::vector<uint8_t> recvData = server->fetchStream(name, size * sizeof(T)); // ????
                size = recvData.size() / sizeof(T);
                localBuffer = reinterpret_cast<T*>(realloc(localBuffer, recvData.size()));
                std::memcpy(localBuffer, recvData.data(), recvData.size());
            }
        } else {
            throw std::runtime_error("Streaming buffer not implemented for this platform");
        }
    }

    template <typename T>
    std::string StreamingBuffer<T>::getName() const {
        return name;
    }

}