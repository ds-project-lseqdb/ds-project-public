#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "src/utils/yamlConfig.hpp"
#include "src/utils/grpcConfig.hpp"

TEST(configTest, GeneralConfig) {
    YAMLConfig config("resources/config.yaml");
    EXPECT_EQ(config.getDbFile(), "db");
    EXPECT_EQ(config.getId(), 2);
    EXPECT_EQ(config.getReplicas(), std::vector<std::string>({"localhost:12", "localhost:13", "localhost:14"}));
}

TEST(configTest, GRPCConfig) {
    YAMLConfig config("resources/config.yaml");
    EXPECT_EQ(config.getGRPCConfig().port, 8888);
}