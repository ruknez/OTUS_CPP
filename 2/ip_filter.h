#pragma once
#include <string>
#include <set>

union uIpV4 {

    uIpV4();
    struct sIpV4
    {
        unsigned short int v1 : 8;
        unsigned short int v2 : 8;
        unsigned short int v3 : 8;
        unsigned short int v4 : 8;
    } ipV4;
    unsigned int valueForComparison;
};

std::ostream &operator<<(std::ostream &stream, const uIpV4 &data);
bool operator<(const uIpV4 &rhs, const uIpV4 &lhs);

std::multiset<uIpV4> readDataFromFileToUipV4(const std::string &fileName);
uIpV4 parsStrToUipV4(const std::string &stBuffer);

#define SWAP_INT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))