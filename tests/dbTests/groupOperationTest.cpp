#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "src/utils/yamlConfig.hpp"
#include "src/utils/grpcConfig.hpp"
#include "src/db/dbConnector.hpp"
#include "leveldb/db.h"

TEST(groupOperationTest, baseGroupInsert) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    //'000000000'
    EXPECT_TRUE(db.putBatch({
        {dbConnector::generateLseqKey(12, 1), dbConnector::generateNormalKey("ab", 1), "val"},
        {dbConnector::generateLseqKey(15, 1), dbConnector::generateNormalKey("ab2", 1), "val2"},
        {dbConnector::generateLseqKey(16, 1), dbConnector::generateNormalKey("abc", 1), "valc"},
        {dbConnector::generateLseqKey(19, 3), dbConnector::generateNormalKey("ab", 3), "val3"}
    }).ok());
    EXPECT_EQ(std::get<2>(db.get("ab", 1)), "val");
    EXPECT_EQ(std::get<2>(db.get("ab2", 1)), "val2");
    EXPECT_EQ(std::get<2>(db.get("abc", 1)), "valc");
    EXPECT_EQ(std::get<2>(db.get("ab", 3)), "val3");
    EXPECT_TRUE(std::get<1>(db.get("ab", 2)).IsNotFound());
}

TEST(groupOperationTest, lseqSeek) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    EXPECT_TRUE(db.putBatch({
        {dbConnector::generateLseqKey(100, 2), dbConnector::generateNormalKey("abc", 2), "val"},
        {dbConnector::generateLseqKey(200, 2), dbConnector::generateNormalKey("abc2", 2), "val2"},
        {dbConnector::generateLseqKey(300, 2), dbConnector::generateNormalKey("abcc", 2), "val3"},
        {dbConnector::generateLseqKey(400, 2), dbConnector::generateNormalKey("abcd", 2), "val4"}
    }).ok());
    replyBatchFormat repl = db.getByLseq(100, 2);
    EXPECT_TRUE(repl.first.ok());
    EXPECT_EQ(repl.second.size(), 4);
    EXPECT_EQ(std::get<2>(repl.second[0]), "val");
    EXPECT_EQ(std::get<2>(repl.second[1]), "val2");
    EXPECT_EQ(std::get<2>(repl.second[2]), "val3");
    EXPECT_EQ(std::get<2>(repl.second[3]), "val4");

    repl = db.getByLseq(101, 2);
    EXPECT_TRUE(repl.first.ok());
    EXPECT_EQ(repl.second.size(), 3);
    EXPECT_EQ(std::get<2>(repl.second[0]), "val2");
    EXPECT_EQ(std::get<2>(repl.second[1]), "val3");
    EXPECT_EQ(std::get<2>(repl.second[2]), "val4");

    repl = db.getByLseq(100, 1);
    EXPECT_TRUE(repl.first.ok());
    for (auto res : repl.second) {
        std::cerr << std::get<0>(res) << " " << std::get<1>(res) << " " << std::get<2>(res) << std::endl;
    }
    EXPECT_EQ(repl.second.size(), 0);
}