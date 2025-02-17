#include "allocator/allocator.hpp"
#include <gtest/gtest.h>

class AllocatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        allocator = new vrt::Allocator(1024);
        allocator->addMemoryRange(vrt::MemoryRangeType::HBM, vrt::HBM_START, vrt::HBM_SIZE);
        allocator->addMemoryRange(vrt::MemoryRangeType::DDR, vrt::DDR_START, vrt::DDR_SIZE);
    }

    vrt::Allocator* allocator;
};

TEST_F(AllocatorTest, AllocateWithinRange) {
    uint64_t addr = allocator->allocate(512, vrt::MemoryRangeType::HBM);
    EXPECT_GE(addr, vrt::HBM_START);
    EXPECT_LT(addr, vrt::HBM_START + vrt::HBM_SIZE);
}

TEST_F(AllocatorTest, AllocateDifferentSizes) {
    uint64_t addr1 = allocator->allocate(256, vrt::MemoryRangeType::HBM);
    uint64_t addr2 = allocator->allocate(512, vrt::MemoryRangeType::HBM);
    EXPECT_NE(addr1, addr2);
}

TEST_F(AllocatorTest, AllocateInvalidMemoryRangeType) {
    EXPECT_THROW(allocator->allocate(512, static_cast<vrt::MemoryRangeType>(999)), std::out_of_range);
}

TEST_F(AllocatorTest, DeallocateMemory) {
    uint64_t addr = allocator->allocate(512, vrt::MemoryRangeType::HBM);
    allocator->deallocate(addr);
    // Ensure the address can be reallocated
    uint64_t addr2 = allocator->allocate(512, vrt::MemoryRangeType::HBM);
    EXPECT_EQ(addr, addr2);
}

TEST_F(AllocatorTest, AllocateFromDifferentRanges) {
    uint64_t addr1 = allocator->allocate(512, vrt::MemoryRangeType::HBM);
    uint64_t addr2 = allocator->allocate(512, vrt::MemoryRangeType::DDR);
    EXPECT_NE(addr1, addr2);
    EXPECT_GE(addr1, vrt::HBM_START);
    EXPECT_LT(addr1, vrt::HBM_START + vrt::HBM_SIZE);
    EXPECT_GE(addr2, vrt::DDR_START);
    EXPECT_LT(addr2, vrt::DDR_START + vrt::DDR_SIZE);
}

TEST_F(AllocatorTest, AllocateMultipleRanges) {
    uint64_t addr1 = allocator->allocate(1024, vrt::MemoryRangeType::HBM, 0);
    uint64_t addr2 = allocator->allocate(512, vrt::MemoryRangeType::HBM, 0);
    uint64_t addr3 = allocator->allocate(512, vrt::MemoryRangeType::HBM, 5);
    uint64_t addr4 = allocator->allocate(512, vrt::MemoryRangeType::HBM, 5);
    EXPECT_NE(addr1, addr2);
    EXPECT_NE(addr1, addr3);
    EXPECT_NE(addr1, addr4);
    EXPECT_NE(addr2, addr3);
    EXPECT_NE(addr2, addr4);
    EXPECT_NE(addr3, addr4);
}

TEST_F(AllocatorTest, AllocateWithOverlappingPorts) {
    uint64_t addr1 = allocator->allocate(4L * 1024 * 1024 * 1024, vrt::MemoryRangeType::HBM, 0);
    uint64_t addr2 = allocator->allocate(4L * 1024 * 1024 * 1024, vrt::MemoryRangeType::HBM, 1);
    EXPECT_NE(addr1, addr2);
    EXPECT_GE(addr2, addr1);
}
