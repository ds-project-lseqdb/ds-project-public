#pragma once

#include <string>

#include "yaml-cpp/yaml.h"

class YAMLConfig {
public:
    YAMLConfig(std::string s);

    YAML::Node get() const;

    YAML::Node& getReference();

private:
    YAML::Node value;
};