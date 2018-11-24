#include <iostream>
#include <cassert>
#include "typelists-m.h"
#include "hierarchy-m.h"

using namespace m;

using tl = TypeList<int, float, double, char> ;
using tl8 = TypeList<int, int, int, int, int, int, int, int> ;


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

template<typename parent, typename Base>
struct InheritanceProxy: public parent, public Base {

};

template<int fib, int n, typename TList>
struct GetCurrentRow {
    const static int curFibNum = Fib<fib>::value;
    const static int curCount = curFibNum > n? n: curFibNum;
    const static int nextCount = curFibNum > n? 0: n-curFibNum;
    using curTypeList = Take<curCount, TList>;
    using next = GetCurrentRow<fib+1, nextCount, Skip<curCount, TList>>;
    using parent = GenLinearHierarchy<curTypeList, InheritanceProxy, NullType>; // TODO: not NullType should be here
};

template<int fib, typename TList>
struct GetCurrentRow<fib, 0, TList>{
};

template<int n, typename TList>
struct Branch { // TODO: looks like it must be ScatterHierarchy
    using num = GetCurrentRow<0, n, TList>;
};

struct Root {

};

int main()
{
    std::cout << "Hello, World!" << std::endl;

    static_assert(Fib<0>::value == 1);
    static_assert(Fib<0>::value == 1);
    static_assert(Fib<6>::value == 13);

    static_assert(Branch<8, tl8>::num::curCount == 1);
    static_assert(Branch<8, tl8>::num::next::curCount == 1);
    static_assert(Branch<8, tl8>::num::next::next::curCount == 2);
    static_assert(Branch<8, tl8>::num::next::next::next::curCount == 3);

    assert(typeid(Skip<2, tl>::value) == typeid(TypeList<double, char>));
    assert(typeid(Skip<1, tl>::value) == typeid(TypeList<float, double, char>));
    assert(typeid(Skip<3, tl>::value) == typeid(TypeList<char>));

    assert(typeid(Take<0, tl>::value) == typeid(TypeList<>));
    assert(typeid(Take<1, tl>::value) == typeid(TypeList<int>));
    assert(typeid(Take<2, tl>::value) == typeid(TypeList<int, float>));
    assert(typeid(Take<3, tl>::value) == typeid(TypeList<int, float, double>));

    return 0;
}
