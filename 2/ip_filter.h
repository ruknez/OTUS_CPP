#pragma once
#include <string>

union uIpV4 {

    struct sIpV4
    {
        unsigned short int v1 : 8;
        unsigned short int v2 : 8;
        unsigned short int v3 : 8;
        unsigned short int v4 : 8;
    } ipV4;
    int valueForComparison;
};

uIpV4 readDataFromFileToUipV4(const std::string &fullFileName);
