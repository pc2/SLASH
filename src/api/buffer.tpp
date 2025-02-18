#include "api/buffer.hpp"

namespace vrt {

    template <typename T>
    size_t Buffer<T>::bufferIndex = 0;

    template <typename T>
    Buffer<T>::Buffer(Device device, size_t size, MemoryRangeType type)
        : device(device), size(size), type(type), index(bufferIndex++) {

        startAddress = device.getAllocator()->allocate(size * sizeof(T), type);
        if (startAddress == 0) {
            throw std::bad_alloc();
        }

        // Allocate local buffer
        localBuffer = new T[size];
        Platform platform = device.getPlatform();
        if(platform == Platform::EMULATION) {
            // send initial buffer so it is populated in the emulation environment
            ZmqServer* server = device.getZmqServer();
            std::vector<uint8_t> sendData;
            std::size_t dataSize = size * sizeof(T);
            sendData.resize(dataSize);
            std::memcpy(sendData.data(), localBuffer, dataSize);
            server->sendBuffer(std::to_string(getPhysAddr()), sendData);
        }
    }

    template <typename T>
    Buffer<T>::Buffer(Device device, size_t size, MemoryRangeType type, uint8_t port)
        : device(device), size(size), type(type), index(bufferIndex++) {
        this->device = device;

        startAddress = device.getAllocator()->allocate(size * sizeof(T), type, port);
        if (startAddress == 0) {
            throw std::bad_alloc();
        }

        // Allocate local buffer
        localBuffer = new T[size];
    }

    template <typename T>
    Buffer<T>::~Buffer() {
        device.getAllocator()->deallocate(startAddress);
        // Deallocate local buffer
        delete[] localBuffer;
    }

    template <typename T>
    T* Buffer<T>::get() const {
        return localBuffer;
    }

    template <typename T>
    T& Buffer<T>::operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return localBuffer[index];
    }

    template <typename T>
    const T& Buffer<T>::operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return localBuffer[index];
    }

    template <typename T>
    uint64_t Buffer<T>::getPhysAddr() const {
        return startAddress;
    }

    template <typename T>
    uint32_t Buffer<T>::getPhysAddrLow() const {
        return startAddress & 0xFFFFFFFF;
    }

    template <typename T>
    uint32_t Buffer<T>::getPhysAddrHigh() const {
        return (startAddress >> 32) & 0xFFFFFFFF;
    }

    // template <typename T>
    // void Buffer<T>::sync(SyncType syncType) {
    //     auto& qdmaIntf = QdmaIntf::getInstance();
    //     if(syncType == SyncType::HOST_TO_DEVICE) {
    //         qdmaIntf.write_buff(reinterpret_cast<char*>(localBuffer), startAddress, size * sizeof(T));
    //     } else if (syncType == SyncType::DEVICE_TO_HOST) {
    //         qdmaIntf.read_buff(reinterpret_cast<char*>(localBuffer), startAddress, size * sizeof(T));
    //     } else {
    //             throw std::invalid_argument("Invalid sync type");
    //     }
    // }

    template <typename T>
    std::string Buffer<T>::getName() {
        return "buffer_" + std::to_string(index);
    }

    template <typename T>
    void Buffer<T>::sync(SyncType syncType) {
        Platform platform = device.getPlatform();
        if(platform == Platform::HARDWARE) {
            size_t maxChunkSize = 1 << 24; //22
            size_t totalSize = size * sizeof(T);
            size_t chunkSize = maxChunkSize * sizeof(T);
            size_t offset = 0;

            while (totalSize > 0) {
                size_t currentChunkSize = std::min(chunkSize, totalSize);
                if (syncType == SyncType::HOST_TO_DEVICE) {
                    this->device.qdmaIntf.write_buff(reinterpret_cast<char*>(localBuffer) + offset, startAddress + offset, currentChunkSize);
                } else if (syncType == SyncType::DEVICE_TO_HOST) {
                    this->device.qdmaIntf.read_buff(reinterpret_cast<char*>(localBuffer) + offset, startAddress + offset, currentChunkSize);
                } else {
                    throw std::invalid_argument("Invalid sync type");
                }
                offset += currentChunkSize;
                totalSize -= currentChunkSize;
            }
        } else if (platform == Platform::EMULATION) {
            ZmqServer* server = device.getZmqServer();
            if(syncType == SyncType::HOST_TO_DEVICE) {
                std::vector<uint8_t> sendData;
                std::size_t dataSize = size * sizeof(T);
                sendData.resize(dataSize);
                std::memcpy(sendData.data(), localBuffer, dataSize);
                server->sendBuffer(std::to_string(getPhysAddr()), sendData);
            } else if (syncType == SyncType::DEVICE_TO_HOST) {
                // std::vector<uint8_t> recvData = server.recvBuffer(bufferIdx);
                // std::memcpy(localBuffer, recvData.data(), recvData.size());
                std::vector<uint8_t> recvData = server->fetchBuffer(std::to_string(getPhysAddr()));
                // Resize localBuffer to match the size of recvData
                size = recvData.size() / sizeof(T);
                localBuffer = reinterpret_cast<T*>(realloc(localBuffer, recvData.size()));
                std::memcpy(localBuffer, recvData.data(), recvData.size());

            } else {
                throw std::invalid_argument("Invalid sync type");
            }

        }
    }
}  // namespace vrt