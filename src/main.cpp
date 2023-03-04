#include <iostream>

#include "leveldb/db.h"
#include "utils/yamlConfig.hpp"

int main(int argc, char** argv) {
    if (argc != 2)
    {
        std::cout << "Usage: config file" << std::endl;
        return 1;
    }
    YAMLConfig config(argv[1]);
    std::cout << "test main file" << std::endl;
    return 0;
}