#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <type_traits>

#include <leveldb/db.h>
#include "leveldb/comparator.h"
#include "leveldb/slice.h"

namespace leveldb {
    namespace {
        class LengthValueComparator : public leveldb::Comparator {

        public:

            const char *Name() const override { return "LengthValueComparator"; }

            int Compare(const leveldb::Slice &a, const leveldb::Slice &b) const override {
                if (a.size() < b.size())
                    return -1;
                if (a.size() > b.size())
                    return 1;
                return a.compare(b);
            }

            void FindShortestSeparator(std::string *, const leveldb::Slice &) const override {}

            void FindShortSuccessor(std::string *) const override {}
        };

        [[maybe_unused]] LengthValueComparator GLOBAL_COMPARATOR;
    }
}