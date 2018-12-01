#include <iostream>
#include <cassert>
#include <vector>
#include "typelists-m.h"
#include "hierarchy-m.h"


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

template<int i, int n, typename TList>
struct GetCurrentRow {
    const static int i_ = i;
    const static int curFibNum = Fib<i>::value+1;
    const static int curCount = curFibNum > n? n: curFibNum;
    const static int nextCount = curFibNum > n? 0: n-curFibNum;
    using curTypeList = typename Take<curCount, TList>::value;
    using nextTypeList = typename Skip<curCount, TList>::value;
    using next = GetCurrentRow<i+1, nextCount, typename Skip<curCount, TList>::value>;
    using hierarchy = GenLinearHierarchy<curTypeList, InheritanceProxy, NullType>;
};

template<int fib, typename TList>
struct GetCurrentRow<fib, 0, TList>{
};

template<int n, typename TList>
struct Branch {
    using num = GetCurrentRow<0, n, TList>;
};


template<class TList, int i, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs;

template<typename T1, typename... T2, int i, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs<TypeList<T1, T2...>, i, n, Unit>
    : public GetCurrentRow<i, n, TypeList<T1, T2...>>::hierarchy,
      public GenScatterHierarchyOurs<
          typename GetCurrentRow<i, n, TypeList<T1, T2...>>::nextTypeList,
          GetCurrentRow<i, n, TypeList<T1, T2...>>::i_+1,
          GetCurrentRow<i, n, TypeList<T1, T2...>>::nextCount,
          Unit>
{
 public:
    using parent1 = typename GetCurrentRow<i, n, TypeList<T1, T2...>>::hierarchy;
    using parent2 = GenScatterHierarchyOurs<
        typename GetCurrentRow<i, n, TypeList<T1, T2...>>::nextTypeList,
        GetCurrentRow<i, n, TypeList<T1, T2...>>::i_+1,
        GetCurrentRow<i, n, TypeList<T1, T2...>>::nextCount,
        Unit>;
};

template<typename AtomicType, int i, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs
    : public GetCurrentRow<i, n, TypeList<AtomicType>>::hierarchy {
 public:
    using parent = typename GetCurrentRow<i, n, TypeList<AtomicType>>::hierarchy;
};

template<int i, int n, template<class, class...> class Unit>
class GenScatterHierarchyOurs<EmptyList, i, n, Unit> {
};

template<class TList>
 struct Root : public GenScatterHierarchyOurs<TList, 0, Length<TList>::value, InheritanceProxy> {
    using parent = GenScatterHierarchyOurs<TList, 0, Length<TList>::value, InheritanceProxy>;
};


int main()
{
    std::cout << "Hello, World!" << std::endl;

    static_assert(Fib<0>::value == 1);
    static_assert(Fib<0>::value == 1);
    static_assert(Fib<6>::value == 13);

    static_assert(Branch<8, TL8>::num::curCount == 2);
    static_assert(Branch<8, TL8>::num::next::curCount == 2);
    static_assert(Branch<8, TL8>::num::next::next::curCount == 3);
    static_assert(Branch<8, TL8>::num::next::next::next::curCount == 1);

    TestLinearHierarchy();

    // Проверка, что это разворачивается в несколько веток с нужными параметрами
    assert(typeid(Root<TL8>::parent) == typeid(GenScatterHierarchyOurs<TL8, 0, 8, InheritanceProxy>));
    assert(typeid(Root<TL8>::parent::parent1) == typeid(GetCurrentRow<0, 8, TL8>::hierarchy));
    assert(typeid(Root<TL8>::parent::parent2::parent1)
        == typeid(GetCurrentRow<1, 6, Skip<2, TL8>::value>::hierarchy));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent1)
        == typeid(GetCurrentRow<2, 4, Skip<4, TL8>::value>::hierarchy));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent2::parent1)
        == typeid(GetCurrentRow<3, 1, Skip<7, TL8>::value>::hierarchy));

    // Проверка, что все элементы TypeList таки стали родителями Root, и что в ветке нужное количество элементов
    assert(typeid(Root<TL8>::parent::parent1::current) == typeid(Int2Type<0>));
    assert(typeid(Root<TL8>::parent::parent1::next::final) == typeid(Int2Type<1>));
    assert(typeid(Root<TL8>::parent::parent2::parent1::current) == typeid(Int2Type<2>));
    assert(typeid(Root<TL8>::parent::parent2::parent1::next::final) == typeid(Int2Type<3>));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent1::current) == typeid(Int2Type<4>));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent1::next::current) == typeid(Int2Type<5>));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent1::next::next::final) == typeid(Int2Type<6>));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent2::parent1::final) == typeid(Int2Type<7>));

    // Проверка, что элементы одной ветки наследуются друг от друга.
    assert(typeid(Root<TL8>::parent::parent1::parent::parent) == typeid(Int2Type<0>));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent1::parent::parent) == typeid(Int2Type<2>));
    assert(typeid(Root<TL8>::parent::parent2::parent2::parent1::base::parent::parent) == typeid(Int2Type<3>));

    return 0;
}
/*
#include "functor_dispatch.h"
int main() {
    TestFunctorDispatcher();
}*/
