#pragma once

#include <iostream>
#include <memory>
#include <list>
#include <map>
#include <set>
#include <vector>
#include "BaseC.h"
#include "B.h"
#include "A.h"

template<>
class C<Role::Proxy> : public IB
{
 private:
    IB* b;
 public:
    C(int num_, C<Role::Mediator>* atc_ );

    void Do() override;
};

template<>
class C<Role::Mediator>
{
 public:

    bool OrderLanding(int number);

    bool AcceptLandingRequest();
    bool AcceptLandingReport();
    void addPlane( B* b, int index );

 private:
    bool runwayClear = true;
    std::map<int, B*> planes;
};

/*template <>
class C<Role::Observer> : public B {
 public:
    // A is Observer
    // B is Subject

    std::list<A*> observers;

    void attach(A* a) {
        observers.insert(a);
    }

    void detach(A* a) {
        observers.remove(a);
    }

    void notify(B* b) {
        for (auto& a: observers) {
            a.Update(b);
        }
    }
};*/

// Почему бы не реализовать паттерн Observer вот так, а не класть его внутрь каждого B?
template<>
class C<Role::Observer>
{
 private:
    std::map<B*, std::set<A*>> pairs;
 public:
    // A is Observer
    // B is Subject

    void attachAtoB( A* a, B* b );

    void detachAfromB( A* a, B* b );

    void notify( B* b );
};

