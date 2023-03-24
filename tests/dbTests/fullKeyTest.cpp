#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "src/db/fullKey.hpp"
#include "leveldb/db.h"

TEST(fullKeyTest, classBuildAndGetTest) {
    FullKey key("a", 1, 1);
    EXPECT_EQ(key.getKey(), std::string("a"));
    EXPECT_EQ(key.getFullKey()[0], '!');
    EXPECT_EQ(key.getSeq(), 1);
    EXPECT_EQ(key.getReplicaId(), 1);
}

TEST(fullKeyTest, classBuildAndGetTestLong) {
    FullKey key("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 112541, 112221);
    EXPECT_EQ(key.getKey(), std::string("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    EXPECT_EQ(key.getFullKey()[0], '!');
    EXPECT_EQ(key.getSeq(), 112541);
    EXPECT_EQ(key.getReplicaId(), 112221);
}

TEST(fullKeyTest, keyToKey) {
    FullKey key("a", 1, 1);
    EXPECT_EQ(key.getKey(), std::string("a"));
    EXPECT_EQ(key.getFullKey()[0], '!');
    EXPECT_EQ(key.getSeq(), 1);
    EXPECT_EQ(key.getReplicaId(), 1);

    FullKey b(key.getFullKey());
    EXPECT_EQ(key.getKey(), std::string("a"));
    EXPECT_EQ(key.getFullKey()[0], '!');
    EXPECT_EQ(key.getSeq(), 1);
    EXPECT_EQ(key.getReplicaId(), 1);
}