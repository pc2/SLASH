cmake_minimum_required(VERSION 3.10)
project(02_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/usr/local/vrt/include
                    /usr/include/ami
                    /usr/include/libxml2)

# Define sources for the executable
set(EXE_SOURCES 02_example.cpp)

# Create the executable
add_executable(${PROJECT_NAME} ${EXE_SOURCES})

# Link the library to the executable
target_link_libraries(${PROJECT_NAME} vrt /scratch/users/aulmamei/git/AVED_work/sw/AMI/api/build/libami.a xml2)