#pragma once

#include <string>
#include <utility>
#include <vector>
#include <variant>

#include "leveldb/db.h"
#include "src/utils/yamlConfig.hpp"
#include "src/db/comparator.hpp"

using replyFormat = std::pair<std::string, leveldb::Status>;
using pureReplyValue = std::tuple<std::string, leveldb::Status, std::string>;
//lseq, key, value
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

    pureReplyValue get(std::string key);

    pureReplyValue get(std::string key, int id);

    leveldb::Status putBatch(batchValues keyValuePairs);

    replyBatchFormat getByLseq(leveldb::SequenceNumber seq, int id, int limit = -1);

    replyBatchFormat getByLseq(const std::string& lseq, int limit = -1);

    int sequenceNumberForReplica(int id) const;

    static std::string generateGetseqKey(std::string realKey);

    static std::string generateLseqKey(leveldb::SequenceNumber seq, int id);

    static std::string stampedKeyToRealKey(const std::string& stampedKey);

    static std::string generateNormalKey(std::string key, int id);

    static std::string idToString(int id);

    static std::string lseqToReplicaId(const std::string& lseq);

private:
    std::vector<int> seqCount;
    leveldb::DB* db{};

    int selfId;

};