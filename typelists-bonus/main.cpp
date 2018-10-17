#include <iostream>
#include <cassert>

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


template <typename TList, typename T> struct Erase;
template <typename T, typename... Tail>
struct Erase<TypeList<T, Tail...>, T>
{
    using Result = TypeList<Tail...>;
};
template <typename Head, typename... Tail, typename T >
struct Erase<TypeList<Head, Tail...>, T>
{
    using Result = typename Append<TypeList<Head>,
                                   typename Erase<TypeList<Tail...>, T>::Result>::Result;
};



using CharList = TypeList<char, signed char, unsigned char>;
using NumList = TypeList<int, double>;
using AllList = Append<CharList, NumList>::Result;
using AllWithoutInt = Erase<AllList, int>::Result;

int main() {
    // Задание 1: пофиксить длину EmptyList. Просто специфицируем шаблон.
    static_assert(Length<CharList>::value == 3);
    static_assert(Length<TypeList<int>>::value == 1);
    static_assert(Length<EmptyList>::value == 0);

    // Остальные штуки и тесты.
    static_assert(IndexOf<EmptyList, signed char>::value == -1);
    static_assert(IndexOf<CharList, signed char>::value == 1);
    static_assert(IndexOf<CharList, char>::value == 0);

    TypeAt<CharList, 0>::Result c;
    TypeAt<CharList, 1>::Result sc;

    static_assert(Length<AllList>::value == 5);
    static_assert(IndexOf<AllList, double>::value == 4);

    static_assert( Length<Append<EmptyList, NumList>::Result>::value == 2);
    static_assert( Length<Append<NumList, EmptyList>::Result>::value == 2);
    static_assert( Length<Append<NumList, float>::Result>::value == 3);

    // Задание 2: удаление.
    static_assert(Length<AllWithoutInt>::value == 4);
    static_assert(IndexOf<AllWithoutInt, int>::value == -1);

    static_assert(IndexOf<AllWithoutInt, char>::value == 0);
    static_assert(IndexOf<AllWithoutInt, signed char>::value == 1);
    static_assert(IndexOf<AllWithoutInt, unsigned char>::value == 2);
    static_assert(IndexOf<AllWithoutInt, double>::value == 3);

    static_assert(Length<Erase<AllList, char>::Result>::value == 4);
    static_assert(Length<Erase<AllList, double>::Result>::value == 4);

    return 0;
}
