#include <iostream>
#include <vector>
#include <map>

#include "my_allocator.h"
//#include "test_allocator.cpp"

int main()
{

    //std::vector<std::pair<const int, int>, logging_allocator<std::pair<const int, int>, 5>> v;

    auto newMap = std::map<int, int, std::less<int>, logging_allocator<std::pair<const int, int>, 4>>{};

    newMap[8] = 8;
    newMap[14] = 14;

    std::cout << "newMap 14 = " << newMap[14] << std::endl;

    return 0;
}
