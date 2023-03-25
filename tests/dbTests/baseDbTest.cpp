#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#if defined(__GNUC__ ) && __GNUC__  < 8
#include <experimental/filesystem>
using namespace std::experimental::filesystem;
#else
#include <filesystem>
using namespace std::filesystem;
#endif

#include "src/utils/yamlConfig.hpp"
#include "src/utils/grpcConfig.hpp"
#include "src/db/dbConnector.hpp"
#include "leveldb/db.h"

class baseDbTest : public ::testing::Test {
protected:
    void SetUp() override {
        YAMLConfig config("resources/config.yaml");
        std::string fileName = config.getDbFile();
        remove_all(fileName);
    }

    void TearDown() override {
        YAMLConfig config("resources/config.yaml");
        std::string fileName = config.getDbFile();
        remove_all(fileName);
    }
};

TEST(baseDbTest, baseCorrectness) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    EXPECT_TRUE(db.put("testval", "testdata").second.ok());

    //get should return same seq for same values
    EXPECT_EQ(std::get<0>(db.get("testval")), std::get<0>(db.get("testval")));

    //get on nonexistent key
    EXPECT_TRUE(std::get<1>(db.get("asghsqag")).IsNotFound());
}

TEST(baseDbTest, putSeqResult) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    std::string firstLseq = db.put("aasgas", "bsagas").first;
    EXPECT_EQ(dbConnector::lseqToSeq(firstLseq), db.sequenceNumberForReplica(2));
    std::string secondLseq = db.put("gsagaa", "bsagas").first;
    EXPECT_EQ(dbConnector::lseqToSeq(secondLseq), db.sequenceNumberForReplica(2));
    EXPECT_TRUE(firstLseq < secondLseq);
}

TEST(baseDbTest, putsAndGets) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    EXPECT_TRUE(db.put("a", "b").second.ok());
    EXPECT_TRUE(std::get<1>(db.get("a")).ok());
    EXPECT_EQ(std::get<2>(db.get("a")), "b");
    EXPECT_TRUE(db.put("c", "d").second.ok());

    EXPECT_EQ(std::get<2>(db.get("a")), "b");
    EXPECT_EQ(std::get<2>(db.get("c")), "d");
}

TEST(baseDbTest, putSameValues) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    EXPECT_TRUE(db.put("a2", "b2").second.ok());
    EXPECT_EQ(std::get<2>(db.get("a2")), "b2");
    EXPECT_TRUE(db.put("a2", "d2").second.ok());

    EXPECT_EQ(std::get<2>(db.get("a2")), "d2");
}

TEST(baseDbTest, putAndDelete) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    EXPECT_TRUE(db.put("a3", "b3").second.ok());
    EXPECT_EQ(std::get<2>(db.get("a3")), "b3");
    EXPECT_TRUE(db.remove("a3").second.ok());

    EXPECT_TRUE(std::get<1>(db.get("a3")).IsNotFound());

    EXPECT_TRUE(db.put("a3", "c3").second.ok());
    EXPECT_EQ(std::get<2>(db.get("a3")), "c3");
}

TEST(baseDbTest, getLSEQKey) {
    YAMLConfig config("resources/config.yaml");
    dbConnector db(std::move(config));
    std::string lseq = db.put("a4", "b4").first;
    EXPECT_EQ(std::get<0>(db.get("a4")), lseq);

    lseq = db.put("a4", "c4").first;
    EXPECT_EQ(std::get<0>(db.get("a4")), lseq);

}