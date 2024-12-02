#ifndef CUSTOM_ALLOCATOR_HPP
#define CUSTOM_ALLOCATOR_HPP

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace vrt {
    enum class MemoryRangeType {
        HBM,
        DDR
    };

    constexpr uint64_t HBM_START = 0x4000000000;
    constexpr uint64_t HBM_SIZE = 32L * 1024 * 1024 * 1024; // 32G

    constexpr uint64_t DDR_START = 0x50080000000;
    constexpr uint64_t DDR_SIZE = 32L * 1024 * 1024 * 1024; // 32G

    class Superblock {
    public:
        Superblock(uint64_t startAddress, uint64_t size);
        uint64_t allocate(uint64_t size);
        void deallocate(uint64_t addr);

    private:
        uint64_t startAddress;
        uint64_t size;
        uint64_t offset;
        std::vector<uint64_t> freeList;
    };

    struct MemoryRange {
        uint64_t startAddress;
        uint64_t size;
        uint64_t offset;
        std::vector<Superblock> superblocks;
        std::vector<uint64_t> freeList;

        MemoryRange(uint64_t startAddress, uint64_t size);
    };

    class Allocator {
    public:
        static Allocator& getInstance(uint64_t superblockSize = 0x1000);

        void addMemoryRange(MemoryRangeType type, uint64_t startAddress, uint64_t size);
        uint64_t allocate(uint64_t size, MemoryRangeType type);
        void deallocate(uint64_t addr);
        uint64_t getSize(MemoryRangeType type) const;

    private:
        Allocator(uint64_t superblockSize);
        Allocator(const Allocator&) = delete;
        Allocator& operator=(const Allocator&) = delete;

        uint64_t superblockSize;
        std::unordered_map<MemoryRangeType, MemoryRange> memoryRanges;
        std::unordered_map<uint64_t, Superblock*> addrToSuperblock;
    };
} // namespace vrt
#endif // CUSTOM_ALLOCATOR_HPP