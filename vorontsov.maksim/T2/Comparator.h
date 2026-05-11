#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <cmath>

#include "DataStruct.h"

struct DataStructComparator {
    bool operator()(const DataStruct& lhs,
        const DataStruct& rhs) const {
        if (lhs.key1_ != rhs.key1_) {
            return lhs.key1_ < rhs.key1_;
        }

        const double lhsMod = std::abs(lhs.key2_);
        const double rhsMod = std::abs(rhs.key2_);
        if (lhsMod != rhsMod) {
            return lhsMod < rhsMod;
        }

        return lhs.key3_.length() < rhs.key3_.length();
    }
};

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs) {
    if (lhs.key1_ != rhs.key1_) {
        return lhs.key1_ < rhs.key1_;
    }
    const double lhsMag = std::abs(lhs.key2_);
    const double rhsMag = std::abs(rhs.key2_);
    if (lhsMag != rhsMag) {
        return lhsMag < rhsMag;
    }
    return lhs.key3_.length() < rhs.key3_.length();
}

#endif
