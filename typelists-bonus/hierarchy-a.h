#pragma once
#include "typelists-a.h"


namespace a
{
template<class TList, template<class> class Unit>
class GenScatterHierarchy;

// Специализация класса GenScatterHierarchy:
// преобразование класа Typelist в класс Unit
template<class T1, class T2, template<class> class Unit>
class GenScatterHierarchy<TypeList<T1, T2>, Unit>
    :
        public GenScatterHierarchy<T1, Unit>,
        public GenScatterHierarchy<T2, Unit> {
 public:
    typedef TypeList<T1, T2> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase;
    typedef GenScatterHierarchy<T2, Unit> RightBase;
};

// Передача атомарного типа (не списка типов) классу Unit
template<class AtomicType,
    template<class> class Unit>
class GenScatterHierarchy :
    public Unit<AtomicType> {
    typedef Unit<AtomicType>
        LeftBase;
};

// С классом NullType не делаем ничего
template<template<class> class Unit>
class GenScatterHierarchy<NullType, Unit> {
};


template
    <
        class TList,
        template<class AtomicType, class Base> class Unit,
        class Root = NullType // Класс NullType описан в главе 2
    >
class GenLinearHierarchy;

template
    <
        class T1,
        class T2,
        template<class, class> class Unit,
        class Root
    >
class GenLinearHierarchy<TypeList<T1, T2>, Unit, Root>
    :
        public Unit<T1, GenLinearHierarchy<T2, Unit, Root>> {
};

template
    <
        class T,
        template<class, class> class Unit,
        class Root
    >
class GenLinearHierarchy<TypeList<T, NullType>, Unit, Root>
    : public Unit<T, Root> {
};

}