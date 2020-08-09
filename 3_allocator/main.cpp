#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <chrono>   /* for timer */
#include <string>
#include <list>
#include <forward_list>

#include "my_allocator.h"
//#include "test_allocator.cpp"

class Timer final
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;

public:
    Timer();
    Timer(const std::string &st);

    ~Timer();

private:
    const clock_t::time_point start_;
    const std::string prefix_;
};

Timer::Timer()
    : start_(clock_t::now())
{
}

Timer::Timer(const std::string &st)
    : start_(clock_t::now()), prefix_(st)
{
}

Timer::~Timer()
{
    const auto finish = clock_t::now();
    const auto us =
        std::chrono::duration_cast<microseconds>(finish - start_).count();
    std::cout << prefix_ << us << " us" << std::endl;
}

struct L
{
    int l = 1;
    int k = 2;
    double r = 3;
};

int main()
{

    //std::vector<std::pair<const int, int>, logging_allocator<std::pair<const int, int>, 5>> v;

    auto newMap = std::map<int, L, std::greater<int>, logging_allocator<std::pair<const int, L>, 8>>{};
    auto TotalMap = std::map<int, L, std::greater<int>>{};
    // newMap[8] = 8;
    // newMap[14] = 14;

    const int NMAX = 113;

    {
        Timer t("TotalMap ");
        for (int i = 0; i < NMAX; i++)
        {
            TotalMap[i] = L{i * i, i, i * 2};
        }
    }

    {
        Timer t("newMap ");
        for (int i = 0; i < NMAX; i++)
        {
            newMap[i] = L{i * i, i, i * 2};
        }
    }

    //auto it = newMap.upper_bound(5);
    // auto it2 = newMap.lower_bound(111);

    for (int i = 0; i < NMAX - 40; i++)
    {
        newMap.erase(i);
    }

    for (int i = 0; i < NMAX - 40; i++)
    {
        newMap[i] = L{i * i, i, i * 2};
    }

    //std::cerr << "LOLO = " << it->first << std::endl;

    for (const auto &m : newMap)
    {
        std::cout << " m = " << m.second.k << " ";
    }
    std::cout << "\n";

    std::list<size_t> l1;

    std::cout << "size of list = " << sizeof(l1) << std::endl;

    std::forward_list<size_t> l2;

    std::cout << "size of forward_list = " << sizeof(l2) << std::endl;
 

    return 0;
}