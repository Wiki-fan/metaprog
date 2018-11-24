#include <iostream>
#include <cassert>
#include "typelists-m.h"

using namespace m;

using tl = TypeList<int, float, double, char> ;


template<int n>
struct Fib{
    static const int value = Fib<n-1>::value+Fib<n-2>::value;
};

template<>
struct Fib<1> {
    static const int value = 1;
};

template<>
struct Fib<0> {
    static const int value = 1;
};

template<int fib, int n>
struct GetCurrentRow {
    const static int curFibNum = Fib<fib>::value;
    const static int curCount = curFibNum > n? n: fib;
    const static int nextCount = curFibNum > n? 0: n-curFibNum;
    using next = GetCurrentRow<fib+1, nextCount>;
};

template<int fib>
struct GetCurrentRow<fib, 0>{
};

template<int n>
struct Branch {
    using num = GetCurrentRow<1, n>;
};

struct Root {

};

int main()
{
    std::cout << "Hello, World!" << std::endl;

    static_assert(Fib<0>::value == 1);
    static_assert(Fib<0>::value == 1);
    static_assert(Fib<6>::value == 13);

    std::cout << Branch<8>::num::curCount <<'\n';
    std::cout << Branch<8>::num::next::curCount <<'\n';
    std::cout << Branch<8>::num::next::next::curCount <<'\n';
    std::cout << Branch<8>::num::next::next::next::curCount <<'\n';

    assert(typeid(Skip<2, tl>::value) == typeid(TypeList<double, char>));
    assert(typeid(Skip<1, tl>::value) == typeid(TypeList<float, double, char>));
    assert(typeid(Skip<3, tl>::value) == typeid(TypeList<char>));

    assert(typeid(Take<0, tl>::value) == typeid(TypeList<>));
    assert(typeid(Take<1, tl>::value) == typeid(TypeList<int>));
    assert(typeid(Take<2, tl>::value) == typeid(TypeList<int, float>));
    assert(typeid(Take<3, tl>::value) == typeid(TypeList<int, float, double>));

    //assert(typeid(Take<2, tl>::value::head) == typeid(int));
    //TypeList<int, TypeList<float >>;
    //assert(typeid(Take<2, tl>::value::tail::head::head) == typeid(EmptyList));
    //assert(typeid(Take<2, tl>::value::tail) == typeid(EmptyList));
    //assert(typeid(Take<3, tl>::value::head) == typeid(int));
    //assert(typeid(Take<3, tl>::value::tail::head::head) == typeid(float));

    return 0;
}
