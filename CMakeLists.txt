cmake_minimum_required(VERSION 3.15)
project(huffman CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(fmt REQUIRED)
find_package(Boost REQUIRED)

set(COMPRESSION_COORDINATOR src/coordinator/compression_coordinator.cpp)
set(DECOMPRESSION_COORDINATOR src/coordinator/decompression_coordinator.cpp)
set(ENCODER src/coder/encoder.cpp)
set(DECODER src/coder/decoder.cpp)
set(HUFFMAN src/huffman/huffman.cpp)
set(SRCS src/main.cpp ${HUFFMAN} ${ENCODER} ${DECODER} ${COMPRESSION_COORDINATOR} ${DECOMPRESSION_COORDINATOR})

add_executable(${PROJECT_NAME} ${SRCS})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion)
endif()

target_link_libraries(${PROJECT_NAME} fmt::fmt)
target_link_libraries(${PROJECT_NAME} boost::boost)
