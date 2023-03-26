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

TEST(restartDbTest, baseCorrectness) {
    std::string secondLseq;
    YAMLConfig config = YAMLConfig("resources/config.yaml");
    {
        std::string fileName = config.getDbFile();
        std::filesystem::remove_all(fileName);
        dbConnector db = dbConnector(config);
        std::string firstLseq = db.put("aasgas", "bsagas").first;
        secondLseq = db.put("gsagaa", "bsagas").first;
        EXPECT_TRUE(firstLseq<secondLseq);
    }

    {
        dbConnector db = dbConnector(config);
        EXPECT_EQ(db.sequenceNumberForReplica(2), dbConnector::lseqToSeq(secondLseq));
    }

    std::string fileName = config.getDbFile();
    std::filesystem::remove_all(fileName);

}
