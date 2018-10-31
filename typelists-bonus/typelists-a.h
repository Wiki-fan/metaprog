#pragma once

namespace a
{
class NullType {};

class EmptyType {};

template<class T, class U>
struct TypeList {
    typedef T head;
    typedef U Tail;
};

typedef TypeList<NullType, NullType> EmptyList;

typedef TypeList<signed char,
                 TypeList<short int,
                          TypeList<int, TypeList<long int, NullType>>>>
    SignedIntegrals;

typedef TypeList<char,
                 TypeList<signed char,
                          TypeList<unsigned char, NullType>>>
    CharList;

typedef TypeList<int,
                 TypeList<double, NullType>>
    NumList;

template<class TList>
struct Length;
template<>
struct Length<NullType> {
    enum { value = 0 };
};
template<class T, class U>
struct Length<TypeList<T, U>> {
    enum { value = 1 + Length<U>::value };
};


template<class TList, unsigned int index>
struct TypeAt;// { using Result = TypeAt<TList::head, TList::tail, index>; };

template<class Head, class Tail>
struct TypeAt<TypeList<Head, Tail>, 0> {
    using Result = Head;
};

template<class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i> {
    using Result = typename TypeAt<Tail, i - 1>::Result;
};


template<class TList, class T>
struct IndexOf;
template<class T>
struct IndexOf<NullType, T> {
    enum { value = -1 };
};
template<class T, class Tail>
struct IndexOf<TypeList<T, Tail>, T> {
    enum { value = 0 };
};
template<class Head, class Tail, class T>
struct IndexOf<TypeList<Head, Tail>, T> {
 private:
    enum { temp = IndexOf<Tail, T>::value };
 public:
    enum { value = temp == -1 ? -1 : 1 + temp };
};

template<class TList, class T>
struct Append;

template<>
struct Append<NullType, NullType> {
    using Result = NullType;
};

template<class T>
struct Append<NullType, T> {
    using Result = TypeList<T, NullType>;
};

template<class Head, class Tail>
struct Append<NullType, TypeList<Head, Tail>> {
    using Result = TypeList<Head, Tail>;
};

template<class Head, class Tail, class T>
struct Append<TypeList<Head, Tail>, T> {
    using Result = TypeList<Head,
                            typename Append<Tail, T>::Result>;
};

typedef Append<CharList, NumList>::Result AllList;

template<class TList, class T>
struct Erase;
template<class T>
struct Erase<NullType, T> {
    typedef NullType Result;
};
template<class T, class Tail>
struct Erase<TypeList<T, Tail>, T> {
    typedef Tail Result;
};
template<class Head, class Tail, class T>
struct Erase<TypeList<Head, Tail>, T> {
    typedef TypeList<Head,
                     typename Erase<Tail, T>::Result>
        Result;
};

typedef Erase<AllList, int>::Result AllWithoutInt;

}