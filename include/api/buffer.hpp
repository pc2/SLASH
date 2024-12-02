#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "allocator/allocator.hpp"
#include "qdma/qdma_intf.hpp"

namespace vrt {

    enum class SyncType {
        HOST_TO_DEVICE,
        DEVICE_TO_HOST,
    };

    template <typename T>
    class Buffer {
    public:
        Buffer(size_t size, MemoryRangeType type);
        ~Buffer();
        T* get() const;
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        uint64_t getPhysAddr() const;
        void sync(SyncType syncType);
    private:
        static void initializeMemoryRanges();
        uint64_t startAddress;
        T* localBuffer;
        size_t size;
        MemoryRangeType type;
    };

} // namespace vrt
#include "buffer.tpp"


#endif // BUFFER_HPP