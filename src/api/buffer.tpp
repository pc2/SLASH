#include "api/buffer.hpp"

namespace vrt {
    template <typename T>
    void Buffer<T>::initializeMemoryRanges() {
        static bool memoryRangesInitialized = false;
        if (!memoryRangesInitialized) {
            Allocator& allocator = Allocator::getInstance();
            allocator.addMemoryRange(MemoryRangeType::HBM, HBM_START, HBM_SIZE);
            allocator.addMemoryRange(MemoryRangeType::DDR, DDR_START, DDR_SIZE);
            memoryRangesInitialized = true;
        }
    }

    template <typename T>
    Buffer<T>::Buffer(size_t size, MemoryRangeType type)
        : size(size), type(type) {
        // Initialize memory ranges if not already done
        initializeMemoryRanges();
        Allocator& allocator = Allocator::getInstance();
        startAddress = allocator.allocate(size * sizeof(T), type);
        if (startAddress == 0) {
            throw std::bad_alloc();
        }

        // Allocate local buffer
        localBuffer = new T[size];
    }

    template <typename T>
    Buffer<T>::~Buffer() {
        // Deallocate memory using the allocator
        Allocator& allocator = Allocator::getInstance();
        allocator.deallocate(startAddress);
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
    void Buffer<T>::sync(SyncType syncType) {
        auto& qdmaIntf = QdmaIntf::getInstance();
        if(syncType == SyncType::HOST_TO_DEVICE) {
            qdmaIntf.write_buff(reinterpret_cast<char*>(localBuffer), startAddress, size * sizeof(T));
        } else if (syncType == SyncType::DEVICE_TO_HOST) {
            qdmaIntf.read_buff(reinterpret_cast<char*>(localBuffer), startAddress, size * sizeof(T));
        } else {
                throw std::invalid_argument("Invalid sync type");
        }
    }
}  // namespace vrt