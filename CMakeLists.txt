cmake_minimum_required(VERSION 3.15)

project(Key-value-lseq)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

#Needed to make custom comparator (see https://github.com/google/leveldb/issues/891)
STRING(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS "-fno-rtti -DGTEST_HAS_RTTI=0 -std=c++17 -fno-omit-frame-pointer")

option(BuildGRPC "Whether GRPC should be build" ON)
option(UseGTest "Whether GoogleTest should be linked and test should be performed" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Fastens build a lot and simply gtest usage
set(LEVELDB_BUILD_TESTS OFF)
set(LEVELDB_BUILD_BENCHMARKS  OFF)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(Key-value-lseq
        src/main.cpp
        src/utils/yamlConfig.cpp
        src/db/dbConnector.cpp
        src/proto-src/lseqDb.grpc.pb.cc
        src/proto-src/lseqDb.pb.cc
        src/server/grpc-server.cpp
        )

#Setup leveldb
add_subdirectory(third-party/leveldb)
target_link_libraries(Key-value-lseq PUBLIC leveldb)

#Setup yaml parser
add_subdirectory(third-party/yaml-cpp)
target_link_libraries(Key-value-lseq PUBLIC yaml-cpp)

if (BuildGRPC)
    include(FetchContent)
    FetchContent_Declare(
            gRPC
            GIT_REPOSITORY https://github.com/grpc/grpc
            GIT_TAG        v1.52.1
    )
    set(protobuf_BUILD_TESTS OFF)
    set(gRPC_BUILD_TESTS OFF)

    FetchContent_MakeAvailable(gRPC)
    target_link_libraries(Key-value-lseq PUBLIC grpc++)
endif()

#Using GTest is equivalent to performing testing
if (UseGTest)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest
            GIT_TAG        v1.13.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(
            configTest
            tests/configTests/configTest.cpp
            src/utils/yamlConfig.cpp
    )
    target_link_libraries(
            configTest
            gtest_main
            yaml-cpp
            gtest
    )
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/resources/config.yaml
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

    add_executable(
            baseDbTest
            tests/dbTests/baseDbTest.cpp
            src/utils/yamlConfig.cpp
            src/db/dbConnector.cpp
    )
    target_link_libraries(
            baseDbTest
            leveldb
            gtest_main
            yaml-cpp
            gtest
    )

    add_executable(
            groupOperationTest
            tests/dbTests/groupOperationTest.cpp
            src/utils/yamlConfig.cpp
            src/db/dbConnector.cpp
    )
    target_link_libraries(
            groupOperationTest
            leveldb
            gtest_main
            yaml-cpp
            gtest
    )
endif()