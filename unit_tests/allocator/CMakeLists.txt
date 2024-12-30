cmake_minimum_required(VERSION 3.10)
project(AllocatorTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add source files
add_executable(allocator_test
    allocator_test.cpp
    ../../src/allocator/allocator.cpp
)

# Include directories
target_include_directories(allocator_test PRIVATE ../../include)

# Link GoogleTest
target_link_libraries(allocator_test gtest gtest_main)

# Enable testing
enable_testing()

# Add test
add_test(NAME AllocatorTest COMMAND allocator_test)