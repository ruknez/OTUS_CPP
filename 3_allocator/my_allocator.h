#pragma once
#include <iostream>
#include <memory>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define __DEBUGE__

namespace
{
    template <class X>
    struct DeleterForAllocator
    {
        void operator()(X *memPtp)
        {
            std::cout << "Calling delete for logging_allocator ... \n";
            free(static_cast<void *>(memPtp));
        }
    };
} // namespace

template <class T, size_t MaxSize>
struct logging_allocator
{
private:
    std::unique_ptr<T[], DeleterForAllocator<T>> ptrToData_; // nullptr, myDeliter);
    T *currentElement = nullptr;
    size_t counter = 0;
    // const size_t MaxSize = 5;

public:
    using value_type = T;

    using pointer = T *;               // (deprecated in C++17)(removed in C++20)
    using const_pointer = const T *;   // (deprecated in C++17)(removed in C++20)
    using reference = T &;             // (deprecated in C++17)(removed in C++20)
    using const_reference = const T &; //(deprecated in C++17)(removed in C++20)

    template <typename U> // (deprecated in C++17)(removed in C++20)
    struct rebind
    {                                                // для дополнительных данных (мето информации)
        using other = logging_allocator<U, MaxSize>; // зависит  от компелятора
    };

    //=============================================================================
    logging_allocator() : ptrToData_(nullptr, DeleterForAllocator<T>())
    {
        std::cout << "CONSTRUCTOR" << std::endl;
        auto memPtr = static_cast<T *>(std::malloc(MaxSize * sizeof(T)));
        if (!memPtr)
            throw std::bad_alloc();

        ptrToData_.reset(memPtr); //, myDeliter);
        currentElement = ptrToData_.get();
    }

    ~logging_allocator() = default;

    //=============================================================================
    template <class U, size_t MS>
    logging_allocator(const logging_allocator<U, MS> &)
    {
        std::cout << "COPY CONSTRUCTOR" << std::endl;
    }

    //=============================================================================
    T *allocate(std::size_t n)
    { // количество элементов
#ifdef __DEBUGE__
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl
                  << std::endl;
#endif
        if (n != 1 || counter >= MaxSize)
            throw std::bad_alloc();

        return (currentElement + counter++);
    }

    //=============================================================================
    void deallocate(T *p, std::size_t n)
    {
#ifdef __DEBUGE__
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl
                  << std::endl;
#endif
        //std::free(p);
    }

    //=============================================================================
    //Constructs an object of type T in allocated uninitialized storage
    // pointed to by p, using placement-new
    template <typename U, typename... Args> // отличен от типа аллокатора
    void construct(U *p, Args &&... args)
    {
#ifdef __DEBUGE__
        std::cout << __PRETTY_FUNCTION__ << std::endl
                  << std::endl;
#endif
        new (p) U(std::forward<Args>(args)...);
    };

    //=============================================================================
    //Calls the destructor of the object pointed to by p
    template <typename U> // отличен от типа аллокатора
    void destroy(U *p)
    {
#ifdef __DEBUGE__
        std::cout << __PRETTY_FUNCTION__ << std::endl
                  << std::endl;
#endif
        p->~U();
    }
};