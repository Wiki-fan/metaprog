#pragma once
#include "typelists-m.h"


namespace m
{
using namespace m;

template<class TList, template<class, class...> class Unit>
class GenScatterHierarchy;

template<typename T1, typename... T2, template<class, class...> class Unit>
class GenScatterHierarchy<TypeList<T1, T2...>, Unit>
    : public GenScatterHierarchy<T1, Unit>,
      public GenScatterHierarchy<TypeList<T2...>, Unit>
{
 public:
    typedef TypeList <T1, T2...> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase;
    typedef GenScatterHierarchy<TypeList<T2...>, Unit> RightBase;
};

template<typename AtomicType, template<class, class...> class Unit>
class GenScatterHierarchy
    : public Unit<AtomicType> {
 public:
    typedef Unit<AtomicType> LeftBase;
};

template<template<class, class...> class Unit>
class GenScatterHierarchy<EmptyList, Unit> {
};


template<class TList, template<class T, class Parent, class...> class Unit, class Base=NullType>
class GenLinearHierarchy;

template<class T1, class... T2, template<class T, class Parent, class...> class Unit, class Base>
class GenLinearHierarchy<TypeList<T1, T2...>, Unit, Base>
    : public Unit<T1, GenLinearHierarchy<TypeList<T2...>, Unit, Base>>
{
 public:
    using current = T1;
    using next = GenLinearHierarchy<TypeList<T2...>, Unit, Base>;
    using TList = TypeList<T1, T2...>;
    using parent = Unit<T1, GenLinearHierarchy<TypeList<T2...>, Unit, Base>>;
};

template<typename AtomicType, template<class T, class Parent, class...> class Unit, class Base>
class GenLinearHierarchy<TypeList<AtomicType>, Unit, Base>
    : public Unit<AtomicType, Base>
{
public:
    using current = AtomicType;
    using parent = Unit<AtomicType, Base>;
};


template<typename T, template<typename, typename...> typename Unit, typename TL>
Unit<T>& GetSpec(GenScatterHierarchy<TL, Unit>& obj) {
    return static_cast<Unit<T>&>(obj);
}

template<typename T, template<typename, typename, typename...> typename Unit, typename TL, typename Base>
Unit<T, Base>& GetSpecLinear(GenLinearHierarchy<TL, Unit, Base>& obj) {
    return static_cast<Unit<T, Base>&>(obj);
}

}
