#include <fstream>
#include <iostream>
#include <exception>
#include <regex>
#include <set>

#include "ip_filter.h"

uIpV4::uIpV4()
    : valueForComparison(0)
{
}

std::ostream &operator<<(std::ostream &stream, const uIpV4 &data)
{
    stream << data.ipV4.v1 << '.' << data.ipV4.v2 << '.'
           << data.ipV4.v3 << '.' << data.ipV4.v4;
    return stream;
}

bool operator<(const uIpV4 &rhs, const uIpV4 &lhs)
{
    // все из-за биг/лител представления числа
    return SWAP_INT32(rhs.valueForComparison) < SWAP_INT32(lhs.valueForComparison);
    /*
    if (rhs.ipV4.v1 != lhs.ipV4.v1)
    {
        return rhs.ipV4.v1 < lhs.ipV4.v1;
    }
    else if (rhs.ipV4.v2 != lhs.ipV4.v2)
    {
        return rhs.ipV4.v2 < lhs.ipV4.v2;
    }
    else if (rhs.ipV4.v3 != lhs.ipV4.v3)
    {
        return rhs.ipV4.v3 < lhs.ipV4.v3;
    }
    return rhs.ipV4.v4 < lhs.ipV4.v4;
    */
}

std::multiset<uIpV4> readDataFromFileToUipV4([[maybe_unused]] const std::string &fileName)
{
#ifdef __READFROMFILE__
    std::ifstream inputData(fileName, std::ifstream::in);

    if (!inputData.is_open())
    {
        throw std::logic_error("I cannot open file " + fileName + "\n");
    }
#endif

    std::multiset<uIpV4> tmpIpSet;
    std::string stBuffer = "";
#ifdef __READFROMFILE__
    while (getline(inputData, stBuffer))
#else
    while (getline(std::cin, stBuffer))
#endif
    {
        tmpIpSet.insert(parsStrToUipV4(stBuffer));
    }

    return tmpIpSet;
}

uIpV4 parsStrToUipV4(const std::string &stBuffe)
{
    std::cmatch result;
    // Начал использовать регулярку - это очень медленно :(
    std::regex regular("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");

    if (std::regex_search(stBuffe.c_str(), result, regular))
    {
        uIpV4 tmpIpV4;
        tmpIpV4.ipV4.v1 = std::stoi(result.str(1));
        tmpIpV4.ipV4.v2 = std::stoi(result.str(2));
        tmpIpV4.ipV4.v3 = std::stoi(result.str(3));
        tmpIpV4.ipV4.v4 = std::stoi(result.str(4));

        return tmpIpV4;
    }
    else
    {
        throw std::invalid_argument("it is not ip addres " + stBuffe + "\n");
    }
}