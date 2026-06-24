cmake_minimum_required(VERSION 3.15)

project(HuffmanTextCompressor)

set(CMAKE_CXX_STANDARD 17)

add_executable(compressor src/main.cpp src/huffman.cpp)