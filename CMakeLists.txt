cmake_minimum_required(VERSION 3.10)

project(Key-value-lseq VERSION 1.0)
find_package(GTest CONFIG REQUIRED)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

add_executable(Key-value-lseq src/main.cpp)

add_subdirectory(third-party/leveldb)

enable_testing()

include(GoogleTest)