#include "dbConnector.hpp"

#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <utility>
#include <variant>

#include "leveldb/db.h"
#include "leveldb/write_batch.h"
#include "src/utils/yamlConfig.hpp"

dbConnector::dbConnector(YAMLConfig config)
{
    selfId = config.getId();
    std::string filename = config.getDbFile();
    leveldb::Options options;
    options.create_if_missing = true;
    db = nullptr;
    leveldb::Status status = leveldb::DB::Open(options, filename, &db);
    assert(status.ok());
}

dbConnector::~dbConnector()
{
    delete db;
}

replyFormat dbConnector::put(std::string key, std::string value) {
    std::string realKey = generateNormalKey(std::move(key), selfId);
    auto [seq, s] = db->PutSequence(leveldb::WriteOptions(), realKey, value);
    useconds_t delay = 100000;
    while (true)
    {
        if (!s.ok()) {
            return {0, s};
        }
        leveldb::Status intermediateStatus = db->Put(leveldb::WriteOptions(), generateGetseqKey(realKey), generateLseqKey(seq, selfId));
        if (!intermediateStatus.ok()) {
            return {0, intermediateStatus};
        }
        auto [secondSeq, s] = db->PutSequence(leveldb::WriteOptions(), generateLseqKey(seq, selfId), realKey);
        if (secondSeq != seq + 2) {
            usleep(delay + rand() % 100000);
            auto [inSeq, inS] = db->DeleteSequence(leveldb::WriteOptions(), realKey);
            seq = inSeq;
            s = inS;
            continue;
        }
        break;
    }

    return {generateLseqKey(seq, selfId), s};
}

replyFormat dbConnector::remove(std::string key) {
    std::string realKey = generateNormalKey(std::move(key), selfId);
    auto [seq, s] = db->DeleteSequence(leveldb::WriteOptions(), realKey);
    useconds_t delay = 100000;
    while (true)
    {
        if (!s.ok()) {
            return {0, s};
        }
        leveldb::Status intermediateStatus = db->Put(leveldb::WriteOptions(), generateGetseqKey(realKey), generateLseqKey(seq, selfId));
        if (!intermediateStatus.ok()) {
            return {0, intermediateStatus};
        }
        auto [secondSeq, s] = db->DeleteSequence(leveldb::WriteOptions(), generateLseqKey(seq, selfId));
        if (secondSeq != seq + 2) {
            usleep(delay + rand() % 100000);
            auto [inSeq, inS] = db->DeleteSequence(leveldb::WriteOptions(), realKey);
            seq = inSeq;
            s = inS;
            continue;
        }
        break;
    }

    return {generateLseqKey(seq, selfId), s};
}

pureReplyValue dbConnector::get(std::string key) {
    int cnt = 0;
    std::string realKey = generateNormalKey(std::move(key), selfId);
    std::string lseqKey = generateGetseqKey(realKey);
    std::string subSearchKey;

    while (true)
    {
        ++cnt;
        leveldb::ReadOptions options;
        options.snapshot = db->GetSnapshot();
        //MAXREAD constant
        if (cnt > 100) {
            return {"", leveldb::Status::IOError("Unsuccessfully read. Retry when db load decreases"), std::string()};
        }
        std::string value;
        auto [seq, s] = db->GetSequence(options, realKey, &value);
        if (!s.ok()) {
            return {"", s, std::string()};
        }
        s = db->Get(options, lseqKey, &subSearchKey);
        if (!s.ok() && !s.IsNotFound()) {
            return {"", s, ""};
        }
        std::string requestedKey;
        s = db->Get(options, subSearchKey, &requestedKey);
        if (!s.ok() && !s.IsNotFound()) {
            return {"", s, ""};
        }
        if (requestedKey != realKey) {
            continue;
        }
        return {subSearchKey, s, value};
    }
}

pureReplyValue dbConnector::get(std::string key, int id) {
    if (id == selfId)
        return get(std::move(key));
    std::string res;
    leveldb::ReadOptions options;
    options.snapshot = db->GetSnapshot();
    std::string realKey = generateNormalKey(std::move(key), id);
    auto [seq, s] = db->GetSequence(options, realKey, &res);
    if (!s.ok()) {
        return {"", s, ""};
    }
    std::string lseq;
    s = db->Get(options, generateGetseqKey(realKey), &lseq);
    return {lseq, s, res};
}

//UNTESTED
//UNSAFE, this method can override existing value
//Should never be called with unchecked value
//Argument should contain precise keys from another replica
leveldb::Status
dbConnector::putBatch(batchValues keyValuePairs) {
    leveldb::WriteBatch batch;
    for (auto [lseq, key, value] : keyValuePairs) {
        batch.Put(lseq, key);
        batch.Put(key, value);
        batch.Put(generateGetseqKey(key), lseq);
    }
    leveldb::Status s = db->Write(leveldb::WriteOptions(), &batch);
    return s;
}

//UNTESTED
replyBatchFormat dbConnector::getByLseq(leveldb::SequenceNumber seq, int id, int limit) {
    batchValues res;
    int cnt = -1;
    leveldb::ReadOptions options;
    options.snapshot = db->GetSnapshot();
    leveldb::Iterator* it = db->NewIterator(options);
    for (it->Seek(generateLseqKey(seq, id));
         it->Valid() && cnt <= limit;
         it->Next())
    {
        if (limit != -1)
            ++cnt;
        std::string realKey = it->value().ToString();
        realKey[0] = '0';
        std::string  realValue;
        auto s = db->Get(options, realKey, &realValue);
        if (!s.ok())
            return {s, res};
        res.push_back({it->key().ToString(), it->value().ToString(), realValue});
    }
    leveldb::Status status = it->status();
    db->ReleaseSnapshot(options.snapshot);
    delete it;
    return {status, res};
}

std::string dbConnector::generateLseqKey(leveldb::SequenceNumber seq, int id) {
    std::string lseqNumber = idToString(id);
    lseqNumber[0] = '#';
    return  lseqNumber + std::to_string(seq);
}

std::string dbConnector::generateNormalKey(std::string key, int id) {
    return idToString(id) + std::move(key);
}

std::string dbConnector::idToString(int id) {
    std::stringstream ss;
    //id - up to 256^3
    ss << std::setw(10) << std::setfill('0') << id;
    return ss.str();
}

std::string dbConnector::generateGetseqKey(std::string realKey) {
    realKey[0] = '@';
    return realKey;
}
