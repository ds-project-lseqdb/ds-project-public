#pragma once

#include <iomanip>
#include <sstream>
#include <string>

#include "leveldb/db.h"

struct FullKey {
    std::string key;
    int keyLength;

    FullKey(const std::string& fullKey) : key(fullKey) {
        keyLength = static_cast<int>(fullKey.size()) - 1 - 10 - 15;
    }

    FullKey(const std::string& key, leveldb::SequenceNumber seq, int id) {
        std::stringstream ss;
        //seq number
        ss << std::setw(15) << std::setfill('0') << seq;
        //id - up to 256^3
        ss << std::setw(10) << std::setfill('0') << id;
        this->key = std::string("!") + key + ss.str();
        keyLength = key.size();
    }

    std::string getFullKey() const {
        return key;
    }

    std::string getKey() const {
        return key.substr(1, keyLength);
    }

    leveldb::SequenceNumber getSeq() const {
        return std::stoll(key.substr(1 + keyLength, 15));
    }

    int getReplicaId() const {
        return std::stoi(key.substr(1 + keyLength + 15));
    }
};
