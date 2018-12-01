#pragma once
#include <typeinfo>
#include <cassert>

namespace m
{

struct NullType {};

template<typename T=NullType, typename... U>
struct TypeList {
    using head = T;
    using tail = TypeList<U...>;
};
using EmptyList = TypeList<>;

template<typename TypeList>
struct Length {
    enum { value = Length<typename TypeList::tail>::value + 1 };
};
template<>
struct Length<EmptyList> {
    enum { value = 0 };
};
template<>
struct Length<NullType> {
    enum { value = 0 };
};


template<typename TList, unsigned int index>
struct TypeAt;
template<typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0> {
    using Result = Head;
};
template<typename Head, typename... Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail...>, i> {
    using Result = typename TypeAt<TypeList<Tail...>, i - 1>::Result;
};


template<typename TList, typename T>
struct IndexOf;
template<typename T>
struct IndexOf<EmptyList, T> {
    enum { value = -1 };
};
template<typename T, typename... Tail>
struct IndexOf<TypeList<T, Tail...>, T> {
    enum { value = 0 };
};
template<typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T> {
 private:
    enum { temp = IndexOf<TypeList<Tail...>, T>::value };
 public:
    enum { value = temp == -1 ? -1 : 1 + temp };
};


template<typename TList, typename T>
struct Append;
template<typename T, typename... Args>
struct Append<TypeList<Args...>, T> {
    using Result = TypeList<Args..., T>;
};
template<typename... Args2>
struct Append<EmptyList, TypeList<Args2...>> {
    using Result = TypeList<Args2...>;
};
template<typename... Args1, typename... Args2>
struct Append<TypeList<Args1...>, TypeList<Args2...>> {
    using Result = TypeList<Args1..., Args2...>;
};


template<typename TList, typename T>
struct Erase;
template<typename T>
struct Erase<EmptyList, T> {
    using Result = EmptyList;
};
template<typename T, typename... Tail>
struct Erase<TypeList<T, Tail...>, T> {
    using Result = TypeList<Tail...>;
};
template<typename Head, typename... Tail, typename T>
struct Erase<TypeList<Head, Tail...>, T> {
    using Result = typename Append<TypeList<Head>,
                                   typename Erase<TypeList<Tail...>, T>::Result>::Result;
};

// TL[n:]
template<int n, typename Head, typename... Tail >
struct Skip;
template <typename Head, typename... Tail>
struct Skip<0, TypeList<Head, Tail...> > {
    using value =TypeList<Head, Tail...>;
};
template<int n, typename Head, typename... Tail>
struct Skip<n, TypeList<Head, Tail...>> {
    using value = typename Skip<n-1, TypeList<Tail...>>::value;
};

// TL[:n]
template<int n, typename Head, typename... Tail >
struct Take;
template <typename Head, typename... Tail>
struct Take<0, TypeList<Head, Tail...> > {
    using value = EmptyList;
};
template <typename Head, typename... Tail>
struct Take<1, TypeList<Head, Tail...> > {
    using value = TypeList<Head>;
};
template<int n, typename Head, typename... Tail>
struct Take<n, TypeList<Head, Tail...>> {
    using value = typename Append<TypeList<Head>, typename Take<n-1, TypeList<Tail...>>::value>::Result;
};


using tl = TypeList<int, float, double, char>;
void TestTypeList() {
    assert(typeid(Skip<2, tl>::value) == typeid(TypeList<double, char>));
    assert(typeid(Skip<1, tl>::value) == typeid(TypeList<float, double, char>));
    assert(typeid(Skip<3, tl>::value) == typeid(TypeList<char>));

    assert(typeid(Take<0, tl>::value) == typeid(TypeList<>));
    assert(typeid(Take<1, tl>::value) == typeid(TypeList<int>));
    assert(typeid(Take<2, tl>::value) == typeid(TypeList<int, float>));
    assert(typeid(Take<3, tl>::value) == typeid(TypeList<int, float, double>));
}

}
