cmake_minimum_required(VERSION 3.10)
project(05_emulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(/usr/local/vrt/include
                    /usr/include/ami
                    /usr/include/libxml2
                    /usr/include/jsoncpp)

# Define sources for the executable
set(EXE_SOURCES 05_emulation.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${EXE_SOURCES})

# Link the library to the executable
target_link_libraries(${PROJECT_NAME} vrt ami xml2 zmq jsoncpp)