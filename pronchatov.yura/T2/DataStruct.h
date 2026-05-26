#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <string>

struct DataStruct
{
    double key1;
    unsigned long long key2;
    std::string key3;
};

struct DataStructComparator
{
    bool operator()(const DataStruct& left, const DataStruct& right) const;
};

std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);

#endif
