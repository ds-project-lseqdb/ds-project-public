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
    dbConnector database(config);

    std::thread server([config, &database](){ RunServer(config, &database); });

    while (true) {
        std::this_thread::sleep_for(15000ms);
//        std::cout << "tick sync" << std::endl;
        SyncLoop(config, &database);
    }

    server.join();

    return 0;
}