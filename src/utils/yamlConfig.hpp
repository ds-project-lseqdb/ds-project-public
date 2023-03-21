#pragma once

#include <string>
#include <vector>

#include "grpcConfig.hpp"

class YAMLConfig {
public:
    explicit YAMLConfig(std::string s);

    [[nodiscard]] int getId() const;

    [[nodiscard]] std::string getDbFile() const;

    [[nodiscard]] std::vector<std::string> getReplicas() const;

    [[nodiscard]] GRPCConfig getGRPCConfig() const;

    [[nodiscard]] int getMaxReplicaId() const;

    void addReplica(std::string newReplica);

private:
    GRPCConfig grpcConfig;
    std::string dbFilename;
    std::vector<std::string> replicas;
    int id;
    int maxReplicaId;
};