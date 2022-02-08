#pragma once
#include "types.h"
#include <future>


// how to pass non const ref?
template <typename ReturnType = void, typename... ArgType>
struct Task
{
//#define USE_GET

    using FunctionPointer = ReturnType(*)(ArgType...);

public:
    Task( FunctionPointer _f ) { this->__func = _f; }
    ~Task()
    {
        if ( this->__cThread )
        {
            if ( this->__cThread->joinable() )
                this->__cThread->join();

            delete this->__cThread;
        }
    }


    ReturnType operator()( ArgType... _args )
    {
        if ( std::is_void<ReturnType>::value )
        {
#ifdef USE_GET
            this->__cThread = new std::thread(this->__func, Task::get<ArgType>()(_args)...);
#else
            this->__cThread = new std::thread(this->__func, std::ref(_args)...);
            //this->__cThread = new std::thread(this->__func, _args...);
#endif
        }
        else
        {
            return this->__func(_args...);
        }
    }


    std::future<ReturnType> runFuture( ArgType... _args )
    {
#ifdef USE_GET
        return std::async(this->__func, Task::get<ArgType>()(_args)...);
#else
        return std::async(this->__func, _args...);
#endif
    }

private:
    std::thread*    __cThread   = nullptr;
    FunctionPointer __func      = nullptr;

#ifdef USE_GET
    template<typename T>
    struct get      { constexpr T&& operator()( T& t ) const { return std::move(t); } };

    template<typename T>
    struct get<T&>  { constexpr T&  operator()( T& t ) const { return std::ref(t);  } };

    template<typename T>
    struct get<T&&> { constexpr T&& operator()( T& t ) const { return std::move(t); } };
#endif
};


/* preservation */
//template <size_t I = 0>
//typename std::enable_if<(I == sizeof... (ArgType)), void>::type
//unpack( ArgType... _args UNUSED) { return; }


//template <size_t I = 0>
//typename std::enable_if<(I < sizeof... (ArgType)), void>::type
//unpack( ArgType... _args )
//{
//    std::get<I>(_args...);

//    unpack<I + 1>(_args...);
//}
