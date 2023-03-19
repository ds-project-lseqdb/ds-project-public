#include <iostream>

#include "db/dbConnector.hpp"
#include "utils/yamlConfig.hpp"
#include <grpc/grpc.h>

int main(int argc, char** argv) {
    if (argc != 2)
    {
        std::cout << "Usage: config file" << std::endl;
        return 1;
    }
    YAMLConfig config(argv[1]);
    dbConnector db(config);
    db.put("testval", "testdata");
    std::cout << "test main file" << std::endl;
    return 0;
}