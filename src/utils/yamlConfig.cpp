#include "yamlConfig.hpp"

#include <vector>
#include <cassert>

#include "configVars.hpp"

YAMLConfig::YAMLConfig(std::string s) {
    auto value = YAML::LoadFile(s);
    grpcConfig = value[ConfigConstant::GRPC_VALUE_NAME].as<GRPCConfig>();
    //maybe assert id value
    id = value[ConfigConstant::ID].as<int>();
    dbFilename = value[ConfigConstant::DB_FILENAME].as<std::string>();
    assert(value[ConfigConstant::REPLICA_LIST].Type() == YAML::NodeType::Sequence);
    replicas.reserve(value[ConfigConstant::REPLICA_LIST].size());
    for (auto it : value[ConfigConstant::REPLICA_LIST])
    {
        replicas.push_back(it.as<std::string>());
    }
}

int YAMLConfig::getId() const {
    return id;
}

std::string YAMLConfig::getDbFile() const {
    return dbFilename;
}

std::vector<std::string> YAMLConfig::getReplicas() const {
    return replicas;
}

void YAMLConfig::addReplica(std::string newReplica) {
    replicas.push_back(std::move(newReplica));
}

GRPCConfig YAMLConfig::getGRPCConfig() {
    return grpcConfig;
}
