cmake_minimum_required(VERSION 3.10)
project(vrt-api)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("-Wunused-result")

include_directories(${CMAKE_SOURCE_DIR}/include/ ${CMAKE_SOURCE_DIR}/src/ /usr/include/ami/ /usr/include/libxml2 /usr/include/jsoncpp)

file(GLOB LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/allocator/*.cpp ${CMAKE_SOURCE_DIR}/include/buffer/*.hpp
${CMAKE_SOURCE_DIR}/src/qdma/*.cpp ${CMAKE_SOURCE_DIR}/src/api/*.cpp 
${CMAKE_SOURCE_DIR}/src/parser/*.cpp ${CMAKE_SOURCE_DIR}/src/register/*.cpp ${CMAKE_SOURCE_DIR}/src/driver/*.cpp
${CMAKE_SOURCE_DIR}/src/utils/*.cpp)

add_library(vrt SHARED ${LIB_SOURCES})

set_target_properties(vrt PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

install(TARGETS vrt
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION vrt/include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/scripts/ DESTINATION vrt
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)