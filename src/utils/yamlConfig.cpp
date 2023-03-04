#include "yamlConfig.hpp"

YAMLConfig::YAMLConfig(std::string s) {
    value = YAML::LoadFile(s);
}

YAML::Node YAMLConfig::get() const {
    return value;
}

YAML::Node& YAMLConfig::getReference() {
    return value;
}