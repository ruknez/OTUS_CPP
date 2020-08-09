#pragma once
#include <iostream>
#include <memory>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

//#define __DEBUGE__

namespace
{
    template <class X>
    struct DeleterForAllocator
    {
        void operator()(X *memPtp)
        {
            //std::cout << "Calling delete for logging_allocator ... \n";
            free(static_cast<void *>(memPtp));
        }
    };
} // namespace

template <class T, size_t MaxSize>
struct logging_allocator
{
private:
    using uniqPtr = std::unique_ptr<T[], DeleterForAllocator<T>>;

    std::map<size_t, std::pair<uniqPtr, size_t>, std::greater<size_t>> mainMemRepository_;
    //std::map<uniqPtr ,size_t, std::greater<uniqPtr>> mainMemRepository_;
    decltype(mainMemRepository_.begin()) currentMemBloc_;

    void addNewMemBloc()
    {
        auto memPtr = static_cast<T *>(std::malloc(MaxSize * sizeof(T)));
        if (!memPtr)
            throw std::bad_alloc();

        //std::cout << "memPte = " << reinterpret_cast<size_t>(memPtr) << std::endl;

        // было  бы неплохо провеять на успе  // 17 стандарт что он не может догадаться что тут пара ???? (очевидно же)
        std::tie(currentMemBloc_, std::ignore) = mainMemRepository_.emplace(std::make_pair(reinterpret_cast<size_t>(memPtr), std::make_pair(uniqPtr{memPtr, DeleterForAllocator<T>()}, 0)));
    }

public:
    using value_type = T;

    using pointer = T *;               // (deprecated in C++17)(removed in C++20)
    using const_pointer = const T *;   // (deprecated in C++17)(removed in C++20)
    using reference = T &;             // (deprecated in C++17)(removed in C++20)
    using const_reference = const T &; //(deprecated in C++17)(removed in C++20)

    template <class U> // (deprecated in C++17)(removed in C++20)
    struct rebind
    {                                                // для дополнительных данных (мето информации)
        using other = logging_allocator<U, MaxSize>; // зависит  от компелятора
    };

    //=============================================================================
    logging_allocator() //: mainMemRepository_({(nullptr, DeleterForAllocator<T>()), 0})//,currentMemBloc_(mainMemRepository_.begin())
    {
        //        std::cout << "CONSTRUCTOR" << std::endl;
        addNewMemBloc();
    }

    logging_allocator(const logging_allocator<T, MaxSize> & al)
    {
         std::cout << "COPY CONSTRUCTOR" << std::endl;
    }

    //=============================================================================
    ~logging_allocator()
    {
        // for (const auto &it : mainMemRepository_)
        // {
        //     std::cout << "it = " << reinterpret_cast<size_t>(it.second.first.get()) << std::endl;
        // }
    }

    // template <class U, size_t MaxSize1>
    // logging_allocator<U, MaxSize1> select_on_container_copy_construction()
    // {
    //     logging_allocator<U, MaxSize1> r;
    //     return r;
    // }

    //=============================================================================
    template <class U, size_t MS>
    logging_allocator(const logging_allocator<U, MS> &alloc)
    {
        std::cout << "COPY CONSTRUCTOR" << std::endl;
    }

    //=============================================================================
    T *allocate(std::size_t n) // количество элементов
    {
#ifdef __DEBUGE__
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl
                  << std::endl;
#endif
        if (n != 1)
            throw std::bad_alloc();
        if (currentMemBloc_->second.second >= MaxSize)
        {
            addNewMemBloc();
        }
        //        std::cout << "counter = " << currentMemBloc_->second.second << std::endl;
        return (currentMemBloc_->second.first.get() + currentMemBloc_->second.second++);
    }

    //=============================================================================
    void deallocate(T *p, std::size_t n)
    {

#ifdef __DEBUGE__
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl
                  << std::endl;
#endif
        if (n != 1)
            throw("Cannot deallocate n = " + std::to_string(n) + "\n");

        //        std::cout << "p = " << size_t(p) << std::endl;
        auto it = mainMemRepository_.lower_bound(reinterpret_cast<size_t>(p));
        it->second.second--;
        if (it->second.second <= 0)
        {
            mainMemRepository_.erase(it);
        }
        //        std::cout << "deallocate mainMemRepository_ size = " << mainMemRepository_.size() << std::endl;
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