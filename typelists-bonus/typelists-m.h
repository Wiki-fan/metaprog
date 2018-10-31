#pragma once

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

}
