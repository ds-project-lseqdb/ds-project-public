#include "yamlConfig.hpp"

#include <vector>
#include <cassert>

#include "configVars.hpp"

YAMLConfig::YAMLConfig(std::string s) {
    auto value = YAML::LoadFile(s);
    grpcConfig = value[ConfigConstant::GRPC_VALUE_NAME].as<GRPCConfig>();
    //maybe assert id value
    id = value[ConfigConstant::ID].as<int>();
    maxReplicaId = value[ConfigConstant::MAX_REPLICA_ID].as<int>() + 1;
    dbFilename = value[ConfigConstant::DB_FILENAME].as<std::string>();
    assert(value[ConfigConstant::REPLICA_LIST].Type() == YAML::NodeType::Sequence);
    assert(maxReplicaId >= id);
    replicas.reserve(value[ConfigConstant::REPLICA_LIST].size());
    for (auto it : value[ConfigConstant::REPLICA_LIST])
    {
        replicas.push_back(it.as<std::string>());
    }
}

int YAMLConfig::getId() const {
    return id;
}

int YAMLConfig::getMaxReplicaId() const {
    return maxReplicaId;
}
std::string YAMLConfig::getDbFile() const {
    return dbFilename;
}

const std::vector<std::string>& YAMLConfig::getReplicas() const {
    return replicas;
}

void YAMLConfig::addReplica(std::string newReplica) {
    replicas.push_back(std::move(newReplica));
}

const GRPCConfig& YAMLConfig::getGRPCConfig() const {
    return grpcConfig;
}
