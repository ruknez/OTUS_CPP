#include <fstream>
#include <iostream>
#include "ip_filter.h"

uIpV4 readDataFromFileToUipV4(const std::string &fullFileName)
{
    std::ifstream inputData(fullFileName, std::ifstream::in);

    std::cout << inputData.is_open();
}
