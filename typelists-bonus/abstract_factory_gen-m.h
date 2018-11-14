#pragma once
#include <iostream>
#include "hierarchy-m.h"
#include <cassert>


class Infantry {
 public:
    void respond() {
        std::cout << "Infantry" << std::endl;
    }
};

class Archer {
 public:
    void respond() {
        std::cout << "Archer" << std::endl;
    }
};

class Cavalry {
 public:
    void respond() {
        std::cout << "Cavalry" << std::endl;
    }
};

namespace m
{
using namespace m;

template<class T>
class AbstractFactoryUnit {
 public:
    virtual T* DoCreate() const = 0;
    virtual ~AbstractFactoryUnit() = default;
};

template
    <
        class TList,
        template <class> class Unit = AbstractFactoryUnit
    >
class AbstractFactory : public GenScatterHierarchy<TList, Unit>
{
 public:
    typedef TList ProductList;

    template <class T>
    T* Create()
    {
        return (static_cast<Unit<T>*>(this)).DoCreate();
    }
};

template<class T, class Base = NullType>
class OpNewFactoryUnit : public AbstractFactoryUnit<T> {
 public:
    T t;

    T* DoCreate() const override {
        return new T;
    }
};

template
    <
        class AbstractFact,
        template <class, class> class Creator,
        class TList = typename AbstractFact::ProductList
    >
class ConcreteFactory
    : public GenLinearHierarchy<TList, Creator, AbstractFact>
{
 public:
    typedef typename AbstractFact::ProductList ProductList;
    typedef TList ConcreteProductList;
};

using AbstractArmyFactory = AbstractFactory<TypeList<Infantry, Archer, Cavalry>, AbstractFactoryUnit>;

using ArmyFactory = ConcreteFactory<AbstractArmyFactory, OpNewFactoryUnit>;

/*void test_factory() {
    ArmyFactory* factory = new ArmyFactory;

    Infantry* infantry = (static_cast<OpNewFactoryUnit<Infantry>&>(factory)).DoCreate();
    delete infantry;

    infantry = factory->Create<Infantry>();

    assert(typeid(*infantry) == typeid(Infantry));
}*/

}

namespace simple {
using namespace m;

template<class T>
class AbstractFactoryUnit {
 public:
    virtual T* DoCreate() const = 0;
    virtual ~AbstractFactoryUnit() = default;
};

template<class T>
class OpNewFactoryUnit {
 public:
    T t;

    virtual T* DoCreate() const {
        return new T;
    }
};

using AbstractArmyFactory = GenScatterHierarchy
    <TypeList<Infantry, Archer, Cavalry>,
     AbstractFactoryUnit>;

using OpNewArmyFactory = GenScatterHierarchy
    <
        TypeList<Infantry, Archer, Cavalry>,
        OpNewFactoryUnit
    >;

OpNewArmyFactory factory;

class ArmyFactory : public GenScatterHierarchy<TypeList<Infantry, Archer, Cavalry>, OpNewFactoryUnit>
{
 public:
    template<typename T>
    T* Create() {
        return (static_cast<OpNewFactoryUnit<T>&>(factory)).DoCreate();
    }
};

void test_factory() {
    Infantry* infantry = (static_cast<OpNewFactoryUnit <Infantry>&>(factory)).DoCreate();
    delete infantry;

    ArmyFactory armyFactory;
    infantry = armyFactory.Create<Infantry>();

    assert(typeid(*infantry) == typeid(Infantry));
}

}