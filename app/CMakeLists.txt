cmake_minimum_required(VERSION 3.10)

# Project name
project(v80-smi)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include /usr/include/ami/ /usr/include/libxml2 /usr/include/jsoncpp)

file(GLOB APP_SOURCES v80-smi.cpp src/*.cpp src/commands/*.cpp src/utils/*.cpp)

# Source files
set(SOURCES
    ${APP_SOURCES}
)

# Executable
add_executable(v80-smi ${SOURCES})

# Link libraries (if any)
target_link_libraries(v80-smi ami jsoncpp xml2)

# Install the executable
install(TARGETS v80-smi DESTINATION /usr/local/bin)