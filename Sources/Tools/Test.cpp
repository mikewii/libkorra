/*
 * Just random tests
*/
#pragma once

#include "Tools/Test.hpp"
#include "Tools/Task.hpp"

#include <iostream>
#include <proc/readproc.h>



class AAA
{
public:
    AAA(){ AAA::singleton = this; }

    void callMe( void ) { printf("you did it!\n"); }

    static AAA* getSingleton( void ) { return singleton; }

private:
    static AAA* singleton;

};
AAA* AAA::singleton = nullptr;

class Caller
{
public:
    template<class T>
    void sure(std::function<void(T&)> fun)
    {
        auto& ref = *T::getSingleton();
        fun(ref);
    }
};

void memberTest()
{
    AAA a;
    Caller c;


    c.sure<AAA>(&AAA::callMe);
}




// with `const` ref this have no point
void lambdaTest()
{
    Task<std::pair<int, int>, int, int> getComplex([](int a, int b) -> std::pair<int, int>
    {
        return { a - b , a + b };
    });

    auto out = getComplex(10, 20);

    printf("OutA: %d | OutB: %d\n", out.first, out.second);

    static u32 t = 1;
    u32 n = 1, nn = 2;


    Task<u32, u32, u32, u32, bool>  task0([](u32 a, u32 b, u32 c, bool isSum) -> u32
    {
        if ( isSum )
        {
            return a + b + c;
        }
        else
        {
            return a - b - c;
        }
    });

    printf("Result is: %d\n", task0(1, 2, 3, true));


    Task<void, u32>  task([](u32 a)
    {
        printf("value: %d\n", a + 2);
    });

    Task<void, const char*>     task1([](const char* str)
    {
        printf("Result: %s\n", str);
    });

    Task<void, u32> task3([](u32 a)
    {
        std::this_thread::sleep_for(std::chrono::seconds(++t));
        printf("test %d\n", a);
    });

    // passed
    Task<u32, u32>              move0([](u32 a) -> u32 { return a++; });
    Task<u32, u32, u32, u32>    move1([](u32 a, u32 b, u32 c) -> u32 { return a + b + c; });


    // passed, cant check all references in future
    Task<u32, const u32&, const u32&>       ref2([](const u32& a, const u32& b) -> u32 { return a + b; });

    double p1 = 4.f / 5.f;
    double p2 = 2.f / 3.f;
    double ress1 = p1 / p2;
    double ress2 = 6.f / 5.f;
    printf("calculus res: %f %f\n", ress1, ress2);

    printf("after ref0 : %d\n", n);                 // &n == 2
    printf("ref2 : %d\n", ref2(n, n));              // &n == 4

    auto res0 = move1(n, n, n);                     // n == 12
    auto res1 = move0.runFuture(1);                 // undefined

    printf("move1: %d\n", move1(n, n, n));          // n == 12
    printf("move0: %d\n", move0(n));                // n == 4
    printf("move1 | blocking: %d\n", res0);         // n == 12
    printf("move0 | future  : %d\n", res1.get());   // undefined


    Task<bool, u32, u32>      task2([](u32 a, u32 b) -> bool
    {
        return a > b ? true : false;
    });

    /*##########*/

    task1("first");
    task3(t);

    if ( task2(nn, n) )
    {
        task3(t);
        task1("second");
    }

    task3(t);

    static bool isDone = false;

    if ( !isDone )
    {
        isDone = true;

        Task<void> a(lambdaTest);
        a();
    }
}








// virtual:
//struct Shared
//{
//    int a;
//};

//class A : virtual public Shared
//{
//public:
//    int sum() const { return this->a + 5; }
//};

//class B : virtual public Shared
//{
//public:
//    int sum() const { return this->a + 10; }
//};

//class C : A, B
//{
//public:
//    C()
//    {
//        A::a = 1;
//        B::a = 2;
//    }

//    void print()
//    {
//        printf("A a: %d | B a: %d\n", A::a, B::a);
//        printf("SUM A: %d | B: %d\n", A::sum(), B::sum());
//    }
//};



namespace TEST {

void Timer::start( void )
{
    this->timer.first = this->getTime();
}

void Timer::stop( void )
{
    std::chrono::duration<double> elapsed;


    this->timer.second = this->getTime();

    elapsed = this->timer.second - this->timer.first;

    printf("Operation took: %g\n", elapsed.count());
}

void referenceAssign(int& a)
{
    a++;
}

int copyAssign(int a)
{
    return ++a;
}

void copyAsignTest( void )
{
    u32 count = 1000000;
    std::vector<int> vec(count, 99);
    Timer t;


    auto lfun = [&]() -> bool {
        printf("c: %d\n", ++count);
        return count > 100000 ? true : false;
    };

    t.start();

    for ( auto& a : vec )
    {
        referenceAssign(a);

        //a = copyAssign(a);
    }

    t.stop();

    printf("count type: %s\n", typeid(count).name());
    printf("lfun type: %s\n", typeid(lfun).name());
    printf("vec last: %d\n", vec.back());

    lfun();
    lfun();
    lfun();
//    lfun(count);
//    lfun(count);
//    lfun(count);
}

void virtualTest( void )
{
//    C c;
//    c.print();
}

long getRSS( void )
{
    pid_t       pid = getpid();
    proc_t      proc{0};
    PROCTAB*    proctab = nullptr;

    proctab = openproc(PROC_FILLMEM| PROC_PID, &pid);

    if ( proctab )
    {
        readproc(proctab, &proc);
        closeproc(proctab);
    }

    return ( proc.resident * sysconf(_SC_PAGESIZE) );
}


void timeTest( void )
{
#define REPEAT_TIMES 9
    using Time = std::chrono::duration<double>;

    std::vector<std::pair<Time, long>>   result;
    Time                elapsed_seconds;
    Time                average = std::chrono::nanoseconds(0);
    const auto*         test    = "this is a test";
    const auto          count   = 1000000;


//#define RESIZE
#ifdef RESIZE
    for( auto i = 0; i < REPEAT_TIMES; i++ )
    {
        auto    start   = std::chrono::system_clock::now();
        auto*   temp    = new std::vector<std::string>;

        /* load */
        temp->resize(count);
        for( auto& a : *temp )
            a = test;
        /* load */

        /* time */
        auto end = std::chrono::system_clock::now();

        elapsed_seconds = end - start;

        result.push_back( {elapsed_seconds, getRSS()} );
        /* time */

        delete temp;
    }
#else
    for ( auto i = 0; i < REPEAT_TIMES; i++ )
    {
        auto    start   = std::chrono::system_clock::now();
        auto*   temp    = new std::vector<std::string>;

        /* load */
        temp->reserve(count);
        for( auto i = 0; i < count; i++ )
            temp->push_back(test);
        /* load */

        /* time */
        auto end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        /* time */

        result.push_back( {elapsed_seconds, getRSS()} );

        delete temp;
    }
#endif

    u32 i = 1;
    for ( auto& res : result )
    {
        std::string bytes   = std::to_string(res.second);
        std::string kb      = std::to_string(res.second / 1024);
        std::string mb      = std::to_string(res.second / 1024 / 1024);

        average += res.first;

        printf("\n#%d Operation took: %g\n", i, res.first.count());
        printf("#%d RSS: %sb | %skb | %smb\n", i, bytes.c_str(), kb.c_str(), mb.c_str());
        i++;
    }

    average /= result.size();

    std::cout << "Average: " << average.count() << "s\n";

    printf("Diffrence: %lu\n", result.at(1).second - result.at(0).second);
}



template <typename... Args>
void doPrint(Args... args)
{
    using expander = int[];
    (void)expander{0, (void(std::cout << ',' << std::forward<Args>(args)), 0)...};
}

void ff(std::thread& t)
{
    t.join();
}

template< typename... Args >
class variadics
{
public:


    template<typename T>
//    typename std::enable_if<std::is_literal_type<T>::value, void>::type
//    //void wrap( typename std::remove_reference<T>::type& t)
    T& check( T& t)
    {
        auto a = 3;

        if ( a == 1 )
        {
            if ( std::is_literal_type<T>::value ) printf("is_literal_type ");
            if ( std::is_reference<T>:: value ) printf("is_reference ");
            if ( std::is_lvalue_reference<T>::value ) printf("is_lvalue_reference ");
            if ( std::is_rvalue_reference<T>::value ) printf("is_rvalue_reference ");
            if ( std::is_pointer<T>::value ) printf("is_pointer ");
            if ( std::is_object<T>::value ) printf("is_object ");
        }
        else if ( a == 2 )
        {
            if ( std::is_literal_type<std::remove_reference<T>>::value ) printf("is_literal_type ");
            if ( std::is_reference<std::remove_reference<T>>::value ) printf("is_reference ");
            if ( std::is_lvalue_reference<std::remove_reference<T>>::value ) printf("is_lvalue_reference ");
            if ( std::is_rvalue_reference<std::remove_reference<T>>::value ) printf("is_rvalue_reference ");
            if ( std::is_pointer<std::remove_reference<T>>::value ) printf("is_pointer ");
            if ( std::is_object<std::remove_reference<T>>::value ) printf("is_object ");
        }
        else if ( a == 3 )
        {
            if ( std::is_literal_type<typename std::decay<T>::type>::value ) printf("is_literal_type ");
            if ( std::is_reference<typename std::decay<T>::type>::value ) printf("is_reference ");
            if ( std::is_lvalue_reference<typename std::decay<T>::type>::value ) printf("is_lvalue_reference ");
            if ( std::is_rvalue_reference<typename std::decay<T>::type>::value ) printf("is_rvalue_reference ");
            if ( std::is_pointer<typename std::decay<T>::type>::value ) printf("is_pointer ");
            if ( std::is_object<typename std::decay<T>::type>::value ) printf("is_object ");
        }
        printf("\n");

        return std::ref(t);
    }

    void operator()( Args&&... args)
    {
        printf("##### variadic start #####\n");
        ( this->check<Args>(args), ... );
        printf("##### variadic end #####\n");
    }

    //    template<typename T>
    //    T&
    //    get( typename std::remove_reference<T>::type& t)
    //    {
    //        printf("ref\n");
    //        return std::ref(t);
    //    }

    //    template<typename T>
    //    T&&
    //    get( typename std::remove_reference<T>::type&& t)
    //    {
    //        printf("move\n");
    //        return std::move(t);
    //    }

#define TYPE 1
#if TYPE == 0
    template<typename T>
    typename std::enable_if<std::is_rvalue_reference<T>::value, T&&>::type
    get(T& t)
    {
        printf("r_move\n");
        return std::move(t);
    }
    template<typename T>
    typename std::enable_if<std::is_object<T>::value, T&>::type
    get(T& t)
    {
        printf("o_ref\n");
        return std::ref(t);
    }
    template<typename T>
    typename std::enable_if<std::is_lvalue_reference<T>::value, T&>::type
    get(T& t)
    {
        printf("l_ref\n");
        return std::ref(t);
    }
#elif TYPE == 1
    template<typename T>
    struct get      { constexpr T&& operator()(T& t) const { return std::move(t); } };

    template<typename T>
    struct get<T&>  { constexpr T&  operator()(T& t) const { return std::ref(t);  } };

    template<typename T>
    struct get<T&&> { constexpr T&& operator()(T& t) const { return std::move(t); } };
#elif TYPE == 2
    template<typename T>
    T&& get(T&& t) const{ return std::move(t); }

    template<typename T>
    T get(T& t) const { return std::reference_wrapper<T>( std::ref(t) ); }

//    template<typename T>
//    T&& get(T& t) const { return std::move(t); }
#endif






    std::thread getcall( Args... args)
    {
        return std::thread(doPrint<Args...>, get<Args>()(args)...);
    }
};





void isTest()
{
    printf("true: %s\n", Utils::bool_to_str<std::is_void<void>::value>::value);
    printf("false: %s\n", Utils::bool_to_str<false>::value);
}


void runTests( void )
{
    int aaa = 14;

    variadics<int, int&, int&&>()(10, aaa, 10);

    auto t0 = variadics<int&&, int, const int&>()
            .getcall( 42, aaa, aaa );

    [&](){ t0.join(); }();

    //isTest();
    //memberTest();
    //lambdaTest();
    //copyAsignTest();
    //virtualTest();
    //timeTest();
}

} // TEST
