#pragma once

#include <string>

#include "configVars.hpp"
#include "yaml-cpp/yaml.h"

struct GRPCConfig {
public:
    int port;
};

namespace YAML {
    template<>
    struct convert<GRPCConfig> {
        static Node encode(const GRPCConfig& rhs) {
            Node node;
            node[ConfigConstant::SELF_PORT] = rhs.port;
            return node;
        }

        static bool decode(const Node& node, GRPCConfig& rhs) {
            rhs.port = node[ConfigConstant::SELF_PORT].as<int>();
            return true;
        }
    };
}