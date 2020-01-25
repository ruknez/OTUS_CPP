#include <iostream>
#include <functional>
#include <vector>

#include "ip_filter.h"
#include "gConstGlobal.h"

int main()
{
    /*
    uIpV4 test;

    test.ipV4.v1 = 46;
    test.ipV4.v2 = 70;
    test.ipV4.v3 = 0;
    test.ipV4.v3 = 0;

    std::cout << "v1 = " << test.ipV4.v1 << std::endl;
    std::cout << "v2 = " << test.ipV4.v2 << std::endl;
    std::cout << "v3 = " << test.ipV4.v3 << std::endl;
    std::cout << "v4 = " << test.ipV4.v4 << std::endl;
    std::cout << "v4 int  = " << test.valueForComparison << std::endl;

    std::cout << "size = " << sizeof(uIpV4) << std::endl;
    std::cout << "int size = " << sizeof(int) << std::endl;
*/
    try
    {
        std::multiset<uIpV4> setOfIpAddr = readDataFromFileToUipV4(gRealFileName);

        std::vector<std::function<bool(decltype(setOfIpAddr.rbegin()) ipAddr)>> poolOfTasks;
        poolOfTasks.push_back([](auto it) { return true; });
        poolOfTasks.push_back([](auto it) { return gFirstBitOne == (it->valueForComparison & 0xFF); });
        poolOfTasks.push_back([](auto it) { return gFirst46Second70 == (it->valueForComparison & 0xFFFF); });
        poolOfTasks.push_back([](auto it) { return ((gAny46 == (it->ipV4.v1)) || (gAny46 == (it->ipV4.v2)) || (gAny46 == (it->ipV4.v3)) || (gAny46 == (it->ipV4.v4))); });

        for (auto cmpFunc : poolOfTasks)
        {
            for (auto it = setOfIpAddr.rbegin(); it != setOfIpAddr.rend(); it++)
            {
                auto fcmp = poolOfTasks[0];
                if (cmpFunc(it))
                {
                    std::cout << *it << std::endl;
                }
            }
        }
    }
    catch (const std::logic_error &logErr)
    {
        std::cerr << "Problem whith file " << logErr.what();
    }
    catch (const std::invalid_argument &Err)
    {
        std::cerr << "Bad input data " << Err.what();
    }
    catch(...)
    {
        std::cerr << "Unknown exception happend \n";
    }

    return 0;
}