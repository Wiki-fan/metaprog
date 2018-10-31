#pragma once
#include "typelists-m.h"


namespace m
{
using namespace m;

template<class TList, template<class, class... Other> class Unit>
class GenScatterHierarchy;

// Специализация класса GenScatterHierarchy:
// преобразование класа Typelist в класс Unit
template<class T1, class T2, template<class, class... Other> class Unit>
class GenScatterHierarchy<TypeList<T1, T2>, Unit>:
public GenScatterHierarchy<T1, Unit>,
public GenScatterHierarchy<T2, Unit>
{
public:
typedef TypeList <T1, T2> TList;
typedef GenScatterHierarchy<T1, Unit> LeftBase;
typedef GenScatterHierarchy<T2, Unit> RightBase;
};

// Передача атомарного типа (не списка типов) классу Unit
template<class AtomicType, template<class, class... > class Unit>
class GenScatterHierarchy : public Unit<AtomicType> {
 public:
    typedef Unit<AtomicType> LeftBase;
};

// С классом NullType не делаем ничего
template<template<class> class Unit>
class GenScatterHierarchy<NullType, Unit> {
};

}
