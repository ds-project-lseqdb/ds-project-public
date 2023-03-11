#pragma once

#include <string>
#include <utility>
#include <variant>

#include "leveldb/db.h"
#include "src/utils/yamlConfig.hpp"

using replyFormat = std::pair<std::string, leveldb::Status>;
using pureReplyValue = std::tuple<std::string, leveldb::Status, std::string>;
using batchValues = std::vector<std::tuple<std::string, std::string, std::string>>;
using replyBatchFormat = std::pair<leveldb::Status, batchValues>;

class dbConnector {
public:
    explicit dbConnector(YAMLConfig config);

    dbConnector(const dbConnector&) = delete;

    dbConnector(dbConnector&&) = default;

    ~dbConnector();

    replyFormat put(std::string key, std::string value);

    replyFormat remove(std::string key);

    //get returns sequence number. It is sequence id of db when value was gotten. Use with care
    pureReplyValue get(std::string key);

    //get returns sequence number. It is sequence id of db when value was gotten. Use with care
    pureReplyValue get(std::string key, int id);

    leveldb::Status putBatch(batchValues keyValuePairs);

    replyBatchFormat getByLseq(leveldb::SequenceNumber seq, int id, int limit = -1);
private:

    static std::string generateLseqKey(leveldb::SequenceNumber seq, int id);

    static std::string generateNormalKey(std::string key, int id);

    static std::string idToString(int id);

    leveldb::DB* db{};

    int selfId;

    static std::string generateGetseqKey(std::string realKey);
};