#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <filesystem>

#include "src/utils/yamlConfig.hpp"
#include "src/utils/grpcConfig.hpp"
#include "src/db/dbConnector.hpp"
#include "leveldb/db.h"

class baseDbTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string fileName = config.getDbFile();
        std::filesystem::remove_all(fileName);
    }

    void TearDown() override {
        std::string fileName = config.getDbFile();
        std::filesystem::remove_all(fileName);
    }

    YAMLConfig config = YAMLConfig("resources/config.yaml");
    dbConnector db = dbConnector(config);
};

TEST_F(baseDbTest, baseCorrectness) {
    EXPECT_TRUE(db.put("testval", "testdata").second.ok());

    //get should return same seq for same values
    EXPECT_EQ(std::get<0>(db.get("testval")), std::get<0>(db.get("testval")));

    //get on nonexistent key
    EXPECT_TRUE(std::get<1>(db.get("asghsqag")).IsNotFound());
}

TEST_F(baseDbTest, putSeqResult) {
    std::string firstLseq = db.put("aasgas", "bsagas").first;
    EXPECT_EQ(dbConnector::lseqToSeq(firstLseq), db.sequenceNumberForReplica(2));
    std::string secondLseq = db.put("gsagaa", "bsagas").first;
    EXPECT_EQ(dbConnector::lseqToSeq(secondLseq), db.sequenceNumberForReplica(2));
    EXPECT_TRUE(firstLseq < secondLseq);
}

TEST_F(baseDbTest, putsAndGets) {
    EXPECT_TRUE(db.put("a", "b").second.ok());
    EXPECT_TRUE(std::get<1>(db.get("a")).ok());
    EXPECT_EQ(std::get<2>(db.get("a")), "b");
    EXPECT_TRUE(db.put("c", "d").second.ok());

    EXPECT_EQ(std::get<2>(db.get("a")), "b");
    EXPECT_EQ(std::get<2>(db.get("c")), "d");
}

TEST_F(baseDbTest, putSameValues) {
    EXPECT_TRUE(db.put("a2", "b2").second.ok());
    EXPECT_EQ(std::get<2>(db.get("a2")), "b2");
    EXPECT_TRUE(db.put("a2", "d2").second.ok());

    EXPECT_EQ(std::get<2>(db.get("a2")), "d2");
}

TEST_F(baseDbTest, putAndDelete) {
    EXPECT_TRUE(db.put("a3", "b3").second.ok());
    EXPECT_EQ(std::get<2>(db.get("a3")), "b3");
    EXPECT_TRUE(db.remove("a3").second.ok());

    EXPECT_TRUE(std::get<1>(db.get("a3")).IsNotFound());

    EXPECT_TRUE(db.put("a3", "c3").second.ok());
    EXPECT_EQ(std::get<2>(db.get("a3")), "c3");
}

TEST_F(baseDbTest, getLSEQKey) {
    std::string lseq = db.put("a4", "b4").first;
    EXPECT_EQ(std::get<0>(db.get("a4")), lseq);

    lseq = db.put("a4", "c4").first;
    EXPECT_EQ(std::get<0>(db.get("a4")), lseq);

}