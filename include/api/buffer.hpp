#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "allocator/allocator.hpp"
#include "api/device.hpp"
#include "qdma/qdma_intf.hpp"
#include "utils/platform.hpp"
#include "utils/zmq_server.hpp"

namespace vrt {

/**
 * @brief Enum class representing the type of synchronization.
 */
enum class SyncType {
    HOST_TO_DEVICE,  ///< Synchronize from host to device
    DEVICE_TO_HOST,  ///< Synchronize from device to host
};

/**
 * @brief Class representing a buffer.
 * @tparam T The type of elements in the buffer.
 */
template <typename T>
class Buffer {
   public:
    /**
     * @brief Constructor for Buffer.
     * @param device VRT Device of the buffer.
     * @param size The size of the buffer.
     * @param type The type of memory range.
     */
    Buffer(Device device, size_t size, MemoryRangeType type);

    /**
     * @brief Constructor for Buffer.
     * @param device VRT Device of the buffer.
     * @param size The size of the buffer.
     * @param type The type of memory range.
     * @param port The HBM port number. This would not have any effect if the type is DDR.
     */
    Buffer(Device device, size_t size, MemoryRangeType type, uint8_t port);

    /**
     * @brief Destructor for Buffer.
     */
    ~Buffer();

    /**
     * @brief Gets a pointer to the buffer.
     * @return A pointer to the buffer.
     */
    T* get() const;

    /**
     * @brief Overloads the subscript operator to access buffer elements.
     * @param index The index of the element to access.
     * @return A reference to the element at the specified index.
     */
    T& operator[](size_t index);

    /**
     * @brief Overloads the subscript operator to access buffer elements (const version).
     * @param index The index of the element to access.
     * @return A const reference to the element at the specified index.
     */
    const T& operator[](size_t index) const;

    /**
     * @brief Gets the physical address of the buffer.
     * @return The physical address of the buffer.
     */
    uint64_t getPhysAddr() const;

    /**
     * @brief Gets the lower 32 bits of the physical address of the buffer.
     * @return The lower 32 bits of the physical address of the buffer.
     */
    uint32_t getPhysAddrLow() const;

    /**
     * @brief Gets the upper 32 bits of the physical address of the buffer.
     * @return The upper 32 bits of the physical address of the buffer.
     */
    uint32_t getPhysAddrHigh() const;

    /**
     * @brief Synchronizes the buffer.
     * @param syncType The type of synchronization.
     */
    void sync(SyncType syncType);

    std::string getName();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

   private:
    uint64_t startAddress;           ///< The starting address of the buffer
    T* localBuffer;                  ///< Pointer to the local buffer
    size_t size;                     ///< The size of the buffer
    MemoryRangeType type;            ///< The type of memory range
    Device device;                   ///< The device associated with the buffer
    std::size_t index;               // Member variable to store the index of the buffer
    static std::size_t bufferIndex;  // Static variable to track the buffer index
};

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
    if (platform == Platform::EMULATION) {
        // send initial buffer so it is populated in the emulation environment
        std::shared_ptr<ZmqServer> server = device.getZmqServer();
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
    if (startAddress != 0) {
        device.getAllocator()->deallocate(startAddress);
    }
    // Deallocate local buffer
    if (localBuffer != nullptr) {
        delete[] localBuffer;
    }
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
//         qdmaIntf.write_buff(reinterpret_cast<char*>(localBuffer), startAddress, size *
//         sizeof(T));
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
    if (platform == Platform::HARDWARE) {
        size_t maxChunkSize = 1 << 24;  // 22
        size_t totalSize = size * sizeof(T);
        size_t chunkSize = maxChunkSize * sizeof(T);
        size_t offset = 0;

        while (totalSize > 0) {
            size_t currentChunkSize = std::min(chunkSize, totalSize);
            if (syncType == SyncType::HOST_TO_DEVICE) {
                this->device.qdmaIntf.write_buff(reinterpret_cast<char*>(localBuffer) + offset,
                                                 startAddress + offset, currentChunkSize);
            } else if (syncType == SyncType::DEVICE_TO_HOST) {
                this->device.qdmaIntf.read_buff(reinterpret_cast<char*>(localBuffer) + offset,
                                                startAddress + offset, currentChunkSize);
            } else {
                throw std::invalid_argument("Invalid sync type");
            }
            offset += currentChunkSize;
            totalSize -= currentChunkSize;
        }
    } else if (platform == Platform::EMULATION) {
        std::shared_ptr<ZmqServer> server = device.getZmqServer();
        if (syncType == SyncType::HOST_TO_DEVICE) {
            std::vector<uint8_t> sendData;
            std::size_t dataSize = size * sizeof(T);
            sendData.resize(dataSize);
            std::memcpy(sendData.data(), localBuffer, dataSize);
            server->sendBuffer(std::to_string(getPhysAddr()), sendData);
        } else if (syncType == SyncType::DEVICE_TO_HOST) {
            std::vector<uint8_t> recvData = server->fetchBuffer(std::to_string(getPhysAddr()));
            size = recvData.size() / sizeof(T);
            localBuffer = reinterpret_cast<T*>(realloc(localBuffer, recvData.size()));
            std::memcpy(localBuffer, recvData.data(), recvData.size());

        } else {
            throw std::invalid_argument("Invalid sync type");
        }

    } else if (platform == Platform::SIMULATION) {
        std::shared_ptr<ZmqServer> server = device.getZmqServer();
        if (syncType == SyncType::HOST_TO_DEVICE) {
            std::vector<uint8_t> sendData;
            std::size_t dataSize = size * sizeof(T);
            sendData.resize(dataSize);
            std::memcpy(sendData.data(), localBuffer, dataSize);
            server->sendBufferSim(getPhysAddr(), sendData);
        } else if (syncType == SyncType::DEVICE_TO_HOST) {
            std::vector<uint8_t> recvData;
            server->fetchBufferSim(getPhysAddr(), size * sizeof(T), recvData);

            size = recvData.size() * sizeof(T);
            localBuffer = reinterpret_cast<T*>(realloc(localBuffer, recvData.size()));
            std::memcpy(localBuffer, recvData.data(), recvData.size());
        } else {
            throw std::invalid_argument("Invalid sync type");
        }
    }
}
template <typename T>
Buffer<T>::Buffer(Buffer&& other) noexcept
    : device(other.device),
      size(other.size),
      type(other.type),
      index(other.index),
      startAddress(other.startAddress),
      localBuffer(other.localBuffer) {
    // Reset the moved-from object
    other.startAddress = 0;
    other.localBuffer = nullptr;
    other.size = 0;
}

template <typename T>
Buffer<T>& Buffer<T>::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        if (localBuffer) {
            delete[] localBuffer;
        }

        if (startAddress != 0) {
            device.getAllocator()->deallocate(startAddress);
        }

        device = other.device;
        size = other.size;
        type = other.type;
        index = other.index;
        startAddress = other.startAddress;
        localBuffer = other.localBuffer;

        other.startAddress = 0;
        other.localBuffer = nullptr;
        other.size = 0;
    }
    return *this;
}

}  // namespace vrt

// #include "api/buffer.tpp"

#endif  // BUFFER_HPP