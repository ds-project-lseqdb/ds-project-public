#include <iostream>

#include "utils/yamlConfig.hpp"
#include "src/server/grpc-server.h"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char** argv) {
    if (argc != 2)
    {
        std::cout << "Usage: config file" << std::endl;
        return 1;
    }
    YAMLConfig config(argv[1]);

    std::thread server([config](){ RunServer(config); });

    while (true) {
        std::this_thread::sleep_for(5000ms);
        std::cout << "tick sync" << std::endl;
    }

    return 0;
}