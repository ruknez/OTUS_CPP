#include <iostream>
#include "ip_filter.h"


int main()
{

    uIpV4 test;

    test.ipV4.v2 = 255;
    test.ipV4.v1 = 200;
    test.ipV4.v3 = 25;

    std::cout << "v1 = " << test.ipV4.v1 << std::endl;
    std::cout << "v2 = " << test.ipV4.v2 << std::endl;
    std::cout << "v3 = " << test.ipV4.v3 << std::endl;

    std::cout << "size = " << sizeof(uIpV4) << std::endl;
    std::cout << "int size = " << sizeof(int) << std::endl;

    return 0;
}