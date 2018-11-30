#include <iostream>
#include <cassert>
#include <vector>
#include "typelists-m.h"
#include "hierarchy-m.h"
#include "functor_dispatch.h"


using namespace m;


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

template<typename Parent, typename Base>
struct InheritanceProxy: public Parent, public Base {
    using parent = Parent;
    using base = Base;
};

template<int fib, int n, typename TList>
struct GetCurrentRow {
    const static int fib_ = fib;
    const static int curFibNum = Fib<fib>::value;
    const static int curCount = curFibNum > n? n: curFibNum;
    const static int nextCount = curFibNum > n? 0: n-curFibNum;
    using curTypeList = typename Take<curCount, TList>::value;
    using nextTypeList = typename Skip<curCount, TList>::value;
    using next = GetCurrentRow<fib+1, nextCount, typename Skip<curCount, TList>::value>;
    using hierarchy = GenLinearHierarchy<curTypeList, InheritanceProxy, NullType>;
};

template<int fib, typename TList>
struct GetCurrentRow<fib, 0, TList>{
};

template<int n, typename TList>
struct Branch {
    using num = GetCurrentRow<0, n, TList>;
};


template<class TList, int fib, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs;

template<typename T1, typename... T2, int fib, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs<TypeList<T1, T2...>, fib, n, Unit>
    : public GetCurrentRow<fib, n, TypeList<T1, T2...>>::hierarchy,
      public GenScatterHierarchyOurs<
          typename GetCurrentRow<fib, n, TypeList<T1, T2...>>::nextTypeList,
          GetCurrentRow<fib, n, TypeList<T1, T2...>>::fib_+1,
          GetCurrentRow<fib, n, TypeList<T1, T2...>>::nextCount,
          Unit>
{
 public:
    using parent1 = typename GetCurrentRow<fib, n, TypeList<T1, T2...>>::hierarchy;
    using parent2 = GenScatterHierarchyOurs<
        typename GetCurrentRow<fib, n, TypeList<T1, T2...>>::nextTypeList,
        GetCurrentRow<fib, n, TypeList<T1, T2...>>::fib_+1,
        GetCurrentRow<fib, n, TypeList<T1, T2...>>::nextCount,
        Unit>;
};

template<typename AtomicType, int fib, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs
    : public GetCurrentRow<fib, n, TypeList<AtomicType>>::hierarchy {
 public:
    using parent = typename GetCurrentRow<fib, n, TypeList<AtomicType>>::hierarchy;
};

template<int fib, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs<EmptyList, fib, n, Unit> {
};

template<class TList, int n>
struct Root : public GenScatterHierarchyOurs<TList, 0, n, InheritanceProxy> {
    using parent = GenScatterHierarchyOurs<TList, 0, n, InheritanceProxy>;
};

using tl = TypeList<int, float, double, char>;

int main()
{
    std::cout << "Hello, World!" << std::endl;

    static_assert(Fib<0>::value == 1);
    static_assert(Fib<0>::value == 1);
    static_assert(Fib<6>::value == 13);

    static_assert(Branch<8, TL8>::num::curCount == 1);
    static_assert(Branch<8, TL8>::num::next::curCount == 1);
    static_assert(Branch<8, TL8>::num::next::next::curCount == 2);
    static_assert(Branch<8, TL8>::num::next::next::next::curCount == 3);

    assert(typeid(Skip<2, tl>::value) == typeid(TypeList<double, char>));
    assert(typeid(Skip<1, tl>::value) == typeid(TypeList<float, double, char>));
    assert(typeid(Skip<3, tl>::value) == typeid(TypeList<char>));

    assert(typeid(Take<0, tl>::value) == typeid(TypeList<>));
    assert(typeid(Take<1, tl>::value) == typeid(TypeList<int>));
    assert(typeid(Take<2, tl>::value) == typeid(TypeList<int, float>));
    assert(typeid(Take<3, tl>::value) == typeid(TypeList<int, float, double>));


    TestLinearHierarchy();

    // Проверка, что это разворачивается в несколько веток с нужными параметрами
    assert(typeid(Root<TL8, 8>::parent) == typeid(GenScatterHierarchyOurs<TL8, 0, 8, InheritanceProxy>));
    assert(typeid(Root<TL8, 8>::parent::parent1) == typeid(GetCurrentRow<0, 8, TL8>::hierarchy));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent1) == typeid(GetCurrentRow<1, 7, Skip<1, TL8>::value>::hierarchy));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent2::parent1) == typeid(GetCurrentRow<2, 6, Skip<2, TL8>::value>::hierarchy));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent2::parent2::parent1) == typeid(GetCurrentRow<3, 4, Skip<4, TL8>::value>::hierarchy));

    // Проверка, что все элементы TypeList таки стали родителями Root, и что в ветке нужное количество элементов
    assert(typeid(Root<TL8, 8>::parent::parent1::final) == typeid(Int2Type<0>));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent1::final) == typeid(Int2Type<1>));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent2::parent1::current) == typeid(Int2Type<2>));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent2::parent1::next::final) == typeid(Int2Type<3>));

    // Проверка, что элементы одной ветки наследуются друг от друга.
    assert(typeid(Root<TL8, 8>::parent::parent1::parent::parent) == typeid(Int2Type<0>));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent2::parent1::parent::parent) == typeid(Int2Type<2>));
    assert(typeid(Root<TL8, 8>::parent::parent2::parent2::parent1::base::parent::parent) == typeid(Int2Type<3>));

    TestFunctor();
    TestFunctorDispatcher();

    return 0;
}
/*
#include "functor_dispatch.h"
int main() {
    TestFunctorDispatcher();
}*/
