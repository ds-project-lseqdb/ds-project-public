cmake_minimum_required(VERSION 3.10)

project(Key-value-lseq VERSION 1.0)
find_package(GTest CONFIG REQUIRED)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

add_executable(Key-value-lseq
        src/main.cpp
        src/utils/yamlConfig.cpp
        )

#Setup leveldb
add_subdirectory(third-party/leveldb)
target_link_libraries(Key-value-lseq PUBLIC leveldb)

#Setup yaml parser
add_subdirectory(third-party/yaml-cpp)
target_link_libraries(Key-value-lseq PUBLIC yaml-cpp)

enable_testing()

include(GoogleTest)