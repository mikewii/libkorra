#pragma once
#include "types.h"
#include <future>

// how to pass non const ref?
template <typename ReturnType = void, typename... ArgType>
class Task
{
    template<typename T, typename... TT>
    using FunctionPointer = T(*)(TT...);

public:
    Task( FunctionPointer<ReturnType, ArgType...> _f ) { this->__func = _f; }
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
            this->__cThread = new std::thread(this->__func, Task::get<ArgType>()(_args)...);
        }
        else
        {
            return this->__func(_args...);
        }
    }


    std::future<ReturnType> runFuture( ArgType... _args )
    {
        return std::async(this->__func, Task::get<ArgType>()(_args)...);
    }

private:
    std::thread*                            __cThread   = nullptr;
    FunctionPointer<ReturnType, ArgType...> __func      = nullptr;

    template<typename T>
    struct get      { constexpr T&& operator()( T& t ) const { return std::move(t); } };

    template<typename T>
    struct get<T&>  { constexpr T&  operator()( T& t ) const { return std::ref(t);  } };

    template<typename T>
    struct get<T&&> { constexpr T&& operator()( T& t ) const { return std::move(t); } };

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
