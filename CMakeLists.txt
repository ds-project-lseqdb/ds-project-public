cmake_minimum_required(VERSION 3.10)

project(Key-value-lseq VERSION 1.0)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++1z -pthread")

#Needed to make custom comparator (see https://github.com/google/leveldb/issues/891)
STRING(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -DGTEST_HAS_RTTI=0 ")

option(BuildGRPC "Whether GRPC should be build" OFF)
option(UseGTest "Whether GoogleTest should be linked and test should be performed" ON)

#Fastens build a lot and simply gtest usage
set(LEVELDB_BUILD_TESTS OFF)
set(LEVELDB_BUILD_BENCHMARKS  OFF)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(Key-value-lseq
        src/main.cpp
        src/utils/yamlConfig.cpp
        src/db/dbConnector.cpp
        )

#Setup leveldb
add_subdirectory(third-party/leveldb)
target_link_libraries(Key-value-lseq PUBLIC leveldb)

#Setup yaml parser
add_subdirectory(third-party/yaml-cpp)
target_link_libraries(Key-value-lseq PUBLIC yaml-cpp)

if (BuildGRPC)
    #Simply install gRPC
    add_subdirectory(third-party/)
endif()

#Using GTest is equivalent to performing testing
if (UseGTest)

    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
            )
    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
            )

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This adds the following targets:
    # gtest, gtest_main, gmock and gmock_main
    add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
            "${CMAKE_BINARY_DIR}/googletest-build"
            )

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