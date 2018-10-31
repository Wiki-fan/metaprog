#pragma once
#include "typelists-m.h"


namespace m
{
using namespace m;

template<class TList, template<class, class... Other> class Unit>
class GenScatterHierarchy;

template<class T1, class T2, template<class, class... Other> class Unit>
class GenScatterHierarchy<TypeList < T1, T2>, Unit>
    : public GenScatterHierarchy<T1, Unit>,
      public GenScatterHierarchy<T2, Unit>
{
 public:
    typedef TypeList <T1, T2> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase;
    typedef GenScatterHierarchy<T2, Unit> RightBase;
};

// Передача атомарного типа (не списка типов) классу Unit
template<class AtomicType, template<class, class...> class Unit>
class GenScatterHierarchy
    : public Unit<AtomicType> {
 public:
    typedef Unit<AtomicType> LeftBase;
};

template<template<class> class Unit>
class GenScatterHierarchy<NullType, Unit> {
};


template<class TList, template<class T, class Base, class... Other> class Unit, class Base=NullType>
class GenLinearHierarchy;

template<class T1, class T2, template<class T, class Base, class... Other> class Unit, class Base>
class GenLinearHierarchy<TypeList<T1, T2>, Unit, Base>
    : public Unit<T1, GenLinearHierarchy<T2, Unit, Base>>
{};

template<typename AtomicType, template<class T, class Base, class... Other> class Unit, class Base>
class GenLinearHierarchy<TypeList <AtomicType>, Unit, Base>
    : public Unit<AtomicType, Base>
{};

}
