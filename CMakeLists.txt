cmake_minimum_required(VERSION 3.10)
project(vrt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include/
 ${CMAKE_SOURCE_DIR}/include/allocator/
 ${CMAKE_SOURCE_DIR}/include/api/  ${CMAKE_SOURCE_DIR}/src/api/
 ${CMAKE_SOURCE_DIR}/include/qdma/dma-utils ${CMAKE_SOURCE_DIR}/src/qdma/)
# Define sources for the library
file(GLOB LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/allocator/*.cpp ${CMAKE_SOURCE_DIR}/include/buffer/*.hpp
${CMAKE_SOURCE_DIR}/src/qdma/*.cpp ${CMAKE_SOURCE_DIR}/src/qdma/dma-utils/*.c)

# Create the shared library
add_library(api SHARED ${LIB_SOURCES})

# Specify the output directory for the shared library
set_target_properties(api PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Define sources for the executable
file(GLOB EXE_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${EXE_SOURCES})

# Link the library to the executable
target_link_libraries(${PROJECT_NAME} api aio)